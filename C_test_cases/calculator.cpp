/*
����������ʽ��ֵ
�����㷨��
	�Ƚ��ַ�������ɲ�����Ԫ�������������������������ջ������������
�����㷨����м��㣬���ó������
*/

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<stdlib.h>

const int max_length = 100;//���ʽ��󳤶� 
struct Unit
{
	//���������Ԫ 
	int    flag;// 1:������ 0:������ -1:���� 
	char   op;//������ 
	double num;//������
};

int Check(char Input_array[]);
void Evaluate(); //�ȵ���Convert������Ԫ�����ٵ���Calculate��������������� 
int  Convert(struct Unit Unit_arry[], char Input_array[]);//���ַ�������ɲ�����Ԫ 
int  Isop(char ch);//�жϺϷ��ַ���+ - * / ( ) =��
int  Compare(char op1, char op2);//�������������ȼ��Ƚ� 
double Calculate(struct Unit Unit_arry[], int Unit_len, int& flag);//��ջ������ʽ��� 
double Compute(double x, double y, char op);//�������� 

int main()
{
	Evaluate();
	return 0;
}

int Check(char Input_array[])
{
	//����Ƿ��зǷ��ַ�������1��ʾ���Ϸ���0��ʾ�Ϸ� 
	int len = strlen(Input_array), i;
	for (i = 0; i < len; i++)
	{
		if (!Isop(Input_array[i]) && Input_array[i] != '.' && !isdigit(Input_array[i]))
			return 1;
		if (isdigit(Input_array[i]))
		{
			int num_len = 0, Cur_positoin = i + 1;
			while (isdigit(Input_array[Cur_positoin]) || Input_array[Cur_positoin] == '.')
			{
				num_len++;
				Cur_positoin++;
			}
			if (num_len >= 16)//15λ��Ч���� 
				return 1;
		}
	}
	return 0;
}

void Evaluate()
{
	//�ȵ���Convert�������ַ���������Ԫ�����ٵ���Calculate��������������� 
	char Input_array[max_length];
	int flag = 0; 
	struct Unit Unit_arry[max_length];

	printf("����������������ʽ��\n");
	gets_s(Input_array);
	flag = Check(Input_array);
	if (flag)
		printf("�ñ��ʽ���Ϸ���\n");
	else
	{
		int Unit_len = Convert(Unit_arry, Input_array);
		double        ans = Calculate(Unit_arry, Unit_len, flag);
		if (flag)
		{
			printf("������Ϊ��\n");
			printf("%s%s%lf\n", Input_array,"=", ans);
		}
		else
			printf("�ñ��ʽ���Ϸ���\n");
	}
	system("pause");
}

int  Convert(struct Unit Unit_arry[], char Input_array[])
{
	//���ַ���������Ԫ�� 
	int len = strlen(Input_array);
	int i, Unit_len = 0;
	for (i = 0; i < len; i++)
	{
		if (Isop(Input_array[i]))//������ 
		{
			Unit_arry[Unit_len].flag = 0;
			Unit_arry[Unit_len++].op = Input_array[i];
		}
		else//������ 
		{
			Unit_arry[Unit_len].flag = 1;
			char temp[max_length];
			int k = 0;
			for (; isdigit(Input_array[i]) || Input_array[i] == '.'; i++)
			{
				temp[k++] = Input_array[i];
			}
			i--;
			temp[k] = '\0';
			Unit_arry[Unit_len].num = atof(temp);

			//���� 
			if (Unit_len == 1 && Unit_arry[Unit_len - 1].flag == 0
				&& Unit_arry[Unit_len - 1].op == '-')
			{
				Unit_arry[Unit_len - 1].flag = -1;
				Unit_arry[Unit_len].num *= -1;
			}// -x
			if (Unit_len >= 2 && Unit_arry[Unit_len - 1].flag == 0
				&& Unit_arry[Unit_len - 1].op == '-' && Unit_arry[Unit_len - 2].flag == 0
				&& Unit_arry[Unit_len - 2].op != ')')
			{
				Unit_arry[Unit_len - 1].flag = -1;
				Unit_arry[Unit_len].num *= -1;
			}// )-x

			//���� 
			if (Unit_len == 1 && Unit_arry[Unit_len - 1].flag == 0
				&& Unit_arry[Unit_len - 1].op == '+')
			{
				Unit_arry[Unit_len - 1].flag = -1;
			}// +x 
			if (Unit_len >= 2 && Unit_arry[Unit_len - 1].flag == 0
				&& Unit_arry[Unit_len - 1].op == '+' && Unit_arry[Unit_len - 2].flag == 0
				&& Unit_arry[Unit_len - 2].op != ')')
			{
				Unit_arry[Unit_len - 1].flag = -1;
			}// )+x
			Unit_len++;
		}
	}
	return Unit_len;
}

double Calculate(struct Unit Unit_arry[], int Unit_len, int& flag)
{
	//����ջ���� 
	int i, Num_pointer = 0, Op_pointer = 0;//Num_pointerָ�������ջ����Op_pointerָ�������ջ�� 
	double Num_stack[max_length] = { 0 };//������ջ 
	char   Op_stack[max_length] = { 0 };//������ջ 

	for (i = 0; i < Unit_len ; i++)
	{
		if (Unit_arry[i].flag != -1)
		{
			if (Unit_arry[i].flag)//������ 
			{
				Num_stack[Num_pointer++] = Unit_arry[i].num;
			}
			else//������ 
			{
				//������ջΪ�ջ��������� ��ջ
				if (Op_pointer == 0 || Unit_arry[i].op == '(')
				{
					Op_stack[Op_pointer++] = Unit_arry[i].op;
				}
				else
				{
					if (Unit_arry[i].op == ')')// ������ �������һֱ��ջ��ֱ������������ 
					{
						Op_pointer--;
						Num_pointer--;
						while (Op_stack[Op_pointer] != '(' && Op_pointer != 0)
						{
							Num_stack[Num_pointer - 1] = Compute(Num_stack[Num_pointer - 1], Num_stack[Num_pointer],
								Op_stack[Op_pointer--]);
							Num_pointer--;
							
						}
						Num_pointer++;
					}
					else if (Compare(Unit_arry[i].op, Op_stack[Op_pointer - 1]))//��ջ��Ԫ�رȽ� 
					{
						Op_stack[Op_pointer++] = Unit_arry[i].op;
					}
					else//�������ջ���ٽ��ò�������ջ 
					{
						Op_pointer--;
						Num_pointer--;
						while (Compare(Unit_arry[i].op, Op_stack[Op_pointer]) == 0 && Op_pointer != -1)
						{
							//��ǰ��������ջ�����������ȼ��� 
							Num_stack[Num_pointer - 1] = Compute(Num_stack[Num_pointer - 1], Num_stack[Num_pointer],
								Op_stack[Op_pointer--]);
							Num_pointer--;
						}
						Op_pointer++;
						Num_pointer++;
						Op_stack[Op_pointer++] = Unit_arry[i].op;
					}
				}
			}
		}
	}
	
	Op_pointer--;//ָ��ջ��Ԫ�� 
	Num_pointer--;//ָ��ջ��Ԫ�� 
	while (Op_pointer != -1)
	{
		Num_stack[Num_pointer - 1] = Compute(Num_stack[Num_pointer - 1], Num_stack[Num_pointer], Op_stack[Op_pointer--]);//Op_pointer--Ϊ��������ջ 
		Num_pointer--;//ǰһ����������ջ 
	}
	
	if (Op_pointer == -1 && Num_pointer == 0)
		flag = 1;//���ʽ�Ϸ� 
	return Num_stack[0];
}

int  Compare(char op1, char op2)
{
	//�������������ȼ��Ƚ�
	char list[] = { "(+-*/" };
	int map[5][5] = 
	{
		//�б��е����㼶���ȼ���Ϊ0����Ϊ1 
		//		  ( + - * /    
		/*  (  */ 1,0,0,0,0,
		/*  +  */ 1,0,0,0,0,
		/*  -  */ 1,0,0,0,0,
		/*  *  */ 1,1,1,0,0,
		/*  /  */ 1,1,1,0,0 
	};
	int i, j;
	for (i = 0; i < 5; i++)
		if (op1 == list[i]) break;
	for (j = 0; j < 5; j++)
		if (op2 == list[j]) break;
	return map[i][j];
}

double Compute(double x, double y, char op)
{
	//�������� 
	switch (op)
	{
	case '+': return x + y;
	case '-': return x - y;
	case '*': return x * y;
	case '/': return x / y;//y����Ϊ0 
	default: return 0;
	}
}

int  Isop(char ch)
{
	//�жϺϷ��ַ�  + - * / ( ) =
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '=')
		return 1;
	return 0;
}

