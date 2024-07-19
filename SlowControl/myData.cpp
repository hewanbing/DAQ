#include "myData.h"
using namespace std;

myData::myData(uint32_t init) {
  value=init;
}
myData::~myData() {}
uint32_t myData::get(int begin,int end){
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
uint32_t myData::set0(int begin,int end){
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
uint32_t myData::set1(int begin,int end){
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
float myData::getIEEE754Float(){
    int s=get(31);
    int ep=get(23,30)-127;
    int d=get(0,22)+1;
    return pow(-1,s)*pow(2,ep)*d;
}
uint32_t myData::swap(){
    return get(16,31)+get(0,15)*pow(2,16);
}
