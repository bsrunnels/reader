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
#include "Color.h"
#include "muParser.h"
#include "StringUtils.h"

using namespace std;

class Reader
{
public:
  //
  // Constructors
  // {{{
  Reader() {variableDelimiter="$";commentDelimiter="#";lineOverflowDelimiter="\\";readFileAndStoreData();}
  Reader(string _filename, string _variableDelimiter="$", string _commentDelimiter="#", string _lineOverflowDelimiter="\\"):
    filename(_filename), 
    argc(0),
    variableDelimiter(_variableDelimiter), 
    commentDelimiter(_commentDelimiter), 
    lineOverflowDelimiter(_lineOverflowDelimiter)
  {
    ifstream test(filename.c_str());
    if(!test)
      READER_NEW_EXCEPTION("File \""<<filename<<"\" not found");
    readFileAndStoreData();
  }
  
  Reader(string _filename, int _argc, char ** _argv, string _variableDelimiter="$", string _commentDelimiter="#", string _lineOverflowDelimiter="\\"):
    filename(_filename), 
    argc(_argc),
    argv(_argv),
    variableDelimiter(_variableDelimiter), 
    commentDelimiter(_commentDelimiter), 
    lineOverflowDelimiter(_lineOverflowDelimiter)
  {
    ifstream test(filename.c_str());
    if(!test)
      READER_NEW_EXCEPTION("File \""<<filename<<"\" not found")
    readFileAndStoreData();
  }
  // }}}

  // 
  // Finders

  bool Find(const string varName)
  {
    string varValue;
    if (argc && searchCommandLineArgsForVariable(varName,varValue,argc,argv))
      return true;
    if (variables.find(varName) != variables.end())
      return true;
    else
      return false;
  }


  // 
  // Variable Readers

  template<class Type>
  void Read(const string varName, Type *varValue)
  {
    READER_TRY;
    Interpreter<Type> interpreter;
    if (variables.find(varName) != variables.end())
      interpreter(variables[varName], varValue);
    else
      READER_NEW_EXCEPTION("Variable \"" << varName << "\" not defined and no default specified");
    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }
  template<class Type>
  Type Read(const string varName)
  {
    READER_TRY;
    Interpreter<Type> interpreter;
    // First attempt: did user specify in the command line?
    string varValue;
    if (argc && searchCommandLineArgsForVariable(varName,varValue,argc,argv))
      {
	Type retValue;
	interpreter(varValue, &retValue);
	return retValue;
      }
    // Next attempt: is it located in the file?
    else if (variables.find(varName) != variables.end())
      {
	Type retValue;
	interpreter(variables[varName],&retValue);
	return retValue;
      }
    // Can't find variable: scream at user
    else
      READER_NEW_EXCEPTION("Variable \""+ varValue + "\" not defined and no default specified");
    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }
  template<class Type>
  Type Read(const string varName, Type defaultValue)
  {
    READER_TRY;
    Interpreter<Type> interpreter;
    // First attempt: did user specify in the command line?
    string varValue;
    if (argc && searchCommandLineArgsForVariable(varName,varValue,argc,argv))
      {
	Type retValue;
	interpreter(varValue, &retValue);
	return retValue;
      }
    // Next attempt: is it located in the file?
    else if (variables.find(varName) != variables.end())
      {
	Type retValue;
	interpreter(variables[varName],&retValue);
	return retValue;
      }
    // All else fails: return default value
    else
      return defaultValue;

    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }


  //
  // Struct Readers

  template<class Type>
  void Read(const string structName, const string varName, Type *varValue)
  {
    READER_TRY;
    Interpreter<Type> interpreter;
    if (structs.find(structName) == structs.end()) 
      READER_NEW_EXCEPTION("Struct \"" << varName << "\" not defined and no default specified");
    if (structs[structName].find(varName) == structs[structName].end())
      READER_NEW_EXCEPTION("Variable \"" << varName << "\" not defined and no default specified");
    interpreter(structs[structName][varName], varValue);
    READER_CATCH_MSG("Error occurred while reading struct \"" << varName << "\"");
  }


private: // Private member functions

  bool searchCommandLineArgsForVariable(string varName, string &varValue, int argc, char **argv)
  {
    READER_TRY;
    for (int i=0; i<argc; i++)
      {
	string line(argv[i]);
	if (line.find("-D"+varName+"=") == 0)
	  {
	    varValue= line.replace(0, varName.size()+3, "");
	    return true;
	  }
      }
    return false;
    READER_CATCH;
  }

  int readFileAndStoreData()
  {
    string line;
    ifstream inputFile(filename.c_str());
    while (getline(inputFile,line))
      {
	// 0. Strip of all comments and remove excess whitespace
	// if (line.find(commentDelimiter) != string::npos)
	//   line.resize(line.find(commentDelimiter));
	StringUtils::deleteAfter(line,commentDelimiter);
	StringUtils::replaceAll(line, "  ", " ");

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

	// 2. Include other files
	string include("include ");
	if (line.find(include) == 0)
	  {
	    string includeFileName = line.replace(line.find(include),include.size(),"");
	    Reader includeReader(includeFileName, variableDelimiter, commentDelimiter,lineOverflowDelimiter);
	    variables.insert(includeReader.variables.begin(),includeReader.variables.end());
	  }

	// 3. Store all structs
	if (StringUtils::contains(line,variableDelimiter) &&
	    StringUtils::contains(line,"{") &&
	    line.find("{") >  line.find(variableDelimiter))
	  {
	    StringUtils::replaceAll(line,"{",""); // Remove opening {
	    StringUtils::replaceAll(line," ",""); // Remove all spaces
	    if (!StringUtils::beginsWith(line, variableDelimiter))
	      READER_NEW_EXCEPTION("Struct not delimited properly");
	    StringUtils::replaceAll(line,variableDelimiter,"");
	    string structLabel = line;

	    map<string, string> structVariables;
	    do
	      {
		if (!getline(inputFile,line))
		  READER_NEW_EXCEPTION("File ended while reading struct");

		// Remove comments
		StringUtils::deleteAfter(line,commentDelimiter); 

		if (!StringUtils::contains(line,variableDelimiter))
		  continue;

		istringstream iss(line); 
		string variableLabel; iss >> variableLabel;
		if (!StringUtils::beginsWith(variableLabel,variableDelimiter))
		  READER_NEW_EXCEPTION("Variable not delimited properly: label= " << variableLabel);
		StringUtils::replaceAll(variableLabel,variableDelimiter,"");
		string variableValue; string token;
		iss >> variableValue;
		while (iss>>token)
		  variableValue += " " + token;
		
		// TODO: replace macros, maybe?

		structVariables.insert(make_pair(variableLabel, variableValue));

	      } while (!StringUtils::contains(line,"}"));
	    
	    structs.insert(make_pair(structLabel,structVariables));
	  }

	// 4. Store all variable declarations in the map
	if (StringUtils::contains(line, variableDelimiter))
	  {
	    istringstream iss(line); 
	    string variableLabel; iss >> variableLabel;
	    if (!StringUtils::beginsWith(variableLabel,variableDelimiter))
	      READER_NEW_EXCEPTION("Variable not delimited properly: label= " << variableLabel);
	    StringUtils::replaceAll(variableLabel,variableDelimiter,"");
	    string variableValue; string token;
	    iss >> variableValue;
	    while (iss>>token)
	      variableValue += " " + token;
	    
	    // 4.1 Replace all macros,
	    map<string,string>::iterator varIterator;
	    for (varIterator = variables.begin(); varIterator != variables.end(); varIterator++)
	      {
		string macroName = variableDelimiter + varIterator->first;
		while(variableValue.find(macroName) != string::npos)
		  variableValue.replace(variableValue.find(macroName), macroName.size(), varIterator->second);
	      }
	    variables.insert(make_pair(variableLabel,variableValue));
	  }

      }
    inputFile.close();
    return 0;
  }

private: // Private variables
  string filename;
  string variableDelimiter;
  string commentDelimiter;
  string lineOverflowDelimiter;
  map<string,string> variables;
  map<string,map<string, string> > structs;
  int argc;
  char **argv;
};

#endif
