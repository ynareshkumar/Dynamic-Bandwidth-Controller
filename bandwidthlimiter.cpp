#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>	
#include <sys/time.h>
#include <string>
#include <netinet/tcp.h>

using namespace std;

#define FILE_NOT_FOUND "Connection refused" 
#define NO_RELATIVE_PATH "No relative path"
#define BUFFER_SIZE 32
#define RECV_BUF_SIZE 32 //34 for icone 37 ifdata welcome 37 camera
#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */

std::string getservername(std::string url);
std::string getrelativepath(std::string url);
void callget(std::string receivedurl,std::string relativepath,int httpserversd,string hostname,int* ret);

string hash_str(const char* s)
{
  char buffer[50];
  unsigned h = 31 /* also prime */;
  while (*s) {
    h = (h * A) ^ (s[0] * B);
    s++;
  }
  sprintf (buffer, "%u", h);
  std::string a = buffer;
  cout << a << endl;
  return a; // or return h % C;
}

int nooffiles = 0;
int limitrate;

int* callHead(char *url){
 
 int port,sd,bytestoread,numofbytes;//sd is the socket descriptor of the client,numofbytes is the number of bytes read.
 int fdmax,i,j=1,k,yes=1,selectresult,nread;//fdmax is the maximum number of connected clients,i,j,k are temp variables 
//host is the ip address of server,buffer is a pointer to the string buffer,sendbuf is buffer which stores the value to be sent to server
 char *host,sendbuf[512],receivebuf[512];
 fd_set master;//master socket descriptor
 fd_set read_fds;//temporary socket descriptor
 int sentbytes,recbytes,pos;
 struct sockaddr_in serveraddress;//Contains server details like port and IP address
 struct in_addr **ipaddrlist;
 struct hostent *hp,*test;//gethostbyname()
 //char * url;
 char * urlip;
 FILE *pFile;
 char sendbufHead[100000];
 int sendresult;
 int receiveresult;
 std:string hostHead = "Host: ";
 std::string urlstring,relativepath,servername;
 


   port = 80;

  //Create socket
  sd = socket(AF_INET,SOCK_STREAM,0);

 //Check for failure for creating socket
 if(sd == -1)
   {
      cout<<"\n Unable to create socket\n";
      exit(1);
   }

   setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

//set as 0 initially
 memset(&serveraddress,0,sizeof(serveraddress));
 serveraddress.sin_family = AF_INET;
 serveraddress.sin_port = htons(port);

   urlstring = url;
   pos = urlstring.find("http://");
   if(pos != string::npos)
   {
     urlstring=urlstring.substr(pos+7);
   //  url = urlstring;
   }
   servername = getservername(urlstring);
   cout<<"\n Server Name is:"<<servername;
   hp = gethostbyname(servername.c_str());
   ipaddrlist = (struct in_addr **)hp->h_addr_list;
    if( *ipaddrlist != NULL )
    {
     urlip = inet_ntoa(**ipaddrlist);
    cout<<"\n URL IP add is "<<urlip<<endl;
   }

hp = gethostbyname(urlip);
 
 if(hp == NULL)
 {
    cout<<"\n Unable to get host address\n";
   exit(1);
 }
//Copy host address to server address
 bcopy(hp->h_addr,(char *)&serveraddress.sin_addr,hp->h_length);



//Establish connection to server
if(connect(sd,(struct sockaddr *)&serveraddress,sizeof(serveraddress)) == -1)
 {
   cout<<"\n Cannot connect to server\n";
   exit(1);
 }

   cout<<"\n Connected to server whose address is "<<hp->h_name<<"\n";   

   relativepath = getrelativepath(urlstring);

   cout<<"\n Relative path is:"<<relativepath<<endl;
   
   
   strcpy(sendbuf,urlip);
   strcat(sendbuf,relativepath.c_str());

strcpy(sendbufHead,"HEAD ");
strcat(sendbufHead,relativepath.c_str());
strcat(sendbufHead," HTTP/1.1\n");
//host.append(servername);
hostHead.append(servername);
strcat(sendbufHead,hostHead.c_str());
strcat(sendbufHead,"\n\n");
cout << "HEAD REQUEST " << endl;
cout << sendbufHead ;
sendresult = send(sd,sendbufHead,sizeof(sendbufHead),0);
bzero(sendbufHead,100000);
receiveresult = recv(sd,sendbufHead,100000,0);
if(receiveresult > 0){
cout << "HEAD size " << strlen(sendbufHead) << endl;
cout << sendbufHead;
}
string clength = std::string(sendbufHead);
std::size_t found = clength.find("Content-Length: ");
char clen[15];
int k1=0;
if (found!=std::string::npos){
for (unsigned i=found + 16; clength.at(i) != '\n'; ++i)
       {
clen[k1] = clength.at(i);
k1++;
}
clen[k1] = '\0';
}
int cleni = atoi(clen);
close(sd);
int *ret = new int[2];
ret[0] = strlen(sendbufHead);
ret[1] = cleni;
return ret;

}
int main(int argc,char *argv[])
{
 
 int port,sd,bytestoread,numofbytes;//sd is the socket descriptor of the client,numofbytes is the number of bytes read.
 int fdmax,i,j=1,k,yes=1,selectresult,nread;//fdmax is the maximum number of connected clients,i,j,k are temp variables 
//host is the ip address of server,buffer is a pointer to the string buffer,sendbuf is buffer which stores the value to be sent to server
 char *host,sendbuf[512],receivebuf[512];
 fd_set master;//master socket descriptor
 fd_set read_fds;//temporary socket descriptor
 int sentbytes,recbytes,pos;
 struct sockaddr_in serveraddress;//Contains server details like port and IP address
 struct in_addr **ipaddrlist;
 struct hostent *hp,*test;//gethostbyname()
 char * url;
 char * urlip;
 FILE *pFile;
 //int ret[2];
 std::string urlstring,relativepath,servername;
 
 
//Get command line argumentvalues for username, server IP and port
 if(argc == 3)
   {      
     url = argv[1];  
     limitrate = atoi(argv[2]);    
   }
 else//Force exit
   {
     cout<<"\n The number of arguments entered is "<<argc-1;
     cout<<"\n Please enter the argument in ./main URL format \n";
     exit(1); 
   }
int *ret = callHead(url);
   port = 80;

  //Create socket
  sd = socket(AF_INET,SOCK_STREAM,0);

 //Check for failure for creating socket
 if(sd == -1)
   {
      cout<<"\n Unable to create socket\n";
      exit(1);
   }
   int p = 32;
   setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
   //setsockopt(sd,SOL_SOCKET,SO_RCVBUF,&p,sizeof(int));

//set as 0 initially
 memset(&serveraddress,0,sizeof(serveraddress));
 serveraddress.sin_family = AF_INET;
 serveraddress.sin_port = htons(port);

   urlstring = url;
   pos = urlstring.find("http://");
   if(pos != string::npos)
   {
     urlstring=urlstring.substr(pos+7);
   //  url = urlstring;
   }
   servername = getservername(urlstring);
   cout<<"Server Name is:"<<servername;
   hp = gethostbyname(servername.c_str());
   ipaddrlist = (struct in_addr **)hp->h_addr_list;
    if( *ipaddrlist != NULL )
    {
     urlip = inet_ntoa(**ipaddrlist);
    cout<<"\n URL IP add is "<<urlip<<endl;
   }

hp = gethostbyname(urlip);
 
 if(hp == NULL)
 {
    cout<<"\n Unable to get host address\n";
   exit(1);
 }
//Copy host address to server address
 bcopy(hp->h_addr,(char *)&serveraddress.sin_addr,hp->h_length);



//Establish connection to server
if(connect(sd,(struct sockaddr *)&serveraddress,sizeof(serveraddress)) == -1)
 {
   cout<<"\n Cannot connect to server\n";
   exit(1);
 }

   cout<<"\n Connected to server whose address is "<<hp->h_name<<"\n";   

   relativepath = getrelativepath(urlstring);

   cout<<"\n Relative path is:"<<relativepath<<endl;
   
   
   strcpy(sendbuf,urlip);
   strcat(sendbuf,relativepath.c_str());
//callhead(sendbuf,relativepath,sd,servername);
   callget(sendbuf,relativepath,sd,servername,ret);

return 0;
}

std::string getrelativepath(std::string url)
{
 std::string relativepath = "/";
 int pos;
 pos = url.find("/");
 if(pos != string::npos)
   {
     relativepath.append(url.substr(pos+1));  
   }
 else
 {
   relativepath = "index.html";
 }
 return relativepath;
}

std::string getservername(std::string url)
{
std::string servername;
 int pos;
 pos = url.find("/");
 if(pos != string::npos)
   {
   	servername = url.substr(0,pos);
   }	
 else
 {
 	 servername = url;
 }

   return servername;
}

void callget(std::string receivedurl,std::string relativepath,int httpserversd, string servername, int* ret)
{
std::string getcommand;
std::string getdata;
std::string filename;
std:string host = "Host: ";
FILE *fptr,*serverfile;
char buffer[RECV_BUF_SIZE];	
char sendbuf[100000];
char headbuf[1000];
char fname[BUFFER_SIZE];
int sendresult,receiveresult;
double sizeoffile = 0;
fd_set master;//master socket descriptor
fd_set read_fds;//temporary socket descriptor
struct timeval starttime,endtime,inststarttime,instendtime,actualinsttime;
double downloadrate,timetodownload,instrate,expectedtime,delay;
 

strcpy(sendbuf,"GET ");
strcat(sendbuf,relativepath.c_str());
strcat(sendbuf," HTTP/1.1\r\n");
host.append(servername);
strcat(sendbuf,host.c_str());
strcat(sendbuf,"\r\n\r\n");	
filename = hash_str(relativepath.c_str());
strcpy(fname,filename.c_str());
cout<<"\nFile written to is "<<filename<<endl;
serverfile = fopen(filename.c_str(),"wb");
cout << "\nRequest ... " << endl;
cout << sendbuf;
cout << endl << "Request end" << endl;

//Send GET to HTTP Server



sendresult = send(httpserversd,sendbuf,sizeof(sendbuf),0);
cout << endl;
if(sendresult <= 0)
{
cout<<"\n Error sending to HTTP server.";
}else{
perror("send");
}
bzero(sendbuf,100000);
bzero(buffer,strlen(buffer));
//Receive from HTTP server
//gettimeofday(&starttime, NULL);
FD_SET(httpserversd, &master);
int i = 0;
 gettimeofday(&starttime, NULL);
for(;;){
//cout << endl;
//cout << "?" << endl;
           read_fds = master;
           if(select(httpserversd+1, &read_fds, NULL, NULL, NULL) == -1){
fclose(serverfile);
close(httpserversd);
               exit(4);
           }
           if(FD_ISSET(httpserversd, &read_fds)){
               //gettimeofday(&inststarttime, NULL);	
bzero(buffer,strlen(buffer));
//cout << strlen(buffer) << endl;
       gettimeofday(&inststarttime, NULL); 
receiveresult = recv(httpserversd,buffer,RECV_BUF_SIZE,MSG_WAITALL);
//cout << endl;
//cout << receiveresult << endl;
//cout << endl << "Buf length " << strlen(buffer) << " " <<receiveresult;
if(receiveresult > 0)
{
//cout << receiveresult; //<< endl << buffer << endl;	
if(i == 0){
//cout << "i = 0";
string headString = std::string(buffer);
//cout << "HEAD STRING : " << headString;
//cout << endl;
int pos = 0;
pos = headString.find("\r\n\r\n");
//cout << "POS " << pos;
if(pos != string::npos)
{	
 headString=headString.substr(pos+4);
 bzero(buffer,strlen(buffer));
 strcpy(buffer,headString.c_str());
 //cout << "BUFF LEN " << strlen(buffer);
 receiveresult = strlen(buffer);
 i++;
}else{
receiveresult = 0;
}	

}
if(receiveresult > 0){
           gettimeofday(&instendtime, NULL);
//cout << endl << "End time " << instendtime.tv_usec << " Start time " << inststarttime.tv_usec << endl;
           timetodownload = ((instendtime.tv_sec * 1000000 + instendtime.tv_usec) - (inststarttime.tv_sec * 1000000 + inststarttime.tv_usec));
         //cout<<"Current download time "<<timetodownload<<endl; 
         //cout<<"\n Receiv Result "<<receiveresult<<" limit rate "<<limitrate<<endl;
         expectedtime = (double)receiveresult * 8/limitrate;
         expectedtime = expectedtime * 1000;
         //cout<<"Expected time "<<expectedtime<<endl;
         delay =  expectedtime - timetodownload;
        // cout<<"\n Delay is "<<delay;
         //instrate = receiveresult / (timetodownload *  1000);
         if(delay > 0)
         {
           usleep(delay);  
         }

//cout << buffer;
int p = sizeoffile + receiveresult - ret[1];
if(p > 0){
p = receiveresult - p;
}else{
p = receiveresult;
}	
fwrite (buffer , sizeof(char), p, serverfile);
sizeoffile += p;
           gettimeofday(&actualinsttime, NULL);
         timetodownload = ((actualinsttime.tv_sec * 1000000 + actualinsttime.tv_usec) - (inststarttime.tv_sec * 1000000 + inststarttime.tv_usec));
         if(timetodownload > 0 && delay > 0)
         {
           instrate = receiveresult * 8 * 1000 / (timetodownload);
           cout<<"\nCurrent rate is "<<instrate<<"Kbps"<<endl;
           gettimeofday(&endtime, NULL);
         }
if(sizeoffile >= ret[1]){
             gettimeofday(&endtime, NULL);
cout << "END";
break;
}	
}
}
else{
         gettimeofday(&endtime, NULL);
perror("recv");
fclose(serverfile);
close(httpserversd);
break;
}
           }
   }	
   cout << "Size " << sizeoffile <<endl;

   timetodownload = ((endtime.tv_sec * 1000000 + endtime.tv_usec) - (starttime.tv_sec * 1000000 + starttime.tv_usec));
   cout<<"\n Time taken for download is "<<timetodownload/1000000<<"s"<<endl;

   cout<<"\n Size of received file is "<<sizeoffile<<endl;

   downloadrate = sizeoffile * 8 * 1000 / (timetodownload);
   cout<<"\n Average Download rate is "<<downloadrate<<"Kbps"<<endl;     
   
}
