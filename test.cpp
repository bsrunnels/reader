#include <iostream>
#include "Reader2.h"

int main()
{
  Reader r;
  // cout << r.Read<string>("name") << endl;
  // cout << r.Read<int>("age") << endl;
  // cout << r.Read<float>("pi") << endl;
  // cout << r.Read<float>("otherpi",3.1820) << endl;
  //vector<float> v; r.Read("multiline",&v);
  vector<float> v = r.Read<vector<float> >("multiline");
  for (unsigned int i=0; i<v.size(); i++) cout << v[i] << endl;
}
