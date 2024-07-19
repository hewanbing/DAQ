#include "Event.h"
#include <iostream>
Event::Event(){
  eventID=0;
  reset();
  for(int i=0;i<32;i++){
    ADC[i].channelID=i;
  }
}
Event::~Event(){
}
void Event::reset(){
  channelFilled=0;
}
bool Event::fillChannel(Channel ch){
  if(eventID!=ch.eventID){
      eventID=ch.eventID;
      reset();
  }
  for(int i=0;i<32;i++){
    if(ch.channelID==ADC[i].channelID){
      ADC[i]=ch;
      channelFilled++;
      break;
    }
  }
  if(channelFilled==32)return true;
  return false;
}
Channel Event::getADCChannel(int i){
  return ADC[i];
}
