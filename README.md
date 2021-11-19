# CMPE279- Software Security Technologies

## Team Members
Ruchir Agarwal - 015237469 <br>
Jayant Prakash - 015276521

## Assignment 1 and Assignment 2
### Steps to run
#### Server terminal
* sudo gcc server.c -o server
* sudo ./server
#### Client terminal
* gcc client.c -o client
* ./client

#### *Note:
In order to run the program, removed SO_REUSEPORT from setsockopt in server.c file. <br>

##### Modified Statment
```
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
```
