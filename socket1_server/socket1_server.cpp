#ifndef __FILEOP_H_
#define __FILEOP_H_
#define WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll


long g_fileSize = 75288;//�����ļ���С
char* g_fileBUf ;//�����ļ�����


int main() {
    printf("��ӭ������༾ʥ��FTPϵͳ\n");
    //��ʼ�� DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //�����׽���
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //���׽���
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
    sockAddr.sin_port = htons(1234);  //�˿�
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //�������״̬
    listen(servSock, 20);
    //���տͻ�������
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
    printf("�Ϳͻ������ӳɹ�\n");
    while (1) {
        printf("����������Ҫ����Ҫ���еĹ���:\n1.�ϴ��ļ�   2.�����ļ�   3.�˳�ϵͳ\n");
        ////���շ��������ص�����
        //char szBuffer[MAXBYTE] = { 0 };
        //recv(sock, szBuffer, MAXBYTE, NULL);
        ////������յ�������
        //printf("������������Ϣ: %s", szBuffer);
        //puts("");
        //�����˷�������
        char* str = "jsp1";
        char tmp[100];
        //printf("���������������˷��͵�����:");
        scanf("%s", tmp);
        str = tmp;
        send(clntSock, str, strlen(str) + sizeof(char), NULL);
        if (str[0] == '1') {
            char* str1 = "jsp1";
            const char* fileName;
            char tmp1[100];
            printf("������������ͻ��˷��͵��ļ���·��:");
            scanf("%s", tmp1);
            str1 = tmp1;
            fileName = tmp1;
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

            int ret = send(clntSock, g_fileBUf, g_fileSize, NULL);
            printf("���ͳɹ�(%d)�ֽ�:\n", ret);

           /* send(clntSock, str1, strlen(str1) + sizeof(char), NULL);*/
            printf("\n\n\n\n");
        }
        else {
            if (str[0] == '2') {
                char* str1 = "jsp1";
                const char* fileName;
                char tmp1[100];
                printf("����������ӿͻ������ص��ļ���·��:");
                scanf("%s", tmp1);
                str1 = tmp1;
                send(clntSock, str1, strlen(str1) + sizeof(char), NULL);


                char szBuffer1[MAXBYTE] = { 0 };
                int ret = recv(clntSock, szBuffer1, MAXBYTE, NULL);
                //������յ�������
                printf("�ɹ�����%d�ֽ�\n", ret);
                FILE* write = fopen("D:\\������\\vs������\\socket1_server\\�����ļ�1.txt", "wb");
                fwrite(szBuffer1, sizeof(char), g_fileSize, write);
                fclose(write);
                printf("\n\n\n\n");
            }
            else {
                if (str[0] == '3') {
                    printf("������˳�ϵͳ\n");
                    break;
                }
            }
        }
    }
    //while (1) {
    //    ////��ͻ��˷�������
    //    //char* str = "jsp";
    //    //char tmp[100];
    //    //printf("������������ͻ��˷��͵��ļ���:");
    //    //scanf("%s", tmp);
    //    //str = tmp;
    //    //send(clntSock, str, strlen(str) + sizeof(char), NULL);
    //    //���տͻ��˴��ص�����
    //    char szBuffer[MAXBYTE] = { 0 };
    //    recv(clntSock, szBuffer, MAXBYTE, NULL);
    //    //������յ�������
    //    printf("�ͻ������㷢����Ϣ: %s\n", szBuffer);
    //    if (szBuffer[0] == '1') {
    //        int ret = recv(clntSock, g_fileBUf, g_fileSize, NULL);
    //        printf("���ͳɹ�(%d)�ֽ�:\n", ret);
    //        //char szBuffer1[MAXBYTE] = { 0 };
    //        //recv(clntSock, szBuffer1,MAXBYTE, NULL);
    //        ////������յ�������
    //        //printf("�ͻ������㷢����Ϣ: %s\n", szBuffer1);
    //        //FILE* write = fopen("C:\\Users\\��ʥ���ĸɶ���\\Desktop\\1.png", "wb");
    //        //fwrite(g_fileBUf, sizeof(char), g_fileSize, write);
    //        //fclose(write);
    //    }
    //    else {
    //        if (szBuffer[0] == '2') {
    //            printf("2\n");
    //        }
    //        else {
    //            if (szBuffer[0] == '3') {
    //                printf("������˳�ϵͳ\n");
    //                break;
    //            }
    //        }
    //    }
    //}
    //�ر��׽���
    closesocket(clntSock);
    closesocket(servSock);
    //��ֹ DLL ��ʹ��
    WSACleanup();
    return 0;
}

#endif