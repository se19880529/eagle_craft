//
//  P2PVideoNetManager.h
//  p2pplayer
//
//  Created by 刘瀚东 on 14-10-10.
//  Copyright (c) 2014年 eaglelun. All rights reserved.
//

#ifndef p2pplayer_P2PVideoNetManager_h
#define p2pplayer_P2PVideoNetManager_h
#include "P2PVideoProtocol.h"
#include <map>
#define SOCKET int
class IVideoProtocolHandler
{
public:
    void OnProtocol(VideoProtocol* p, SOCKET s);
};

class VideoNetManager
{
public:
    void AddSocket(SOCKET sock);
    void RemoveSocket(SOCKET sock);
    void HandlerProtocol(long millisecond);
    void SendProtocol(VideoProtocol* p, SOCKET s);
    void SetProtocolHandler(IVideoProtocolHandler* h)
    {
        handler = h;
    }
    void SetBufferSize(int size);
private:
    std::map<SOCKET, SOCKET> sockSet;
    IVideoProtocolHandler* handler;
    char* buffer;
    int bufferSize;
    int bufferSizeInUse;
};


#endif
