#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

char* file_name(char *path){
    char *p = path + strlen(path);
    while(p >= path && *p != '/'){
        p--;
    }
    p++;
    return p;
}

void find(char *path,char *file){
    struct dirent de;//一个目录项
    struct stat st;
    int fd;
    int ret;
    char buf[512];
    char *p;
    fd = open(path,O_RDONLY);
    if(fd < 0){
        fprintf(2,"find: cannot open %s\n",path);
        return;
    }
    ret = stat(path,&st);
    if(ret < 0){
        fprintf(2,"find: cannot stat %s\n",path);
        close(fd);
        return;
    }
    switch(st.type){
        case T_DIR://如果是目录的话 就进入目录 递归查找
            memset(buf,0,sizeof(buf));//清空buf
            uint path_len = strlen(path);
            memmove(buf,path,path_len);
            buf[path_len] = '/';
            p = buf + path_len + 1;
            while(read(fd,&de,sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0){
                    continue;
                }
                memcpy(p,de.name,DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf,file);
            }
            close(fd);
        break;
        case T_FILE: //如果是文件的话，首先从path中提取文件名，然后判断是否和file相等
            char *filename = file_name(path);
            int match = 0;
            if(strcmp(filename,file) == 0){
                match = 1;
            }
            if(match){
                fprintf(1,"%s\n",path);
            }
            close(fd);
        break;
            
    }
}

int main(int argc,char *argv[]){
    if(argc != 3){
        fprintf(2,"usage: find <path> <name>\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}