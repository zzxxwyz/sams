#ifndef SAMS_H
#define SAMS_H

#include <stdint.h>

#define ID_PATH "data/id.bin"

// 申请内存加载数据 
void sams_init (void);
// 开始运行管理系统
void sams_start (void);
// 保存数据释放内存
void sams_exit (void);

extern uint32_t* id_a;

#endif//SAMS_H
