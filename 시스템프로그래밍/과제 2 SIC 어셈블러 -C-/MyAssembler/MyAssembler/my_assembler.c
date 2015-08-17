/*
 * ȭ�ϸ� : my_assembler.c 
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
 *
 */

/*
 *
 * ���α׷��� ����� �����Ѵ�. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * my_assembler ���α׷��� �������� �����͵��� ���ǵ� ���̴�. 
 */

#include "my_assembler.h"
#define _CRT_SECURE_NO_WARNINGS

/* -----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ����� ���� 
 * ��ȯ : ���� = 0, ���� = < 0 
 * ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�. 
 *		   ���� �߰������� �������� �ʴ´�. 
 * -----------------------------------------------------------------------------------
 */


int main(int args, char *arg[]) 
{
	if (init_my_assembler()< 0)
	{
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
		return -1;
	}

	if (assem_pass1() < 0){
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}
	if (assem_pass2() < 0){
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}
}

/* -----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�. 
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ� 
 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
 *		   �����Ͽ���. 
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
 * ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�. 
 *		   �н�1������..
 *		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
 *		   ���̺��� �����Ѵ�. 
 * 
 * �Ű� : ����
 * ��ȯ : ���� ���� = 0 , ���� = < 0 
 * ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
 *	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�. 
 *		
 * -----------------------------------------------------------------------------------
 */

static int assem_pass1(void)
{
	int line_index = 0; //Ž���� ���� �ε����Դϴ�.
	for (line_index = 0; line_index < line_num; line_index++)
	{
		token_parsing(line_index); //��ū�� �Ľ��� �Լ��� ȣ���մϴ�.
	}
}

/* -----------------------------------------------------------------------------------
 * ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�. 
 *		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�. 
 *		   ������ ���� �۾��� ����Ǿ� ����. 
 *		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�. 
 * �Ű� : ����
 * ��ȯ : �������� = 0, �����߻� = < 0 
 * ���� : 
 * -----------------------------------------------------------------------------------
 */

static int assem_pass2(void)
{
	make_objectcode("output.txt");
}
/* -----------------------------------------------------------------------------------
 * ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)�� 
 *        �����ϴ� �Լ��̴�. 
 * �Ű� : ���� ��� ����
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : ���� ������� ������ ������ ����. 
 *	
 *	===============================================================================
 *		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
 *	===============================================================================	   
 *		
 * -----------------------------------------------------------------------------------
 */

int init_inst_file(char *inst_file)
{
	FILE* inst_table = fopen(inst_file, "r"); //������ �б��������� �ҷ��ɴϴ�.
	
	int read_num; //�а��ִ� ��ġ�Դϴ�.
	int letter_count; //���ڸ� ���� Ƚ���Դϴ�.
	int letter_index; //���ڿ��� ���� �� ���� �ε����Դϴ�.
	int column_index; //���� ������� ���Ŀ� ���� ������ �� ���� �ε����Դϴ�.
	int row_index; //���� ����� �����ٿ� �����Ҷ� ���� �ε����Դϴ�.

	char get_letter; //getchar�� �Ѱ��� ���ڸ� �޾ҽ��ϴ�.
	char letter_array[8] = { 0 }; //inst�� ������ ���ڿ��Դϴ�.

	read_num = 0;
	letter_count = 0;
	letter_index = 0;
	column_index = 0;
	row_index = 0;
	while (!feof(inst_table))
	{
		if (row_index > 255)
		{
			printf("��ɾ� ������ ������ �� �ִ� ������ �� �����ϴ�. 256�� ������ ��ɾ�� �������� ���߽��ϴ�.\n");
			return -1;
		}
		get_letter=getc(inst_table);
		if (check_letter_for_inst(get_letter))
		{
			letter_array[letter_count] = get_letter;
			letter_count++; //�´� �����̹Ƿ� letter_count�� ������ŵ�ϴ�.
		}
		else
		{
			inst[row_index][column_index] = calloc(letter_count+1, sizeof(char)); //���� ���̿� �´� �迭�� �������� �Ҵ��մϴ�.
			for (letter_index = 0; letter_index < letter_count; letter_index++)
			{
				//��ū�� inst�� �����ϸ鼭 letter_array �迭�� �ʱ�ȭ�մϴ�.
				inst[row_index][column_index][letter_index] = letter_array[letter_index];
				letter_array[letter_index] = 0; //��ū�� inst�� �����ϸ鼭 letter_array �迭�� �ʱ�ȭ�մϴ�.
			}
			letter_count = 0; //��ū �Ѱ��� ���� ���������Ƿ� letter_count�� �ʱ�ȭ�մϴ�.
			if (column_index < 3) //��ū �� ���� �� ���� �����ϹǷ� ���� ���� �̵��մϴ�.
				column_index++;
			else //4��° ������ �����ߴٸ� ���� �ٷ� �̵��մϴ�.
			{
				row_index++;
				column_index = 0;
			}
		}
	}
	inst_index = row_index;
	return 1;
	//eof���� ���� �о����ϴ�.
}

/* -----------------------------------------------------------------------------------
 * ���� : ����� �� �ҽ��ڵ带 �о���� �Լ��̴�. 
 * �Ű� : ������� �ҽ����ϸ�
 * ��ȯ : �������� = 0 , ���� < 0  
 * ���� : 
 *		
 * -----------------------------------------------------------------------------------
 */


int init_input_file(char *input_file)
{

	int line_length = 0; //�ش� �ڵ� line_num��° ���� �����Դϴ�.
	int line_index = 0; //�ε����� �����Դϴ�.
	char getletter = 0;

	FILE* input = fopen(input_file, "r");
	while (!feof(input))
	{
		if (line_num >= 5000)
		{
			printf("5000�� �̻��� �ҽ��ڵ�� ���� �� �����ϴ�.\n");
			return -1;
		}
		getletter = getc(input);
		if (getletter == '\n')
		{
			line_length += 2; //���� �����Ͱ� ������ �Ÿ�(���� \n���� �а� ���� ĭ�� ����ŵ�ϴ�.)�� ����ؼ� line_length�� 2 ������ŵ�ϴ�.
			input_data[line_num] = calloc(line_length, sizeof(char)); //�� ���� ���̸�ŭ �迭�� �ޱ����� line_length+1������ �޸𸮸� �������� �Ҵ��մϴ�.
			fseek(input, -line_length, SEEK_CUR); //�� ���� ó������ �б����� ���� ��ġ �����ڸ� �� ó��(���� - line_length)���� �����ϴ�.
			fgets(input_data[line_num], line_length, input); //���� �� ���� �н��ϴ�.
			line_num++;
			line_length = 0; //���� �ٷ� �Ѿ���Ƿ�, ���� ���� ���� ��Ÿ���� ������ �ʱ�ȭ�մϴ�.
		}
		else
		{
			line_length++; //���� ���� ���̸� 1�� �÷����ϴ�.
		}
	}
	return 1;
}

/* -----------------------------------------------------------------------------------
 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�. 
 *        �н� 1�� ���� ȣ��ȴ�. 
 * �Ű� : �ҽ��ڵ��� ���ι�ȣ  
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�. 
 * -----------------------------------------------------------------------------------
 */

int token_parsing(int index) 
{
	token_table[index] = calloc(1, sizeof(token)); //��ū ������ �и��� �����͸� ������ token Ÿ�� ������ �������� �Ҵ��մϴ�.
	int array_index = 0; //Ž���ϴ� ���� ���ڿ� �ε����Դϴ�.
	int column_length = 0; //Ž���ϰ��ִ� ���� ���� �����Դϴ�. '\t'�� �� �����Դϴ�.
	int column_index = 0; //���� ��� �迭�� �ε����Դϴ�.
	int operand_index = 0; //�ǿ����ڵ��� �ε����Դϴ�.
	int search_result = 0; //�����ڿ��� �Է¹��� ���� OP���̺� �ִ��� �˻��� ����Դϴ�.

	if (input_data[index][0] == '.') //���� ù��° ���ڰ� '.'�̸� �ּ��Դϴ�. ��ū ���̺� �������� �ʰ� �ٷ� ���� �ٷ� �Ѿ�ϴ�.
	{
		return 0;
	}
	else if (input_data[index][0] == 0) //���� ù��° ���ڰ� 0=NULL�̸� �Է����Ͽ� ������ �ִٴ� ���Դϴ�. �����Դϴ�.
	{

		return -1;
	}
	else
	{
		//���� ���� �н��ϴ�.
		while (input_data[index][array_index] != '\t' && input_data[index][array_index] != '\n')
		{
			column_length++;
			array_index++;
		}
		array_index -= column_length; //input_data�� ����Ű�� array_index�� �ٽ� �� ó������ �ǵ����ϴ�.
		token_table[index]->label = calloc(column_length + 1, sizeof(char)); //���� ���� + 1(\n���� ���� 1)��ŭ �޸𸮸� �Ҵ��մϴ�.
		for (column_index = 0; column_index < column_length; column_index++, array_index++)
		{
			token_table[index]->label[column_index] = input_data[index][array_index]; //���� �˻��Ҷ����� �� ���� ������ �˷��ִ� array_index�� �̿�˴ϴ�.
		}
		if (input_data[index][array_index] == '\n') //�� ���� �� ���� ������ ������ �ľ��մϴ�. ������ ���̸� ���� �ٷ� �̵��մϴ�.
		{
			return 1;
		}
			array_index++; //\t�� �Ѿ�� ���� ���� �н��ϴ�.
		column_length = 0;//���� ���� ���̸� �˾Ƴ������� ���� �ʱ�ȭ�մϴ�.

		//���� �����ڸ� �н��ϴ�.
		while (input_data[index][array_index] != '\t' && input_data[index][array_index] != '\n')
		{
			column_length++;
			array_index++;
		}
		array_index -= column_length; //input_data�� ����Ű�� array_index�� �ٽ� �� ó������ �ǵ����ϴ�.
		token_table[index]->operator = calloc(column_length + 1, sizeof(char));
		for (column_index = 0; column_index < column_length; column_index++, array_index++)
		{
			token_table[index]->operator[column_index] = input_data[index][array_index]; //���� �˻��Ҷ����� �� ���� ������ �˷��ִ� array_index�� �̿�˴ϴ�.
		}
		
		search_result = search_opcode(token_table[index]->operator);
		printf("%d�� ���� %d�� ��ɾ� %s�� OP�ڵ�� %s�Դϴ�.\n", index, search_result, token_table[index]->operator,inst[search_result][2]);

		if (input_data[index][array_index] == '\n') //������ ���� �� ���� ������ 1������ �ľ��մϴ�. ������ ���̸� ���� �ٷ� �̵��մϴ�.
		{
			return 0;
		}
			array_index++; //\t�� �Ѿ�� ���� ���� �н��ϴ�.
		column_length = 0;//���� ���� ���̸� �˾Ƴ������� ���� �ʱ�ȭ�մϴ�.

		//�̹��� �ǿ����ڸ� �н��ϴ�.
		//�ǿ����ڴ� �ִ� 3���� ������ �� ������, ������ ���ҿ� ���� ��ġ�� �����մϴ�.
		//operand[0]=������ �޸� �ּ�
		//operand[1]=��������
		//operand[2]=������ ����
		while (input_data[index][array_index] != '\t' && input_data[index][array_index] != '\n') //�ּ��� ���� ��쵵 �����Ƿ� ���๮�ڰ� �ִ����� �ľ��մϴ�.
		{
			column_length++;
			array_index++;
		}
		array_index -= column_length; //input_data�� ����Ű�� array_index�� �ٽ� �� ó������ �ǵ����ϴ�.
		token_table[index]->operand[0] = calloc(column_length + 1, sizeof(char));
		for (column_index = 0; column_index < column_length; column_index++, array_index++)
		{
			if (input_data[index][array_index] == ',')
			{
				operand_index++; //,�� �ִٴ°� �ǿ����ڰ� �ϳ� �� �ִٴ� ���Դϴ�. ���� �ǿ����ڸ� ���� �����ϱ����� operand_index�� ������ŵ�ϴ�.
			}
			else
			{
				token_table[index]->operand[0][column_index] = input_data[index][array_index]; //���� �˻��Ҷ����� �� ���� ������ �˷��ִ� array_index�� �̿�˴ϴ�.
			}
		}
		if (input_data[index][array_index] == '\n') //�ǿ����� ���� �� ���� ������ ������ �ľ��մϴ�.
		{
			token_table[index]->comment = calloc(2, sizeof(char));
			token_table[index]->comment[0] = '\t';
			return 0;
		}
			array_index++; //\t�� �Ѿ�� ���� ���� �н��ϴ�.
		column_length = 0;//���� ���� ���̸� �˾Ƴ������� ���� �ʱ�ȭ�մϴ�.
		//���������� �ּ��� �н��ϴ�.
		while (input_data[index][array_index] != '\n')
		{
			column_length++;
			array_index++;
		}
		if (column_length == 0) //column_length�� 0�̶�� ���� \t�� \n�� �ִٴ� ���Դϴ�. ���� ����� ���� �� ���� �Ǻ��� ���� '\t'�� �����մϴ�.
		{
			token_table[index]->comment = calloc(2, sizeof(char));
			token_table[index]->comment[0] = '\t';
		}
		else
		{
			array_index -= column_length; //input_data�� ����Ű�� array_index�� �ٽ� �� ó������ �ǵ����ϴ�.
			token_table[index]->comment = calloc(column_length + 1, sizeof(char));
			for (column_index = 0; column_index < column_length; column_index++, array_index++)
			{
				token_table[index]->comment[column_index] = input_data[index][array_index]; //���� �˻��Ҷ����� �� ���� ������ �˷��ִ� array_index�� �̿�˴ϴ�.
			}
		}
		return 0;
	}
}
/* -----------------------------------------------------------------------------------
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�. 
 * �Ű� : ��ū ������ ���е� ���ڿ� 
 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0 
 * ���� : 
 *		
 * -----------------------------------------------------------------------------------
 */

int search_opcode(char *str) 
{
	int search_index; //inst�迭 Ž���� ���� �ε����Դϴ�.
	int search_result = 0; //Ž������� �����ϴ� �����Դϴ�. 
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
	return search_result; //ã�� ���Ѱ���Դϴ�.
}
/* -----------------------------------------------------------------------------------
 * ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�. 
 * �Ű� : ������ ������Ʈ ���ϸ� 
 * ��ȯ : ����
 * ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������ 
 *        ȭ�鿡 ������ش�. 
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
	int line_index = 0; //����� ���� �ε����Դϴ�.
	int OPtab_index = 0; //�ش� ��ɾ��� OP�ڵ� �ε����Դϴ�.
	for (line_index = 0; line_index < line_num; line_index++)
	{
		if (input_data[line_index][0] == '.') //����ϴ� ���� �ּ����� �ľ��մϴ�.
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
* ���� : �Էµ� ���ڰ� OP�ڵ� �� OP�ڵ��� Ư¡ ���� �̿�Ǵ� �������� �ľ��Ѵ�.
* �Ű� : �Էµ� ����(char)
* ��ȯ : int (1 = ���̴� ����, 0 = ������ �ʴ� ����)
* ���� : ���ڷ� NULL���� �ý��� ���� ���͵� ��ȯ�� ���������� �Ѵ�.(�� ��쿡�� false�� ���������� ��ȯ�ȴ�.)
*
* -----------------------------------------------------------------------------------
*/

int check_letter_for_inst(char letter)
{
	if ((letter > 47) && (letter < 58)) //���� �������� �ƴ��� �ľ��մϴ�.
		return 1;
	else if ((letter > 64) && (letter < 91)) //���� ���ĺ����� �ƴ��� �ľ��մϴ�.
		return 1;
	else //���ڵ�, ���ĺ� �빮�ڵ� �ƴϸ� OP�ڵ� ������ �ƴմϴ�. inst_table�� �Է��� �ʿ䰡 �����ϴ�.
		return 0;
}