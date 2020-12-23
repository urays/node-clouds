/*<!-- author:urays email:urays@foxmail.com date: 2020.10.28 -->*/
#ifndef _NCLOUD_STD_STRUCTURE_H
#define _NCLOUD_STD_STRUCTURE_H

#ifdef __cplusplus
#define _EXTERN_ extern "C"
#else
#define _EXTERN_ extern
#endif

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef bool
#define bool short
#endif
#ifndef uint8
#define uint8 unsigned char
#endif
#ifndef uint16
#define uint16 unsigned short
#endif
#ifndef uint32
#define uint32 unsigned int
#endif
#ifndef uint64
#define uint64 unsigned long long
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER)  ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, m) ((type*)((char*)ptr - offsetof(type, m)))
#endif

typedef struct __nc_snode {
	void* data; struct __nc_snode* next;
}*_psnode, * _pcache;

#define HASHTAB_SIZE      (32)

//TABLE
typedef struct __nc_table {
	_pcache* _tab;

	uint32 size;
	bool(*contain)(struct __nc_table*, void*);
	//void* (*find)(struct __nc_table*, bool(*cond)(void*, void*), void*);
	bool(*join)(struct __nc_table*, void*);
	void(*remove)(struct __nc_table*, void*);
	void(*clean)(struct __nc_table**);
	void(*free)(struct __nc_table**);
}*_ptable;

_EXTERN_ _ptable table_create();

//STACK
typedef struct __nc_stack {
	struct __nc_snode* _ptop; //栈顶指针

	bool(*empty)(struct __nc_stack*);	/* 判断栈是否为空*/
	uint32(*size)(struct __nc_stack*);	/* 获取栈的大小*/
	void(*push)(struct __nc_stack*, void* _dt);/* 压入一个新元素到栈中*/
	bool(*cmp)(void*, void*); /* 创建最小栈/最大栈 如果cmp返回值永远是true则为普通栈*/
	void* (*top)(struct __nc_stack*);	/* 获取栈顶元素*/
	void* (*pop)(struct __nc_stack*);	/* 弹出栈顶元素,并返回弹出元素*/
	void(*clean)(struct __nc_stack*);	/* 清空栈中元素*/
	void(*free)(struct __nc_stack**); /* 销毁_pstk指向的堆空间,并将_pstk置NULL,释放内存*/
}*_pstack;

_EXTERN_ _pstack stack_create(bool(*cmp)(const void*, const void*));
//bool cmp(const void* a, const void* b) { return (*(int*)a < *(int*)b); } 最小栈
//bool cmp(const void* a, const void* b) { return (*(int*)a > *(int*)b); } 最大栈

//QUEUE
typedef struct __nc_queue {
	struct __nc_snode* _phead; //队列首指针
	struct __nc_snode* _prear; //队列尾指针

	bool(*empty)(struct __nc_queue*);	/* 判断队列是否为空*/
	uint32(*size)(struct __nc_queue*);	/* 获取队列的长度*/
	void(*push)(struct __nc_queue*, void* _dt);/* 添加一个新元素到队列尾*/
	bool(*cmp)(void*, void*); /* 创建优先队列 如果cmp返回值永远是false则为普通队列,永远为true则为普通栈*/
	void* (*pop)(struct __nc_queue*);	/* 弹出队列首元素,并返回弹出元素*/
	void* (*front)(struct __nc_queue*);/* 获取队列首元素*/
	void* (*back)(struct __nc_queue*);	/* 获取队列尾元素*/
	void(*clean)(struct __nc_queue*);	/* 清空队列中的所有元素*/
	void(*free)(struct __nc_queue**); /* 销毁_pque指向的堆空间,并将_pque置NULL,释放内存*/
}*_pqueue;

_EXTERN_ _pqueue queue_create(bool(*cmp)(const void*, const void*));
//bool cmp(const void* a, const void* b) { return (*(int*)a < *(int*)b); } 小->大
//bool cmp(const void* a, const void* b) { return (*(int*)a > *(int*)b); } 大->小

//LIST
typedef struct __nc_list {
	struct __nc_snode* _phead;
	struct __nc_snode* _pcur;  //当前活动指针

	struct __nc_table* _tab;  //加速查找冗余
	bool(*contain)(struct __nc_list*, void*); //判断当前列表是否包含元素

	uint32(*size)(struct __nc_list*);  /* 获取列表的长度*/
	void* (*next)(struct __nc_list*); //获取下一个节点地址
	void(*reset)(struct __nc_list*);  //初始化活动指针
	void(*add)(struct __nc_list*, void*); //添加一个新元素到列表
	bool(*cmp)(void*, void*);  //创建有序队列,
	void* (*find)(struct __nc_list*, bool(*cond)(void*, void*), void*);
	void(*remove)(struct __nc_list*, bool(*cond)(void*, void*), void*); //移除满足cond的一系列元素
	//void(*join)(struct __nc_list* _ldst, struct __nc_list* _lsrc,)
	void(*clean)(struct __nc_list*);  //清空队列中的所有元素
	void(*free)(struct __nc_list**); //销毁_plist指向的堆空间,并将_plist置为NULL,释放内存
}*_plist;

_EXTERN_ _plist list_create(bool(*cmp)(const void*, const void*));
//bool cmp(const void* x, const void* y) { return (*(int*)x > * (int*)y); } 升序
//bool cmp(const void* x, const void* y) { return (*(int*)x < * (int*)y); } 降序
//bool cond(const void* x, const void* y) { return (*(node*)x).a == *(*int)y; }

//ALGORITHM
_EXTERN_ void nc_qsort(void* pdt, int st, int ed, bool(*cmp)(void*, int, int), void(*agn)(void*, int, int));
//!pdt[0]将作为buff被使用,待排数据至少从索引1开始,即st>=1;待排数据索引范围 st~ed(包括两个端点)
//bool cmp(void* dt, int xi, int yi) { return ((snode*)dt + xi)->b <= ((snode*)dt + yi)->b; } 小->大
//bool cmp(void* dt, int xi, int yi) { return ((snode*)dt + xi)->b >= ((snode*)dt + yi)->b; } 大->小
//void agn(void* dt, int xi, int yi) { ((int*)dt + xi)->b = ((int*)dt + yi)->b; }  assign function

//NOTE:__nc_list也能使用nc_qsort,但实测效率低,就删了:(

#endif
