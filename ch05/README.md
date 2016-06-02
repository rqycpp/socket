## Exercise 1

网络编程中使用多进程和多线程设计并发服务器有什么区别？

- 多进程并发服务器中各个进程具有独立的地址空间，所以具有较高的可靠性。但是创建进程的系统开销
大、切换时间长、并且进程间共享比较复杂。

- 多线程并发服务其中各个线程开销小、切换容易，但由于线程共享相同的内存区域，所以在对共享数据
进行操作时，要注意同步问题。

## Exercise 2

在多进程并发服务器中，父进程创建子进程后，父、子进程是如何对描述符进行操作的，理由是什么？

父进程close(connfd),子进程close(listenfd)。理由是父进程关闭已连接描述符，继续接收下一个
客户的连接请求；子进程关闭监听描述符，然后调用相应函数处理客户的请求。由此使得父、子进程各
尽其职，更加高效。同时也避免了父、子进程对同一个描述符进行操作，造成的输出混合。

## Exercise 3

试比较`wait()`和`waitpid()`函数的区别。

```cpp
pid_t wait(int *statloc);
pid_t waitpid(pit_t pid, int *statloc, int option);//pid == -1 && option == 0 时与wait相同
```

- 父进程可以利用`waitpid()`知道具体某些子进程的状态，而`wait()`不可以指定子进程。
- `waitpid()`可以利用option = WNO_HANG 来通知内核，在没有已终止子进程时不要阻塞。

## 上机实战

1. 多进程并发服务器：

> 服务端程序：[Server](Multiprocessing_server.c)

> 客户端程序：[Client](Multiprocessing_client.c)

2. 多线程并发服务器：

> 服务端程序：[Server](Multithreading_server.c)

> 客户端程序：[Client](Multiprocessing_client.c)

> 服务端程序(利用线程安全函数):[TSD_Server](TSD_server.c)

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

- 线程是进程内的**独立执行实体**和**调度单元**，又称为“轻量级”进程，创建线程比创建进程
快10~100倍。一个进程内的所有线程共享相同的内存空间、全局变量等信息。

- `pthread_create()`函数用于创建新线程。成功时返回0，出错则返回非0。

```cpp
#include <pthread.h>
int pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*func)(void *), void *arg);
//                                 ^线程属性                    ^线程执行的函数         ^向func()传递参数
```

- `pthread_join()`函数与进程的`waitpid()`函数功能类似，等待一个线程终止。

```cpp
#include <pthread.h>
int pthread_join(pthread_t tid, void **status);
```

- 线程分为：**可联合**的和**分离**的。可联合的线程终止时，其线程ID和终止状态将保留，直到
线程调用`pthread_join()`函数；而分离的线程退出后，系统将释放其所有资源。

- `pthread_death()`函数将指定的线程变成**分离**的。

```cpp
#include <pthread.h>
int pthread_death(pthread_t tid);
pthread_t pthread_self(void); //返回自己的线程ID
void pthread_exit(void *status); //终止当前线程，返回状态值。
```

- 在Linux系统中提供线程特定数据TSD来取代静态变量。它类似于全局变量，但是是各个线程私有的，
以线程为界限。

```cpp
#include <pthread.h>
//参数key指向创建的关键字，该关键字对于一个进程中的所有线程是唯一的。每个进程只能调用一次。
int pthread_key_create(pthread_key_t *key, void (* destructor)(void *value));

//使用once参数所指的变量，保证每个进程只调用一次init函数。
int pthread_once(pthread_once_t *once, void (*init)(vid));

//函数为TSD关键字绑定一个与本线程相关的值。
int pthread_setspecific(pthread_key, const void *value);


//获取与调用线程相关的TSD关键字所绑定的值。
void *thpread_getspecific(pthread_key_t key);
```
