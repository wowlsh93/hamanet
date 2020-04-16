# hamanet
This is an asynchronous network library based on Linux ePoll for C++ programmers.

### enviroment 

- ubuntu 18.04 (https://en.wikipedia.org/wiki/Epoll)
- C++ compiler with C++17 support.
- CMake(>=3.15)

### build (server)
<pre><code>
 git clone https://github.com/wowlsh93/hamanet.git
 cd hamanet
 mkdir build
 cd build
 cmake ..
 cmake --build .
 ./lyricspoet 
</pre></code>


### build (client)
<pre><code>
 In Same hamanet directory 
 
 CMakeLists(client).txt <-- remove "(client)"
 CMakeLists.txt <--- add "(server)" 

</pre></code>

### run 

- server start   
./lyricspoet
- client start 
./lyricsreader

### example (very easy!!)

server
<pre><code>
//
// Created by hama on 20. 4. 16..
//

#include <iostream>


#include "src/net/netserver.h"
#include "src/net/nethandler.h"
#include "src/net/netsession.h"

//server
class LyricsPoet : public hama::NetHandler {
public:

    virtual void OnRecv(hama::NetSession * sess) {

        std::cout << "LyricsPoet:  recv ok" << std::endl;
        std::cout << sess->m_recv.m_data << std::endl;

        int ntimes = atoi(sess->m_recv.m_data.c_str());

        const char* contents = R"""(
           Hello client !!
        )""""";

        for (int i = 1; i < ntimes; i++){
            sess->write(contents);
        }
    }

    virtual void OnSend(hama::NetSession * sess){

        std::cout << "LyricsPoet: send ok" << std::endl;
        sess->m_io->terminateSession(sess);
    }

};

int main() {
    std::cout << "========================" << std::endl;
    std::cout << "----- start server ----" << std::endl;
    std::cout << "========================" << std::endl;

    auto myhandler = std::make_shared<LyricsPoet>();

    hama::NetServer server;
    server.setHandler(myhandler);
    server.init();
    server.open(2826);
    server.start();
    server.join();
    server.stop();

    return 0;
}

</pre></code>


### Architecture

- 

### Future work 

-

### Email
wowlsh93@gmail.com
