#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h> // strncpy, strncat �Լ��� ����ϱ� ���� �������

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack ������ �켱������ ��������, lparen = 0 ���� ���� */
/* typedef enum (�������̸�) {
    ��1 = �ʱⰪ, ��2, ��3 ...
} ������ ��Ī; �� ���·� Ư���� enum�� precedence�� ������ */
typedef enum {
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 7,   /* * ���� */
	divide = 6,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1 /* �ǿ����� */
} precedence;

/* �迭�� ������ ���� */
char infixExp[MAX_EXPRESSION_SIZE]; // ����ǥ����� ���� �迭 ����
char postfixExp[MAX_EXPRESSION_SIZE]; // ����ǥ����� ���� �迭 ����
char postfixStack[MAX_STACK_SIZE]; // ����ǥ������� ��ȯ�� ����� ���� ����
int evalStack[MAX_STACK_SIZE]; // ����ǥ����� ������ �� ����� ���� ����

/* ���� ����� top�� -1�� ���� */
int postfixStackTop = -1;
int evalStackTop = -1;

/* ����ǥ��� �������� ���� ���� ���� �� �ʱ�ȭ */
int evalResult = 0;

/* �Լ� ���� ���� */
void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
    printf("------ [�����] [2019036068] ------\n");

	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/* postfixStack������ push, pop �����ϴ� �Լ� */
/* ǥ��� ��ȯ�� ���ÿ� ���� �����ϴ� �Լ� */
void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x; // ������ top�� 1 ������Ű�� �ش� top�� ����Ű�� �ڸ��� x ����
}
/* ǥ��� ��ȯ�� ������ ���� �����ϴ� �Լ� */
char postfixPop()
{
	char x; // ���ÿ��� ������ ���Ҹ� ���� ����
    // ������ �� ���, ���� �������� �˷��� ���� ��ȯ
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--]; // ������ top�� ��ġ�� ���Ҹ� ������ top 1 ����
    }
    return x; // ������ ���Ҹ� ��ȯ
}

/* evalStack������ push, pop �����ϴ� �Լ� */
/* ���� ����� ���ÿ� ���� �����ϴ� �Լ� */
void evalPush(int x)
{
    evalStack[++evalStackTop] = x; // ������ top�� 1 ������Ű�� �ش� top�� ����Ű�� �ڸ��� x ����
}
/* ���� ����� ������ ���� �����ϴ� �Լ� */
int evalPop()
{
    // ������ �� ��� -1 ��ȯ
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--]; // ������ top�� ��ġ�� ���Ҹ� ������ top ����
}


/* ����ǥ������� ǥ���� ������ �Է¹޴� �Լ� */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); // �Է¹��� ������ infixExp�� ����
}

/* ��ū�� �޾ƿ� Ư�� ������ �Ǵ� �ǿ����ڸ� �����Ͽ� ����� ��ȯ�ϴ� �Լ� */
precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand; // ��� �����ڿ��� �ش����� ������ �ǿ��������� �˷���
	}
}

/* ������ �켱������ �Ǵ��ϴ� �Լ� */
precedence getPriority(char x)
{
	return getToken(x); // enum���� ������ �ش� ������ �켱�������� ��ȯ
}

/* �����ϳ��� ���޹޾�, postfixExp�� �߰��ϴ� �Լ� */
void charCat(char* c)
{
	if (postfixExp == '\0') 
    // postfixExp�� ����ִ� ��� ���� ���ڿ��� ���� 1��ŭ �߶� postfixExp�� ����
		strncpy(postfixExp, c, 1); 
    // postfixExp�� �̹� ���Ұ� �ִ� ��� ���� ���ڿ��� ���� 1��ŭ �߶� postfixExp�� ����
	else
		strncat(postfixExp, c, 1);
}

/* infixExp�� ���ڸ� �ϳ��� �о�鼭 ������ �̿��Ͽ� postfix�� ������. ����� postfix�� postFixExp�� �����. */
void toPostfix()
{
	char *exp = infixExp; // infixExp�� ���� �ϳ����� �б����� ������ 
	char x; // �����ϳ��� �ӽ÷� �����ϱ� ���� ���� 

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0') // infixExp�� ���� ��Ÿ���� ���� ������ ������ ��ȯ������ ��� ����
	{
        /* �ǿ������� ��� */
		if(getPriority(*exp) == operand)
		{
			x = *exp;
            charCat(&x); // ����ǥ��Ŀ� ����
		}
        /* ���� ��ȣ�� ��� */
        else if(getPriority(*exp) == lparen) {
            postfixPush(*exp); // ���ÿ� push
        }
        /* ������ ��ȣ�� ��� */
        else if(getPriority(*exp) == rparen)
        {
            // ���� ��ȣ�� ���ö����� pop ����
            while((x = postfixPop()) != '(') {
                charCat(&x); // pop�� ���Ҹ� ����ǥ��Ŀ� ����
            }
        }
        /* ��ȣ�� �ƴ� �������� ��� */
        else
        {
            // �ش� �����ڰ� ������ top�� ������ �켱������ ���ų� ������� while�� ����
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            {
                x = postfixPop(); // ���ÿ��� pop �� x�� ����
                charCat(&x); // x�� ����ǥ��Ŀ� ����
            }
            postfixPush(*exp); // �ش� �����ڸ� ���ÿ� push
        }

        exp++; // ����ǥ����� ���� ���ҷ� �����ϱ� ���� exp 1����
	}

    /* ���ÿ� ���� �����ڵ��� ó�� */
    while(postfixStackTop != -1) // ������ �� ������ ����
    {
        x = postfixPop(); // ���ÿ��� pop ����
        charCat(&x); // �����ڸ� ����ǥ��Ŀ� ����
    }
}

/* ��������� ����� ����ϴ� �Լ� */
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); // �Էµ� ����ǥ����� ���
	printf("postExp =  %s\n", postfixExp); // ��ȯ�� ����ǥ��� ��, ����ǥ����� ���
	printf("eval result = %d\n", evalResult); // ����ǥ����� ������ ����� ���

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
        // for �ݺ����� ����ũ�⸸ŭ ���� ����ǥ������� ��ȯ �Ϸ� �� ���ÿ� �����ִ� �����ڵ��� ���
		printf("%c  ", postfixStack[i]);

	printf("\n");
}

/* ��� ���� �ʱ�ȭ�ϴ� �Լ� */
void reset()
{
	infixExp[0] = '\0'; // ����ǥ����� ���� �迭�� ù ���Ҹ� �η� ���� (�迭�� ������� �ǹ�)
	postfixExp[0] = '\0'; // ����ǥ����� ���� �迭�� ù ���Ҹ� �η� ���� (�迭�� ������� �ǹ�)

	for(int i = 0; i < MAX_STACK_SIZE; i++)
        // for �ݺ����� ����ũ�⸸ŭ ���� ����ǥ��� ��ȯ�� ���ÿ� �����ִ� �����ڵ��� ���� (�η� ����)
		postfixStack[i] = '\0'; 
    // ������ top���� -1�� �ʱ�ȭ
	postfixStackTop = -1;
	evalStackTop = -1;
    // �������� �����ϴ� ���� �ʱ�ȭ
	evalResult = 0;
}

/* ����ǥ����� �����ϴ� �Լ� */
void evaluation()
{
	int opr1, opr2, i; // �ǿ����ڸ� ���� ���� opr1, opr2 ����

	int length = strlen(postfixExp); // ����ǥ����� ���̸� strlen()���� ����Ͽ� ������ ����
	char symbol;
	evalStackTop = -1; // ������ ���� ������ ������ top�� -1�� ����

    // for �ݺ����� ����ǥ��� ���̸�ŭ ���� ���� ����
	for(i = 0; i < length; i++)
	{
		symbol = postfixExp[i]; // ����ǥ����� ���ʺ��� ���� �ϳ��� ������ symbol ���� �ʱ�ȭ

        // symbol�� �ǿ������̸� ���ÿ� �ش� ���Ҹ� ���ÿ� ����. �̶�, symbol�� �������̹Ƿ� '0'�� �ƽ�Ű�ڵ带 �� ���� ������ ���ڷ� ����.
		if(getToken(symbol) == operand) { 
			evalPush(symbol - '0');
		}
        // symbol�� �������̸� ���ÿ� �ִ� �ǿ����ڸ� ������ ������ ����
		else {
			opr2 = evalPop(); // ���߿� �� �ǿ����ڸ� opr2�� ����
			opr1 = evalPop(); // ���� �� �ǿ����ڸ� opr1�� ����
			switch(getToken(symbol)) {
            // ���̽��� ���� +, -, *, / ������ �����Ͽ� ����� �ٽ� ���ÿ� ���� 
			case plus: evalPush(opr1 + opr2); break;
			case minus: evalPush(opr1 - opr2); break;
			case times: evalPush(opr1 * opr2); break;
			case divide: evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}
	evalResult = evalPop(); // ������ �����ִ� ���� ��, �������� evalResult�� ����
}

