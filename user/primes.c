#include "kernel/types.h"
#include "user/user.h"

void sieve(int fd){
    int prime;//本进程接受的素数
    int num;//用于过滤
    int p[2];

    int ret;
    ret = read(fd, &prime, sizeof(int));//读取第一个数,为了防止读到0，所以对ret进行判断
    if(ret == 0){
        exit(0);
    }
    printf("prime %d\n", prime);//读到的第一个数为本进程接受的素数，以此素数为基础过滤
    
    
    pipe(p);
    if(fork() == 0){//过滤后子节点接受
        close(p[1]);
        close(fd);
        sieve(p[0]);
        exit(0);
    }else{
        close(p[0]);
        while(read(fd,&num,4) > 0){
            if(num % prime != 0){
                write(p[1],&num,4);
            }
        }
        close(p[1]);
        close(fd);
        wait(0);
        exit(0);
    }

}

int main(){
    int fd[2];
    int ret;
    ret = pipe(fd);
    if(ret == -1){
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    for(int i = 2; i <= 35; i++){
        write(fd[1], &i, sizeof(int));
    }
    close(fd[1]);
    sieve(fd[0]);
    exit(0);
}