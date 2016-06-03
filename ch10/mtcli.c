//多播的实例
//客户端程序：加入多播组，监听指定端口，收到服务器多播的时间数据报后打印
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFLEN 255

int main(int argc, char const *argv[]) {
  struct sockaddr_in localaddr;
  int sockfd, n;
  char msg[BUFLEN + 1];
  struct ip_mreq mcaddr; //加入或离开多播组需要的数据类型

  if(argc != 3){
    printf("Usage:%s<port>\n", argv[0]);
    exit(0);
  }

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0){
    fprintf(stderr, "socket creating error in tcli.c\n");
    exit(1);
  }

  memset(&localaddr, 0, sizeof(struct sockaddr_in));
  localaddr.sin_port = htons(atoi(argv[2]));
  localaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(inet_pton(AF_INET, "192.168.1.108", &mcaddr.imr_interface) <= 0){
    perror("Local ip_add is wrong.\n");
    exit(0);
  }

  //加入哪个多播组由命令行参数给出
  if(inet_pton(AF_INET, argv[1], &mcaddr.imr_multiaddr) <= 0){
    perror("Multicast ip_addr is wrong.\n");
    exit(0);
  }

  printf("Multicast ip is:%s\n", inet_ntoa(mcaddr.imr_multiaddr));
  fflush(stdout);

  //加入多播分组
  if(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mcaddr, sizeof(struct ip_mreq)) < 0){
    perror("setsockopt error in mtcli.c, join multicast failed.\n");
    exit(4);
  }

  int opt = SO_REUSEADDR;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  //
  if(bind(sockfd, (struct sockaddr *)&localaddr, sizeof(struct sockaddr_in)) < 0){
    fprintf(stderr, "bind error in mtcli.c\n");
    exit(2);
  }

  n = read(sockfd, msg, BUFLEN);
  if(n == -1){
    fprintf(stderr, "read error in mtcli.c\n");
    exit(3);
  }
  else{
    msg[n] = '\0';
    printf("%s\n", msg);
  }
  return 0;
}
