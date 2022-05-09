#include "wifibot.h"

int Data_Of_Thread_In = 1;
char buffso_rcv[32]; 
char buffso_send[15];
int dbgcnt=0;
int rawproto=0;

HANDLE Handle_Of_Thread_TCP=0;

int servermode = 0;
int watchdog = 0;
int cycles = 0;
unsigned char speed1 = 0;
unsigned char speed2 = 0;

char buffso_rcvIN[100]; 
char buffso_rcvOUT[100]; 
char buffso_sendIN[100];
char buffso_sendOUT[100];
char buffer_address[sizeof(struct sockaddr)] = "0.0.0.0";
char buffer_localip[sizeof(struct sockaddr)] = "0.0.0.0";

char buffso_rcvIN_Tcp[30]; 
char buffso_rcvOUT_Tcp[30]; 
char buffso_rcvIN_Udp[30]; 
char buffso_rcvOUT_Udp[30]; 


HANDLE Handle_Of_Thread_TCP_trooper_In=0;
HANDLE Handle_Of_Thread_TCP_trooper_Out=0;
HANDLE Handle_Of_Thread_TCP_trooper_I2C=0;
HANDLE Handle_Of_Thread_Dog=0;
HANDLE Handle_Of_Thread_TCP_I2C_Simu=0;
HANDLE Handle_Of_Thread_TCP_RS232_33F=0;

Mutex mutexSend;
Mutex mutexRcv;

//Mutex mutexRaw;
//Mutex mutexRawrcv;

bool tcpon=false;
SensorData dataL;
SensorData dataR;

DWORD startTime, endTime;
DWORD elapsedTime;

LARGE_INTEGER freq;
LARGE_INTEGER startTime1, endTime1;
LARGE_INTEGER elapsedTime1, elapsedMilliseconds;

HANDLE SetupRS232CommPort( LPCSTR comport)
{
	HANDLE hCom;
	DCB dcb;
	COMMTIMEOUTS ct;

	//if (!hCom) CloseHandle(hCom);
	hCom = CreateFileA( comport, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	GetCommState(hCom, &dcb);
	dcb.BaudRate = RS232_SPEED;
	dcb.fParity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fAbortOnError = FALSE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = 0;
	SetCommState(hCom, &dcb);
	GetCommTimeouts(hCom, &ct);
	ct.ReadIntervalTimeout = 500;
	ct.ReadTotalTimeoutMultiplier =500; 
	ct.ReadTotalTimeoutConstant = 500; 
	SetCommTimeouts(hCom, &ct);
	SetCommMask(hCom, EV_RXCHAR);
	return hCom;
}

int GetMotorRS23233f(HANDLE hUSB,SensorData *dataL,SensorData *dataR)
{	    
	DWORD n;
	BYTE sbuf[30];//unsigned char for LINUX
	bool res=false;

	do {
		ReadFile(hUSB, &sbuf, 1, &n, NULL);
	}while(sbuf[0]!=255);

	res = ReadFile(hUSB, &sbuf, 21, &n, NULL);
	short mycrcrcv = (short)((sbuf[20] << 8) + sbuf[19]);
	short mycrcsend = Crc16(sbuf,19);

	if (mycrcrcv!=mycrcsend)
	{
		do {
			ReadFile(hUSB, &sbuf, 1, &n, NULL);
		}while(sbuf[0]!=255);
	}
	else {
		dataL->SpeedFront=(int)((sbuf[1] << 8) + sbuf[0]);
		if (dataL->SpeedFront > 32767) dataL->SpeedFront=dataL->SpeedFront-65536;
		dataL->BatLevel=sbuf[2];
		dataL->IR=sbuf[3];
		dataL->IR2=sbuf[4];
		dataL->odometry=((((long)sbuf[8] << 24))+(((long)sbuf[7] << 16))+(((long)sbuf[6] << 8))+((long)sbuf[5]));

		dataR->SpeedFront=(int)(sbuf[10] << 8) + sbuf[9];
		if (dataR->SpeedFront > 32767) dataR->SpeedFront=dataR->SpeedFront-65536;
		dataR->BatLevel=0;
		dataR->IR=sbuf[11];
		dataR->IR2=sbuf[12];
		dataR->odometry=((((long)sbuf[16] << 24))+(((long)sbuf[15] << 16))+(((long)sbuf[14] << 8))+((long)sbuf[13]));
		dataL->Current=sbuf[17];
		dataR->Current=sbuf[17];
		dataL->Version=sbuf[18];
		dataR->Version=sbuf[18];
		//mutexRaw.acquire();
		memcpy(buffso_rcvOUT_Tcp,sbuf,21);
		//mutexRaw.release();
	}
	return res;
}

int SetMotorRS23233f_low_res(HANDLE hUSB, BYTE speed1,BYTE speed2)
{
	DWORD n;
	BYTE sbuf[30];
	BYTE tt=0;
	sbuf[0] = 255;
	sbuf[1] = 0x07;

	int tmp1 = 8*(speed1&0x3F);
	int tmp2 = 8*(speed2&0x3F);
	if (speed2&0x80) tt=tt+32;
	if (speed2&0x40) tt=tt+16;
	sbuf[2] = (BYTE)tmp1;
	sbuf[3] = (BYTE)(tmp1 >> 8);

	sbuf[4] = (BYTE)tmp2;
	sbuf[5] = (BYTE)(tmp2 >> 8);

	sbuf[6] = (speed1&0x80) + (speed1&0x40) + tt +0+0+8;//+1 Relay ON +8 10ms pid mode ;

	short mycrcsend = Crc16(sbuf+1,6);

	sbuf[7] = (BYTE)mycrcsend;
	sbuf[8] = (BYTE)(mycrcsend >> 8);

	bool res = WriteFile(hUSB, &sbuf, 9,&n, NULL);
	return res;
}

int SetMotorRS23233f(HANDLE hUSB, short speed1,short speed2,BYTE SpeedFlag)
{
	DWORD n;
	BYTE sbuf[10];
	sbuf[0] = 255;
	sbuf[1] = 0x07;
	sbuf[2] = (BYTE)speed1;
	sbuf[3] = (BYTE)(speed1 >> 8);
	sbuf[4] = (BYTE)speed2;
	sbuf[5] = (BYTE)(speed2 >> 8);
	sbuf[6] = SpeedFlag;
	short mycrcsend = Crc16(sbuf+1,6);
	sbuf[7] = (BYTE)mycrcsend;
	sbuf[8] = (BYTE)(mycrcsend >> 8);

	bool res = WriteFile(hUSB, &sbuf, 9,&n, NULL);
	return res;
}

int SetMotorPIDRS23233f(HANDLE hUSB,BYTE speed1,BYTE speed2,BYTE pp,BYTE ii,BYTE dd,short maxspeed)
{
	DWORD n;
	BYTE sbuf[30];

	sbuf[0] = 255;
	sbuf[1] = 0x09;
	sbuf[2] = speed1;
	sbuf[3] = speed2;
	sbuf[4] = pp;
	sbuf[5] = ii;
	sbuf[6] = dd;
	sbuf[7] = (BYTE)maxspeed;
	sbuf[8] = (BYTE)(maxspeed >> 8);
	short mycrcsend = Crc16(sbuf+1,8);
	sbuf[9] = (BYTE)mycrcsend;
	sbuf[10] = (BYTE)(mycrcsend >> 8);
	WriteFile(hUSB, &sbuf, 11, &n, NULL);
	return sbuf[0];
}

int StopMotorRS23233f(HANDLE hUSB)
{
	SetMotorRS23233f_low_res(hUSB,0x00,0x00);
	return 0;
}

DWORD WINAPI Thread_TCP_Serial( LPVOID lpParam ) 
{
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN sin;
	int sinsize;
	int recvMsgSize=0;

	HANDLE hUSB = (HANDLE*)lpParam;
	//Initialisation du serveur
	WSAStartup(MAKEWORD(2,2),&wsa);
	//Création du socket
	sock=socket(PF_INET,SOCK_STREAM,0);
	sin.sin_family=AF_INET;
	//Lancement du bind
	sin.sin_port=htons((unsigned short)15020);
	sin.sin_addr.s_addr=INADDR_ANY;
	bind(sock,(SOCKADDR*)&sin,sizeof(sin));
	sinsize=sizeof(sin);
	printf("go \n");
	while(1)
	{
		listen(sock,1);
		SOCKET hAccept = accept(sock, NULL, NULL);
		rawproto=1;
		printf("client tcp raw ok %d\n",rawproto);
		Sleep(500);
		do{
			//mutexRawrcv.acquire();
			if ((recvMsgSize = recv(hAccept, (char*)buffso_rcvIN_Tcp, 9, 0)) < 1) shutdown(hAccept,1);
			//mutexRawrcv.release();
			//mutexSend.acquire();
			//printf("coucou %d\n",recvMsgSize);
			speed1=buffso_rcv[0];
			speed2=buffso_rcv[1];
			//mutexSend.release();
			watchdog = 0;
			cycles = 3;
			//mutexRcv.acquire();
			//			mutexRaw.acquire();
			send(hAccept,buffso_rcvOUT_Tcp,21,0);
			//			mutexRaw.release();
			//mutexRcv.acquire();
		}while(recvMsgSize>0);
	}
	return 0;
}

DWORD WINAPI Thread_Dog( LPVOID lpParam ) 
{
	while (1) {
		if(watchdog < cycles) watchdog = watchdog + 1;
		else if(watchdog == cycles)
		{
			speed1 = 0;
			speed2 = 0;
			buffso_rcvIN_Tcp[2] = (BYTE)speed1;
			buffso_rcvIN_Tcp[3] = (BYTE)(speed1 >> 8);
			buffso_rcvIN_Tcp[4] = (BYTE)speed2;
			buffso_rcvIN_Tcp[5] = (BYTE)(speed2 >> 8);
		}//end of else if watchdog
		Sleep(400);
	}
	return 0;
}

DWORD WINAPI Thread_UDP_In( LPVOID lpParam ) 
{
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN sin;
	int dataLen;
	int sinsize;

	HANDLE hUSB = (HANDLE*)lpParam;
	//Initialisation du serveur
	WSAStartup(MAKEWORD(2,2),&wsa);
	//Création du socket
	sock=socket(AF_INET,SOCK_DGRAM,0);
	sin.sin_family=AF_INET;
	//Lancement du bind
	sin.sin_port=htons((unsigned short)15000);
	sin.sin_addr.s_addr=INADDR_ANY;
	sinsize=sizeof(sin);
	bind(sock,(SOCKADDR*)&sin,sizeof(sin));

	while(1)
	{    
		//We wait for the client to send "init" and send "ok" in return
		dataLen = recvfrom(sock, buffso_rcvIN, 9,0,(SOCKADDR*)&sin, &sinsize);

		/*	
		//printf(" crc %d %d\n",(short)((buffso_rcvIN[8] << 8) + (buffso_rcvIN[7])),Crc16((unsigned char*)(buffso_rcvIN+1),6));
		if(!(strstr(buffso_rcvIN,"init")==NULL))
		{
		sprintf_s(buffso_sendIN,"ok");
		dataLen = sendto(sock,buffso_sendIN,3,0,(SOCKADDR*)&sin,sizeof(sin));
		printf("client udp ok %d \n",dataLen);
		}
		else */if(((short)((((unsigned char)buffso_rcvIN[8] << 8) + (unsigned char)buffso_rcvIN[7]))) == Crc16((unsigned char*)(buffso_rcvIN+1),6))
		{
			watchdog = 0;
			//mutexSend.acquire();
			//speed2 = buffso_rcvIN[3];
			//speed1 = buffso_rcvIN[5];
			//printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port), buffso_rcvIN);

			//printf("%d %d %d %d %d %d %d %d %d \n",buffso_rcvIN[0],buffso_rcvIN[1],buffso_rcvIN[2],buffso_rcvIN[3],buffso_rcvIN[4],buffso_rcvIN[5],buffso_rcvIN[6],buffso_rcvIN[7],buffso_rcvIN[8]);
			//printf("yes")

			memcpy(buffso_rcvIN_Tcp,buffso_rcvIN,9);

			//mutexSend.release();
#ifdef debug_msg 
			printf("speed1 %d speed2 %d steer1 %d steer2 %d \n",buffso_rcvIN[5],buffso_rcvIN[6],buffso_rcvIN[7],buffso_rcvIN[8]);
#endif
			cycles = 3;
			servermode = 0;
		}//end of else if
		else Sleep(1);
	}//end of main loop 
	return 0;
}

DWORD WINAPI Thread_UDP_Out( LPVOID lpParam ) 
{
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN sin;
	int dataLen;
	int sinsize;
	bool firstudp=true;

	//Initialisation du serveur
	WSAStartup(MAKEWORD(2,2),&wsa);
	//Création du socket
	sock=socket(AF_INET,SOCK_DGRAM,0);
	sin.sin_family=AF_INET;
	//Lancement du bind
	sin.sin_addr.s_addr=INADDR_ANY;
	sin.sin_port=htons((unsigned short)15010);
	//bind(sock,(SOCKADDR*)&sin,sizeof(sin));
	sinsize=sizeof(sin);
	bind(sock,(SOCKADDR*)&sin,sizeof(sin));
	HANDLE hUSB = (HANDLE*)lpParam;

	while(1)
	{	
		//We wait for the client to send "init" and send "ok" in return
		dataLen = recvfrom(sock, buffso_rcvOUT, 21,0,(SOCKADDR*)&sin, &sinsize);
		//printf("rcv");
		if((!(strstr(buffso_rcvOUT,"init")==NULL)))
		{
			//sprintf_s(buffso_sendOUT,2,"ok");
			sprintf_s(buffso_sendOUT,"ok");
			dataLen = sendto(sock,buffso_sendOUT,3,0,(SOCKADDR*)&sin,sizeof(sin));
			//firstudp=false;
		}
		else if(!(strstr(buffso_rcvOUT,"data")==NULL))
		{  
			//mutexRaw.acquire();
			//mutexRcv.acquire();
			short mycrcsend = Crc16((unsigned char*)(buffso_rcvOUT_Tcp+1),18);
			buffso_rcvOUT_Tcp[19]=(BYTE)mycrcsend;
			buffso_rcvOUT_Tcp[20]=(BYTE)(mycrcsend >> 8);

			dataLen = sendto(sock,buffso_rcvOUT_Tcp,21,0,(SOCKADDR*)&sin,sizeof(sin));
			memset(buffso_rcvOUT_Tcp,0,21);
			//mutexRaw.release();
			Sleep(1);
			//mutexRcv.release();
		}//end of else if

	}//end of main loop
	return 0;
}

DWORD WINAPI Thread_RS23233f( LPVOID lpParam ) 
{
	HANDLE hUSB = (HANDLE*)lpParam;
	DWORD n;
	speed1=0;speed2=0;
	SetMotorRS23233f_low_res(hUSB,0x00,0x00);
	Sleep(200);
	SetMotorPIDRS23233f(hUSB,0x00,0x00,80,45,0,360);
	SetMotorPIDRS23233f(hUSB,0x00,0x00,80,45,0,360);
	Sleep(200);
	int foo=0;

	while(1)
	{
#ifdef debug_msg 
		printf("speed1 %d speed2 %d\n",speed1,speed2);
#endif
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&startTime1);
		startTime = timeGetTime();
		mutexSend.acquire();
		int res=0;
		//if (rawproto==0) res=SetMotorRS23233f_low_res(hUSB,speed1,speed2);
		//else
		//mutexRawrcv.acquire();
		if ((buffso_rcvIN_Tcp[6]&0x01)==0) buffso_rcvIN_Tcp[6]=buffso_rcvIN_Tcp[6]+1;
		res = WriteFile(hUSB,buffso_rcvIN_Tcp , 9,&n, NULL);
		//mutexRawrcv.release();
		res = GetMotorRS23233f(hUSB,&dataL,&dataR);
		mutexSend.release();
		//We prepare the data to be sent to the user
		// run some code that takes many milliseconds
		endTime = timeGetTime();
		elapsedTime = endTime - startTime;

		QueryPerformanceCounter(&endTime1);
		elapsedTime1.QuadPart = endTime1.QuadPart - startTime1.QuadPart;
		elapsedMilliseconds.QuadPart = (1000 * elapsedTime1.QuadPart) / freq.QuadPart;

		//#ifdef debug_msg 
		//	printf("codeurL= %ld codeurR= %ld speed1 %d odo %ld %ld bat = %d IR_R = %d IR_L = %d time to set and get I2C = %d %d res %d\n",buffso_sendOUT[1],buffso_sendOUT[2],speed1,dataR.odometry,dataL.odometry,dataL.BatLevel,dataR.IR,dataL.IR,elapsedTime,elapsedMilliseconds.QuadPart,res);
		//	printf("speedL %d R%d\n",dataL.SpeedFront,dataR.SpeedFront);
		//#endif
		mutexRcv.release();	
	}
}

int init_tcp_com(HANDLE hUSB)
{
	Handle_Of_Thread_TCP = CreateThread( NULL, 0, Thread_TCP_Serial, &hUSB, 0, NULL);  
	if ( Handle_Of_Thread_TCP == NULL)  ExitProcess(Data_Of_Thread_In);

	Handle_Of_Thread_Dog = CreateThread( NULL, 0, Thread_Dog, &hUSB, 0, NULL);  
	if ( Handle_Of_Thread_Dog == NULL)  ExitProcess(Data_Of_Thread_In);
	return 1;	
}

int init_udp_com(HANDLE hUSB)
{
	Handle_Of_Thread_TCP_trooper_In = CreateThread( NULL, 0, Thread_UDP_In, &hUSB, 0, NULL);  
	if ( Handle_Of_Thread_TCP_trooper_In == NULL)  ExitProcess(Data_Of_Thread_In);

	Handle_Of_Thread_TCP_trooper_Out = CreateThread( NULL, 0, Thread_UDP_Out, &hUSB, 0, NULL);  
	if ( Handle_Of_Thread_TCP_trooper_Out == NULL)  ExitProcess(Data_Of_Thread_In);
	return 1;	
}

int init_RS232_33f(HANDLE hUSB)
{
	Handle_Of_Thread_TCP_trooper_I2C = CreateThread( NULL, 0, Thread_RS23233f, hUSB, 0, NULL);  
	if ( Handle_Of_Thread_TCP_trooper_I2C == NULL)  ExitProcess(Data_Of_Thread_In);
	return 1;	
}

int ctrlHandler(DWORD fdwCtrlType)
{
	switch( fdwCtrlType ) {
case CTRL_C_EVENT:
	printf( "Ctrl-C event\n\n" );
	CloseHandle(tmp_handle);
	return 1;
case CTRL_CLOSE_EVENT:
	CloseHandle(tmp_handle);
	printf( "Close event\n\n" );
default:
	return 0;
	}
}

short Crc16(unsigned char *Adresse_tab , unsigned char Taille_max)
{
	unsigned int Crc = 0xFFFF;
	unsigned int Polynome = 0xA001;
	unsigned int CptOctet = 0;
	unsigned int CptBit = 0;
	unsigned int Parity= 0;

	Crc = 0xFFFF;
	Polynome = 0xA001;
	for ( CptOctet= 0 ; CptOctet < Taille_max ; CptOctet++)
	{
		Crc ^= *( Adresse_tab + CptOctet);

		for ( CptBit = 0; CptBit <= 7 ; CptBit++)
		{
			Parity= Crc;
			Crc >>= 1;
			if (Parity%2 == true) Crc ^= Polynome;
		}
	}
	return(Crc);
}