#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    int fd1[2];
    int fd2[2];
    int ret;
    char buf[1] = {'a'};
    int pid;
    ret = pipe(fd1);
    if(ret == -1){
        fprintf(2,"pipe failed\n");
        exit(1);
    }
    ret = pipe(fd2);
    if(ret == -1){
        fprintf(2,"pipe failed\n");
        exit(1);
    }

    if((pid = fork()) > 0){
        close(fd1[0]);//关闭读端
        close(fd2[1]);//关闭写端
        ret = write(fd1[1],buf,1);
        if(ret == -1){
            fprintf(2,"write failed\n");
            exit(1);
        }
        ret = read(fd2[0],buf,1);
        if(ret == -1){
            fprintf(2,"read failed\n");
            exit(1);
        }
        printf("%d: received pong\n",getpid());
    }else{
        close(fd1[1]);//关闭写端
        close(fd2[0]);//关闭读端
        ret = read(fd1[0],buf,1);
        if(ret == -1){
            fprintf(2,"read failed\n");
            exit(1);
        }
        printf("%d: received ping\n",pid);
        ret = write(fd2[1],buf,1);
        if(ret == -1){
            fprintf(2,"write failed\n");
            exit(1);
        }
    }
    exit(0);
}