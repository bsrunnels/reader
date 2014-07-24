#include <iostream>
#include <cstring>


#define READER_TEST(CONDITIONAL,DESCRIPTION)				\
  {									\
  if (CONDITIONAL)							\
    {									\
      cout.width(30);							\
      cout << left << DESCRIPTION;					\
      cout << READER_COLOR_B_ON << "[";					\
      cout << READER_COLOR_FG_GREEN << "PASS";				\
      cout << READER_COLOR_FG_DEFAULT;					\
      cout << "]" << READER_COLOR_B_OFF << endl;			\
    }									\
  else									\
    {									\
      cout.width(30);							\
      cout << left << DESCRIPTION;					\
      cout << READER_COLOR_B_ON << "[";					\
      cout << READER_COLOR_FG_RED << "FAIL";				\
      cout << READER_COLOR_FG_DEFAULT;					\
      cout << "]" << READER_COLOR_B_OFF << endl;			\
    }									\
  }


#include "Reader.h"

int main(int argc, char ** argv)
{
  Reader r("test.in",argc,argv);
  string test_cpp_string;  r.Read<string>("test_cpp_string", &test_cpp_string);
  READER_TEST(test_cpp_string == "This is a C++ string", "C++ String");
  char*  test_c_string;    r.Read<char*>("test_c_string",  &test_c_string);
  READER_TEST((strcmp(test_c_string,"This is a C string") == 0), "C String");
  int    test_int;         r.Read<int>("test_int", &test_int);
  READER_TEST(test_int == 8, "Int");
  float  test_float;       r.Read<float>("test_float", &test_float);
  READER_TEST(fabs(test_float - 3.14) < 1E-6, "Float");
  double test_double;       r.Read<double>("test_double", &test_double);
  READER_TEST(fabs(test_double - 3.14159265) < 1E-16, "Double");
  vector<double> test_double_vector; r.Read<vector<double> >("test_double_vector", &test_double_vector);
  READER_TEST(test_double_vector.size() == 3 && 
	      fabs(test_double_vector[0]-0.1 + test_double_vector[1]-1.1 + test_double_vector[2]-2.1) < 1E-16, 
	      "Double vector (multiline)");
  double test_struct_double; r.Read<double>("test_struct", "test_struct_double",  &test_struct_double);
  READER_TEST(fabs(test_struct_double - 2.71828) < 1E-16, "Struct double");
}
