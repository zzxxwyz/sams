#include "sams.h"
#include "admin.h"
#include "manager.h"
#include "status.h"

// 根据帐号查找管理员对象的下标
int admin_find_mgr(uint32_t id)
{
    for(int i=0; i<mgr_n; i++)
    {
        if(id == mgr_a[i].id) return i;
    }
    
    return ERROR;
}

// 添加管理员
static void admin_add_mgr (void)
{
    if(mgr_n >= MGR_MAX)
    {
        show_msg("*** 管理员已满,无法添加! ***",1);
        return;
    }
    
    mgr_a[mgr_n].id = id_a[0]++;
    mgr_a[mgr_n].pd = 111111;
    mgr_a[mgr_n].is_stop = false;
    pf("*** 管理员:%u 添加成功! ***",mgr_a[mgr_n].id);
    show_msg("",1);
    mgr_n++;
    
}

// 删除管理员
static void admin_del_mgr (void)
{
    if(0 >= mgr_n)
    {
        show_msg("*** 系统内无管理员! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要删除的管理员帐号:");
    sf("%u",&id);
    
    int index = admin_find_mgr(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    
    pf("请再次输入要删除的管理员帐号:");
    sf("%u",&id);
    if(id != mgr_a[index].id)
    {
        show_msg("*** 两次输入的帐号不符,删除失败! ***",1);
        return;
    }
    
    memcpy(mgr_a+index,mgr_a+index+1,(mgr_n-index-1)*sizeof(Manager));
    show_msg("*** 删除帐号成功! ***",1);     
    mgr_n--;
}

// 解锁管理员
static void admin_unlock_mgr (void)
{
    if(0 >= mgr_n)
    {
        show_msg("*** 系统内无管理员! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要解锁的管理员帐号:");
    sf("%u",&id);
    
    int index = admin_find_mgr(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    
    if(!mgr_a[index].is_stop)
    {
        show_msg("*** 此帐号未被锁! ***",1);
        return;
    }
    
    mgr_a[index].is_stop = false;
    show_msg("*** 解锁帐号成功! ***",1); 
}

// 重置管理员密码
static void admin_reset_mgr (void)
{
    if(0 >= mgr_n)
    {
        show_msg("*** 系统内无管理员! ***",1);
        return;
    }
    
    uint32_t id = 0;
    pf("请输入要重置的管理员帐号:");
    sf("%u",&id);
    int index = admin_find_mgr(id);
    if(ERROR == index)
    {
        show_msg("*** 不存在此帐号! ***",1);
        return;
    }
    pf("请再次输入要重置的管理员帐号:");
    sf("%u",&id);
    if(id != mgr_a[index].id)
    {
        show_msg("*** 两人些输入的帐号不符,重置失败! ***",1);
        return;
    }
    
    mgr_a[index].pd = 111111;
    show_msg("*** 重置密码成功! ***",1); 
}

// 列出所有管理员
static void admin_list_mgr (void)
{
    clear_src();
    puts("******显示所有管理员******");
    for(int i=0; i<mgr_n; i++)
    {
        mgr_show(mgr_a+i);        
    }
    anykey_continue();
}

// 登陆超级管理员
int admin_login(const char* id,const char* pd)
{
    if(NULL == id || NULL == pd) return PRNIL;

    if(strcmp(id,"zzxx")) return IDERR;

    if(strcmp(pd,"1234")) return PDERR;

    admin_enter();
    return RIGHT;
}


// 进入超级管理员子系统
void admin_enter (void)
{
    while(true)
    {
        clear_src();
        puts("***超级管理员子系统***");
        puts("   0.添加管理员");
        puts("   1.删除管理员");
        puts("   2.重置管理员密码");
        puts("   3.解锁管理员");
        puts("   4.列出所有管理员");
        puts("   5.退出超级管理员");
        puts("---------------------");

        switch(get_cmd('0','5'))
        {
            case '0': admin_add_mgr(); break;
            case '1': admin_del_mgr(); break;
            case '2': admin_reset_mgr(); break;
            case '3': admin_unlock_mgr(); break;
            case '4': admin_list_mgr(); break;
            case '5': return;
        }
    }
}
