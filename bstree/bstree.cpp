// bstree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "bstree.h"
#include <stdlib.h>
#include <time.h>

class npc
{
public:
	int id;
	int hp;
	operator int()
	{
		return id;
	}
};

npc npcpool[100000000];

__int64 Rand()
{
	__int64 a = rand();
	__int64 b = rand();
	return a * b;
}

int main(int argc, char* argv[])
{
	btree<npc, int> tree;
	int i;
	for( i = 0; i < 10000000; i++)
	{
		npc data;
		data.id = i;
		data.hp = rand();
		tree.Insert(data);
		npcpool[i] = data;
	}
	/*
	for(int j = 0; j < 100000; j++)
	{
		int key = rand();
		btree<int, int>::node* node = tree.Delete(key);
		if(node != NULL)
		{
			printf("\n%d is deleted, key is %d", node->data, key);
		}
	}*/
	int id = 0;
	do
	{
		scanf("%d", &id);
		int* indexs = new int[id];
		for( i = 0; i < id; i++)
		{
			indexs[i] = Rand() % 10000000;
		}
		clock_t start = clock();
		for(int i = 0; i < id; i++)
		{
			npc* entry = &(tree.Find(indexs[i])->data);
		}
		int delta = clock() - start;
		printf("binary tree search time:%d\n", delta);
		start = clock();
		for( i = 0; i < id; i++)
		{
			for(int t = 0; t < 10000000; t++)
			{
				if(npcpool[t].id == indexs[i])
				{
					npc* entry = &npcpool[t];
					break;
				}
			}
		}
		delta = clock() - start;
		printf("linear search time:%d\nindex:", delta);
		for(i = 0; i < id; i++)
		{
			printf("%d,", indexs[i]);
		}
		delete[] indexs;
	} while(id >= 0);
	return 0;
}

