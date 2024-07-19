#include "MTimer.h"
using namespace std;

MTimer::MTimer() {
}
MTimer::~MTimer() { }
TString MTimer::getString(int timeZoon){
  stringstream timeSS;
  time_t tt;
  time( &tt );
  tt = tt + timeZoon*3600;
  tm* t= gmtime( &tt );
  timeSS<<"Y"<<t->tm_year+1900
	<<"M"<<t->tm_mon+1
	<<"D"<<t->tm_mday
	<<"h"<<t->tm_hour
	<<"m"<<t->tm_min
	<<"s"<<t->tm_sec;
  string buf;
  timeSS>>buf;
  return buf;
}
double MTimer::getSeconds(){
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return tv.tv_sec%10000;
}
