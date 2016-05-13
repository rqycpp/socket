##Exercise 1
> 当TCP连接的两端都已关闭了TCP连接时，为什么TCP主动关闭的一端还要在状态TIME_WAIT下等待一段时间才删除原来的连接记录，并返回到初始的CLOSED状态。

执行主动关闭的一端进入TIME_WAIT状态，并且留在该状态的持续时间是报文最长生命周期的两倍，有时称为2MSL(Maximum Segment Lifetime)。

(1) 实现终止TCP连接的可靠性，如果终止连接的第4个分节ACK丢失了，那么主动关闭者必须重发。假设主动关闭的一端忽略TIME_WAIT状态，直接进入CLOSED状态，
被动关闭的一端会以为之前发送的FIN包对方没有收到，会重新发送一个FIN包。这时主动关闭的一端收到该FIN后发现旧的TCP连接已经不存在了，系统此时只能返回
RST包。

(2) 保证原来连接上的重复分节在网络中消失。假设主动关闭的一端忽略TIME_WAIT状态，直接进入CLOSED状态。这时主动关闭端可能收到延迟包，若此时已经建立了
新的TCP连接，这种来自于旧TCP连接中延迟包很可能会干扰新建立的TCP连接。由此可以看出TIME_WAIT的重要性，主动关闭端可以在2MSL的时段内处理遗留问题。

推荐文章：

- [理解TIME_WAIT](http://www.firefoxbug.com/index.php/archives/2795/) 
- [再续TIME_WAIT](http://huoding.com/2013/12/31/316)

##Exercise 2
> 从服务器进程的角度来分析TCP建立连接和关闭连接时，其状态的变化过程。

##TCP套接字编程实例
>服务器端程序：[TCPserver](TCPserver.c)

>客户端程序：[TCPclient](TCPclient.c)

首先在窗口1中运行服务器端程序：`./TCPserver`

然后再窗口2中运行客户端程序：`./TCPclient 127.0.0.1`

服务器端运行结果：`You got a connection from client's ip is 127.0.0.1, port is 53064.`

客户端运行结果：`server message: Welcome`