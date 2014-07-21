#include <iostream>
#include "Reader.h"

int main(int argc, char ** argv)
{
  Reader r("test.in");
  // cout << r.Read<string>("name") << endl;
  // cout << r.Read<int>("age") << endl;
  // cout << r.Read<float>("pi") << endl;
  // cout << r.Read<float>("otherpi",3.1820) << endl;
  //vector<float> v; r.Read("multiline",&v);
  // vector<float> v = r.Read<vector<float> >("multiline");
  // for (unsigned int i=0; i<v.size(); i++) cout << v[i] << endl;
  cout << r.Read<double>("check") << endl;
  cout << r.Read<double>("testincludefloat") << endl;
  cout << r.Read<double>("cmd",8.8888888,argc,argv) << endl;
}
