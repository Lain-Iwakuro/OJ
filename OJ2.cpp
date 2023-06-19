#include<stdio.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
// 补完了注释，放弃压缩行数了，if太多，应该还有优化空间
// 所以真的有交换“两个”编号一样的商品的操作？我觉得题干应该说得再清楚一些...
int main()
{
	bool is_reversed = false, x_is_head, x_is_tail, y_is_head, y_is_tail, x_pred_is_y, y_pred_is_x;
	int N, M, K, L, S, op, i, head = -1, tail, x, y, temp[2][2];
	int* a[2];
	a[0] = new int[200001]; //pred array
	a[1] = new int[200001]; //next array
	scanf("%d%d%d%d", &N, &M, &K, &L); //numbers of each operation
	S = N + M + K + L;
	for (i = 0; i < S; i++) //start operating
	{
		scanf("%d", &op); //get op code
		switch (op)
		{
		case 1: //place a new node x
		{
			scanf("%d%d%d", &x, &y, &op);
			if (head == -1) head = tail = x; //there is no node
			else
			{
				if ((op == 0) == !is_reversed) //place x before y
				{
					if (y != head) //y->pred exists
					{
						a[0][x] = a[0][y]; //x->pred = y->pred
						a[1][a[0][y]] = x; //y->pred->next = x
					}
					else head = x; //updeate head
					a[1][x] = y; //x->next = y
					a[0][y] = x; //y->pred = x
				}
				else //place x after y
				{
					if (y != tail) //y->next exists
					{
						a[1][x] = a[1][y]; //x->next = y->next
						a[0][a[1][y]] = x; //y->next->pred = x
					}
					else tail = x; //update tail
					a[0][x] = y; //x->pred = y
					a[1][y] = x; //y->next = x
				}
			}
			break;
		}
		case 0: //delete an old node x
		{
			scanf("%d", &x);
			switch ((int)(x == head) + 2 * (int)(x == tail))
			{
			case 0: //not head and not tail
			{
				a[1][a[0][x]] = a[1][x]; //x->pred->next = x->next
				a[0][a[1][x]] = a[0][x]; //x->next->pred = x->pred
				break;
			}
			case 1: //head and not tail
			{
				head = a[1][x]; //update head = x->next
				break;
			}
			case 2: //not head and tail
			{
				tail = a[0][x]; //update tail = x->pred
				break;
			}
			case 3: //head and tail
			{
				head = -1; //reset
				break;
			}
			}
			break;
		}
		case -1: //swap two nodes x and y
		{
			scanf("%d%d", &x, &y);
			if (x != y) //if x == y, do nothing
			{
				temp[0][0] = a[0][x];
				temp[1][0] = a[1][x];
				temp[0][1] = a[0][y];
				temp[1][1] = a[1][y];
				if (x != head) //x->pred exists
				{
					x_is_head = false;
					x_pred_is_y = temp[0][0] == y;
					if (!x_pred_is_y) //x->pred != y
					{
						a[1][temp[0][0]] = y; //x->pred->next = y
						a[0][y] = temp[0][0]; //y->pred = x->pred
					}
				}
				else
				{
					x_is_head = true;
					x_pred_is_y = false;
				}
				if (x != tail) //x->next exists
				{
					x_is_tail = false;
					y_pred_is_x = temp[1][0] == y;
					if (!y_pred_is_x) //x->next != y
					{
						a[0][temp[1][0]] = y; //x->next->pred = y
						a[1][y] = temp[1][0]; //y->next = x->next
					}
				}
				else
				{
					x_is_tail = true;
					y_pred_is_x = false;
				}
				if (y != head) //y->pred exists
				{
					y_is_head = false;
					if (!y_pred_is_x) //y->pred != x
					{
						a[1][temp[0][1]] = x; //y->pred->next = x
						a[0][x] = temp[0][1]; //x->pred = y->pred
					}
					else //y->pred == x i.e. x->next == y
					{
						a[0][x] = y; //x->pred = y
						a[1][y] = x; //y->next = x
					}
				}
				else y_is_head = true;
				if (y != tail) //y->next exists
				{
					y_is_tail = false;
					if (!x_pred_is_y) //y->next != x
					{
						a[0][temp[1][1]] = x; //y->next->pred = x
						a[1][x] = temp[1][1]; //x->next = y->next
					}
					else //y->next == x i.e. x->pred == y
					{
						a[0][y] = x; //y->pred = x
						a[1][x] = y; //x->next = y
					}
				}
				else y_is_tail = true;
				if (x_is_head || y_is_head) head = (int)(x_is_head)*y + (int)(y_is_head)*x; //update head
				if (x_is_tail || y_is_tail) tail = (int)(x_is_tail)*y + (int)(y_is_tail)*x; //update tail
			}
			break;
		}
		case -2: //reverse the list
		{
			is_reversed = !is_reversed;
			break;
		}
		}
	}
	if (!is_reversed) //from head to tail
		for (i = 0; i < N - M; i++)
		{
			printf("%d ", head);
			head = a[1][head]; //head = head->next
		}
	else //from tail to head	
		for (i = 0; i < N - M; i++)
		{
			printf("%d ", tail);
			tail = a[0][tail]; //tail = tail->pred
		}
	printf("-1"); //end of the output
	delete a[0];
	delete a[1];
	return 0;
}