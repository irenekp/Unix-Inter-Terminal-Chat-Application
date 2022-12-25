#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
char *str,name[25];
int shmid;
struct proc
{
	int pid;
	char user[25];
};
struct proc p;

void my_handler(int a)
{
 	if(str[0] == 0||str[0]=='\n')
		 return;
 	if(str[0] == '*')
 	{
		 printf("\n%s has exited.\n",p.user);
		 shmdt(str);
 		 shmctl(shmid,IPC_RMID,NULL);
 		 exit(0);
 	}
 	printf("\n%s:",p.user);
 	fputs(str,stdout);
	 
}

void send(int signum)
{
   	 memset(str,0,1024);
   	 printf("\nYou>");
    	fflush(stdin);
    	fgets(str,100,stdin);
  	 if(str[0]== '*')
    	{
  		 printf("You left\n");
  		 kill(p.pid,SIGUSR1);
  		 exit(0);  		 
     	}  
    	kill(p.pid,SIGUSR1);
}

int main()
{
	int fd;
	signal(SIGUSR1,my_handler);
	signal(SIGINT,send);
	shmid = shmget((key_t)4000,1024,0666|IPC_CREAT);
	if(shmid<0)
	{
		 printf("Shmget error!!\n");
   	 	 exit(0);
	 }
	 str= (char*) shmat(shmid,(void*)0,0);
	 if(str==NULL)
	{
   	 	printf("Shmat error!!\n");
   	 	exit(0);
	 }   
	p.pid = getpid();
	printf("\nWelcome,enter your name: ");
	scanf("%s",name);
	strcpy(p.user,name);
	fflush(stdin);
	fd = open("process1.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
	write(fd,&p,sizeof(p));
	close(fd);
	sleep(5);
	fd = open("process2.txt",O_RDONLY);
	read(fd,&p,sizeof(p));
	close(fd);
	memset(str,0,1024);
	printf("\nPress Ctrl+C to type a message,type * to exit");
	raise(SIGINT);
	while(1)
	{ }
	return 0;
}
