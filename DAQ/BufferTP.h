#ifndef __BufferTP_h_
#define __BufferTP_h_
#include <pthread.h>
#include <iostream>
#include <string.h>
#include "stdio.h"
using namespace std;
template<class dataType> class BufferTP{
 public:
  BufferTP(int,int);
  virtual ~BufferTP();
  virtual dataType * getBuffer4Read();
  virtual dataType * getBuffer4Fill(int length);
  int getBufferLength(){return bufferLength;}
  int getBufferNumber(){return bufferNumber;}
  int getBufferCharSize(){return bufferLength*sizeof(dataType)/sizeof(char);}
  void readDone();
  void fillDone();
  void insertText(const char* text);
  int getStatus();
 private:
  int bufferLength;
  int bufferNumber;
  dataType **bufferPtr;
  pthread_mutex_t **bufferMutex;
  int fillIndex=0;
  int fillOffset=0;
  int readIndex=0;
  int _fillIndex=0;
  int _fillOffset=0;
  int _readIndex=0;
  dataType* _ptr4Read=nullptr;
};
template<class dataType> BufferTP <dataType>::BufferTP(int l,int n){
  bufferLength=l;
  bufferNumber=n;
  bufferPtr=new dataType*[bufferNumber];
  bufferMutex=new pthread_mutex_t*[bufferNumber];
  for(int i=0;i<bufferNumber;i++){
    bufferPtr[i]=new dataType[bufferLength];
    bufferMutex[i]=new pthread_mutex_t();
  }
  pthread_mutex_lock(bufferMutex[fillIndex]);  
}
template<class dataType> BufferTP <dataType>::~BufferTP(){
  for(int i=0;i<bufferNumber;i++){
    delete bufferPtr[i];
    delete bufferMutex[i];
  } 
}
template<class dataType>
int BufferTP<dataType>::getStatus(){
    int status=fillIndex-readIndex;
    if(status<0)status+=bufferNumber;
    float value=((float)status)*100/bufferNumber;
    return value;
}
template<class dataType>
void BufferTP<dataType>::insertText(const char *text){
  _fillIndex=(fillIndex+1)%bufferNumber;
  pthread_mutex_lock(bufferMutex[_fillIndex]);
  pthread_mutex_unlock(bufferMutex[fillIndex]);
  fillIndex=_fillIndex;
  char* ptr=(char*)bufferPtr[fillIndex];
  strcpy(ptr,text);
  fillOffset=0;
  _fillOffset=0;
  _fillIndex=(fillIndex+1)%bufferNumber;
  pthread_mutex_lock(bufferMutex[_fillIndex]);
  pthread_mutex_unlock(bufferMutex[fillIndex]);
  fillIndex=_fillIndex;  
}
template<class dataType>void BufferTP<dataType>::readDone(){
  pthread_mutex_lock(bufferMutex[_readIndex]);
  pthread_mutex_unlock(bufferMutex[readIndex]);
  readIndex=_readIndex;
}
template<class dataType>void BufferTP<dataType>::fillDone(){
  fillOffset=_fillOffset;
}
template<class dataType>dataType* BufferTP<dataType>::getBuffer4Fill(int dataSize){
  dataType *ptr=bufferPtr[fillIndex];
  if((fillOffset+dataSize)<=bufferLength){
    _fillOffset=fillOffset+dataSize;
    ptr=ptr+fillOffset;
  }else{
    _fillIndex=(fillIndex+1)%bufferNumber;
    pthread_mutex_lock(bufferMutex[_fillIndex]);
    pthread_mutex_unlock(bufferMutex[fillIndex]);
    fillIndex=_fillIndex;
    ptr=bufferPtr[fillIndex];
    fillOffset=0;
    _fillOffset=dataSize;
  }
  return ptr;
}
template<class dataType>dataType* BufferTP<dataType>::getBuffer4Read(){
  if(!_ptr4Read) pthread_mutex_lock(bufferMutex[readIndex]);
  _ptr4Read=bufferPtr[readIndex];
  _readIndex=(readIndex+1)%bufferNumber;
  return _ptr4Read;
}
#endif
