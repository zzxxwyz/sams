#ifndef MANAGER_H
#define MANAGER_H

#include "tools.h"

#define M_DATA_PATH     "data/m_data.bin"
#define MD_DATA_PATH    "data/md_data.bin"
#define MGR_MAX     5

typedef struct Manager
{
    uint32_t    id;         // 帐号
    uint32_t    pd;         // 密码
    bool        is_stop;    // 是否停用
}Manager;

// 管理员数组指针
extern Manager* mgr_a;
// 管理员数组使用量
extern uint32_t mgr_n;

// 管理员登陆
int mgr_login(const char* id_p,const char* pd_p);
// 进行管理员子系统
void mgr_enter(Manager* mgr_p);
// 显示管理员
void mgr_show(Manager* mgr_p);

#endif//MANAGER_H
