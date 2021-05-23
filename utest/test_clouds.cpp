#include "option.h"
#if _UTEST_COUNDS

#include "../lib/utest.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/clouds.h"

uint8_t test_cloud_cond(const void* a, const void* b)
{
	return (((_pcnode)a)->loc.x == (*(_clocal*)b).x && ((_pcnode)a)->loc.y == (*(_clocal*)b).y);
}

#define N    11

static int INIT_NODES[][4] = {
	{0,0,0,0},
	{1,1,-2,-1},
	{2,2,-2,1},
	{3,2,-1,2},
	{4,1,-1,1},
	{5,1,2,-1},
	{6,1,1,1},
	{7,2,1,2},
	{8,3,2,2},
	{9,3,2,3},
	{10,4,3,3},
};
static int INIT_CONNECT[][2] = {
	{0,1},
	{0,5},
	{0,6},
	{0,4},
	{2,4},
	{3,4},
	{7,6},
	{7,8},
	{7,9},
	{9,10},
};

TEST(COUNDS, USAGE)
{
	int i = 0;

	//create data for test
	_pcnode* pdata = (_pcnode*)malloc(sizeof(_pcnode) * N);
	for (i = 0; i < N; ++i) {
		pdata[i] = cnode_create(INIT_NODES[i][0], INIT_NODES[i][1], INIT_NODES[i][2], INIT_NODES[i][3]);
	}
	for (i = 0; i < N - 1; ++i) {
		pdata[INIT_CONNECT[i][0]]->contact(pdata[INIT_CONNECT[i][0]], pdata[INIT_CONNECT[i][1]]);
	}

	_pcloud cod = cloud_create();
	for (i = 0; i < N; ++i) {
		cod->join(cod, pdata[i]);
	}
	DISP(11, cod->size(cod));

	_clocal xx = { 2, -1 };

	_pcnode  re = cod->find(cod, test_cloud_cond, &xx);
	DISP(true, re != NULL);
	if (re) {
		DISP(5, re->id);
		DISP(1, re->lev);
		DISP(2.0, re->loc.x);
		DISP(-1.0, re->loc.y);

		_plist fa_list = re->father(re);
		_pcnode x = NULL;
		fa_list->reset(fa_list);

		while ((x = (_pcnode)fa_list->next(fa_list))) {
			DISP(0, x->id);
			DISP(0, x->lev);
			DISP(0, x->loc.x);
			DISP(0, x->loc.y);
		}
		fa_list->free(&fa_list);
	}

	cod->free(&cod);
	for (i = 0; i < N; ++i) { pdata[i]->free(&pdata[i]); }
	free(pdata);
}

#endif