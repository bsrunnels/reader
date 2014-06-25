#ifndef READER_H
#define READER_H
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include "Translators.h"
#include "Exception.h"
#include "muParser.h"

using namespace std;

class Reader
{
public:
  Reader() {variableDelimiter="$";commentDelimiter="#";lineOverflowDelimiter="\\";readFileAndStoreData();}
  Reader(string _filename, string _variableDelimiter="$", string _commentDelimiter="#", string _lineOverflowDelimiter="\\"):
    filename(_filename), 
    variableDelimiter(_variableDelimiter), 
    commentDelimiter(_commentDelimiter), 
    lineOverflowDelimiter(_lineOverflowDelimiter)
  {
    ifstream test(filename.c_str());
    if(!test)
      READER_NEW_EXCEPTION("File \""<<filename<<"\" not found")
    readFileAndStoreData();
  }

  bool Find(const string varName)
  {
    if (variables.find(varName) == variables.end())
      return false;
    else
      return true;
  }

  template<class Type>
  void Read(const string varName, Type *varValue, Interpreter<Type> interpreter = Interpreter<Type>())
  {
    READER_TRY;
    if (variables.find(varName) == variables.end())
      READER_NEW_EXCEPTION("Variable \"" << varName << "\" not defined and no default specified");
    interpreter(variables[varName], varValue);
    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }
  
  template<class Type>
  Type Read(const string varName, Interpreter<Type> interpreter = Interpreter<Type>())
  {
    READER_TRY;
    if (variables.find(varName) == variables.end())
      READER_NEW_EXCEPTION("Variable \"" << varName << "\" not defined and no default specified");
    Type retValue;
    interpreter(variables[varName],&retValue);
    return retValue;
    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }

  template<class Type>
  Type Read(const string varName, Type defaultValue, Interpreter<Type> interpreter = Interpreter<Type>())
  {
    READER_TRY;
    if (variables.find(varName) == variables.end())
      return defaultValue;
    else
      {
	Type retValue;
	interpreter(variables[varName],&retValue);
	return retValue;
      }
    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }

private: // Private member functions
  int readFileAndStoreData()
  {
    string line;
    ifstream inputFile(filename.c_str());
    while (getline(inputFile,line))
      {
	// 0. Strip of all comments
	if (line.find(commentDelimiter) != string::npos)
	  line.resize(line.find(commentDelimiter));

	// 1. Mash multilines togethern
	if (line.find(lineOverflowDelimiter) != string::npos)
	  {
	    string anotherLine;
	    do 
	      {
		line.resize(line.find(lineOverflowDelimiter));
		getline(inputFile,anotherLine);
		line += anotherLine;
	      }
	    while (line.find(lineOverflowDelimiter)!=string::npos);
	  }

	// 2. Store all variable declarations in the map
	if (line.find(variableDelimiter) != string::npos)
	  {
	    istringstream iss(line); 
	    string variableLabel; iss >> variableLabel;
	    while(variableLabel.find(variableDelimiter) != string::npos)
	      variableLabel.replace(variableLabel.find(variableDelimiter), variableDelimiter.size(), "");
	    string variableValue; string token;
	    iss >> variableValue;
	    while (iss>>token)
	      variableValue += " " + token;
	    
	    // 2.1 Replace all macros,
	    map<string,string>::iterator varIterator;
	    for (varIterator = variables.begin(); varIterator != variables.end(); varIterator++)
	      {
		string macroName = variableDelimiter + varIterator->first;
		while(variableValue.find(macroName) != string::npos)
		  variableValue.replace(variableValue.find(macroName), macroName.size(), varIterator->second);
	      }
	    variables.insert(make_pair(variableLabel,variableValue));
	  }
	// TODO: 3. Evaluate mathematical expressions
      }
    inputFile.close();
  }

private: // Private variables
  string filename;
  string variableDelimiter;
  string commentDelimiter;
  string lineOverflowDelimiter;
  map<string,string> variables;
  
};

#endif
