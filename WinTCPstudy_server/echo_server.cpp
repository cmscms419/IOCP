#include "common.h"
#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256

SOCKET SetTCPServer(short pnum, int blog);
void AcceptLoop(SOCKET sock);
void Doit(SOCKET dosock);

int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); // 원속 초기화
	
	SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE); //대기 소켓 가동
	AcceptLoop(sock);//Accept Loop
	closesocket(sock);//소켓 해제
	WSACleanup();// 원속 헤제화
	return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 소켓 생성
	
	SOCKADDR_IN seraddr = { 0 }; // 서버 주소 초기화
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr = GetDefaultMyIP();
	seraddr.sin_port = htons(pnum);

	//네트워크 인터페이스와 소켓 결합
	int re = 0;
	re = bind(sock, (SOCKADDR*)&seraddr, sizeof(seraddr));
	if (re == -1)
		return -1; // => 0xFFFFFFFFFFFFF 소켓 에러

	re = listen(sock, blog);
	if (re == -1)
		return -1; // => 0xFFFFFFFFFFFFF 소켓 에러

	printf("IP : %s: %d\n", inet_ntoa(seraddr.sin_addr), pnum);

	return sock;
}

void AcceptLoop(SOCKET sock)
{
	SOCKET dosock;
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	while (1)
	{
		dosock = accept(sock, (SOCKADDR*)&cliaddr, &len); // 연결 수락
		if (dosock == -1)
		{
			perror("accept 실패");
			break;
		}
		printf("%s: %d 연결 요청 수학\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
		Doit(dosock);

	}
}

void Doit(SOCKET dosock)
{
	char msg[MAX_MSG_LEN] = "";
	while (recv(dosock,msg, sizeof(msg),0) > 0)
	{
		printf("recv:%s\n", msg);
		send(dosock, msg, sizeof(msg), 0);
	}
	closesocket(dosock);
}