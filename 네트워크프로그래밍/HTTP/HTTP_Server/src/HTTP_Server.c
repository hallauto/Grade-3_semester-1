/*
 * HTTP_Server.c
 *
 *  Created on: 2015. 5. 22.
 *      Author: kimchangyu
 */

#include "HTTP_Server.h"
#include "parse.h"

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
}

int main()
{
	int message_length;
	//응답 메시지를 파싱한 데이터를 저장한 변수 token_request_value에서 초기화 해야할 멤버 변수를 초기화 합니다.
	token_request_value.content_type = 0;
	token_request_value.char_set = 0;
	token_request_value.body = 0;

	//응답 메시지는 기본적으로 HTTP/1.1로 시작합니다. 이 점을 고려해서 초기화합니다.
	token_respond_value.protocall = "HTTP/1.1";
	token_respond_value.status_code = 0;
	token_respond_value.content_type = 0;
	token_respond_value.char_set = 0;
	token_respond_value.location=0;
	token_respond_value.body=0;


	char select;
	int serv_sock, clnt_sock;
	char message[BUFFERSIZE];
	int str_len, i;
	int option = 1;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;


	printf("포트번호를 수동으로 입력하시겠습니까? Y/N\n");
	scanf("%c",&select);
	if (select == 'y' || select == 'Y')
		insertPortNumber();
	else
		inputPortNumber = HTTPNUMBER;

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,(void *) option, sizeof(option));
	if(serv_sock==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(inputPortNumber);

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	clnt_adr_sz=sizeof(clnt_adr);

	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	if(clnt_sock==-1)
		error_handling("accept() error");
	else
		printf("Connected client\n");

	while((str_len=read(clnt_sock, message, BUFFERSIZE))!=0) //메시지를 읽어옵니다.
	{
		message_length = str_len;
		write(clnt_sock, message, str_len);
	}
	//문자열을 만들기 위해 마지막에 NULL을 넣습니다.
	message[message_length-1] = 0;

	parse_message(message);
	parse_data(request_data);

	printf("%s %s %s",token_request_value.method,token_request_value.operand,token_request_value.protocall);
	printf("\nHost: %s",token_request_value.host);
	printf("");
	//해당하는 호스트 루트 디렉토리를 만듭니다.
	if (access(token_request_value.host,0))
		mkdir(token_request_value.host, 0755);
	//만든 루트 디렉토리를 작업 경로로 지정합니다. 앞으로 이 디렉토리를 기준으로 작동합니다.
	chdir(token_request_value.host);

	check_data_form(token_request_value);
	make_respond_message();

	close(clnt_sock);

	close(serv_sock);
	return 0;
}

int check_data_form(token_request token)
{
	int return_value; //return value입니다. 이 값이 1이여야 오류가 없는 것입니다.

	//먼저, 프로토콜이 맞는지 확인합니다.
	if (strcmp(token.protocall,"HTTP/1.1"))
	{
		return -1;
	}

	//요청 메소드가 맞는지 확인합니다.

	if (!strcmp(token.method,"GET"))
	{

	}
	else if (!strcmp(token.method,"POST"))
	{
		//return_value = post_method();
	}
	else if (!strcmp(token.method,"PUT"))
	{
		put_method();
	}
	else if (!strcmp(token.method,"DELETE"))
	{
		del_method();
	}
}

void get_method()
{
	if (token_request_value.operand[0] == '/')
	{
		token_request_value.operand[0] = ' ';
		delete_slash(token_request_value.operand);
	}
}

void post_method()
{

}

void put_method()
{
	if (token_request_value.operand[0] == '/')
	{
		token_request_value.operand[0] = ' ';
		delete_slash(token_request_value.operand);
	}

	//먼저 파일이나 디렉토리가 존재하는지 파악합니다. 없다면 파일이 생성될 것이고, 이에 따라 token_respond.status_code에는 201이 들어갑니다.
	if (access(token_request_value.operand,0))
	{
		token_respond_value.status_code = "201 Created";
	}
	else
	{
		token_respond_value.status_code = "200";
	}

	//put은 body가 없으면 디렉토리를 제작(존재하면 의미 없고), body가 있으면 파일을 제작합니다.
	if (token_request_value.body == 0)
	{
		if (token_request_value.operand[0] == '/')
		{
			token_request_value.operand[0] = ' ';
			delete_slash(token_request_value.operand);
		}
		mkdir(token_request_value.host, 0755);
		mkdir(token_request_value.operand, 0755);
	}
	else
	{
		FILE* setFile = fopen(token_request_value.operand,"w+");
		fscanf(setFile,"%s",token_request_value.body);
	}
	if (token_request_value.content_type != 0)
		token_respond_value.content_type = strdup(token_request_value.content_type);
	else
		token_request_value.content_type = 0;
	if (token_request_value.char_set != 0)
		token_respond_value.char_set = strdup(token_request_value.content_type);
	else
		token_request_value.content_type = 0;
	if (token_request_value.body != 0)
		token_respond_value.body = strdup(token_request_value.body);
	else
		token_request_value.body = 0;
}

void del_method()
{
	//먼저 파일이나 디렉토리가 존재하는지 파악합니다. 존재하지않는다면 오류이므로, 이에 따라 token_respond.status_code에는 404가 저장됩니다.
	if (token_request_value.operand[0] == '/')
	{
		token_request_value.operand[0] = ' ';
		delete_slash(token_request_value.operand);
	}
	if (access(token_request_value.operand,0))
	{
		token_respond_value.status_code = "404";
		token_respond_value.content_type = 0;
		token_respond_value.char_set = 0;
		token_respond_value.body = 0;
	}
	else
	{
		remove(token_request_value.operand);
		rmdir(token_request_value.operand);
	}
}

void make_respond_message()
{
	char tmp_string[BUFFERSIZE];

	strcpy(tmp_string,token_respond_value.protocall);
	strcat(tmp_string,token_respond_value.status_code);
	strcat(tmp_string,"\n");
	if (token_request_value.content_type != 0)
		strcat(tmp_string,token_respond_value.content_type);
	if (token_request_value.char_set != 0)
	{
		strcat(tmp_string,token_respond_value.char_set);
		strcat(tmp_string,"\n");
	}
	if (token_respond_value.location != 0)
	{
		strcat(tmp_string,token_respond_value.location);
		strcat(tmp_string,"\n");
	}
	if (token_respond_value.body != 0)
	{
		strcat(tmp_string,"\n");
		strcat(tmp_string,token_respond_value.body);
	}
}

//문자열 맨 앞에 있는 /를 지워주는 함수입니다.
void delete_slash(char* delete_string)
{
	int string_index;
	for (string_index = 0; string_index < strlen(delete_string); string_index++)
	{
		delete_string[string_index] = delete_string[string_index+1];
	}
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
