/*
 * HTTP_Server.h
 *
 *  Created on: 2015. 5. 22.
 *      Author: kimchangyu
 */

#ifndef SRC_HTTP_SERVER_H_
#define SRC_HTTP_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static int BUFFERSIZE = 1024; //버퍼의 사이즈입니다.
static int HTTPNUMBER = 8080; //기본값으로 정해지는 포트번호입니다.

int inputPortNumber; //포트번호로써 8080외에 입력된 값입니다.

struct sockaddr_in server_sock_addr;
struct sockaddr_in client_sock_addr;

/*
 * 받은 메시지를 파싱한 후에 나오는 token_request입니다.
 */
struct token_request
{
	char* method; //요청 메시지의 메소드입니다.
	char* operand; //요청 메시지의 operand입니다.
	char* protocall; //요청 메시지의 프로토콜입니다. 이 프로토콜 값이 서버와 같아야 정확한 메시지 입니다.
	char* host; //요청 메시지의 호스트 루트 디렉토리입니다.
	char* content_type; //요청 메시지의 body의 형식입니다.
	char* char_set; //요청메시지의 문자 인코딩 형식입니다.

	char* body; //요청메시지의 body입니다.
};

typedef struct token_request token_request;

token_request token_request_value;

struct token_respond
{
	char* protocall;
	char* status_code; //응답 메시지의 상태 코드입니다. 오류 알림도 여기에 저장되며, 오류의 경우 입력과 동시에 메시지의 body를 수정합니다.
	char* content_type; //응답 메시지의 문자 인코딩 형식입니다.
	char* char_set; //응답 메시지의 문자 인코딩 형식입니다.
	char* location; //요청 메시지가 처리된 주소입니다.
	char* body; //응답 메시지의 body입니다.
};

typedef struct token_respond token_respond;

token_respond token_respond_value;

//write에 넣을 응답 메시지 문자열입니다. 위의 token_respond_value의 멤버변수를 이용해서 제작합니다.
char* respond_message_string;


int check_data_form(token_request token);
//GET메소드를 처리하는 함수입니다. 오류가 있으면 이를 알리는 메시지를 작성하고, 아니면 정상적인 메시지를 작성합니다.
void get_method();
//POST메소드를 처리하는 함수입니다.
void post_method();
//PUT메소드를 처리하는 함수입니다. 오류가 있으면 이를 알리는 메시지를 작성하고, 아니면 정상적인 메시지를 작성합니다.
void put_method();
//DELTE메소드를 처리하는 함수입니다. 오류가 있으면 이를 알리는 메시지를 작성하고, 아니면 정상적인 메시지를 작성합니다.
void del_method();

void insertPortNumber();
void delete_slash(char* delete_string);
void error_handling(char *message);

#endif /* SRC_HTTP_SERVER_H_ */
