/*
 * 程序名：demo07.cpp，此程序用于演示网银APP的客户端。
 * 作者：Liy。
*/
#include "../_public.h"

CTcpClient TcpClient;

bool srv001();          // 登录业务
bool srv002();          // 我的账户(查询余额)

int main(int argc,char *argv[])
{
  if (argc!=3)
  {
    printf("Using:./demo11 ip port\nExample:./demo11 127.0.0.1 5005\n\n"); return -1;
  }

  // 向服务端发起连接请求。
  if (TcpClient.ConnectToServer(argv[1],atoi(argv[2]))==false)
  {
    printf("TcpClient.ConnectToServer(%s,%s) failed.\n",argv[1],argv[2]); return -1;
  }

  if (srv001() == false) {
    printf("srv001() failed.\n");
    return -1;
  }

  if (srv002() == false) {
    printf("srv002() failed.\n");
    return 0;
  }
}

// 登录业务
bool srv001(){
  char buffer[1024];
  SPRINTF(buffer, sizeof(buffer), "<srvcode>1</srvcode><tel>1392220000</tel><password>123456</password>");
  printf("发送： %s\n", buffer);
  
  if(TcpClient.Write(buffer) == false) return false;

  memset(buffer, 0, sizeof(buffer));
  if(TcpClient.Read(buffer) == false) return false;
  printf("接收： %s\n", buffer);

  //解析接收的xml。
  int iretcode = -1;
  GetXMLBuffer(buffer, "retcode", &iretcode);
  if(iretcode != 0) {
    printf("登录失败。 \n");
    return false;
  }
  printf("登录成功。 \n");
  return true;
}

// 我的账户
bool srv002(){
  char buffer[1024];
  SPRINTF(buffer, sizeof(buffer), "<srvcode>2</srvcode><cardid>62620000000001</cardid>");
  printf("发送： %s\n", buffer);
  
  if(TcpClient.Write(buffer) == false) return false;

  memset(buffer, 0, sizeof(buffer));
  if(TcpClient.Read(buffer) == false) return false;
  printf("接收： %s\n", buffer);

  //解析返回的的xml。
  int iretcode = -1;
  GetXMLBuffer(buffer, "retcode", &iretcode);
  if(iretcode != 0) {
    printf("查询余额失败。 \n");
    return false;
  }
  double ye = 0;
  GetXMLBuffer(buffer, "ye", &ye);

  printf("查询余额成功(%.2f)。 \n", ye);
  return true;
}