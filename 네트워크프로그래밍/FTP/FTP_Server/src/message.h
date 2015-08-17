/*
 * message.h
 *
 *  Created on: 2015. 6. 3.
 *      Author: kimchangyu
 */

#ifndef SRC_MESSAGE_H_
#define SRC_MESSAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "FTP_Server.h"

//각각의 메시지의 헤더부분입니다. 토큰으로 나눈 메시지를 이 변수들과 비교하고 알맞게 파싱합니다.
extern char* MESSAGE_PWD = "PWD";
extern char* MESSAGE_CWD = "CWD";
extern char* MESSAGE_LIST = "LIST";
extern char* MESSAGE_NLST = "NLST";
extern char* MESSAGE_STOR = "STOR";
extern char* MESSAGE_RETR = "RETR";
extern char* MESSAGE_DELE = "DELE";
extern char* MESSAGE_MKD = "MKD";
extern char* MESSAGE_RMD = "RMD";
extern char* MESSAGE_QUIT = "QUIT";

extern char* CRLF = "\r\n";

char* message_head = "";
char* message_body = "";
//리눅스 명령어를 쓴 후, 반환된 결과값이 담긴 배열입니다.
char commend_result[1024];

//메시지의 헤더(명령어)를 확인하고 그에 맞는 함수를 호출합니다.
int check_message_head(char* message_string);

//리눅스 커맨드를 작동하고 그 결과값을 문자열로 반환하는 함수입니다.
char* commend_function(char* commend);

//PWD 명령어에서 절대경로 부분이 출력되는 것을 막기위해 절대경로부분을 제거해주는 함수입니다.
char* remove_absolute(char* remove_to, char* remove_string);
//각각의 메시지에 해당하는 기능들을 수행하는 함수입니다.
char* pwd_function();
char* CWD_function();
char* LIST_function();
char* NLST_function();
char* STOR_function();
char* RETR_function();
char* DELE_function();
char* MKD_function();
char* RMD_function();
char* QUIT_function();


#endif /* SRC_MESSAGE_H_ */
