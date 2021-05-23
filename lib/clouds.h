/*<!-- author:urays email:urays@foxmail.com date: 2020.10.28 -->*/
#ifndef _U_NODE_CLOUDS_H
#define _U_NODE_CLOUDS_H

#include "cdsa.h"

_LASM_C_BEGIN

//cloud node
typedef struct __nc_clocal { float x, y; }_clocal;
typedef struct __nc_cnode {
	int id;                 //节点标识
	int lev;                //节点等级
	struct __nc_clocal loc;   //节点坐标

	_plist ajnds;//邻居节点
	void(*contact)(struct __nc_cnode*, struct __nc_cnode*);
	_plist(*childs)(struct __nc_cnode*);
	_plist(*father)(struct __nc_cnode*);
	void(*free)(struct __nc_cnode**);
}*_pcnode;

_pcnode cnode_create(int _id, int _lev, float _x, float _y);

//cloud
typedef struct __nc_cloud {
	//lev max node'id = 0
	_plist _reg;

	void(*join)(struct __nc_cloud*, void*);  //在点云中添加一个新节点
	_pcnode(*find)(struct __nc_cloud*, uint8_t(*cond)(const void*, const void*), const void*);
	uint32_t(*size)(struct __nc_cloud*);
	void(*free)(struct __nc_cloud**);
}*_pcloud;

_pcloud cloud_create();

_LASM_C_END
#endif