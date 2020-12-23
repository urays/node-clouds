/*<!-- author:urays email:urays@foxmail.com date: 2020.10.28 -->*/
#ifndef _NCLOUD_NODE_CLOUDS_H
#define _NCLOUD_NODE_CLOUDS_H

#include "cdsar.h"

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

_EXTERN_ _pcnode cnode_create(int _id, int _lev, float _x, float _y);

//cloud
typedef struct __nc_cloud {
	//lev max node'id = 0
	_plist _reg;

	void(*join)(struct __nc_cloud*, void*);  //在点云中添加一个新节点
	_pcnode(*find)(struct __nc_cloud*, bool(*cond)(void*, void*), void*);
	uint32(*size)(struct __nc_cloud*);
	void(*free)(struct __nc_cloud**);
}*_pcloud;

_EXTERN_ _pcloud cloud_create();

#endif