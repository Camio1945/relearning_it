# @Deprecated 废弃说明

本来是想学一下使用 C 语言怎么解决 C10k 问题，但是原仓库中并没有压测示例，因此不确定是否值得深入研究。

***

### 总体说明

1. NPIP 是 Network Programming In Practice（网络编程实战）的缩写。
2. 四个文件夹分别对应四种语言：C、Go、Java、Rust。
3. C 语言的代码来自 [https://github.com/froghui/yolanda](https://github.com/froghui/yolanda) ，代码结构做了一些调整，是的每个 c 文件可以单独运行；其他语言的代码是仿照  C 语言的功能写的。
4. 为了让每个文件都能单独运行，系统中有大量的重复代码。
5. 2023年12月31日更新：由于我是在 Windows 上用 JetBrains 家族的 IDE 开发的，而目标又是想运行在 Linux 服务器上的，只有 C 语言可以通过配置 WSL 编译环境来很好地支持这种场景，因此其他几种语言的中途的功能都会略掉，可能只在最终的一个环节才会用其他三种语言写一遍来做对比。（比如 Rust 上有一个 UnixListener 类是只在 Unix 环境中才存在的，在 Windows 上编译器直接会报错）

***

### 子文件夹说明

以 NPIP_C 为例，其下的子文件夹的功能如下：

t01_tcp_handshake：tcp 握手，服务端监听，客户端连接，不会发送数据。

t02_tcp_data_transfer：服务端与客户端使用 tcp 协议进行数据传输。

t03_udp_data_transfer：服务端与客户端使用 udp 协议进行数据传输。

t04_local_data_stream：服务端与客户端使用本地套间字进行数据流传输。

t04_local_datagram：服务端与客户端使用本地套间字进行数据报传输。

t06_grace_close：怎样优雅地关闭连接。这里面也有关于 select() I/O多路复用的例子。

t07_keep_alive：应用层心跳检测探活。

t08_batch_write：小数据包批量写。

t09_udp_connect：UDP 也可以使用 connect() 方法将 UDP 套接字和 IPv4 地址进行“绑定”。

t10_reuse_address：使用 SO_REUSEADDR 选项来重用地址以减少 "Address in use" 的问题。

t11_tcp_stream：如何理解 TCP 的“流”。

t12_reliable_tcp：增强 TCP 的可靠性。

t13_poll：使用 poll() 进行 I/O 多路复用。

t14_non_blocking：非阻塞 I/O

t15_epoll：使用 epoll() 进行 I/O 多路复用。

t16_fork：阻塞 I/O + 进程 模型。

t17_thread：阻塞 I/O + 线程 模型。

t18_poll_server_one_thread： poll() I/O 多路复用 + 单线程 模型。

t19_poll_server_multi_threads：poll() I/O 多路复用 + 多线程 模型。

t20_epoll_server_multi_threads：poll() I/O 多路复用 + 多线程 模型。

t21_aio：异步 I/O 。

t22_http_server：一个 http 服务器。