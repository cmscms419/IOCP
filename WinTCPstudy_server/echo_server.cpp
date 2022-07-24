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
	WSAStartup(MAKEWORD(2, 2), &wsadata); // ���� �ʱ�ȭ
	
	SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE); //��� ���� ����
	AcceptLoop(sock);//Accept Loop
	closesocket(sock);//���� ����
	WSACleanup();// ���� ����ȭ
	return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // ���� ����
	
	SOCKADDR_IN seraddr = { 0 }; // ���� �ּ� �ʱ�ȭ
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr = GetDefaultMyIP();
	seraddr.sin_port = htons(pnum);

	//��Ʈ��ũ �������̽��� ���� ����
	int re = 0;
	re = bind(sock, (SOCKADDR*)&seraddr, sizeof(seraddr));
	if (re == -1)
		return -1; // => 0xFFFFFFFFFFFFF ���� ����

	re = listen(sock, blog);
	if (re == -1)
		return -1; // => 0xFFFFFFFFFFFFF ���� ����

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
		dosock = accept(sock, (SOCKADDR*)&cliaddr, &len); // ���� ����
		if (dosock == -1)
		{
			perror("accept ����");
			break;
		}
		printf("%s: %d ���� ��û ����\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
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