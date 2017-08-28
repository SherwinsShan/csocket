# csocket
linux c下的tcp/udp接口封装（C/S）

由socketNromal重构而成

### 特征 
1. 封装了tcp/udp基本的绑定、连接、读写、超时等待、广播功能，极大地简化socket操作；
2. 面向对象化，连接关系如下：
```
         (tcp/udp)Server <--------- (tcp/udp)Client 
              |                           |
             生成                        变为
              ↓                           ↓
         (tcp/udp)Connect <-------> (tcp/udp)Connect
```
3. 提供伪同步事件宏，提供代码可读性

