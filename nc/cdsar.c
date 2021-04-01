#include <stdio.h>
#include <stdlib.h> //malloc realloc
#include <string.h>
#include <assert.h>

#define MALLOC(_ty,_sz)           ((_ty*)malloc(sizeof(_ty)*(_sz)))
#define REALLOC(_op,_ty,_nsz)     ((_ty*)realloc((_ty*)_op, sizeof(_ty)*(_nsz)))
#define MEMOFREE(_x,_v)           do {free(_x); (_x)=(_v);}while(0)

#include "cdsar.h"

/* 创建单指针结点*/
_psnode _sp_node(void* _e, const _psnode _nt)
{
	_psnode n = MALLOC(struct __nc_snode, 1);
	assert(n != NULL);

	n->e = _e, n->next = _nt;
	return n;
}

//#define NC_HASH_FUNC(faddr)  do{                                \
//    register unsigned int hash = 0;                             \
//    const char* p = faddr;                                      \
//    while (*p) { hash += (hash << 5) + *p++; }                  \
//    hsi = hash & (CACHE_SIZE - 1);                              \
//}while(0)

/* table*/
#define NC_HASH_FUNC(x)   do{                   \
    x = (~x) + (x << 21);                       \
    x = x ^ (x >> 24);                          \
    x = (x + (x << 3)) + (x << 8);              \
    x = x ^ (x >> 14);                          \
    x = (x + (x << 2)) + (x << 4);              \
    x = x ^ (x >> 28);                          \
    x = x + (x << 31);                          \
    x = x & (HASHTAB_SIZE - 1);                 \
}while(0)

bool table_contain(_ptable _s, void* _addr)
{
	uint64 hsi = (uint64)_addr;
	NC_HASH_FUNC(hsi);

	_pcache p = _s->_tab[hsi];
	while (p != NULL) {
		if (p->e == _addr) { return (bool)1; }
		p = p->next;
	}
	return (bool)0;
}

bool table_join(_ptable _s, void* _addr)
{
	uint64 hsi = (uint64)_addr;
	NC_HASH_FUNC(hsi);

	_pcache p = _s->_tab[hsi];
	for (; p;) {
		if (p->e == _addr) { return (bool)0; }
		p = p->next;
	}
	if (_s->_tab[hsi]) {
		_s->_tab[hsi]->next = _sp_node(_addr, _s->_tab[hsi]->next);
	}
	else {
		_s->_tab[hsi] = _sp_node(_addr, NULL);
	}
	++(_s->size);
	return (bool)1;
}

void table_remove(const _ptable _s, void* _addr)
{
	uint64 hsi = (uint64)_addr;
	NC_HASH_FUNC(hsi);

	_pcache* p = &_s->_tab[hsi];
	_pcache lp = NULL, n;

	for (; (*p); lp = (*p), (*p) = (*p)->next) {
		if ((*p)->e == _addr) {
			n = (*p);
			if (lp) { lp->next = (*p)->next; }
			else { (*p) = (*p)->next; }
			MEMOFREE(n, NULL);
			--(_s->size);
			break; //表中元素唯一
		}
	}
}

void table_clean(_ptable* _s)
{
	_pcache n, * t = NULL;
	int i = 0;

	for (i = 0; i < HASHTAB_SIZE; ++i) {
		t = (*(_s))->_tab + i;
		for (; *t;) {
			n = (*t)->next;
			MEMOFREE((*t), NULL);
			t = &n;
			--((*_s)->size);
		}
	}
	//(*_s)->size = 0;
}

void table_free(_ptable* _s)
{
	table_clean(_s);
	MEMOFREE((*(_s))->_tab, NULL);
	MEMOFREE(*(_s), NULL);
}

_ptable table_create()
{
	_ptable t = MALLOC(struct __nc_table, 1);
	assert(t != NULL);

	t->_tab = MALLOC(_pcache, HASHTAB_SIZE);
	assert(t->_tab != NULL);

	memset(t->_tab, 0, sizeof(_pcache) * HASHTAB_SIZE);

	t->size = 0;
	t->contain = table_contain;
	//t->find = table_find;
	t->join = table_join;
	t->remove = table_remove;
	t->clean = table_clean;
	t->free = table_free;
	return t;
}

/* 栈 */
inline bool stack_empty(_pstack _s) {
	return (_s->_ptop == NULL);
}

inline void stack_push(_pstack _s, void* _e) {
	_s->_ptop = _sp_node(_e, _s->_ptop);
}

void stack_push_cmp(_pstack _s, void* _e)
{
	_psnode n = NULL;
	if (_s->_ptop == NULL || _s->_cmp(_e, _s->_ptop->e)) {
		_s->_ptop = _sp_node(_e, _s->_ptop);
	}
	else {
		_psnode p = _s->_ptop;
		_psnode t = p->next;
		while (t != NULL) {
			if (_s->_cmp(_e, t->e)) break;
			p = t;
			t = t->next;
		}
		n = _sp_node(_e, t);
		p->next = n;
	}
}

//需要外加判断 返回是否为NULL
inline void* stack_top(_pstack _s) {
	return (_s->_ptop == NULL ? NULL : _s->_ptop->e);
}

//需要外加判断 返回是否为NULL
void* stack_pop(_pstack _s)
{
	void* top = NULL;
	_psnode t = NULL;

	if (_s->_ptop != NULL) {
		top = _s->_ptop->e;
		t = _s->_ptop->next;
		MEMOFREE(_s->_ptop, t);
	}
	return top;
}

uint32 stack_size(_pstack _s)
{
	_psnode t = _s->_ptop;

	int stack_size = 0;
	while (t != NULL) {
		++stack_size;
		t = t->next;
	}
	return stack_size;
}

void stack_clean(_pstack _s)
{
	_psnode t = NULL;
	while (_s->_ptop != NULL) {
		t = _s->_ptop->next;
		MEMOFREE(_s->_ptop, t);
	}
}

void stack_free(_pstack* _pstk)
{
	stack_clean(*_pstk);
	MEMOFREE(*_pstk, NULL);
}

_pstack stack_create(bool(*cmp)(void*, void*))
{
	_pstack _s = MALLOC(struct __nc_stack, 1);
	assert(_s != NULL);

	_s->_ptop = NULL;
	_s->empty = stack_empty;
	_s->pop = stack_pop;
	_s->push = (cmp == NULL) ? stack_push : stack_push_cmp;
	_s->_cmp = cmp;
	_s->size = stack_size;
	_s->top = stack_top;
	_s->clean = stack_clean;
	_s->free = stack_free;
	return _s;
}

/* 队列 */
inline bool queue_empty(_pqueue _s) {
	return (_s->_phead->next == NULL);
}

inline uint32 queue_size(_pqueue _s) {
	return (*(uint32*)_s->_phead->e);
}

void queue_push(_pqueue _s, void* _e)
{
	_psnode n = _sp_node(_e, NULL);
	_s->_prear->next = n;
	_s->_prear = n;
	++(*(uint32*)(_s->_phead->e));
}

void queue_push_cmp(_pqueue _s, void* _e)
{
	_psnode n = _sp_node(_e, NULL);
	if (_s->_phead->next == NULL) {
		_s->_prear->next = n;
		_s->_prear = n;
	}
	else {
		_psnode p = _s->_phead;
		_psnode t = p->next;
		while (t != NULL) {
			if (_s->_cmp(_e, t->e))
				break;
			p = t;
			t = t->next;
		}
		p->next = n;
		n->next = t;
		if (t == NULL) {
			_s->_prear = n;
		}
	}
	++(*(uint32*)(_s->_phead->e));
}

//需要外加判断 返回是否为NULL
void* queue_pop(_pqueue _s)
{
	void* pop = NULL;
	_psnode t = _s->_phead->next;
	if (t != NULL) {
		_s->_phead->next = t->next;
		pop = t->e;
		if (t->next == NULL)
			_s->_prear = _s->_phead;
		MEMOFREE(t, NULL);
		--(*(uint32*)(_s->_phead->e));
	}
	return pop;
}

//需要外加判断 返回是否为NULL
void* queue_front(_pqueue _s) {
	return (_s->_phead->next == NULL ? NULL : _s->_phead->next->e);
}

//需要外加判断 返回是否为NULL
void* queue_back(_pqueue _s) {
	return (_s->_prear == _s->_phead ? NULL : _s->_prear->e);
}

void queue_clean(_pqueue _s)
{
	_psnode t = _s->_phead->next;
	_psnode n = _s->_phead;
	while (t != NULL) {
		n = t->next;
		MEMOFREE(t, n);
	}
	_s->_phead->next = t;
	*(uint32*)(_s->_phead->e) = 0;
	_s->_prear = _s->_phead;
}

void queue_free(_pqueue* _pque)
{
	queue_clean(*_pque);
	MEMOFREE((*_pque)->_phead, NULL);
	MEMOFREE(*_pque, NULL);
}

_pqueue queue_create(bool(*cmp)(void*, void*))
{
	_pqueue _que = MALLOC(struct __nc_queue, 1);
	assert(_que != NULL);

	_que->_phead = _sp_node(MALLOC(uint32, 1), NULL);

	*((uint32*)_que->_phead->e) = 0;

	_que->_prear = _que->_phead;
	_que->back = queue_back;
	_que->clean = queue_clean;
	_que->empty = queue_empty;
	_que->front = queue_front;
	_que->pop = queue_pop;
	_que->push = (cmp == NULL) ? queue_push : queue_push_cmp;
	_que->_cmp = cmp;
	_que->size = queue_size;
	_que->free = queue_free;
	return _que;
}

/* 列表 */
inline bool list_contain(_plist _s, void* _e) {
	return (_s->_tab->contain(_s->_tab, _e));
}

void list_add(_plist _s, void* _e)
{
	if (_s->_tab->join(_s->_tab, _e)) {
		_s->_phead->next = _sp_node(_e, _s->_phead->next);
	}
}

void list_add_cmp(_plist _s, void* _e)
{
	_psnode n, p, t;
	if (_s->_tab->join(_s->_tab, _e))
	{
		n = _sp_node(_e, NULL);
		p = _s->_phead;
		t = p->next;
		while (t != NULL) {
			if (_s->_cmp(_e, t->e)) { break; }
			p = t;
			t = t->next;
		}
		p->next = n;
		n->next = t;
	}
}

void* list_next(_plist _s)
{
	if (_s->_pcur != NULL) {
		_s->_pcur = _s->_pcur->next;
		if (_s->_pcur != NULL) {
			return _s->_pcur->e;
		}
	}
	return NULL;
}

void* list_find(_plist _s, bool(*cond)(void*, void* _o), void* _o)
{
	_psnode t = _s->_phead->next;
	_psnode n = NULL;

	while (t != NULL) {
		n = t->next;
		if (cond(t->e, _o)) { return t->e; }
		t = n;
	}
	return NULL;
}

void list_remove(_plist _s, bool(*cond)(void*, void* _o), void* _o)
{
	_psnode t = _s->_phead->next, n;
	_psnode x = _s->_phead;

	while (t != NULL) {
		n = t->next;
		if (cond(t->e, _o)) {
			if (_s->_pcur == t) { list_next(_s); }
			_s->_tab->remove(_s->_tab, t->e);
			MEMOFREE(t, NULL);
			x->next = n;
		}
		else { x = t; }
		t = n;
	}
}

void list_clean(_plist _s)
{
	_psnode t = _s->_phead->next, n;
	while (t != NULL) {
		n = t->next;
		MEMOFREE(t, n);
	}
	_s->_phead->next = t;
	_s->_tab->clean(&_s->_tab);
	_s->_pcur = _s->_phead;
}

void list_free(_plist* _s)
{
	list_clean(*_s);
	(*_s)->_tab->free(&((*_s)->_tab));
	MEMOFREE((*_s)->_phead, NULL);
	MEMOFREE(*_s, NULL);
}

void list_reset(_plist _s) {
	_s->_pcur = _s->_phead;
}

inline uint32 list_size(_plist _s) { return _s->_tab->size; }

_plist list_create(bool(*cmp)(const void*, const void*))
{
	_plist _lst = MALLOC(struct __nc_list, 1);
	assert(_lst != NULL);

	_lst->_phead = _sp_node(NULL, NULL);
	_lst->_pcur = _lst->_phead;

	_lst->_tab = table_create();
	_lst->contain = list_contain;

	_lst->add = (cmp == NULL) ? list_add : list_add_cmp;
	_lst->_cmp = cmp;
	_lst->next = list_next;
	_lst->find = list_find;
	_lst->remove = list_remove;
	_lst->clean = list_clean;
	_lst->free = list_free;
	_lst->reset = list_reset;
	_lst->size = list_size;

	return _lst;
}

/* 堆*/
inline bool heap_empty(_pheap _h) { return _h->_sz == 0; }

void heap_insert(_pheap _h, void* _e)
{
	if (_h->_sz >= _h->_cap - 1)
	{
		_h->_dt = REALLOC(_h->_dt, void*, _h->_cap + HEAP_ADDSIZE);
		assert(_h->_dt != NULL);
		_h->_cap += HEAP_ADDSIZE;
	}

	uint32 i = ++_h->_sz;
	uint32 pa = i >> 1;

	for (; i > 1 && _h->_cmp(_e, _h->_dt[pa]);)
	{
		_h->_dt[i] = _h->_dt[pa];
		i = pa, pa >>= 1;
	}
	_h->_dt[i] = _e;
}

void* heap_top(_pheap _h)
{
	if (_h->_sz >= 1) { return _h->_dt[1]; }
	else { return NULL; }
}

void* heap_pop(_pheap _h)
{
	if (_h->_sz == 0) { return NULL; }

	void* ret = _h->_dt[1];
	void* la = _h->_dt[_h->_sz--];

	uint32 pa = 1;
	uint32 ch = pa << 1;

	for (; ch <= _h->_sz;)
	{
		if (ch < _h->_sz && _h->_cmp(_h->_dt[ch + 1], _h->_dt[ch])) { ++ch; }
		if (_h->_cmp(_h->_dt[ch], la))
		{
			_h->_dt[pa] = _h->_dt[ch];
			pa = ch;
			ch <<= 1;
		}
		else { break; }
	}
	_h->_dt[pa] = la;
	return ret;
}

inline void heap_clean(_pheap _h) { _h->_sz = 0; }

void heap_free(_pheap* _h)
{
	heap_clean(*_h);
	MEMOFREE((*_h)->_dt, NULL);
	MEMOFREE(*_h, NULL);
}

_pheap heap_create(bool(*cmp)(const void*, const void*))
{
	assert(cmp != NULL);

	_pheap _hp = MALLOC(struct __nc_heap, 1);
	assert(_hp != NULL);

	_hp->_dt = MALLOC(void*, HEAP_INISIZE);
	assert(_hp->_dt);

	_hp->_sz = 0;
	_hp->_cap = HEAP_INISIZE;
	_hp->empty = heap_empty;
	_hp->_cmp = cmp;
	_hp->insert = heap_insert;
	_hp->pop = heap_pop;
	_hp->top = heap_top;
	_hp->clean = heap_clean;
	_hp->free = heap_free;

	return _hp;
}

#define SORT_CUTOFF  (50)

/*
 * @ 快速排序+插入排序
 * @ 快速排序:O(Nlog2N)
 * @ 插入排序:O(N^2)
 * !!pdt[0]默认作为buff使用,即要求 st>=1
 */
void nc_qsort(void* pdt, int st, int ed, bool(*cmp)(void*, int, int), void(*agn)(void*, int, int))
{
	int i, j;
	int n = ed - st + 1;

	assert(pdt && st >= 1);

	if (SORT_CUTOFF <= n) {
		if (cmp(pdt, ed, st)) {
			agn(pdt, 0, st); agn(pdt, st, ed); agn(pdt, ed, 0);
		}
		if (n == 2) { return; }

		agn(pdt, 0, st + (n >> 1)); agn(pdt, st + (n >> 1), st + 1); agn(pdt, st + 1, 0);
		if (cmp(pdt, st + 1, st)) { agn(pdt, 0, st); agn(pdt, st, st + 1); }
		else if (cmp(pdt, ed, st + 1)) { agn(pdt, 0, ed); agn(pdt, ed, st + 1); }
		if (n == 3) { agn(pdt, st + 1, 0); return; }

		i = st + 1, j = ed - 1;
		while (i < j) {
			while (i < j && cmp(pdt, 0, j)) --j;
			agn(pdt, i, j);
			while (i < j && cmp(pdt, i, 0)) ++i;
			agn(pdt, j, i);
		}
		agn(pdt, i, 0);
		if (i - st > 1) { nc_qsort(pdt, st, i - 1, cmp, agn); }
		if (ed - i > 1) { nc_qsort(pdt, i + 1, ed, cmp, agn); }
	}
	else {
		for (i = st + 1; i <= ed; i++) {
			agn(pdt, 0, i);
			for (j = i; j > st && cmp(pdt, 0, j - 1); --j) {
				agn(pdt, j, j - 1);
			}
			agn(pdt, j, 0);
		}
	}
}