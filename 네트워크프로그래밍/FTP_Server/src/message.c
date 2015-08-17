/*
 * message.c
 *
 *  Created on: 2015. 6. 3.
 *      Author: kimchangyu
 */

#include "message.h"

int check_message_head(char* message_string)
{
	message_head = strtok(message_string,"\t");

	if (message_head == NULL)
	{
		printf("헤더가 비어있습니다.");
	}
	else if (!strcmp(message_head,MESSAGE_PWD)) //PWD메소드인지 확인
	{
		pwd_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_CWD)) //CWD메소드인지 확인
	{
		message_body = strtok(NULL,"\t");
		CWD_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_LIST)) //LIST메소드인지 확인
	{
		LIST_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_NLST)) //NLST메소드인지 확인
	{
		NLST_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_STOR)) //STOR메소드인지 확인
	{
		message_body = strtok(NULL,"\t");
		STOR_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_RETR)) //RETR메소드인지 확인
	{
		message_body = strtok(NULL,"\t");
		RETR_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_DELE)) //DELE메소드인지 확인
	{
		message_body = strtok(NULL,"\t");
		DELE_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_MKD)) //MKD메소드인지 확인
	{
		message_body = strtok(NULL,"\t");
		MKD_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_RMD)) //RMD메소드인지 확인
	{
		message_body = strtok(NULL,"\t");
		RMD_function();
		return 1;
	}
	else if (!strcmp(message_head,MESSAGE_QUIT)) //QUIT메소드인지 확인
	{
		QUIT_function();
		return 0;
	}
}

char* commend_function(char* commend)
{
	//커맨드가 적힌 배열입니다. 이를 이용해서 popen을 하면 됩니다. 이야 신난다.
    char  pszCommand[100];
    strcat(pszCommand,commend);
    FILE        *fp = NULL;
    size_t      readSize = 0;
    //결과 값이 저장될 버퍼입니다.

    //popen함수로 커맨드를 실행합니다.
    fp = popen(pszCommand, "r");
    if(!fp)
    {
        printf("!fp error [%d:%s]\n", errno, strerror(errno));
        return -1;
    }

    // read the result
    readSize = fread( (void*)commend_result, sizeof(char), 1024-1, fp );

    if( readSize == 0 )
    {
        pclose(fp);
        printf("readSize == 0 error [%d:%s]\n", errno, strerror(errno));
        strcpy(respond_message_string,"ERROR");
        return -1;
    }

    pclose(fp);
    //마지막에 null을 넣어서 문자열로 만듭니다.
    commend_result[readSize]=0;

    //결과 출력
    //printf("%s\n", commend_result);
    return 0;
}

//PWD 명령어에서 절대경로 부분이 출력되는 것을 막기위해 절대경로부분을 제거해주는 함수입니다.
char* remove_absolute(char* remove_to, char* remove_string)
{
	int index = 0;
	int to_index = 0;
	for (index = 0; index < strlen(remove_string); index++)
	{
		if (remove_string[index] == remove_to[0])
		{
			for (to_index = 0; to_index < strlen(remove_to); to_index++)
			{
				remove_to[to_index] = remove_to[to_index+1];
			}
		}
	}
}

char* pwd_function()
{
	getcwd(respond_message_string,BUFFERSIZE);
	//루트를 나타낼 슬래시 기호를 이어 넣습니다.
	strcat(respond_message_string,"/");
	//이제 서버의 절대경로 부분을 제거합니다.
	remove_absolute(respond_message_string,directory_server);
}

char* CWD_function()
{
	char new_directory[BUFFERSIZE];

	if (message_body == NULL)
	{
		strcpy(respond_message_string,"ERROR 경로를 입력하세요.");
		return respond_message_string;
	}
	//먼저 접근하려는 주소에 연결되도록 서버의 절대경로를 클라이언트가 입력한 경로 앞에 연결합니다.
	strcpy(new_directory,directory_server);
	strcat(new_directory,"/");
	strcat(new_directory,message_body);
	if (access(new_directory,0))
	{
		mkdir(new_directory, 0755);
	}
	chdir(new_directory);

	getcwd(respond_message_string,BUFFERSIZE);
	remove_absolute(respond_message_string,directory_server);
	return respond_message_string;
}

char* LIST_function()
{
	commend_function("find . -maxdepth 1 -type f");
	if (strlen(commend_result) > 0)
		strcpy(respond_message_string,commend_result);
	else
		strcpy(respond_message_string,"파일이 없습니다.");
	return respond_message_string;
}

char* NLST_function()
{
	commend_function("find . -maxdepth 1 -type d");
	if (strlen(commend_result) > 0)
		strcpy(respond_message_string,commend_result);
	else
		strcpy(respond_message_string,"ERROR");
	return respond_message_string;
}

char* STOR_function()
{
	int read_len;
	char* file_name = message_body;
	char recv_tmp[BUFFERSIZE];
	FILE* recv_file = fopen(file_name,"w");
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

char* RETR_function()
{
	int read_len;
	char data;
	char* file_name = message_body;
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
			memset(respond_message_string,0,BUFFERSIZE);
			//파일에서 BUFFERSIZE만큼 읽고, 이를 서버 데이터 소켓에 입력하면 됩니다.
			read_len = fread(respond_message_string, sizeof(char), BUFFERSIZE, send_file);

			write(client_data_sock,respond_message_string,read_len);
			//fwrite(request_message_string,sizeof(char),BUFFERSIZE,client_data_sock);
			//만약 파일을 끝까지 읽었거나 읽던 중에 오류가 발생했다면 read_len은 0이 됩니다. 이 때에는 전송을 끝냅니다.
			if (read_len <= 0 || read_len == EOF)
			{
				//파일 마지막까지 읽고 전송했습니다. 마지막을 전송했음을 알리기위해 EOF를 전송합니다.
				close(client_data_sock);
				//이제 성공적으로 전송했음을 알리는 메시지 내용을 작성합니다.
				strcpy(respond_message_string,"SUCESS");
				fclose(send_file);
				break;
			}
		}
	}
}

char* DELE_function()
{
	if (remove(message_body) == -1 )
	{
		strcpy(respond_message_string,"ERROR");
	}
	else
	{
		strcpy(respond_message_string,"SUCESS");
	}
}

char* MKD_function()
{
	char new_directory[BUFFERSIZE];

	if (message_body == NULL)
	{
		strcpy(respond_message_string,"ERROR 경로를 입력하세요.");
		return respond_message_string;
	}
	//먼저 접근하려는 주소에 연결되도록 서버의 절대경로를 클라이언트가 입력한 경로 앞에 연결합니다.
	strcpy(new_directory,directory_server);
	strcat(new_directory,"/");
	strcat(new_directory,message_body);
	if (access(new_directory,0))
	{
		mkdir(new_directory, 0755);
	}

	strcpy(respond_message_string,new_directory);
	remove_absolute(respond_message_string,directory_server);
	return respond_message_string;
}

char* RMD_function()
{
	if (rmdir(message_body) == -1)
	{
		strcpy(respond_message_string,"ERROR");
	}
	else
	{
		strcpy(respond_message_string,"SUCESS");
	}
	return respond_message_string;
}

char* QUIT_function()
{
	strcpy(respond_message_string,"BYE Friends!");
}
