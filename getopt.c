#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <ulimit.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

void print_id_user(){
    printf("Real UID %d\n",getuid());
    printf("Effective UID: %d\n",geteuid());
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

// void print_ulimit(){
//     struct rlimit limit;

//     if(getrlimit()==0)

// }

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

int set_environment_variable(const char*str){

    char*name = strdup(str);

    char*eq=strchr(name,'=');
    if(!eq){
        fprintf(stderr, "Invalid environment assignment: %s\n", assignment);
    }

    eq='\0';
    char*name_env_var = name;
    char*value_env_var=eq+1;
    if(setenv(name_env_var,value_env_var,1)==-1){
        perror("setenv");
        return -1;
    }
    printf("Set environment variable: %s=%s\n", name, value);

    return 0;

}

int set_core_size(const char *str){

    char *endptr;
    char*str_copy = strdop(str)
    long new_size=strtol(str_copy,&endptr,10);
    if(*endptr!='\0'){
        fprintf(stderr, "Invalid core size value: %s\n", new_size);
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

int main(int argc, char *argv[])
{

    int c;

    for(int i=argc-1; i>0; i--)
    {
        int temp_argc = 2, optind = 1;
        const char* temp_argv = {argv[0],argv[i],NULL};

        while((c = getopt(temp_argc,temp_argv,"ispucdvhU:C:V:"))!=-1)
        {

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
                // case 'u':

                //     break;
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
            }   

        }


    }

    return 0;
}