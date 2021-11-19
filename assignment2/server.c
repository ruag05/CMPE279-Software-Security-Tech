// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>
#include <pwd.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    struct passwd *pwd;
    pid_t new_pid;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char * childUser = "nobody";
    if (strcmp(argv[0], "child") == 0)
    {
        printf("\nIn child process\n");
        int duplicate_new_socket = atoi(argv[1]); 
        valread = read(duplicate_new_socket, buffer, 1024);
        printf("Read %d bytes: %s\n", valread, buffer);
        send(duplicate_new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
    }
    else{
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );
        if (bind(server_fd, (struct sockaddr *)&address,
        sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        new_pid = fork();
        if(new_pid == 0){
            int duplicate_parent_socket = dup(new_socket);
            if(duplicate_parent_socket == -1){
                perror("duplicate socket not available");
                exit(EXIT_FAILURE);
            }
            if ((pwd = getpwnam(childUser)) == NULL)
            {
                perror("Cannot find UID for nobody");
            }
            setuid(pwd->pw_uid);
            char new_args[10];
            sprintf(new_args, "%d", duplicate_parent_socket);
            char *args[] = {"child", new_args, NULL};
            execvp(argv[0], args);
        }
        else if(new_pid < 0){
            printf("\nFork failed\n");
            _exit(2);
        }
        printf("\nPrimary: Waiting for child to finish\n");
        wait(NULL);

    }
    return 0;
}