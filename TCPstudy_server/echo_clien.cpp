#include "common.h"
#define PORT_NUM 10200
#define MAX_MSG_LEN 256

int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); // ���� �ʱ�ȭ
	
	char server_ip[40] = "";
	printf("���� IP:");
	gets_s(server_ip, sizeof(server_ip));

	SOCKET sock;
	sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); //���� ����
	SOCKADDR_IN seraddr = { 0 }; // ���� �ּ� �ʱ�ȭ
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
		printf("����:%s\n", msg);
	}
	closesocket(sock);

	WSACleanup();// ���� ����ȭ
	return 0;
}
