//客户端程序：监听指定端口，收到服务器广播的时间数据报后打印
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
  if(argc != 2){
    printf("Usage:%s<port>\n", argv[0]);
    exit(0);
  }

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0){
    fprintf(stderr, "socket creating error in tcli.c\n");
    exit(1);
  }

  memset(&localaddr, 0, sizeof(struct sockaddr_in));
  localaddr.sin_port = htons(atoi(argv[1]));
  localaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt = SO_REUSEADDR;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if(bind(sockfd, (struct sockaddr *)&localaddr, sizeof(struct sockaddr_in)) < 0){
    fprintf(stderr, "bind error in tcli.c\n");
    exit(2);
  }

  n = read(sockfd, msg, BUFLEN);
  if(n == -1){
    fprintf(stderr, "read error in tcli.c\n");
    exit(3);
  }
  else{
    msg[n] = '\0';
    printf("%s\n", msg);
  }
  return 0;
}
