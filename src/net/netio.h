//
// Created by hama on 20. 4. 15..
//

#ifndef HAMANET_NETIO_H
#define HAMANET_NETIO_H

#include <sys/epoll.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <list>
#include <algorithm>
// hama library
#include "../util/util.h"
#include "netsession.h"
#include "nethandler.h"

namespace hama {
    class NetIO
    {
    public:
        NetIO() = default;
        ~NetIO() {
            for (auto sessions : m_sessions){
                delete sessions;
            }
        }
        DISALLOW_COPY_AND_ASSIGN_AND_MOVE(NetIO)

    private:
        static const int MAX_DESCS_CNT = 256;

        struct epoll_event* m_events = nullptr;
        int m_epoll = -1;
        int m_server = -1;

        std::thread m_loop;
        bool m_stop = false;

        std::list<NetSession*> m_sessions;
        std::shared_ptr<NetHandler> m_handler; // this also can be multiple handler

    public:

        void setServer(int sd){
            m_server = sd;
        }

        void setHandler( std::shared_ptr<NetHandler>  handler){
            m_handler = handler;
        }


        int init(){
            m_epoll = epoll_create(MAX_DESCS_CNT);
            if (m_epoll == -1){
                LOGGING("epoll_create error")
                return errno;
            }

            m_events = new epoll_event[MAX_DESCS_CNT];

            return 0;
        }

        void stop() {

            LOGGING("epoll IO stop - start")

            m_stop = true;

            ::close(m_epoll);

            if (m_events != nullptr){
                delete m_events;
                m_events = nullptr;
            }

            if (m_loop.joinable())
            {
                m_loop.detach();
            }

            LOGGING("epoll IO stop - end")
        }

        int start(){
            m_loop = std::thread([&]() { epoll_loop(); });
            if (!m_loop.joinable()){
                LOGGING("failed to create epoll_loop thread")
                return -1;
            }


            return 0;
        }

        void join(){
            m_loop.join();
        }

        int registerEvent(NetSession* sess){
            struct epoll_event ev{};

            ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
            ev.data.ptr = sess;

            int ret = epoll_ctl(m_epoll, EPOLL_CTL_ADD, sess->m_sd, &ev);

            if (ret != 0) {
                LOGGING("failed epoll_ctl()")
                return ret;
            }

            m_sessions.push_back(sess);
            return 0;
        }

        int deleteEvent(NetSession* sess)
        {
            return epoll_ctl(m_epoll, EPOLL_CTL_DEL, sess->m_sd, NULL);
        }


        int updateEvent(NetSession* sess){
            struct epoll_event ev{};

            ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
            ev.data.ptr = sess;

            return epoll_ctl(m_epoll, EPOLL_CTL_MOD, sess->m_sd, &ev);
        }

        void removeSession(NetSession* sess) {
            std::list<NetSession*>::iterator fp = std::find(m_sessions.begin(), m_sessions.end(), sess);
            if (fp != m_sessions.end()){
                m_sessions.erase(fp);
                delete *fp;
            }
        }

        void terminateSession(NetSession* sess){
            deleteEvent(sess);
            sess->onClose();
            removeSession(sess);

            LOGGING("disconnect session")

        }

        void cleanup() {
            for (auto sess : m_sessions) {
                sess->onClose();
                delete sess;
            }
        }

    protected:
        void epoll_loop(){
            LOGGING("epoll IO running start")

            while (!m_stop)
            {
                int changed_events = epoll_wait(m_epoll, m_events, MAX_DESCS_CNT , 10);

                if (changed_events < 0){
                    continue;
                }

                for (int i = 0; i < changed_events; ++i)
                {
                    auto sess = static_cast<NetSession*>(m_events[i].data.ptr);

                    if (sess->m_sd == m_server) {

                        if (m_sessions.size() >= 256){
                            LOGGING("connections can assume no more than 256 simultaneously.");

                        }
                        else {
                            struct sockaddr_in client_addr;
                            int client_len = sizeof(client_addr);
                            int client = accept(m_server, (struct sockaddr *) &client_addr, (socklen_t *) &client_len);

                            auto clientsession = new NetSession(this);
                            clientsession->m_sd = client;
                            registerEvent(clientsession);

                            LOGGING(string_format("Client connected : %s", inet_ntoa(client_addr.sin_addr)));
                        }

                    }
                    else if (m_events[i].events & EPOLLIN)
                    {
                        int ret = sess->onRecv();

                        if (ret < 0) {
                            sess->onClose();
                            terminateSession(sess);
                            continue;
                        }

                        if (ret > 0)
                            m_handler->OnRecv(sess);
                    }
                    else if (m_events[i].events & EPOLLOUT)
                    {
                        int ret = sess->onSend();

                        if (ret < 0) {
                            sess->onClose();
                            terminateSession(sess);
                            continue;
                        }

                        if (ret > 0)
                            m_handler->OnSend(sess);

                    }
                    else if ((m_events[i].events & EPOLLHUP)
                             || (m_events[i].events & EPOLLRDHUP)  // half close by the remote connection
                             || (m_events[i].events & EPOLLERR)
                             || !(m_events[i].events & EPOLLIN))
                    {
                        sess->onClose();
                        terminateSession(sess);
                        continue;
                    }

                    updateEvent(sess);
                }
            }

            cleanup();
        }
    };

}

#endif //HAMANET_NETIO_H
