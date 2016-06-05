//服务端程序代码
//客户端向服务器监听端口发送请求，服务器将系统的当前时间发送给客户端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXFD 64
#define MAXLINE 255
#define PORT 1234
#define BACKLOG 5
#define TIMESIZE 64

void daemon_init(const char *pname, int facility)
{
  int i;
  pid_t pid;
  if((pid = fork()) !=0 ){
    exit(0);
  }

  setsid();
  signal(SIGHUP, SIG_IGN);

  if((pid = fork()) != 0){
    exit(0);
  }

  chdir("/");
  umask(0);

  for(i = 0; i < MAXFD; i++){
    close(i);
  }

  openlog(pname, LOG_PID, facility);
}

void getcurtime(char *curtime){
	time_t tm;
	time(&tm);//返回从1970.1至今的秒数，
	snprintf(curtime, TIMESIZE, "%s", ctime(&tm));//用ctime将time_t转换成标准字符串时间
  //ctime会在ASCII时间结尾加\n，而snprintf会在整个字符串最后加\0
  //strlen(curtime) == 25,其中24byte时间+1byte换行符，curtime[24] == '\n', curtime[25] == '\0'
}

int main(int argc, char const *argv[]) {
  int listenfd, connfd;
  socklen_t addrlen, len;
  struct sockaddr_in client;
  struct sockaddr_in server;
  char curtime[TIMESIZE];
  time_t ticks;

  bzero(&server, sizeof(server));
  bzero(&client, sizeof(client));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  daemon_init(argv[0], 0);

  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    syslog(LOG_NOTICE | LOG_LOCAL0, "socket error.");
    exit(-1);
  }

  if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1){
    syslog(LOG_NOTICE | LOG_LOCAL0, "bind error.");
    exit(-1);
  }

  if(listen(listenfd, BACKLOG) == -1){
    syslog(LOG_NOTICE | LOG_LOCAL0, "listen error.");
    exit(-1);
  }

  while(1){
    len = sizeof(client);
    connfd = accept(listenfd, (struct sockaddr *)&client, &len);
		getcurtime(curtime);
    int a;
    if((a = write(connfd, curtime, strlen(curtime))) == -1){//发送时间，包括换行符
      syslog(LOG_NOTICE | LOG_LOCAL0, "write error.");
      exit(0);
    }
  }

  close(connfd);
  return 0;
}
