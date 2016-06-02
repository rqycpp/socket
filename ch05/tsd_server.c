//线程安全（TSD）服务器
//增加了对客户所有的发送数据进行存储的功能
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
void savedata_r(char *recvbuf, int len, char *cli_data);
void *function(void *arg);

struct ARG{
  int connfd;
  struct sockaddr_in client;
};

pthread_key_t key; //TSD关键字key
pthread_once_t once = PTHREAD_ONCE_INIT; //保证每个进程只调用一次createkey_once()函数

static void destructor(void *ptr){ //每个线程推退出时执行的析构函数
  free(ptr);
}
static void creatkey_once(void){
  pthread_key_create(&key, destructor); //在进程内部分配一个标志TSD的关键字key
}

struct ST_DATA{
  int index;
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

    if(pthread_create(&tid, NULL, function, (void *)arg)){
      perror("pthread_create() error.\n");
      exit(1);
    }
  }
  close(listenfd);
}

void process_cli(int connfd, struct sockaddr_in client)
{
  int num;
  char recvbuf[MAXDATASIZE], sendbuf[MAXDATASIZE];
  char cli_data[MAXDATASIZE], cli_name[MAXDATASIZE];
  printf("You got a connection from %s.\n",inet_ntoa(client.sin_addr));

  num = recv(connfd, cli_name, MAXDATASIZE, 0);
  if(num == 0){
    close(connfd);
    printf("Client disconnected.\n");
    return;
  }
  cli_name[num - 1] = '\0';
  printf("Client's name is %s.\n", cli_name);

  while(num = recv(connfd, recvbuf, MAXDATASIZE, 0)){
    recvbuf[num - 1] = '\0';
    printf("Received client(%s) message:%s\n", cli_name, recvbuf);
    savedata_r(recvbuf, num, cli_data);//线程安全函数
    int i;
    for(i = 0; i < num - 1; ++i){
      sendbuf[num - 2 - i] = recvbuf[i];
    }
    sendbuf[num - 1] = '\0';
    send(connfd, sendbuf, strlen(sendbuf), 0);
  }
  close(connfd);
  printf("Client(%s) closed connection.\nUser's data [%s]\n", cli_name, cli_data);
}

void *function(void *arg)
{
  struct ARG *info;
  info = (struct ARG *)arg;
  process_cli(info->connfd, info->client);
  free(arg);
  pthread_exit(NULL);
}

//线程安全函数
void savedata_r(char *recvbuf, int len, char *cli_data)
{
  struct ST_DATA *data;
  //保证一个进程只创建一个key
  pthread_once(&once, creatkey_once);
  //获取TSD关键字所绑定的值，若无则进行绑定。
  if( (data = (struct ST_DATA *)pthread_getspecific(key)) == NULL){
    data = (struct ST_DATA *)malloc(sizeof(struct ST_DATA));
    //TSD关键字key与本线程动态分配内存区域进行绑定
    pthread_setspecific(key, data);
    data->index = 0;//首次创建后初始为0，以后会不断递增。以线程为边界，与static不同。
  }

  //将客户收到的数据存放于缓冲区中
  int i = 0;
  while(i < len - 1){
    cli_data[data->index++] = recvbuf[i++];
  }
  cli_data[data->index] = '\0';
}
