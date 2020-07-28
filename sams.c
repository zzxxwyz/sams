#include <malloc.h>
#include "sams.h"
#include "admin.h"
#include "manager.h"
#include "teacher.h"
#include "student.h"
#include "status.h"

uint32_t* id_a;

static int sams_login(const char* id,const char* pd)
{
    Status ret = admin_login(id,pd);
    if(RIGHT == ret) return RIGHT;
    if(PDERR == ret) return IDERR;
    
    ret = mgr_login(id,pd);
    if(IDERR != ret) return ret;
    
    ret = tch_login(id,pd);
    if(IDERR != ret) return ret;

    ret = stu_login(id,pd);
    return ret;
}

// 锁定帐号
static void sams_lock(const char* id_p)
{
    uint32_t id = atoi(id_p);
    for(int i=0; i<mgr_n; i++)
    {
        if(id == mgr_a[i].id)
        {
            mgr_a[i].is_stop = true;
            return;
        }
    }
    
    for(int i=0; i<tch_n; i++)
    {
        if(id == tch_a[i].id)
        {
            tch_a[i].is_stop = true;
            return;
        }
    }
    
    for(int i=0; i<stu_n; i++)
    {
        if(id == stu_a[i].id)
        {
            stu_a[i].is_stop = true;
            return;
        }
    }
}


// 申请内存加载数据 
void sams_init(void)
{
    mgr_a = malloc(sizeof(Manager)*MGR_MAX);
    mgr_n = load_data(M_DATA_PATH,mgr_a,sizeof(Manager)); 

    tch_a = malloc(sizeof(Teacher)*TCH_MAX);
    tch_n = load_data(T_DATA_PATH,tch_a,sizeof(Teacher));

    stu_a = malloc(sizeof(Student)*STU_MAX);
    stu_n = load_data(S_DATA_PATH,stu_a,sizeof(Student));
    
    id_a = malloc(sizeof(uint32_t)*3);
    printf("-----1-----------\n");
    if(0 >= load_data(ID_PATH,id_a,sizeof(uint32_t)))
    {
    printf("----2------------\n");
    	id_a[0] = 1001;
    	id_a[1] = 2001;
    	id_a[2] = 3001;
    }
    printf("----3------------\n");
}

// 开始运行管理系统
void sams_start(void)
{
    char id[20] = {};
    char pd[20] = {};
    char prev_id[20] = {};
    size_t login_num = 3;

    while(true)
    {
        clear_src();
        pf("***欢迎使用指针成绩管理系统***\n");
       
        pf("请输入帐号:");
        if(NULL == get_str(id,20)) break;

		pf("请输入密码:");
		get_passwd(pd,20,true);

        login_num--;
        switch(sams_login(id,pd))
        {
            case RIGHT: 
                login_num = 3; break;
            case IDERR: 
                show_msg("*** 帐号或密码错误,请重新登陆! ***",1);
                break;
            case PDERR:
                if(0 == login_num) 
                {
                    sams_lock(id); 
                    login_num = 3;
                    show_msg("*** 此帐号被锁,请联系上级解锁! ***",1);
                    break;
                }
                if(strcmp(id,prev_id))
                {
                    strcpy(prev_id,id);
                    login_num = 2;
                }
                pf("密码错误,你还有%d次机会",login_num);
                show_msg("",1);
        }        
    }
}

// 保存数据释放内存
void sams_exit(void)
{
    pf("\n已退出管理系统\n");
    save_data(M_DATA_PATH,mgr_a,sizeof(Manager),mgr_n);
    free(mgr_a);
    save_data(T_DATA_PATH,tch_a,sizeof(Teacher),tch_n);
    free(tch_a);
    save_data(S_DATA_PATH,stu_a,sizeof(Student),stu_n);
    free(stu_a);
    save_data(ID_PATH,id_a,sizeof(uint32_t),3);
    free(id_a);
}
