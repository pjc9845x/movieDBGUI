
#include <iostream>
#include <signal.h>
#include <fstream>
#include <unistd.h>
#include <WINSOCK2.h>
#include <windows.h>
using namespace std;

/*Definitions*/
#define SEARCH_BUTTON 1
#define ABOUT_BUTTON 2
#define EXIT_BUTTON 3

/*Global Variables and Items*/
char g_szClassName[]="myWindowClass";

//Initials
SOCKET sockfd;

//Texts
HWND socketStat;
HWND titleHeader;
HWND titleSearchResultText;
HWND titleText, yearText, directorText, genreText, starText, runtimeText;

//Buttons
HWND searchButton, aboutButton, exitButton;

//Text Fields
HWND searchField;
HWND titleField, yearField, directorField, genreField, starField, rtField;

/*Functions*/
LRESULT CALLBACK WindowProcess (HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);
void establishConnectionWithServer();
void readServerMsg(int);

void registerAboutDialogClass(HINSTANCE hInst);
void displayAboutDialogClass ();

/*Implementations*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpcmd, int ncmdshow)
{
    /* Socket Setup */

    WSADATA wsaData;
    WORD versionRequired=MAKEWORD(1,1);
    WSAStartup(versionRequired, &wsaData);

    SOCKADDR_IN sAddr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    //2 - Server settings
    sAddr.sin_family=AF_INET;
    sAddr.sin_port=htons(9845);
    sAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");

    cout<<"Connecting to server..."<<endl;

    int result=connect(sockfd,(struct sockaddr*) &sAddr, sizeof(sAddr));
    while(result==-1)
    {
        result=connect(sockfd,(struct sockaddr*) &sAddr, sizeof(sAddr)==-1);
        if(result==-1)
            sleep(1);
    }

    cout<<"Server connected! Program is starting..."<<endl;
    //1 - Class Registration
    HWND hwnd;


    WNDCLASSEX wc={0};

    wc.cbSize = sizeof(WNDCLASSEX); //Size of structure
    wc.lpfnWndProc = WindowProcess; //Pointer to Window Procedure
    wc.hInstance = hInst; //Handle to app instance
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Cursor to be displayed over window
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW); //Background color
    wc.lpszClassName = g_szClassName; //Name to identify class

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //2 - Create Window
    hwnd = CreateWindowEx(
        0,   //Extended windows style
        g_szClassName,  //Create window with the class we registered.
        "Movie Database",   //Window title in the title bar
        WS_OVERLAPPEDWINDOW,    //Window style
        CW_USEDEFAULT, CW_USEDEFAULT, 750, 600, //X-ordinate origin,Y-ordinate origin, width, height
        NULL, NULL, hInst, NULL); //Parent window handle, menu handle, app instance handle, pointer to WIndow

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, ncmdshow);
    UpdateWindow(hwnd);

    //Message Looping
    MSG msg;

    while(GetMessage(&msg, NULL, 0, 0) > 0) //GetMessage() - Application Message queue
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;

}


//Procedures
LRESULT CALLBACK WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
            {
                AddControls(hwnd);break;
            }
        case WM_COMMAND:
            {
                switch(wParam)
                {
                    case SEARCH_BUTTON:
                        {
                            char query[1024], updated[1024], sMessage[1024];
                            GetWindowText(searchField, query, 1024);
                            SetWindowText(titleField, query);

                            send(sockfd, query, strlen(query), 0);
                            recv(sockfd, sMessage, sizeof(sMessage),0);

                            if(!strcmp(sMessage,"Title not found :("))
                            {
                                MessageBox(hwnd, sMessage, "Error",MB_OK);
                                memset(sMessage,0,sizeof(sMessage));
                            }
                            else if(!strcmp(sMessage,"Displaying Search Query"))
                            {
                                readServerMsg(sockfd);
                                memset(sMessage,0,sizeof(sMessage));
                            }

                            break;
                        }
                    case ABOUT_BUTTON:
                        {
                            MessageBox(NULL,"About coming soon.","MovieDatabase",MB_ICONEXCLAMATION | MB_OK);
                            break;
                        }
                    case EXIT_BUTTON:
                        {
                            send(sockfd, "Exit", strlen("Exit"), 0);
                            DestroyWindow(hwnd);
                            break;
                        }
                }
                break;
            }
        case WM_CLOSE:
            {
                send(sockfd, "Exit", strlen("Exit"), 0);
                DestroyWindow(hwnd); break;
            }
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                break;
            }
        default: return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


/* Socket Functions */
void readServerMsg(int sockfd)
{
    char title[500],year[500],director[500],stars[500],genre[500],runtime[500];

    memset(title,'\0',sizeof(title));
    memset(year,'\0',sizeof(year));
    memset(director,'\0',sizeof(director));
    memset(stars,'\0',sizeof(stars));
    memset(genre,'\0',sizeof(genre));
    memset(runtime,'\0',sizeof(runtime));

    recv(sockfd, title, sizeof(title)-1,0);
    recv(sockfd, year, sizeof(year)-1,0);
    recv(sockfd, director, sizeof(director)-1,0);
    recv(sockfd, stars, sizeof(stars)-1,0);
    recv(sockfd, genre, sizeof(genre)-1,0);
    recv(sockfd, runtime, sizeof(runtime)-1,0);

    SetWindowText(yearField,year);
    SetWindowText(directorField,director);
    SetWindowText(starField,stars);
    SetWindowText(genreField,genre);
    SetWindowText(rtField,runtime);


}

/* GUI Stuff */
void AddControls(HWND hwnd)
{

    /* ============================================
              WHEN SERVER IS CONNECTED
       ============================================ */
    titleHeader=CreateWindow("Static", "Movie Database", WS_VISIBLE|WS_CHILD, 275, 20, 200, 25, hwnd, NULL, NULL,NULL);
    CreateWindow("Static", "Search for title:", WS_VISIBLE|WS_CHILD, 100, 90, 100, 25, hwnd, NULL, NULL,NULL);
    titleSearchResultText=CreateWindow("Static", "Search Results", WS_VISIBLE|WS_CHILD, 275, 160, 200, 25, hwnd, NULL, NULL,NULL);

    titleText=CreateWindow("Static", "Title: ", WS_VISIBLE|WS_CHILD, 110, 190, 200, 25, hwnd, NULL, NULL,NULL);
    yearText=CreateWindow("Static", "Year", WS_VISIBLE|WS_CHILD, 110, 230, 60, 25, hwnd, NULL, NULL,NULL);
    directorText=CreateWindow("Static", "Director", WS_VISIBLE|WS_CHILD, 110, 260, 60, 25, hwnd, NULL, NULL,NULL);
    genreText=CreateWindow("Static", "Stars", WS_VISIBLE|WS_CHILD, 110, 290, 260, 25, hwnd, NULL, NULL,NULL);
    starText=CreateWindow("Static", "Genre", WS_VISIBLE|WS_CHILD, 110, 320, 60, 25, hwnd, NULL, NULL,NULL);
    runtimeText=CreateWindow("Static", "Runtime", WS_VISIBLE|WS_CHILD, 110, 380, 60, 25, hwnd, NULL, NULL,NULL);

//Buttons
    searchButton=CreateWindow("Button", "Search", WS_VISIBLE|WS_CHILD, 550, 90, 75, 25, hwnd, (HMENU)SEARCH_BUTTON, NULL,NULL);
    exitButton=CreateWindow("Button", "Exit", WS_VISIBLE|WS_CHILD, 275, 500, 75, 25, hwnd, (HMENU)EXIT_BUTTON, NULL,NULL);

//Text Fields
    searchField=CreateWindow("Edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER, 223, 90, 310, 25, hwnd, NULL, NULL,NULL);
    titleField=CreateWindow("Static", "", WS_VISIBLE|WS_CHILD, 223, 190, 310, 25, hwnd, NULL, NULL,NULL);
    yearField=CreateWindow("Static", "", WS_VISIBLE|WS_CHILD|WS_BORDER, 223, 230, 400, 25, hwnd, NULL, NULL,NULL);
    directorField=CreateWindow("Static", "", WS_VISIBLE|WS_CHILD|WS_BORDER, 223, 260, 400, 25, hwnd, NULL, NULL,NULL);
    genreField=CreateWindow("Static", "", WS_VISIBLE|WS_CHILD|WS_BORDER, 223, 290, 400, 25, hwnd, NULL, NULL,NULL);
    starField=CreateWindow("Static", "", WS_VISIBLE|WS_CHILD|WS_BORDER, 223, 320, 400, 50, hwnd, NULL, NULL,NULL);
    rtField=CreateWindow("Static", "", WS_VISIBLE|WS_CHILD|WS_BORDER, 223, 380, 400, 25, hwnd, NULL, NULL,NULL);
}
