#ifndef __myString_h__
#define __myString_h__
#pragma once
#include <string>
#include <sstream>
#include "stdlib.h"
#include <vector>
#include <TString.h>
#include <fstream>
#include <iostream>
#include "CAENVMEtypes.h"
using namespace std;
class myString {
 public:
  myString();
  myString(const char* text);
  virtual ~myString();
  int getInt();
  long getLong();
  int32_t getInt32_t();
  short getShort();
  uint32_t getUInt32_t();
  char getChar();
  unsigned char getUnSignedChar();
  unsigned short getUnSignedShort();
  TString getContent(){return content;}
  static TString makeBinary(uint32_t value);
  //  int getBufferSize();
  //  static TString getErrorCodeString(CVErrorCodes code);
  static TString getCVString(CVAddressModifier value);
  static TString getCVString(CVArbiterTypes    value);
  static TString getCVString(CVBoardTypes      value);
  static TString getCVString(CVBusReqLevels    value);
  static TString getCVString(CVDataWidth       value);
  static TString getCVString(CVDisplay         value);
  static TString getCVString(CAEN_BYTE         value);
  static TString getCVString(CVErrorCodes      value);
  static TString getCVString(CVInputSelect     value);
  static TString getCVString(CVIOSources       value);
  static TString getCVString(CVIOPolarity      value);
  static TString getCVString(CVIRQLevels       value);
  static TString getCVString(CVLEDPolarity     value);
  static TString getCVString(CVOutputSelect    value);
  static TString getCVString(CVPulserSelect    value);
  static TString getCVString(CVTimeUnits       value);
  static TString getCVString(CVRegisters       value);
  static TString getCVString(CVReleaseTypes    value);
  static TString getCVString(CVRequesterTypes  value);
  static TString getCVString(CVVMETimeouts     value);
  static TString makeString(double value,int precision);
  CVAddressModifier   getCVAddressModifier();
  CVArbiterTypes      getCVArbiterTypes   ();
  CVBoardTypes        getCVBoardTypes     ();
  CVBusReqLevels      getCVBusReqLevels   ();
  CVDataWidth         getCVDataWidth      ();
  CVDisplay           getCVDisplay        ();
  CAEN_BYTE           getCAEN_BYTE        ();
  CVErrorCodes        getCVErrorCodes     ();
  CVInputSelect       getCVInputSelect    ();
  CVIOSources         getCVIOSources      ();
  CVIOPolarity        getCVIOPolarity     ();
  CVIRQLevels         getCVIRQLevels      ();
  CVLEDPolarity       getCVLEDPolarity    ();
  CVOutputSelect      getCVOutputSelect   ();
  CVPulserSelect      getCVPulserSelect   ();
  CVTimeUnits         getCVTimeUnits      ();
  CVRegisters         getCVRegisters      ();
  CVReleaseTypes      getCVReleaseTypes   ();
  CVRequesterTypes    getCVRequesterTypes ();
  CVVMETimeouts       getCVVMETimeouts    ();

 private:
  unsigned long value;
  TString content;
  TString contentLower;
  bool analysis();
};
#endif
