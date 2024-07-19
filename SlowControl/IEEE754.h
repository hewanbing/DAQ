#ifndef IEEE754_H
#define IEEE754_H

#endif // IEEE754_H
#include "myData.h"
#include <QDebug>
class IEEE754{
public:
    IEEE754();
    static float toFloat(int value){
        myData data(value);
        int s=data.get(31);
        int ep=data.get(23,30)-127;
        float m=data.get(0,22);
        m=m/pow(2,23)+1;
        return pow(-1,s)*pow(2,ep)*m;
    }
    static int toInt(float value){
        if(value==0)return 0;
        float tmp=value;
        int s=0;
        if(value<0){
            s=1;
            tmp=-tmp;
        }
        int ep=0;
        while(tmp<1){
            tmp=tmp*2;
            ep--;
        }
        while(tmp>=2){
            tmp=tmp/2;
            ep++;
        }
        ep=ep+127;
        int m=((tmp-1)*pow(2,23));
        ep=ep<<23;
        s=s<<31;
        return m+ep+s;
    }
};
