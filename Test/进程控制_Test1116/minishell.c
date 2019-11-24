#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/wait.h>

int main()
{
    while(1)
    {
        printf("[username@locathost]$ ");
        fflush(stdout);
        char cmd_buf[1024]={0};
        fgets(cmd_buf,1023,stdin);
        cmd_buf[strlen(cmd_buf)-1] = '\0';
        printf("cmd:[%s]\n",cmd_buf);

        char *argv[32]={NULL};
        int argc = 0;
        char *ptr = cmd_buf;
        while(*ptr)
        {
            if(!isspace(*ptr))
            {
                argv[argc] = ptr;
                argc++;
                while(!isspace(*ptr) && *ptr != '\0')
                 {
                      ptr++;
                 }
                 *ptr = '\0';
             }
             ptr++;
         }

        int i;
        argv[argc] = NULL;
        for(i=0;i<argc;i++)
        {
            printf("argv[%d]=[%s]\n",i,argv[i]);
        }

        pid_t pid = fork();

        if(pid < 0)
        {
            continue;
        }

        else if(pid == 0)
        {
            execvp(argv[0],argv);
            exit(0);
        }

        else
        {
            wait(NULL);
        }
    }
    
    return 0;
}

