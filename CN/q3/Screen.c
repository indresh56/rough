#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
struct msg
{
    char message[200];
    long type;
};
int main()
{
     key_t k=ftok("news",65);
 int msqid=msgget(k,0666|IPC_CREAT);
  struct pollfd PFD[1];
  PFD[0].fd=msqid;
  PFD[0].events=POLLIN;
  while(1)
  {
    int a=poll(PFD,1,100);
    if(a>0)
    {
        struct msg M;
        msgrcv(msqid,&M,sizeof(M.message),3,0);
        printf("%s ",M.message);
    }
  }

}