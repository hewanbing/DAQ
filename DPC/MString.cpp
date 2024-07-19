#include "MString.h"
using namespace std;

MString::MString() {
}
MString::MString(const char* text) {
  content=text;
  contentLower=text;
  contentLower.ToLower();
  analysis();
}
MString::~MString() {}
TString MString::makeString(double value,int precision){
  int high=(int)value;
  int low=(int)((value-(double)high)*pow(10,precision));
  TString st=TString::LLtoa(high,10);
  st+=".";
  st+=TString::LLtoa(low,10);
  return st;
}
bool MString::analysis(){
  if(content.Length()==0){
    //cout<<"No string"<<endl;
    return false;
  }
  value=0;
  TString sub=contentLower(0,2);
  if(sub.EqualTo("0b")){
    for(int i=2;i<content.Length();i++){
      sub=contentLower(i,1);
      if(sub.EqualTo(" "))continue;
      if(!sub.IsBin()){
	//cout<<"Bad bit"<<endl;
	return false;
      }
      int bit=sub.Atoi();
      value=value*2+bit;
    }
    return true;
  }else if(sub.EqualTo("0o")){
    for(int i=2;i<content.Length();i++){
      sub=contentLower(i,1);
      if(sub.EqualTo(" "))continue;
      if(!sub.IsOct()){
	//cout<<"Bad bit"<<endl;
	return false;
      }
      int bit=sub.Atoi();
      value=value*8+bit;
    }
    return true;
  }else if(sub.EqualTo("0d")){
    for(int i=2;i<content.Length();i++){
      sub=contentLower(i,1);
      if(sub.EqualTo(" "))continue;
      if(!sub.IsDec()){
	//cout<<"Bad bit"<<endl;
	return false;
      }
      int bit=sub.Atoi();
      value=value*10+bit;
    }
    return true;
  }else if(sub.EqualTo("0x")){
    for(int i=2;i<content.Length();i++){
      sub=contentLower(i,1);
      if(sub.EqualTo(" "))continue;
      if(!sub.IsHex()){
	//cout<<"Bad bit"<<endl;
	return false;
      }
      int bit=sub.Atoi();
      if(sub.EqualTo("a"))bit=10;
      if(sub.EqualTo("b"))bit=11;
      if(sub.EqualTo("c"))bit=12;
      if(sub.EqualTo("d"))bit=13;
      if(sub.EqualTo("e"))bit=14;
      if(sub.EqualTo("f"))bit=15;
      value=value*16+bit;
    }
    return true;
  }else if(sub.EqualTo("cv")){
    value=-1;
    return true;
  }else{
    stringstream line;
    line.clear();
    line.str(string(content));
    line>>value;
    return true;
  }
  value=-1;
  cout<<"MString ERROR"<<endl;
  return false;
}
//int MString::getBufferSize(){
//  return content.Atoi();
//}
unsigned short MString::getUnSignedShort(){
  int length=sizeof(unsigned short)*8;
  ULong_t tmp=value>>length;
  tmp=tmp<<length;
  value=value-tmp;
  return value;
}
int MString::getInt(){
  int length=sizeof(int)*8-1;
  ULong_t tmp=value>>length;
  tmp=tmp<<length;
  value=value-tmp;
  return value;
}
long MString::getLong(){
  int length=sizeof(long)*8-1;
  ULong_t tmp=value>>length;
  tmp=tmp<<length;
  value=value-tmp;
  return value;
}
int32_t MString::getInt32_t(){
  int length=sizeof(int32_t)*8-1;
  ULong_t tmp=value>>length;
  tmp=tmp<<length;
  value=value-tmp;
  return value;
}
short MString::getShort(){
  int length=sizeof(short)*8-1;
  ULong_t tmp=value>>length;
  tmp=tmp<<length;
  value=value-tmp;
  return value;
}
uint32_t MString::getUInt32_t(){
  int length=sizeof(uint32_t)*8;
  ULong_t tmp=value>>length;
  tmp=tmp<<length;
  value=value-tmp;
  return value;
}
char MString::getChar(){
  int length=sizeof(char)*8-1;
  ULong_t tmp=value>>length;
  tmp=tmp<<length;
  value=value-tmp;
  return value;
}
unsigned char MString::getUnSignedChar(){
  int length=sizeof(unsigned char)*8;
  ULong_t tmp=value>>length;
  tmp=tmp<<length;
  value=value-tmp;
  return value;
}
CVAddressModifier   MString::getCVAddressModifier(){
  if(content.EqualTo("cvA16_S"        ))return cvA16_S        ;
  if(content.EqualTo("cvA16_U"        ))return cvA16_U        ;
  if(content.EqualTo("cvA16_LCK"      ))return cvA16_LCK      ;

  if(content.EqualTo("cvA24_S_BLT"    ))return cvA24_S_BLT    ;
  if(content.EqualTo("cvA24_S_PGM"    ))return cvA24_S_PGM    ;
  if(content.EqualTo("cvA24_S_DATA"   ))return cvA24_S_DATA   ;
  if(content.EqualTo("cvA24_S_MBLT"   ))return cvA24_S_MBLT   ;
  if(content.EqualTo("cvA24_U_BLT"    ))return cvA24_U_BLT    ;
  if(content.EqualTo("cvA24_U_PGM"    ))return cvA24_U_PGM    ;
  if(content.EqualTo("cvA24_U_DATA"   ))return cvA24_U_DATA   ;
  if(content.EqualTo("cvA24_U_MBLT"   ))return cvA24_U_MBLT   ;
  if(content.EqualTo("cvA24_LCK"      ))return cvA24_LCK      ;

  if(content.EqualTo("cvA32_S_BLT"    ))return cvA32_S_BLT    ;
  if(content.EqualTo("cvA32_S_PGM"    ))return cvA32_S_PGM    ;
  if(content.EqualTo("cvA32_S_DATA"   ))return cvA32_S_DATA   ;
  if(content.EqualTo("cvA32_S_MBLT"   ))return cvA32_S_MBLT   ;
  if(content.EqualTo("cvA32_U_BLT"    ))return cvA32_U_BLT    ;
  if(content.EqualTo("cvA32_U_PGM"    ))return cvA32_U_PGM    ;
  if(content.EqualTo("cvA32_U_DATA"   ))return cvA32_U_DATA   ;
  if(content.EqualTo("cvA32_U_MBLT"   ))return cvA32_U_MBLT   ;
  if(content.EqualTo("cvA32_LCK"      ))return cvA32_LCK      ;

  if(content.EqualTo("cvCR_CSR"       ))return cvCR_CSR       ;

  if(content.EqualTo("cvA40_BLT"      ))return cvA40_BLT      ;
  if(content.EqualTo("cvA40_LCK"      ))return cvA40_LCK      ;
  if(content.EqualTo("cvA40"          ))return cvA40          ;

  if(content.EqualTo("cvA64"          ))return cvA64          ;
  if(content.EqualTo("cvA64_BLT"      ))return cvA64_BLT      ;
  if(content.EqualTo("cvA64_MBLT"     ))return cvA64_MBLT     ;
  if(content.EqualTo("cvA64_LCK"      ))return cvA64_LCK      ;

  if(content.EqualTo("cvA3U_2eVME"    ))return cvA3U_2eVME    ;
  return cvA6U_2eVME    ;
}
CVArbiterTypes      MString::getCVArbiterTypes  (){
  if(content.EqualTo("cvPriorized" ))return cvPriorized ;
  return cvRoundRobin;
}
CVBoardTypes        MString::getCVBoardTypes    (){
  if(content.EqualTo("cvV1718"))return cvV1718;
  if(content.EqualTo("cvV2718"))return cvV2718;
  if(content.EqualTo("cvA2818"))return cvA2818;
  if(content.EqualTo("cvA2719"))return cvA2719;
  return cvA3818;
}
CVBusReqLevels      MString::getCVBusReqLevels  (){
  if(content.EqualTo("cvBR0"))return cvBR0;
  if(content.EqualTo("cvBR1"))return cvBR1;
  if(content.EqualTo("cvBR2"))return cvBR2;
  return cvBR3;
}
CVDataWidth         MString::getCVDataWidth     (){
  if(content.EqualTo("cvD8"         ))return cvD8         ;
  if(content.EqualTo("cvD16"        ))return cvD16        ;
  if(content.EqualTo("cvD32"        ))return cvD32        ;
  if(content.EqualTo("cvD64"        ))return cvD64        ;
  if(content.EqualTo("cvD16_swapped"))return cvD16_swapped;
  if(content.EqualTo("cvD32_swapped"))return cvD32_swapped;
  return cvD64_swapped;
}
CVDisplay           MString::getCVDisplay       (){
  CVDisplay display;
  return display;
}
CAEN_BYTE           MString::getCAEN_BYTE       (){
  CAEN_BYTE b=getUnSignedChar();
  return b;
}
CVErrorCodes        MString::getCVErrorCodes    (){
  if(content.EqualTo("cvSuccess"     ))return cvSuccess     ;
  if(content.EqualTo("cvBusError"    ))return cvBusError    ;
  if(content.EqualTo("cvCommError"   ))return cvCommError   ;
  if(content.EqualTo("cvGenericError"))return cvGenericError;
  if(content.EqualTo("cvInvalidParam"))return cvInvalidParam;
  return cvTimeoutError;
}
CVInputSelect       MString::getCVInputSelect   (){
  if(content.EqualTo("cvInput0"))return cvInput0;
  return cvInput1;
}
CVIOSources         MString::getCVIOSources     (){
  if(content.EqualTo("cvManualSW"   ))return cvManualSW   ;
  if(content.EqualTo("cvInputSrc0"  ))return cvInputSrc0  ;
  if(content.EqualTo("cvInputSrc1"  ))return cvInputSrc1  ;
  if(content.EqualTo("cvCoincidence"))return cvCoincidence;
  if(content.EqualTo("cvVMESignals" ))return cvVMESignals ;
  return cvMiscSignals;
}
CVIOPolarity        MString::getCVIOPolarity    (){
  if(content.EqualTo("cvDirect"  ))return cvDirect  ;
  return cvInverted;
}
CVIRQLevels         MString::getCVIRQLevels     (){
  if(content.EqualTo("cvIRQ1"))return cvIRQ1;
  if(content.EqualTo("cvIRQ2"))return cvIRQ2;
  if(content.EqualTo("cvIRQ3"))return cvIRQ3;
  if(content.EqualTo("cvIRQ4"))return cvIRQ4;
  if(content.EqualTo("cvIRQ5"))return cvIRQ5;
  if(content.EqualTo("cvIRQ6"))return cvIRQ6;
  return cvIRQ7;
}
CVLEDPolarity       MString::getCVLEDPolarity   (){
  if(content.EqualTo("cvActiveHigh"))return cvActiveHigh;
  return cvActiveLow ;
}
CVOutputSelect      MString::getCVOutputSelect  (){
  if(content.EqualTo("cvOutput0"))return cvOutput0;
  if(content.EqualTo("cvOutput1"))return cvOutput1;
  if(content.EqualTo("cvOutput2"))return cvOutput2;
  if(content.EqualTo("cvOutput3"))return cvOutput3;
  return cvOutput4;
}
CVPulserSelect      MString::getCVPulserSelect  (){
  if(content.EqualTo("cvPulserA"))return cvPulserA;
  return cvPulserB;
}
CVTimeUnits         MString::getCVTimeUnits     (){
  if(content.EqualTo("cvUnit25ns"  ))return cvUnit25ns  ;
  if(content.EqualTo("cvUnit1600ns"))return cvUnit1600ns;
  if(content.EqualTo("cvUnit410us" ))return cvUnit410us ;
  return cvUnit104ms ;
}
CVRegisters         MString::getCVRegisters     (){
  if(content.EqualTo("cvStatusReg"      ))return cvStatusReg      ;
  if(content.EqualTo("cvVMEControlReg"  ))return cvVMEControlReg  ;
  if(content.EqualTo("cvFwRelReg"       ))return cvFwRelReg       ;
  if(content.EqualTo("cvFwDldReg"       ))return cvFwDldReg       ;
  if(content.EqualTo("cvFlenaReg"       ))return cvFlenaReg       ;
  if(content.EqualTo("cvVMEIRQEnaReg"   ))return cvVMEIRQEnaReg   ;
  if(content.EqualTo("cvInputReg"       ))return cvInputReg       ;
  if(content.EqualTo("cvOutRegSet"      ))return cvOutRegSet      ;
  if(content.EqualTo("cvInMuxRegSet"    ))return cvInMuxRegSet    ;
  if(content.EqualTo("cvOutMuxRegSet"   ))return cvOutMuxRegSet   ;
  if(content.EqualTo("cvLedPolRegSet"   ))return cvLedPolRegSet   ;
  if(content.EqualTo("cvOutRegClear"    ))return cvOutRegClear    ;
  if(content.EqualTo("cvInMuxRegClear"  ))return cvInMuxRegClear  ;
  if(content.EqualTo("cvOutMuxRegClear" ))return cvOutMuxRegClear ;
  if(content.EqualTo("cvLedPolRegClear" ))return cvLedPolRegClear ;
  if(content.EqualTo("cvPulserA0"       ))return cvPulserA0       ;
  if(content.EqualTo("cvPulserA1"       ))return cvPulserA1       ;
  if(content.EqualTo("cvPulserB0"       ))return cvPulserB0       ;
  if(content.EqualTo("cvPulserB1"       ))return cvPulserB1       ;
  if(content.EqualTo("cvScaler0"        ))return cvScaler0        ;
  if(content.EqualTo("cvScaler1"        ))return cvScaler1        ;
  if(content.EqualTo("cvDispADL"        ))return cvDispADL        ;
  if(content.EqualTo("cvDispADH"        ))return cvDispADH        ;
  if(content.EqualTo("cvDispDTL"        ))return cvDispDTL        ;
  if(content.EqualTo("cvDispDTH"        ))return cvDispDTH        ;
  if(content.EqualTo("cvDispC1"         ))return cvDispC1         ;
  if(content.EqualTo("cvDispC2"         ))return cvDispC2         ;
  if(content.EqualTo("cvLMADL"          ))return cvLMADL          ;
  if(content.EqualTo("cvLMADH"          ))return cvLMADH          ;
  return cvLMC            ;
}
CVReleaseTypes      MString::getCVReleaseTypes  (){
  if(content.EqualTo("cvRWD"))return cvRWD;
  return cvROR;
}
CVRequesterTypes    MString::getCVRequesterTypes(){
  if(content.EqualTo("cvFair"  ))return cvFair  ;
  return cvDemand;
}
CVVMETimeouts       MString::getCVVMETimeouts   (){
  if(content.EqualTo("cvTimeout50us" ))return cvTimeout50us ;
  return cvTimeout400us;
}

TString MString::getCVString(CVAddressModifier value){
  if(value==cvA16_S        )return "cvA16_S"        ;
  if(value==cvA16_U        )return "cvA16_U"        ;
  if(value==cvA16_LCK      )return "cvA16_LCK"      ;

  if(value==cvA24_S_BLT    )return "cvA24_S_BLT"    ;
  if(value==cvA24_S_PGM    )return "cvA24_S_PGM"    ;
  if(value==cvA24_S_DATA   )return "cvA24_S_DATA"   ;
  if(value==cvA24_S_MBLT   )return "cvA24_S_MBLT"   ;
  if(value==cvA24_U_BLT    )return "cvA24_U_BLT"    ;
  if(value==cvA24_U_PGM    )return "cvA24_U_PGM"    ;
  if(value==cvA24_U_DATA   )return "cvA24_U_DATA"   ;
  if(value==cvA24_U_MBLT   )return "cvA24_U_MBLT"   ;
  if(value==cvA24_LCK      )return "cvA24_LCK"      ;

  if(value==cvA32_S_BLT    )return "cvA32_S_BLT"    ;
  if(value==cvA32_S_PGM    )return "cvA32_S_PGM"    ;
  if(value==cvA32_S_DATA   )return "cvA32_S_DATA"   ;
  if(value==cvA32_S_MBLT   )return "cvA32_S_MBLT"   ;
  if(value==cvA32_U_BLT    )return "cvA32_U_BLT"    ;
  if(value==cvA32_U_PGM    )return "cvA32_U_PGM"    ;
  if(value==cvA32_U_DATA   )return "cvA32_U_DATA"   ;
  if(value==cvA32_U_MBLT   )return "cvA32_U_MBLT"   ;
  if(value==cvA32_LCK      )return "cvA32_LCK"      ;

  if(value==cvCR_CSR       )return "cvCR_CSR"       ;

  if(value==cvA40_BLT      )return "cvA40_BLT"      ;
  if(value==cvA40_LCK      )return "cvA40_LCK"      ;
  if(value==cvA40          )return "cvA40"          ;

  if(value==cvA64          )return "cvA64"          ;
  if(value==cvA64_BLT      )return "cvA64_BLT"      ;
  if(value==cvA64_MBLT     )return "cvA64_MBLT"     ;
  if(value==cvA64_LCK      )return "cvA64_LCK"      ;

  if(value==cvA3U_2eVME    )return "cvA3U_2eVME"    ;
  return "cvA6U_2eVME"    ;
}
TString MString::getCVString(CVArbiterTypes value){
  if(value==cvPriorized )return "cvPriorized" ;
  return "cvRoundRobin";
}
TString MString::getCVString(CVBoardTypes value){
  if(value==cvV1718)return "cvV1718";
  if(value==cvV2718)return "cvV2718";
  if(value==cvA2818)return "cvA2818";
  if(value==cvA2719)return "cvA2719";
  return "cvA3818";
}
TString MString::getCVString(CVBusReqLevels  value){
  if(value==cvBR0)return "cvBR0";
  if(value==cvBR1)return "cvBR1";
  if(value==cvBR2)return "cvBR2";
  return "cvBR3";
}
TString MString::getCVString(CVDataWidth     value){
  if(value==cvD8         )return "cvD8"         ;
  if(value==cvD16        )return "cvD16"        ;
  if(value==cvD32        )return "cvD32"        ;
  if(value==cvD64        )return "cvD64"        ;
  if(value==cvD16_swapped)return "cvD16_swapped";
  if(value==cvD32_swapped)return "cvD32_swapped";
  return "cvD64_swapped";
}
TString MString::getCVString(CVErrorCodes value){
  if(value==cvSuccess     )return "cvSuccess"     ;
  if(value==cvBusError    )return "cvBusError"    ;
  if(value==cvCommError   )return "cvCommError"   ;
  if(value==cvGenericError)return "cvGenericError";
  if(value==cvInvalidParam)return "cvInvalidParam";
  return "cvTimeoutError";
}
TString MString::getCVString(CVInputSelect   value){
  if(value==cvInput0)return "cvInput0";
  return "cvInput1";
}
TString MString::getCVString(CVIOSources     value){
  if(value==cvManualSW   )return "cvManualSW"   ;
  if(value==cvInputSrc0  )return "cvInputSrc0"  ;
  if(value==cvInputSrc1  )return "cvInputSrc1"  ;
  if(value==cvCoincidence)return "cvCoincidence";
  if(value==cvVMESignals )return "cvVMESignals" ;
  return "cvMiscSignals";
}
TString MString::getCVString(CVIOPolarity    value){
  if(value==cvDirect  )return "cvDirect"  ;
  return "cvInverted";
}
TString MString::getCVString(CVIRQLevels     value){
  if(value==cvIRQ1)return "cvIRQ1";
  if(value==cvIRQ2)return "cvIRQ2";
  if(value==cvIRQ3)return "cvIRQ3";
  if(value==cvIRQ4)return "cvIRQ4";
  if(value==cvIRQ5)return "cvIRQ5";
  if(value==cvIRQ6)return "cvIRQ6";
  return "cvIRQ7";
}
TString MString::getCVString(CVLEDPolarity   value){
  if(value==cvActiveHigh)return "cvActiveHigh";
  return "cvActiveLow" ;
}
TString MString::getCVString(CVOutputSelect  value){
  if(value==cvOutput0)return "cvOutput0";
  if(value==cvOutput1)return "cvOutput1";
  if(value==cvOutput2)return "cvOutput2";
  if(value==cvOutput3)return "cvOutput3";
  return "cvOutput4";
}
TString MString::getCVString(CVPulserSelect  value){
  if(value==cvPulserA)return "cvPulserA";
  return "cvPulserB";
}
TString MString::getCVString(CVTimeUnits     value){
  if(value==cvUnit25ns  )return "cvUnit25ns"  ;
  if(value==cvUnit1600ns)return "cvUnit1600ns";
  if(value==cvUnit410us )return "cvUnit410us" ;
  return "cvUnit104ms" ;
}
TString MString::getCVString(CVRegisters     value){
  if(value==cvStatusReg      )return "cvStatusReg"      ;
  if(value==cvVMEControlReg  )return "cvVMEControlReg"  ;
  if(value==cvFwRelReg       )return "cvFwRelReg"       ;
  if(value==cvFwDldReg       )return "cvFwDldReg"       ;
  if(value==cvFlenaReg       )return "cvFlenaReg"       ;
  if(value==cvVMEIRQEnaReg   )return "cvVMEIRQEnaReg"   ;
  if(value==cvInputReg       )return "cvInputReg"       ;
  if(value==cvOutRegSet      )return "cvOutRegSet"      ;
  if(value==cvInMuxRegSet    )return "cvInMuxRegSet"    ;
  if(value==cvOutMuxRegSet   )return "cvOutMuxRegSet"   ;
  if(value==cvLedPolRegSet   )return "cvLedPolRegSet"   ;
  if(value==cvOutRegClear    )return "cvOutRegClear"    ;
  if(value==cvInMuxRegClear  )return "cvInMuxRegClear"  ;
  if(value==cvOutMuxRegClear )return "cvOutMuxRegClear" ;
  if(value==cvLedPolRegClear )return "cvLedPolRegClear" ;
  if(value==cvPulserA0       )return "cvPulserA0"       ;
  if(value==cvPulserA1       )return "cvPulserA1"       ;
  if(value==cvPulserB0       )return "cvPulserB0"       ;
  if(value==cvPulserB1       )return "cvPulserB1"       ;
  if(value==cvScaler0        )return "cvScaler0"        ;
  if(value==cvScaler1        )return "cvScaler1"        ;
  if(value==cvDispADL        )return "cvDispADL"        ;
  if(value==cvDispADH        )return "cvDispADH"        ;
  if(value==cvDispDTL        )return "cvDispDTL"        ;
  if(value==cvDispDTH        )return "cvDispDTH"        ;
  if(value==cvDispC1         )return "cvDispC1"         ;
  if(value==cvDispC2         )return "cvDispC2"         ;
  if(value==cvLMADL          )return "cvLMADL"          ;
  if(value==cvLMADH          )return "cvLMADH"          ;
  return "cvLMC"            ;
}
TString MString::getCVString(CVReleaseTypes  value){
  if(value==cvRWD)return "cvRWD";
  return "cvROR";
}
TString MString::getCVString(CVRequesterTypes value){
  if(value==cvFair  )return "cvFair"  ;
  return "cvDemand";
}
TString MString::getCVString(CVVMETimeouts   value){
  if(value==cvTimeout50us )return "cvTimeout50us" ;
  return "cvTimeout400us";
}
TString MString::makeBinary(uint32_t value){
  TString tmp="";
  uint32_t data=value;
  for(int i=0;i<32;i++){
    if(data%2==1){
      tmp="1"+tmp;
    }else{
      tmp="0"+tmp;
    }
    if((i+1)%4==0)tmp=" "+tmp;
    data=data/2;
  }
  return tmp;
}
