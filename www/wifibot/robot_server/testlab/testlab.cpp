// testlab.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <math.h>

#include "wifibot.h"

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlHandler, 1);
	HANDLE hUSB=NULL;
	hUSB = SetupRS232CommPort((LPCSTR)PORT_I2C_USB);
	tmp_handle = hUSB;
	printf("\nCOM HANDLE %ld \n",hUSB);

	init_RS232_33f(hUSB);
	init_tcp_com(hUSB);
	init_udp_com(hUSB);
	unsigned char check=0;
	int pidi=0;
	int pidp=0;
	while (1)
	{
		Sleep(100);	
		char c=getchar();
		if (c=='p') {
			pidi=pidi+5;
			SetMotorPIDRS23233f(hUSB,0x00,0x00,pidp,pidi,0,360);
			printf("pidi%d\n",pidi);
			if (pidi==100) pidi=0;
		}
		if (c=='l') {
			pidi=pidi-5;
			SetMotorPIDRS23233f(hUSB,0x00,0x00,pidp,pidi,0,360);
			printf("pidi%d\n",pidi);
			if (pidi==0) pidi=0;
		}
		if (c=='a') {
			pidp=pidp+5;
			SetMotorPIDRS23233f(hUSB,0x00,0x00,pidp,pidi,0,360);
			printf("pidp%d\n",pidp);
			if (pidp==100) pidp=0;
		}
		if (c=='q') {
			pidp=pidp-5;
			SetMotorPIDRS23233f(hUSB,0x00,0x00,pidp,pidi,0,360);
			printf("pidp%d\n",pidp);
			if (pidp==0) pidp=0;
		}
	};
	return 0;
}

