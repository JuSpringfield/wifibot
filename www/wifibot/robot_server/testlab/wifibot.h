//wifibot laurent@wifibot.com 
#ifndef WIFIBOT
#define WIFIBOT

#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <math.h>
#include <winsock.h>
#include "mutex.h"

#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 255
#define PORT_I2C_USB "COM1"
#define RS232_SPEED CBR_19200
//#define debug_msg

struct SensorData
{
	int SpeedFront;
	int BatLevel;
	int IR;
	int IR2;
	long odometry;
	int Current;
	int Version;
};

static HANDLE tmp_handle;

HANDLE SetupRS232CommPort(LPCSTR comport);

int GetMotorRS23233f(HANDLE hUSB,SensorData*,SensorData*);
int SetMotorRS23233f(HANDLE hUSB,short speed1,short speed2,BYTE SpeedFlag);
int SetMotorRS23233f_low_res(HANDLE hUSB,BYTE speed1,BYTE speed2);
int SetMotorPIDRS23233f(HANDLE hUSB,BYTE speed1,BYTE speed2,BYTE pp,BYTE ii,BYTE dd,short maxspeed);

int StopMotorRS23233f(HANDLE hUSB);

DWORD WINAPI Thread_RS23233f_Serial( LPVOID lpParam );
DWORD WINAPI Thread_UDP_In( LPVOID lpParam );
DWORD WINAPI Thread_UDP_Out( LPVOID lpParam );
DWORD WINAPI Thread_RS23233f( LPVOID lpParam );
DWORD WINAPI Thread_Dog( LPVOID lpParam );

int init_RS232_33f(HANDLE hUSB);
int init_tcp_com(HANDLE hUSB);
int init_tcp_Trooper_com(HANDLE hUSB);
int init_udp_com(HANDLE hUSB);
int ctrlHandler(DWORD fdwCtrlType);
short Crc16(unsigned char *Adresse_tab , unsigned char Taille_max);
#endif