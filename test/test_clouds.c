#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "test_def.h"
#include "../nc/clouds.h"

#define N    11

bool test_cloud_cond(const void* a, const void* b)
{
	return (((_pcnode)a)->loc.x == (*(_clocal*)b).x &&
		((_pcnode)a)->loc.y == (*(_clocal*)b).y);
}

#ifdef TEST_PROJECT_CLOUDS
int main()
{
	int i = 0;
	int INIT_NODES[][4] = {
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
	int INIT_CONNECT[][2] = {
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
	printf("[cloud size] %d\n", cod->size(cod));

	_clocal xx = {
		.x = 2,
		.y = -1,
	};

	_pcnode  re = cod->find(cod, test_cloud_cond, &xx);
	if (re) {
		printf("%d %d %f %f", re->id, re->lev, re->loc.x, re->loc.y);
	}

	cod->free(&cod);
	for (i = 0; i < N; ++i) { pdata[i]->free(&pdata[i]); }
	free(pdata);
	return 0;
}
#endif