/*
 * FTP_Client.h
 *
 *  Created on: 2015. 5. 29.
 *      Author: kimchangyu
 */

#ifndef SRC_FTP_CLIENT_H_
#define SRC_FTP_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFERSIZE 1024

int line_num;

//static int BUFFERSIZE = 1024; //send-que의 크기입니다.
static int FTP_COMMANT_NUMBER = 2121; //FTP Comment 포트 번호입니다.

char read_message_string[BUFFERSIZE]; //FTP 서버에서 보낸 메시지가 담긴 배열입니다.
char request_message_string[BUFFERSIZE]; //FTP 서버에 보낼 메시지가 담긴 배열입니다.

char* local_address = "127.0.0.1"; //이 프로그램은 기본적으로 로컬 주소를 이용합니다.

int data_port_number; //FTP 서버의 데이터 포트 번호입니다.
//커맨드 입력에 쓰이는 소켓입니다. 이 소켓은 연결이 지속됩니다.
int client_comment_sock;
//데이터 전송에 쓰이는 소켓입니다. 이 소켓은 데이터 관련 작업이 끝나면 연결을 종료합니다.
int client_data_sock;

//커맨드 입력에 쓰이는 소켓 주소 구조체입니다. 이 소켓은 연결이 지속됩니다.
struct sockaddr_in client_comment_sock_addr;

//데이터 전송에 쓰이는 소켓 주소 구조체입니다. 이 소켓은 데이터 관련 작업이 끝나면 연결을 종료합니다.
struct sockaddr_in client_data_sock_addr;

//FTP의 데이터 전송을 위한 소켓을 연결하는 함수입니다.
int access_data_socket();
//보낼 메시지를 작성하는 함수입니다.
int sending_message_make();
//STOR 명령어는 파일을 전송하는 명령어로, 다른 명령어들보다 복잡한 기능을 요구합니다. 이를 구현한 함수입니다.
int STOR_function();

#endif /* SRC_FTP_CLIENT_H_ */
