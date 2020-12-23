#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "test_def.h"
#include "../nc/cdsar.h"
#include "../nc/timer.h"

typedef struct _snode
{
	int a;
	int b;
}snode;

void local_qsort(snode* pdt, int st, int ed);

bool cmp(void* dt, int xi, int yi) { return ((snode*)dt + xi)->b <= ((snode*)dt + yi)->b; }
void agn(void* dt, int xi, int yi) {
	((snode*)dt + xi)->b = ((snode*)dt + yi)->b;
	//((snode*)dt + xi)->a = ((snode*)dt + yi)->a;
	//...
}

void test_ns_qsort(int len)
{
	double st, ed;
	snode* dt1 = (snode*)malloc(sizeof(snode) * (len + 1));
	snode* dt2 = (snode*)malloc(sizeof(snode) * (len + 1));

	if (dt1 == NULL || dt2 == NULL) {
		printf("[err] malloc failed.\n");
		return;
	}

	int i = 0;
	srand((unsigned)(time(0)));
	for (i = 1; i <= len; i++)   //dt[0]将作为buff被nc_qsort使用,待排序数据位置>=1
	{
		dt1[i].a = i;
		dt1[i].b = rand() % len;
		dt2[i].a = i;
		dt2[i].b = dt1[i].b;
	}

	GET_TIME(st);
	//
	nc_qsort(dt1, 1, len, cmp, agn); //第二个参数必须>=1, 待排数据索引范围 1~len(包括两个端点)
	//
	GET_TIME(ed);
	printf("nc_qsort elapsed = %lf\n", ed - st);

	GET_TIME(st);
	//
	local_qsort(dt2, 1, len);
	//
	GET_TIME(ed);
	printf("lc_qsort elapsed = %lf\n", ed - st);

	//for (i = 1; i <= len; i++)
	//{
	//	printf("[%d][%d] = %d, %d\n", dt1[i].a, dt2[i].a, dt1[i].b, dt2[i].b);
	//}

	free(dt1); dt1 = NULL;
	free(dt2); dt2 = NULL;
}

#ifdef TEST_PROJECT_SORT
int main()
{
	test_ns_qsort(100000);

	return 0;
}
#endif

//------------------------------------------------------------------
void local_qsort(snode* pdt, int st, int ed)
{
	int i, j;
	int n = ed - st + 1;

	if (50 <= n) {
		if (pdt[ed].b <= pdt[st].b) {
			pdt[0].b = pdt[st].b;; pdt[st].b = pdt[ed].b;; pdt[ed].b = pdt[0].b;
		}
		if (n == 2) { return; }

		pdt[0].b = pdt[st + (n >> 1)].b;
		pdt[st + (n >> 1)].b = pdt[st + 1].b;
		pdt[st + 1].b = pdt[0].b;
		if (pdt[st + 1].b <= pdt[st].b) { pdt[0].b = pdt[st].b; pdt[st].b = pdt[st + 1].b; }
		else if (pdt[ed].b <= pdt[st + 1].b) { pdt[0].b = pdt[ed].b; pdt[ed].b = pdt[st + 1].b; }
		if (n == 3) { pdt[st + 1].b = pdt[0].b; return; }

		i = st + 1, j = ed - 1;
		while (i < j) {
			while (i < j && pdt[0].b <= pdt[j].b) --j;
			pdt[i].b = pdt[j].b;
			while (i < j && pdt[i].b <= pdt[0].b) ++i;
			pdt[j].b = pdt[i].b;
		}
		agn(pdt, i, 0);
		if (i - st > 1) { nc_qsort(pdt, st, i - 1, cmp, agn); }
		if (ed - i > 1) { nc_qsort(pdt, i + 1, ed, cmp, agn); }
	}
	else {
		for (i = st + 1; i <= ed; i++) {
			pdt[0].b = pdt[i].b;
			for (j = i; j > st && pdt[0].b <= pdt[j - 1].b; --j) {
				pdt[j].b = pdt[j - 1].b;
			}
			pdt[j].b = pdt[0].b;
		}
	}
}