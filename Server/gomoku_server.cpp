#include <stdio.h>
#include <winsock2.h>

int room_status=0;

int last_chess_x=-1, last_chess_y=-1;
int last_chess_color = -1; //0�� 1�� 

int main(int argc, char* argv[])
{
	//��ʼ��WSA
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA wsaData;
	if(WSAStartup(sockVersion, &wsaData)!=0)
	{
		return 0;
	}

	//�����׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����
	if(listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255],Send_Data[4096];
	while (true)
	{
		printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if(sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		SYSTEMTIME time;
		GetLocalTime(&time);
		printf("[%d-%02d-%02d %02d:%02d:%02d]\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond); 
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		//��������
		int ret = recv(sClient, revData, 255, 0);
		if(ret > 0)
		{
			revData[ret] = 0x00;
			if(revData[0]=='1')
			{
//				if(room_status == 0) 
//				{
					room_status = 1;
					last_chess_x=-1;
					last_chess_y=-1;
					last_chess_color=-1;
					sprintf(Send_Data,"1");
					printf("��������ɹ�\n");
//				}
//				else
//				{
//					sprintf(Send_Data,"0");
//					printf("��������ʧ��\n");
//				}
			}
			else if(revData[0]=='2')
			{
				if(room_status == 1) 
				{
					room_status = 2;
					sprintf(Send_Data,"1");
					printf("���뷿��ɹ�\n");
				}
				else
				{
					sprintf(Send_Data,"0");
					printf("���뷿��ʧ��\n");
				}
			}
			else if(revData[0]=='3')
			{
				if(room_status == 2) 
				{
					sprintf(Send_Data,"1");
					printf("ƥ��ɹ�\n");
				}
				else
				{
					sprintf(Send_Data,"0");
					printf("ƥ��ʧ��\n");
				}
			}
			else if(revData[0]=='4')
			{
				last_chess_x = (revData[1]-'0')*10+(revData[2]-'0');
				last_chess_y = (revData[3]-'0')*10+(revData[4]-'0');
				last_chess_color = 1;
				printf("�������ӣ�%d %d",last_chess_x,last_chess_y);
			}
			else if(revData[0]=='6')
			{
				last_chess_x = (revData[1]-'0')*10+(revData[2]-'0');
				last_chess_y = (revData[3]-'0')*10+(revData[4]-'0');
				last_chess_color = 0;
				printf("�������ӣ�%d %d",last_chess_x,last_chess_y);
			}
			else if(revData[0]=='5')
			{
				printf("Debug:%s\n",revData);
				if(revData[1]=='1')
				{
					if(last_chess_color == 0)
					{
						sprintf(Send_Data,"%d",10000+last_chess_x*100+last_chess_y);
					}
					else
						sprintf(Send_Data,"0");
				}
				else
				{
					if(last_chess_color == 1)
					{
						sprintf(Send_Data,"%d",10000+last_chess_x*100+last_chess_y);
					}
					else
						sprintf(Send_Data,"0");
				}
			}
			send(sClient, Send_Data, strlen(Send_Data), 0);	
			printf("Send:%s\n\n",Send_Data);
		}
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}
