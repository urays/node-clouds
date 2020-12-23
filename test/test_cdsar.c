#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "test_def.h"
#include "../nc/cdsar.h"

#define N 10

typedef struct _cnode
{
	int a;
	int b;
	char chr;
}cnode;

//------------------------------------------------------------------
bool test_list_cmp(const void* x, const void* y) { return (*(cnode*)x).b > (*(cnode*)y).b; }
bool test_list_cond(const void* x, const void* y) { return (*(cnode*)x).a == *(int*)y; }
void test_list_print(_plist _list)
{
	void* x = NULL;
	int cnt = 0;
	printf("%d\n", _list->size(_list));
	while ((x = _list->next(_list))) {
		printf("[%d] %d %d %c\n", cnt++, ((cnode*)x)->a, ((cnode*)x)->b, ((cnode*)x)->chr);
	}
	_list->reset(_list);
}

void test_list(cnode** pdata, bool(*cmp)(const void*, const void*))
{
	int i = 0;
	_plist lst = list_create(cmp);
	//_plist lst = list_create(NULL);

	for (i = 0; i < N; i++) {
		lst->add(lst, pdata[i]);
	}

	test_list_print(lst);

	//lst->clean(lst);
	//test_list_print(lst, cnode_print);

	int o = 2;
	lst->remove(lst, test_list_cond, &o);
	test_list_print(lst);

	printf("%d %d %c\n", pdata[2]->a, pdata[2]->b, pdata[2]->chr);
	printf("find: %d\n", lst->contain(lst, pdata[2]));

	lst->clean(lst);
	printf("%d\n", lst->size(lst));
	lst->free(&lst);
}
//------------------------------------------------------------------
bool test_queue_cmp(const void* x, const void* y) { return (*(cnode*)x).b > (*(cnode*)y).b; }
void test_queue_print(_pqueue _que)
{
	void* x = NULL;
	int cnt = 0;
	printf("%d\n", _que->size(_que));
	while (!_que->empty(_que))
	{
		//x = _que->front(_que);
		//printf("[%d] %d %d %c  ", cnt++, ((cnode*)x)->a, ((cnode*)x)->b, ((cnode*)x)->chr);
		x = _que->pop(_que);
		printf("[%d] %d %d %c <%d>\n", cnt++, ((cnode*)x)->a, ((cnode*)x)->b, ((cnode*)x)->chr, _que->size(_que));
		//x = _que->back(_que);
		//if (x != NULL) {
		//	printf("[%d] %d %d %c\n", cnt++, ((cnode*)x)->a, ((cnode*)x)->b, ((cnode*)x)->chr);
		//}
	}
}

void test_queue(cnode** pdata, bool(*cmp)(const void*, const void*))
{
	int i = 0;
	_pqueue que = queue_create(cmp);

	for (i = 0; i < N; i++) { que->push(que, pdata[i]); }

	test_queue_print(que);
	que->push(que, pdata[3]);
	que->size(que);
	test_queue_print(que);
	que->push(que, pdata[1]);
	printf("%d\n", que->size(que));
	que->clean(que);
	printf("%d\n", que->size(que));

	que->free(&que);
}
//------------------------------------------------------------------
bool test_stack_cmp(const void* x, const void* y) { return (*(cnode*)x).b > (*(cnode*)y).b; }
void test_stack_print(_pstack _stk)
{
	void* x = NULL;
	int cnt = 0;
	printf("%d\n", _stk->size(_stk));
	while (!_stk->empty(_stk))
	{
		x = _stk->pop(_stk);
		printf("[%d] %d %d %c <%d>\n", cnt++, ((cnode*)x)->a, ((cnode*)x)->b, ((cnode*)x)->chr, _stk->size(_stk));
	}
}

void test_stack(cnode** pdata, bool(*cmp)(const void*, const void*))
{
	int i = 0;
	_pstack _stk = stack_create(cmp);

	for (i = 0; i < N; i++) { _stk->push(_stk, pdata[i]); }

	test_stack_print(_stk);
	_stk->push(_stk, pdata[3]);
	_stk->size(_stk);
	test_stack_print(_stk);
	_stk->push(_stk, pdata[1]);
	printf("%d\n", _stk->size(_stk));
	_stk->clean(_stk);
	printf("%d\n", _stk->size(_stk));

	_stk->free(&_stk);
}
//------------------------------------------------------------------

void test_cdsar()
{
	int i = 0;

	//create data for test
	cnode** pdata = (cnode**)malloc(sizeof(cnode*) * N);
	srand((unsigned int)time(NULL));
	for (i = 0; i < N; i++)
	{
		pdata[i] = (cnode*)malloc(sizeof(cnode));
		pdata[i]->a = i;
		pdata[i]->b = rand() % N;
		pdata[i]->chr = i + 'a';
	}

	//test list
	printf("-----------<TEST LIST]\n");
	test_list(pdata, NULL);

	//test queue
	printf("-----------<TEST QUEUE]\n");
	test_queue(pdata, test_queue_cmp);

	//test stack
	printf("-----------<TEST STACK]\n");
	test_stack(pdata, test_stack_cmp);

	//release memory
	for (i = 0; i < N; i++) { free(pdata[i]); }
	free(pdata);
}

#ifdef TEST_PROJECT_CDSAR
int main(int argv, char** args)
{
	test_cdsar();
}
#endif