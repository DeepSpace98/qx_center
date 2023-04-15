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

//用于存放站点参数的容器
vector<struct st_stcode> vstcode;

//把站点参数文件加载到vstcode容器中
bool LoadSTCode(const char *inifile);



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

  if(LoadSTCode(argv[1])==false) return -1;

  logfile.Write("crtsurfdata2 运行结束！\n");
  return 0;
}

bool LoadSTCode(const char *inifile){
  //打开站点参数文件。
  CFile File;
  if (File.Open(inifile, "r") == false){
    logfile.Write("File.Open(%s) failed.\n", inifile);
    return false;
  }

  char strBuffer[301];

  CCmdStr CmdStr;

  struct st_stcode stcode;

  while(true){
    //从站点参数文件中读取一行， 如果已读取完， 跳出循环。
    //memset(strBuffer, 0, sizeof(strBuffer));   Fgets()里含有初始化，可不写。
    if(File.Fgets(strBuffer,300,true) == false) break;
    
    //把读取的一行拆分
    CmdStr.SplitToCmd(strBuffer, ",", true);
    if(CmdStr.CmdCount()!= 6) continue;   //去掉无效行
    CmdStr.GetValue(0, stcode.provname, 30);
    CmdStr.GetValue(1, stcode.obtid, 30);
    CmdStr.GetValue(2, stcode.obtname, 30);
    CmdStr.GetValue(3, &stcode.lat);
    CmdStr.GetValue(4, &stcode.lon);
    CmdStr.GetValue(5, &stcode.height);
    vstcode.push_back(stcode);
    //logfile.Write("=%s=\n", strBuffer);
  }
  
  // for(int i = 0; i < vstcode.size(); i ++ ){
  //   logfile.Write(" pro = %s, obtid = %s, obtname = %s, last = %.2f, lon = %.2f, height = %.2f\n", vstcode[i].provname, vstcode[i].obtid, vstcode[i].obtname, vstcode[i].lat,
  //       vstcode[i].lon, vstcode[i].height);
  // }
  return true;
}