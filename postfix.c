#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h> // strncpy, strncat 함수를 사용하기 위한 헤더파일

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
/* typedef enum (열거형이름) {
    값1 = 초기값, 값2, 값3 ...
} 열거형 별칭; 의 형태로 특수한 enum인 precedence를 정의함 */
typedef enum {
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

/* 배열과 스택을 정의 */
char infixExp[MAX_EXPRESSION_SIZE]; // 중위표기식을 담을 배열 지정
char postfixExp[MAX_EXPRESSION_SIZE]; // 후위표기식을 담을 배열 지정
char postfixStack[MAX_STACK_SIZE]; // 후위표기식으로 변환시 사용할 스택 지정
int evalStack[MAX_STACK_SIZE]; // 후위표기식을 연산할 때 사용할 스택 지정

/* 스택 연산시 top을 -1로 설정 */
int postfixStackTop = -1;
int evalStackTop = -1;

/* 후위표기식 연산결과를 담을 변수 선언 및 초기화 */
int evalResult = 0;

/* 함수 원형 선언 */
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
    printf("------ [허원일] [2019036068] ------\n");

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

/* postfixStack에서의 push, pop 수행하는 함수 */
/* 표기식 변환시 스택에 값을 삽입하는 함수 */
void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x; // 스택의 top을 1 증가시키고 해당 top이 가리키는 자리에 x 삽입
}
/* 표기식 변환시 스택의 값을 추출하는 함수 */
char postfixPop()
{
	char x; // 스택에서 추출할 원소를 담을 변수
    // 스택이 빈 경우, 식의 마지막을 알려줄 널을 반환
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--]; // 스택의 top에 위치한 원소를 추출후 top 1 감소
    }
    return x; // 추출한 원소를 반환
}

/* evalStack에서의 push, pop 수행하는 함수 */
/* 연산 수행시 스택에 값을 삽입하는 함수 */
void evalPush(int x)
{
    evalStack[++evalStackTop] = x; // 스택의 top을 1 증가시키고 해당 top이 가리키는 자리에 x 삽입
}
/* 연산 수행시 스택의 값을 추출하는 함수 */
int evalPop()
{
    // 스택이 빈 경우 -1 반환
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--]; // 스택의 top에 위치한 원소를 추출후 top 감소
}


/* 중위표기식으로 표현된 수식을 입력받는 함수 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); // 입력받은 수식을 infixExp에 저장
}

/* 토큰을 받아와 특정 연산자 또는 피연산자를 구별하여 결과를 반환하는 함수 */
precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand; // 어느 연산자에도 해당하지 않으면 피연산자임을 알려줌
	}
}

/* 문자의 우선순위를 판단하는 함수 */
precedence getPriority(char x)
{
	return getToken(x); // enum으로 설정한 해당 문자의 우선순위값을 반환
}

/* 문자하나를 전달받아, postfixExp에 추가하는 함수 */
void charCat(char* c)
{
	if (postfixExp == '\0') 
    // postfixExp가 비어있는 경우 받은 문자열을 길이 1만큼 잘라 postfixExp에 복사
		strncpy(postfixExp, c, 1); 
    // postfixExp에 이미 원소가 있는 경우 받은 문자열을 길이 1만큼 잘라 postfixExp에 연결
	else
		strncat(postfixExp, c, 1);
}

/* infixExp의 문자를 하나씩 읽어가면서 스택을 이용하여 postfix로 변경함. 변경된 postfix는 postFixExp에 저장됨. */
void toPostfix()
{
	char *exp = infixExp; // infixExp의 문자 하나씩을 읽기위한 포인터 
	char x; // 문자하나를 임시로 저장하기 위한 변수 

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') // infixExp의 끝을 나타내는 널을 만나지 않으면 변환과정을 계속 시행
	{
        /* 피연산자인 경우 */
		if(getPriority(*exp) == operand)
		{
			x = *exp;
            charCat(&x); // 후위표기식에 연결
		}
        /* 왼쪽 괄호인 경우 */
        else if(getPriority(*exp) == lparen) {
            postfixPush(*exp); // 스택에 push
        }
        /* 오른쪽 괄호인 경우 */
        else if(getPriority(*exp) == rparen)
        {
            // 왼쪽 괄호가 나올때까지 pop 수행
            while((x = postfixPop()) != '(') {
                charCat(&x); // pop한 원소를 후위표기식에 연결
            }
        }
        /* 괄호가 아닌 연산자인 경우 */
        else
        {
            // 해당 연산자가 스택의 top의 원소의 우선순위와 같거나 작은경우 while문 수행
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            {
                x = postfixPop(); // 스택에서 pop 후 x에 대입
                charCat(&x); // x를 후위표기식에 연결
            }
            postfixPush(*exp); // 해당 연산자를 스택에 push
        }

        exp++; // 중위표기식의 다음 원소로 접근하기 위해 exp 1증가
	}

    /* 스택에 남은 연산자들을 처리 */
    while(postfixStackTop != -1) // 스택이 빌 때까지 수행
    {
        x = postfixPop(); // 스택에서 pop 수행
        charCat(&x); // 연산자를 후위표기식에 연결
    }
}

/* 현재까지의 결과를 출력하는 함수 */
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); // 입력된 중위표기식을 출력
	printf("postExp =  %s\n", postfixExp); // 변환된 중위표기식 즉, 후위표기식을 출력
	printf("eval result = %d\n", evalResult); // 후위표기식을 연산한 결과를 출력

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
        // for 반복문을 스택크기만큼 돌며 후위표기식으로 변환 완료 후 스택에 남아있는 연산자들을 출력
		printf("%c  ", postfixStack[i]);

	printf("\n");
}

/* 모든 값을 초기화하는 함수 */
void reset()
{
	infixExp[0] = '\0'; // 중위표기식을 담은 배열의 첫 원소를 널로 지정 (배열이 비었음을 의미)
	postfixExp[0] = '\0'; // 후위표기식을 담은 배열의 첫 원소를 널로 지정 (배열이 비었음을 의미)

	for(int i = 0; i < MAX_STACK_SIZE; i++)
        // for 반복문을 스택크기만큼 돌며 후위표기식 변환시 스택에 남아있던 연산자들을 제거 (널로 지정)
		postfixStack[i] = '\0'; 
    // 스택의 top들을 -1로 초기화
	postfixStackTop = -1;
	evalStackTop = -1;
    // 연산결과를 저장하는 변수 초기화
	evalResult = 0;
}

/* 후위표기식을 연산하는 함수 */
void evaluation()
{
	int opr1, opr2, i; // 피연산자를 담을 변수 opr1, opr2 선언

	int length = strlen(postfixExp); // 후위표기식의 길이를 strlen()으로 계산하여 변수에 저장
	char symbol;
	evalStackTop = -1; // 연산을 위해 생성한 스택의 top을 -1로 지정

    // for 반복문을 후위표기식 길이만큼 돌며 연산 수행
	for(i = 0; i < length; i++)
	{
		symbol = postfixExp[i]; // 후위표기식의 왼쪽부터 원소 하나씩 가져와 symbol 변수 초기화

        // symbol이 피연산자이면 스택에 해당 원소를 스택에 삽입. 이때, symbol은 문자형이므로 '0'의 아스키코드를 뺀 값인 정수형 숫자로 삽입.
		if(getToken(symbol) == operand) { 
			evalPush(symbol - '0');
		}
        // symbol이 연산자이면 스택에 있던 피연산자를 꺼내와 연산을 수행
		else {
			opr2 = evalPop(); // 나중에 들어간 피연산자를 opr2로 지정
			opr1 = evalPop(); // 먼저 들어간 피연산자를 opr1로 지정
			switch(getToken(symbol)) {
            // 케이스에 따라 +, -, *, / 연산을 수행하여 결과를 다시 스택에 저장 
			case plus: evalPush(opr1 + opr2); break;
			case minus: evalPush(opr1 - opr2); break;
			case times: evalPush(opr1 * opr2); break;
			case divide: evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}
	evalResult = evalPop(); // 마지막 남아있는 원소 즉, 연산결과를 evalResult에 대입
}

