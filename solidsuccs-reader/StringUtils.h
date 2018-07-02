#ifndef READER_STRINGUTILS_H
#define READER_STRINGUTILS_H
#include <string.h>

namespace Reader
{
namespace StringUtils
{
int replaceFirst(std::string &str, const std::string before, const std::string after)
{
  READER_TRY;
  str.replace(str.find(before), before.size(), after);
  return 0;
  READER_CATCH;
}

int replaceAll(std::string &str, const std::string before, const std::string after)
{
  READER_TRY;
  while(str.find(before) != std::string::npos)
    str.replace(str.find(before), before.size(), after);
  return 0;
  READER_CATCH;
}

int deleteAfter(std::string &str, const std::string find)
{
  READER_TRY;
  if (str.find(find) != std::string::npos)
    str.resize(str.find(find));
  return 0;
  READER_CATCH;
}

int deleteBefore(std::string &str, const std::string find)
{
  READER_TRY;
  if (str.find(find) != std::string::npos)
    str.erase(0,str.find(find));
  return 0;
  READER_CATCH;
}

bool contains(std::string &str, const std::string find)
{
  if (str.find(find) != std::string::npos) return true;
  else return false;
}

bool beginsWith(std::string &str, const std::string find)
{
  if (str.find(find) == 0) return true;
  else return false;
}
}
}
#endif
