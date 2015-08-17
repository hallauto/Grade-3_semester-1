/*
 * ftp_server.h
 *
 *  Created on: 2015. 5. 29.
 *      Author: kimchangyu
 */

#ifndef SRC_FTP_SERVER_H_
#define SRC_FTP_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFERSIZE 1024 //버퍼의 사이즈입니다.

//기본값으로 정해지는 포트번호입니다.
static int FTP_COMMENT_NUMBER = 2121;
//Passive 모드에 쓰일 데이터 소켓 포트 번호입니다.
static int FTP_DATA_NUMBER = 2020;

//서버 기준의 디렉토리입니다. 이 부분은 pwd등으로 디렉토리를 알릴 때 지웁니다.
char directory_server[1024];

//커맨드 입력에 쓰이는 소켓입니다. 이 두 소켓은 연결이 지속됩니다.
int server_command_sock;
int client_command_sock;

//커맨드 입력에 쓰이는 소켓 주소 구조체입니다. 이 두 소켓은 연결이 지속됩니다.
struct sockaddr_in server_command_sock_addr;
struct sockaddr_in client_command_sock_addr;

//데이터 전송에 쓰이는 소켓입니다. 이 두 소켓은 데이터 관련 작업이 끝나면 연결을 종료합니다.
int server_data_sock;
int client_data_sock;

//데이터 전송에 쓰이는 소켓 주소 구조체입니다.
struct sockaddr_in server_data_sock_addr;
struct sockaddr_in client_data_sock_addr;

//write에 넣을 응답 메시지 문자열입니다.
char respond_message_string[1024];

int moveClientDirectory();
int readyDataServer();
int sendRespondMessage();

#endif /* SRC_FTP_SERVER_H_ */
