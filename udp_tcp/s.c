#include "headerFiles.h"

#define BUFF_SIZE 256
#define PORT_NUMBER 50000
#define PORT_TCP 50001

int   main( void)
{
    int   sock;  //udp socket
    int   sock2; //tcp_listen
    int   s;     //tcp socket
    int   client_addr_size;
 
    //UDP
    struct sockaddr_in   server_addr;
    struct sockaddr_in   client_addr;
    char  buff_rcv[BUFF_SIZE+1];
    
    //TCP
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    int clntAddrlen;
    int waitSize = 16;
    char buffer[256];
    char *ptr = buffer;
    int maxLen = sizeof(buffer);
    int n;
    int len=0;
    int test;


    //TCP socket 설정
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT_TCP);
 
    //TCP socket
    sock2 = socket(PF_INET, SOCK_STREAM, 0);


    //UDP socket 설정
    memset( &server_addr, 0, sizeof( server_addr));
    server_addr.sin_family     = AF_INET;
    server_addr.sin_port       = htons(PORT_NUMBER);
    server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

     //UDP socket
    sock  = socket(PF_INET, SOCK_DGRAM, 0);




    if( -1 == sock)
    {
        printf( "UDP socket 생성 실패\n");
        exit(1);
    }
 
    if(-1 == sock2){
        printf("TCP socket 생성 실패\n");
        exit(1);
    }

 
    //UDP bind
    if( -1 == bind( sock, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
    {
       printf( "UDP bind() 실행 에러\n");
       exit( 1);
    }
    client_addr_size  = sizeof( client_addr);

    //TCP bind
    if(bind(sock2, (struct sockaddr*)&servAddr, sizeof(servAddr))<0){
        printf("TCP bind() 실행 에러\n");
    }
    //TCP listen
    if(listen(sock2, waitSize)<0){
        printf("TCP listen ERROR");
        exit(1);
    }
    clntAddrlen = sizeof(clntAddr);

    //TCP
    if((s=accept(sock2, &clntAddr, &clntAddrlen))<0){
        printf("TCP accept ERROR");
        exit(1);
    }

 
    while(1)
    {
       printf("\n");


       //UDP
       recvfrom( sock, buff_rcv, BUFF_SIZE, 0 , (struct sockaddr*)&client_addr, &client_addr_size);
       printf( "UDP : receive: %s", buff_rcv);
       sendto(sock, buff_rcv, strlen(buff_rcv)+1, 0, (struct sockaddr*)&client_addr, sizeof( client_addr)); 
        printf("\n");
        
        while((n = recv(s, ptr, maxLen, 0)) > 0){
            printf("TCP : received:");
            len += n;
            fputs(buffer, stdout);
            test = send(s, ptr, len, 0);
            len=0;
            break;
        }


    }
    close(sock);
    close(s);

}
