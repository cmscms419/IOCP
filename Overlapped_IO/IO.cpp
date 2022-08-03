#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024


void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(char* msg);

int main()
{
	WSABUF data;
	WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	WSAEVENT event;
	WSAOVERLAPPED overlapped;
	WSABUF databuf;
	char buf[BUF_SIZE] = "안녕하세여!";
	int recvbytes = 0;

	event = WSACreateEvent();
	memset(&overlapped, 0, sizeof(overlapped)); // 모든 비트를 0으로 초기화
	overlapped.hEvent = event;
	databuf.len = sizeof(buf);
	databuf.buf = buf;

	return 0;
}

void CALLBACK CompRoutine(WSABUF databuf,DWORD recvBytes,DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED ipOverlapped, DWORD flags)
{
	if (dwError != 0)
	{
		printf("Comproutine error\n");
	}
	else
	{
		recvBytes = szRecvBytes;
		printf("Received message : %s\n", databuf.buf);
	}
}