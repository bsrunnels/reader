#ifndef READER_STRINGUTILS_H
#define READER_STRINGUTILS_H
#include <string.h>

using namespace std;

namespace StringUtils
{
int replaceAll(string &str, const string before, const string after)
{
  READER_TRY;
  while(str.find(before) != string::npos)
    str.replace(str.find(before), before.size(), after);
  return 0;
  READER_CATCH;
}

int deleteAfter(string &str, const string find)
{
  READER_TRY;
  if (str.find(find) != string::npos)
    str.resize(str.find(find));
  return 0;
  READER_CATCH;
}

bool contains(string &str, const string find)
{
  if (str.find(find) != string::npos) return true;
  else return false;
}

bool beginsWith(string &str, const string find)
{
  if (str.find(find) == 0) return true;
  else return false;
}

}

#endif
