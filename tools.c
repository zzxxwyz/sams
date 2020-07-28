#include <getch.h>
#include "tools.h"
#include "status.h"

// 获取当前年份
uint16_t get_current_year(void)
{
    time_t sec = time(NULL);
    struct tm* gmt = gmtime(&sec);
    return 1900 + gmt->tm_year;

}
// 清理输入缓冲区
static void clear_stdin(void)
{
    stdin->_IO_read_ptr = stdin->_IO_read_end;
}

// 清理屏幕
void clear_src(void)
{
    system("clear");
}

// 任意键继续
void anykey_continue(void)
{
    pf("按任意键继续...");
    getch();
    pf("\n");
}

// 显示提示信息
void show_msg(const char* msg,time_t t)
{
    pf("%s\n",msg);
    sleep(t);
}

// 获取性别
char get_sex(void)
{
    while(true)
    {
        char sex = getch();
        if('w' == sex || 'm' == sex) 
        {
            pf("%c\n",sex);
            return sex;
        }
    }
}

// 获取年份
uint16_t get_year(void)
{
    while(true)
    {
        uint16_t year = 0;
        for(int i=0; i<4; i++)
        {
            year *= 10;
            char ch = getch();
            pf("%c",ch);
            year += ch-'0';
        }
        
        if(year > 1950 && year < get_current_year()) 
        {
            pf("\n");
            return year;
        }
        pf("\b\b\b\b    \b\b\b\b");
    }
}

// 获取成绩
bool get_score(float* score_p)
{
    float score = 0;
    sf("%f",&score);
    if(score < 0 || score > 100)
    {
        show_msg("*** 输入的数据有误,请重新输入! ***",1);
        return false;
    }
    
    *score_p = score;
    return true;
}

// 从键盘获取密码
char* get_passwd(char* pd,size_t hope_len,bool is_show)
{
    if(NULL == pd) return NULL;

    size_t index = 0;
    while(index < hope_len-1)
    {
        int32_t key_val = getch();
        if(KEY_BACKSPACE == key_val)
        {
            if(index > 0)
            {
                index--;
                is_show ? pf("\b \b") : nil;
            }
        }
        else if(KEY_ENTER == key_val)
        {
            break;
        }
        else
        {
            pd[index++] = key_val;
            is_show ? pf("*") : nil;
        }
    }

    pd[index] = '\0';

    pf("\n");

    clear_stdin();

    return pd;
}

// 从键盘获取指令
char get_cmd(char begin_cmd,char end_cmd)
{
    if( begin_cmd > end_cmd) return SPILL;

    pf("   请输入指令:");
    while(true)
    {
        char key_val = getch() ;
        if(key_val >= begin_cmd && key_val <= end_cmd)
        {
            pf("%c\n",key_val);
            return key_val;
        }
    }
}

// 从键盘获取字符串
char* get_str(char* str,size_t hope_len)
{
    if(NULL == str || 0 == hope_len) return NULL;

    size_t index = 0;
    while(index < hope_len-1)
    {
        int8_t key_val = getch();
        if(KEY_ESC == key_val) return NULL;
        if(KEY_ENTER == key_val) break;
        if(KEY_BACKSPACE == key_val)
        {
            if(index > 0)
            {
                pf("\b \b");
                index--;
            }
            continue;
        }
        pf("%c",key_val);
        str[index++] = key_val;
    }
    
    str[index] = '\0';
    
    pf("\n");

    clear_stdin();

    return str;
}

// 从文件加载数据
int load_data (const char* path,void* ptr,size_t node_size)
{
    if(NULL == path || NULL == ptr) return PRNIL;

    FILE* frp = fopen (path,"r");
    if(NULL == frp) return 0;

    size_t cnt = 0,num = 0;
    while ((cnt = fread(ptr+num*node_size,node_size,1,frp))) num += cnt;

    fclose(frp);
    return num;
}

// 保存数据到文件
int save_data (const char* path,const void* ptr,size_t node_size,size_t num)
{
    if(NULL == path || NULL == ptr) return PRNIL;

    FILE* fwp = fopen(path,"w");
    if(NULL == fwp) return ERROR;

    size_t cnt = fwrite(ptr,node_size,num,fwp);

    fclose(fwp);
    return cnt;
}
