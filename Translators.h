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
#include <cmath>
#include <complex>

#include "Exception.h"
#include "StringUtils.h"

#ifdef MUPARSER
#include "muParser.h"
#endif

using namespace std;

namespace Reader
{

template<class T> struct Interpreter;

#ifdef MUPARSER
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

// chars

template<> struct Interpreter<vector<char> >
{
  void operator() (const string varUnparsed, vector<char> *varParsed)
  {
    READER_TRY;
    std::istringstream iss(varUnparsed); 
    string token;
    for (unsigned int i=0; iss >> token ;i++)
      {
	if (i<varParsed->size())
	  (*varParsed)[i] = token.at(0);
	else
	  (*varParsed).push_back(token.at(0));
      }
    READER_CATCH_MSG("Error parsing vector<char>: " << varUnparsed);
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
    std::istringstream iss(varUnparsed); 
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
    std::istringstream iss(varUnparsed); 
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

//
// complex vector
//

template<> struct Interpreter<vector<complex<double> > >
{
  void operator() (const string varUnparsed, vector<complex<double> > *varParsed)
  {
    READER_TRY;
    std::istringstream iss(varUnparsed); 
    string token;
    for (unsigned int i=0; iss >> token ;i++)
      {
	string re=token, im=token;
	Reader::StringUtils::deleteAfter(re,"+i");
	Reader::StringUtils::deleteBefore(im,"+i");
	if (i<varParsed->size())
	  (*varParsed)[i] = complex<double>(atof(re.c_str()),atof(im.c_str()));
	else
	  (*varParsed).push_back(complex<double>(atof(re.c_str()),atof(im.c_str())));
      }
    READER_CATCH_MSG("Error parsing vector<double>: " << varUnparsed);
  }
};
}


#endif
