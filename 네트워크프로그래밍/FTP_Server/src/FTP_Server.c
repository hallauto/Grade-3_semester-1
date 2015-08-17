/*
 * ftp_server.c
 *
 *  Created on: 2015. 5. 29.
 *      Author: kimchangyu
 */


#include "FTP_Server.h"

//클라이언트의 루트 디렉토리로 이동합니다. 이 때, directory_server에 절대경로 값을 저장합니다. 이렇게 해서 후에 클라이언트가 상대경로로 접근을 해도 문제가 없습니다.
int moveClientDirectory()
{
	if (access("host",0))
		mkdir("host", 0755);
	//만든 루트 디렉토리를 작업 경로로 지정합니다. 앞으로 이 디렉토리를 기준으로 작동합니다.
	chdir("host");
	//이후 클라이언트의 루트는 이 /host입니다. 따라서 이후에 디렉토리관련 출력에서 이 부분은 제거합니다.
	getcwd(directory_server,1024);
}

//FTP는 한번 작업을 하고 나면 데이터 소켓간의 연결을 종료합니다. 다음 작업을 위해서는 소켓을 새로 준비시켜야합니다.
int readyDataServer()
{
	int option = 1;
	socklen_t client_data_addr_len;
	server_data_sock = socket(PF_INET,SOCK_STREAM,0);
	setsockopt(server_data_sock,SOL_SOCKET,SO_REUSEADDR,(void *)&option, sizeof(option));

	memset(&server_data_sock_addr,0,sizeof(server_data_sock_addr));
	server_data_sock_addr.sin_family = AF_INET;
	server_data_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_data_sock_addr.sin_port = htons(FTP_DATA_NUMBER);

	if (bind(server_data_sock,(struct sockaddr*)&server_data_sock_addr,sizeof(server_data_sock_addr)) == -1)
	{
		printf("\ndata bind에서 오류 발생\n");
		return -1;
	}
	if (listen(server_data_sock,5) == -1)
	{
		printf("\nlisten에서 오류 발생\n");
		return -1;
	}

	return server_data_sock;
}

int sendRespondMessage()
{
	if (strlen(respond_message_string) < 1)
	{
		write(client_data_sock,"ERROR",BUFFERSIZE);
	}
	else
	{
		write(client_data_sock,respond_message_string,BUFFERSIZE);
	}
}

int main(void)
{
	int option = 1;
	socklen_t client_addr_len;

	printf("서버 프로그램 작동\n");

	//먼저 명령어 소켓을 생성합니다.
	server_command_sock = socket(PF_INET,SOCK_STREAM,0);
	setsockopt(server_command_sock,SOL_SOCKET,SO_REUSEADDR,(void *)&option, sizeof(option));

	//명령어 소켓의 주소 구조체에 주소를 할당합니다.
	memset(&server_command_sock_addr,0,sizeof(server_command_sock_addr));
	server_command_sock_addr.sin_family=AF_INET;
	server_command_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_command_sock_addr.sin_port = htons(FTP_COMMENT_NUMBER);

	//바인드하고 listen상태로 만들면 이제 클라이언트가 접속할 일만 남았습니다!
	if (bind(server_command_sock,(struct sockaddr*)&server_command_sock_addr,sizeof(server_command_sock_addr)) == -1)
	{
		printf("\nbind에서 오류 발생\n");
		return -1;
	}
	if (listen(server_command_sock,5) == -1)
	{
		printf("\nlisten에서 오류 발생\n");
		return -1;
	}

	client_addr_len = sizeof(client_command_sock_addr);

	//클라이언트가 접속하면 다음 단계로 넘어갑니다.
	client_command_sock = accept(server_command_sock,(struct sockaddr*)&client_command_sock_addr, &client_addr_len);

	if (client_command_sock == -1)
	{
		printf("\naccept에서 오류 발생\n");
		return -1;
	}
	//클라이언트와의 연결이 이루어졌습니다. 클라이언트의 디렉토리로 이동합시다!
	moveClientDirectory();
	//이제 클라이언트에게 FTP data 소켓의 포트번호를 전송합니다.
	write(client_command_sock,(char*)&FTP_DATA_NUMBER,sizeof(int));

	//이제 곧 클라이언트가 우리에게 FTP data 소켓에 접속을 요청할 것입니다! 이에 대비해서 우리도 소켓을 대기시킵시다!
	if (readyDataServer() == -1)
	{
		printf("서버 연결에 오류 발생");
	}
	//클라이언트가 QUIT메시지를 보내기 전까지는 서버는 계속 작동합니다.
	for (;;)
	{
		client_data_sock = accept(server_data_sock,(struct sockaddr*)&client_data_sock_addr, &client_addr_len);

		if (client_data_sock == -1)
		{
			printf("\naccept에서 오류 발생\n");
			return -1;
		}

		//이제 클라이언트가 보낼 명령어를 받을 준비를 합시다.
		char recv_tmp[BUFFERSIZE] = "";
		int read_len; //지금까지 read함수로 읽은 바이트 수입니다.
		int read_count; //read함수 호출 한번으로 읽은 바이트 수입니다. 만약 이 값이 -1이라면 읽기에 문제가 생긴 것입니다.
		read_count = read(client_data_sock,recv_tmp,BUFFERSIZE);
		if (read_count<-1)
		{
			printf("\nread에서 오류 발생\n");
			return -1;
		}
		recv_tmp[read_count - 1] = 0;
		printf("받은 메시지:%s\n",recv_tmp);
		//명령어 문자열을 받았으니 이제 이 명령어를 해독하고 해당하는 작업을 합시다.
		int check_message = check_message_head(recv_tmp);
		if (check_message)
		{
			printf("처리 결과:%s\n",respond_message_string);
			sendRespondMessage();
			//응답 메시지까지 보냈으니, 이제 출력 버퍼와 메시지 제작 버퍼를 비웁니다.
			strcpy(recv_tmp,"");
			strcpy(respond_message_string,"");
		}
		//클라이언트가 QUIT 메시지를 보냈습니다. 이제 소켓들을 모두 닫고 프로그램을 종료합니다.
		else if (check_message == 0)
		{
			printf("상대방이 프로그램을 종료했습니다. 이제 안녀~엉\n");
			sendRespondMessage();
			close(client_data_sock);
			close(server_data_sock);
			close(client_command_sock);
			close(server_command_sock);
			break;
		}
	}
}
