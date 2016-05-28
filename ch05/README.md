## Exercise 1

网络编程中使用多进程和多线程设计并发服务器有什么区别？

## Exercise 2

在多进程并发服务器中，父进程创建子进程后，父、子进程是如何对描述符进行操作的，理由是什么？

## Exercise 3

试比较`wait()`和`waitpid()`函数的区别。

## 上机实战

## 课堂笔记

- 服务器按处理方式分类：迭代服务器、并发服务器。

- 服务器按连接类型分类：面向连接（TCP），无面向连接（UDP）。

- Linux系统主要提供3种方式支持并发：进程、线程、I/O多路复用。

- Linux有3种资源拷贝的方式：共享(vfork)、直接拷贝、Copy on Write(fork)。

- fork()函数，如果调用成功，该函数返回2次。在父进程中返回子进程ID号，在子进程中返回0。

```cpp
#include <unistd.h>
pit_t fork(void);
```

- 如果父进程在子进程之前终止，则所有子进程的父进程被改为init进程，就是由init进程**领养**。

- 一个已经终止但是其父进程尚未对其进行善后处理的进程称为**僵尸进程**。

- 父进程调用`wait()`,`waitpid()`函数获得子进程的终止信息。

```cpp
#include <sys/wait.h>
pit_t wait(int *statloc);
pit_t waitpid(pit_t pid, int *statloc, int option);
```

- exit() 函数终止进程，关闭所有子进程打开的描述符，向父进程发送SIGCHLD信号，返回状态。

```cpp
#include <stdlib.h>
void exit(int status);
```
