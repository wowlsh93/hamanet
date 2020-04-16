//
// Created by hama on 20. 4. 15..
//

#ifndef HAMANET_NETSERVER_H
#define HAMANET_NETSERVER_H

// hama library
#include "../util/util.h"
#include "netbase.h"
#include "netio.h"

namespace hama {

    class NetServer : public NetBase , public NetIO
    {
    public:
        NetServer() = default;
        ~NetServer() = default;
        DISALLOW_COPY_AND_ASSIGN_AND_MOVE(NetServer)
    private:
        static const int LISTEN_BACKLOG = SOMAXCONN;


    public:

        int open(int port) {

            struct sockaddr_in server_addr;
            memset(&server_addr, 0,  sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            server_addr.sin_port = htons(port);

            int ret = ::bind(m_sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

            if (ret != 0)
            {
                LOGGING("::bind() error")

                ::close(m_sd);
                m_sd = -1;
                return errno;
            }

            ret = ::listen(m_sd, LISTEN_BACKLOG);

            if (ret != 0)
            {
                LOGGING("::listen() error")

                ::close(m_sd);
                m_sd = -1;
                return errno;
            }

            setNonblocking();
            setReuseaddr();
            setTcpnodelay();
            setNosigpipe();

            setServer(m_sd);
            auto serversession = new NetSession(this);
            serversession->m_sd = m_sd;
            registerEvent(serversession);

            LOGGING(string_format("open port : %d", port))

        }

    };

}

#endif //HAMANET_NETSERVER_H
