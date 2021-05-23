#include "option.h"
#if _UTEST_CDSA

#include "../lib/utest.h"
#include "../lib/cdsa.h"

#include <stdlib.h>
#include <string.h>

typedef struct _hnode
{
	int a;
	int b;
} hnode;

uint8_t cmp_max(const void* x, const void* y) { return ((hnode*)x)->b > ((hnode*)y)->b; }
uint8_t cmp_min(const void* x, const void* y) { return ((hnode*)x)->a > ((hnode*)y)->a; }

TEST(CDSA, Heap)
{
	hnode _data1[] = {
		{1, 1},
		{6, 0},
		{3, 7},
		{8, 1},
		{3, 4},
		{5, 2},
		{7, 5},
		{0, 6},
	};

	hnode _test1[] = {
		{3, 7},
		{0, 6},
		{7, 5},
		{3, 4},
		{5, 2},
		{1, 1},
		{8, 1},
		{6, 0},
	};

	hnode _test2[] = {
		{8, 1},
		{7, 5},
		{6, 0},
		{5, 2},
		{3, 4},
		{3, 7},
		{1, 1},
		{0, 6},
	};

	size_t i = 0;
	size_t _i_1 = 0, _i_2 = 0;

	//create data for test
	hnode** pdata = (hnode**)malloc(sizeof(hnode*) * array_length_of(_data1));
	for (i = 0; i < array_length_of(_data1); i++)
	{
		pdata[i] = (hnode*)malloc(sizeof(hnode));
		pdata[i]->a = _data1[i].a;
		pdata[i]->b = _data1[i].b;
	}

	//maxheap
	_pheap maxheap = heap_create(cmp_max);
	for (i = 0; i < array_length_of(_data1); ++i)
	{
		maxheap->insert(maxheap, pdata[i]);
	}
	DISP(8U, maxheap->_sz);

	hnode* tmp = NULL;
	while ((tmp = (hnode*)maxheap->pop(maxheap)) != NULL)
	{
		DISP(_test1[_i_1].a, tmp->a);
		DISP(_test1[_i_1].b, tmp->b);
		_i_1++;
	}
	DISP(0U, maxheap->_sz);

	maxheap->free(&maxheap);
	DISP(true, maxheap == NULL);

	//minheap
	_pheap minheap = heap_create(cmp_min);
	for (i = 0; i < array_length_of(_data1); ++i)
	{
		minheap->insert(minheap, pdata[i]);
	}
	DISP(8U, minheap->_sz);
	//minheap->clean(minheap);

	while (!minheap->empty(minheap))
	{
		tmp = (hnode*)minheap->pop(minheap);
		DISP(_test2[_i_2].a, tmp->a);
		DISP(_test2[_i_2].b, tmp->b);
		_i_2++;
	}
	minheap->free(&minheap);

	//release memory
	for (i = 0; i < array_length_of(_data1); i++)
	{
		free(pdata[i]);
	}
	free(pdata);
}

typedef struct _cnode
{
	int a;
	int b;
	char chr;
} cnode;

cnode _data2[] = {
	{1, 1, 'c'},
	{6, 0, 'a'},
	{3, 7, 'd'},
	{8, 1, 'a'},
	{3, 4, 'c'},
	{5, 2, 'h'},
	{7, 5, 'b'},
	{0, 6, 'z'},
};

uint8_t test_list_cmp(const void* x, const void* y) { return (*(cnode*)x).b > (*(cnode*)y).b; }
uint8_t test_list_cond(const void* x, const void* y) { return (*(cnode*)x).b == *(int*)y; }

TEST(CDSA, LIST)
{
	cnode _test1[] = {
		{0, 6, 'z'},
		{7, 5, 'b'},
		{5, 2, 'h'},
		{3, 4, 'c'},
		{8, 1, 'a'},
		{3, 7, 'd'},
		{6, 0, 'a'},
		{1, 1, 'c'} };

	cnode _test2[] = {
		{0, 6, 'z'},
		{7, 5, 'b'},
		{3, 4, 'c'},
		{8, 1, 'a'},
		{3, 7, 'd'},
		{6, 0, 'a'},
		{1, 1, 'c'},
	};

	size_t i = 0;
	size_t _i_1 = 0, _i_2 = 0;

	cnode** pdata = (cnode**)malloc(sizeof(cnode*) * array_length_of(_data2));
	for (i = 0; i < array_length_of(_data2); i++)
	{
		pdata[i] = (cnode*)malloc(sizeof(cnode));
		pdata[i]->a = _data2[i].a;
		pdata[i]->b = _data2[i].b;
		pdata[i]->chr = _data2[i].chr;
	}

	_plist lst = list_create(NULL);

	for (i = 0; i < array_length_of(_data2); i++)
	{
		lst->add(lst, pdata[i]);
	}

	void* x = NULL;
	DISP(8U, lst->size(lst));

	lst->reset(lst);
	while ((x = lst->next(lst)))
	{
		DISP(_test1[_i_1].a, ((cnode*)x)->a);
		DISP(_test1[_i_1].b, ((cnode*)x)->b);
		DISP(_test1[_i_1].chr, ((cnode*)x)->chr);
		_i_1++;
	}

	int o = 2;
	lst->remove(lst, test_list_cond, &o);

	DISP(7U, lst->size(lst));

	lst->reset(lst);
	while ((x = lst->next(lst)))
	{
		DISP(_test2[_i_2].a, ((cnode*)x)->a);
		DISP(_test2[_i_2].b, ((cnode*)x)->b);
		DISP(_test2[_i_2].chr, ((cnode*)x)->chr);
		_i_2++;
	}

	DISP(true, lst->contain(lst, pdata[2]) == 1);
	DISP(false, lst->contain(lst, pdata[5]) == 1);

	lst->clean(lst);
	DISP(0U, lst->size(lst));
	lst->free(&lst);
}

uint8_t test_queue_cmp(const void* x, const void* y) { return (*(cnode*)x).b > (*(cnode*)y).b; }

TEST(CDSA, QUEUE)
{
	size_t i = 0;
	_pqueue que = queue_create(test_queue_cmp);

	for (i = 0; i < array_length_of(_data2); i++)
	{
		que->push(que, &_data2[i]);
	}

	cnode _test1[] = {
		{3, 7, 'd'},
		{0, 6, 'z'},
		{7, 5, 'b'},
		{3, 4, 'c'},
		{5, 2, 'h'},
		{1, 1, 'c'},
		{8, 1, 'a'},
		{6, 0, 'a'},
	};
	size_t sz = 7;
	size_t _i_1 = 0;

	void* x = NULL;
	DISP(8U, que->size(que));
	while (!que->empty(que))
	{
		x = que->pop(que);
		DISP(_test1[_i_1].a, ((cnode*)x)->a);
		DISP(_test1[_i_1].b, ((cnode*)x)->b);
		DISP(_test1[_i_1].chr, ((cnode*)x)->chr);
		_i_1++;
		DISP(sz--, que->size(que));
	}

	que->push(que, &_data2[3]);
	DISP(1U, que->size(que));

	x = que->front(que);
	DISP(8, ((cnode*)x)->a);
	DISP(1, ((cnode*)x)->b);
	DISP('a', ((cnode*)x)->chr);

	que->push(que, &_data2[1]);
	DISP(2U, que->size(que));

	que->clean(que);
	DISP(0U, que->size(que));

	que->free(&que);
	DISP(true, que == NULL);
}

uint8_t test_stack_cmp(const void* x, const void* y) { return (*(cnode*)x).b > (*(cnode*)y).b; }

TEST(CDSA, STACK)
{
	size_t i = 0;

	_pstack _stk = stack_create(test_stack_cmp);

	for (i = 0; i < array_length_of(_data2); i++)
	{
		_stk->push(_stk, &_data2[i]);
	}

	cnode _test1[] = {
		{3, 7, 'd'},
		{0, 6, 'z'},
		{7, 5, 'b'},
		{3, 4, 'c'},
		{5, 2, 'h'},
		{1, 1, 'c'},
		{8, 1, 'a'},
		{6, 0, 'a'},
	};
	size_t sz = 7;
	size_t _i_1 = 0;

	void* x = NULL;
	while (!_stk->empty(_stk))
	{
		x = _stk->pop(_stk);
		DISP(_test1[_i_1].a, ((cnode*)x)->a);
		DISP(_test1[_i_1].b, ((cnode*)x)->b);
		DISP(_test1[_i_1].chr, ((cnode*)x)->chr);
		_i_1++;
		DISP(sz--, _stk->size(_stk));
	}

	_stk->push(_stk, &_data2[3]);
	DISP(1U, _stk->size(_stk));

	x = _stk->top(_stk);
	DISP('a', ((cnode*)x)->chr);

	_stk->push(_stk, &_data2[1]);
	DISP(2U, _stk->size(_stk));

	_stk->clean(_stk);
	DISP(0U, _stk->size(_stk));

	_stk->free(&_stk);
	DISP(true, _stk == NULL);
}

#endif