//
// Created by hama on 20. 4. 15..
//

#ifndef HAMANET_NETSESSION_H
#define HAMANET_NETSESSION_H

#include <mutex>
#include <unistd.h>
#include <sys/socket.h>
#include <tgmath.h>
#include <cmath>
// hama library
#include "../util/util.h"
#include "netio.h"


namespace hama {

    #define MAX_RECV_BYTES 1024 * 64
    class NetIO;
    class NetPacket
    {
    public:
        NetPacket() {
            m_data.reserve(MAX_RECV_BYTES);
        }
        void reset(int sendsize)
        {
            if (sendsize == -1)
                return;
            m_data.erase(0, sendsize);
        }
        void clear(){
            m_data.clear();
        }
        std::string	m_data;
    };

    class NetSession
    {
    public:
        NetSession(NetIO * io){
            m_io = io;
        }
        ~NetSession() = default;
        DISALLOW_COPY_AND_ASSIGN_AND_MOVE(NetSession)

        int m_sd = -1;

        NetIO *     m_io;
        NetPacket   m_send;
        NetPacket   m_recv;

        std::mutex  m_sendmtx;

        bool write(const std::string& data){
            std::lock_guard<std::mutex> guard(m_sendmtx);
            if (m_send.m_data.size() > 64 * 1024 * 1024)
                return false;

            m_send.m_data.append(data);

            return true;
        }

        bool onClose(){
            if(m_sd != -1)
                ::close(m_sd);
            m_sd = -1;
        }

        int onSend(){

            if (  m_send.m_data == "")
                return 0;
            std::lock_guard<std::mutex> guard(m_sendmtx);

            if (m_sd == -1){
                return -1;
            }

            auto sendlength = ::send(m_sd, (void *) m_send.m_data.c_str(), m_send.m_data.length(), 0);

            // In case data is large, it can remain some part.
            // So next write event time, we will write it continueasly
            m_send.reset(sendlength);

            return 1;
        }

        // you should read current all buffer  because event happens only once
        // when state is changed in epoll EL mode.
        // [todo] make efficient buffer for users instead of std::string
        int onRecv(){
            if (m_sd == -1)
            {
                return 0;
            }

            char buffer[MAX_RECV_BYTES];
            int length = recv(m_sd, buffer, sizeof(buffer), 0);

            if (length == 0)
                return 0;

            if (length == -1){
                if (errno == EAGAIN || errno == EWOULDBLOCK){
                    return 0;
                }
                return -1;
            }

            m_recv.m_data.append(buffer, length);

//            [todo] bug : read all buffer data.
//            but If i call recv continueasly until "return 0",  something wrong happended
//            while(true){
//                char buffer[1024];
//                int length = recv(m_sd, buffer, sizeof(buffer), 0);
//                m_recv.m_data.append(buffer, length);
//                totallength += length;
//
//                if (length > 0){
//                    continue;
//                }
//                else {
//                    if (length == -1){
//                        if (errno == EAGAIN || errno == EWOULDBLOCK){
//                            return 0;
//                        }
//                        return -1;
//                    }
//                    break;
//                }
//            }

            return length;
        }

    };

}
#endif //HAMANET_NETSESSION_H
