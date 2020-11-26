#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

void parse(char* buf){
        int i = 0 ; 
        while(1){
                if ( buf[i] == '\0') 
                        break ;
                if ( buf[i] >= 'a' && buf[i] <= 'z' ) 
                        buf[i] -= 'a' - 'A' ;
                ++i ;
        }
}
/*
 *      当客户端连接上来之后，服务器创建一个进程对其进行服务，服务完之后，客户>端强行下线，则导致僵尸进程
 *      因此：此处需要处理僵尸进程
 */
void handler(int s){
        // 解决同时有多个客户端连接上来之后
        // 导致后续无法被处理的问题
        // 所有的僵尸子进程都被回收
        while ( waitpid(-1 , NULL, WNOHANG) != -1) ;
}
void main(){

        struct sigaction act ;
        act.sa_handler = handler ;
        act.sa_flags = 0 ;
        sigemptyset(&act.sa_mask) ;
        sigaction(SIGCHLD,&act,NULL) ;

        int fd = socket(AF_INET,SOCK_STREAM,0) ;
        struct sockaddr_in addr ;
        addr.sin_family = AF_INET ;
        addr.sin_port = htons(1234) ;
        inet_aton("192.168.73.129",&addr.sin_addr) ;
        int r = bind(fd,(struct sockaddr*)&addr,sizeof addr ) ;
        if ( -1 == r ) perror("bind") , exit(1) ;

        r = listen(fd,SOMAXCONN) ;
        if ( -1 == r ) perror("listen") , exit(1) ;
        while( 1 ) {
                int newfd = accept(fd,NULL,NULL) ;
                // 当在客户端键入ctrl + c 的时候，父进程也会收到一个同样的信号>，因此需要对其进行处理，否则就会向下进行操作
                if ( -1 == newfd && errno == EINTR) {
                        perror("accept") ;
                        continue ;
                }
                printf("new client connect success\n") ;
                pid_t pid = fork() ;
                if ( 0 == pid ) {
                        close(fd) ;
                        while(1) {
                                char buf[1024] ;
                                memset(buf,0x00,sizeof buf) ;
                                int r = read(newfd,buf,1024) ;
                                if ( 0 == r ) {
                                        break ;
                                }
                                if ( -1 == r ) {
                                        perror("read") ;
                                        exit(0) ;
                                }
                                printf("buf = %s\n",buf) ;
                                parse(buf) ;
                                write(newfd,buf,strlen(buf) + 1 ) ;
                        }
                        close(newfd) ;
                        exit(1) ;
                }
                close(newfd) ;
        }
        close(fd) ;
}

