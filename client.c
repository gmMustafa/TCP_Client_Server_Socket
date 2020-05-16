#include<stdio.h>
#include<stdlib.h>
#include<errno.h> 
#include<unistd.h> 
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<string.h>
#include<stdbool.h> 
#include<ctype.h> 
#include <time.h>
#include <math.h>

#define SERVPORT 1233/* server’s well-known port */
#define SERVIP "127.0.0.1"/* server IP adress */
#define MAXDATASIZE 500 /* max number of bytes we can get at once */


/*Struct to handle message*/
struct Data {
  char message[MAXDATASIZE];
};

/*Prototypes*/
void clearString(struct Data dataToClear);
void savtoFile(char message[],char file[]);

/*main Function Start here*/
int main(int argc, char * argv[]) {
  int sockfd, numbytes;
  char buf[MAXDATASIZE]={'\0'};
  struct sockaddr_in serv_addr;

/* server’s address information */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVPORT);
serv_addr.sin_addr.s_addr = inet_addr(SERVIP);  /* interpreted by host */
bzero( & (serv_addr.sin_zero), 8);  /* zero the rest of struct */

/*Connecting the Server*/
  connect(sockfd, (struct sockaddr * ) & serv_addr, sizeof(struct sockaddr));

/*Recieving Message about Connection Details From Server*/
  numbytes = recv(sockfd, buf, MAXDATASIZE, 0);

if (numbytes != -1) //check for validity 
  printf("%s\n", buf);


struct Data data; //String taken from Client 
int counter_time=0;
bool inner_flag=false;
do{
  counter_time++;
  clearString(data);
  printf("%s \n", "Enter UserID::");
  if(counter_time!=1){
    getchar();        
  }
scanf("%[^\n]", data.message); //Asking Client to enter UseriD 
send(sockfd, data.message, strlen(data.message), 0);
char *user_id={data.message};          
//recieve respone 
char respone[MAXDATASIZE]={'\0'};
recv(sockfd, (void*) respone, (size_t) MAXDATASIZE, 0);
if(respone[0]=='1')
{
  printf("%s\n","Valid UserId");
  int inner_counter_time=0;
  do{
    inner_counter_time++;
    inner_flag=true;
    printf("%s\n","Enter Password");
    clearString(data);
    getchar();        
scanf("%[^\n]", data.message); //Asking Client to enter Password 
send(sockfd, data.message, strlen(data.message), 0);
//recieve respone 
char respone[MAXDATASIZE]={'\0'};
recv(sockfd, (void*) respone,(size_t) MAXDATASIZE, 0);
if(respone[0]=='1'){
  printf("%s\n","Valid Password");

  printf("%s\n", "Enter the name of file to be fetched from server");
  clearString(data);
  getchar();        
scanf("%[^\n]", data.message); //Asking Client to enter Password 
send(sockfd, data.message, strlen(data.message), 0);

char Message[MAXDATASIZE]={'\0'};
recv(sockfd, (void*) Message,(size_t) MAXDATASIZE, 0);
  if(Message[0]=='0'){
    printf("%s\n", "ERROR 404::FILE NOT FOUND.");
  }
  else
  {
    recv(sockfd, (void*) Message,(size_t) MAXDATASIZE, 0);
    printf("%s\n",Message);
    savtoFile(Message,user_id);
  }

close(sockfd);
exit(0);
}
else{
  printf("%s\n","Invalid Password...");
  if(inner_counter_time==3){
    printf("%s\n", "3 password tries reached \n Server Disconnect  .....");
/*Close Connection*/
    close(sockfd);
    exit(0);
  }
}
}while(1);
}  
else{
  printf("%s\n","Invalid UserID...");
  if(counter_time==3 || inner_flag){
    printf("%s\n", "3 UserID tries reached \n Server Disconnect.....");
    break;
  }
}      
}while(true);

/*Close Connection*/
close(sockfd);
return 0;
} /* main function ends */

/*Clearing the String for Continious Processing*/
void clearString(struct Data dataToClear) {
  for (int i = 0; i < MAXDATASIZE; i++) {
    dataToClear.message[i] = '\0';
  }
}


void savtoFile(char message[],char file[])
{
  srand(time(NULL));
  int r = rand(); 
  
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%d",r);


  char * path = malloc(sizeof(char) * 50);
  strcat(path, "/home/gm/Downloads/A4/file_");
  strcat(path,buffer);
  strcat(path,"_");
  strcat(path,file);
  FILE *f = fopen(path, "w");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f, "%s\n", message);
  printf("%s,%s\n", "File Downloaded at ",path);
  fclose(f);
}