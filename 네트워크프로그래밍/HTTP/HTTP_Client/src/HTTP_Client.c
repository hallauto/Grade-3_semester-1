/*
 * HTTP_Client.c
 *
 *  Created on: 2015. 5. 22.
 *      Author: kimchangyu
 */

#include "HTTP_Client.h"

void insertPortNumber()
{
	do
	{
		printf("포트번호를 입력하세요.\n");
		scanf("%d",&inputPortNumber);

		printf("입력된 포트번호:%d\n", inputPortNumber);
		if (inputPortNumber < 0)
		{
			printf("잘못된 입력입니다. 포트번호를 다시 입력하세요\n");
		}
	}while(inputPortNumber < 0);
	fflush(stdin);
}

void insertIPAddress()
{
	char select;
	do
	{
		printf("IP 주소를 입력하세요\n");
		scanf("%s", inputIPAddress);
		printf("입력된 IP주소:%s\n", inputIPAddress);
		printf("이 IP주소로 통신하시겠습니까? Y/N\n");
		scanf("%*c", &select);
		scanf("%c", &select);
	}while (select != 'Y' && select != 'y');
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int input_message_file(char* input_file)
{
	line_num = 0;
	int line_length = 0; //해당 코드 line_num번째 줄의 길이입니다.
	char getletter = 0;

	FILE* input = fopen(input_file, "r");

	for (line_num = 0; !feof(input); line_num++)
	{
		request_data[line_num] = fgetc(input);
	}
	request_data[line_num] = 0;
	return 1;
}

int main(void)
{
	int sock;
	char message[BUFFERSIZE];
	int str_len, recv_len, recv_cnt;
	struct sockaddr_in serv_adr;
	char select;

	printf("포트번호를 수동으로 입력하시겠습니까? Y/N\n");
	scanf("%c", &select);
	scanf("%*c", &select);
	if (select == 'y' || select == 'Y')
		insertPortNumber();
	else
		inputPortNumber = HTTPNUMBER;

	printf("IP주소를 로컬주소로 설정하시겠습니까? Y/N\n");
	scanf("%c", &select);
	scanf("%*c", &select);
	if (select == 'y' || select == 'Y')
		strcpy(inputIPAddress,localAddress);
	else
		insertIPAddress();


	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(inputIPAddress);
	serv_adr.sin_port=htons(inputPortNumber);

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	input_message_file("input.txt");

	write(sock,request_data,strlen(request_data));
	recv_len=0;
	while(recv_len < line_num-1)
	{
		recv_cnt=read(sock, &message[recv_len], BUFFERSIZE-1);
		if(recv_cnt==-1)
			error_handling("read() error!");
		recv_len+=recv_cnt;
	}

	message[line_num-1]=0;
	printf("Message from server: %s", message);

	close(sock);
	return 0;
}

