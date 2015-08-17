/* 
 * my_assembler 함수를 위한 변수 선언 및 매크로를 담고 있는 헤더 파일이다. 
 * 
 */
#ifndef MY_ASSEMBLER //중복 참조를 막기위한 매크로입니다.

#define MY_ASSEMBLER
#define MAX_INST 256
#define MAX_LINES 5000

#define MAX_COLUMNS 4
#define MAX_OPERAND 3
#define _CRT_SECURE_NO_WARNINGS

/* 
 * 기계어 목록 파일로 부터 정보를 받아와 생성하는 기계어 변환 테이블이다.
 * 해당 기계어의 정보를 토큰으로 나눠 기록하고 있다. 
 */
char *inst[MAX_INST][MAX_COLUMNS]; //MAX_INST에 OP코드 번호순서대로 넣고, MAX_COLUMNS 각각에 이름, 형식, OP코드, 피연산자 갯수를 작성
int inst_index;

/*
 * 어셈블리 할 소스코드를 토큰 단위로 관리하는 테이블이다. 
 * 관리 정보는 소스 라인 단위로 관리되어진다.
 */
char *input_data[MAX_LINES];
static int line_num ;

int label_num ; 

struct token_unit {
	char *label; //해당 소스 라인의 라벨입니다.
	char *operator; //해당 소스 라인의 연산자 = OP코드 입니다.
	char *operand[MAX_OPERAND]; //해당 소스 라인의 피연산자 = 참조할 메모리 주소, 레지스터, 루프의 유무 등을 표현
	char *comment; //주석입니다.
};

typedef struct token_unit token ; 
token *token_table[MAX_LINES] ; 

int init_my_assembler(void);
static int assem_pass1(void); 
static int assem_pass2(void);
int init_inst_file(char *inst_file);
int init_input_file(char *input_file);
int token_parsing(int index);
int search_opcode(char *str);
void make_objectcode(char *file_name);
int check_letter_for_inst(char letter);

#endif