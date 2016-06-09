//服务端程序
//接收客户端字符串，将其大小写变换后返回。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 1234
#define BACKLOG 5
#define MAXDATASIZE 256

int main()
{
	int listenfd, connectfd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t addrlen;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket() error.\n");
		exit(1);
	}

	int opt = SO_REUSEADDR;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("Bind() error.\n");
		exit(1);
	}

	if(listen(listenfd, BACKLOG) == -1)
	{
		perror("listen() error.\n");
		exit(1);
	}

	addrlen = sizeof(client);

	if((connectfd = accept(listenfd, (struct sockaddr *)&client, &addrlen)) == -1)
	{
		perror("accept() error\n");
		exit(1);
	}
	printf("You got a connection from client's ip is %s, port is %d.\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

  char buf[MAXDATASIZE];
	while(1){
    int i, num;
    if((num = read( connectfd, buf, MAXDATASIZE)) == -1){
      perror("read() error.\n");
      exit(1);
    }
    for(i = 0; i < num; ++i){
      if(buf[i] >= 'a' && buf[i] <= 'z')
        buf[i] = toupper(buf[i]);
      else
        buf[i] = tolower(buf[i]);
    }
    write( connectfd, buf, num);
  }

	close(connectfd);
	close(listenfd);
	return 0;
}
