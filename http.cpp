#pragma warning(disable : 4996) 
#pragma comment(lib, "Ws2_32.lib")
#include<iostream>
#include<winsock2.h>
#include<string>
#include <ws2tcpip.h>
using namespace std;

int main(){
    cout<<"Creating a web server......"<<endl;
    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct  sockaddr_in server;
    int server_len;
    int BUFFER_SIZE = 30720;

    //Successfully initialized the server 
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        cout<<"Could not initialize the server......"<<endl;
    }

    //Creating the web socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(wsocket == INVALID_SOCKET){
        cout<<"Could not create a web socket......"<<endl;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(3000);
    server_len = sizeof(server);

    //binding the sockets
    if(bind(wsocket, (SOCKADDR *)&server, server_len) != 0){
        cout<<"Could not bind the socket......"<<endl;
    }

    //listening the sockets
    if(listen(wsocket, 20) != 0){
        cout<<"Could not find the port to listen....."<<endl;
    }
    cout<<"Server is listening on port......"<<endl;

    int bytes;
    while(true){
        //accept client request 
        new_wsocket = accept(wsocket, (SOCKADDR *)&server, &server_len);
        if(new_wsocket == INVALID_SOCKET){
            cout<<"Could not accept client request......"<<endl;
        }

        //reading client request
        char buff[37020] = {0};
        bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
        if(bytes < 0){
            cout<<"Could not read the client data...."<<endl;
        }

        string statusMessage = "HTTP/1.1 200 OK\n Content type : html/text \n Content-lenght";
        string response = "<html><h1>Hello World</h1></html>";
        statusMessage.append(to_string(response.size()));
        statusMessage.append("\n\n");
        statusMessage.append(response);


        int bytesSend = 0;
        int totalBytesSend = 0;
        //checking how many bites have been send to the client
        while(totalBytesSend < statusMessage.size()){
            bytesSend = send(new_wsocket, statusMessage.c_str(), statusMessage.size(), 0);

            //if no bits are send then something i went wrong with the server
            if(bytesSend < 0){
                cout<<"Something went wrong....."<<endl;
            }
            totalBytesSend += bytesSend;
        }
        cout<<"Response is sent to the client....."<<endl;

        //The socket which was created need to be closed or to be terminated
        closesocket(new_wsocket);
    }

    closesocket(wsocket);
    WSACleanup();    
    return 0;
}