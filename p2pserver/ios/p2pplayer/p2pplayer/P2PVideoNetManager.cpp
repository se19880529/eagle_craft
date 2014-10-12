//
//  P2PVideoNetManager.cpp
//  p2pplayer
//
//  Created by 刘瀚东 on 14-10-10.
//  Copyright (c) 2014年 eaglelun. All rights reserved.
//

#include "P2PVideoNetManager.h"

using namespace std;

void VideoNetManager::AddSocket(SOCKET sock)
{
    sockSet.insert(map<SOCKET,SOCKET>::value_type(sock, sock));
}

void VideoNetManager::RemoveSocket(SOCKET sock)
{
    sockSet.erase(sock);
}

void VideoNetManager::HandlerProtocol(long millisecond)
{
    fd_set fdseterr, fdsetread, fdsetwrite;
    map<SOCKET,SOCKET>::iterator iter = sockSet.begin();
    while(iter != sockSet.end())
    {
        FD_SET(iter->second, &fdseterr);
        FD_SET(iter->second, &fdsetread);
        FD_SET(iter->second, &fdsetwrite);
        iter++;
    }
    timeval tv;
    tv.tv_sec
}