##Exercise 1
> 简述通用套接字地址结构的作用。

##Exercise 2
> 简述字节排序函数的作用。

##Exercise 3
> 举例说明从进程到内核与从内核到进程两种传递套接字地址结构的方式有什么区别。

##上机实战


##课堂笔记
- IPv4套接字地址结构通常也称为“**网际套接字地址结构**”，它的名字为`sockaddr_in`，定义在头文件`<netinet/in.h>`中，其结果定义如下：

```cpp
typedef unit32_t in_addr_t; //32位IP地址
typedef unit16_t in_port_t; //16位端口号
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

- 