#ifndef __FILEOP_H_
#define __FILEOP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

long g_fileSize = 2;//保存文件大小
char* g_fileBUf;//保存文件数据


int main() {
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起请求
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    printf("和服务端建立连接\n");
    while (1) {
        ////向客户端发送数据
        //char* str = "jsp";
        //char tmp[100];
        //printf("请输入你想向客户端发送的文件名:");
        //scanf("%s", tmp);
        //str = tmp;
        //send(clntSock, str, strlen(str) + sizeof(char), NULL);
        //接收客户端传回的数据
        char szBuffer[MAXBYTE] = { 0 };
        recv(sock, szBuffer, MAXBYTE, NULL);
        //输出接收到的数据
        printf("服务端向你发来信息: %s\n", szBuffer);
        if (szBuffer[0] == '1') {
            /*int ret = recv(sock, g_fileBUf, g_fileSize, NULL);
            printf("发送成功(%d)字节:\n", ret);*/
            char szBuffer1[MAXBYTE] = { 0 };
            int ret=recv(sock, szBuffer1, MAXBYTE, NULL);
            //输出接收到的数据
            printf("成功发送%d字节\n",ret );
            FILE* write = fopen("C:\\Users\\季圣鹏的干儿子\\Desktop\\上传文件1.txt", "wb");
            fwrite(szBuffer1, sizeof(char), g_fileSize, write);
            fclose(write);
        }
        else {
            if (szBuffer[0] == '2') {
                char szBuffer1[MAXBYTE] = { 0 };
                recv(sock, szBuffer1, MAXBYTE, NULL);
                //输出接收到的数据
                printf("已经找到需要下载的文件地址\n");
                FILE* read = fopen("D:\\编译器\\vs代码区\\socket1_server\\1.txt", "rb");
                if (!read) {
                    perror("file open failed: \n");
                }
                //获取文件大小
                fseek(read, 0, SEEK_END);//将位置指针偏移到最后
                g_fileSize = ftell(read);
                fseek(read, 0, SEEK_SET);//移动到开头
                printf("文件尺寸的大小:%d\n", g_fileSize);

                //将文件分配内存
                g_fileBUf = (char*)calloc(g_fileSize, sizeof(char));
                //将文件读到内存中
                fread(g_fileBUf, sizeof(char), g_fileSize, read);
                fclose(read);

                int ret = send(sock, g_fileBUf, g_fileSize, NULL);
                printf("发送成功(%d)字节:\n", ret);
            }
            else {
                if (szBuffer[0] == '3') {
                    printf("客户端退出系统\n");
                    break;
                }
            }
        }
    }
    //关闭套接字
    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
    system("pause");
    return 0;
}

#endif