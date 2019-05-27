#include <iostream>
#include <fstream>
#include <winsock.h>
#include "Movie.h"
#include "movieDBFunction.cpp"
#include <map>
#include <sstream>
using namespace std;

map <string, Movie> movieList;

void sendData(int, Movie);

int main()
{
    //Movie Data
    movieList=readData(movieList);
    //Test

    //Initials
    WSADATA wsaData;
    WORD versionRequired=MAKEWORD(1,1);
    WSAStartup(versionRequired, &wsaData);

    SOCKET sockfd;
    SOCKADDR_IN sAddr, cAddr;

    //1 - Socket Initials
    int serverFd;

    //2 - Socket settings

    serverFd=socket(AF_INET,SOCK_STREAM,0);
    sAddr.sin_family=AF_INET;
    sAddr.sin_port=htons(9845);
    sAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //3 - Bind
    bind(serverFd,(SOCKADDR*) &sAddr, sizeof(sAddr));

    //4 - Listen

    if(listen(serverFd,1)==-1)
    {
        cout<<"Error listening!"<<endl;
        return 0;
    }

    //5 - Begin
    char sMessage[1024],cMessage[1024];
    const char *welcome="Welcome!";
    memset(cMessage,0,sizeof(cMessage));

    int sLen=sizeof(sAddr);
    int cLen=sizeof(cAddr);

    cout<<"Waiting for connection..."<<endl;

    int clientFd=accept(serverFd, (struct sockaddr *)&cAddr, &cLen);
    if(clientFd==-1)
    {
        cout<<"Error accepting client"<<endl;
        return 0;
    }

    cout<<"Client connected!"<<endl;
    cout<<"-------------------------------------------"<<endl;
    bool cont=true;

    while(cont)
    {
        recv(clientFd, cMessage, sizeof(cMessage),0);
        cout<<"Client: "<<cMessage<<endl;


        if(!strcmp(cMessage,"Exit"))
        {
            send(clientFd, "Goodbye!", strlen("Goodbye!"), 0);
            cout<<"Client disconnected."<<endl;
            cont=false;
            memset(cMessage,0,sizeof(cMessage));
        }
        else
        {

            map<string, Movie>::iterator it;
            it=movieList.find(string(cMessage));

            if(it==movieList.end())
                send(clientFd, "Title not found :(", strlen("Title not found :("), 0);
            else
            {
                Movie searchResult=it->second;
                send(clientFd, "Displaying Search Query", strlen("Match found for result: "), 0);
                sendData(clientFd, searchResult);
            }
            memset(cMessage,0,sizeof(cMessage));
        }
    cout<<"-------------------------------------------"<<endl;
    }

    closesocket(sockfd);
    WSACleanup();
}

void sendData(int clientFd, Movie movie)
{
    char title[(movie.getTitle()).length()+1],year[4],director[(movie.getDirector()).length()+1];
    char stars[(movie.getStars()).length()+1],genre[(movie.getGenre()).length()+1],runtime[4];

    strcpy(title, (movie.getTitle()).c_str());
    itoa(movie.getYear(),year,10);
    strcpy(director,(movie.getDirector()).c_str());
    strcpy(stars,(movie.getStars()).c_str());
    strcpy(genre, (movie.getGenre()).c_str());
    itoa(movie.getRuntime(),runtime,10);

    cout<<"Title: "<<title<<"\n"<<endl;
    cout<<"Year: "<<year<<endl;
    cout<<"Director: "<<director<<endl;
    cout<<"Actors: "<<stars<<endl;
    cout<<"Genre: "<<genre<<endl;
    cout<<"Runtime: "<<runtime<<endl;

    send(clientFd, title, strlen(title), 0);
    send(clientFd, year, strlen(year), 0);
    send(clientFd, director, strlen(director), 0);
    send(clientFd, stars, strlen(stars), 0);
    send(clientFd, genre, strlen(genre), 0);
    send(clientFd, runtime, strlen(runtime), 0);

}
