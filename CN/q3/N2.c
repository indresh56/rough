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
#include<signal.h>
int livetelecast=0;
void handle1(int sig)
{
    if(livetelecast==0) livetelecast=1;
    else livetelecast=0;
}
struct msg
{
    char message[200];
    long type;
};
int main()
{
 signal(SIGUSR1,handle1);
 key_t k=ftok("news",65);
 int msqid=msgget(k,0666|IPC_CREAT);

 while(1)
 {
    while(livetelecast==1);
    struct msg M;
    msgrcv(msqid,&M,sizeof(M.message),2,0);
    if(M.message[0]>='0'&&M.message[0]<='9'&&M.message[1]>='0'
    &&M.message[1]<='9'&&M.message[2]>='0'&&M.message[2]<='9'
    &&M.message[3]>='0'&&M.message[3]<='9')
    {
    
    int sfd = socket(AF_INET, SOCK_STREAM, 0); // 0 given for default
    struct sockaddr_in serveaddr;
    char port[4];
    for(int i=0;i<4;i++) port[i]=M.message[i];

    serveaddr.sin_family = AF_INET;
    serveaddr.sin_port = htons(8080);//stoi(port)
    serveaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sfd, (struct sockaddr *)&serveaddr, sizeof(serveaddr));

    char buffer[128];
    int pid;
    FILE *fp= popen("pgrep N1", "r");
    fgets(buffer, sizeof(buffer), fp);
    pid = atoi(buffer); 
    kill(pid,SIGUSR1);
    livetelecast=1;
     char buff[200];
    int n = recv(sfd, buff, sizeof(buff), 0);
    buff[n] = '\0';
    struct msg Temp;
    Temp.type=3;
    //Temp.message=buff;
    strcpy(Temp.message,buff);
    msgsnd(msqid,&Temp,sizeof(Temp.message),0);
    livetelecast=0;
    // fp= popen("pgrep N1", "r");
    // fgets(buffer, sizeof(buff), fp);
    // pid = atoi(buffer); 
    kill(pid,SIGUSR1);
    }
    else{
         struct msg Temp;
    Temp.type=3;
    strcpy(Temp.message,M.message);
    //Temp.message=M.message;
    msgsnd(msqid,&Temp,sizeof(Temp.message),0);
    }

 }
}