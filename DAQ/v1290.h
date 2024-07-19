#ifndef V1290_H
#define V1290_H
#include "FEEIRQ.h"
#include <vector>
#include "FEE.h"
#include <string>
#include <sstream>
#include "stdlib.h"
#include "MString.h"
#include "BufferTP.h"
#include "Channel.h"
#include <json/json.h>
#include "DAQHandle.h"

using namespace std;
class v1290:public FEE
{
public:
    v1290(DAQHandle *handle,Json::Value json);
    virtual ~v1290();
    void decode(void* data, int dataLength, BufferTP<Channel>* buffer);
private:
    bool isMCReadOK();
    bool isMCWriteOK();
    bool writeMCCommand(int code,int obj=0);
    int readMCCommand();
    //_____________________________
    bool setTrigMatchMode();
    bool setContinuousMode();
    bool isTrigMatchMode();
    bool isContinuousMode();
    bool loadDefaultConfig();
    bool saveUserConfig();
    bool setAutoLoadUserConfig();
    bool setAutoLoadDefaultConfig();
    bool setTrigWindowWidth(int width);
    bool setTrigWindowOffset(int offset);
    bool setTrigExtraSearchMargin(int margin);
    bool setTrigRejectMargin(int margin);
    bool enableTrigSubtraction();
    bool disableTrigSubtraction();
    bool isTrigSubtraction();
    bool setTrigEdgePairMode();
    bool setTrigEdgeTrailMode();
    bool setTrigEdgeLeadMode();
    bool setTrigEdgeTrailLeadMode();
    bool isTrigEdgePairMode();
    bool isTrigEdgeTrailMode();
    bool isTrigEdgeLeadMode();
    bool isTrigEdgeTrailLeadMode();
    bool setLSBEdge(int edge);
    bool setLeadTimeWidthResolution(int time,int resolution);
    int getResolution();
    bool setChannelDeadTime(int time);
    int getChannelDeadTime();
    bool enableHeaderTrailer();
    bool disableHeaderTrailer();
    bool isHeaderTrailerInReadout();
    bool setMaxHitsPerEvent(int n);
    int getMaxHitsPerEvent();
    bool enableErrorMark();
    bool disableErrorMark();
    bool enableBypassError();
    bool disableBypassError();
    bool setInternalErrorType(int code);
    int getInternalErrorType();
    bool setFIFOSize(int n);
    int getFIFOSize();
    bool enableChannel(int ch);
    bool disableChannel(int ch);
    bool enableChannelAll();
    bool disableChannelAll();
    bool setEventTrigger(int n);
    bool setIRQLevel(int n);
    bool setIRQID(int id);
};

#endif // V1290_H
