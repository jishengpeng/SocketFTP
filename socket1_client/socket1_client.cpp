#ifndef __FILEOP_H_
#define __FILEOP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll

long g_fileSize = 2;//�����ļ���С
char* g_fileBUf;//�����ļ�����


int main() {
    //��ʼ��DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //�����׽���
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //���������������
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    printf("�ͷ���˽�������\n");
    while (1) {
        ////��ͻ��˷�������
        //char* str = "jsp";
        //char tmp[100];
        //printf("������������ͻ��˷��͵��ļ���:");
        //scanf("%s", tmp);
        //str = tmp;
        //send(clntSock, str, strlen(str) + sizeof(char), NULL);
        //���տͻ��˴��ص�����
        char szBuffer[MAXBYTE] = { 0 };
        recv(sock, szBuffer, MAXBYTE, NULL);
        //������յ�������
        printf("��������㷢����Ϣ: %s\n", szBuffer);
        if (szBuffer[0] == '1') {
            /*int ret = recv(sock, g_fileBUf, g_fileSize, NULL);
            printf("���ͳɹ�(%d)�ֽ�:\n", ret);*/
            char szBuffer1[MAXBYTE] = { 0 };
            int ret=recv(sock, szBuffer1, MAXBYTE, NULL);
            //������յ�������
            printf("�ɹ�����%d�ֽ�\n",ret );
            FILE* write = fopen("C:\\Users\\��ʥ���ĸɶ���\\Desktop\\�ϴ��ļ�1.txt", "wb");
            fwrite(szBuffer1, sizeof(char), g_fileSize, write);
            fclose(write);
        }
        else {
            if (szBuffer[0] == '2') {
                char szBuffer1[MAXBYTE] = { 0 };
                recv(sock, szBuffer1, MAXBYTE, NULL);
                //������յ�������
                printf("�Ѿ��ҵ���Ҫ���ص��ļ���ַ\n");
                FILE* read = fopen("D:\\������\\vs������\\socket1_server\\1.txt", "rb");
                if (!read) {
                    perror("file open failed: \n");
                }
                //��ȡ�ļ���С
                fseek(read, 0, SEEK_END);//��λ��ָ��ƫ�Ƶ����
                g_fileSize = ftell(read);
                fseek(read, 0, SEEK_SET);//�ƶ�����ͷ
                printf("�ļ��ߴ�Ĵ�С:%d\n", g_fileSize);

                //���ļ������ڴ�
                g_fileBUf = (char*)calloc(g_fileSize, sizeof(char));
                //���ļ������ڴ���
                fread(g_fileBUf, sizeof(char), g_fileSize, read);
                fclose(read);

                int ret = send(sock, g_fileBUf, g_fileSize, NULL);
                printf("���ͳɹ�(%d)�ֽ�:\n", ret);
            }
            else {
                if (szBuffer[0] == '3') {
                    printf("�ͻ����˳�ϵͳ\n");
                    break;
                }
            }
        }
    }
    //�ر��׽���
    closesocket(sock);
    //��ֹʹ�� DLL
    WSACleanup();
    system("pause");
    return 0;
}

#endif