#include "v1290.h"
#include "MData.h"
v1290::v1290(DAQHandle* _handle, Json::Value _json)
{
    setHandle(_handle);
    setJson(_json);
    setBaseAddress(MString::parse(json["base"].asString().c_str()).getLong());
    setCrateID(MString::parse(json["crateID"].asString().c_str()).getInt());
    setModuleID(MString::parse(json["moduleID"].asString().c_str()).getInt());
    setTrigMatchMode();
    loadDefaultConfig();
    setMaxHitsPerEvent(1);
    setFIFOSize(0b111);
    enableChannelAll();
    FEEIRQ irq;
    irq.level=1;
    irq.ID=getModuleID();
    irq.AM=0xB;
    irq.dataSize=32;
    irq.dataAddress=0;
    setIRQLevel(irq.level);
    setIRQID(irq.ID);
    setIRQ(irq);
}
v1290::~v1290(){}

void v1290::decode(void *data, int dataLength, BufferTP<Channel> *buffer){

}
bool v1290::setEventTrigger(int n){
    return handle->write(9,address(0x1022),n);
}
bool v1290::setIRQLevel(int n){
    return handle->setBit(9,address(0x100A),n,0,2);
}
bool v1290::setIRQID(int id){
    return handle->setBit(9,address(0x100C),id,0,7);
}
bool v1290::isMCReadOK(){
    return handle->getBit(9,0x1030,1,1);
}
bool v1290::isMCWriteOK(){
    return handle->getBit(9,0x1030,0,0);
}
bool v1290::writeMCCommand(int code,int obj){
    while(!isMCWriteOK())usleep(100000);
    int command=code*0x100+obj;
    return handle->write(0x9,0x102E,command);
}
int v1290::readMCCommand(){
    while(isMCReadOK())usleep(100000);
    return handle->getBit(0x9,0x102E,8,15);
}
bool v1290::setTrigMatchMode(){
    return writeMCCommand(0);
}
bool v1290::setContinuousMode(){
    return writeMCCommand(0x100);
}
bool v1290::isTrigMatchMode(){
    writeMCCommand(0x200);
    return readMCCommand()==1;
}
bool v1290::isContinuousMode(){
    writeMCCommand(0x200);
    return readMCCommand()==0;
}
bool v1290::loadDefaultConfig(){
    return writeMCCommand(0x500);
}
bool v1290::saveUserConfig(){
    return writeMCCommand(0x600);
}
bool v1290::setAutoLoadUserConfig(){
    return writeMCCommand(0x800);
}
bool v1290::setAutoLoadDefaultConfig(){
    return writeMCCommand(0x900);
}
bool v1290::setTrigWindowWidth(int width){
    writeMCCommand(0x1000);
    return writeMCCommand(width);
}
bool v1290::setTrigWindowOffset(int offset){
    writeMCCommand(0x1100);
    return writeMCCommand(offset);
}
bool v1290::setTrigExtraSearchMargin(int margin){
    writeMCCommand(0x1200);
    return writeMCCommand(margin);
}
bool v1290::setTrigRejectMargin(int margin){
    writeMCCommand(0x1300);
    return writeMCCommand(margin);
}
bool v1290::enableTrigSubtraction(){
    return writeMCCommand(0x1400);
}
bool v1290::disableTrigSubtraction(){
    return writeMCCommand(0x1500);
}
bool v1290::isTrigSubtraction(){
    writeMCCommand(0x1600);
    return readMCCommand();
}
bool v1290::setTrigEdgePairMode(){
    writeMCCommand(0x2200);
    return writeMCCommand(0);
}
bool v1290::setTrigEdgeTrailMode(){
    writeMCCommand(0x2200);
    return writeMCCommand(1);
}
bool v1290::setTrigEdgeLeadMode(){
    writeMCCommand(0x2200);
    return writeMCCommand(0b10);
}
bool v1290::setTrigEdgeTrailLeadMode(){
    writeMCCommand(0x2200);
    return writeMCCommand(0b11);
}
bool v1290::isTrigEdgePairMode(){
    writeMCCommand(0x2300);
    return readMCCommand()==0;
}
bool v1290::isTrigEdgeTrailMode(){
    writeMCCommand(0x2300);
    return readMCCommand()==1; 
}
bool v1290::isTrigEdgeLeadMode(){
    writeMCCommand(0x2300);
    return readMCCommand()==0b10;
}
bool v1290::isTrigEdgeTrailLeadMode(){
    writeMCCommand(0x2300);
    return readMCCommand()==0b11;
}
bool v1290::setLSBEdge(int edge){
    //0x00->800ps
    //0x01->200ps
    //0x10->100ps
    //0x11->25ps
    writeMCCommand(0x2400);
    return writeMCCommand(edge);
}
bool v1290::setLeadTimeWidthResolution(int time,int resolution){
    writeMCCommand(0x2500);
    int command=time+resolution*0b1000;
    return writeMCCommand(command);
}
int v1290::getResolution(){
    writeMCCommand(0x2500);
    return readMCCommand();
}
bool v1290::setChannelDeadTime(int time){
    writeMCCommand(0x2800);
    return writeMCCommand(time);
}
int v1290::getChannelDeadTime(){
    writeMCCommand(0x2900);
    return readMCCommand();
}
bool v1290::enableHeaderTrailer(){
    return writeMCCommand(0x3000);
}
bool v1290::disableHeaderTrailer(){
    return writeMCCommand(0x3100);
}
bool v1290::isHeaderTrailerInReadout(){
    writeMCCommand(0x3200);
    return readMCCommand();
}
bool v1290::setMaxHitsPerEvent(int n){
    writeMCCommand(0x3300);
    return writeMCCommand(n);
}
int v1290::getMaxHitsPerEvent(){
    writeMCCommand(0x3400);
    return readMCCommand();
}
bool v1290::enableErrorMark(){
    return writeMCCommand(0x3500);
}
bool v1290::disableErrorMark(){
    return writeMCCommand(0x3600);
}
bool v1290::enableBypassError(){
    return writeMCCommand(0x3700);
}
bool v1290::disableBypassError(){
    return writeMCCommand(0x3800);
}
bool v1290::setInternalErrorType(int code){
    writeMCCommand(0x3900);
    return writeMCCommand(code);
}
int v1290::getInternalErrorType(){
    writeMCCommand(0x3A00);
    return readMCCommand();
}
bool v1290::setFIFOSize(int n){
    writeMCCommand(0x3B00);
    return writeMCCommand(n);
}
int v1290::getFIFOSize(){
    writeMCCommand(0x3C00);
    return readMCCommand();
}
bool v1290::enableChannel(int ch){
    int code=0x4000+ch;
    return writeMCCommand(code);
}
bool v1290::disableChannel(int ch){
    int code=0x4100+ch;
    return writeMCCommand(code);
}
bool v1290::enableChannelAll(){
    return writeMCCommand(0x4200);
}
bool v1290::disableChannelAll(){
    return writeMCCommand(0x4300);
}


