#include <strings.h>
#include "teacher.h"
#include "student.h"
#include "sams.h"
#include "status.h"

Teacher* tch_a;
size_t tch_n;

int tch_find_stu(uint32_t id)
{
    for(int i=0; i<stu_n; i++)
    {
        if(id == stu_a[i].id) return i;
    }
    
    return ERROR;
}

// 单个添加学生
void tch_add_one_stu()
{
    bzero(stu_a+stu_n,sizeof(Student));
    
    pf("请输入学生姓名:");
    get_str(stu_a[stu_n].name,20);
    pf("请输入学生性别:");
    stu_a[stu_n].sex = get_sex();
    pf("请输入学生出生年份:");
    stu_a[stu_n].age = get_year();
    
    stu_a[stu_n].id = id_a[2]++;
    stu_a[stu_n].pd = 222222;
    stu_n++;
    
    show_msg("*** 添加学生成功! ***",1);
}

// 批量添加学生
void tch_add_multi_stu()
{
    char path[30] = {};
    pf("请输入要导入的文件名:");
    get_str(path,30);
    FILE* frp = fopen(path,"r");
    if(NULL == frp)
    {
        show_msg("*** 输入的路径有误! ***",1);
    }
    
    Student* stu_p = stu_a+stu_n;
    size_t num_r = 0;
    size_t num_s = 0;
    while(stu_n<STU_MAX)
    {
        bzero(stu_p,sizeof(Student));
        if(!fscanf(frp,"%s %c %hu\n",
            stu_p->name,&stu_p->sex,&stu_p->age))
            break;
            
        num_r++;
        if('f' != stu_p->sex && 'm' != stu_p->sex) continue;
        if(1950 > stu_p->age || stu_p->age > get_current_year()-5) continue;
           
        stu_p->id = id_a[1]++;
        stu_p->pd = 333333;
        
        stu_show(stu_p);
        stu_n++;
        stu_p++;
        num_s++;
    }
    printf("*** 共读取到%u条数据,成功添加%u位教师! ***",num_r,num_s);
    anykey_continue();  
}

// 添加学生
void tch_add_stu(void)
{
    if(stu_n >= STU_MAX)
    {
        show_msg("*** 学生已满,无法添加! ***",1);
        return;
    }
    
    clear_src();
    puts("***添加学生***");
    puts("   0.单个添加");
    puts("   1.批量添加");
    puts("-------------");
    switch(get_cmd('0','1'))
    {
        case '0': tch_add_one_stu(); break;
        case '1': tch_add_multi_stu(); break;
    } 
}

// 删除学生
void tch_del_stu(void)
{
    if(0 >= stu_n)
    {
        show_msg("*** 系统内无学生! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要删除的学生帐号:");
    sf("%u",&id);
    
    int index = tch_find_stu(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    
    pf("请再次输入要删除的管理员帐号:");
    sf("%u",&id);
    if(id != stu_a[index].id)
    {
        show_msg("*** 两次输入的帐号不符,删除失败! ***",1);
        return;
    }
    
    FILE* fap = fopen(SD_DATA_PATH,"a");
    fwrite(stu_a+index,sizeof(Student),1,fap);
    fclose(fap);
        
    memcpy(stu_a+index,stu_a+index+1,(stu_n-index-1)*sizeof(Student));
    show_msg("*** 删除帐号成功! ***",1);     
    stu_n--;
}

// 修改学生信息
void tch_mod_stu(void)
{
    uint32_t id;
    pf("请输入要修改的学生帐号:");
    sf("%u",&id);
    int index = tch_find_stu(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    while(true)
    {
        clear_src();
        stu_show(stu_a+index);
        puts("***请选择要修改的学生信息***");
        puts("   0.修改学生姓名");
        puts("   1.修改学生性别");
        puts("   2.修改学生出生年份");
        puts("   3.返回上一级");
        puts("-------------------------");
        switch(get_cmd('0','4'))
        {
            case '0': 
                pf("请输入学生姓名:");
                get_str(stu_a[index].name,20); break;
            case '1':
                pf("请输入学生性别:");
                stu_a[index].sex = get_sex(); break;
            case '2': 
                pf("请输入学生出生年份:");
                stu_a[index].age = get_year(); break;
            case '3': return;
        }
        show_msg("*** 修改成功! ***",1);
    }
}

// 重置学生密码
void tch_reset_stu(void)
{
    if(0 >= stu_n)
    {
        show_msg("*** 系统内无学生! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要重置的学生帐号:");
    sf("%u",&id);
    int index = tch_find_stu(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    pf("请再次输入要重置的教师帐号:");
    sf("%u",&id);
    if(id != stu_a[index].id)
    {
        show_msg("*** 两人些输入的帐号不符,重置失败! ***",1);
        return;
    }
    
    stu_a[index].pd = 333333;
    show_msg("*** 重置密码成功! ***",1);
}

// 解锁学生帐号
void tch_nulock_stu(void)
{
    if(0 >= stu_n)
    {
        show_msg("*** 系统内无学生! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要解锁的学生帐号:");
    sf("%u",&id);
    int index = tch_find_stu(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    if(!stu_a[index].is_stop)
    {
        show_msg("*** 此帐号未被锁! ***",1);
        return;
    }
    stu_a[index].is_stop = false;
    show_msg("*** 解锁帐号成功! ***",1); 
}

// 查询学生信息
void tch_query_stu(void)
{
    char name[20] = {};
    pf("请输入要查询的学生姓名:");
    get_str(name,20);
    
    bool flag = true;
    for(int i=0; i<stu_n; i++)
    {
        if(!strcmp(name,stu_a[i].name))
        {
            stu_show(stu_a+i);
            flag = false;
        }
    }
    
    flag ? show_msg("*** 查无此人! ***",1) : anykey_continue();
}

// 显示在读学生
void tch_inshl_stu(void)
{

    if(0 >= stu_n)
    {
        show_msg("*** 系统内无学生! ***",1);
        return;
    }

    clear_src();
    puts("******在读学生******");
    for(int i=0; i<stu_n; i++)
    {
        stu_show(stu_a+i);
    }
    anykey_continue();
}

// 显示退学学生
void tch_drop_stu(void)
{
    FILE* frp = fopen(SD_DATA_PATH,"r");
    if(NULL == frp)
    {
        show_msg("*** 斩无退学学生! ***",1);
        return;
    }
    
    clear_src();
    puts("******所有退学学生******");
    Student stu;
    while(fread(&stu,sizeof(stu),1,frp)) stu_show(&stu);
    fclose(frp);
    
    anykey_continue();
}

// 录入单个学生的成绩
void tch_input_one_stu()
{
    if(0 >= stu_n)
    {
        show_msg("*** 系统内无学生! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要修改成绩的学生帐号:");
    sf("%u",&id);
    int index = tch_find_stu(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    
    
    pf("请输入语文,数学,英语成绩:");
    float sc = 0 , sm = 0 , se = 0;
    sf("%f%f%f",&sc,&sm,&se);
    if(sc < 0 || sc > 100 || sm < 0 || sm > 100 || se < 0 || se > 100)
    {
        show_msg("*** 输入的成绩有误! ***",1);
        return;
    }
    stu_a[index].score_c = sc;
    stu_a[index].score_m = sm;
    stu_a[index].score_e = se;
    stu_a[index].score_s = sc + sm + se;
    stu_a[index].score_a = stu_a[index].score_s /3;
    stu_show_score(stu_a+index);
    show_msg("*** 录入成功! ***",1);
}

void tch_input_multi_stu()
{
    char path[30] = {};
    pf("请输入要导入的文件名:");
    get_str(path,30);
    FILE* frp = fopen(path,"r");
    if(NULL == frp)
    {
        show_msg("*** 输入的路径有误! ***",1);
    }
    
    size_t num_r = 0;
    size_t num_s = 0;
    uint32_t id = 0;
    int index = 0;
    float sc = 0 , sm = 0 , se = 0;
    while(fscanf(frp,"%u %f %f %f\n",&id,&sc,&sm,&se))
    {      
        num_r++;
         
        if(ERROR == (index = tch_find_stu(id))) continue;
        if(sc < 0 || sc > 100 || sm < 0 || sm > 100 || se < 0 || se > 100)
            continue;
        stu_a[index].score_c = sc;
        stu_a[index].score_m = sm;
        stu_a[index].score_e = se;
        stu_a[index].score_s = sc + sm + se;
        stu_a[index].score_a = stu_a[index].score_s /3;
        stu_show_score(stu_a+index);
        num_s++;
    }
    printf("*** 共读取到%u条数据,成功添加%u位教师! ***",num_r,num_s);
    anykey_continue();  
}

// 录入学生成绩
void tch_input_scr(void)
{
    if(0 >= stu_n)
    {
        show_msg("*** 系统内无学生! ***",1);
        return;
    }
    
    clear_src();
    puts("***添加学生***");
    puts("   0.单个录入");
    puts("   1.批量录入");
    puts("-------------");
    switch(get_cmd('0','1'))
    {
        case '0': tch_input_one_stu(); break;
        case '1': tch_input_multi_stu(); break;
    } 
}

// 修改学生成绩
void tch_mod_scr(void)
{
    if(0 >= stu_n)
    {
        show_msg("*** 系统内无学生! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要修改成绩的学生帐号:");
    sf("%u",&id);
    int index = tch_find_stu(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }

    while(true)
    {
        clear_src();
        stu_show_score(stu_a+index);
        puts("***请选择要修改的成绩的科目**");
        puts("   0.修改语文成绩");
        puts("   1.修改数学成绩");
        puts("   2.修改英语成绩");
        puts("   3.返回上一级");
        puts("-------------------------");
        switch(get_cmd('0','3'))
        {
            case '0': 
                pf("请输入语文成绩:");
                if(!get_score(&stu_a[index].score_c)) continue;
                break;
            case '1':
                pf("请输入数学成绩:");
                if(!get_score(&stu_a[index].score_m)) continue;
                break;
            case '2': 
                pf("请输入英语成绩:");
                if(!get_score(&stu_a[index].score_e)) continue;
                break;
            case '3': return;
        }
        stu_a[index].score_s = stu_a[index].score_c + 
            stu_a[index].score_m + stu_a[index].score_e;
        stu_a[index].score_a = stu_a[index].score_s /3;  
        show_msg("*** 修改成功! ***",1);
    } 
}

// 查询学生成绩
void tch_query_scr(void)
{
    char name[20] = {};
    pf("请输入要查询的学生姓名:");
    get_str(name,20);
    
    bool flag = true;
    for(int i=0; i<stu_n; i++)
    {
        if(!strcmp(name,stu_a[i].name))
        {
            stu_show_score(stu_a+i);
            flag = false;
        }
    }
    
    flag ? show_msg("*** 查无此人! ***",1) : anykey_continue();
}

// 各科成绩排名
void tch_rank_scr(void)
{
    while(true)
    {
        clear_src();
        puts("***查看各科成绩排名***");
        puts("   0.语文成绩排名");
        puts("   1.数学成绩排名");
        puts("   2.英语成绩排名");
        puts("   3.总分成绩排名");
        puts("   4.退出");
        puts("------------------");
        switch(get_cmd('0','4'))
        {
            case '0': stu_rank_c(); break;
            case '1': stu_rank_m(); break;
            case '2': stu_rank_e(); break;
            case '3': stu_rank_s(); break;
            case '4': return;
        }
        for(int i=0; i<stu_n; i++)
        {
            stu_show_score(stu_a+i);
        }
        anykey_continue();
    }
}

// 修改登陆密码
void tch_mod_pd(Teacher* tch_p)
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
    
    tch_p->pd = atoi(pd);
    show_msg("*** 修改密码成功! ***",1);
}

// 教师登陆
int tch_login(const char* id_p,const char* pd_p)
{
    uint32_t id = atoi(id_p);
    uint32_t pd = atoi(pd_p);
    
    int i = 0;
    for(; i<tch_n; i++)
    {
        if(id == tch_a[i].id) break;
    }
    
    if(i == tch_n) return IDERR;
    
    if(tch_a[i].is_stop)
    {
        show_msg("***此帐号被锁,请联系管理员解锁!***",1);
        return RIGHT;
    }
    
    if(pd != tch_a[i].pd) return PDERR;
    
    tch_enter(tch_a+i);
    return RIGHT;
}



// 进入教师子系统
void tch_enter(Teacher* tch_p)
{
    while(true)
    {
        clear_src();
        pf("教师:%u  姓名:%s\n",tch_p->id,tch_p->name);
        puts("***进入教师子系统***");
        puts("   0.添加学生");
        puts("   1.删除学生");
        puts("   2.修改学生信息");
        puts("   3.重置学生密码");
        puts("   4.解锁学生帐号");
        puts("   5.查询学生信息");
        puts("   6.显示在读学生");
        puts("   7.显示退学学生");
        puts("   8.录入学生成绩");
        puts("   9.修改学生成绩");
        puts("   a.查询学生成绩");
        puts("   b.各科成绩排名");
        puts("   c.修改登陆密码");
        puts("   d.退出教师子系统");
        puts("------------------");
        switch(get_cmd('0','d'))
        {
            case '0': tch_add_stu(); break;
            case '1': tch_del_stu(); break;
            case '2': tch_mod_stu(); break;
            case '3': tch_reset_stu(); break;
            case '4': tch_nulock_stu(); break;
            case '5': tch_query_stu(); break;
            case '6': tch_inshl_stu(); break;
            case '7': tch_drop_stu(); break;
            case '8': tch_input_scr(); break;
            case '9': tch_mod_scr(); break;
            case 'a': tch_query_scr(); break;
            case 'b': tch_rank_scr(); break;
            case 'c': tch_mod_pd(tch_p); break;
            case 'd': return;
        }
    }
}

// 显示老师基本信息
void tch_show(Teacher* tch_p)
{
    pf("帐号:%u 姓名:%s 性别:%s 年龄:%hhu 工龄:%hhu 状态:%s\n",
        tch_p->id,tch_p->name,'m' == tch_p->sex ? "男" : "女",
        get_current_year() - tch_p->age,get_current_year() - tch_p->age_w,
        tch_p->is_stop ? "锁定" : "正常");
}
