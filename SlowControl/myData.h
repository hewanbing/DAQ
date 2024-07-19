#ifndef __myData_h__
#define __myData_h__
#pragma once
#ifndef __CLING__
#include <string>
#include <sstream>
#include "stdlib.h"
#include <vector>
#include <TGClient.h>
#include <fstream>
#include <iostream>
using namespace std;
class myData {
 public:
  myData(uint32_t init);
  virtual ~myData();
  uint32_t get(int begin,int end=-1);
  uint32_t set0(int begin,int end=-1);
  uint32_t set1(int begin,int end=-1);
  uint32_t swap();
  float getIEEE754Float();
 private:
  uint32_t value;
  //  ClassDef(myData,0)  
};
#endif /* __CLING__ */
#endif
