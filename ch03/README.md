## Exercise 1
> 简述使用TCP套接字编程的基本步骤。

![](https://upload.wikimedia.org/wikipedia/commons/a/a1/InternetSocketBasicDiagram_zhtw.png)

服务端实现的步骤如下：

1. 使用socket()函数创建套接字。
2. 将创建的套接字绑定到指定的地址结构。
3. listen()函数设置套接字为监听模式，使服务器进入被动打开的状态。
4. 接受客户的连接请求，建立连接。
5. 接收、应答客户端的数据请求。

客户端实现的步骤如下：

1. 使用socket()函数创建套接字。
2. 调用connect()函数建立一个与TCP服务器的连接。
3. 发送数据请求，接收服务器的数据应答。
4. 终止连接。

## Exercise 2
> 简述bind函数中使用通配地址的作用。

```cpp
bzero(&server, sizeof(server));
server.sin_family = AF_INET;
server.sin_port = htons(PORT);
server.sin_addr.s_addr = htol(INADDR_ANY); //通配地址
bind(sockfd, (struct sockaddr *)&server, sizeof(server));
```
对于IPv4来说，通配地址由常数INADDR_ANY来指定，其值一般为0，它通知内核选择IP地址。

## Exercise 3
> 简述使用TCP套接字基本函数编程的过程中，TCP链接的状态变化。

- 服务端依次执行：socket(),bind(),listen()后状态变为 **LISTEN** ，accept()阻塞。
- 客户端依次执行：socket(),connect()后状态变为 **SYN_SENT** ，发送SYN J 。
- 服务端收到SYN J，状态变为 **SYN_RCVD** ，发送 SYN K 、ACK J + 1。
- 客户端收到SYN K、ACK j + 1，状态变为 **ESTABLISHED** ，发送 ACK K + 1。
- 服务端收到ACK K + 1，状态变为 **ESTABLISHED**，accept()返回。

## 上机实战
> 请编写一个TCP套接字程序，要求客户端根据用户输入的IP地址链接到相应的服务器，服务器等待客户的链接，当链接建立后，完成客户与服务器的信息交互。

服务端程序：[Server](server.c) `./server`

客户端程序：[Client](client.c) `./client localhost`

功能简介：服务端将接收客户端发送的字符串，经大小写变换后返回。
