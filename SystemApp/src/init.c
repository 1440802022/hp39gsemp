#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "init.h"
#include "cdc_console.h"
#include "keyboard.h"
#include "ff.h"
#include "rtc.h"

#include "ServiceGraphic.h"
#include "ServiceSTMPPartition.h"
#include "ServiceFlashMap.h"
#include "ServiceUSBDevice.h"
#include "ServiceFatFs.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


const unsigned char HzString1[6][33] = {



	{'F', 0x08,0x00,0xC8,0x1F,0x48,0x10,0x48,0x10,0xDF,0x1F,0x48,0x10,0x4C,0x10,0xDC,0x1F,0x6A,0x22,0x4A,0x12,0x49,0x0C,0x48,0x04,0x48,0x08,0x48,0x11,0xC8,0x60,0x48,0x00},/*"��",0*/
	{'F', 0x00,0x00,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10},/*"Ŀ",1*/
	{'F', 0x00,0x00,0xFC,0x0F,0x00,0x08,0x00,0x08,0xF8,0x0F,0x00,0x08,0x00,0x08,0xFF,0x7F,0x80,0x00,0x84,0x10,0x88,0x09,0xA0,0x06,0x90,0x04,0x8C,0x18,0xA3,0x60,0x40,0x00},/*"¼",2*/
	{'F', 0x00,0x00,0xFC,0x0F,0x04,0x08,0x04,0x08,0xFC,0x0F,0x04,0x20,0x04,0x20,0xF8,0x3F,0x00,0x00,0x10,0x04,0x10,0x04,0xFF,0x7F,0x10,0x04,0x08,0x04,0x04,0x04,0x02,0x04},/*"��",3*/
	{'F', 0x80,0x00,0x88,0x08,0x90,0x04,0xFE,0x7F,0x02,0x40,0xF9,0x2F,0x08,0x08,0xF8,0x0F,0x80,0x00,0xFC,0x1F,0x84,0x10,0x84,0x10,0x84,0x14,0x84,0x08,0x80,0x00,0x80,0x00},/*"��",4*/
	{'F', 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00},/*"��",5*/


};

const unsigned char HzString2[44][33]={

	{'F', 0x08,0x00,0xC8,0x1F,0x48,0x10,0x48,0x10,0xDF,0x1F,0x48,0x10,0x4C,0x10,0xDC,0x1F,0x6A,0x22,0x4A,0x12,0x49,0x0C,0x48,0x04,0x48,0x08,0x48,0x11,0xC8,0x60,0x48,0x00},/*"��",0*/
	{'F', 0x00,0x00,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10},/*"Ŀ",1*/
	{'F', 0x00,0x00,0xFC,0x0F,0x00,0x08,0x00,0x08,0xF8,0x0F,0x00,0x08,0x00,0x08,0xFF,0x7F,0x80,0x00,0x84,0x10,0x88,0x09,0xA0,0x06,0x90,0x04,0x8C,0x18,0xA3,0x60,0x40,0x00},/*"¼",2*/
	{'F', 0x40,0x00,0x80,0x00,0x80,0x00,0xFF,0x7F,0x08,0x08,0x08,0x08,0x10,0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x00,0x40,0x01,0x20,0x02,0x10,0x04,0x0C,0x18,0x03,0x60},/*"��",3*/
	{'F', 0x10,0x04,0x10,0x04,0x90,0x04,0x88,0x04,0x88,0x3F,0x4C,0x04,0x4C,0x04,0x2A,0x04,0x09,0x04,0xE8,0x7F,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04},/*"��",4*/
	{'F', 0x00,0x1F,0xFC,0x00,0x20,0x00,0x10,0x04,0x08,0x02,0xFC,0x01,0x80,0x00,0x60,0x08,0x18,0x10,0xFE,0x3F,0x80,0x20,0x90,0x04,0x88,0x08,0x84,0x10,0xA2,0x20,0x40,0x00},/*"ϵ",5*/
	{'F', 0x08,0x02,0x08,0x04,0x04,0x04,0xC4,0x7F,0x12,0x02,0x1F,0x11,0x88,0x20,0xC4,0x7F,0x02,0x49,0x1F,0x09,0x02,0x09,0x00,0x09,0x98,0x48,0x87,0x48,0x42,0x70,0x20,0x00},/*"ͳ",6*/
	{'F', 0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x3E,0x7F,0x22,0x48,0x22,0x48,0x22,0x48,0x22,0x48,0x22,0x48,0x22,0x48,0x22,0x48,0x22,0x44,0x22,0x44,0x3E,0x52,0x22,0x21,0x00},/*"��",7*/
	{'F', 0x10,0x04,0x10,0x14,0xFE,0x24,0x10,0x04,0xFF,0x7F,0x08,0x04,0x08,0x24,0xFF,0x24,0x04,0x24,0x12,0x14,0xFE,0x14,0x10,0x08,0xF0,0x48,0x1F,0x54,0x10,0x62,0x10,0x41},/*"��",8*/
	{'F', 0x80,0x00,0x88,0x00,0x88,0x00,0x88,0x00,0xFC,0x1F,0x84,0x00,0x82,0x00,0x80,0x00,0xFF,0x7F,0x40,0x01,0x20,0x02,0x20,0x02,0x10,0x04,0x08,0x08,0x04,0x10,0x03,0x60},/*"ʧ",9*/
	{'F', 0x00,0x02,0x3E,0x02,0x22,0x02,0x2A,0x01,0x2A,0x7F,0xAA,0x10,0x6A,0x11,0x2A,0x11,0x2A,0x11,0x2A,0x0A,0x2A,0x0A,0x08,0x04,0x14,0x0A,0x24,0x11,0xA2,0x20,0x41,0x40},/*"��",10*/
	{'F', 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x08,0x00,0x04,0x00,0x00,0x00},/*"��",11*/
	{'F', 0x00,0x02,0x02,0x02,0xE4,0x3F,0x04,0x02,0xC0,0x1F,0x00,0x02,0xE7,0x7F,0x04,0x00,0xC4,0x1F,0x44,0x10,0xC4,0x1F,0x44,0x10,0xD4,0x1F,0x4C,0x10,0x44,0x14,0x40,0x08},/*"��",12*/
	{'F', 0x08,0x02,0x08,0x04,0x08,0x04,0xC8,0x7F,0x5F,0x40,0x28,0x22,0x08,0x02,0xD8,0x7F,0x0C,0x11,0x0B,0x11,0x88,0x10,0x08,0x0B,0x08,0x04,0x08,0x0A,0x0A,0x11,0xC4,0x20},/*"��",13*/
	{'F', 0x10,0x04,0x92,0x04,0x54,0x04,0x10,0x7C,0xFF,0x22,0x54,0x22,0x92,0x22,0x11,0x25,0x08,0x14,0x7F,0x14,0x44,0x08,0x42,0x08,0x26,0x14,0x18,0x14,0x2C,0x22,0x43,0x41},/*"��",14*/
	{'F', 0x40,0x00,0x80,0x00,0xFE,0x7F,0x02,0x40,0x01,0x20,0xF8,0x07,0x00,0x02,0x00,0x01,0x80,0x00,0xFF,0x7F,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xA0,0x00,0x40,0x00},/*"��",15*/
	{'F', 0x04,0x08,0x04,0x08,0xDC,0x3E,0x84,0x28,0x82,0x7F,0x5E,0x28,0x45,0x3E,0xE4,0x08,0x9F,0x3E,0xA4,0x08,0xA4,0x7F,0x44,0x08,0x54,0x08,0xAC,0x00,0x14,0x7F,0x00,0x00},/*"��",16*/
	{'H', 0x00,0x00,0x00,0x10,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"1",17*/
	{'F', 0x00,0x08,0x00,0x1F,0xFC,0x00,0x80,0x00,0xFF,0x7F,0x80,0x00,0xF8,0x0F,0x88,0x08,0xF8,0x0F,0x88,0x08,0xF8,0x0F,0x80,0x00,0xFC,0x1F,0x80,0x00,0xFF,0x7F,0x00,0x00},/*"��",18*/
	{'F', 0x08,0x00,0x10,0x20,0xFE,0x1E,0x00,0x02,0x44,0x02,0x28,0x02,0xFF,0x7E,0x10,0x12,0x10,0x12,0xFE,0x12,0x10,0x12,0x54,0x12,0x92,0x12,0x11,0x11,0x14,0x11,0x88,0x10},/*"��",19*/
	{'F', 0x08,0x01,0x08,0x01,0x08,0x1F,0x88,0x10,0xDF,0x08,0x28,0x05,0x0C,0x02,0x1C,0x05,0xAA,0x18,0x4A,0x60,0xA9,0x1F,0x88,0x10,0x88,0x10,0x88,0x10,0x88,0x1F,0x88,0x10},/*"��",20*/
	{'F', 0x00,0x12,0x00,0x22,0x00,0x22,0x00,0x02,0xFF,0x7F,0x00,0x02,0x00,0x02,0x7C,0x02,0x10,0x02,0x10,0x02,0x10,0x04,0x10,0x44,0xF0,0x48,0x1E,0x50,0x04,0x60,0x00,0x40},/*"ʽ",21*/
	{'F', 0x10,0x01,0x10,0x01,0x10,0x21,0x08,0x11,0x08,0x09,0x0C,0x05,0x0C,0x03,0x0A,0x01,0x89,0x01,0x48,0x01,0x28,0x01,0x08,0x41,0x08,0x41,0x08,0x41,0x08,0x7E,0x08,0x00},/*"��",22*/
	{'F', 0x08,0x00,0xC8,0x1F,0x48,0x10,0x48,0x10,0xDF,0x1F,0x48,0x10,0x4C,0x10,0xDC,0x1F,0x6A,0x22,0x4A,0x12,0x49,0x0C,0x48,0x04,0x48,0x08,0x48,0x11,0xC8,0x60,0x48,0x00},/*"��",23*/
	{'F', 0x00,0x00,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10},/*"Ŀ",24*/
	{'F', 0x00,0x00,0xFC,0x0F,0x00,0x08,0x00,0x08,0xF8,0x0F,0x00,0x08,0x00,0x08,0xFF,0x7F,0x80,0x00,0x84,0x10,0x88,0x09,0xA0,0x06,0x90,0x04,0x8C,0x18,0xA3,0x60,0x40,0x00},/*"¼",25*/
	{'F', 0x40,0x00,0x80,0x00,0x80,0x00,0xFF,0x7F,0x08,0x08,0x08,0x08,0x10,0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x00,0x40,0x01,0x20,0x02,0x10,0x04,0x0C,0x18,0x03,0x60},/*"��",26*/
	{'F', 0x10,0x04,0x10,0x04,0x90,0x04,0x88,0x04,0x88,0x3F,0x4C,0x04,0x4C,0x04,0x2A,0x04,0x09,0x04,0xE8,0x7F,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04},/*"��",27*/
	{'F', 0x00,0x1F,0xFC,0x00,0x20,0x00,0x10,0x04,0x08,0x02,0xFC,0x01,0x80,0x00,0x60,0x08,0x18,0x10,0xFE,0x3F,0x80,0x20,0x90,0x04,0x88,0x08,0x84,0x10,0xA2,0x20,0x40,0x00},/*"ϵ",28*/
	{'F', 0x08,0x02,0x08,0x04,0x04,0x04,0xC4,0x7F,0x12,0x02,0x1F,0x11,0x88,0x20,0xC4,0x7F,0x02,0x49,0x1F,0x09,0x02,0x09,0x00,0x09,0x98,0x48,0x87,0x48,0x42,0x70,0x20,0x00},/*"ͳ",29*/
	{'F', 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x08,0x00,0x04,0x00,0x00,0x00},/*"��",30*/
	{'F', 0x08,0x08,0x10,0x08,0x10,0x04,0x00,0x00,0xFE,0x3F,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0xFF,0x7F,0x10,0x04,0x10,0x04,0x08,0x04,0x08,0x04,0x04,0x04,0x02,0x04},/*"��",31*/
	{'F', 0x10,0x01,0x10,0x1F,0x90,0x10,0x52,0x08,0x14,0x05,0x14,0x02,0x10,0x09,0xD0,0x08,0x18,0x08,0xD4,0x7F,0x13,0x08,0x90,0x08,0x10,0x09,0x10,0x08,0x10,0x0A,0x10,0x04},/*"��",32*/
	{'F', 0x00,0x1F,0xFC,0x00,0x20,0x00,0x10,0x04,0x08,0x02,0xFC,0x01,0x80,0x00,0x60,0x08,0x18,0x10,0xFE,0x3F,0x80,0x20,0x90,0x04,0x88,0x08,0x84,0x10,0xA2,0x20,0x40,0x00},/*"ϵ",33*/
	{'F', 0x08,0x02,0x08,0x04,0x04,0x04,0xC4,0x7F,0x12,0x02,0x1F,0x11,0x88,0x20,0xC4,0x7F,0x02,0x49,0x1F,0x09,0x02,0x09,0x00,0x09,0x98,0x48,0x87,0x48,0x42,0x70,0x20,0x00},/*"ͳ",34*/
	{'F', 0x40,0x00,0x80,0x00,0x80,0x00,0xFF,0x7F,0x08,0x08,0x08,0x08,0x10,0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x00,0x40,0x01,0x20,0x02,0x10,0x04,0x0C,0x18,0x03,0x60},/*"��",35*/
	{'F', 0x10,0x04,0x10,0x04,0x90,0x04,0x88,0x04,0x88,0x3F,0x4C,0x04,0x4C,0x04,0x2A,0x04,0x09,0x04,0xE8,0x7F,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04},/*"��",36*/
	{'F', 0x08,0x02,0x08,0x22,0x88,0x2F,0x08,0x12,0x3F,0x0A,0xC8,0x7F,0x08,0x02,0x28,0x01,0x98,0x7F,0x4C,0x02,0x2B,0x01,0x08,0x3F,0x08,0x20,0x08,0x20,0x0A,0x14,0x04,0x08},/*"��",39*/
	{'F', 0x00,0x00,0xF8,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0x08,0x88,0x08,0x88,0x08,0x88,0x08,0x88,0x08,0x48,0x08,0x40,0x01,0x20,0x02,0x10,0x04,0x08,0x08,0x04,0x10},/*"��",40*/
	{'F', 0x00,0x00,0xFE,0x3F,0x40,0x00,0x20,0x00,0x10,0x04,0x08,0x08,0xFC,0x1F,0x00,0x10,0x80,0x00,0x80,0x00,0xFC,0x1F,0x80,0x00,0x80,0x00,0x80,0x00,0xFF,0x7F,0x00,0x00},/*"��",41*/
	{'F', 0x08,0x00,0xC8,0x1F,0x48,0x10,0x48,0x10,0xDF,0x1F,0x48,0x10,0x4C,0x10,0xDC,0x1F,0x6A,0x22,0x4A,0x12,0x49,0x0C,0x48,0x04,0x48,0x08,0x48,0x11,0xC8,0x60,0x48,0x00},/*"��",42*/
	{'F', 0x00,0x00,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10},/*"Ŀ",43*/
	{'F', 0x00,0x00,0xFC,0x0F,0x00,0x08,0x00,0x08,0xF8,0x0F,0x00,0x08,0x00,0x08,0xFF,0x7F,0x80,0x00,0x84,0x10,0x88,0x09,0xA0,0x06,0x90,0x04,0x8C,0x18,0xA3,0x60,0x40,0x00},/*"¼",44*/
	{'F', 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x24,0x00,0x24,0x00,0x18,0x00,0x00,0x00,0x00,0x00},/*"��",45*/




};


const unsigned char HzString3[3][33]={
	
	{'F',0x00,0x02,0x44,0x02,0x48,0x02,0xC8,0x1F,0x20,0x02,0x00,0x02,0xEF,0x3F,0x88,0x04,0x88,0x04,0x88,0x04,0x48,0x24,0x48,0x24,0x28,0x38,0x14,0x00,0xE2,0x7F,0x00,0x00},/*"ѡ",1*/
	{'F',0x00,0x00,0x80,0x7F,0x00,0x04,0x3F,0x02,0x88,0x3F,0x88,0x20,0x88,0x24,0x88,0x24,0x88,0x24,0x88,0x24,0x88,0x24,0xB8,0x22,0x07,0x0A,0x02,0x11,0x80,0x20,0x40,0x40},/*"��",2*/
	{'F',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00},/*"��",3*/
	
};

const unsigned char HzString4[9][33]={
	{'F', 0x00,0x00,0x80,0x3F,0x80,0x1F,0x80,0x0F,0x80,0x07,0x80,0x07,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x07,0x80,0x07,0x80,0x0F,0x80,0x1F,0x80,0x3F,0x00,0x00},/*"��",0*/
	{'H', 0x00,0x00,0x00,0x10,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"1",1*/
	{'F', 0x00,0x00,0xFC,0x01,0xF8,0x01,0xF0,0x01,0xE0,0x01,0xE0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xE0,0x01,0xE0,0x01,0xF0,0x01,0xF8,0x01,0xFC,0x01,0x00,0x00},/*"��",2*/
	{'F', 0x08,0x01,0x08,0x01,0x08,0x1F,0x88,0x10,0xDF,0x08,0x28,0x05,0x0C,0x02,0x1C,0x05,0xAA,0x18,0x4A,0x60,0xA9,0x1F,0x88,0x10,0x88,0x10,0x88,0x10,0x88,0x1F,0x88,0x10},/*"��",3*/
	{'F', 0x00,0x12,0x00,0x22,0x00,0x22,0x00,0x02,0xFF,0x7F,0x00,0x02,0x00,0x02,0x7C,0x02,0x10,0x02,0x10,0x02,0x10,0x04,0x10,0x44,0xF0,0x48,0x1E,0x50,0x04,0x60,0x00,0x40},/*"ʽ",4*/
	{'F', 0x10,0x01,0x10,0x01,0x10,0x21,0x08,0x11,0x08,0x09,0x0C,0x05,0x0C,0x03,0x0A,0x01,0x89,0x01,0x48,0x01,0x28,0x01,0x08,0x41,0x08,0x41,0x08,0x41,0x08,0x7E,0x08,0x00},/*"��",5*/
	{'F', 0x08,0x00,0xC8,0x1F,0x48,0x10,0x48,0x10,0xDF,0x1F,0x48,0x10,0x4C,0x10,0xDC,0x1F,0x6A,0x22,0x4A,0x12,0x49,0x0C,0x48,0x04,0x48,0x08,0x48,0x11,0xC8,0x60,0x48,0x00},/*"��",6*/
	{'F', 0x00,0x00,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0xFC,0x1F,0x04,0x10},/*"Ŀ",7*/
	{'F', 0x00,0x00,0xFC,0x0F,0x00,0x08,0x00,0x08,0xF8,0x0F,0x00,0x08,0x00,0x08,0xFF,0x7F,0x80,0x00,0x84,0x10,0x88,0x09,0xA0,0x06,0x90,0x04,0x8C,0x18,0xA3,0x60,0x40,0x00},/*"¼",8*/
};

const unsigned char HzString5[2][33] = {
	{'F',0x40,0x00,0x80,0x00,0xFE,0x7F,0x02,0x40,0x01,0x20,0xF8,0x0F,0x00,0x00,0x00,0x00,0xFE,0x3F,0x20,0x02,0x20,0x02,0x20,0x02,0x10,0x22,0x10,0x22,0x08,0x22,0x06,0x3C},/*"��",0*/
	{'F',0x00,0x0A,0x00,0x12,0x00,0x02,0xFC,0x7F,0x04,0x02,0x04,0x02,0x04,0x22,0x7C,0x22,0x44,0x22,0x44,0x14,0x44,0x14,0x44,0x48,0x54,0x4C,0x22,0x52,0x02,0x61,0x81,0x40},/*"��",1*/
};

unsigned char *vbuf;

GraphicMessage GM;
GraphicTextOutArgs text1;
BYTE work[FF_MAX_SS];

static void Hz16chOut(const unsigned char** HZlist,unsigned int len, unsigned int x, unsigned int y){
	unsigned char c,i;
	const unsigned char *p;
	unsigned char mode = 0;
	p = (const unsigned char *)HZlist;
	while(len){
		c=0;
		i=0;
		c = *p;
		p++;
		if(c == 'F'){
			if(x+16>256){
				x=0;
				y+=16;
			}
			for(int ly = 0; ly < 32; ly ++){
					c = *p;
					for(int lx = 0; lx < 8; lx++){
						
						if(c&1){
							vbuf[(x++) + 258 * y ] = 0xFF;
						}else{
							vbuf[(x++) + 258 * y ] = 0;
						}
						c=c>>1;
					}
						i++;
						p++;
					if((int)i % 2 == 0){
						y++;
						x-=16;
					}
			}
			x+=16;
			y-=16;
		}else{
			if(x+8>256){
				x=0;
				y+=16;
			}
			for(int ly = 0; ly < 16; ly ++){
					c = *p;
					for(int lx = 0; lx < 8; lx++){
						
						if(c&1){
							vbuf[(x++) + 258 * y ] = 0xFF;
						}else{
							vbuf[(x++) + 258 * y ] = 0;
						}
						c=c>>1;
					}
						p++;
						y++;
						x-=8;
			}
			x+=8;
			y-=16;
			p+=16;
		}
		len--;
	}
	
	GM.type = GRAPHIC_MSG_TYPE_FLUSH;
	xQueueSend(GraphicQueue, &(GM.selfAddr), ( TickType_t ) 0 );
}


void displayRecovery(){
	GM.type = GRAPHIC_MSG_TYPE_CLEAR;
	xQueueSend(GraphicQueue, &(GM.selfAddr), ( TickType_t ) 0 );	
	
	
	Hz16chOut((const unsigned char**)&HzString1,6,1,1);
	Hz16chOut((const unsigned char**)&HzString2,44,1+32, 1 + 16 * 1);
	Hz16chOut((const unsigned char**)&HzString3,3,1+32,1 + 16 * 4);
	Hz16chOut((const unsigned char**)&HzString4,9,1+32,1 + 16 * 5);
	
}

extern unsigned int FSOK;

void runInRecoverMode(){
	
	for(;;)
	{
		if(is_key_down(KEY_1))
			{
			GM.type = GRAPHIC_MSG_TYPE_CLEAR;
			xQueueSend(GraphicQueue, &(GM.selfAddr), ( TickType_t ) 0 );	
			vTaskDelay(100);
			
			if(isRawFlash()){
				
				//Hz16chOut((const unsigned char**)&HzString6,9,1+32,1 + 16 * 3);
				//vTaskDelay(1000);
				//resetFlashRegionInfo();
				//flashMapReset();
				
				displayRecovery();
				goto next_loop;
			}
			
			
		 
			FSOK = 0;

			lockFmap(true);
			flashMapClear();
			
			printf("start earsing\n");
			for(int i=getDataRegonStartBlock(); i<getDataRegonStartBlock() + getDataRegonTotalBlocks(); i++){
		
				xEraseFlashBlocks(i, 1, 5000);
				vTaskDelay(1);
				
			}
			
			flashMapReset();
			
			MKFS_PARM opt;
			opt.fmt = FM_FAT;
			opt.au_size = 2048;
			opt.align = 2048;
			
			FATFS fs;
			
			//int fr = f_mkfs("", &opt, work, sizeof work);
			int fr = f_mkfs("", 0, work, sizeof work);
			
			printf("format :%d\n",fr);
			
			f_mount(&fs, "/", 1);
			
			lockFmap(false);
			printf("format done.\n");
			
			FSOK = 1;
			
			
			Hz16chOut((const unsigned char**)&HzString5,2,1+16* 10,1 + 16 * 4);

			vTaskDelay(2000);
			
			flashSyncNow();
			displayRecovery();
		}
		
		
		
		next_loop:
		
		vTaskDelay(100);
	}
	
}

void vInit(){
	vTaskDelay(200);
	/*
	GM.selfAddr = &GM;
	GM.type = GRAPHIC_MSG_TYPE_CLEAR;
	xQueueSend(GraphicQueue, &(GM.selfAddr), ( TickType_t ) 0 );
	
	GM.type = GRAPHIC_MSG_TYPE_TEXTOUT;
	GM.argsList = &text1;
	text1.x = 1;
	text1.y = 1;
	text1.area_width = 100;
	text1.area_height = 16;
	text1.font_size = 12;
	text1.font_color = 255;
	text1.text = "Test a";
	xQueueSend(GraphicQueue, &(GM.selfAddr) , ( TickType_t ) 0 );
	*/
	xTaskCreate( vCDC_Console, "CDC Console", configMINIMAL_STACK_SIZE, NULL, 3, NULL );

	GM.selfAddr = &GM;
	GM.type = GRAPHIC_MSG_TYPE_CLEAR;
	xQueueSend(GraphicQueue, &(GM.selfAddr), ( TickType_t ) 0 );
	vbuf = (char *) getVramAddress();
	
	/*
	
	taskENTER_CRITICAL();
	
	
	
	
	taskEXIT_CRITICAL();
	*/
	
	
	
	if(!isfatFsInited()){
		displayRecovery();
		runInRecoverMode();
	}
	
	if(is_key_down(KEY_PLUS)){
		displayRecovery();
		runInRecoverMode();
	}
	
	GM.selfAddr = &GM;
	GM.type = GRAPHIC_MSG_TYPE_CLEAR;
	xQueueSend(GraphicQueue, &(GM.selfAddr), ( TickType_t ) 0 );	
	//modifyRegion(0,22,49);
	//saveRegionTable();
	
	GM.type = GRAPHIC_MSG_TYPE_TEXTOUT;
	GM.argsList = &text1;
	text1.x = 1;
	text1.y = 1;
	text1.area_width = 100;
	text1.area_height = 16;
	text1.font_size = 12;
	text1.font_color = 255;
	text1.text = "Booting 2...";
	xQueueSend(GraphicQueue, &(GM.selfAddr) , ( TickType_t ) 0 );
	
	//int x = 0;
	//unsigned int args[3] = {11 ,32, 43};
	//x = testCall(args,3);
	
	//printf("retx:%d\n",x);
	
	
	malloc_stats();
	
	
	for(int i=0;i<6;i++){
		printf("RTC:%d : %08x\n",i,rtc_persistent_get(i));	
	}
	
	rtc_persistent_set(3,0x55aa5a5a);

	for(int i=0;i<6;i++){
		printf("RTC:%d : %08x\n",i,rtc_persistent_get(i));	
	}
	
	
	asm volatile ("swi #1001");
	
	
	vTaskDelete(NULL);
	for(;;){}
}


/*
int testCall(unsigned int *args, unsigned int argsLen){
	unsigned int ret = 0;
	unsigned int *retAddr = ret;
	asm volatile ("ldr r0,%0" :: "m"(args));
	asm volatile ("ldr r1,%0" :: "m"(argsLen));
	asm volatile ("ldr r2,%0" :: "m"(retAddr));
	asm volatile ("swi #1000");
	return ret;
}
*/