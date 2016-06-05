## Exercise 1

> 什么是守护进程？它与后台运行的程序有什么区别？

1. 守护进程是在后台运行不受终端控制的进程（如输入、输出等）。
2. 加&启动的程序是后台运行的程序，虽然运行时与守护进程相似，但它拥有控制终端。

## Exercise 2

> 创建守护进程的步骤是什么？

1. 调用frok()函数产生后台子进程，然后终止父进程，留下子进程继续运行。
2. 调用setid()函数产生新的会话期并失去控制终端。
3. 忽略SIGHUP信号并再次调用fork()函数产生新子进程。终止第一个子进程。
4. 改变工作目录。（通常将目录改变到根目录下）
5. 关闭已打开的文件描述符，并打开一个空设备，把它复制到标准输出、标准错误上。
6. 用openlog()函数建立与syslogd的连接。

```cpp
#define MAXFD 64
void daemon_init(const char *pname, int facility)
{
  pit_t pid;
  if((pid = fork()) != 0) //创建第一个子进程
    exit(0); //终止父进程

  setsid(); //产生新的会话期，成为会话头
  singal(SIGHUP, SIG_IGN); //忽略SIGHUP信号

  if((pid = fork()) != 0) //创建第二个子进程
    exit(0);  //终止第一个子进程

  chdir("/"); //改变工作目录
  umask(0); //清除文件模式创建掩码，使新文件的权限位不受原先文件模式创建掩码权限位的影响。

  for(i = 0; i < MAXFD; i++){ //关闭所有打开的文件描述字
    close(i);
  }

  openlog(pname, LOG_PID, facility); //用syslogd处理错误
}
```

## Exercise 3

> 请使用syslog函数向syslogd守护进程登记一个消息：send message。

## Exercise 4

> 请编写一个守护进程，进程能接收客户端发来的字符串，并将字符串反转后发送给客户端。

## 上机练习

> 请编写一个时间服务器程序，要求这个服务器通过inetd超级服务器以守护进程的方式运行。
