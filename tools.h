#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define pf  printf
#define sf  scanf

// 获取当前年份
uint16_t get_current_year(void);
// 清理屏幕
void clear_src(void);
// 任意键继续
void anykey_continue(void);
// 显示提示信息
void show_msg(const char* msg,time_t t);
// 从键盘获取指令
char get_cmd(char begin_cmd,char end_cmd);
// 获取性别
char get_sex(void);
// 获取年份
uint16_t get_year(void);
// 获取成绩
bool get_score(float* score_p);
// 从键盘获取密码
char* get_passwd(char* pd,size_t hope_len,bool is_show);
// 从键盘获取字符串
char* get_str(char* str,size_t hope_len);
// 从文件加载数据
int load_data(const char* path,void* ptr,size_t node_size);
// 保存数据到文件
int save_data(const char* path,const void* ptr,size_t node_size,size_t num);

#endif//TOOLS_H
