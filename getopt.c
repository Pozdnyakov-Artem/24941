#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <ulimit.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

void print_id_user(){
    printf("Real ID %d\n",getuid());
    printf("Effective ID: %d\n",geteuid());
}

void become_a_proc_leader(){
    if (setpgid(0,0) == -1){
        printf("Fail setpgid\n");
    }
    else{
        printf("Successful setpgid\n");
    }
}

void print_id_proc(){
    printf("Process: PID=%d\nParent PID=%d\nGroup PID=%d\n", getpid(), getppid(),getpgrp());
}

void print_ulimit(){
    long limit = ulimit(UL_GETFSIZE);

    if(limit==-1 && errno != 0){
        perror("Error getting ulimit\n");
    }
    else{
        printf("ulimit:  %ld\n",limit);   
    }

}

void print_size_corfile(){
    struct rlimit limit;

    if(getrlimit(RLIMIT_CORE,&limit)==0){
        if(limit.rlim_cur==RLIM_INFINITY){
            printf("Size cor-file: undefinded\n");
        }
        else{
            printf("Max size cor-file: %lld",(long long) limit.rlim_cur);
        }
    }
    else{
        printf("Error getrlimit\n");
    }
}

void print_current_directory(){
    char cwd[PATH_MAX];
    if(getcwd(cwd,PATH_MAX)!=NULL){
        printf("Current directory: %s\n",cwd);
    }
    else{
        perror("getcwd\n");
    }
}

void print_environment(){
    extern char **environ;
    char **env = environ;
    printf("Environment variables:\n");
    while(*env){
        printf("%s\n",*env);
        env++;
    }
}

int set_environment_variable(char*str){

    
    if(putenv(str)==-1){
        perror("Error changing or creating envirnment variable");
        return -1;
    }
    printf("Set environment variable successful");

    return 0;

}

int set_core_size(const char *str){

    char *endptr;
    char*str_copy = strdup(str);
    long new_size=strtol(str_copy,&endptr,10);
    if(*endptr!='\0'){
        fprintf(stderr, "Invalid core size value: %s\n", str_copy);
        return -1;
    }

    struct rlimit limit;
    limit.rlim_cur = (rlim_t)new_size;
    limit.rlim_max = (rlim_t)new_size;

    if(setrlimit(RLIMIT_CORE,&limit)==-1){
        perror("setrlimit");
        return -1;
    }

    printf("Core file size limit changed to: %ld bytes\n", new_size);
    return 0;

}

void set_new_ulimit(const char* str){
    char*endptr;
    long new_val = strtol(str,&endptr,10);

    if(*endptr!='\0'){
        fprintf(stderr, "Invalid ulimit value: %s\n", str);
    }
    else{
        long new_ulimit = ulimit(UL_SETFSIZE,new_val);
        printf("New limit: %ld\n",new_val);
    }

}

int main(int argc, char *argv[])
{

    int c;
    int i;

    if(argc<=1){
        printf("No arguments\n");
        return 0;
    }

    for(i=argc-1; i>0; i--)
    {
        int temp_argc = 2, optind = 1;
        char* temp_argv[] = {argv[0],argv[i],NULL};
        int count=0;

        while((c = getopt(temp_argc,temp_argv,"ispuU:cC:dvV:"))!=-1)
        {
            count++;
            switch(c){
                case 'i':
                    print_id_user();
                    break;
                case 's':
                    become_a_proc_leader();
                    break;
                case 'p':
                    print_id_proc();
                    break;
                case 'u':
                    print_ulimit();
                    break;
                case 'c':
                    print_size_corfile();
                    break;
                case 'd':
                    print_current_directory();
                    break;
                case 'v':
                    print_environment();
                    break;
                case 'V':
                    set_environment_variable(optarg);
                    break;
                case 'C':
                    set_core_size(optarg);
                    break;
                case 'U':
                    set_new_ulimit(optarg);
                    break;
                case '?':
                    printf("Invalid option %c\n",optarg);
                    break;
            }   

        }

        if(count==0)
            printf("No options\n");

    }

    return 0;
}