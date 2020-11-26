#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>


void main(){
        int fd = socket(AF_INET,SOCK_STREAM,0) ;
    
        struct sockaddr_in addr ;
        addr.sin_family = AF_INET ;
        addr.sin_port = htons(1234) ;
        inet_aton("192.168.73.129",&addr.sin_addr) ;
        int r = connect(fd,(struct sockaddr*)&addr,sizeof addr) ;
        if ( -1 == r ) perror("connect") , exit(1) ;
    
        char buf[1024] = {} ;   
        while( fgets(buf,1024,stdin) != NULL ) { 
                printf("buf == %s\n",buf) ;
                write(fd,buf,strlen(buf) +1 )  ;
                memset(buf,0x00,sizeof buf) ;
                int r = read(fd,buf,1024) ;
                if ( -1 == r ) { 
                        close(fd) ;
                        break ;
                }
                printf("buf == %s\n",buf) ;
        }
}
        