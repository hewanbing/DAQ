#include <iostream>
#include "wss.h"
using namespace std;

int main(int argc, char* argv[])
{
    string user="hirg";
    if(argc>1)user=argv[1];
    new WSS("60001",user.c_str());
    return 0;
}
