#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MAXARG 32

char *cutoffinput(char *buf);
void substring(char s[], char *sub, int pos, int len);


/* 子串 */
void substring(char s[], char *sub, int pos, int len) {
   int c = 0;   
   while (c < len) {
      *(sub + c) = s[pos+c];
      c++;
   }
   *(sub + c) = '\0';
}

/* 截断 '\n' */
char* cutoffinput(char *buf){
    /* 记得要为char *新分配一片地址空间，否则编译器默认指向同一片地址 */
    if(strlen(buf) > 1 && buf[strlen(buf) - 1] == '\n'){
        char *subbuff = (char*)malloc(sizeof(char) * (strlen(buf) - 1));
        substring(buf, subbuff, 0, strlen(buf) - 1);
        return subbuff;
    }
    else
    {
        char *subbuff = (char*)malloc(sizeof(char) * strlen(buf));
        strcpy(subbuff, buf);
        return subbuff;
    }
}

int main(int argc, char *argv[])
{
    /* code */
    int pid;
    char buf[MAXPATH];
    char *args[MAXARG];//存放每一行的参数
    char *exec_args[MAXARG];//存放exec的参数
    char *cmd;
    int num = 0;
    int i = 0;

    if(argc == 1){
        cmd = "echo";
    }
    else{
        cmd = argv[1];
    }

    while(1){
        memset(buf, 0, sizeof(buf));
        gets(buf, sizeof(buf));
        if(strlen(buf) == 0 || num > MAXARG){
            break;
        }
        char *subbuff = cutoffinput(buf);//截断'\n'
        args[num++] = subbuff;
    }
    //已经把输入的命令存入args数组中 这里不包括argv[]
    //将args数组中的命令转换为exec的参数
    //将args和argv进行拼接
    exec_args[0] = cmd;
    for(i = 1; i < argc - 1; i++){
        exec_args[i] = argv[i + 1];
    }
    for(int j = 0; j < num; j++){
        exec_args[i++] = args[j];
    }
    exec_args[i] = 0;
    pid = fork();
    if(pid == 0){
        exec(cmd, exec_args);
        exit(0);
    }
    else{
        wait(0);
    }
    exit(0);
}



