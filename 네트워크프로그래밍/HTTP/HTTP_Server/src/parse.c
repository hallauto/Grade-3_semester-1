/*
 * parse.c
 *
 *  Created on: 2015. 5. 22.
 *      Author: kimchangyu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "HTTP_Server.h"
#include "parse.h"

int parse_message(char* input_message)
{
	line_num = 0;
	int line_length = 0;
	int line_index = 0;
	int line_start_index = 0;
	int line_many = 0;
	char getletter = 0;
	char tmp_array[BUFFERSIZE];

	for (line_index = 0; line_index < strlen(input_message); line_index++)
	{
		if (input_message[line_index] == '\n')
		{
			//if (input_message[line_index-1] != '\n') //바로 전에 \n이 있었다는 것은 공백이 존재했다는 뜻입니다. 이는 무시합니다.
			line_num++;
		}
	}
	/*
	for (line_index = 0, line_length=0; line_length < strlen(input_message); line_length++)
	{
		getletter = input_message[line_index];
		if (getletter == '\n')
		{
			request_data[line_many] = calloc(line_length+1, sizeof(char)); //그 줄의 길이만큼 배열을 받기위해 line_length+1길이의 메모리를 동적으로 할당합니다.

			for (line_start_index = line_index - line_length; line_start_index < line_index; line_start_index++)
			{
				request_data[line_start_index - line_index] = input_message[line_start_index];
			}

			line_many++;
			line_length = 0; //다음 줄로 넘어갔으므로, 줄의 길이 값을 나타내는 변수를 초기화합니다.
		}
		else
		{
			line_length++; //읽은 줄의 길이를 1씩 늘려갑니다.
			line_index++;
		}
	}
	*/

	request_data = calloc(line_num+1,sizeof(char*));

	request_data[0] = strdup(strtok(input_message,"\n"));

	for (line_index = 1; line_index < line_num; line_index++)
	{
		strcpy(tmp_array,strtok(NULL,"\n"));
		if (strlen(tmp_array)>0)
			request_data[line_index] = strdup(tmp_array);
	}

	return 1;
}

int parse_data(char** requestData)
{
	token_request value;
	int line_index;
	for (line_index = 0; line_index < line_num; line_index++)
	{
		printf("%s\n",requestData[line_index]);
	}
	//먼저 첫번째 줄을 파싱합니다. 첫번째 줄은 메소드에 상관없이 request line입니다.

	token_request_value.method = strtok(request_data[0]," ");
	value.method = strdup(token_request_value.method);
	token_request_value.operand = strtok(NULL," ");
	value.operand = strdup(token_request_value.operand);
	token_request_value.protocall = strtok(NULL," ");
	value.protocall = strdup(token_request_value.protocall);

	//복사를 위해 임시로 만드는 char배열 입니다.
	char* tmp_str;
	for (line_index = 1; line_index < line_num; line_index++)
	{
		if (request_data[line_index][0] == 0)
			continue;
		tmp_str = strdup(strtok(request_data[line_index]," "));

		if (!strcmp(tmp_str,"Host:"))
		{
			token_request_value.host = strdup(strtok(NULL," "));
			value.host = strdup(token_request_value.host);
		}
		else if (!strcmp(tmp_str,"Content-Type:"))
		{
			token_request_value.content_type = strdup(strtok(NULL," "));
			token_request_value.char_set = strdup(strtok(NULL," "));
			value.content_type = strdup(token_request_value.content_type);
			value.char_set = strdup(token_request_value.char_set);
		}

		//request body를 확인합니다.
		else
		{
			token_request_value.body = strdup(request_data[line_index]);
			value.body = strdup(token_request_value.body);
		}
	}

}


