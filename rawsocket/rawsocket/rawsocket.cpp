#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)

std::string Utf8ToGbk(const char* src_str)
{

	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	std::cout << "接收到的数据段长度为：" << len << std::endl;
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

typedef struct _IP_HEADER
{
	char m_cVersionAndHanderLen; //版本信息（前4位），头长度（后4位）
	char m_cTypeOfService; //服务类型8位
	short m_sTotalLenOfPacket; //数据包长度
	short m_sPacketID; //数据包标识
	short m_sSliceinfo; //分片使用
	char m_cTTL; //存活时间
	char m_cTypeOfProtocol; //协议类型
	short m_sChechSum; //校验和
	unsigned int m_uiSourIp; //源IP地址
	unsigned int m_uiDestIp; //目的IP地址
}IP_HEADER, * PIP_HEADER;

typedef struct _UDP_HEADER
{
	unsigned short m_usSourPort; //源端口号16bit
	unsigned short m_usDestPort; //目的端口号16bit
	unsigned short m_usLength; //数据包长度16bit
	unsigned short m_usCheckSum; //校验和16bit
}UDP_HEADER, * PUDP_HEADER;

int main()
{
	WSADATA wsadata;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsadata); if (ret != 0) { printf("初始化winsock2失败！"); return -1; } //if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) { puts("版本号错误！"); return -1; }

	SOCKADDR_IN saddr;
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	SOCKET sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_IP); if (sockfd < 0) { puts("创建套接字失败！"); return system("pause"); }

	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));

	ret = bind(sockfd, (SOCKADDR*)&saddr, sizeof(SOCKADDR)); if (ret != 0) { puts("绑定失败！"); return system("pause"); }

	DWORD dwlen[10], dwlenrtned = 0, optval = 1;
	WSAIoctl(sockfd, SIO_RCVALL, &optval, sizeof(optval), &dwlen, sizeof(dwlen), &dwlenrtned, NULL, NULL); //允许套接字接收所有经过本机的网络数据包

	char buf[500] = "";
	int len = sizeof(SOCKADDR);
	IP_HEADER iph;
	UDP_HEADER udph;
	int sumcount = 0;
	while (sumcount < 2)
	{
		sumcount++;
		puts("等待接收数据......");
		ret = recvfrom(sockfd, buf, 200, 0, (SOCKADDR*)&saddr, &len); if (ret < 0) { puts("接收错误！"); }

		memcpy(&iph, buf, 20);
		memcpy(&udph, buf + 20, 8);

		in_addr ias, iad;
		ias.S_un.S_addr = iph.m_uiSourIp;
		iad.S_un.S_addr = iph.m_uiDestIp;

		printf("协议类型：%d\n", iph.m_cTypeOfProtocol);
		switch (iph.m_cTypeOfProtocol)
		{
		case IPPROTO_ICMP:
			puts("收到ICMP包");
			break;
		case IPPROTO_IGMP:
			puts("收到IGMP包");
			break;
		case IPPROTO_UDP:
			puts("收到UDP包");
			break;
		case IPPROTO_TCP:
			puts("收到TCP包");
			break;
		default:
			puts("未指定协议类型");
			break;
		}

		char str1[20];
		char str2[20];
		strcpy_s(str1, inet_ntoa(ias));
		strcpy_s(str2, inet_ntoa(iad));

		printf("源地址IP:%s 源地址端口：%d \n目标地址IP：%s 目标地址端口：%d \n", str1, ntohs(udph.m_usSourPort), str2, ntohs(udph.m_usDestPort));


		std::string str = "0";
		str = Utf8ToGbk(buf + 28);
		/*std::cout << "接收到的数据段：" << str << std::endl;*/
		puts("");
	}
	closesocket(sockfd);
	WSACleanup();

	return system("pause");
}