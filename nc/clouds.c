#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h>

#include "clouds.h"

#define MALLOC(_ty,_sz)      ((_ty*)malloc(sizeof(_ty)*(_sz)))
#define MEMOFREE(_x,_v)      do {free(_x); (_x)=(_v);}while(0)

/* cnode */
bool _cnode_list_cmp(const void* x, const void* y) {
	return (*(_pcnode)x).lev >= (*(_pcnode)y).lev;
}

_plist cnode_childs(_pcnode _s)
{
	_plist res = list_create(NULL);
	_pqueue q = queue_create(NULL);
	_pcnode n = NULL, tn = NULL;

	for (; (n = _s->ajnds->next(_s->ajnds));) {
		if (n->lev > _s->lev) {
			q->push(q, n);
		}
		else { break; }
	}
	_s->ajnds->reset(_s->ajnds);

	while (!q->empty(q)) {
		tn = (_pcnode)(q->pop(q));
		if (tn->lev > _s->lev) {
			res->add(res, tn);
		}
		for (; (n = tn->ajnds->next(tn->ajnds));) {
			q->push(q, n);
		}
		tn->ajnds->reset(tn->ajnds);
	}
	q->free(&q);
	return res;
}

_plist cnode_father(_pcnode _s)
{
	_plist res = list_create(NULL);
	_pcnode n = NULL;

	for (; (n = _s->ajnds->next(_s->ajnds));) {
		if (n->lev < _s->lev) { //父节点只存在于相邻节点中
			res->add(res, n);
		}
	}
	return res;
}

void cnode_contact(_pcnode _s, _pcnode _o)
{
	if (_s == NULL || _o == NULL) { return; }

	_s->ajnds->add(_s->ajnds, _o);
	_o->ajnds->add(_o->ajnds, _s);
}

void cnode_free(_pcnode* _s)
{
	(*_s)->ajnds->free(&((*_s)->ajnds));
	MEMOFREE(*_s, NULL);
}

_pcnode cnode_create(int _id, int _lev, float _x, float _y)
{
	_pcnode n = NULL;
	while (!(n = MALLOC(struct __nc_cnode, 1)));

	n->id = _id;
	n->lev = _lev;
	n->loc.x = _x;
	n->loc.y = _y;
	n->ajnds = list_create(_cnode_list_cmp);
	n->contact = cnode_contact;
	n->childs = cnode_childs;
	n->father = cnode_father;
	n->free = cnode_free;

	return n;
}

/* node cloud */
inline void cloud_join(_pcloud _s, _pcnode _n) { _s->_reg->add(_s->_reg, _n); }
inline uint32 cloud_size(_pcloud _s) { return _s->_reg->size(_s->_reg); }

_pcnode cloud_find(struct __nc_cloud* _s, bool(*cond)(void*, void*), void* _o)
{
	return (_s->_reg->find(_s->_reg, cond, _o));
}

void cloud_free(_pcloud* _s)
{
	(*_s)->_reg->free(&((*_s)->_reg));
	MEMOFREE(*_s, NULL);
}

_pcloud cloud_create()
{
	_pcloud m = NULL;
	//_pcnode _n = cnode_create(0, 0, 0, 0);

	while (!(m = MALLOC(struct __nc_cloud, 1)));

	m->_reg = list_create(NULL);
	//m->_reg->add(m->_reg, _n);

	m->join = cloud_join;
	m->find = cloud_find;
	m->size = cloud_size;
	m->free = cloud_free;

	return m;
}