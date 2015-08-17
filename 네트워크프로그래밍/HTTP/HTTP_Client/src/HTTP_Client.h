/*
 * HTTP_Client.h
 *
 *  Created on: 2015. 5. 22.
 *      Author: kimchangyu
 */

#ifndef SRC_HTTP_CLIENT_H_
#define SRC_HTTP_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int line_num; //입력된 파일의 줄 수입니다.

static int BUFFERSIZE = 1024; //버퍼의 사이즈입니다.
static int HTTPNUMBER = 8080; //HTTP의 포트번호입니다.

char request_data[1024]; //파일에 있는 모든 문자열을 전부 가져온 값입니다.

int inputPortNumber; //포트번호로써 8080외에 입력된 값입니다.
char inputIPAddress[30]; //IP주소로 입력된 값입니다.
char* localAddress = "127.0.0.1";

struct sockaddr_in sock_addr;

void insertPortNumber();
void insertIPAddress();
int input_message_file(char* file_name);

#endif /* SRC_HTTP_CLIENT_H_ */
