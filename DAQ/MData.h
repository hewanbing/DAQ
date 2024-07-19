#ifndef __MData_h__
#define __MData_h__
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
class MData {
 public:
  MData(uint32_t init);
  virtual ~MData();
  static MData parse(uint32_t data){MData mData(data);return mData;}
  uint32_t get(int begin,int end=-1);
  uint32_t set0(int begin,int end=-1);
  uint32_t set1(int begin,int end=-1);
 private:
  uint32_t value;
};
#endif /* __CLING__ */
#endif
