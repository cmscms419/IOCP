#include "common.h"
#define PORT_NUM 10200
#define MAX_MSG_LEN 256

int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); // 원속 초기화
	
	char server_ip[40] = "";
	printf("서버 IP:");
	gets_s(server_ip, sizeof(server_ip));

	SOCKET sock;
	sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); //소켓 생성
	SOCKADDR_IN seraddr = { 0 }; // 서버 주소 초기화
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr(server_ip);
	seraddr.sin_port = htons(PORT_NUM);

	int re;
	re = connect(sock, (SOCKADDR*)&seraddr, sizeof(seraddr));
	if (re == -1)
		return -1;

	char msg[MAX_MSG_LEN] = "";
	while (true)
	{
		gets_s(msg, MAX_MSG_LEN);
		send(sock, msg, sizeof(msg), 0);
		if (strcmp(msg, "exit") == 0)
			break;
		recv(sock, msg, sizeof(msg), 0);
		printf("수신:%s\n", msg);
	}
	closesocket(sock);

	WSACleanup();// 원속 헤제화
	return 0;
}
