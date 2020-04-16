//
// Created by hama on 20. 4. 15..
//

#ifndef HAMANET_NETHANDLER_H
#define HAMANET_NETHANDLER_H

#include "netsession.h"

namespace hama {

    class NetSession;
    class NetHandler {
    public:
        virtual void OnRecv(NetSession *sess) = 0;
        virtual void OnSend(NetSession *sess) = 0;
    };

}

#endif //HAMANET_NETHANDLER_H
