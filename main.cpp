#include "my_server.h"
#include <signal.h>

using namespace std;



void signalHandler(int sigNum) {
    if (sigNum == 20 ) {
        cout << " " << sigNum << " Stop server" << endl;
        exit(0);
    }
    if (sigNum == 2 ) {
        return;
    }
}

int main(int argc, const char* argv[])
{
    if (argc == 1) {
        cout << "need to input port number" << endl;
        exit(-1);
    }
    if (argc > 2) {
        cout << "too many parametrs" << endl;
        exit(-1);
    }
    if (std::to_string(atoi(argv[1])) != argv[1]) {
        cout << "not number " << argv[1] << endl;
        exit(-1);
    }
    if (atoi(argv[1]) < 1024 || atoi(argv[1]) > 65000 ) {
        cout << "Not correct number port" << endl;
    }
    MyTCPServer Server(atoi(argv[1]));
    struct sigaction action;
    action.sa_handler = signalHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGTSTP, &action, nullptr) == -1) {
        perror("signal");
    }

    if (sigaction(SIGINT, &action, nullptr) == -1) {
        perror("signal");
    }
    thread thread_(&MyTCPServer::start, &Server);
    cout << "Thread №" << thread_.get_id() << endl;
    thread_.join();
//    while (true) {

//    }
    return 0;
}
