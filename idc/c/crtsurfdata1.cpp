/*
 *  程序名：crtsurfdata1.cpp  本程序用于生成全国气象站点观测的分钟数据。
 *  作者：Liy。
*/

#include "_public.h"

int main(int argc, char *argvp[]){
  //inifile outpath logfile
  if(argc != 4){
    printf("Using:./crtsurfdata1 inifile outpath logfile\n");
    printf("Example:/project/idc1/bin/crtsurfdata1 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc\n");

    printf("inifile 全国气象站点参数文件名。\n");
    printf("outpath 全国气象站点数据文件存放的目录。\n");
    printf("logfile 本程序运行的日志文件名。\n\n");

    return -1;
  }
  return 0;
}
