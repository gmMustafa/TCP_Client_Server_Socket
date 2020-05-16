#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


#define PORT 1233 /* my well-known port */
#define BACKLOG 10 /* length of pending connections queue */
#define MAXDATASIZE 1000 /* max number of bytes we can get at once */

char * ChangeStringToInt(int a);
void ReverseString(char * a);
bool checkUserId(char c[],char match[]);
bool checkPassword(char match[]);


char password[50];


int main(void)
{
	int sockfd, new_fd;
/* listen on sockfd, new connection*/
/* on new_fd */
struct sockaddr_in my_addr; /* server address */
	struct sockaddr_in their_addr;
/* client addr */
	int sin_size;
/* address size */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
/* host byte order */
/* convert to short,
network byte order */
	my_addr.sin_addr.s_addr = (INADDR_ANY);
/* automatically fill with my IP */
bzero(&(my_addr.sin_zero), 8); /* zero the rest of the struct */



	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1)
	{
		perror("bind");
		exit(1);
	}	

	if (listen(sockfd, BACKLOG) == -1) {
		perror ("listen"); exit (1);
	}

	while(1) 
	{
/* main accept() loop ; always true*/
		sin_size = sizeof (struct sockaddr_in);
		if ((new_fd = accept (sockfd, (struct sockaddr*) &their_addr,
			&sin_size)) == -1) {
			perror ("accept");
		continue;
	}

/*printf("server: got connection from %s\n",inet_ntoa (their_addr.sin_addr));*/
	if (!fork()) 
	{
		close (sockfd);
/* this is the child process */
/* child doesn't need it */
/*Printing details for the new Client*/
		char *ip = inet_ntoa(their_addr.sin_addr);
		int port = htons(their_addr.sin_port);

		printf("Client Connectd <%s> <%d> \n",ip,port);

/*Concating message back to Client*/	    
		char * sendMessageBack = malloc(sizeof(char) * 50);
		strcat(sendMessageBack, "welcome! ");
		strcat(sendMessageBack, inet_ntoa(their_addr.sin_addr));
		strcat(sendMessageBack, " ");

/*message send::   Welcom <ip> <port>*/
		char * temp = ChangeStringToInt(port);
		ReverseString(temp);
		strcat(sendMessageBack, temp);

/*Sending message back to Client*/
		if (send(new_fd, sendMessageBack, strlen(sendMessageBack), 0) == -1){
			perror ("send");
			close (new_fd);
			exit(0);
		}

		int turn_count=0;
		do{
			turn_count++;
/*Recieving message from client*/
			char u_id[MAXDATASIZE]={'\0'};
			recv(new_fd, (void*) u_id, (size_t) MAXDATASIZE, 0);
			printf("User ID Recieved %s\n", u_id);

			FILE *fptr;
			char c[100];
			if ((fptr = fopen("/home/gm/Downloads/A4/Server/database.txt","r")) == NULL){
				printf("Error! opening file");
				exit(1);
			}

			int i=0;
			bool flag_idValid=false;
			while(1)
				{	i++;
					if( fgets (c, 100, fptr)==NULL ) {break;}
					else if(strlen(c)==1){break;}	
					else
					{
//printf("rec %d:\n%s",i, c);
						if(checkUserId(c,u_id)){

//send valid
							flag_idValid=true;
							send(new_fd, "1", 1, 0);

							int inner_turn_count=0;
							do{
								inner_turn_count++;
								char pass[MAXDATASIZE]={'\0'};
								recv(new_fd, (void*) pass, (size_t) MAXDATASIZE, 0);
								printf("Password Recieved %s\n", pass);
//recieve password
								if(checkPassword(pass)) 
								{
// printf("%s\n","valid Password");
									send(new_fd, "1", 1, 0);

									char file[MAXDATASIZE]={'\0'};
									recv(new_fd, (void*) file, (size_t) MAXDATASIZE, 0);
// printf("File Name Recieved %s\n", file);

									FILE *clinetFileptr;
									char c[100];
									char * path = malloc(sizeof(char) * 50);
									strcat(path, "/home/gm/Downloads/A4/Server/");
									strcat(path,file);
									if ((clinetFileptr = fopen(path,"r")) == NULL){
										send(new_fd, "0", 5, 0);
									}
									else{
										send(new_fd, "1", 5, 0);
										printf("%s : %s \n", "File Found at path",path);
										char * main_string= malloc(sizeof(char) * MAXDATASIZE);
										char * message= malloc(sizeof(char) * MAXDATASIZE);  
										while(1)
										{
											if( fgets (message, MAXDATASIZE, clinetFileptr)==NULL ) {break;}
											else if(strlen(message)==1){break;}
											else{
												strcat(main_string,message);
											}	
										}
										send(new_fd,main_string,strlen(main_string),0);
									}
									break;
								}
								else
								{
// printf("%s\n","Invalid Password");
									send(new_fd, "0", 1, 0); 
									if(inner_turn_count==3)
									{
//connection terminated after 3 tries
										break;	
									}	
								}

							}while(1); 	
							break;		
						}
					}	
				}

				if(!flag_idValid)
				{
//send invalid id
					send(new_fd, "0", 1, 0);
					fclose(fptr); 
					if(turn_count==3)
					{
//connection terminated after 3 tries
						break;	
					}
				}

				if(flag_idValid){break;}
			}while(1);

close (new_fd);/* parent doesn't need this */
			break;
		}

} /* outer while loop ends here */
} /* main function ends here */


/*Function to Reverse Port Num orignal integer value*/
		void ReverseString(char * a) {
			char temp = '\0';
			int j = strlen(a) - 1;
			for (int i = 0; i < strlen(a) / 2; i++, j--) {
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}

/*Function to change Port Num to integer value*/
		char * ChangeStringToInt(int a) {
			char * arr = malloc(50 * sizeof(char));
			int temp;
			while (a != 0) {
				temp = a % 10;
				switch (temp) {
					case 0:
					strcat(arr, "0");
					break;
					case 1:
					strcat(arr, "1");
					break;
					case 2:
					strcat(arr, "2");
					break;
					case 3:
					strcat(arr, "3");
					break;
					case 4:
					strcat(arr, "4");
					break;
					case 5:
					strcat(arr, "5");
					break;
					case 6:
					strcat(arr, "6");
					break;
					case 7:
					strcat(arr, "7");
					break;
					case 8:
					strcat(arr, "8");
					break;
					case 9:
					strcat(arr, "9");
					break;
					default:
					break;
				}
				a = a / 10;
			}
			return arr;
		}



		bool checkUserId(char c[],char match[])
		{for(int i=0;i<strlen(c);i++){
			if(c[i]==','){
				int j=0;
				for(int k=i+1;k<strlen(c)-1;k++){
					password[j]=c[k];j++;				
				}
				break;
			}		
			if(match[i]!=c[i]){return false;}
		}return true;
	}



	bool checkPassword(char match[])
	{
		for(int i=0;i<strlen(password);i++){
			if(password[i]!=match[i])
				{return false;}
		}
		return true;
	}