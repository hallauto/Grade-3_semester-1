/*
 * parse.h
 *
 *  Created on: 2015. 5. 22.
 *      Author: kimchangyu
 */

#ifndef SRC_PARSE_H_
#define SRC_PARSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int line_num; //입력된 파일의 줄 수입니다.

char** request_data; //파일에 있는 모든 문자열을 전부 가져온 값입니다.

//메시지를 줄 단위로 나눕니다.
int parse_message(char* input_message);

/*
 * 줄단위로 나뉜 입력된 메시지를 파싱해서 token_request_value에 저장합니다.
 */
int parse_data(char** insert_string);

#endif /* SRC_PARSE_H_ */
