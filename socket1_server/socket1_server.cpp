#ifndef __FILEOP_H_
#define __FILEOP_H_
#define WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll


long g_fileSize = 75288;//保存文件大小
char* g_fileBUf ;//保存文件数据


int main() {
    printf("欢迎来到五班季圣鹏FTP系统\n");
    //初始化 DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //绑定套接字
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(1234);  //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //进入监听状态
    listen(servSock, 20);
    //接收客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
    printf("和客户端连接成功\n");
    while (1) {
        printf("请输入你想要你想要进行的功能:\n1.上传文件   2.下载文件   3.退出系统\n");
        ////接收服务器传回的数据
        //char szBuffer[MAXBYTE] = { 0 };
        //recv(sock, szBuffer, MAXBYTE, NULL);
        ////输出接收到的数据
        //printf("服务器发来信息: %s", szBuffer);
        //puts("");
        //向服务端发送数据
        char* str = "jsp1";
        char tmp[100];
        //printf("请输入你想向服务端发送的文字:");
        scanf("%s", tmp);
        str = tmp;
        send(clntSock, str, strlen(str) + sizeof(char), NULL);
        if (str[0] == '1') {
            char* str1 = "jsp1";
            const char* fileName;
            char tmp1[100];
            printf("请输入你想向客户端发送的文件的路径:");
            scanf("%s", tmp1);
            str1 = tmp1;
            fileName = tmp1;
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

            int ret = send(clntSock, g_fileBUf, g_fileSize, NULL);
            printf("发送成功(%d)字节:\n", ret);

           /* send(clntSock, str1, strlen(str1) + sizeof(char), NULL);*/
            printf("\n\n\n\n");
        }
        else {
            if (str[0] == '2') {
                char* str1 = "jsp1";
                const char* fileName;
                char tmp1[100];
                printf("请输入你想从客户端下载的文件的路径:");
                scanf("%s", tmp1);
                str1 = tmp1;
                send(clntSock, str1, strlen(str1) + sizeof(char), NULL);


                char szBuffer1[MAXBYTE] = { 0 };
                int ret = recv(clntSock, szBuffer1, MAXBYTE, NULL);
                //输出接收到的数据
                printf("成功发送%d字节\n", ret);
                FILE* write = fopen("D:\\编译器\\vs代码区\\socket1_server\\下载文件1.txt", "wb");
                fwrite(szBuffer1, sizeof(char), g_fileSize, write);
                fclose(write);
                printf("\n\n\n\n");
            }
            else {
                if (str[0] == '3') {
                    printf("服务端退出系统\n");
                    break;
                }
            }
        }
    }
    //while (1) {
    //    ////向客户端发送数据
    //    //char* str = "jsp";
    //    //char tmp[100];
    //    //printf("请输入你想向客户端发送的文件名:");
    //    //scanf("%s", tmp);
    //    //str = tmp;
    //    //send(clntSock, str, strlen(str) + sizeof(char), NULL);
    //    //接收客户端传回的数据
    //    char szBuffer[MAXBYTE] = { 0 };
    //    recv(clntSock, szBuffer, MAXBYTE, NULL);
    //    //输出接收到的数据
    //    printf("客户端向你发来信息: %s\n", szBuffer);
    //    if (szBuffer[0] == '1') {
    //        int ret = recv(clntSock, g_fileBUf, g_fileSize, NULL);
    //        printf("发送成功(%d)字节:\n", ret);
    //        //char szBuffer1[MAXBYTE] = { 0 };
    //        //recv(clntSock, szBuffer1,MAXBYTE, NULL);
    //        ////输出接收到的数据
    //        //printf("客户端向你发来信息: %s\n", szBuffer1);
    //        //FILE* write = fopen("C:\\Users\\季圣鹏的干儿子\\Desktop\\1.png", "wb");
    //        //fwrite(g_fileBUf, sizeof(char), g_fileSize, write);
    //        //fclose(write);
    //    }
    //    else {
    //        if (szBuffer[0] == '2') {
    //            printf("2\n");
    //        }
    //        else {
    //            if (szBuffer[0] == '3') {
    //                printf("服务端退出系统\n");
    //                break;
    //            }
    //        }
    //    }
    //}
    //关闭套接字
    closesocket(clntSock);
    closesocket(servSock);
    //终止 DLL 的使用
    WSACleanup();
    return 0;
}

#endif