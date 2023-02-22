# ARP


## 功能

实现任意网络层地址到物理层地址的转换


## 工作原理

1. 主机向自身所在网络广播一个arp请求（包含目标机器的网络地址)
2. 被请求的目标机器响应一个arp应答（包含自己的物理地址）
3. 其他非目标机器收到arp请求后丢弃


## 报文格式

<table>
    <tr> 
        <td> 2bytes <br> 硬件类型 </td>
        <td> 2bytes <br> 协议类型 </td>
        <td> 1bytes <br> 硬件地址长度 </td>
        <td> 1bytes <br> 协议地址长度 </td>
        <td> 2bytes <br> 操作 </td>
        <td> 6types <br> 发送端以太网地址 </td>
        <td> 4types <br> 发送端IP地址 </td>
        <td> 6types <br> 目的端以太网地址 </td>
        <td> 4types <br> 目的端IP地址 </td>
    </tr>
</table>

```

/* ARP protocol opcodes. */
#define ARPOP_REQUEST   1       /* ARP request       */
#define ARPOP_REPLY     2       /* ARP reply         */
#define ARPOP_RREQUEST  3       /* RARP request      */
#define ARPOP_RREPLY    4       /* RARP reply        */

#define ETH_P_ARP   0x0806      /* Address Resolution packet    */


struct arphdr {
    unsigned short      ar_hrd;     /* format of hardware address   */
    unsigned short      ar_pro;     /* format of protocol address   */
    unsigned char       ar_hln;     /* length of hardware address   */
    unsigned char       ar_pln;     /* length of protocol address   */
    unsigned short      ar_op;      /* ARP opcode (command)     */

#if 0
    /*
     *  Ethernet looks like this : This bit is variable sized however...
     */
    unsigned char       ar_sha[6];   /* sender hardware address  */
    unsigned char       ar_sip[4];   /* sender IP address        */
    unsigned char       ar_tha[6];   /* target hardware address  */
    unsigned char       ar_tip[4];   /* target IP address        */
#endif
};

```

以上是linux中对arp协议的定义。



## ARP 高速缓存


维护一个缓存，包含经常使用或最近使用的机器IP地址到物理地址的映射。

避免重复的arp请求，提高发送数据包的速度。

Linux下可以通过`arp`命令查看或修改arp缓存。



