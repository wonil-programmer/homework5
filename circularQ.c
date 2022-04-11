#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 원형큐의 크기 선언

typedef char element;
// 문자 원소를 갖는 큐와 원형큐의 시작과 끝을 의미하는 변수를 담는 구조체 정의
typedef struct { 
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;

/* 함수 원형 선언 */
QueueType* createQueue(); 
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();


int main(void)
{
    printf("------ [허원일] [2019036068] ------\n");

	QueueType* cQ = createQueue(); // 원형큐를 생성후 구조체 포인터변수에 대입
	element data;

	char command; // 사용자로부터 입력받을 명령값

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

        /* 명령값에 따라 수행할 case를 실행하는 switch문 */
		switch(command) {
        // 원형큐에 원소를 삽입
        // 사용자로부터 삽입할 원소를 입력받은 후 원형큐에 해당원소를 삽입
		case 'i': case 'I':
			data = getElement(); 
			enQueue(cQ, data);
			break;
        // 원형큐에서 원소를 삭제
		case 'd': case 'D':
			deQueue(cQ,&data);
			break;
        // 원형큐 원소 출력
		case 'p': case 'P':
			printQ(cQ);
			break;
        // 현재까지의 원형큐 상황 출력
		case 'b': case 'B':
			debugQ(cQ);
			break;
        // 프로그램 종료
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

/* 원형큐를 생성하고 초기화하는 함수 */
QueueType* createQueue()
{
	QueueType* cQ; // 원형큐를 가리키는 포인터변수 cQ 선언
	cQ = (QueueType*)malloc(sizeof(QueueType)); // cQ에 구조체 크기만큼의 메모리 할당
    // 원형큐의 시작과 끝을 0으로 초기화
	cQ->front = 0; 
	cQ->rear = 0;
	return cQ; // 포인터변수 cQ 반환
}

/* 원형큐에 할당된 메모리를 해제하는 함수 */
int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; // cQ에 할당된 메모리가 없으면 반환
    free(cQ); // 포인터변수 cQ에 할당된 메모리 해제 후 반환
    return 1;
}

/* 사용자로부터 원형큐에 삽입할 원소를 입력받는 함수 */
element getElement()
{
	element item; // 원소를 저장할 변수 선언
	printf("Input element = ");
	scanf(" %c", &item);
	return item; // 해당 원소 반환
}

/* 원형큐가 비어있는지 확인하는 함수 */
int isEmpty(QueueType *cQ)
{
	if (cQ->front == cQ->rear){
    // 원형큐의 시작지점을 가리키는 front와 rear 값이 같으면 원형큐가 비어있음을 의미
		printf("Circular Queue is empty!");
		return 1; // 비어있으면 1을 반환
	}
	else return 0; // 비어있지 않으면 0을 반환
}

/* 원형큐가 가득 차 있는지 확인하는 함수 */
int isFull(QueueType *cQ) 
{
	if (((cQ->rear + 1) % MAX_QUEUE_SIZE) == cQ->front) {
    // rear + 1 값을 원형큐의 크기로 나눈 나머지가 front 값과 같으면 원형큐의 포화상태로 봄
    // 이때, rear에 1을 더하는 이유는 원형큐의 공백상태에서의 조건과 달리하기 위해서임
    // 즉, 원형큐에 한 칸의 빈공간을 둠으로써 공백상태와 구분함
    // 가령, 크기가 6인 원형큐에서 front가 0이고 rear이 인덱스 5를 가리키는 경우
    // (원소의 삽입이 연속해서 다섯번 일어난 경우) 나머지 연산결과 0이므로 한칸이 비었음에도 포화상태로 
    // 인식함 
		printf(" Circular Queue is full!");
		return 1;
	}
	else return 0;
}

/* 원형큐에 원소를 삽입하는 함수 */
void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) return; // 원형큐가 포화상태이면 함수종료
	else { // 원형큐가 포화상태가 아니라면 원형큐에 원소 삽입 
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear가 한칸 뒤를 가리키게 함
		cQ->queue[cQ->rear] = item; // 현재 rear를 인덱스로 갖는 큐의 자리에 해당원소 삽입
	}
}

/* 원형큐에서 원소를 삭제하는 함수 */
void deQueue(QueueType* cQ, element* item)
{
	if(isEmpty(cQ)) return; // 원형큐가 공백상태라면 함수종료
	else { // 원형큐가 공백상태가 아니라면 원형큐에서 원소 삭제
		cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front가 한칸 뒤를 가리키게 함
		*item = cQ->queue[cQ->front]; // 현재 front를 인덱스로 갖는 큐의 자리에 해당원소 삽입 
		return;
	}
}

/* 원형큐를 출력하는 함수 */
void printQ(QueueType *cQ)
{
	int i, first, last; // 

	first = (cQ->front + 1) % MAX_QUEUE_SIZE; // first가 front 다음값을 가리키도록 설정
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE; // last가 rear 다음값을 가리키도록 설정

	printf("Circular Queue : [");

	i = first; // i에 first 값을 대입
    /* 원형큐의 first부터 last 바로 전 인덱스 원소들을 순서대로 출력 */
	while(i != last){ // i가 last를 가리킬때 까지 반복 (last에 rear 다음 인덱스가 들어있으므로)
		printf("%3c", cQ->queue[i]); // 원형큐의 인덱스 i인 원소를 출력
		i = (i+1)%MAX_QUEUE_SIZE; // i에 다음 인덱스를 대입
	}
	printf(" ]\n");
}

/* 현재까지의 원형큐 상태를 출력하는 함수 */
void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
    // for 반복문을 원형큐 크기만큼 돌며 수행
	{
        // i가 front와 같으면 해당 i에 front가 위치한다고 출력
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
        // i번째 원소를 출력
		printf("  [%d] = %c\n", i, cQ->queue[i]);
	}
	// front와 rear가 가리키는 인덱스를 각각 출력
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}

