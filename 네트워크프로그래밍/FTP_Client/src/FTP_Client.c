/*
 * FTP_Client.c
 *
 *  Created on: 2015. 5. 29.
 *      Author: kimchangyu
 */

#include "FTP_Client.h"

//FTP의 데이터 전송을 위한 소켓을 연결하는 함수입니다.
int access_data_socket()
{
	char send_tmp[BUFFERSIZE] = "이거 작동 잘 하나요?";
	char recv_tmp[BUFFERSIZE] = "";
	int read_count = 0;

	//소켓을 생성합니다.
	client_data_sock = socket(PF_INET,SOCK_STREAM,0);
	memset(&client_data_sock_addr,0,sizeof(struct sockaddr_in));
	client_data_sock_addr.sin_family = AF_INET;
	client_data_sock_addr.sin_addr.s_addr = inet_addr(local_address);
	client_data_sock_addr.sin_port = htons(data_port_number);

	//먼저 서버에 연결합니다.
	if (connect(client_data_sock,(struct sockaddr*)&client_data_sock_addr,sizeof(client_data_sock_addr)) == -1)
	{
		printf("\n서버에 연결이 안되는 게 이거 참 큰일났네\n");
		close(client_data_sock);
		return -1;
	}
	else
	{
		printf("\ndata 소켓에 connect 성공\n");
	}

	//새로 연결한 데이터 소켓을 반환합니다.
	return client_data_sock;
}

//보낼 메시지를 작성하는 함수입니다.
int sending_message_make()
{
	printf("메시지를 작성하십시오\n");
	scanf("%[^\n]s",request_message_string);
	getchar();
	printf("%s\n",request_message_string);
	return 0;
}

//STOR 명령어는 파일을 전송하는 명령어로, 다른 명령어들보다 복잡한 기능을 요구합니다. 이를 구현한 함수입니다.
int STOR_function()
{
	int read_len;
	char data;
	char* file_name = strtok(NULL,"\t");
	//먼저 입력한 이름의 파일을 엽니다.
	FILE* send_file;
	send_file = fopen(file_name,"r");
	if (send_file == NULL)
	{
		printf("파일을 열 수 없습니다.");
		return 1;
	}
	else
	{
		while(1)
		{
			//파일의 내용을 전송하기 전에 버퍼를 비웁니다.
			memset(request_message_string,0,BUFFERSIZE);
			//파일에서 BUFFERSIZE만큼 읽고, 이를 서버 데이터 소켓에 입력하면 됩니다.
			read_len = fread(request_message_string, sizeof(char), BUFFERSIZE, send_file);

			write(client_data_sock,request_message_string,read_len);
			//fwrite(request_message_string,sizeof(char),BUFFERSIZE,client_data_sock);
			//만약 파일을 끝까지 읽었거나 읽던 중에 오류가 발생했다면 read_len은 0이 됩니다. 이 때에는 전송을 끝냅니다.
			if (read_len <= 0)
			{
				fclose(send_file);
				break;
			}
		}
	}
}

int RETR_function()
{
	int read_len;
	char* file_name = strtok(NULL,"\t");
	char recv_tmp[BUFFERSIZE];
	FILE* recv_file = fopen(file_name,"w+");
	if (recv_file == NULL)
	{
		printf("파일을 열 수 없습니다.");
		return 1;
	}
	else
	{

		while(1)
		{
			memset(recv_tmp,0,BUFFERSIZE);
			read_len = read(client_data_sock,recv_tmp,BUFFERSIZE);
			fwrite(recv_tmp,sizeof(char),read_len,recv_file);
			if (read_len <= 0 || read_len == EOF)
			{
				fclose(recv_file);
				break;
			}
		}
	}
}


int main(void)
{
	int read_len = 0; //소켓에서 보내온 메시지를 읽어온 총 길이입니다.
	int read_count = 0; //소켓에서 보내온 메시지를 read로 한번 읽은 길이입니다.

	client_comment_sock = socket(PF_INET,SOCK_STREAM,0);
	memset(&client_comment_sock_addr,0,sizeof(struct sockaddr_in));
	client_comment_sock_addr.sin_family = AF_INET;
	client_comment_sock_addr.sin_addr.s_addr = inet_addr(local_address);
	client_comment_sock_addr.sin_port = htons(FTP_COMMANT_NUMBER);

	//먼저 서버에 연결합니다.
	if (connect(client_comment_sock,(struct sockaddr*)&client_comment_sock_addr,sizeof(client_comment_sock_addr)) == -1)
	{
		printf("\n서버에 연결이 안되는 게 이거 참 큰일났네\n");
		close(client_comment_sock);
		return -1;
	}
	else
	{
		printf("\nconnect 성공\n");
	}

	read(client_comment_sock,&data_port_number,sizeof(int));

	printf("data_port_number:%d\n",data_port_number);

	int select;
	for (;;)
	{
		printf("뭐 할꺼냐?\n1.SendMessage\t2.EXIT\n");
		scanf("%d",&select);
		getchar();
		if (select == 1)
		{
			if (access_data_socket())
			{
				sending_message_make();

				write(client_data_sock,request_message_string,BUFFERSIZE);
				//파일 전송, 또는 가져오기인지 검사하기 명령어인지 알기 위해 입력한 메시지를 복사해서 검사합니다.
				char tmp_msg[BUFFERSIZE];
				char* check_msg;
				strcpy(tmp_msg,request_message_string);
				check_msg = strtok(tmp_msg,"\t");

				if (!strcmp(check_msg,"STOR"))
				{
					STOR_function();
				}
				else if (!strcmp(check_msg,"RETR"))
				{
					RETR_function();
				}
				else
				{
					read_count = read(client_data_sock,read_message_string,BUFFERSIZE);
					if (read_count<-1)
					{
						printf("\nread에서 오류 발생\n");
						return -1;
					}

					read_message_string[read_count - 1] = 0;
					printf("%s\n",read_message_string);

					if (!strcmp(request_message_string,"QUIT"))
					{
						close(client_comment_sock);
						break;
					}
				}

				close(client_data_sock);
			} //if (access_data_socket())문 종료
		}
		else
		{
			break;
		}
	}
}
