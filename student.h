#ifndef STUDENT_H
#define STUDENT_H

#include "tools.h"

#define S_DATA_PATH     "data/s_data.bin"
#define SD_DATA_PATH    "data/sd_data.bin"
#define STU_MAX         50

typedef struct Student
{
    uint32_t    id;        // 学号
    uint32_t    pd;        // 密码
    char        name[20];  // 姓名
    char        sex;       // 性别
    uint16_t    age;       // 年龄
    float       score_c;   // 语文
    float       score_m;   // 数学
    float       score_e;   // 英语
    float       score_s;   // 总分
    float       score_a;   // 平均
    int8_t      rank;      // 排名
    bool        is_stop;   // 是否禁用
}Student;

// 学生数组指针
extern Student* stu_a;
// 学生数组使用量
extern size_t stu_n;

// 学生登陆
int stu_login(const char* id_p,const char* pd_p);
// 进入学生子系统
void stu_enter(Student* stu_p);
// 显示学生信息
void stu_show(Student* stu_p);
// 显示学生成绩
void stu_show_score(Student* stu_p);
// 语文成绩排名
void stu_rank_c(void);
// 数学成绩排名
void stu_rank_m(void);
// 英语成绩排名
void stu_rank_e(void);
// 总分成绩排名
void stu_rank_s(void);

#endif//STUDENT_H
