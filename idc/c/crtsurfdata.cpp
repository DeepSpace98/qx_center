/*
 *  程序名：crtsurfdata.cpp  本程序用于生成全国气象站点观测的分钟数据。
 *  作者：Liy。
*/
// 更新日志：
// 1.增加生成历史数据文件的功能，为压缩文件和清理文件模块做准备
// 2.增加信号处理函数，处理2和15的信号
// 3.解决调用exit函数退出时局部对象没有调用析构函数的问题
// 4.把心跳信息写入共享内存中

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

//全国气象站点分钟观测数据结构
struct st_surfdata{
  char obtid[11];     //站点代码
  char ddatetime[21]; //数据时间：格式yyyymmddhh24miss
  int t;              //气温：单位，0.1摄氏度
  int p;              //气压：0.1百帕
  int u;              //相对湿度， 0-100之间的值
  int wd;             //风向，0-360之间的值
  int wf;             //风速：单位0.1m/s
  int r;              //降雨量：0.1mm
  int vis;            //能见度：0.1米
};


vector<struct st_surfdata> vsurfdata;

//模拟生成全国气象站点分钟观测数据，存放在vsurfdata容器中
void CrtSurfData();

//获取当前时间
char strddatetime[21];
//把容器vsurfdata中的数据写入文件中
bool CrtSurfFile(const char *outpath, const char *datafmt);

CLogFile logfile;    //日志类
CFile File;          //文件类
CPActive PActive;    //心跳类

void EXIT(int sig);

int main(int argc, char *argv[]){
  //inifile outpath logfile
  if((argc != 5) and (argc != 6)){
    printf("Using:./crtsurfdata inifile outpath logfile datafmt [datatime]\n");
    printf("Example:/project/idc/bin/crtsurfdata /project/idc/ini/stcode.ini /tmp/idc/surfdata /log/idc/crtsurfdata.log xml,json,csv 20230314123000\n\n");
    printf("Example:/project/tools/bin/procctl 60 /project/idc/bin/crtsurfdata /project/idc/ini/stcode.ini /tmp/idc/surfdata /log/idc/crtsurfdata.log xml,json,csv 20230314123000\n\n");

    printf("inifile 全国气象站点参数文件名。\n");
    printf("outpath 全国气象站点数据文件存放的目录。\n");
    printf("logfile 本程序运行的日志文件名。\n\n");
    printf("datafmt 生成数据文件的格式， 支持xml、json、csv三种格式。\n\n");    
    printf("datatime 这是一个可选参数，表示生成指定时间的数据和文件。\n\n"); 
    return -1;
  }

  //关闭信号和IO，设置信号处理函数。
  CloseIOAndSignal(true);
  signal(SIGINT, EXIT);
  signal(SIGTERM, EXIT);

  if(logfile.Open(argv[3], "a+", true) == false){
    printf("logfile.Open(%s) failed.\n", argv[3]);
    return -1;
  }

  logfile.Write("crtsurfdata 开始运行！\n");

  PActive.AddPInfo(20, "crtsurfdata", &logfile);

  if(LoadSTCode(argv[1])==false) return -1;

  //获取当前时间 
  memset(strddatetime, 0, sizeof(strddatetime));
  if(argc == 5)
    LocalTime(strddatetime, "yyyymmddhh24miss");
  else
    STRCPY(strddatetime, sizeof(strddatetime), argv[5]);
  
  CrtSurfData();

  if (strstr(argv[4], "xml") != 0) CrtSurfFile(argv[2], "xml");
  if (strstr(argv[4], "json") != 0) CrtSurfFile(argv[2], "json");
  if (strstr(argv[4], "csv") != 0) CrtSurfFile(argv[2], "csv");

  logfile.Write("crtsurfdata 运行结束！\n");
  return 0;
}

bool LoadSTCode(const char *inifile){
  //打开站点参数文件。
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

void CrtSurfData(){
  //随机数种子
  srand(time(0));
  
  st_surfdata stsurfdata;
  //遍历容器
  for(int i = 0; i < vstcode.size(); i ++ ){
    //随机数填充st_surfdata
    memset(&stsurfdata, 0, sizeof(struct st_surfdata));
    strncpy(stsurfdata.obtid, vstcode[i].obtid, 10);
    strncpy(stsurfdata.ddatetime, strddatetime, 14);
    stsurfdata.t = rand() % 351;   
    stsurfdata.p = rand() % 265 + 10000; 
    stsurfdata.u = rand() % 100 + 1; 
    stsurfdata.wd = rand() % 360; 
    stsurfdata.wf = rand() % 150; 
    stsurfdata.r = rand() % 16; 
    stsurfdata.vis = rand() % 5001 + 100000;               
    
    //将st_surfdata放入到vsurfdata
    vsurfdata.push_back(stsurfdata);
  }
}

bool CrtSurfFile(const char *outpath, const char *datafmt){
  
  //拼接生成数据的文件名，例如：SURF_ZH_20230417063000_2254.csv
  char strFileName[301];
  sprintf(strFileName, "%s/SURF_ZH_%s_%d.%s", outpath, strddatetime, getpid(), datafmt);
  //打开文件
  if(File.OpenForRename(strFileName, "w") == false){
    logfile.Write("File.OpenForRename(%s) failed.\n", strFileName);
    return false;
  }
  //写入第一行标题
  if (strcmp(datafmt, "csv") == 0) File.Fprintf("站点代码，数据时间，气温，气压，相对湿度，风向，风速，降雨量，能见度\n");

  if (strcmp(datafmt, "xml") == 0) File.Fprintf("<data>\n");

  if (strcmp(datafmt, "json") == 0) File.Fprintf("{\"data\":[\n");
  //写入数据
  for (int i = 0; i < vsurfdata.size(); i ++ ){
    //写入一条记录
    if(strcmp(datafmt, "csv") == 0) {
      File.Fprintf("%s, %s, %.1f, %.1f, %d, %d, %.1f, %.1f, %.1f\n", vsurfdata[i].obtid, vsurfdata[i].ddatetime, vsurfdata[i].t / 10.0, vsurfdata[i].p / 10.0, vsurfdata[i].u, \
                      vsurfdata[i].wd, vsurfdata[i].wf / 10.0, vsurfdata[i].r / 10.0, vsurfdata[i].vis / 10.0);
    }

    if(strcmp(datafmt, "xml") == 0) {
      File.Fprintf("<obtid>%s</obtid><ddatetime>%s</ddatetime><t>%.1f</t><p>%.1f</p><u>%d</u><wd>%d</wd><wf>%.1f</wf><r>%.1f</r><vis>%.1f</vis><endl/>\n", vsurfdata[i].obtid, vsurfdata[i].ddatetime, vsurfdata[i].t / 10.0, vsurfdata[i].p / 10.0, vsurfdata[i].u, \
                      vsurfdata[i].wd, vsurfdata[i].wf / 10.0, vsurfdata[i].r / 10.0, vsurfdata[i].vis / 10.0);
    }

    if(strcmp(datafmt, "json") == 0) {
      File.Fprintf("{\"obtid\":\"%s\", \"ddatetime\":\"%s\", \"t\":\"%.1f\", \"p\":\"%.1f\", \"u\":\"%d\", \"wd\":\"%d\", \"wf\":\"%.1f\",\"r\":\"%.1f\",\"vis\":\"%.1f\"}", vsurfdata[i].obtid, vsurfdata[i].ddatetime, vsurfdata[i].t / 10.0, vsurfdata[i].p / 10.0, vsurfdata[i].u, \
                      vsurfdata[i].wd, vsurfdata[i].wf / 10.0, vsurfdata[i].r / 10.0, vsurfdata[i].vis / 10.0);
      if(i < vsurfdata.size() - 1) File.Fprintf(",\n");
      else File.Fprintf("\n");
    }
  }

  if (strcmp(datafmt, "xml") == 0) File.Fprintf("</data>\n");

  if (strcmp(datafmt, "json") == 0) File.Fprintf("]}\n");
  //关闭文件
  File.CloseAndRename();

  UTime(strFileName, strddatetime);  //修改文件时间属性

  logfile.Write("生成数据文件%s成功， 数据时间%s, 记录数%d。\n", strFileName, strddatetime, vsurfdata.size());

  return true;
}

void EXIT(int sig){
  logfile.Write("程序退出， sig = %d\n\n", sig);
  exit(0);
}