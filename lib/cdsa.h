/*<!-- author:urays email:urays@foxmail.com date: 2020.10.28 -->*/
#ifndef LASM_C_STD_STRUCTURE_H
#define LASM_C_STD_STRUCTURE_H

#include <stdint.h>

#ifdef __cplusplus
#define _LASM_C_BEGIN  extern "C" { 
#define _LASM_C_END    }
#else
#define _LASM_C_BEGIN 
#define _LASM_C_END  
#endif

_LASM_C_BEGIN

typedef struct __nc_snode {
	void* e; struct __nc_snode* next;
}*_psnode, * _pcache;

#define HASHTAB_SIZE      (32)

//TABLE
typedef struct __nc_table {
	_pcache* _tab;

	uint32_t size;
	uint8_t(*contain)(struct __nc_table*, const void*);
	//void* (*find)(struct __nc_table*, uint8_t(*cond)(const void*,const void*),const void*);
	uint8_t(*join)(struct __nc_table*, void*);
	void(*remove)(struct __nc_table*, const  void*);
	void(*clean)(struct __nc_table**);
	void(*free)(struct __nc_table**);
}*_ptable;

_ptable table_create();

//STACK
typedef struct __nc_stack {
	struct __nc_snode* _ptop; //栈顶指针

	uint8_t(*empty)(struct __nc_stack*);	/* 判断栈是否为空*/
	uint32_t(*size)(struct __nc_stack*);	/* 获取栈的大小*/
	void(*push)(struct __nc_stack*, void* _e);/* 压入一个新元素到栈中*/
	uint8_t(*_cmp)(const void*, const void*); /* 创建最小栈/最大栈 如果cmp返回值永远是true则为普通栈*/
	void* (*top)(struct __nc_stack*);	/* 获取栈顶元素*/
	void* (*pop)(struct __nc_stack*);	/* 弹出栈顶元素,并返回弹出元素*/
	void(*clean)(struct __nc_stack*);	/* 清空栈中元素*/
	void(*free)(struct __nc_stack**); /* 销毁_pstk指向的堆空间,并将_pstk置NULL,释放内存*/
}*_pstack;

_pstack stack_create(uint8_t(*cmp)(const void*, const void*));
//uint8_t cmp(const void* a, const void* b) { return (*(int*)a < *(int*)b); } 最小栈
//uint8_t cmp(const void* a, const void* b) { return (*(int*)a > *(int*)b); } 最大栈

//QUEUE
typedef struct __nc_queue {
	struct __nc_snode* _phead; //队列首指针
	struct __nc_snode* _prear; //队列尾指针

	uint8_t(*empty)(struct __nc_queue*);	/* 判断队列是否为空*/
	uint32_t(*size)(struct __nc_queue*);	/* 获取队列的长度*/
	void(*push)(struct __nc_queue*, void* _e);/* 添加一个新元素到队列尾*/
	uint8_t(*_cmp)(const void*, const void*); /* 创建优先队列 如果cmp返回值永远是false则为普通队列,永远为true则为普通栈*/
	void* (*pop)(struct __nc_queue*);	/* 弹出队列首元素,并返回弹出元素*/
	void* (*front)(struct __nc_queue*);/* 获取队列首元素*/
	void* (*back)(struct __nc_queue*);	/* 获取队列尾元素*/
	void(*clean)(struct __nc_queue*);	/* 清空队列中的所有元素*/
	void(*free)(struct __nc_queue**); /* 销毁_pque指向的堆空间,并将_pque置NULL,释放内存*/
}*_pqueue;

_pqueue queue_create(uint8_t(*cmp)(const void*, const void*));
//uint8_t cmp(const void* a, const void* b) { return (*(int*)a < *(int*)b); } 小->大
//uint8_t cmp(const void* a, const void* b) { return (*(int*)a > *(int*)b); } 大->小

//LIST
typedef struct __nc_list {
	struct __nc_snode* _phead;
	struct __nc_snode* _pcur;  //当前活动指针

	struct __nc_table* _tab;  //加速查找冗余
	uint8_t(*contain)(struct __nc_list*, const void*); //判断当前列表是否包含元素

	uint32_t(*size)(struct __nc_list*);  /* 获取列表的长度*/
	void* (*next)(struct __nc_list*); //获取下一个节点地址
	void(*reset)(struct __nc_list*);  //初始化活动指针到队列首,调用next之前用
	void(*add)(struct __nc_list*, void*); //添加一个新元素到列表(若没有比较函数,则默认插入队列首)
	uint8_t(*_cmp)(const void*, const void*);  //创建有序队列,
	void* (*find)(struct __nc_list*, uint8_t(*cond)(const void*, const void* _o), const void* _o);
	void(*remove)(struct __nc_list*, uint8_t(*cond)(const void*, const void* _o), const void* _o); //移除满足cond的一系列元素
	//void(*join)(struct __nc_list* _ldst, struct __nc_list* _lsrc,)
	void(*clean)(struct __nc_list*);  //清空队列中的所有元素
	void(*free)(struct __nc_list**); //销毁_plist指向的堆空间,并将_plist置为NULL,释放内存
}*_plist;

_plist list_create(uint8_t(*cmp)(const void*, const void*));
//uint8_t cmp(const void* x, const void* y) { return (*(int*)x > * (int*)y); } 升序
//uint8_t cmp(const void* x, const void* y) { return (*(int*)x < * (int*)y); } 降序
//uint8_t cond(const void* x, const void* y) { return (*(node*)x).a == *(*int)y; }

#define HEAP_INISIZE       (32)  //初始堆大小
#define HEAP_ADDSIZE       (64)  //增加堆大小

//HEAP
typedef struct __nc_heap {
	void** _dt;
	uint32_t _cap;
	uint32_t _sz;

	uint8_t(*empty)(struct __nc_heap*);
	uint8_t(*_cmp)(const void*, const void*);  //最大堆或最小堆. >:最大堆;<:最小堆
	void(*insert)(struct __nc_heap*, void*); //插入一个元素
	void* (*pop)(struct __nc_heap*); //弹出最大/最小值,并返回
	void* (*top)(struct __nc_heap*); //获取最大/最小值
	void(*clean)(struct __nc_heap*);
	void(*free)(struct __nc_heap**);
}*_pheap;
_pheap heap_create(uint8_t(*cmp)(const void*, const void*));
//uint8_t cmp_max(const void* x, const void* y) { return *((int*)x) > *((int*)y); }  >:最大堆
//uint8_t cmp_min(const void* x, const void* y) { return ((hnode*)x)->a < ((hnode*)y)->a; }  <:最小堆

//ALGORITHM
void nc_qsort(void* pdt, uint32_t st, uint32_t ed,
	uint8_t(*cmp)(void*, uint32_t, uint32_t), void(*agn)(void*, uint32_t, uint32_t));
//!pdt[0]将作为buff被使用,待排数据至少从索引1开始,即st>=1;待排数据索引范围 st~ed(包括两个端点)
//uint8_t cmp(void* dt, uint32 xi, uint32 yi) { return ((snode*)dt + xi)->b <= ((snode*)dt + yi)->b; } 小->大
//uint8_t cmp(void* dt, uint32 xi, uint32 yi) { return ((snode*)dt + xi)->b >= ((snode*)dt + yi)->b; } 大->小
//void agn(void* dt, uint32 xi, uint32 yi) { ((int*)dt + xi)->b = ((int*)dt + yi)->b; }  assign function

//NOTE:__nc_list也能使用nc_qsort,但实测效率低,就删了:(

_LASM_C_END
#endif
