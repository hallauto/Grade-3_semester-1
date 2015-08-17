/*
 * server.h
 *
 *  Created on: 2015. 6. 4.
 *      Author: panda
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pwd.h>
#include <time.h>

#define BUF_SIZE 10000
#define CLIENT_MIN 4
#define CLIENT_MAX 8
#define TIME_CHAT 100
#define TIME_VOTE 30

#define STATE_READY "Ready"
#define STATE_DAY "Day"
#define STATE_VOTE "Vote"
#define STATE_NIGHT "Night"

#define TYPE_ACCESS "Access"
#define TYPE_START "Start"
#define TYPE_CHAT "Chat"
#define TYPE_CHOICE "Choice"
#define TYPE_INVITE "Invite"
#define TYPE_EXIT "Exit"

typedef struct {
	char* type;		//메시지 타입
	char* contents;	//메시지 내용
} message;

typedef struct {
	char name[10];//이름
	int start;		//시작 여부
	int voted;		//투표 여부
	int killed;	//죽음 여부
	int mafia;		//마피아 여부
	int sock;		//소켓 fd
	int pid;		//프로세스 pid
} user;

typedef struct {
	int clnt_num;					//메인 채팅방 클라이언트 수
	int clnt_scrt_num;			//비밀 채팅방 클라이언트 수
	char state[10];				//게임 상태
	user clnt[CLIENT_MAX];		//메인 채팅방 클라이언트 유저 배열
	user clnt_scrt[CLIENT_MAX];	//비밀 채팅방 클라이언트 유저 배열
	int mafia_only;				//밤 채팅때 마피아들만 채팅하기 위한 변수
	int start;						//게임 시작시 유저들에게 시민, 마피아를 알려주기 위한 변수
	int voted;						//투표 완료 확인을 위한 변수
	int revote;					//재투표 확인을 위한 변수
	int gend;						//GEnd 메시지를 보낸 상태인지 확인하기 위한 변수
	int handling_usr1;			//handler_usr1 동기화를 위한 변수
	int handling_usr2;			//handler_usr2 동기화를 위한 변수
//	int removing_main;
//	int removing_scrt;
} game_info;

void initialize_mmap();
void error_handling(char *message);
void read_childproc(int sig);
void handler_usr1(int sig);
void handler_usr2(int sig);
void handler_arlam(int sig);
game_info* reset_game();
void reset_clnt();
void reset_clnt_scrt();
int get_name_index(char* username);
game_info* get_mmap();
int get_available(int type);
void remove_clnt(int index);
void remove_clnt_scrt(int index);
message* parse_message(char* buf);
char* make_message(message* msg, int index);
int check_gameover(int index);
#endif /* SERVER_H_ */

