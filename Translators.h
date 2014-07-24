#ifndef READER_TRANSLATORS_H
#define READER_TRANSLATORS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <cstring>

#include "Exception.h"

using namespace std;

template<class T> struct Interpreter;

#ifdef MUPARSER
#include "muParser.h"
double EvaluateMath(const string varUnparsed)
{
  try
    {
      mu::Parser p;
      p.SetExpr(varUnparsed);
      return p.Eval();
    }
  catch(mu::ParserError e)
    {
      READER_NEW_EXCEPTION("MuParser had an error parsing the expression: " << varUnparsed);
    }
}
#else
double EvaluateMath(const string varUnparsed)
{
  READER_TRY;
  return atof(varUnparsed.c_str());
  READER_CATCH;
}
#endif

//
// Strings
// 

template<> struct Interpreter<string>
{
  void operator() (const string varUnparsed, string *varParsed)
  {
    READER_TRY;
    *varParsed = varUnparsed;
    READER_CATCH_MSG("Error parsing string: " << varUnparsed);
  }
};

template<> struct Interpreter<char *>
{
  void operator() (const string varUnparsed, char **varParsed)
  {
    READER_TRY;
    *varParsed = (char*)malloc(varUnparsed.size());
    memcpy(*varParsed, varUnparsed.c_str(), varUnparsed.size()+1);
    READER_CATCH_MSG("Error parsing string: " << varUnparsed);
  }
};

//
// integer
// 

template<> struct Interpreter<int>
{
  void operator() (const string varUnparsed, int *varParsed)
  {
    READER_TRY;
    *varParsed = (int)(EvaluateMath(varUnparsed));
    READER_CATCH_MSG("Error parsing integer: " << varUnparsed);
  }
};

//
// float
// 

template<> struct Interpreter<float>
{
  void operator() (const string varUnparsed, float *varParsed)
  {
    READER_TRY;
    *varParsed = EvaluateMath(varUnparsed);
    READER_CATCH_MSG("Error parsing float:" << varUnparsed);
  }
};

//
// double
// 

template<> struct Interpreter<double>
{
  void operator() (const string varUnparsed, double *varParsed)
  {
    READER_TRY;
    *varParsed = EvaluateMath(varUnparsed);
    READER_CATCH_MSG("Error parsing " << varUnparsed);
  }
};

//
// float vector
// 

template<> struct Interpreter<vector<float> >
{
  void operator() (const string varUnparsed, vector<float> *varParsed)
  {
    READER_TRY;
    istringstream iss(varUnparsed); 
    string token;
    for (unsigned int i=0; iss >> token ;i++)
      {
	if (i<varParsed->size())
	  (*varParsed)[i] = EvaluateMath(token);
	else
	  (*varParsed).push_back(EvaluateMath(token));
      }
    READER_CATCH_MSG("Error parsing vector<float>: " << varUnparsed);
  }
};

//
// double vector
// 

template<> struct Interpreter<vector<double> >
{
  void operator() (const string varUnparsed, vector<double> *varParsed)
  {
    READER_TRY;
    istringstream iss(varUnparsed); 
    string token;
    for (unsigned int i=0; iss >> token ;i++)
      {
	if (i<varParsed->size())
	  (*varParsed)[i] = EvaluateMath(token);
	else
	  (*varParsed).push_back(EvaluateMath(token));
      }
    READER_CATCH_MSG("Error parsing vector<double>: " << varUnparsed);
  }
};



#endif
