##Exercise 1
> 当TCP连接的两端都已关闭了TCP连接时，为什么TCP主动关闭的一端还要在状态TIME_WAIT下等待一段时间才删除原来的连接记录，并返回到初始的CLOSED状态。

##Exercise 2
> 从服务器进程的角度来分析TCP建立连接和关闭连接时，其状态的变化过程。

##TCP套接字编程实例
>服务器端程序：[TCPserver](ch01/TCPserver.c)

>客户端程序：[TCPclient](ch01/TCPclient.c)

首先在窗口1中运行服务器端程序：`./TCPserver`

然后再窗口2中运行客户端程序:`./TCPclient 127.0.0.1`

服务器端运行结果：`You got a connection from client's ip is 127.0.0.1, port is 53064.`

客户端运行结果：`server message: Welcome`