#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<Windows.h>

#define OK 1

typedef int Status;//定义Status为整形

typedef struct Custom//定义客户结点结构体
{
    int dealtime;//处理客户时间变量
    int amonut;//处理客户交易金额变量（正为存款/还款，负为取款/借款）
    int num;//定义客户序号
}ElemType;

typedef struct event
{
    int type;//事件类型，1为到达，0为离开
    int time;//事件发生时间
    int num;//事件触发的客户序号
    int index;//事件操作类型，1为存款/还款，0为取款/借款
    int money;//客户在该事件中的交易金额
}event;

typedef struct//定义事件表结点
{
    event data;
    struct LNode* next;
}LNode, * Link;

typedef struct//定义事件表为链表的数据结构
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
    printf("\n时间表一览：\n");
    printf("------------------------------------------------------------------------------------");
    printf("\n|  客户序号：  |  事件类型：  |  事件触发时间：   |  事件任务：  |  事件处理金额：  |");
    printf("\n------------------------------------------------------------------------------------");
    while (p != NULL)
    {
        printf("\n|     %d       |", p->data.num);
        if (p->data.type == 1)
        {
            printf("      到达     |       %d         |", p->data.time);
            if (p->data.index == 1)printf("     存款/还款   |     %d     |", p->data.money);
            else printf("     取款/借款   |     %d     |", p->data.money);
            printf("\n------------------------------------------------------------------------------------");
        }
        else
        {
            if (p->data.type == 0)
            {
                printf("      离开     |       %d         |", p->data.time);
                if (p->data.index == 1)printf("     存款/还款   |     %d     |", p->data.money);
                else printf("     取款/借款   |     %d     |", p->data.money);
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



/*-------------------------------------定义需要使用的全局变量--------------------------------------*/

int total;//银行余额
int max_money;//最大交易金额
LQueue* deal_queue = NULL;//客户处理队列
LQueue* wait_queue = NULL;//客户等待队列
LinkList event_link = NULL;//一个事件表（包含到达和离开）
int nextTime = 0;//下一个事件的发生时间
int NextSolveTime = 0;//下一个事件的处理时间
int CustomNumber = 1;//客户的服务序号
ElemType* leave_Custom = NULL;//准备离开的客户变量
ElemType* wait_Custom = NULL;//待处理的客户变量

/*-------------------------------------定义需要使用的全局变量--------------------------------------*/

ElemType* myMalloc(int MaxDealTime, int MinDealTime)
{
    ElemType* e;
    e = (ElemType*)malloc(sizeof(ElemType));
    if (e == NULL)return NULL;
    e->dealtime = (rand() % (MaxDealTime - MinDealTime) + MinDealTime);//处理用户事件事件在最小交易时间和最大交易时间之间的随机数
    e->amonut = (rand() % (2 * max_money) - max_money);//用户交易金额的随机数
    e->num = CustomNumber;//用户序号
    CustomNumber++;
    return e;
}

ElemType* arrival_event(int i, int MaxTimeGap, int MinTimeGap, int MaxTradeTime, int MinTradeTime)
{
    if (nextTime == i)//如果当前时间等于下次客户到达时间
    {
       
        printf("____________________________________________________________________________\n");
        printf("当前时间是:%d\n", i);
        nextTime = nextTime + (rand() % (MaxTimeGap - MinTimeGap) + MinTimeGap);//下个用户到达时间为当前时间加上时间间隔范围的随机数
        ElemType* p = NULL;
        p = myMalloc(MaxTradeTime, MinTradeTime);
        event* e = (event*)malloc(sizeof(event));
        e->type = 1;
        e->num = p->num;
        e->time = i;
        e->money = p->amonut;
        if (p->amonut < 0)e->index = 0;//如果随机交易金额为负，则定义为取款/借款
        else e->index = 1;//反之则定义为存款/还款
        InsertLinkList(event_link, *e);//将到达事件插入到事件表的尾部
        if (NextSolveTime < i)
        {
            if (-(p->amonut) <= total)//当事件为存款/（取款/借款）金额小于银行余额
            {
                NextSolveTime = p->dealtime+ i;//下一事件的处理时间为该用户交易时间的随机数+当前时间
                total = total + p->amonut;
                EnQueue(deal_queue, *p);//将p入队
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
        printf("客户序号：%d 客户交易需要时间：%d  客户需要处理的钱：%d  银行余额：%d\n", p->num, p->dealtime, p->amonut, total);
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
    if (NextSolveTime == i)//当前时间等于交易完成的时间
    {
        DeQueue(deal_queue, leave_Custom);//将该客户移出处理队列
        int last_money = total - leave_Custom->amonut;//last_money为用户移出后的银行余额
        leave_event(leave_Custom, i);//将移出的客户和时间信息放进事件表
        if (((leave_Custom->amonut > 0) && wait_queue->front != NULL))//准备离开用户是存钱的
        {
            int index = 0;
            LQueue* e = InitQueue();
            int TempMoney = total;
            while (TempMoney > last_money && wait_queue->front != NULL)
            {
                DeQueue(wait_queue, wait_Custom);//尝试将准备解决的用户移出等待队列
                if (-(wait_Custom->amonut) <= TempMoney)
                {
                    EnQueue(e, *wait_Custom);//将准备解决的用户入队列e
                    TempMoney = TempMoney + wait_Custom->amonut;//计算暂时的银行余额
                }
                else EnQueue(wait_queue, *wait_Custom);//如果失败就将该用户重新移入等待队列的尾部
                index++;
                if (index >= wait_queue->length)break;
            }
            CombineQueue(deal_queue, e);//将队列e顺序接在处理队列的尾部
            if (deal_queue->front != NULL)
            {
                while (deal_queue->front != NULL && -(deal_queue->front->data.amonut) > total)//无法处理
                {
                    DeQueue(deal_queue, leave_Custom);//从处理队列移出
                    EnQueue(wait_queue, *leave_Custom);//加入等待队列
                }
                if (deal_queue->front != NULL)
                {
                    NextSolveTime = deal_queue->front->data.dealtime+ NextSolveTime;//更新下个用户处理时间
                    total = total + deal_queue->front->data.amonut;//更新银行余额
                }
            }
        }
        else//为取款/借款类型，其余同上
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
    double sum = 0, come, leave;//sum为总等待时间，a为到达时间，b为离开时间
    int AllCustom = 0;//总到达人数
    int success = 0, deposit = 0, withdrawal = 0, DpAmount = 0, WdAmount = 0;//设置index来作为标记，为交易失败人数
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
    printf("*           时间表信息：                    *\n");
    printf("_____________________________________________\n");

    printf("*        全部用户数: %d 人                  *\n", AllCustom);
    printf("_____________________________________________\n");

    printf("*        总共消耗时间: %f 分钟     *\n", sum);
    printf("_____________________________________________\n");

    printf("*        成功人数: %d 人                    *\n", AllCustom - success);
    printf("_____________________________________________\n");

    printf("*        取款/借款成功人数: %d 人           *\n",WdAmount - withdrawal);
    printf("_____________________________________________\n");

    printf("*        存款/还款成功人数: %d 人           *\n", DpAmount - deposit);
    printf("_____________________________________________\n");

    printf("*        客户平均逗留时间： %f 分钟  *\n", sum / AllCustom);
    printf("_____________________________________________\n");

}

void menu()
{
    printf("_____________________________________________________________________________________\n");

    printf("|                                    银行模拟系统                                   |\n");
    printf("_____________________________________________________________________________________\n");

    printf("|                                    1、程序模拟                                    |\n");
    printf("|                                    2、退出程序                                    |\n");

    printf("_____________________________________________________________________________________\n");


    printf("|                                     3122004665                                    |\n");
    printf("|                                    22级计科(2)班                                  |\n");
    printf("|			                王展锐		                            |\n");
    printf("_____________________________________________________________________________________\n");
    printf("|					 请输入:                                    |\n");

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
                srand((unsigned int)time(NULL));//随机函数随电脑时间变化,确保不会随机数一样
                int Runtime, MaxTimeGap, MinTimeGap, MaxTradeTime, MinTradeTime, i;
                printf("请输入银行的运营时间（分钟）：");
                scanf_s("%d", &Runtime);
                while (Runtime <= 0)
                {
                    printf("运营时间必须大于0\n");
                    printf("请输入银行的运营时间（分钟）：");
                    scanf_s("%d", &Runtime);
                }
                printf("请输入银行的运营金额（元）：");
                scanf_s("%d", &total);
                while (total <= 0)
                {
                    printf("初始运营金额必须大于0\n");
                    printf("请输入银行的运营金额（元）：");
                    scanf_s("%d", &total);
                }
                printf("请输入客户最大交易金额（元）：");
                scanf_s("%d", &max_money);
                while (max_money > total || max_money <= 0)
                {
                    printf("客户最大交易金额必须小于银行初始运营金额且大于0\n");
                    printf("请输入客户最大交易金额（元）：");
                    scanf_s("%d", &max_money);
                }
                printf("请输入客户最大到达时间间隔（分钟）：");
                scanf_s("%d", &MaxTimeGap);
                while (MaxTimeGap <= 0 || MaxTimeGap > Runtime)
                {
                    printf("客户最大到达时间间隔必须大于0且小于运营时间\n");
                    printf("请输入客户最大到达时间间隔（分钟）：");
                    scanf_s("%d", &MaxTimeGap);
                }
                printf("请输入客户最小时间间隔（分钟）：");
                scanf_s("%d", &MinTimeGap);
                while (MinTimeGap <= 0 || MinTimeGap > MaxTimeGap || MinTimeGap > Runtime)
                {
                    printf("客户最小到达时间间隔必须大于0且小于最大到达时间间隔且小于运营时间\n");
                    printf("请输入客户最小时间间隔（分钟）：");
                    scanf_s("%d", &MinTimeGap);
                }
                printf("请输入客户最大交易时间（分钟）：");
                scanf_s("%d", &MaxTradeTime);
                while (MaxTradeTime <= 0 || MaxTradeTime > Runtime)
                {
                    printf("客户最大交易时间必须大于0且小于运营时间\n");
                    printf("请输入客户最大交易时间（分钟）：");
                    scanf_s("%d", &MaxTradeTime);
                }
                printf("请输入客户最小交易时间（分钟）：");
                scanf_s("%d", &MinTradeTime);
                while (MinTradeTime <= 0 || MinTradeTime > MaxTradeTime || MinTradeTime > Runtime)
                {
                    printf("客户最小交易时间必须大于0且小于最大交易时间且小于运营时间\n");
                    printf("请输入客户最小交易时间（分钟）：");
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
                printf("感谢您的使用！\n");
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