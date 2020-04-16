//
// Created by hama on 20. 4. 15..
//

#ifndef HAMANET_NETBASE_H
#define HAMANET_NETBASE_H

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <cstring>
#include <csignal>

namespace hama{

    class NetBase {

    public:
        NetBase() {
            m_sd = ::socket(PF_INET, SOCK_STREAM, 0);
        }
        ~NetBase() = default;

    protected:
        int m_sd = -1;

    protected:
        int setNonblocking(){
            int flags = fcntl(m_sd, F_GETFL);
            flags |= O_NONBLOCK;
            return fcntl(m_sd, F_SETFL, flags);
        }

        int setReuseaddr(){
            int on = 1;
            return setsockopt(m_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
        }

        int setTcpnodelay(){
            int on = 1;
            return setsockopt(m_sd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(int));
        }

        int setNosigpipe (){
            // Looking at the C++ open source library Poco(https://github.com/pocoproject/poco/blob/master/Net/src/SocketImpl.cpp)
            // the Broken Pipe problem only occurs in the Unix family,
            // and does not occur on Linux.
            signal(SIGPIPE, SIG_IGN);
            return 1;
        }
    };

}

#endif //HAMANET_NETBASE_H
