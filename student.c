#include "student.h"
#include "tools.h"

// 学生数组指针
Student* stu_a;
// 学生数组使用量
uint32_t stu_n;

// 语言成绩比较
int stu_cmp_c(const void* stu_p1,const void* stu_p2)
{
    return ((Student*)stu_p1)->score_c < ((Student*)stu_p2)->score_c;
}

// 数学成绩比较
int stu_cmp_m(const void* stu_p1,const void* stu_p2)
{
    return ((Student*)stu_p1)->score_m < ((Student*)stu_p2)->score_m;
}

// 英语成绩比较
int stu_cmp_e(const void* stu_p1,const void* stu_p2)
{
    return ((Student*)stu_p1)->score_e < ((Student*)stu_p2)->score_e;
}

// 总分成绩比较
int stu_cmp_s(const void* stu_p1,const void* stu_p2)
{
    return ((Student*)stu_p1)->score_s < ((Student*)stu_p2)->score_s;
}

// 学生登陆
int stu_login(const char* id_p,const char* pd_p)
{
    uint32_t id = atoi(id_p);
    uint32_t pd = atoi(pd_p);
    
    int i = 0;
    for(; i<stu_n; i++)
    {
        if(id == stu_a[i].id) break;
    }
   
    if(i == stu_n) return IDERR;
    
    if(stu_a[i].is_stop)
    {
        show_msg("***此帐号被锁,请联系教师解锁!***",1);
        return RIGHT;
    }
    
    if(pd != stu_a[i].pd) return PDERR;
    
    stu_enter(stu_a+i);
    return RIGHT;
}

// 语文成绩排名
void stu_rank_c(void)
{
    qsort(stu_a,stu_n,sizeof(Student),stu_cmp_c);
    for(int i=0; i<stu_n; i++)
        stu_a[i].rank = i+1;
}

// 数学成绩排名
void stu_rank_m(void)
{
    qsort(stu_a,stu_n,sizeof(Student),stu_cmp_m);
    for(int i=0; i<stu_n; i++)
        stu_a[i].rank = i+1;
}

// 英语成绩排名
void stu_rank_e(void)
{
    qsort(stu_a,stu_n,sizeof(Student),stu_cmp_e);
    for(int i=0; i<stu_n; i++)
        stu_a[i].rank = i+1;
}

// 总分成绩排名
void stu_rank_s(void)
{
    qsort(stu_a,stu_n,sizeof(Student),stu_cmp_s);
    for(int i=0; i<stu_n; i++)
        stu_a[i].rank = i+1;
}

// 修改登陆密码
void stu_mod_pd(Student* stu_p)
{
    pf("请输入新密码:");
    char pd[20] = {};
    get_passwd(pd,20,true);
    char re_pd[20] = {};
    pf("请再次输入新密码:");
    get_passwd(re_pd,20,true);
    if(strcmp(pd,re_pd))
    {
        show_msg("*** 两次输入密码不一致,修改密码失败! ***",1);
        return;
    }
    
    stu_p->pd = atoi(pd);
    show_msg("*** 修改密码成功! ***",1);
}

// 进入学生子系统
void stu_enter(Student* stu_p)
{
    while(true)
    {
        clear_src();
        puts("***进入学生子系统***");
        puts("   0.语文成绩排名");
        puts("   1.数学成绩排名");
        puts("   2.英语成绩排名");
        puts("   3.总分成绩排名");
        puts("   4.修改登陆密码");
        puts("   5.退出学生子系统");
        puts("------------------");
        switch(get_cmd('0','5'))
        {
            case '0': 
                stu_rank_c(); 
                pf("语文成绩:%g",stu_p->score_c); 
                break;
            case '1': 
                stu_rank_m(); 
                pf("数学成绩:%g",stu_p->score_m); 
                break;
            case '2': 
                stu_rank_e(); 
                pf("英语成绩:%g",stu_p->score_e); 
                break;
            case '3': 
                stu_rank_s(); 
                pf("总分成绩:%g",stu_p->score_s); 
                break;
            case '4': stu_mod_pd(stu_p); continue;
            case '5': return;
        }
        pf(" 排名第%u \n",stu_p->rank);
        anykey_continue();
    }
}

// 显示学生信息
void stu_show(Student* stu_p)
{
    pf("学号:%u 姓名:%s 性别:%s 年龄:%hu\n",stu_p->id,stu_p->name,
        'm' == stu_p->sex ? "男" : "女",get_current_year() - stu_p->age);
}

// 显示学生成绩
void stu_show_score(Student* stu_p)
{
    pf("学号:%u 姓名:%s 语文:%g 数学%g 英语:%g 平均%g 总分%g\n",
        stu_p->id,stu_p->name,stu_p->score_c,stu_p->score_m,
        stu_p->score_e,stu_p->score_a,stu_p->score_s);
}
