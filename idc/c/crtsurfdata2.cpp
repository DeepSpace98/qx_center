/*
 *  程序名：crtsurfdata2.cpp  本程序用于生成全国气象站点观测的分钟数据。
 *  作者：Liy。
*/

#include "_public.h"

// 示例数据：
// 省   站号  站名 纬度   经度  海拔高度
// 安徽,58015,砀山,34.27,116.2,44.2
struct st_stcode
{
  /* data */
  char provname[31];  //省
  char obtid[11];     //站点
  char obtname[31];   //站号
  double lat;         //纬度
  double lon;         //经度
  double height;      //海拔高度
};


CLogFile logfile;    //日志类

int main(int argc, char *argv[]){
  //inifile outpath logfile
  if(argc != 4){
    printf("Using:./crtsurfdata2 inifile outpath logfile\n");
    printf("Example:/project/idc/bin/crtsurfdata2 /project/idc/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata2.log\n\n");

    printf("inifile 全国气象站点参数文件名。\n");
    printf("outpath 全国气象站点数据文件存放的目录。\n");
    printf("logfile 本程序运行的日志文件名。\n\n");

    return -1;
  }

  if(logfile.Open(argv[3], "a+", true) == false){
    printf("logfile.Open(%s) failed.\n", argv[3]);
    return -1;
  }

  logfile.Write("crtsurfdata2 开始运行！\n");

  //业务代码

  logfile.Write("crtsurfdata2 运行结束！\n");
  return 0;
}
