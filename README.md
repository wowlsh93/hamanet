# hamanet
High performance async network library based on Linux ePoll for C++ programmers.

## enviroment 

- ubuntu 18.04 (https://en.wikipedia.org/wiki/Epoll)
- C++ compiler with C++17 support.
- CMake(>=3.15)

## build (server)
<pre><code>
 git clone https://github.com/wowlsh93/hamanet.git
 cd hamanet
 mkdir build
 cd build
 cmake ..
 cmake --build . [or make] 
 ./hamanet 
</pre></code>


### build (client)
<pre><code>
 In hamanet root directory 
 
 CMakeLists(client).txt <-- remove "(client)"
 CMakeLists.txt <--- add "(server)" 
 
 cd build
 cmake ..
 cmake --build . [or make] 
 ./hamanet_client 
</pre></code>

## run 

- server start

   ./hamanet

- client start

   ./hamanet_client

## example (very simple !!)

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



## Architecture

ePoll is react pattern. So some part  is a little bit  tricky compared to IOCP(WINDOWS) of Proact Pattern.
on IOCP,  "OnRecv, OnSend" happens when  i/o is completed. so I can know how much i send/recv data correctly. 
but on ePoll ,"OnRecv, OnSend" happens when i/o is ready. so I can not know how much I can send/recv data correctly.  It 's  nontransparent on my knowlege. "need more experiment."


- NetBase

  this supports TCP/IP options and common attributes.

- NetServer

  this has server side operations like open / bind / listen 

- NetClient

  this has client side operations like connect and write. write function() do not write to socket directly,  it store at core buffer.  when ePool's Write event happens , It starts to  write it to socket

- NetSession

  this is composed of socket discriptor, read/write temporary buffer. this buffer is just std::string so it can be more efficient structure later
  
- NetIO

  this is core epoll i/o operator. (based on Edge Trigger(ET))
  when some events happen, sessions will react firstly on it and pass over to user application.

- NetHandler

  This is a Interface having OnRecv, OnSend. User who use this network library can override/implement this function for his/her target applications

- LyricsPoet 

  servser side very simple example
  
- LyricsReader

  client side very simple example 

## Future work 

- worker thread pool (for user side about cpu intesive job)
- efficient buffer 
- and a lot :-) 

## Email

wowlsh93@gmail.com
