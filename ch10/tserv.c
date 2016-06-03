//服务端程序：每10秒向局域网内广播该主机的时间
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFLEN 255

void getcurtime(char *curtime)
{
  time_t tm;
  time(&tm);
  //在curtime中放入当前时间的ASC表示
  snprintf(curtime, BUFLEN, "%s\n", ctime(&tm));
}

int main(int argc, char *argv[])
{
  struct sockaddr_in peeraddr;
  int sockfd, on = 1;
  char msg[BUFLEN + 1];

  if(argc != 3){
    printf("Usage:%s<IP address><port>\n", argv[0]);
    exit(0);
  }

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0){
    fprintf(stderr, "socket creating error in tserv.c\n");
  }

  setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(int));//设置广播方式
  memset(&peeraddr, 0, sizeof(struct sockaddr_in));
  peeraddr.sin_family = AF_INET;
  if(inet_pton(AF_INET, argv[1], &peeraddr.sin_addr) <= 0){ //将字符串IP地址转换为二进制IP地址
    printf("Wrong destination IP address.\n");
    exit(1);
  }

  peeraddr.sin_port = htons(atoi(argv[2])); //将字符串端口号转换为网络字节序
  while(1){
    getcurtime(msg);
    int a;
    //sendto参数中的peeraddr是数据将要发往的目的端的协议地址
    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&peeraddr, sizeof(struct sockaddr_in));
    printf("Send a broadcast message.\n");
    fflush(stdout);
    sleep(10);
  }
}
