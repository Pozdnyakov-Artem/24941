#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    char** arr=NULL;
    char str[1000];
    int count=0;

    while(fgets(str,1000,stdin))
    {
        if(str[0]!='.')
        {
            char **temp = (char**)realloc(arr,(count+1)*sizeof(char*));

            if(temp==NULL){
                perror("Error realloc");
                break;
            }

            temp[count++]=strdup(str);

            arr=temp;
        }
        else
        {
            break;
        }
    }

    for(int i=0;i<count;i++)
    {
        printf("%s",arr[i]);
        free(arr[i]);
    }

    free(arr);

    return 0;
}