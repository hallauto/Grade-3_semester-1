/*
 * 화일명 : my_assembler.c 
 * 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
 * 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
 *
 */

/*
 *
 * 프로그램의 헤더를 정의한다. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * my_assembler 프로그램의 의존적인 데이터들이 정의된 곳이다. 
 */

#include "my_assembler.h"
#define _CRT_SECURE_NO_WARNINGS

/* -----------------------------------------------------------------------------------
 * 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
 * 매계 : 실행 파일, 어셈블리 파일 
 * 반환 : 성공 = 0, 실패 = < 0 
 * 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다. 
 *		   또한 중간파일을 생성하지 않는다. 
 * -----------------------------------------------------------------------------------
 */


int main(int args, char *arg[]) 
{
	if (init_my_assembler()< 0)
	{
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n");
		return -1;
	}

	if (assem_pass1() < 0){
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n");
		return -1;
	}
	if (assem_pass2() < 0){
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n");
		return -1;
	}
}

/* -----------------------------------------------------------------------------------
 * 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다. 
 * 매계 : 없음
 * 반환 : 정상종료 = 0 , 에러 발생 = -1
 * 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기 
 *		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
 *		   구현하였다. 
 * -----------------------------------------------------------------------------------
 */

int init_my_assembler(void)
{
	int result ; 

	if((result = init_inst_file("inst.txt")) < 0 )
		return -1 ;
	if((result = init_input_file("input.txt")) < 0 )
		return -1 ; 
	return result ; 
}

/* -----------------------------------------------------------------------------------
 * 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다. 
 *		   패스1에서는..
 *		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
 *		   테이블을 생성한다. 
 * 
 * 매계 : 없음
 * 반환 : 정상 종료 = 0 , 에러 = < 0 
 * 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
 *	  따라서 에러에 대한 검사 루틴을 추가해야 한다. 
 *		
 * -----------------------------------------------------------------------------------
 */

static int assem_pass1(void)
{
	int line_index = 0; //탐색할 줄의 인덱스입니다.
	for (line_index = 0; line_index < line_num; line_index++)
	{
		token_parsing(line_index); //토큰을 파싱할 함수를 호출합니다.
	}
}

/* -----------------------------------------------------------------------------------
 * 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다. 
 *		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다. 
 *		   다음과 같은 작업이 수행되어 진다. 
 *		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다. 
 * 매계 : 없음
 * 반환 : 정상종료 = 0, 에러발생 = < 0 
 * 주의 : 
 * -----------------------------------------------------------------------------------
 */

static int assem_pass2(void)
{
	make_objectcode("output.txt");
}
/* -----------------------------------------------------------------------------------
 * 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을 
 *        생성하는 함수이다. 
 * 매계 : 기계어 목록 파일
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : 기계어 목록파일 형식은 다음과 같다. 
 *	
 *	===============================================================================
 *		   | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
 *	===============================================================================	   
 *		
 * -----------------------------------------------------------------------------------
 */

int init_inst_file(char *inst_file)
{
	FILE* inst_table = fopen(inst_file, "r"); //파일을 읽기전용으로 불러옵니다.
	
	int read_num; //읽고있는 위치입니다.
	int letter_count; //문자를 읽은 횟수입니다.
	int letter_index; //문자열을 받을 때 쓰일 인덱스입니다.
	int column_index; //기계어 목록파일 형식에 맞춰 저장할 때 쓰일 인덱스입니다.
	int row_index; //기계어 목록의 다음줄에 저장할때 쓰일 인덱스입니다.

	char get_letter; //getchar로 한개의 문자를 받았습니다.
	char letter_array[8] = { 0 }; //inst에 저장할 문자열입니다.

	read_num = 0;
	letter_count = 0;
	letter_index = 0;
	column_index = 0;
	row_index = 0;
	while (!feof(inst_table))
	{
		if (row_index > 255)
		{
			printf("명령어 갯수가 저장할 수 있는 수보다 더 많습니다. 256줄 이후의 명령어는 저장하지 못했습니다.\n");
			return -1;
		}
		get_letter=getc(inst_table);
		if (check_letter_for_inst(get_letter))
		{
			letter_array[letter_count] = get_letter;
			letter_count++; //맞는 문자이므로 letter_count를 증가시킵니다.
		}
		else
		{
			inst[row_index][column_index] = calloc(letter_count+1, sizeof(char)); //먼저 길이에 맞는 배열을 동적으로 할당합니다.
			for (letter_index = 0; letter_index < letter_count; letter_index++)
			{
				//토큰을 inst에 저장하면서 letter_array 배열을 초기화합니다.
				inst[row_index][column_index][letter_index] = letter_array[letter_index];
				letter_array[letter_index] = 0; //토큰을 inst에 저장하면서 letter_array 배열을 초기화합니다.
			}
			letter_count = 0; //토큰 한개를 전부 저장했으므로 letter_count를 초기화합니다.
			if (column_index < 3) //토큰 한 개는 한 열을 차지하므로 다음 열로 이동합니다.
				column_index++;
			else //4번째 열까지 저장했다면 다음 줄로 이동합니다.
			{
				row_index++;
				column_index = 0;
			}
		}
	}
	inst_index = row_index;
	return 1;
	//eof까지 전부 읽었습니다.
}

/* -----------------------------------------------------------------------------------
 * 설명 : 어셈블리 할 소스코드를 읽어오는 함수이다. 
 * 매계 : 어셈블리할 소스파일명
 * 반환 : 정상종료 = 0 , 에러 < 0  
 * 주의 : 
 *		
 * -----------------------------------------------------------------------------------
 */


int init_input_file(char *input_file)
{

	int line_length = 0; //해당 코드 line_num번째 줄의 길이입니다.
	int line_index = 0; //인덱스용 변수입니다.
	char getletter = 0;

	FILE* input = fopen(input_file, "r");
	while (!feof(input))
	{
		if (line_num >= 5000)
		{
			printf("5000줄 이상의 소스코드는 읽을 수 없습니다.\n");
			return -1;
		}
		getletter = getc(input);
		if (getletter == '\n')
		{
			line_length += 2; //파일 포인터가 움직인 거리(현재 \n까지 읽고 다음 칸을 가리킵니다.)를 고려해서 line_length를 2 증가시킵니다.
			input_data[line_num] = calloc(line_length, sizeof(char)); //그 줄의 길이만큼 배열을 받기위해 line_length+1길이의 메모리를 동적으로 할당합니다.
			fseek(input, -line_length, SEEK_CUR); //그 줄의 처음부터 읽기위해 파일 위치 지시자를 줄 처음(현재 - line_length)으로 보냅니다.
			fgets(input_data[line_num], line_length, input); //이제 한 줄을 읽습니다.
			line_num++;
			line_length = 0; //다음 줄로 넘어갔으므로, 줄의 길이 값을 나타내는 변수를 초기화합니다.
		}
		else
		{
			line_length++; //읽은 줄의 길이를 1씩 늘려갑니다.
		}
	}
	return 1;
}

/* -----------------------------------------------------------------------------------
 * 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다. 
 *        패스 1로 부터 호출된다. 
 * 매계 : 소스코드의 라인번호  
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다. 
 * -----------------------------------------------------------------------------------
 */

int token_parsing(int index) 
{
	token_table[index] = calloc(1, sizeof(token)); //토큰 단위로 분리된 데이터를 저장할 token 타입 변수를 동적으로 할당합니다.
	int array_index = 0; //탐색하는 줄의 문자열 인덱스입니다.
	int column_length = 0; //탐색하고있는 줄의 열의 길이입니다. '\t'를 뺀 길이입니다.
	int column_index = 0; //열의 멤버 배열의 인덱스입니다.
	int operand_index = 0; //피연산자들의 인덱스입니다.
	int search_result = 0; //연산자열에 입력받은 값이 OP테이블에 있는지 검색한 결과입니다.

	if (input_data[index][0] == '.') //줄의 첫번째 글자가 '.'이면 주석입니다. 토큰 테이블에 저장하지 않고 바로 다음 줄로 넘어갑니다.
	{
		return 0;
	}
	else if (input_data[index][0] == 0) //줄의 첫번째 글자가 0=NULL이면 입력파일에 문제가 있다는 것입니다. 오류입니다.
	{

		return -1;
	}
	else
	{
		//먼저 라벨을 읽습니다.
		while (input_data[index][array_index] != '\t' && input_data[index][array_index] != '\n')
		{
			column_length++;
			array_index++;
		}
		array_index -= column_length; //input_data를 가리키는 array_index는 다시 열 처음으로 되돌립니다.
		token_table[index]->label = calloc(column_length + 1, sizeof(char)); //열의 길이 + 1(\n값을 위한 1)만큼 메모리를 할당합니다.
		for (column_index = 0; column_index < column_length; column_index++, array_index++)
		{
			token_table[index]->label[column_index] = input_data[index][array_index]; //줄을 검색할때에는 열 시작 지점을 알려주는 array_index가 이용됩니다.
		}
		if (input_data[index][array_index] == '\n') //라벨 열이 이 줄의 마지막 열인지 파악합니다. 마지막 열이면 다음 줄로 이동합니다.
		{
			return 1;
		}
			array_index++; //\t를 넘어가서 다음 열을 읽습니다.
		column_length = 0;//다음 열의 길이를 알아내기위해 값을 초기화합니다.

		//이제 연산자를 읽습니다.
		while (input_data[index][array_index] != '\t' && input_data[index][array_index] != '\n')
		{
			column_length++;
			array_index++;
		}
		array_index -= column_length; //input_data를 가리키는 array_index는 다시 열 처음으로 되돌립니다.
		token_table[index]->operator = calloc(column_length + 1, sizeof(char));
		for (column_index = 0; column_index < column_length; column_index++, array_index++)
		{
			token_table[index]->operator[column_index] = input_data[index][array_index]; //줄을 검색할때에는 열 시작 지점을 알려주는 array_index가 이용됩니다.
		}
		
		search_result = search_opcode(token_table[index]->operator);
		printf("%d번 줄의 %d번 명령어 %s의 OP코드는 %s입니다.\n", index, search_result, token_table[index]->operator,inst[search_result][2]);

		if (input_data[index][array_index] == '\n') //연산자 열이 이 줄의 마지막 1열인지 파악합니다. 마지막 열이면 다음 줄로 이동합니다.
		{
			return 0;
		}
			array_index++; //\t를 넘어가서 다음 열을 읽습니다.
		column_length = 0;//다음 열의 길이를 알아내기위해 값을 초기화합니다.

		//이번엔 피연산자를 읽습니다.
		//피연산자는 최대 3개가 존재할 수 있으며, 각각의 역할에 맞춰 위치에 저장합니다.
		//operand[0]=참조할 메모리 주소
		//operand[1]=레지스터
		//operand[2]=루프의 유무
		while (input_data[index][array_index] != '\t' && input_data[index][array_index] != '\n') //주석이 없는 경우도 있으므로 개행문자가 있는지도 파악합니다.
		{
			column_length++;
			array_index++;
		}
		array_index -= column_length; //input_data를 가리키는 array_index는 다시 열 처음으로 되돌립니다.
		token_table[index]->operand[0] = calloc(column_length + 1, sizeof(char));
		for (column_index = 0; column_index < column_length; column_index++, array_index++)
		{
			if (input_data[index][array_index] == ',')
			{
				operand_index++; //,가 있다는건 피연산자가 하나 더 있다는 뜻입니다. 다음 피연산자를 따로 저장하기위해 operand_index를 증가시킵니다.
			}
			else
			{
				token_table[index]->operand[0][column_index] = input_data[index][array_index]; //줄을 검색할때에는 열 시작 지점을 알려주는 array_index가 이용됩니다.
			}
		}
		if (input_data[index][array_index] == '\n') //피연산자 열이 이 줄의 마지막 열인지 파악합니다.
		{
			token_table[index]->comment = calloc(2, sizeof(char));
			token_table[index]->comment[0] = '\t';
			return 0;
		}
			array_index++; //\t를 넘어가서 다음 열을 읽습니다.
		column_length = 0;//다음 열의 길이를 알아내기위해 값을 초기화합니다.
		//마지막으로 주석을 읽습니다.
		while (input_data[index][array_index] != '\n')
		{
			column_length++;
			array_index++;
		}
		if (column_length == 0) //column_length가 0이라는 것은 \t나 \n만 있다는 뜻입니다. 후일 출력의 용이 및 편리한 판별을 위해 '\t'을 저장합니다.
		{
			token_table[index]->comment = calloc(2, sizeof(char));
			token_table[index]->comment[0] = '\t';
		}
		else
		{
			array_index -= column_length; //input_data를 가리키는 array_index는 다시 열 처음으로 되돌립니다.
			token_table[index]->comment = calloc(column_length + 1, sizeof(char));
			for (column_index = 0; column_index < column_length; column_index++, array_index++)
			{
				token_table[index]->comment[column_index] = input_data[index][array_index]; //줄을 검색할때에는 열 시작 지점을 알려주는 array_index가 이용됩니다.
			}
		}
		return 0;
	}
}
/* -----------------------------------------------------------------------------------
 * 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다. 
 * 매계 : 토큰 단위로 구분된 문자열 
 * 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0 
 * 주의 : 
 *		
 * -----------------------------------------------------------------------------------
 */

int search_opcode(char *str) 
{
	int search_index; //inst배열 탐색에 쓰일 인덱스입니다.
	int search_result = 0; //탐색결과를 저장하는 변수입니다. 
	/* add your code here */
	if (str[0] == '+')
	{
		for (search_index = 0; search_index < inst_index; search_index++)
		{
			search_result = strcmp(str + 1, inst[search_index][0]);
			if (search_result == 0)
			{
				return search_index;
			}
		}
	}
	else
	{
		for (search_index = 0; search_index < inst_index; search_index++)
		{
			search_result = strcmp(str, inst[search_index][0]);
			if (search_result == 0)
			{
				return search_index;
			}
		}
	}
	return search_result; //찾지 못한경우입니다.
}
/* -----------------------------------------------------------------------------------
 * 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다. 
 * 매계 : 생성할 오브젝트 파일명 
 * 반환 : 없음
 * 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어 
 *        화면에 출력해준다. 
 *		
 * -----------------------------------------------------------------------------------
 */

void make_objectcode(char *file_name) 
{
	/* add your code here */
	if (file_name == 0)
	{
		file_name = stdout;
	}
	FILE* out_file = fopen(file_name, "w");
	int line_index = 0; //출력할 줄의 인덱스입니다.
	int OPtab_index = 0; //해당 명령어의 OP코드 인덱스입니다.
	for (line_index = 0; line_index < line_num; line_index++)
	{
		if (input_data[line_index][0] == '.') //출력하는 줄이 주석인지 파악합니다.
		{
			fprintf(out_file, "%s\n", input_data[line_index]);
		}
		else
		{
			OPtab_index = search_opcode(token_table[line_index]->operator);
			if (OPtab_index > 0)
			{
				fprintf(out_file, "%s\t%s\t%s\t%s\n", token_table[line_index]->label, token_table[line_index]->operator,token_table[line_index]->operand[0], inst[OPtab_index][2]);
			}
			else
			{
				fprintf(out_file, "%s\t%s\t%s\n", token_table[line_index]->label, token_table[line_index]->operator,token_table[line_index]->operand[0]);
			}
		}
	}
	fclose(out_file);
}

/* -----------------------------------------------------------------------------------
* 설명 : 입력된 문자가 OP코드 및 OP코드의 특징 설명에 이용되는 문자인지 파악한다.
* 매계 : 입력된 문자(char)
* 반환 : int (1 = 쓰이는 문자, 0 = 쓰이지 않는 문자)
* 주의 : 인자로 NULL등의 시스템 값이 들어와도 반환은 정상적으로 한다.(이 경우에는 false가 정상적으로 반환된다.)
*
* -----------------------------------------------------------------------------------
*/

int check_letter_for_inst(char letter)
{
	if ((letter > 47) && (letter < 58)) //먼저 숫자인지 아닌지 파악합니다.
		return 1;
	else if ((letter > 64) && (letter < 91)) //이제 알파벳인지 아닌지 파악합니다.
		return 1;
	else //숫자도, 알파벳 대문자도 아니면 OP코드 설명문이 아닙니다. inst_table에 입력할 필요가 없습니다.
		return 0;
}