##Exercise 1
> 简述通用套接字地址结构的作用。

套接字地址结构作为参数传递给任一个套接字函数时，通常通过指针来传递。当套接字函数取得此参数时，参数中可能存放的是来自所支持的**任何协议族的地址结构**。因此在调用套接字函数时，需要将指向**特定**于协议的地址结构的指针类型转换成指向**通用地址结构**的指针。

```cpp
struct sockaddr_in ser;
bind(sockfd, (struct sockaddr *)&ser, sizeof(ser));
```

##Exercise 2
> 简述字节排序函数的作用。

由于网络上的主机可能采用不同的主机字节序，所以字节排序函数可以将不同的主机字节序转换成网络字节序，从而使网络上不同的主机之间能够相互通信。

##Exercise 3
> 举例说明从进程到内核与从内核到进程两种传递套接字地址结构的方式有什么区别。

```cpp
struct sockaddr_in ser;
bind(sockfd, (struct sockaddr *)&ser, sizeof（ser));

struct sockaddr_in client;
socklen_t len;
accept(listenfd, (struct sockaddr *)&client, &len);
```

- bind函数将套接字地址结构和结构的大小都传递给了内核，所以进程到内核拷贝的数据长度是固定的。
- accept函数被调用时，结构长度是个值，用于告诉内核该结构长度，使内核在写这个结构时不会越界。当函数返回时，结构大小发生了变化，变成了内核在此结构中确切存储的数据长度。

##上机实战


##课堂笔记
- IPv4套接字地址结构通常也称为“**网际套接字地址结构**”，它的名字为`sockaddr_in`，定义在头文件`<netinet/in.h>`中，其结果定义如下：

```cpp
typedef uint32_t in_addr_t; //32位IP地址
typedef uint16_t in_port_t; //16位端口号
typedef unsigned short sa_family_t; //协议族

struct in_addr{
	in_addr_t s_addr;
};

struct sockaddr_in{
	unit8_t sin_len;
	sa_family_t sin_family;
	in_port_t sin_port; //以网络字节序存储
	struct in_addr sin_addr; //s_addr成员以网络字节序存储
	char sin_zero[8]; //暂时没有使用，总是设置为0
};
```

- IPv6套接字地址结构的名字为`sockaddr_in6`，定义在头文件`<netinet/in.h>`中，其结构定义如下：

```cpp
typedef uint16_t in_port_t;
typedef unsigned short sa_family_t;

struct in6_addr{
	uint8_t s6_addr[16]; //存储网络字节序的128位IPv6地址
};

struct sockaddr_in6{
	uint8_t sin6_len;
	sa_family_t sin6_family;
	in_port_t sin6_port;
	uint32_t sin6_flowinfo; //窗口信息，流标签(24位 4位 4位）
	struct in6_addr sin6_addr;
};
```

- 通用套接字地址结构名为`sockaddr`，定义在头文件`<sys/socket.h>`中，其结构定义如下：

```cpp
struct sockaddr{
	uint8_t sa_len;
	sa_family_t sa_family;
	char sa_data[14];
};
```

- 主机字节序和网络字节序之间的相互转换，要用到以下4个函数：

```cpp
#include <netinet/in.h>
uint16_t htons(uint16_t hosts); //host to netword short
uint32_t htonl(uint32_t hosts); //host to netword long
uint16_t ntohs(uint16_t nets);  //netword to host short
uint32_t ntohl(uint32_t netl);  //netword to host long
```

- 字节操纵函数

```cpp
#include <string.h>
void bzero(void *dest, size_t len); //字节置0
void bcopy(const void *src, void *dest, size_t len);
int bcmp(const void *src, const void *dest, size_t len);

void *memset(void *dest, int x, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
int memcmp(const void *str1, const void *str2, size_t len);
```

- IP地址转换函数
可实现字符串的IP和二进制值得IP相互转换：

```cpp
#include <arpa/inet.h>
in_addr_t inet_addr(const char *str);
int inet_aton(const char *str, struct in_addr *numstr);
char *inet_ntoa(struct in_addr inaddr);
```

以上三个函数只能处理IPv4地址，下面介绍两个比较新的函数，它们对IPv4/IPv6地址都能处理：

```cpp
//p(presentation)
//n(numeric)
#include <arpa/inet.h>
int inet_pton(int family, const char *str, void *numstr);
const inet_ntop(int family, const void *numstr, char *str, size_t len);
```

- 值-结果 参数：指在参数传递的过程中其值发生变化的参数。
- 从**进程**到**内核**传递套接字地址结构的函数通常有3个，分别为`bind`,`connect`,`sendto`。
- 从**内核**到**进程**传递套接字地址结构的函数通常有4个，分别为`recvfrom`,`getsockname`,`getpeername`。
