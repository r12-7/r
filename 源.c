#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<Windows.h>

#define OK 1

typedef int Status;//����StatusΪ����

typedef struct Custom//����ͻ����ṹ��
{
    int dealtime;//����ͻ�ʱ�����
    int amonut;//����ͻ����׽���������Ϊ���/�����Ϊȡ��/��
    int num;//����ͻ����
}ElemType;

typedef struct event
{
    int type;//�¼����ͣ�1Ϊ���0Ϊ�뿪
    int time;//�¼�����ʱ��
    int num;//�¼������Ŀͻ����
    int index;//�¼��������ͣ�1Ϊ���/���0Ϊȡ��/���
    int money;//�ͻ��ڸ��¼��еĽ��׽��
}event;

typedef struct//�����¼�����
{
    event data;
    struct LNode* next;
}LNode, * Link;

typedef struct//�����¼���Ϊ��������ݽṹ
{
    Link head;
    Link tail;
    int length;
}*LinkList;

LinkList InitLinkList()
{
    LinkList L = (LinkList*)malloc(sizeof(LNode));
    if (L == NULL)return NULL;
    L->head = NULL;
    L->tail = NULL;
    L->length = 0;
    return L;
}

Status ClearLinkList(LinkList L)
{
    if (L == NULL)return ERROR;
    Link p = L->head;
    Link k = L->head->next;
    while (p != L->tail)
    {
        free(p);
        p = k;
        if (k != L->tail)k = k->next;
    }
    free(p);
    L->head = NULL;
    L->tail = NULL;
    L->length = 0;
    return OK;
}

Status DestroyLinkList(LinkList L)
{
    if (L == NULL)return ERROR;
    ClearLinkList(L);
    free(L);
    return OK;
}

Status InsertLinkList(LinkList L, event e)
{
    if (L == NULL)return ERROR;
    Link p = (Link)malloc(sizeof(LNode));
    if (p == NULL)return ERROR;
    p->data = e;
    p->next = NULL;
    if (L->head == NULL)
    {
        L->head = p;
        L->tail = p;
        L->length = 1;
    }
    else
    {
        L->tail->next = p;
        L->tail = p;
        L->length++;
    }
    return OK;
}

Status PrintLinkList(LinkList L)
{
    if (L == NULL)  return ERROR;
    Link p = L->head;
    printf("\nʱ���һ����\n");
    printf("------------------------------------------------------------------------------------");
    printf("\n|  �ͻ���ţ�  |  �¼����ͣ�  |  �¼�����ʱ�䣺   |  �¼�����  |  �¼������  |");
    printf("\n------------------------------------------------------------------------------------");
    while (p != NULL)
    {
        printf("\n|     %d       |", p->data.num);
        if (p->data.type == 1)
        {
            printf("      ����     |       %d         |", p->data.time);
            if (p->data.index == 1)printf("     ���/����   |     %d     |", p->data.money);
            else printf("     ȡ��/���   |     %d     |", p->data.money);
            printf("\n------------------------------------------------------------------------------------");
        }
        else
        {
            if (p->data.type == 0)
            {
                printf("      �뿪     |       %d         |", p->data.time);
                if (p->data.index == 1)printf("     ���/����   |     %d     |", p->data.money);
                else printf("     ȡ��/���   |     %d     |", p->data.money);
                printf("\n------------------------------------------------------------------------------------");
            }
        }
        p = p->next;
    }
    printf("\n");
    return OK;
}

typedef struct LQNode
{
    ElemType data;
    struct LQNode* next;
}LQNode, * QueuePtr;


typedef struct
{
    QueuePtr front;
    QueuePtr rear;
    int length;
}LQueue;



Status EnQueue(LQueue* Q, ElemType temp )
{
    LQNode* p;
    p = (LQNode*)malloc(sizeof(LQNode));
    if (p == NULL)return ERROR;
    p->data = temp;
    p->next = NULL;
    if (NULL == Q->front)
    {
        Q->front = p;
        Q->rear = p;
        Q->length = 1;
    }
    else
    {
        Q->rear->next = p;
        Q->rear = p;
        Q->length++;
    }
    return OK;
}

Status DeQueue(LQueue* Q, ElemType* temp)
{
    LQNode* p;
    if (NULL == Q->front)return ERROR;
    p = Q->front;
    *temp = p->data;
    Q->front = p->next;
    if (Q->rear == p)Q->rear = NULL;
    free(p);
    Q->length--;
    return OK;
}

LQueue* InitQueue()
{
    LQueue* Q = (LQueue*)malloc(sizeof(LQueue));
    if (Q == NULL)return ERROR;
    Q->front = NULL;
    Q->rear = NULL;
    Q->length = 0;
    return Q;
}

Status myFree(ElemType* e)
{
    if (e == NULL)return ERROR;
    free(e);
    return OK;
}



Status DestroyQueue(LQueue* Q)
{
    if (Q == NULL)return ERROR;
    QueuePtr p = NULL, q = NULL;
    p = Q->front;
    while (p != Q->rear)
    {
        q = p->next;
        myFree(p);
        p = q;
    }
    free(Q->rear);
    return OK;
}

Status CombineQueue(LQueue* Q, LQueue* E)
{
    if (E == NULL || E->front == NULL)return ERROR;
    if (Q == NULL || Q->front == NULL || Q->rear == NULL)
    {
        Q->front = E->front;
        Q->rear = E->rear;
    }
    else
    {
        E->rear->next = Q->front;
        Q->front = E->front;
    }
    return OK;
}



/*-------------------------------------������Ҫʹ�õ�ȫ�ֱ���--------------------------------------*/

int total;//�������
int max_money;//����׽��
LQueue* deal_queue = NULL;//�ͻ��������
LQueue* wait_queue = NULL;//�ͻ��ȴ�����
LinkList event_link = NULL;//һ���¼�������������뿪��
int nextTime = 0;//��һ���¼��ķ���ʱ��
int NextSolveTime = 0;//��һ���¼��Ĵ���ʱ��
int CustomNumber = 1;//�ͻ��ķ������
ElemType* leave_Custom = NULL;//׼���뿪�Ŀͻ�����
ElemType* wait_Custom = NULL;//������Ŀͻ�����

/*-------------------------------------������Ҫʹ�õ�ȫ�ֱ���--------------------------------------*/

ElemType* myMalloc(int MaxDealTime, int MinDealTime)
{
    ElemType* e;
    e = (ElemType*)malloc(sizeof(ElemType));
    if (e == NULL)return NULL;
    e->dealtime = (rand() % (MaxDealTime - MinDealTime) + MinDealTime);//�����û��¼��¼�����С����ʱ��������ʱ��֮��������
    e->amonut = (rand() % (2 * max_money) - max_money);//�û����׽��������
    e->num = CustomNumber;//�û����
    CustomNumber++;
    return e;
}

ElemType* arrival_event(int i, int MaxTimeGap, int MinTimeGap, int MaxTradeTime, int MinTradeTime)
{
    if (nextTime == i)//�����ǰʱ������´οͻ�����ʱ��
    {
       
        printf("____________________________________________________________________________\n");
        printf("��ǰʱ����:%d\n", i);
        nextTime = nextTime + (rand() % (MaxTimeGap - MinTimeGap) + MinTimeGap);//�¸��û�����ʱ��Ϊ��ǰʱ�����ʱ������Χ�������
        ElemType* p = NULL;
        p = myMalloc(MaxTradeTime, MinTradeTime);
        event* e = (event*)malloc(sizeof(event));
        e->type = 1;
        e->num = p->num;
        e->time = i;
        e->money = p->amonut;
        if (p->amonut < 0)e->index = 0;//���������׽��Ϊ��������Ϊȡ��/���
        else e->index = 1;//��֮����Ϊ���/����
        InsertLinkList(event_link, *e);//�������¼����뵽�¼����β��
        if (NextSolveTime < i)
        {
            if (-(p->amonut) <= total)//���¼�Ϊ���/��ȡ��/�����С���������
            {
                NextSolveTime = p->dealtime+ i;//��һ�¼��Ĵ���ʱ��Ϊ���û�����ʱ��������+��ǰʱ��
                total = total + p->amonut;
                EnQueue(deal_queue, *p);//��p���
            }
            else EnQueue(wait_queue, *p);
        }
        else
        {
            if (NextSolveTime == 0)
            {
                if (-(p->amonut) <= total)
                {
                    NextSolveTime = p->dealtime + i;
                    total = total + p->amonut;
                    EnQueue(deal_queue, *p);
                }
                else EnQueue(wait_queue, *p);
            }
            else EnQueue(deal_queue, *p);
        }
        printf("�ͻ���ţ�%d �ͻ�������Ҫʱ�䣺%d  �ͻ���Ҫ�����Ǯ��%d  ������%d\n", p->num, p->dealtime, p->amonut, total);
        return p;
    }
 }

Status leave_event(ElemType* e, int i)
{
    event* p = (event*)malloc(sizeof(event));
    p->type = 0;
    p->num = e->num;
    p->time = i;
    p->money = e->amonut;
    if (e->amonut < 0)p->index = 0;
    else p->index = 1;
    InsertLinkList(event_link, *p);
    return OK;
}

Status deal_event(int i)
{
    if (NextSolveTime == i)//��ǰʱ����ڽ�����ɵ�ʱ��
    {
        DeQueue(deal_queue, leave_Custom);//���ÿͻ��Ƴ��������
        int last_money = total - leave_Custom->amonut;//last_moneyΪ�û��Ƴ�����������
        leave_event(leave_Custom, i);//���Ƴ��Ŀͻ���ʱ����Ϣ�Ž��¼���
        if (((leave_Custom->amonut > 0) && wait_queue->front != NULL))//׼���뿪�û��Ǵ�Ǯ��
        {
            int index = 0;
            LQueue* e = InitQueue();
            int TempMoney = total;
            while (TempMoney > last_money && wait_queue->front != NULL)
            {
                DeQueue(wait_queue, wait_Custom);//���Խ�׼��������û��Ƴ��ȴ�����
                if (-(wait_Custom->amonut) <= TempMoney)
                {
                    EnQueue(e, *wait_Custom);//��׼��������û������e
                    TempMoney = TempMoney + wait_Custom->amonut;//������ʱ���������
                }
                else EnQueue(wait_queue, *wait_Custom);//���ʧ�ܾͽ����û���������ȴ����е�β��
                index++;
                if (index >= wait_queue->length)break;
            }
            CombineQueue(deal_queue, e);//������e˳����ڴ�����е�β��
            if (deal_queue->front != NULL)
            {
                while (deal_queue->front != NULL && -(deal_queue->front->data.amonut) > total)//�޷�����
                {
                    DeQueue(deal_queue, leave_Custom);//�Ӵ�������Ƴ�
                    EnQueue(wait_queue, *leave_Custom);//����ȴ�����
                }
                if (deal_queue->front != NULL)
                {
                    NextSolveTime = deal_queue->front->data.dealtime+ NextSolveTime;//�����¸��û�����ʱ��
                    total = total + deal_queue->front->data.amonut;//�����������
                }
            }
        }
        else//Ϊȡ��/������ͣ�����ͬ��
        {
            if (deal_queue->front != NULL)
            {
                while (deal_queue->front != NULL && -(deal_queue->front->data.amonut) > total)
                {
                    DeQueue(deal_queue, leave_Custom);
                    EnQueue(wait_queue, *leave_Custom);
                }
                if (deal_queue->front != NULL)
                {
                    NextSolveTime = deal_queue->front->data.dealtime + NextSolveTime;
                    total = total + deal_queue->front->data.amonut;
                }
            }
        }
    }
}

void average_StayTime(LinkList e, double last)
{
    double sum = 0, come, leave;//sumΪ�ܵȴ�ʱ�䣬aΪ����ʱ�䣬bΪ�뿪ʱ��
    int AllCustom = 0;//�ܵ�������
    int success = 0, deposit = 0, withdrawal = 0, DpAmount = 0, WdAmount = 0;//����index����Ϊ��ǣ�Ϊ����ʧ������
    LNode* p = e->head;
    for (int i = 1; i <= e->length; i++)
    {
        while (p != NULL)
        {
            if (p->data.num == i)
            {
                if (p->data.type == 1)
                {
                    come = p->data.time;
                    success += 1;
                    AllCustom = AllCustom + 1;
                    if (p->data.index == 1)
                    {
                        DpAmount += 1;
                        deposit += 1;
                    }
                    if (p->data.index == 0)
                    {
                        WdAmount += 1;
                        withdrawal += 1;
                    }
                }
                if (p->data.type == 0)
                {
                    leave = p->data.time;
                    success -= 1;
                    if (p->data.index == 1)deposit -= 1;
                    if (p->data.index == 0)withdrawal -= 1;
                }
            }
            p = p->next;
        }
        p = e->head;
        if (leave == -1.0)
            leave = last;
        sum = sum + leave - come;
        leave = -1.0;
    }
    printf("\n\n");
    printf("_____________________________________________\n");
    printf("*           ʱ�����Ϣ��                    *\n");
    printf("_____________________________________________\n");

    printf("*        ȫ���û���: %d ��                  *\n", AllCustom);
    printf("_____________________________________________\n");

    printf("*        �ܹ�����ʱ��: %f ����     *\n", sum);
    printf("_____________________________________________\n");

    printf("*        �ɹ�����: %d ��                    *\n", AllCustom - success);
    printf("_____________________________________________\n");

    printf("*        ȡ��/���ɹ�����: %d ��           *\n",WdAmount - withdrawal);
    printf("_____________________________________________\n");

    printf("*        ���/����ɹ�����: %d ��           *\n", DpAmount - deposit);
    printf("_____________________________________________\n");

    printf("*        �ͻ�ƽ������ʱ�䣺 %f ����  *\n", sum / AllCustom);
    printf("_____________________________________________\n");

}

void menu()
{
    printf("_____________________________________________________________________________________\n");

    printf("|                                    ����ģ��ϵͳ                                   |\n");
    printf("_____________________________________________________________________________________\n");

    printf("|                                    1������ģ��                                    |\n");
    printf("|                                    2���˳�����                                    |\n");

    printf("_____________________________________________________________________________________\n");


    printf("|                                     3122004665                                    |\n");
    printf("|                                    22���ƿ�(2)��                                  |\n");
    printf("|			                ��չ��		                            |\n");
    printf("_____________________________________________________________________________________\n");
    printf("|					 ������:                                    |\n");

}

void initial()
{
    deal_queue = NULL;
    wait_queue = NULL;
    event_link = NULL;
    nextTime = 0;
    NextSolveTime = 0;
    CustomNumber = 1;
    leave_Custom = NULL;
    wait_Custom = NULL;
}
int main()
{
    int choice=0;
    while (1)
    {
        system("CLS");
        menu();
        scanf_s("%d", &choice);
        if (choice >= 1 && choice <= 2)
        {
            switch (choice)
            {
            case 1:
            {
                leave_Custom = (ElemType*)malloc(sizeof(ElemType));
                wait_Custom = (ElemType*)malloc(sizeof(ElemType));
                deal_queue = InitQueue();
                wait_queue = InitQueue();
                event_link = InitLinkList();
                srand((unsigned int)time(NULL));//������������ʱ��仯,ȷ�����������һ��
                int Runtime, MaxTimeGap, MinTimeGap, MaxTradeTime, MinTradeTime, i;
                printf("���������е���Ӫʱ�䣨���ӣ���");
                scanf_s("%d", &Runtime);
                while (Runtime <= 0)
                {
                    printf("��Ӫʱ��������0\n");
                    printf("���������е���Ӫʱ�䣨���ӣ���");
                    scanf_s("%d", &Runtime);
                }
                printf("���������е���Ӫ��Ԫ����");
                scanf_s("%d", &total);
                while (total <= 0)
                {
                    printf("��ʼ��Ӫ���������0\n");
                    printf("���������е���Ӫ��Ԫ����");
                    scanf_s("%d", &total);
                }
                printf("������ͻ�����׽�Ԫ����");
                scanf_s("%d", &max_money);
                while (max_money > total || max_money <= 0)
                {
                    printf("�ͻ�����׽�����С�����г�ʼ��Ӫ����Ҵ���0\n");
                    printf("������ͻ�����׽�Ԫ����");
                    scanf_s("%d", &max_money);
                }
                printf("������ͻ���󵽴�ʱ���������ӣ���");
                scanf_s("%d", &MaxTimeGap);
                while (MaxTimeGap <= 0 || MaxTimeGap > Runtime)
                {
                    printf("�ͻ���󵽴�ʱ�����������0��С����Ӫʱ��\n");
                    printf("������ͻ���󵽴�ʱ���������ӣ���");
                    scanf_s("%d", &MaxTimeGap);
                }
                printf("������ͻ���Сʱ���������ӣ���");
                scanf_s("%d", &MinTimeGap);
                while (MinTimeGap <= 0 || MinTimeGap > MaxTimeGap || MinTimeGap > Runtime)
                {
                    printf("�ͻ���С����ʱ�����������0��С����󵽴�ʱ������С����Ӫʱ��\n");
                    printf("������ͻ���Сʱ���������ӣ���");
                    scanf_s("%d", &MinTimeGap);
                }
                printf("������ͻ������ʱ�䣨���ӣ���");
                scanf_s("%d", &MaxTradeTime);
                while (MaxTradeTime <= 0 || MaxTradeTime > Runtime)
                {
                    printf("�ͻ������ʱ��������0��С����Ӫʱ��\n");
                    printf("������ͻ������ʱ�䣨���ӣ���");
                    scanf_s("%d", &MaxTradeTime);
                }
                printf("������ͻ���С����ʱ�䣨���ӣ���");
                scanf_s("%d", &MinTradeTime);
                while (MinTradeTime <= 0 || MinTradeTime > MaxTradeTime || MinTradeTime > Runtime)
                {
                    printf("�ͻ���С����ʱ��������0��С�������ʱ����С����Ӫʱ��\n");
                    printf("������ͻ���С����ʱ�䣨���ӣ���");
                    scanf_s("%d", &MinTradeTime);
                }
                for (i = 0; i < Runtime; i++)
                {
                    arrival_event(i, MaxTimeGap, MinTimeGap, MaxTradeTime, MinTradeTime);
                    deal_event(i);
                }
                printf("\n");
                average_StayTime(event_link, Runtime - 1);
                while (deal_queue->front != NULL)
                {
                    DeQueue(deal_queue, leave_Custom);
                    leave_event(leave_Custom, i);
                }
                while (wait_queue->front != NULL)
                {
                    DeQueue(wait_queue, leave_Custom);
                    leave_event(leave_Custom, i);
                }
                PrintLinkList(event_link);
                DestroyQueue(deal_queue);
                DestroyQueue(wait_queue);
                DestroyLinkList(event_link);
                system("pause");
                break;
            }
            case 2:
            {
                printf("��л����ʹ�ã�\n");
                system("PAUSE");
                goto end;
            }
            }
        }
        initial();
    }
end:
    return 0;
}