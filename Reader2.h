#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include "Translators.h"

using namespace std;


class Reader
{
public:
  Reader() {variableDelimiter="$";commentDelimiter="#";lineOverflowDelimiter="\\";readFileAndStoreData();}

  template<class Type>
  void Read(const string varName, Type *varValue, Interpreter<Type> interpreter = Interpreter<Type>())
  {
    if (variables.find(varName) == variables.end())
      throw (std::runtime_error("Variable " + varName + " not defined and no default specified"));
    //Interpreter<Type> interpreter;
    interpreter(variables[varName], varValue);
  }
  
  template<class Type>
  Type Read(const string varName, Interpreter<Type> interpreter = Interpreter<Type>())
  {
    if (variables.find(varName) == variables.end())
      throw (std::runtime_error("Variable " + varName + " not defined and no default specified"));
    Type retValue;
    interpreter(variables[varName],&retValue);
    return retValue;
  }

  template<class Type>
  Type Read(const string varName, Type defaultValue, Interpreter<Type> interpreter = Interpreter<Type>())
  {
    if (variables.find(varName) == variables.end())
      return defaultValue;
    else
      {
	Type retValue;
	interpreter(variables[varName],&retValue);
	return retValue;
      }
  }

private: // Private member functions
  int readFileAndStoreData()
  {
    string line;
    ifstream inputFile("test.in");
    while (getline(inputFile,line))
      {
	// 0. Strip of all comments
	if (line.find(commentDelimiter) != string::npos)
	  line.resize(line.find(commentDelimiter));

	// 1. Mash multilines together
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
	if (line.find("=") != string::npos)
	  {
	    istringstream iss(line); 
	    string variableLabel; iss >> variableLabel;
	    string equalSign; iss >> equalSign;
	    if (equalSign.find("=") == string::npos)
	      throw(std::runtime_error("Input file error: equals sign was not the second token"));
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
  string variableDelimiter;
  string commentDelimiter;
  string lineOverflowDelimiter;
  map<string,string> variables;
};
