//客户端程序
//客户端程序向时间服务器发出请求，并显示服务器回送的结果
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 1234
#define MAXDATASIZE 100

int main(int argc, char const *argv[]) {
  int fd, numbytes;
  char buf[MAXDATASIZE];
  struct hostent *he;
  struct sockaddr_in server;

  if(argc != 2){
    printf("Usage:%s<IP>\n", argv[0]);
    exit(1);
  }

  if((he = gethostbyname(argv[1])) == NULL){
  	printf("argv[1]=%s\n", argv[1]);
    perror("gethostbyname error.\n");
    exit(1);
  }

  if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket error.\n");
    exit(1);
  }

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr = *((struct in_addr *)he->h_addr);

  if(connect(fd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1){
    perror("connect error.\n");
    exit(1);
  }

  if((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1){
    perror("recv error.\n");
    exit(1);
  }

  //printf("numbytes=%d\n", numbytes);
	//numbytes == 25; 25 == 24 + 1
 	buf[numbytes] = '\0';
  printf("Server Message:%s", buf);
  close(fd);

  return 0;
}
