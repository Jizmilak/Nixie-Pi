/*********************************************************************
 * Nixie Clock
 * Author: Hamid Ebrahimi Kahaki
 * Version 0.3
 * *******************************************************************/
 
#include "i2c8Bit.h"
#include <iostream>
#include <ctime>
#include <unistd.h>

#define SADDR 0x20
#define MADDR 0x21
#define HADDR 0x22

#define IODIRA 0x00
#define IODIRB 0x01
#define GPIOA  0x12
#define GPIOB  0x13
#define OLATA  0x14
#define OLATB  0x15


using namespace std;
 
int main(void)
{

    time_t now;
    tm *ltm;
    
    //Defining I2C objects
    i2c8Bit i2c[3] = 
    {
	i2c8Bit(SADDR,string("/dev/i2c-0")),
	i2c8Bit(MADDR,string("/dev/i2c-0")),
	i2c8Bit(HADDR,string("/dev/i2c-0"))
    };

    //Setting Port A (MCP23017) to Output
    i2c[0].writeReg(IODIRA,0x00);
    i2c[1].writeReg(IODIRA,0x00);
    i2c[2].writeReg(IODIRA,0x00);

   // Tube test

	//Turn off tubes
   i2c[0].writeReg(OLATA,0xFF);
   i2c[1].writeReg(OLATA,0xFF);
   i2c[2].writeReg(OLATA,0xFF);

	
   for(int i=0;i<3;i++)
   	for(int j=0;j<10;j++)
   	{
		i2c[i].writeReg(OLATA,j+(j<<4));
		usleep(250000);
   	}
   
    int lastSec=60;
    while(1)
    {
        usleep(10000);
        // Reading current time
        now=time(0);
	ltm=localtime(&now);
	if(lastSec==ltm->tm_sec) continue;
        lastSec=ltm->tm_sec;

	//Updating nixie tubes
    	i2c[0].writeReg(OLATA,(char)(ltm->tm_sec % 10 + ((ltm->tm_sec/10)<< 4)));
    	i2c[1].writeReg(OLATA,(char)(ltm->tm_min % 10 + ((ltm->tm_min/10)<< 4)));
    	i2c[2].writeReg(OLATA,(char)(ltm->tm_hour % 10 + ((ltm->tm_hour/10)<< 4 )));
	
    }
    
    return 0;           
}
