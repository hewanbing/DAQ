#include "vmehandle.h"

VMEHandle::VMEHandle(QObject *parent):QObject(parent)
{

}
bool VMEHandle::setBit(short AM, long address, long _data, int bitLow, int bitHigh, int dataWidth){}
bool VMEHandle::getBit(short AM, long address, long *_data, int bitLow, int bitHigh, int dataWidth){}
QString VMEHandle::getType(){
    return QString("VMEHandle");
}
bool VMEHandle::checkConnection(){}
