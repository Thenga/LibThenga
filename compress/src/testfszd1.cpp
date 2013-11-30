/*
 * testfzd1.cpp
 */
#include <fszdiff.h>
#include <iostream>

using namespace std;

int main()
{
  struct a
  {
    int64_t a1;
    int64_t a2;
    int64_t a3;
    int64_t a4;
    a()
    {
      memset(this,0,sizeof(a));
    }
  };
  a obj1;
  obj1.a1 = 1000000000;
  obj1.a2 = 0;
  obj1.a3 = 0;
  a obj2;
  obj2.a1 = 1000000000;
  obj2.a2 = 1234567;
  obj2.a3 = 10000000005;


  thenga::FSzDiff<a> fzd;
  cout<< fzd.testGetHeaderByteCount() << " bytes " << endl;

  a obj3;
  a obj4;
  auto diffTup = fzd.getDiff(nullptr, obj1);
  fzd.testPrintBitHeader();
  fzd.getObject(nullptr, std::get<0>(diffTup),obj3);

  thenga::FSzDiff<a> fzd2;
  auto diffTup1 = fzd2.getDiff(&obj1, obj2);
  fzd2.testPrintBitHeader();
  fzd2.getObject(&obj1, std::get<0>(diffTup1),obj4);



  cout << "obj3:" << obj3.a1 << " "<< obj3.a2 << " " << obj3.a3 << "\n";
  cout << "obj4:" << obj4.a1 << " "<< obj4.a2 << " " << obj4.a3 << "\n";
  cout << sizeof(a) << " " << std::get<1>(diffTup1) << endl;

  return 0;
}



