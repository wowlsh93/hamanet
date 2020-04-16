#include <iostream>
#include <random>

#include "src/net/netclient.h"
#include "src/net/nethandler.h"
#include "src/net/netsession.h"
#include "src/net/netio.h"

//server
class LyricsClient : public hama::NetHandler {
public:

    virtual void OnRecv(hama::NetSession * sess) {
        std::cout << "LyricsClient recv" << std::endl;
        std::cout << sess->m_recv.m_data << std::endl;

    }

    virtual void OnSend(hama::NetSession * sess){
        std::cout << "LyricsClient requesting!!" << std::endl;
    }

};

#define DATA_SIZE   5

int main() {
    std::cout << "========================" << std::endl;
    std::cout << "----- start client ----" << std::endl;
    std::cout << "========================" << std::endl;

    auto myhandler = std::make_shared<LyricsClient>();

    hama::NetClient client;
    client.setHandler(myhandler);
    client.init();
    client.connect("127.0.0.1", 2826);
    client.start();

    for (int i = 0 ; i < 100 ; i++){

        char sendBuf[DATA_SIZE];
        memset(sendBuf,0, DATA_SIZE);
        std::random_device randDeivce;
        std::default_random_engine randEngine{ randDeivce() };
        std::uniform_int_distribution<int> dist1{ 1, 99999 };
        int number = dist1(randEngine);
        sprintf(sendBuf,"%d",number);

        client.write(sendBuf );
        sleep(1000);
    }

    client.join();
    client.stop();

    return 0;
}
