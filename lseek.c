#include <stdio.h>
#include <unistd.h>

int main(){

    int fd = open("file.txt");

    char str[1000];

    while(read(fd, str, sizeof(str))>0){
        printf("%s\n",str);
    }

    // printf("=========table==========");

    close(fd);

    return 0;
}