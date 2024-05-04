
#include <stdio.h>
#include<stdlib.h>

#define TRUE 1;
#define FALSE 0;
#define OK 1;
#define ERROR 0;
#define OVERFLOW -1;
#define LH 1;	// ��������
#define EH 0;	//�����ȸ�
#define RH -1;	//��������

typedef int ElemType;
typedef int Status;

typedef struct ATNode
{
	ElemType key;
	int bf;	//ƽ������
	struct ATNode* lchild, * rchild;
}ATNode, *ATree;	//ƽ�������


//����
void R_Rotate(ATree &T)
{
	ATree Lchild;
	Lchild = T->lchild;	//Lchildָ��T��������
	T->lchild = Lchild->rchild;	//Lchild�����Һ�����ΪT��������
	Lchild->rchild = T;	//��T���ΪLchild���Һ���
	T = Lchild;	//Tָ���µĸ����
}

//����
void L_Rotate(ATree& T)
{
	ATree Rchild;
	Rchild = T->rchild;	//Rchildָ��T���Һ���
	T->rchild = Rchild->lchild;	//Rchild����������ΪT�����Һ���
	Rchild->lchild = T;	//��T���ΪRchild��������
	T = Rchild;	//Tָ���µĸ����
}

//����T����ƽ�⴦��
void LBalance(ATree& T)
{
	ATree Lc, Rd;	//Lcָ��T������ Rdָ��Lc��������
	Lc = T->lchild;
	switch (Lc->bf)
	{
	case 1: //Lc�������� LL�� ��������
		T->bf = EH;
		Lc->bf = EH;
		R_Rotate(T);
		break;

	case 0: //Lc���ҵȸ� T���ڲ���ǰ�Ѿ�ʧ�� LL�� ��������
		T->bf = LH;
		Lc->bf = RH;
		R_Rotate(T);
		break;

	case -1: //�²�������T���ӵ��������� LR�� ������ ������
		Rd = Lc->rchild;
		switch (Rd->bf) { //�޸�T�������ӵ�ƽ������
			case 1:
				T->bf = RH;
				Lc->bf = EH;
				break;
			case 0:
				T->bf = EH;
				Lc->bf = EH;
			break;
				case -1:
				T->bf = EH;
				Lc->bf = LH;
				break;
		}
		Rd->bf = EH;
		L_Rotate(T->lchild); //��T����������������
		R_Rotate(T);	//��T����������
		break;

	}
}


//����T����ƽ�⴦��
void RBalance(ATree& T)
{
	ATree Rc, Ld;
	Rc = T->rchild;	//Rcָ��T���Һ���
	switch (Rc->bf)
	{
	case -1:	//RR�� ����
		T->bf = EH;
		Rc->bf = EH;
		L_Rotate(T);
		break;

	case 0:
		T->bf = RH;	//��T�ڲ���֮ǰ����ʧ�� T��RR�� ����
		Rc->bf = LH;
		L_Rotate(T);
		break;

	case 1:	//�½�������T���Һ��ӵ��������� RL�� ������������
		Ld = Rc->lchild;
		switch (Ld->bf)	//�޸�T�����Һ��ӵ���ƽ������
		{
		case 1:
			T->bf = EH;
			Rc->bf = RH;
			break;
		case 0:
			T->bf = EH;
			Rc->bf = EH;
			break;
		case -1:
			T->bf = RH;
			Rc->bf = EH;
			break;
		}
		Ld->bf = EH;
		R_Rotate(T->rchild);
		L_Rotate(T);
		break;
	}
}



//AVL���Ĳ���
//key���ڼ�¼���ڲ�������Ƿ������ĸ߶����� 
Status Insert(ATree &T,ElemType e, Status& taller) {
	if (e < 0) printf("��������");
	else {
		if (!T) {			//TΪ�գ�������
			T = (ATree)malloc(sizeof(ATNode));
			if (!T) return OVERFLOW;
			T->key = e;
			T->bf = EH;
			T->lchild = NULL;
			T->rchild = NULL;
			taller = TRUE;
		}
		else if (e == T->key) {         //�����Ѵ��ں�e��ȵĽ��
			taller = FALSE;
			printf("�����Ѵ��ں�e��ȵĽ�㣡");
			return FALSE;           //δ����
		}
		else if (e < T->key) {          //���뵽������
			if (0== Insert(T->lchild, e, taller)) return FALSE;          //δ����
			if (taller) {
				switch (T->bf) {         //���T��ƽ������
				case 1:            //ԭ��ߣ���ƽ�⴦��
					LBalance(T);
					taller = FALSE;
					break;
				case 0:            //ԭ�ȸߣ�����
					T->bf = LH;
					taller = TRUE;
					break;
				case -1:            //ԭ�Ҹߣ���ȸ�
					T->bf = EH;
					taller = FALSE;
					break;
				}
			}
		}
		else {         //���뵽������
			if (0 == Insert(T->rchild, e, taller)) return FALSE;
			if (taller) {
				switch (T->bf) {          //���ƽ������
				case 1:            //ԭ����ߣ���ȸ�
					T->bf = EH;
					taller = FALSE;
					break;
				case 0:            //ԭ���ȸߣ����Ҹ�
					T->bf = RH;
					taller = TRUE;
					break;
				case -1:            //ԭ���Ҹߣ���ƽ�⴦��
					RBalance(T);
					taller = FALSE;
					break;
				}
			}
		}
		return TRUE;
	}
}
/* 
	if (p < 0)
		printf("��������");
	else
	{
		if (!T)	//T��Ϊ�� ������
		{
			T = (ATree)malloc(sizeof(ATNode));
			if (!T)	return OVERFLOW;
			T->key = p;
			T->bf = EH;
			T->lchild = NULL;
			T->rchild = NULL;
			key = TRUE;

		}
		else if (p == T->key) {	//�ж������Ƿ������ͬ���

			key = FALSE;
			printf("�����Ѵ��ڸý��");
			return FALSE;

		}
		else if (p < T->key) {	//���뵽������
			if (Insert(T->lchild, p, key) == 0)	
				return FALSE;
			if (key)	//����ɹ�����T��ƽ������ ��������
			{
				switch (T->bf)
				{
				case 1:	//���,��ƽ�⴦��
					LBalance(T);
					key = false;
					break;
				
				case 0:	//�ȸߣ�����
					T->bf = LH;
					key = true;
					break;

				case -1: //�Ҹߣ���ȸ�
					T->bf = EH;
					key = FALSE;
					break;
				}
			}
		}	
		else
		{
			if (Insert(T, p, key) == 0)
				return 0;
			if (key)
			{
				switch (T->bf)	//���ƽ������
				{
					case 1:	//ԭ��߱�ȸ�
						T->bf = EH;
						key = FALSE;
						break;
					
					case 0:	//ԭ�ȸ߱��Ҹ�
						T->bf = RH;
						key = TRUE;
						break;

					case -1:	//ԭ�Ҹ� ��ƽ�⴦��
						RBalance(T);
						key = FALSE;
						break;
				}
			}
		}
		
	}
}
*/

Status DelAVL(ATree& T, ElemType p, Status key);

//ɾ������
//key�����ж�ɾ���Ƿ�ɹ�
void Delete(ATree &T, ElemType p, Status key)
{

	ATNode	*q, * s;
	q = T;
	if (T->lchild == NULL)//��ɾ�����������Ϊ�գ����������汻ɾ�����
	{
		T = T->rchild;
		free(q);
		key = TRUE;
	}
	else if (T->rchild == NULL)//��ɾ�����������Ϊ�գ����������汻ɾ�����
	{
		T = T->lchild;
		free(q);
		key = TRUE;
	}
	else {
		s = T->lchild;
		while (s->rchild != NULL) {//�ҵ������������Ľ����Ϊǰ�����
			q = s;
			s = s->rchild;
		}
		ElemType data;
		data = s->key;
		DelAVL(T, data, key);//��ǰ������ֵ���Ƹ���ɾ����㣬����ǰ�����ɾȥ
		T->key = data;
	}
}

//AVL�Ľ��ɾ��
Status DelAVL(ATree& T, ElemType p, Status key)
{
	if (T == NULL) {	
		return FALSE;
	}
	else if (T->key == p)	//ɾ�����ڵ�
	{
		Delete(T, T->key, key);
		return TRUE;
	}
	else if (p < T->key)	//ɾ�������������
	{
		if (!DelAVL(T->lchild, p, key))	return FALSE;
		if (key)
		{
			switch (T->bf)	
			{
			case 1:		//ԭ����ߣ���ȸ�
				T->bf = EH;
				key = TRUE;
				break;
			case 0:		//ԭ�ȵȸߣ����Ҹ�
				T->bf = RH;
				key = FALSE;
				break;
			case -1:	//ԭ���Ҹߣ�����ƽ�⴦��
				if (!T->rchild || T->rchild->bf == 0) {
					key = FALSE;
				}
				else
					key = TRUE;
				RBalance(T);
				break;
			}
		}
	}
	else {	//��ɾ�������������
		if (!DelAVL(T->rchild, p, key))
		{
			return FALSE;
		}
		if (key)
		{
			switch (T->bf)
			{
			case 1:		//ԭ��ߣ����߶Ƚ������������ƽ�⴦��
				if (!T->lchild || T->lchild->bf == 0)
				{
					key = FALSE;
					}
				else
				{
					key = TRUE;		
					LBalance(T);
					break;
				}
			case 0:	//ԭ�ȸߣ������
				T->bf = LH;
				key = FALSE;
				break;
			case -1:	//ԭ�Ҹߣ���ȸ�
				T->bf = EH;
				key = TRUE;
				break;
			}
		}
	}
}


//AVL���Ĳ���
ATree find(ATree T, ElemType p)	//����������ATree��ͬ
{
	if (T == NULL)
		return NULL;
	if (T->key == p)
		return T;
	if (p < T->key)		//����ֵ��T��������
		return find(T->lchild, p);
	return find(T->rchild, p);		//����ֵ��T��������
}

//AVL���Ĵ�ӡ
void printTree(ATree &T, int num)
{
	if (T == NULL) {
		return;
	}
	printTree(T->rchild, num + 1);
	for (int i = 0; i < num; i++)
		printf("		");
	printf("%d\n", T->key);
	printTree(T->lchild, num + 1);
}

//AVL�������٣����������
void destory(ATree& T)
{
	if (T) {
		destory(T->lchild);
		destory(T->rchild);
		free(T);
		
	}
}

//����һ��AVL��
void CreatAVL(ATree& T, Status& key)
{
	ElemType temp;
	printf("����������Ҫ������AVL���Ľ��ֵ��\n");
	printf("tips���밴������룬����Ľ��ֵ����Ϊ�Ǹ�����������-1�������롣\n");
	printf("	(����һ����ֵ���ʹ�ûس������У�\n");
	scanf_s("%d", &temp);
	while (temp !=- 1)
	{
		if (temp < 0)
		{
			system("cls");
			printf("�����������󣡳�����ֹ��\n");
				break;
		}
		else
		{
			if (Insert(T, temp, key) == 1) {
				scanf_s("%d", &temp);
			}
			else
			{
				system("cls");

				printf("�����������󣡳�����ֹ��\n");
				break;
			}
			//system("cls");

		}
	}
	//printf("�����ɹ�!\n");

}

//�ϲ�����AVL��
void MergeAVL(ATree& T1, ATree& T2)
{
	if (T2 == NULL)
		return;
	Status key = FALSE;
	MergeAVL(T1, T2->lchild);
	Insert(T1, T2->key, key);
	MergeAVL(T1, T2->rchild);
}


//����AVL����һ�����е����н��ֵС�ڵ���temp����һ��������temp
void DivideAVL(ATree& T, ATree& TS, ATree& TB, ElemType temp)
{
	if (T == NULL)
		return;
	Status key = FALSE;
	if (temp < T->key) {
		Insert(TB, T->key, key);
	}
	else {
		Insert(TS, T->key, key);
	}
	DivideAVL(T->lchild, TS, TB, temp);
	DivideAVL(T->rchild, TS, TB, temp);

}

int main() {
	int i;
	ElemType p1,p2,q2,p3,q3,p4,p5,p7,p8;
	Status key1 = 0, key2 = 0;
	ATree	T = NULL, T2 = NULL, TB = NULL, TS = NULL,TB2 = NULL,TS2 =NULL;
	ElemType temp;
	do {
		printf("--------AVL�����б�------\n");
		printf("1:����һ��AVL��\n");
		printf("2:��AVL���в�����\n");
		printf("3:ɾ��AVL�еĽ��\n");
		printf("4:����AVL���еĽ��\n");
		printf("5:��ӡһ��AVL��\n");
		printf("6:�ϲ�����AVL��\n");
		printf("7:����һ��AVL��\n");
		printf("8:����һ��AVL��\n");
		printf("9:�˳�����\n");
		printf("--------------------------\n");
		printf("������ִ�еĲ���:\n");
		printf("(ÿ������ʼ״̬Ϊ�գ����ڹ������ٽ�����������)\n");
		scanf_s("%d", &i);
		switch (i) {
		case 1:
			printf("��������������AVL������:	\n");
			printf("(������1������ΪT������2������ΪT2,��������������ֵ\n");
			scanf_s("%d", &p1);
			if (p1 == 1)
			{
				T = NULL;
				CreatAVL(T, key1);
				printf("_____________________________________________\n");
				printf("T��Ϊ��\n");
				printTree(T, 1);
				printf("_____________________________________________\n");

			}
			else if  (p1 == 2) {
				T2 = NULL;
				CreatAVL(T2, key1);
				printf("_____________________________________________\n");
				printf("T2��Ϊ��\n");
				printTree(T2, 1);
				printf("_____________________________________________\n");

			}
			else {
				printf("_____________________________________________\n");
				printf("������������������ѡ����Ҫִ�еĲ�����\n");
				printf("_____________________________________________\n");

			}
			break;

		case 2:printf("������Ҫ����Ľ��ֵ��");
			scanf_s("%d", &p2);
					printf("������Ҫ���������");
					printf("(������1������ΪT������2������ΪT2,��������������ֵ\n");
					scanf_s("%d", &q2);
					printf("_____________________________________________\n");
					if (q2 == 1) {
						if (Insert(T, p2, key1) == 1)
						{
							printf("����ɹ���");
							printf("������T��Ϊ��\n");
							printTree(T, 1);
						}
						else
							printf("����ʧ�ܣ�\n");
					}
					else if (q2 == 2)
					{
						if (Insert(T2, q2, key1) == 1)
						{
							printf("����ɹ���");
							printf("������T2��Ϊ��\n");
							printTree(T2, 1);
						}
						else
							printf("����ʧ�ܣ�\n");
					}
					else
					{
						printf("������������������ѡ�������\n");
					}
					printf("_____________________________________________\n");
					break;
		case 3:
			printf("������Ҫɾ��������ڵ�����\n");
			printf("(������1��ΪT������2��ΪT2,��������������ֵ\n");
			scanf_s("%d", &p3);
			printf("_____________________________________________\n");
			if (p3 == 1) {
				printf("������Ҫɾ���Ľ��ֵ��");
				scanf_s("%d", &q3);
				if (DelAVL(T ,q3, key2) == 1)
				{
					printf("ɾ���ɹ���");
					printf("ɾ�����T��Ϊ��\n");
					printTree(T, 1);

				}
				else
					printf("ɾ��ʧ�ܣ�\n");
				printf("_____________________________________________\n");

			}
			else if (p3 == 2)
			{
				printf("������Ҫɾ���Ľ��ֵ��");
				scanf_s("%d", &q3);
				printf("_____________________________________________\n");

				if (DelAVL(T2, q3, key2) == 1)
				{
					printf("ɾ���ɹ���");
					printf("ɾ�����T2��Ϊ��\n");
					printTree(T2, 1);
				}
				else
					printf("ɾ��ʧ�ܣ�\n");
				printf("_____________________________________________\n");

			}
			else
			{
				printf("_____________________________________________\n");
				printf("������������������ѡ�������\n");
				printf("_____________________________________________\n");
			}
			
			break;

		case 4:
			printf("������Ҫ���ҵĽ���ֵ��");
			scanf_s("%d", &p4);
			printf("_____________________________________________\n");
			if (find(T, p4))
				printf("�ý����T����\n");
			else
				printf("�ý�㲻��T����\n");
			if(find(T2, p4))
				printf("�ý����T2����\n");
			else
				printf("�ý�㲻��T2����\n");
			printf("_____________________________________________\n");
			break;

		case 5:
			printf("������Ҫ��ӡ���������ƣ�\n");
			printf("(������1��ΪT������2��ΪT2,��������������ֵ\n");
			scanf_s("%d", &p5);
			if (p5 == 1) {
				printf("_____________________________________________\n");
				printf("T��Ϊ��\n");
				printTree(T, 1);
				printf("_____________________________________________\n");
			}
			else if(p5 == 2)
			{
				printf("_____________________________________________\n");
				printf("T2��Ϊ��\n");
				printTree(T2, 1);
				printf("_____________________________________________\n");
			}
			else
			{
				printf("_____________________________________________\n");
				printf("������������������ѡ�������\n");
				printf("_____________________________________________\n");
			}
			break;  

		case 6:
			MergeAVL(T, T2);
			printf("_____________________________________________\n");
			printf("�������Ѻϲ���T��");
			printf("(T2���Դ��ڣ���δɾ����");
			printf("�ϲ������Ϊ��\n");
			printTree(T, 1);
			printf("_____________________________________________\n");
			break;

		case 7 : 
			printf("��ѡ��Ҫ���ѵĶ�������");
			printf("(������1�����T������2�����T2,��������������ֵ\n");
			scanf_s("%d", &p7);
			if (p7 == 1)
			{
				TB = NULL;
				TS = NULL;
				printf("���Ѻ�õ���������TS�����йؼ���С�ڵ���temp��TS�����йؼ��������temp����ԭ���Դ���\n");
				printf("������temp��");
				scanf_s("%d", &temp);
				DivideAVL(T, TS, TB, temp);
				printf("_____________________________________________\n");
				printf("TSΪ��\n");
				printTree(TS, 1);
				printf("_____________________________________________\n");

				printf("TBΪ��\n");
				printTree(TB, 1);
				printf("_____________________________________________\n");

			}
			else if (p7 == 2) {
				TB2 = NULL;
				TS2 = NULL;
				printf("���Ѻ�õ���������TS2�����йؼ���С�ڵ���temp��TB2�����йؼ��������temp����ԭ���Դ���\n");
				printf("������temp��");
				scanf_s("%d", &temp);
				DivideAVL(T, TS2, TB2, temp);

				printf("_____________________________________________\n");
				printf("TS2Ϊ��");
				printTree(TS2, 1);
				printf("_____________________________________________\n");

				printf("TB2Ϊ��");
				printTree(TB2, 1);
				printf("_____________________________________________\n");

			}
			else {
				printf("_____________________________________________\n");
				printf("������������������ѡ����Ҫִ�еĲ�����\n");
				printf("_____________________________________________\n");
			}
			break;

		case 8:
			printf("������Ҫ���ٵ�AVL��������: \n");
			printf("(������1������T������2������T2,��������������ֵ\n");
			scanf_s("%d", &p8);
			if (p8 == 1) {
				destory(T);
				printf("_____________________________________________\n");
				printf("T��������\n");
				printf("_____________________________________________\n");

			}
			else if (p8 == 2) {
				destory(T2);
				printf("_____________________________________________\n");
				printf("T2��������\n");
				printf("_____________________________________________\n");

			}
			else
			{
				printf("_____________________________________________\n");
				printf("������������������ѡ����Ҫִ�еĲ�����\n");
				printf("_____________________________________________\n");
			}
			break;

		case 9 :
			printf("_____________________________________________\n");
			printf("���˳���\n");
			printf("_____________________________________________\n");
			break;

		default:
			printf("_____________________________________________\n");
			printf("������������������ѡ����Ҫִ�еĲ�����\n");
			printf("_____________________________________________\n");
		}
	} while (i != 9);
}