#ifndef __FEEIRQ_h__
#define __FEEIRQ_h__
typedef struct{
  int level;
  int ID;
  short AM;
  int dataSize;
  long dataAddress;
}FEEIRQ;
#endif
