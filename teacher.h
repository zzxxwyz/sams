#ifndef TEACHER_H
#define TEACHER_H
#include "status.h"

#define T_DATA_PATH     "data/t_data.bin"
#define TD_DATA_PATH    "data/td_data.bin"
#define TCH_MAX         10

typedef struct Teacher
{
    uint32_t    id;         // 工号
    uint32_t    pd;         // 密码
    char        name[20];   // 姓名
    char        sex;        // 性别
    uint16_t    age;        // 年龄
    uint16_t    age_w;      // 工龄
    bool        is_stop;    // 是否停用
}Teacher;

extern Teacher* tch_a;
extern uint32_t tch_n;

// 教师登陆
int tch_login(const char* id_p,const char* pd_p);

// 进入教师子系统
void tch_enter(Teacher* tch_p);

// 显示老师基本信息
void tch_show(Teacher* tch_p);

#endif//TEACHER_H
