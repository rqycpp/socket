/*多线程并发服务器实例*/
/*该代码不涉及线程安全函数*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>

#define PORT 1234
#define BACKLOG 5
#define MAXDATASIZE 1000

void process_cli(int connfd, struct sockaddr_in client);
void *function(void *arg);

struct ARG{
  int connfd;
  struct sockaddr_in client;
};

int main()
{
  int listenfd, connfd;
  pthread_t  tid;
  struct ARG *arg;
  struct sockaddr_in server;
  struct sockaddr_in client;
  socklen_t len;

  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("Creating socket failed.\n");
    exit(1);
  }

  int opt = SO_REUSEADDR;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  bzero(&server,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1){
    perror("bind() error.\n");
    exit(1);
  }

  if(listen(listenfd, BACKLOG) == -1){
    perror("listen() error.\n");
	exit(1);
  }

  len = sizeof(client);
  while(1){
    if((connfd = accept(listenfd, (struct sockaddr *)&client, &len)) == -1){
      perror("accept() error.\n");
      exit(1);
    }
    arg = (struct ARG *)malloc(sizeof(struct ARG));
    arg->connfd = connfd;
    memcpy((void *)&arg->client, &client, sizeof(client));
    //创建线程，每个线程调用function函数，参数为arg
    if(pthread_create(&tid, NULL, function, (void *)arg)){
      perror("pthread_create() error.\n");
      exit(1);
    }
  }
  close(listenfd);
}

//处理客户请求函数，将客户传递过来的字符串逆序返回
void process_cli(int connfd, struct sockaddr_in client)
{
  int num;
  char recvbuf[MAXDATASIZE], sendbuf[MAXDATASIZE], cli_name[MAXDATASIZE];
  printf("You got a connection from %s.\n",inet_ntoa(client.sin_addr));

  num = recv(connfd, cli_name, MAXDATASIZE, 0);//返回字节数num，非字符串长度
  if(num == 0){
    close(connfd);
    printf("Client disconnected.\n");
    return;
  }
  cli_name[num - 1] = '\0';
  printf("Client's name is %s.\n", cli_name);

  while(num = recv(connfd, recvbuf, MAXDATASIZE, 0)){//num为字节数，num-1为字符串长度
    recvbuf[num - 1] = '\0';
    printf("Received client(%s) message:%s\n", cli_name, recvbuf);
    int i;
    for(i = 0; i < num - 1; ++i){//将字符串逆序返回
      sendbuf[num - 2 - i] = recvbuf[i];
    }
    sendbuf[num - 1] = '\0';
    send(connfd, sendbuf, strlen(sendbuf), 0);
  }
  close(connfd);
}

void *function(void *arg)
{
  struct ARG *info;
  info = (struct ARG *)arg;
  process_cli(info->connfd, info->client);//参数分别为：文件描述符、sockaddr_in
  free(arg);//释放内存
  pthread_exit(NULL);//退出线程
}
