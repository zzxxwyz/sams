#include <strings.h>
#include "sams.h"
#include "manager.h"
#include "teacher.h"
#include "status.h"

// 管理员数组指针
Manager* mgr_a;
// 管理员数组使用量
uint32_t mgr_n;

int mgr_find_tch(uint32_t id)
{
    for(int i=0; i<tch_n; i++)
    {
        if(id == tch_a[i].id) return i;
    }
    
    return ERROR;
}

// 添加一个教师
void mgr_add_one_tch(void)
{
    bzero(tch_a+tch_n,sizeof(Teacher));

    pf("请输入教师姓名:");
    get_str(tch_a[tch_n].name,20);
    pf("请输入教师性别:");
    tch_a[tch_n].sex = get_sex();
    pf("请输入教师出生年份:");
    tch_a[tch_n].age = get_year();
    pf("请输入教师入职年份:");
    tch_a[tch_n].age_w = get_year();
    
    tch_a[tch_n].id = id_a[1]++;
    tch_a[tch_n].pd = 222222;
    tch_n++;
    
    show_msg("*** 添加教师成功! ***",1);
}

// 批量添加教师
void mgr_add_multi_tch(void)
{
    char path[30] = {};
    pf("请输入要导入的文件名:");
    get_str(path,30);
    FILE* frp = fopen(path,"r");
    if(NULL == frp)
    {
        show_msg("*** 输入的路径有误! ***",1);
    }
    
    Teacher* tch_p = tch_a+tch_n;
    size_t num_r = 0;
    size_t num_s = 0;
    while(tch_n<TCH_MAX)
    {
        bzero(tch_p,sizeof(Teacher));
        if(!fscanf(frp,"%s %c %hu %hu\n",
            tch_p->name,&tch_p->sex,&tch_p->age,&tch_p->age_w))
            break;
            
        num_r++;
        if('f' != tch_p->sex && 'm' != tch_p->sex) continue;
        if(1950 > tch_p->age || tch_p->age > get_current_year()-18) continue;
        if(tch_p->age+18 > tch_p->age_w  || 
           tch_p->age_w > get_current_year()) continue;
           
        tch_p->id = id_a[1]++;
        tch_p->pd = 222222;
        tch_p->is_stop = false;
        
        tch_show(tch_p);
        tch_n++;
        tch_p++;
        num_s++;
    }
    printf("*** 共读取到%u条数据,成功添加%u位教师! ***",num_r,num_s);
    anykey_continue();  
}

// 添加教师
void mgr_add_tch(void)
{
    if(tch_n >= TCH_MAX)
    {
        show_msg("*** 教师已满,无法添加! ***",1);
        return;
    }
    
    clear_src();
    puts("***添加教师***");
    puts("   0.单个添加");
    puts("   1.批量添加");
    puts("-------------");
    switch(get_cmd('0','1'))
    {
        case '0': mgr_add_one_tch(); break;
        case '1': mgr_add_multi_tch(); break;
    }    
}

// 删除教师
void mgr_del_tch(void)
{
    if(0 >= tch_n)
    {
        show_msg("*** 系统内无教师! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要删除的教师帐号:");
    sf("%u",&id);
    
    int index = mgr_find_tch(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    
    pf("请再次输入要删除的管理员帐号:");
    sf("%u",&id);
    if(id != tch_a[index].id)
    {
        show_msg("*** 两次输入的帐号不符,删除失败! ***",1);
        return;
    }
    
    FILE* fap = fopen(TD_DATA_PATH,"a");
    fwrite(tch_a+index,sizeof(Teacher),1,fap);
    fclose(fap);
        
    memcpy(tch_a+index,tch_a+index+1,(tch_n-index-1)*sizeof(Teacher));
    show_msg("*** 删除帐号成功! ***",1);     
    tch_n--;
}

// 修改教师信息
void mgr_mod_tch(void)
{
    uint32_t id;
    pf("请输入要修改的教师帐号:");
    sf("%u",&id);
    int index = mgr_find_tch(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    while(true)
    {
        clear_src();
        tch_show(tch_a+index);
        puts("***请选择要修改的教师信息***");
        puts("   0.修改教师姓名");
        puts("   1.修改教师性别");
        puts("   2.修改教师出生年份");
        puts("   3.修改教师入职年份");
        puts("   4.返回上一级");
        puts("-------------------------");
        switch(get_cmd('0','4'))
        {
            case '0': 
                pf("请输入教师姓名:");
                get_str(tch_a[index].name,20); break;
            case '1':
                pf("请输入教师性别:");
                tch_a[index].sex = get_sex(); break;
            case '2': 
                pf("请输入教师出生年份:");
                tch_a[index].age = get_year(); break;
            case '3':
                pf("请输入教师入职年份:");
                tch_a[index].age_w = get_year(); break;
            case '4': return;
        }
        show_msg("*** 修改成功! ***",1);
    }
}

// 重置教师密码
void mgr_reset_tch(void)
{
    if(0 >= mgr_n)
    {
        show_msg("*** 系统内无教师! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要重置的教师帐号:");
    sf("%u",&id);
    int index = mgr_find_tch(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    pf("请再次输入要重置的教师帐号:");
    sf("%u",&id);
    if(id != tch_a[index].id)
    {
        show_msg("*** 两人些输入的帐号不符,重置失败! ***",1);
        return;
    }
    
    tch_a[index].pd = 222222;
    show_msg("*** 重置密码成功! ***",1);
}

// 解锁教师帐号
void mgr_nulock_tch(void)
{
    if(0 >= tch_n)
    {
        show_msg("*** 系统内无教师! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要解锁的教师帐号:");
    sf("%u",&id);
    int index = mgr_find_tch(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    if(!tch_a[index].is_stop)
    {
        show_msg("*** 此帐号未被锁! ***",1);
        return;
    }
    tch_a[index].is_stop = false;
    show_msg("*** 解锁帐号成功! ***",1); 
}

// 查询教师基本信息
void mgr_query_tch(void)
{
    char name[20] = {};
    pf("请输入要查询的老师姓名:");
    get_str(name,20);
    
    bool flag = true;
    for(int i=0; i<tch_n; i++)
    {
        if(!strcmp(name,tch_a[i].name))
        {
            tch_show(tch_a+i);
            flag = false;
        }
    }
    
    flag ? show_msg("*** 查无此人! ***",1) : anykey_continue(); 
}

// 显示所有在职教师
void mgr_job_tch(void)
{
    clear_src();
    puts("******所有在职教师******");
    for(int i=0; i<tch_n; i++)
    {
        tch_show(tch_a+i);
    }
    anykey_continue();
}

// 显示所有离职教师
void mgr_quit_tch(void)
{
    FILE* frp = fopen(TD_DATA_PATH,"r");
    if(NULL == frp)
    {
        show_msg("*** 斩无离职教师! ***",1);
        return;
    }
    
    clear_src();
    puts("******所有离职教师******");
    Teacher tch;
    while(fread(&tch,sizeof(tch),1,frp)) tch_show(&tch);
    fclose(frp);
    
    anykey_continue();
}

// 修改管理员登陆密码
void mgr_mod_pd(Manager* mgr_p)
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
    
    mgr_p->pd = atoi(pd);
    show_msg("*** 修改密码成功! ***",1);
}

// 管理员登陆
int mgr_login(const char* id_p,const char* pd_p)
{
    if(NULL == id_p || NULL == pd_p) return PRNIL;
    
    uint32_t id = atoi(id_p);
    uint32_t pd = atoi(pd_p);
    
    int i = 0;
    for(; i<mgr_n; i++)
    {
        if(id == mgr_a[i].id) break;
    }
    
    if(i == mgr_n) return IDERR;
    
    if(mgr_a[i].is_stop)
    {
        show_msg("***此帐号被锁,请联系超级管理员解锁!***",1);
        return RIGHT;
    }
    
    if(pd != mgr_a[i].pd) return PDERR;
    
    mgr_enter(mgr_a+i);
    return RIGHT;
}

// 进行管理员子系统
void mgr_enter(Manager* mgr_p)
{
    while(true)
    {
        clear_src();
        pf("管理员:%u\n",mgr_p->id);
        puts("***进入管理员子系统***");
        puts("   0.添加教师");
        puts("   1.删除教师");
        puts("   2.修改教师信息");
        puts("   3.重置教师密码");
        puts("   4.解锁教师帐号");
        puts("   5.查询教师基本信息");
        puts("   6.显示所有在职教师");
        puts("   7.显示所有离职教师");
        puts("   8.修改管理员密码");
        puts("   9.退出管理员子系统");
        puts("---------------------");
        
        switch(get_cmd('0','9'))
        {
            case '0': mgr_add_tch(); break;
            case '1': mgr_del_tch(); break;
            case '2': mgr_mod_tch(); break;
            case '3': mgr_reset_tch(); break;
            case '4': mgr_nulock_tch(); break;
            case '5': mgr_query_tch(); break;
            case '6': mgr_job_tch(); break;
            case '7': mgr_quit_tch(); break;
            case '8': mgr_mod_pd(mgr_p); break;
            case '9': return;
        }
    }
}

// 显示管理员
void mgr_show(Manager* mgr_p)
{
	pf("帐号:%u  状态:%s\n",mgr_p->id,mgr_p->is_stop?"锁定":"正常");
}
