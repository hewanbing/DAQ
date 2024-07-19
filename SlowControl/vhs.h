#ifndef VHS_H
#define VHS_H

#include <QObject>
#include "vmehandle.h"

class VHS: public QObject
{
    Q_OBJECT
public:
    VHS(VMEHandle* handle, QObject* parent = nullptr);
    void setBase(long base);
    int getSN();
    QString getFW();
    bool checkFirmware();
    int  getVoltage(int ch);
    int  getCurrent(int ch);
    void setVoltage(int ch, float V);
    bool setBit(long offset, long data, int bitLow, int bitHigh,int dataWidth);
    bool getBit(long offset, long *data, int bitLow, int bitHigh,int dataWidth);
    void turnChannelOn(int ch);
    void turnChannelOff(int ch);
    bool channelSwitch(int ch);
    bool refresh();
    bool IsKillEnable;
    bool IsTemperatureGood;
    bool IsSupplyGood;
    bool IsModuleGood;
    bool IsEventActive;
    bool IsSafetyLoopGood;
    bool IsNoRamp;
    bool IsNoSumError;
    bool IsCommandComplete;
    bool IsSpecialMode;
    bool IsInputError;
    bool IsServiceNeeded;
    bool IsStop;
    bool IsInterlockOutput;
    bool IsAdjustment;
    bool Reserved;
    long Voltage[4];
    long Current[4];
    bool IsConnected;
    bool IsChannelOn[4];
    float temperature;
    float voltageSet[4];
private:
    long base;
    VMEHandle * handle;
    short AM;
    long ChAddr[4];
    long SN;
};

#endif // VHS_H
