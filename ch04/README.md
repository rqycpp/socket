## Exercise 1
> 简述使用UDP套接字编程的基本步骤。

服务器端：

1. 使用socket()函数创建套接字。
2. 将创建的套接字绑定到指定的地址结构。
3. 等待客户端的数据请求。
4. 处理客户端请求。
5. 向客户端发送应答数据。
6. 关闭套接字

客户端：

1. 使用socket()函数创建套接字。
2. 发送数据请求给服务器。
3. 等待接收服务器的数据应答。
4. 关闭套接字。

## Exercise 2
> 为什么要在recvfrom 和 sendto 函数中指明目的地址。

`recvfrom()`函数中指明目的地址`*from`，可以返回与之通信的对方的**套接字地址结构**，告诉用户接收到
的数据报来自于谁。

`sendto()`函数中指明目的地址`*to`，可以用来指明数据报将要发往的**目的协议地址**。

## Exercise 3
> 简述connect函数在UDP套接字编程中的作用。



## 上机实战

## 课堂笔记

- UDP使用`recvfrom()`函数接收数据，它类似于标准的read()，但是在recvfrom()函数中要**指明目的地址**。
该函数调用成功时返回接收到数据报的长度（单位：字节），失败时返回-1，并设置相应的errno值。

```cpp
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *from, size_t *addrlen);
```

- UDP使用`sendto()`函数发送数据，它类似于标准的write，但是与`recvfrom()`函数一样，要**指明目的地址**。
该函数调用成功时返回发送数据的长度（单位：字节），失败时返回-1，并设置相应的errno值。

```cpp
#include <sys/types.h>
#include <sys/socket.h>
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *to, int addrlen);
```
