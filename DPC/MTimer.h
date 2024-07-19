#ifndef __MTimer_h__
#define __MTimer_h__
#pragma once
#include <string>
#include <sstream>
#include "stdlib.h"
#include <vector>
#include <TGClient.h>
#include <fstream>
#include <iostream>
#include <sys/time.h>
using namespace std;
class MTimer {
 public:
  MTimer();
  virtual ~MTimer();
  static double getSeconds();
  static TString getString(int timeZoon=8);
 private:
  //  ClassDef(MTimer,0)  
};
#endif
