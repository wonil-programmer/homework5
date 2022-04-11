#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // ����ť�� ũ�� ����

typedef char element;
// ���� ���Ҹ� ���� ť�� ����ť�� ���۰� ���� �ǹ��ϴ� ������ ��� ����ü ����
typedef struct { 
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;

/* �Լ� ���� ���� */
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
    printf("------ [�����] [2019036068] ------\n");

	QueueType* cQ = createQueue(); // ����ť�� ������ ����ü �����ͺ����� ����
	element data;

	char command; // ����ڷκ��� �Է¹��� ��ɰ�

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

        /* ��ɰ��� ���� ������ case�� �����ϴ� switch�� */
		switch(command) {
        // ����ť�� ���Ҹ� ����
        // ����ڷκ��� ������ ���Ҹ� �Է¹��� �� ����ť�� �ش���Ҹ� ����
		case 'i': case 'I':
			data = getElement(); 
			enQueue(cQ, data);
			break;
        // ����ť���� ���Ҹ� ����
		case 'd': case 'D':
			deQueue(cQ,&data);
			break;
        // ����ť ���� ���
		case 'p': case 'P':
			printQ(cQ);
			break;
        // ��������� ����ť ��Ȳ ���
		case 'b': case 'B':
			debugQ(cQ);
			break;
        // ���α׷� ����
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

/* ����ť�� �����ϰ� �ʱ�ȭ�ϴ� �Լ� */
QueueType* createQueue()
{
	QueueType* cQ; // ����ť�� ����Ű�� �����ͺ��� cQ ����
	cQ = (QueueType*)malloc(sizeof(QueueType)); // cQ�� ����ü ũ�⸸ŭ�� �޸� �Ҵ�
    // ����ť�� ���۰� ���� 0���� �ʱ�ȭ
	cQ->front = 0; 
	cQ->rear = 0;
	return cQ; // �����ͺ��� cQ ��ȯ
}

/* ����ť�� �Ҵ�� �޸𸮸� �����ϴ� �Լ� */
int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; // cQ�� �Ҵ�� �޸𸮰� ������ ��ȯ
    free(cQ); // �����ͺ��� cQ�� �Ҵ�� �޸� ���� �� ��ȯ
    return 1;
}

/* ����ڷκ��� ����ť�� ������ ���Ҹ� �Է¹޴� �Լ� */
element getElement()
{
	element item; // ���Ҹ� ������ ���� ����
	printf("Input element = ");
	scanf(" %c", &item);
	return item; // �ش� ���� ��ȯ
}

/* ����ť�� ����ִ��� Ȯ���ϴ� �Լ� */
int isEmpty(QueueType *cQ)
{
	if (cQ->front == cQ->rear){
    // ����ť�� ���������� ����Ű�� front�� rear ���� ������ ����ť�� ��������� �ǹ�
		printf("Circular Queue is empty!");
		return 1; // ��������� 1�� ��ȯ
	}
	else return 0; // ������� ������ 0�� ��ȯ
}

/* ����ť�� ���� �� �ִ��� Ȯ���ϴ� �Լ� */
int isFull(QueueType *cQ) 
{
	if (((cQ->rear + 1) % MAX_QUEUE_SIZE) == cQ->front) {
    // rear + 1 ���� ����ť�� ũ��� ���� �������� front ���� ������ ����ť�� ��ȭ���·� ��
    // �̶�, rear�� 1�� ���ϴ� ������ ����ť�� ������¿����� ���ǰ� �޸��ϱ� ���ؼ���
    // ��, ����ť�� �� ĭ�� ������� �����ν� ������¿� ������
    // ����, ũ�Ⱑ 6�� ����ť���� front�� 0�̰� rear�� �ε��� 5�� ����Ű�� ���
    // (������ ������ �����ؼ� �ټ��� �Ͼ ���) ������ ������ 0�̹Ƿ� ��ĭ�� ��������� ��ȭ���·� 
    // �ν��� 
		printf(" Circular Queue is full!");
		return 1;
	}
	else return 0;
}

/* ����ť�� ���Ҹ� �����ϴ� �Լ� */
void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) return; // ����ť�� ��ȭ�����̸� �Լ�����
	else { // ����ť�� ��ȭ���°� �ƴ϶�� ����ť�� ���� ���� 
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear�� ��ĭ �ڸ� ����Ű�� ��
		cQ->queue[cQ->rear] = item; // ���� rear�� �ε����� ���� ť�� �ڸ��� �ش���� ����
	}
}

/* ����ť���� ���Ҹ� �����ϴ� �Լ� */
void deQueue(QueueType* cQ, element* item)
{
	if(isEmpty(cQ)) return; // ����ť�� ������¶�� �Լ�����
	else { // ����ť�� ������°� �ƴ϶�� ����ť���� ���� ����
		cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front�� ��ĭ �ڸ� ����Ű�� ��
		*item = cQ->queue[cQ->front]; // ���� front�� �ε����� ���� ť�� �ڸ��� �ش���� ���� 
		return;
	}
}

/* ����ť�� ����ϴ� �Լ� */
void printQ(QueueType *cQ)
{
	int i, first, last; // 

	first = (cQ->front + 1) % MAX_QUEUE_SIZE; // first�� front �������� ����Ű���� ����
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE; // last�� rear �������� ����Ű���� ����

	printf("Circular Queue : [");

	i = first; // i�� first ���� ����
    /* ����ť�� first���� last �ٷ� �� �ε��� ���ҵ��� ������� ��� */
	while(i != last){ // i�� last�� ����ų�� ���� �ݺ� (last�� rear ���� �ε����� ��������Ƿ�)
		printf("%3c", cQ->queue[i]); // ����ť�� �ε��� i�� ���Ҹ� ���
		i = (i+1)%MAX_QUEUE_SIZE; // i�� ���� �ε����� ����
	}
	printf(" ]\n");
}

/* ��������� ����ť ���¸� ����ϴ� �Լ� */
void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
    // for �ݺ����� ����ť ũ�⸸ŭ ���� ����
	{
        // i�� front�� ������ �ش� i�� front�� ��ġ�Ѵٰ� ���
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
        // i��° ���Ҹ� ���
		printf("  [%d] = %c\n", i, cQ->queue[i]);
	}
	// front�� rear�� ����Ű�� �ε����� ���� ���
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}

