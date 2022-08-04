#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>


#define BUF_SIZE 1024


void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(const char* msg);

int main()
{
	WSADATA wasData;
	SOCKET lisnSock,recvSock;
	SOCKADDR_IN lisnadr, recvadr;
	int recvadrsz;
	WORD port = 5001;

	WSABUF dataBuf;
	WSAEVENT evobj;
	WSAOVERLAPPED overlapped;

	char buf[BUF_SIZE] = "안녕!!!!";
	DWORD recvbytes = 0, flags = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wasData) != 0)
		ErrorHandling("WSAStartup error");

	lisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (lisnSock == INVALID_SOCKET)
		ErrorHandling("INVALID_SOCKET");

	memset(&lisnadr, 0, sizeof(lisnadr));

	lisnadr.sin_family = AF_INET;
	lisnadr.sin_port = htons(port);
	lisnadr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(lisnSock, (SOCKADDR*)&lisnadr, sizeof(lisnadr)) == SOCKET_ERROR)
		ErrorHandling("bind error");

	if (listen(lisnSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen error");

	recvadrsz = sizeof(recvadr);
	recvSock = accept(lisnSock, (SOCKADDR*)&recvadr, &recvadrsz);

	evobj = WSACreateEvent();

	memset(&overlapped, 0, sizeof(overlapped));

	//생성한 이벤트를 overlapped 구조체에 등록해준다.
	overlapped.hEvent = evobj;

	//송수신할 때 사용할 버퍼
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;

	// WSARecv 비동기 수신    
	// 1. 소켓
	// 2. WSABUF 배열 주소 (버퍼 정보)
	// 3. WSABUF 배열 갯수
	// 4. 수신된 바이트수 (OUT), 함수 호출시 수신이 완료되었다면 의미있음
	// 5. 데이터 전송 특성
	// 6. WSAOVERLAPPED 주소
	// 7. Completion Routine 함수 주소, 전송 완료시 이 함수가 실행된다.

	if (WSARecv(recvSock, &dataBuf, 1, &recvbytes, &flags, &overlapped, NULL) == SOCKET_ERROR)
	{
		puts("background data receive");

		// WSA 이벤트를 기다린다. 전송 완료를 기다림.
		// overlapped 구조체에 등록된 이벤트 오브젝트가 signaled가 되면 반환된다.
		WSAWaitForMultipleEvents(1, &evobj, TRUE, WSA_INFINITE, FALSE);

		// 결과값을 받아온다. 
		WSAGetOverlappedResult(recvSock, &overlapped, &recvbytes, FALSE, NULL);
	}
	else
	{
		ErrorHandling("WSARecv() error");
	}

	printf("message: %s\n", buf);

	WSACloseEvent(evobj);
	closesocket(recvSock);
	closesocket(lisnSock);
	WSACleanup();

	return 0;
	}

	void CALLBACK CompRoutine(WSABUF databuf, DWORD recvBytes, DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED ipOverlapped, DWORD flags)
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

	void ErrorHandling(const char* message)
	{
		fputs(message, stderr);
		fputc('\n', stderr);
		exit(1);
	}

