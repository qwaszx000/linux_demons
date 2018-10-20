#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int port = 2019;
char *ipaddr = "51.75.64.198";

int main()
{
	pid_t pid, sid;
	pid = fork();
	FILE* f;
	//Fail
	if(pid<0){
		exit(EXIT_FAILURE);
	}
	//Success fork
	if(pid>0){
		exit(EXIT_SUCCESS);
	}
	//Set file mode mask
	umask(0);
	sid = setsid();
	if(sid<0){
		exit(EXIT_FAILURE);
	}
	/*Do smth*/
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	f = fopen("demon_inform.log","a+");
        fprintf(f, "SID:%u\nPID:%u\nPort:%d\nIP address:%s\n", sid, pid, port, ipaddr);
        fflush(f);
        //fclose(f);

	//Socket vars
	struct sockaddr_in in;
	in.sin_family = AF_INET;
	in.sin_port = htons(port);
	in.sin_addr.s_addr = inet_addr(ipaddr);
	while(1)
	{
		int s = socket(AF_INET, SOCK_STREAM, 0);
		/*
		Connect to 51.75.64.198:2019
		If cant - use command */
		char *start_command = /*screen*/ "DreamDaemon tgstation.dmb 2019 -trusted -public";
		if(connect(s, (struct sockaddr*)&in, sizeof(in))!=0)
		{
			fprintf(f,"Cant connect to server.\nUsing command %s\n", start_command);
			fflush(f);
			if(system((const char*)start_command)==-1)
			{
				fprintf(f,"Error while uing command!\n\n");
				fflush(f);
			}
		}
		close(s);
		sleep(120);//Wait 2 minutes
	}
fclose(f);
exit(EXIT_SUCCESS);
}
