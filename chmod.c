#include <stdio.h>

void print_id_and_open_file(){

    printf("ID = %d\n UID = %d\n",getuid(),getueid());

    FILE *file = fopen("cool.txt","r");

    if(file==NULL)
    {
        perror("Error open file");
    }
    else
    {
        fclose(file);
    }
}


int main(){

    print_id_and_open_file();    

    setuid(getuid());

    print_id_and_open_file();

    return 0;
}