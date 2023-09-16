//edited 9.15 at 8:36pm
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>


bool NotNumber(char* str){
    if(str==NULL||*str=='\0'){
        return true;
    }
    while(*str != '\0'){
        if(!isdigit(*str)){
            return true;
        }
        str++;
    }
    return false;
}

int main() {
    // Define a pointer to a directory stream
    DIR *directory;

    // Open the directory stream using opendir()
    directory = opendir("/proc");

    if (directory) {
        // If the directory was opened successfully

        struct dirent *entry;
        //char buffer[1024];
        char path[1024];
        //char pid[1024];
        // Read and process each entry in the directory
        printf("  PID CMD\n");
        while ((entry = readdir(directory))) {
            // char *exec_argv[] = {"ls", NULL};
            // char *exec_envp[] = {NULL};
            // int exec_return = execve("/usr/bin/ls", exec_argv, exec_envp);
            // if (exec_return == -1) {
            //     exec_return = errno;
            //     perror("execve failed");
            //     return exec_return;
            // }         
            char buffer[1024] = {'\0'};
 
            if(NotNumber(entry->d_name)){
                continue;
            }
            snprintf(path, sizeof(path),"/proc/%s/status",entry->d_name);
            //printf("%s\n",path);
            int fd = open(path,O_RDONLY);
            if(fd==-1){
                perror("open");
                return 1;
            }
            int name = read(fd, buffer, sizeof(buffer) );
            if(name == -1){
                perror("read");
                return 1;
            }
            int i =6;
            int j=0;
            char Pname[1024]={'\0'};
            while(buffer[i] != '\n' && i<sizeof(buffer)){
                Pname[j] = buffer[i];
                i++;
                j++;
            }
            int length = 5;
            int diff = length - strlen(entry->d_name);
            if(diff>0){
                for(int count=0;count<diff;count++){
                    putchar(' ');
                }
            }
            printf("%s %s\n",entry->d_name,Pname);


            //printf("Name: %s\n", entry->d_name);
            close(fd);
        }

        // Close the directory stream when you're done
        closedir(directory);
    } else {
        // If the directory couldn't be opened, handle the error
        perror("opendir");
    }

    return 0;
}


    //eturn 0;
    
//}
