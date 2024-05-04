
#include <stdio.h>
#include<stdlib.h>

#define TRUE 1;
#define FALSE 0;
#define OK 1;
#define ERROR 0;
#define OVERFLOW -1;
#define LH 1;	// 左子树高
#define EH 0;	//两树等高
#define RH -1;	//右子树高

typedef int ElemType;
typedef int Status;

typedef struct ATNode
{
	ElemType key;
	int bf;	//平衡因子
	struct ATNode* lchild, * rchild;
}ATNode, *ATree;	//平衡二叉树


//右旋
void R_Rotate(ATree &T)
{
	ATree Lchild;
	Lchild = T->lchild;	//Lchild指向T结点的左孩子
	T->lchild = Lchild->rchild;	//Lchild结点的右孩子置为T结点的左孩子
	Lchild->rchild = T;	//置T结点为Lchild的右孩子
	T = Lchild;	//T指向新的根结点
}

//左旋
void L_Rotate(ATree& T)
{
	ATree Rchild;
	Rchild = T->rchild;	//Rchild指向T的右孩子
	T->rchild = Rchild->lchild;	//Rchild结点的左孩子置为T结点的右孩子
	Rchild->lchild = T;	//置T结点为Rchild结点的左孩子
	T = Rchild;	//T指向新的根结点
}

//对树T的左平衡处理
void LBalance(ATree& T)
{
	ATree Lc, Rd;	//Lc指向T的左孩子 Rd指向Lc的右子树
	Lc = T->lchild;
	switch (Lc->bf)
	{
	case 1: //Lc的左树高 LL型 右旋处理
		T->bf = EH;
		Lc->bf = EH;
		R_Rotate(T);
		break;

	case 0: //Lc左右等高 T树在插入前已经失衡 LL型 右旋处理
		T->bf = LH;
		Lc->bf = RH;
		R_Rotate(T);
		break;

	case -1: //新插入结点在T左孩子的右子树上 LR型 先左旋 再右旋
		Rd = Lc->rchild;
		switch (Rd->bf) { //修改T及其左孩子的平衡因子
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
		L_Rotate(T->lchild); //对T的左孩子做左旋调整
		R_Rotate(T);	//对T做右旋调整
		break;

	}
}


//对树T的右平衡处理
void RBalance(ATree& T)
{
	ATree Rc, Ld;
	Rc = T->rchild;	//Rc指向T的右孩子
	switch (Rc->bf)
	{
	case -1:	//RR型 左旋
		T->bf = EH;
		Rc->bf = EH;
		L_Rotate(T);
		break;

	case 0:
		T->bf = RH;	//树T在插入之前就已失衡 T属RR型 左旋
		Rc->bf = LH;
		L_Rotate(T);
		break;

	case 1:	//新结点插入在T的右孩子的左子树上 RL型 先右旋再左旋
		Ld = Rc->lchild;
		switch (Ld->bf)	//修改T及其右孩子的左平衡因子
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



//AVL树的插入
//key用于记录由于插入操作是否导致树的高度增加 
Status Insert(ATree &T,ElemType e, Status& taller) {
	if (e < 0) printf("输入有误！");
	else {
		if (!T) {			//T为空，树长高
			T = (ATree)malloc(sizeof(ATNode));
			if (!T) return OVERFLOW;
			T->key = e;
			T->bf = EH;
			T->lchild = NULL;
			T->rchild = NULL;
			taller = TRUE;
		}
		else if (e == T->key) {         //树中已存在和e相等的结点
			taller = FALSE;
			printf("树中已存在和e相等的结点！");
			return FALSE;           //未插入
		}
		else if (e < T->key) {          //插入到左子树
			if (0== Insert(T->lchild, e, taller)) return FALSE;          //未插入
			if (taller) {
				switch (T->bf) {         //检查T的平衡因子
				case 1:            //原左高，左平衡处理
					LBalance(T);
					taller = FALSE;
					break;
				case 0:            //原等高，左变高
					T->bf = LH;
					taller = TRUE;
					break;
				case -1:            //原右高，变等高
					T->bf = EH;
					taller = FALSE;
					break;
				}
			}
		}
		else {         //插入到右子树
			if (0 == Insert(T->rchild, e, taller)) return FALSE;
			if (taller) {
				switch (T->bf) {          //检查平衡因子
				case 1:            //原本左高，变等高
					T->bf = EH;
					taller = FALSE;
					break;
				case 0:            //原本等高，变右高
					T->bf = RH;
					taller = TRUE;
					break;
				case -1:            //原本右高，右平衡处理
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
		printf("输入有误");
	else
	{
		if (!T)	//T树为空 树长高
		{
			T = (ATree)malloc(sizeof(ATNode));
			if (!T)	return OVERFLOW;
			T->key = p;
			T->bf = EH;
			T->lchild = NULL;
			T->rchild = NULL;
			key = TRUE;

		}
		else if (p == T->key) {	//判断树中是否存在相同结点

			key = FALSE;
			printf("树中已存在该结点");
			return FALSE;

		}
		else if (p < T->key) {	//插入到左子树
			if (Insert(T->lchild, p, key) == 0)	
				return FALSE;
			if (key)	//插入成功后检查T的平衡因子 调节树高
			{
				switch (T->bf)
				{
				case 1:	//左高,左平衡处理
					LBalance(T);
					key = false;
					break;
				
				case 0:	//等高，左变高
					T->bf = LH;
					key = true;
					break;

				case -1: //右高，变等高
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
				switch (T->bf)	//检查平衡因子
				{
					case 1:	//原左高变等高
						T->bf = EH;
						key = FALSE;
						break;
					
					case 0:	//原等高变右高
						T->bf = RH;
						key = TRUE;
						break;

					case -1:	//原右高 右平衡处理
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

//删除操作
//key用于判断删除是否成功
void Delete(ATree &T, ElemType p, Status key)
{

	ATNode	*q, * s;
	q = T;
	if (T->lchild == NULL)//被删除结点左子树为空，右子树代替被删除结点
	{
		T = T->rchild;
		free(q);
		key = TRUE;
	}
	else if (T->rchild == NULL)//被删除结点右子树为空，左子树代替被删除结点
	{
		T = T->lchild;
		free(q);
		key = TRUE;
	}
	else {
		s = T->lchild;
		while (s->rchild != NULL) {//找到左子树中最大的结点作为前驱结点
			q = s;
			s = s->rchild;
		}
		ElemType data;
		data = s->key;
		DelAVL(T, data, key);//将前驱结点的值复制给被删除结点，并将前驱结点删去
		T->key = data;
	}
}

//AVL的结点删除
Status DelAVL(ATree& T, ElemType p, Status key)
{
	if (T == NULL) {	
		return FALSE;
	}
	else if (T->key == p)	//删除根节点
	{
		Delete(T, T->key, key);
		return TRUE;
	}
	else if (p < T->key)	//删除结点在左子树
	{
		if (!DelAVL(T->lchild, p, key))	return FALSE;
		if (key)
		{
			switch (T->bf)	
			{
			case 1:		//原先左高，变等高
				T->bf = EH;
				key = TRUE;
				break;
			case 0:		//原先等高，变右高
				T->bf = RH;
				key = FALSE;
				break;
			case -1:	//原先右高，需右平衡处理
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
	else {	//需删除结点在右子树
		if (!DelAVL(T->rchild, p, key))
		{
			return FALSE;
		}
		if (key)
		{
			switch (T->bf)
			{
			case 1:		//原左高，若高度降低则需进行左平衡处理
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
			case 0:	//原等高，变左高
				T->bf = LH;
				key = FALSE;
				break;
			case -1:	//原右高，变等高
				T->bf = EH;
				key = TRUE;
				break;
			}
		}
	}
}


//AVL树的查找
ATree find(ATree T, ElemType p)	//查找类型与ATree相同
{
	if (T == NULL)
		return NULL;
	if (T->key == p)
		return T;
	if (p < T->key)		//查找值在T的左子树
		return find(T->lchild, p);
	return find(T->rchild, p);		//查找值在T的右子树
}

//AVL树的打印
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

//AVL树的销毁（后序遍历）
void destory(ATree& T)
{
	if (T) {
		destory(T->lchild);
		destory(T->rchild);
		free(T);
		
	}
}

//构造一棵AVL树
void CreatAVL(ATree& T, Status& key)
{
	ElemType temp;
	printf("请输入您需要构建的AVL树的结点值：\n");
	printf("tips：请按层次输入，输入的结点值必须为非负整数，输入-1结束输入。\n");
	printf("	(输入一个数值后可使用回车键换行）\n");
	scanf_s("%d", &temp);
	while (temp !=- 1)
	{
		if (temp < 0)
		{
			system("cls");
			printf("您的输入有误！程序终止！\n");
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

				printf("您的输入有误！程序终止！\n");
				break;
			}
			//system("cls");

		}
	}
	//printf("构建成功!\n");

}

//合并两棵AVL树
void MergeAVL(ATree& T1, ATree& T2)
{
	if (T2 == NULL)
		return;
	Status key = FALSE;
	MergeAVL(T1, T2->lchild);
	Insert(T1, T2->key, key);
	MergeAVL(T1, T2->rchild);
}


//分裂AVL树，一棵树中的所有结点值小于等于temp，另一棵树大于temp
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
		printf("--------AVL操作列表------\n");
		printf("1:构建一棵AVL树\n");
		printf("2:在AVL树中插入结点\n");
		printf("3:删除AVL中的结点\n");
		printf("4:查找AVL树中的结点\n");
		printf("5:打印一颗AVL树\n");
		printf("6:合并两棵AVL树\n");
		printf("7:分裂一棵AVL树\n");
		printf("8:销毁一棵AVL树\n");
		printf("9:退出程序\n");
		printf("--------------------------\n");
		printf("请输入执行的操作:\n");
		printf("(每棵树初始状态为空，请在构建后再进行其他操作)\n");
		scanf_s("%d", &i);
		switch (i) {
		case 1:
			printf("请输入所构建的AVL树名称:	\n");
			printf("(若输入1则名称为T，输入2则名称为T2,请勿输入其他数值\n");
			scanf_s("%d", &p1);
			if (p1 == 1)
			{
				T = NULL;
				CreatAVL(T, key1);
				printf("_____________________________________________\n");
				printf("T树为：\n");
				printTree(T, 1);
				printf("_____________________________________________\n");

			}
			else if  (p1 == 2) {
				T2 = NULL;
				CreatAVL(T2, key1);
				printf("_____________________________________________\n");
				printf("T2树为：\n");
				printTree(T2, 1);
				printf("_____________________________________________\n");

			}
			else {
				printf("_____________________________________________\n");
				printf("您的输入有误！请重新选择您要执行的操作！\n");
				printf("_____________________________________________\n");

			}
			break;

		case 2:printf("请输入要插入的结点值：");
			scanf_s("%d", &p2);
					printf("请输入要插入的树：");
					printf("(若输入1则名称为T，输入2则名称为T2,请勿输入其他数值\n");
					scanf_s("%d", &q2);
					printf("_____________________________________________\n");
					if (q2 == 1) {
						if (Insert(T, p2, key1) == 1)
						{
							printf("插入成功！");
							printf("插入后的T树为：\n");
							printTree(T, 1);
						}
						else
							printf("插入失败！\n");
					}
					else if (q2 == 2)
					{
						if (Insert(T2, q2, key1) == 1)
						{
							printf("插入成功！");
							printf("插入后的T2树为：\n");
							printTree(T2, 1);
						}
						else
							printf("插入失败！\n");
					}
					else
					{
						printf("您的输入有误！请重新选择操作！\n");
					}
					printf("_____________________________________________\n");
					break;
		case 3:
			printf("请输入要删除结点所在的树：\n");
			printf("(若输入1则为T，输入2则为T2,请勿输入其他数值\n");
			scanf_s("%d", &p3);
			printf("_____________________________________________\n");
			if (p3 == 1) {
				printf("请输入要删除的结点值：");
				scanf_s("%d", &q3);
				if (DelAVL(T ,q3, key2) == 1)
				{
					printf("删除成功！");
					printf("删除后的T树为：\n");
					printTree(T, 1);

				}
				else
					printf("删除失败！\n");
				printf("_____________________________________________\n");

			}
			else if (p3 == 2)
			{
				printf("请输入要删除的结点值：");
				scanf_s("%d", &q3);
				printf("_____________________________________________\n");

				if (DelAVL(T2, q3, key2) == 1)
				{
					printf("删除成功！");
					printf("删除后的T2树为：\n");
					printTree(T2, 1);
				}
				else
					printf("删除失败！\n");
				printf("_____________________________________________\n");

			}
			else
			{
				printf("_____________________________________________\n");
				printf("您的输入有误！请重新选择操作！\n");
				printf("_____________________________________________\n");
			}
			
			break;

		case 4:
			printf("请输入要查找的结点的值：");
			scanf_s("%d", &p4);
			printf("_____________________________________________\n");
			if (find(T, p4))
				printf("该结点在T树上\n");
			else
				printf("该结点不在T树上\n");
			if(find(T2, p4))
				printf("该结点在T2树上\n");
			else
				printf("该结点不在T2树上\n");
			printf("_____________________________________________\n");
			break;

		case 5:
			printf("请输入要打印的树的名称：\n");
			printf("(若输入1则为T，输入2则为T2,请勿输入其他数值\n");
			scanf_s("%d", &p5);
			if (p5 == 1) {
				printf("_____________________________________________\n");
				printf("T树为：\n");
				printTree(T, 1);
				printf("_____________________________________________\n");
			}
			else if(p5 == 2)
			{
				printf("_____________________________________________\n");
				printf("T2树为：\n");
				printTree(T2, 1);
				printf("_____________________________________________\n");
			}
			else
			{
				printf("_____________________________________________\n");
				printf("您的输入有误！请重新选择操作！\n");
				printf("_____________________________________________\n");
			}
			break;  

		case 6:
			MergeAVL(T, T2);
			printf("_____________________________________________\n");
			printf("两棵树已合并成T树");
			printf("(T2树仍存在，并未删除）");
			printf("合并后的树为：\n");
			printTree(T, 1);
			printf("_____________________________________________\n");
			break;

		case 7 : 
			printf("请选择要分裂的二叉树：");
			printf("(若输入1则分裂T，输入2则分裂T2,请勿输入其他数值\n");
			scanf_s("%d", &p7);
			if (p7 == 1)
			{
				TB = NULL;
				TS = NULL;
				printf("分裂后得到两棵树，TS中所有关键字小于等于temp，TS中所有关键字则大于temp，且原树仍存在\n");
				printf("请输入temp：");
				scanf_s("%d", &temp);
				DivideAVL(T, TS, TB, temp);
				printf("_____________________________________________\n");
				printf("TS为：\n");
				printTree(TS, 1);
				printf("_____________________________________________\n");

				printf("TB为：\n");
				printTree(TB, 1);
				printf("_____________________________________________\n");

			}
			else if (p7 == 2) {
				TB2 = NULL;
				TS2 = NULL;
				printf("分裂后得到两棵树，TS2中所有关键字小于等于temp，TB2中所有关键字则大于temp，且原树仍存在\n");
				printf("请输入temp：");
				scanf_s("%d", &temp);
				DivideAVL(T, TS2, TB2, temp);

				printf("_____________________________________________\n");
				printf("TS2为：");
				printTree(TS2, 1);
				printf("_____________________________________________\n");

				printf("TB2为：");
				printTree(TB2, 1);
				printf("_____________________________________________\n");

			}
			else {
				printf("_____________________________________________\n");
				printf("您的输入有误！请重新选择您要执行的操作！\n");
				printf("_____________________________________________\n");
			}
			break;

		case 8:
			printf("请输入要销毁的AVL树的名称: \n");
			printf("(若输入1则销毁T，输入2则销毁T2,请勿输入其他数值\n");
			scanf_s("%d", &p8);
			if (p8 == 1) {
				destory(T);
				printf("_____________________________________________\n");
				printf("T树已销毁\n");
				printf("_____________________________________________\n");

			}
			else if (p8 == 2) {
				destory(T2);
				printf("_____________________________________________\n");
				printf("T2树已销毁\n");
				printf("_____________________________________________\n");

			}
			else
			{
				printf("_____________________________________________\n");
				printf("您的输入有误！请重新选择您要执行的操作！\n");
				printf("_____________________________________________\n");
			}
			break;

		case 9 :
			printf("_____________________________________________\n");
			printf("已退出！\n");
			printf("_____________________________________________\n");
			break;

		default:
			printf("_____________________________________________\n");
			printf("您的输入有误！请重新选择您要执行的操作！\n");
			printf("_____________________________________________\n");
		}
	} while (i != 9);
}