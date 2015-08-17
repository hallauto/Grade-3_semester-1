/* 
 * my_assembler �Լ��� ���� ���� ���� �� ��ũ�θ� ��� �ִ� ��� �����̴�. 
 * 
 */
#ifndef MY_ASSEMBLER //�ߺ� ������ �������� ��ũ���Դϴ�.

#define MY_ASSEMBLER
#define MAX_INST 256
#define MAX_LINES 5000

#define MAX_COLUMNS 4
#define MAX_OPERAND 3
#define _CRT_SECURE_NO_WARNINGS

/* 
 * ���� ��� ���Ϸ� ���� ������ �޾ƿ� �����ϴ� ���� ��ȯ ���̺��̴�.
 * �ش� ������ ������ ��ū���� ���� ����ϰ� �ִ�. 
 */
char *inst[MAX_INST][MAX_COLUMNS]; //MAX_INST�� OP�ڵ� ��ȣ������� �ְ�, MAX_COLUMNS ������ �̸�, ����, OP�ڵ�, �ǿ����� ������ �ۼ�
int inst_index;

/*
 * ����� �� �ҽ��ڵ带 ��ū ������ �����ϴ� ���̺��̴�. 
 * ���� ������ �ҽ� ���� ������ �����Ǿ�����.
 */
char *input_data[MAX_LINES];
static int line_num ;

int label_num ; 

struct token_unit {
	char *label; //�ش� �ҽ� ������ ���Դϴ�.
	char *operator; //�ش� �ҽ� ������ ������ = OP�ڵ� �Դϴ�.
	char *operand[MAX_OPERAND]; //�ش� �ҽ� ������ �ǿ����� = ������ �޸� �ּ�, ��������, ������ ���� ���� ǥ��
	char *comment; //�ּ��Դϴ�.
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