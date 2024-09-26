#ifndef MYSERVER_H
#define MYSERVER_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include <mutex>
#include <fstream>
#include <vector>
#include <iomanip>


using namespace std;


pthread_mutex_t mtx_;

ofstream outFile_;

void Write (const string& message) {
    pthread_mutex_lock(&mtx_);
    if (!outFile_.is_open()) {
        cout << "Открытие файла " << endl;

        outFile_.open("log.txt", ios_base::app);
    }
    cout << message << "\n";
    outFile_ << message<< endl;
    pthread_mutex_unlock(&mtx_);
}

//------------------------------------


class MyTCPServer {

    int socket_;
    char buffer[1024];

    void Read(int clientSocket, const thread::id idThread) {
        int n = read(clientSocket, buffer, sizeof(buffer));
        if (n == -1) {
            perror("read") ;
            close(clientSocket);
            return;
        }

        close(clientSocket);
        stringstream ss;
        ss << idThread;

        string data_(buffer, n);
        data_ += "\t" + ss.str();
        Write(data_.c_str());
    }



public:
    explicit MyTCPServer(uint port) {
        socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_ == -1) {
            perror("socket_ :");
            exit(-1);
        }


        sockaddr_in addr_;
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port);
        addr_.sin_addr.s_addr = INADDR_ANY;

        if (bind(socket_, (sockaddr*)&addr_, sizeof (addr_)) == -1) {
            perror("bind");
            exit(-1);
        }

        if (listen(socket_, 5) == -1) {
            perror("listen");
            exit(-1);
        }
    }

    void start() {
        cout << "Server STARTED " << endl;
        while (true) {
            sockaddr_in clientAddr_;
            socklen_t clientAddr_len = sizeof (clientAddr_);
            int clientSocket_ = accept(socket_, (sockaddr*)& clientAddr_, &clientAddr_len);
            if (clientSocket_ == -1) {
                perror("accept");
                continue;
            }

         //   cout << "Server NEW conNECTION " << clientSocket_ << endl;
            thread clientThread_;
            clientThread_ = thread([this, clientSocket_] {
                Read(clientSocket_,  this_thread::get_id());
            });
            clientThread_.detach();
        }
    }
};

#endif // MYSERVER_H

