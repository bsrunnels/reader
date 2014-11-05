#ifndef READER_H
#define READER_H
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <stdlib.h>
#include "Translators.h"
#include "Exception.h"
#include "Warning.h"
#include "Color.h"
#include "muParser.h"
#include "StringUtils.h"

using namespace std;

#define READER_DEPRICATED true

namespace Reader
{

enum VariableStatus
  {
    ACTIVE,
    DEPRICATED
  };

class Reader
{
public:
  //
  // Constructors
  // 
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

  Reader(int _argc, char ** _argv, string _variableDelimiter="$", string _commentDelimiter="#", string _lineOverflowDelimiter="\\"):
    argc(_argc),
    argv(_argv),
    variableDelimiter(_variableDelimiter), 
    commentDelimiter(_commentDelimiter), 
    lineOverflowDelimiter(_lineOverflowDelimiter)
  {}


  // 
  // Finders
  //
  bool Find(const string varName, VariableStatus status=ACTIVE)
  {
    UsedVariable(varName);
    string varValue;
    if (argc && searchCommandLineArgsForVariable(varName,varValue,argc,argv))
      {
	if (status==DEPRICATED) READER_WARNING("Input " << varName << " depricated, consider changing");
	return true;
      }
    if (variables.find(varName) != variables.end())
      {
	if (status==DEPRICATED) READER_WARNING("Input " << varName << " depricated, consider changing");
	return true;
      }
    if (structs.find(varName) != structs.end())
      {
	if (status==DEPRICATED) READER_WARNING("Input " << varName << " depricated, consider changing");
	return true;
      }
    else
      return false;
  }


  ///
  /// \fn Find
  /// \brief Determine if a specified variable in a specified structure has been specified.
  ///        If command line arguments have been passed then it checks there as well as the 
  ///        input file.
  ///
  bool Find(const string structName,      ///< Name of the struct
	    const string varName,         ///< Name of the variable in the struct
	    VariableStatus status=ACTIVE) ///< Optional argument to specify variable status
  {
    UsedStruct(structName,varName);
    string varValueString;
    if (argc && searchCommandLineArgsForVariable(structName,varName,varValueString,argc,argv))
      {
	if (status==DEPRICATED) READER_WARNING("Input " << structName << "." << varName << " depricated, consider changing");
	return true;
      }
    else if (structs.find(structName) != structs.end() &&
	     structs[structName].find(varName) != structs[structName].end())
      {
	if (status==DEPRICATED) READER_WARNING("Input " << structName << "." << varName  << " depricated, consider changing");
	return true;
      }
    else
      return false;
  }

  // 
  // Variable Readers

  template<class Type>
  void Read(const string varName, Type *varValue)
  {
    READER_TRY;
    UsedVariable(varName);
    Interpreter<Type> interpreter;
    string varValueString;
    if (argc && searchCommandLineArgsForVariable(varName,varValueString,argc,argv))
      interpreter(varValueString,varValue);
    else if (variables.find(varName) != variables.end())
      interpreter(variables[varName], varValue);
    else
      READER_NEW_EXCEPTION("Variable \"" << varName << "\" not defined and no default specified");
    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }
  template<class Type>
  Type Read(const string varName, VariableStatus status=ACTIVE)
  {
    READER_TRY;
    UsedVariable(varName);
    Interpreter<Type> interpreter;
    // First attempt: did user specify in the command line?
    string varValueString;
    if (argc && searchCommandLineArgsForVariable(varName,varValueString,argc,argv))
      {
	Type varValue;
	interpreter(varValueString, &varValue);
	return varValue;
      }
    // Next attempt: is it located in the file?
    else if (variables.find(varName) != variables.end())
      {
	Type varValue;
	interpreter(variables[varName],&varValue);
	return varValue;
      }
    // Can't find variable: scream at user
    else
      READER_NEW_EXCEPTION("Variable \""+ varName + "\" not defined and no default specified");
    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }
  template<class Type>
  Type Read(const string varName, Type defaultValue, VariableStatus status=ACTIVE)
  {
    READER_TRY;
    UsedVariable(varName);
    Interpreter<Type> interpreter;
    // First attempt: did user specify in the command line?
    string varValueString;
    if (argc && searchCommandLineArgsForVariable(varName,varValueString,argc,argv))
      {
	Type varValue;
	interpreter(varValueString, &varValue);
	if (status==DEPRICATED) READER_WARNING("Input " << varName << " depricated, consider changing");
	return varValue;
      }
    // Next attempt: is it located in the file?
    else if (variables.find(varName) != variables.end())
      {
	Type varValue;
	interpreter(variables[varName],&varValue);
	if (status==DEPRICATED) READER_WARNING("Input " << varName << " depricated, consider changing");
	return varValue;
      }
    // All else fails: return default value
    else
      {
	if (status==DEPRICATED) READER_WARNING("Input " << varName << " depricated, consider changing");
	return defaultValue;
      }
    READER_CATCH_MSG("Error occurred while reading variable \"" << varName << "\"");
  }


  //
  // Struct Readers
  //

  template<class Type>
  void Read(const string structName, const string varName, Type *varValue, VariableStatus status=ACTIVE)
  {
    READER_TRY;
    UsedStruct(structName,varName);
    Interpreter<Type> interpreter;
    string varValueString;
    
    if (argc && searchCommandLineArgsForVariable(structName,varName,varValueString,argc,argv))
      {
	if (status==DEPRICATED) READER_WARNING("Input " << structName << "." << varName << " depricated, consider changing");
	interpreter(varValueString,varValue);
      }
    else if (structs.find(structName) != structs.end() &&
	     structs[structName].find(varName) != structs[structName].end())
      {
	if (status==DEPRICATED) READER_WARNING("Input " << structName << "." << varName << " depricated, consider changing");
	interpreter(structs[structName][varName], varValue);
      }
    else
      READER_NEW_EXCEPTION("Variable \"" << structName << "." << varName << "\" not defined and no default specified");
    
    READER_CATCH_MSG("Error occurred while reading \"" << structName << "." << varName << "\"");
  }

  template<class Type>
  Type Read(const string structName, const string varName)
  {
    READER_TRY;
    UsedStruct(structName,varName);
    Type retValue;
    Read<Type>(structName, varName, &retValue);
    return retValue;
    READER_CATCH;
  }

  template<class Type>
  Type Read(const string structName, const string varName, Type defaultValue, VariableStatus status=ACTIVE)
  {
    READER_TRY;
    UsedStruct(structName,varName);
    Interpreter<Type> interpreter;
    string varValueString;
    if (argc && searchCommandLineArgsForVariable(structName,varName,varValueString,argc,argv))
      {
	Type varValue;
	if (status==DEPRICATED) READER_WARNING("Input " << structName << "." << varName << " depricated, consider changing");
	interpreter(varValueString,&varValue);
	return varValue;
      }
    else if (structs.find(structName) != structs.end() &&
	     structs[structName].find(varName) != structs[structName].end())
      {
	Type varValue;
	if (status==DEPRICATED) READER_WARNING("Input " << structName << "." << varName << " depricated, consider changing");
	interpreter(structs[structName][varName], &varValue);
	return varValue;
      }
    else
      return defaultValue;
    
    READER_CATCH_MSG("Error occurred while reading \"" << structName << "." << varName << "\"");
  }

  void PrintUnusedVariableWarnings()
  {
    for (map<string,string>::iterator pVariables = variables.begin();
	 pVariables != variables.end();
	 pVariables++)
      {
	if (find(usedVariables.begin(), usedVariables.end(), pVariables->first) == usedVariables.end())
	  READER_WARNING("Variable " << READER_COLOR_FG_BLUE << pVariables->first << READER_COLOR_RESET << " specified but not used");
      }

    for (map<string,map<string,string> >::iterator pStructs = structs.begin();
	 pStructs != structs.end();
	 pStructs++)
      {
	for (map<string,string>::iterator pStructVariables = pStructs->second.begin();
	     pStructVariables != pStructs->second.end();
	     pStructVariables++)
	  {
	    
	    if (find(usedStructs[pStructs->first].begin(), usedStructs[pStructs->first].end(), pStructVariables->first) == usedStructs[pStructs->first].end())
	      READER_WARNING("Variable " 
			     << READER_COLOR_FG_BLUE << pStructs->first 
			     << READER_COLOR_RESET << "." 
			     << READER_COLOR_FG_BLUE << pStructVariables->first 
			     << READER_COLOR_RESET << " specified but not used");
	  }
      }
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
	else if (line.find("-D"+varName+" ") == 0)
	  {
	    varValue="";
	    return true;
	  }
      }
    return false;
    READER_CATCH;
  }

  bool searchCommandLineArgsForVariable(string structName, string varName, string &varValue, int argc, char **argv)
  {
    READER_TRY;
    for (int i=0; i<argc; i++)
      {
	string line(argv[i]);
	if (line.find("-D"+structName+"."+varName+"=") == 0)
	  {
	    varValue= line.replace(0, structName.size() + varName.size()+4, "");
	    return true;
	  }
	else if (line.find("-D"+structName+"."+varName+" ") == 0)
	  {
	    varValue="";
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

		// Get rid of all the comments and whitespace
		StringUtils::deleteAfter(line,commentDelimiter);
		StringUtils::replaceAll(line, "  ", " ");

		// Include any additional files
		if (line.find(include) != string::npos)
		  {
		    StringUtils::replaceFirst(line,"include","");
		    StringUtils::replaceAll(line," ","");
		    Reader includeReader(line, variableDelimiter, commentDelimiter,lineOverflowDelimiter);
		    structVariables.insert(includeReader.variables.begin(),includeReader.variables.end());
		    continue;
		  }

		// Does this line even contain a variable?
		if (!StringUtils::contains(line,variableDelimiter))
		  continue;

		// File away the variable Label and Value into strings
		std::istringstream iss(line); 
		string variableLabel; iss >> variableLabel;
		if (!StringUtils::beginsWith(variableLabel,variableDelimiter))
		  READER_NEW_EXCEPTION("Variable not delimited properly: label= " << variableLabel);
		StringUtils::replaceAll(variableLabel,variableDelimiter,"");
		string variableValue; string token;
		iss >> variableValue;
		while (iss>>token)
		  variableValue += " " + token;

		// Replace Macros
		map<string,string>::iterator varIterator;
		for (varIterator = variables.begin(); varIterator != variables.end(); varIterator++)
		  {
		    string macroName = variableDelimiter + varIterator->first;
		    if (variableValue.find(macroName) != string::npos)
		      UsedVariable(varIterator->first);
		    while(variableValue.find(macroName) != string::npos)
		      variableValue.replace(variableValue.find(macroName), macroName.size(), varIterator->second);
		  }

		structVariables.insert(make_pair(variableLabel, variableValue));

	      } while (!StringUtils::contains(line,"}"));
	    
	    structs.insert(make_pair(structLabel,structVariables));
	  }

	// 4. Store all variable declarations in the map
	if (StringUtils::contains(line, variableDelimiter))
	  {
	    std::istringstream iss(line); 
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
		if (variableValue.find(macroName) != string::npos)
		  UsedVariable(varIterator->first);
		while(variableValue.find(macroName) != string::npos)
		  variableValue.replace(variableValue.find(macroName), macroName.size(), varIterator->second);
	      }
	    variables.insert(make_pair(variableLabel,variableValue));
	  }

      }
    inputFile.close();
    return 0;
  }

  void UsedVariable(string varName)
  {
    usedVariables.push_back(varName);
  }
  void UsedStruct(string structName, string varName)
  {
    usedStructs[structName].push_back(varName);
  }


private: // Private variables
  string filename;
  string variableDelimiter;
  string commentDelimiter;
  string lineOverflowDelimiter;
  map<string,string> variables;
  map<string,map<string, string> > structs;
  vector<string> usedVariables;
  map<string,vector<string> > usedStructs;
  int argc;
  char **argv;
};
}
#endif
