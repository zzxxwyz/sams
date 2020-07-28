#include "sams.h"

int main(void)
{
	// 申请内存加载数据 
	sams_init ();
	
	// 开始运行管理系统
	sams_start ();
	
	// 保存数据释放内存
	sams_exit ();
}
