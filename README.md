# hamanet


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
 git clone https://github.com/wowlsh93/hamanet.git
 cd hamanet
 
  CMakeLists(client).txt <-- remove "(client)"
  CMakeLists.txt <--- add "(server" 

 mkdir build
 cd build
 cmake ..
 cmake --build .
 ./lyricsreader
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
        // send base on recv data

        int ntimes = atoi(sess->m_recv.m_data.c_str());

        const char* contents = R"""(
        If you can keep your head when all about you
        Are losing theirs and blaming it on you,
        If you can trust yourself when all men doubt you,
        But make allowance for their doubting too;
        If you can wait and not be tired by waiting,
        Or being lied about, don’t deal in lies,
        Or being hated, don’t give way to hating,
        And yet don’t look too good, nor talk too wise:

        If you can dream—and not make dreams your master;
        If you can think—and not make thoughts your aim;
        If you can meet with Triumph and Disaster
        And treat those two impostors just the same;
        If you can bear to hear the truth you’ve spoken
        Twisted by knaves to make a trap for fools,
                Or watch the things you gave your life to, broken,
        And stoop and build ’em up with worn-out tools:

        If you can make one heap of all your winnings
        And risk it on one turn of pitch-and-toss,
                And lose, and start again at your beginnings
        And never breathe a word about your loss;
        If you can force your heart and nerve and sinew
        To serve your turn long after they are gone,
        And so hold on when there is nothing in you
        Except the Will which says to them: ‘Hold on!’

        If you can talk with crowds and keep your virtue,
        Or walk with Kings—nor lose the common touch,
        If neither foes nor loving friends can hurt you,
        If all men count with you, but none too much;
        If you can fill the unforgiving minute
        With sixty seconds’ worth of distance run,
        Yours is the Earth and everything that’s in it,
        And—which is more—you’ll be a Man, my son!

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

