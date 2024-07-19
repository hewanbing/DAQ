#include "MData.h"
using namespace std;

MData::MData(uint32_t init) {
  value=init;
}
MData::~MData() {}
uint32_t MData::get(int begin,int end){
  int tmp=0;
  int a=begin;
  int b=end;
  if(end==-1){
    b=a;
  }
  if(a<b){
    tmp=a;
    a=b;
    b=tmp;
    tmp=0;
  }
  uint32_t tmp2=value;
  tmp2=tmp2/(pow(2,b));
  tmp2=tmp2&(((int)(pow(2,a-b)))*2-1);
  return tmp2;
}
uint32_t MData::set0(int begin,int end){
  int tmp=0;
  int a=begin;
  int b=end;
  if(end==-1){
    b=a;
  }
  if(a<b){
    tmp=a;
    a=b;
    b=tmp;
    tmp=0;
  }
  uint32_t tmp2=value;
  uint32_t tmp3=0xFFFFFFFF;
  tmp3=tmp3>>(31-a+b);
  tmp3=tmp3<<b;
  tmp3=~tmp3;
  tmp2=tmp2&tmp3;
  return tmp2;
}
uint32_t MData::set1(int begin,int end){
  int tmp=0;
  int a=begin;
  int b=end;
  if(end==-1){
    b=a;
  }
  if(a<b){
    tmp=a;
    a=b;
    b=tmp;
    tmp=0;
  }
  uint32_t tmp2=value;
  uint32_t tmp3=0xFFFFFFFF;
  tmp3=tmp3>>(31-a+b);
  tmp3=tmp3<<b;
  tmp2=tmp2|tmp3;
  return tmp2;
}
