//
// Created by hama on 20. 4. 15..
//

#ifndef HAMANET_NETCLIENT_H
#define HAMANET_NETCLIENT_H

// hama library
#include "../util/util.h"
#include "netbase.h"
#include "netio.h"

namespace hama {

    class NetClient : public NetBase , public NetIO
    {
    public:
        NetClient() = default;
        ~NetClient() = default;
        DISALLOW_COPY_AND_ASSIGN_AND_MOVE(NetClient)

    private:
        NetSession * m_clientsession;
    public:

        int connect(const std::string& ip, int port) {
            struct sockaddr_in server_addr{};
            bzero(&server_addr, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
            server_addr.sin_port = htons(port);

            auto ret = ::connect(m_sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

            if (ret != 0){
                LOGGING(string_format("connection FAIL !!! ip : %s , port : %d", ip.c_str(), port))

                return ret;
            }
            m_clientsession = new NetSession(this);
            m_clientsession->m_sd = m_sd;
            registerEvent(m_clientsession);

            LOGGING(string_format("connect ip : %s , port : %d", ip.c_str(), port))

            return ret;
        }

        void write(const std::string & data){
            m_clientsession->write(data);
        }
    };

}
#endif //HAMANET_NETCLIENT_H
