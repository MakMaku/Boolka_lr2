#include "TClient.h"
#include "TINS.h"
#include "TSNS.h"
#include <chrono>
#include "Struct.h"

using namespace std;
using namespace chrono;

void thread_work(TClient* Client, microseconds delay, int workTime) {
    double time = 0;
    time_point<steady_clock> Tend = steady_clock::now(), Tbegin = steady_clock::now();
    while (time <= workTime) {
        time = duration<double>(Tend - Tbegin).count();
        Client->Working();
        Tend = steady_clock::now();
        this_thread::sleep_for(delay);
    }
}

void thread_send(TClient* Client, milliseconds delay, int workTime) {
    double time = 0; time_point<steady_clock> Tend = steady_clock::now(), Tbegin = steady_clock::now();
    while (time <= workTime) {
        time = duration<double>(Tend - Tbegin).count();
        Client->Sending();
        Tend = steady_clock::now();
        this_thread::sleep_for(delay);
    }
}


void thread_try(milliseconds delay, int workTime) {
    double time = 0;

    TClient* INS = new TINS();
    INS->set_port(12346);
    INS->set_ipaddres("127.0.0.1");
    INS->Creat_Socket();

    time_point<steady_clock> Tend = steady_clock::now(), Tbegin = steady_clock::now();
    while (time <= workTime) {
        time = duration<double>(Tend - Tbegin).count();
        
        INS->Working();
        INS->Sending();

        Tend = steady_clock::now();
        this_thread::sleep_for(milliseconds(delay));
    }

    INS->Clean_n_Close();
}

int main(){
    system("title UDP Client");
    
    TClient* INS = new TINS();          
    INS->set_port(12346);                              
    INS->set_ipaddres("127.0.0.1");
    INS->Creat_Socket();

    TClient* SNS = new TSNS();
    SNS->set_port(12346);
    SNS->set_ipaddres("127.0.0.1");
    SNS->Creat_Socket();

    thread tWorkINS(thread_work, INS, microseconds(2500), 300);
    thread tSendIns(thread_send, INS, milliseconds(10), 300);

    thread tWorkSNS(thread_work, SNS, microseconds(100000), 300);
    thread tSendSNS(thread_send, SNS, milliseconds(1000), 300);

    tWorkINS.join();    tWorkSNS.join();
    tSendIns.join();    tSendSNS.join();
    
    INS->Clean_n_Close();
    SNS->Clean_n_Close();
}
