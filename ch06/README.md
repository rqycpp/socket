## Exercise 1

> 域名解析的两种方式是什么?

1. 在`/etc/hosts`文件中查找是否有匹配的主机名。
2. 根据域名解析配置文件`/etc/resolv.conf`中指定的本地域名服务器的地址向本地域名服务器发送地址解析请求。

## Exercise 2

> 域名解析的有关函数有哪些？都有一些什么作用？

函数作用见下文*课堂笔记*。

更多内容请见[man7.org](http://man7.org/linux/man-pages/man3/gethostbyname.3.html)

1. `gethostbyname()`
2. `gethostbyname2()`

## Exercise 3

> getnameinfo()函数跟其他函数有什么不同？

1. 该函数以一个套接口地址为参数，返回一个描述主机的字符串和一个描述服务的字符串。
2. 该函数独立于协议，用户不必关心在套接口地址结构中的协议地址的类型，这些细节由函数自己处理。

## 课堂笔记

- gethostbyname <-> gethostbyaddr

- getservbyname <-> getservbyport

- getaddrinfo <-> getnameinfo

- 参数hostname是主机的域名地址，函数将查询的结果作为参数返回。如果失败返回空指针；如果成功此
函数返回的非空指针指向如下的hostent结构：

```cpp
#include <netdb.h>
struct hostent{
  char *h_name; //主机的正式名称
  char *h_aliases[];  //主机的别名列表
  int h_addrtype; //主机的地址类型
  int h_length; //主机地址长度
  char *h_addr_list[];  //主机IP地址的列表
}
# define h_addr h_addr_list[0]; //在列表中的第一个地址

struct hostent * gethostbyname(const char * hostname);
```

- 资源记录RR(Resource Record)，它主要有以下几类：

(1) A。A记录将一个域名地址对应一个32bit的IPv4地址。eg:`NET IN A 202.121.68.5`

(2) AAAA。 AAAA记录将主机名映射为128位的IPv6地址。eg:`NET IN AAAA 551b:df00:ce3e:e200:20:800:2b37:6426`

(3) NS。 NS记录用于指定一个域名服务器，它负责定义由哪个域名服务器负责管理维护本区域的记录。
eg:`NET IN NS server.cuit.edu.cn`

(4) MX。 MX记录用于指定一台主机的域名，所有发送到本域名的电子邮件都由这台主机接收。
eg:`mail.cuit.edu.cn IN MX server.cuit.edu.cn`

(5) PTR。 PTR记录将IP地址映射为主机名。eg:`NET IN PRT 5.68.121.202.in-addr.arpa`

(6) CNAME。CNAME记录代表Canonical Name(规范名字)，作用是允许主机建立别名。如果人们使用这些别名
而不是实际上的主机名，则它的服务挪到其他主机上时是透明的。eg:`ftp IN CNAME NET.cuit.edu.cn`

- RES_USE_INET6 解析器选项

解析器选项是用来通知解析器让gethostbyname返回的是IPv6地址而不是IPv4地址。

- gethostbyname2 函数对IPv6的支持

```cpp
#include <netdb.h>
struct hostent * gethostbyname2(const char * hostname, int family)
```

- gethostbyaddr() 函数可以用来查询指定IP地址对应的主机域名地址。

```cpp
#include <netdb.h>
struct hostent * gethostbyaddr(const char * addr, size_t len, int family);
```

- uname() 函数返回当前主机的名字。

```cpp
#include <sys/utsname.h>
int uname (struct utsname *name);
```

- gethostname() 函数返回当前主机的名字

```cpp
#include <unistd.h>
int gethostname(char *name, size_t namelen);
```
- getservbyname() 函数通过服务名来获取服务器端口号。

```cpp
#include <netdb.h>
struct servent * getservbyname(const char * servname, const char * portoname);

struct servent{
  char * s_name;
  char ** s_aliases;
  int s_port;
  char * s_proto;
};
```
- getservbyport() 函数，该函数的作用是通过端口号来获取服务名。

```cpp
#include <netdb.h>
struct servent * getservbyport(int port, const char *protoname);
```
- getaddrinfo() 函数在库函数中**隐藏**了协议的依赖性，因此只需要处理由getaddrinfo填写
的套接口地址结构即可。

```cpp
#include <netdb.h>
//hostname是主机名或地址串，service是服务名或十进制数的端口号字符串
int getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints,
struct addrinfo **result);

struct addrinfo{
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  size_t ai_addrlen;
  char *ai_canonname;
  struct sockaddr *ai_addr;
  struct addrinfo *ai_next;
}
```

- freeaddrinfo() 函数可以释放getaddrinfo()函数返回的存储空间。

```cpp
#include <netdb.h>
void freeaddrinfo(struct addrinfo *ai);
```

- getnameinfo() 函数与getaddrinfo()函数功能相反，它以一个**套接口地址**为参数，返回一个
描述主机的字符串和一个描述服务的字符串。同样，这两个函数**独立**与协议。

```cpp
#include <sys/socket.h>
#include <netdb.h>
//host为指向要返回的主机名的指针，serv指向要返回服务器名的指针
int getnameinfo(const struct sockaddr *sockaddr, socklen_t addrlen, char *host,
size_t hostlen, char *serv, size_t servlen, int flags);
```
