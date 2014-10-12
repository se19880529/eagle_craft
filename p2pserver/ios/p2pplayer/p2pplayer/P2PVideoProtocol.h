//
//  P2PVideoProtocol.h
//  p2pplayer
//
//  Created by 刘瀚东 on 14-10-8.
//  Copyright (c) 2014年 eaglelun. All rights reserved.
//

#ifndef p2pplayer_P2PVideoProtocol_h
#define p2pplayer_P2PVideoProtocol_h
#include "udpclient.h"

class Octets
{
public:
    Octets(const char* str = NULL)
    {
        head = (char*)1;
        tail = 0;
        if(str != NULL)
            Append(str, str+strlen(str));
    }
    Octets(const char* start, const char* end)
    {
        head = (char*)1;
        tail = 0;
        if(start != NULL)
            Append(start, end);
    }
    void Append(const char* start, const char* end)
    {
        Expand(tail - head + 1 + end-start+1);
        for(char* p = tail;end >= start;p--,end--)
        {
            *p = *end;
        }
    }
    void Append(Octets* oct)
    {
        Append(oct->head, oct->tail);
    }
    void Erase(int start, int end)
    {
        if(end - start < tail - head)
        {
            
        }
    }
protected:
    void Expand(int size)
    {
        if(tail - head + 1 < size)
        {
            //round to 4
            size = (size+3) / 4 * 4;
            char* newbuf = (char*)malloc(size);
            for(char* h = head; h <= tail; h++)
            {
                *(newbuf + (h - head)) = *h;
            }
            if(tail != NULL)
                free(head);
            head = newbuf;
            tail = newbuf + size - 1;
        }
    }
    char* head;
    char* tail;
};
class OctetsException
{
};
class OctetsStream:public Octets
{
public:
    OctetsStream(const Octets& oct):Octets(oct){Reset();}
    template < typename T>
    T Read(bool peek = false) throw()
    {
        if(!CheckLeft(sizeof(T)))
            throw new OctetsException;
        T* res = (T*)ptr;
        if(!peek)
            Advance(sizeof(T));
        return *res;
    }
    template < typename T>
    void Write(T v)
    {
        Append((const char*)&v, (const char*)&v+sizeof(T)-1);
    }
private:
    bool CheckLeft(int need)
    {
        return tail - ptr >= need;
    }
    void Advance(int step)
    {
        ptr += step;
    }
    void Reset()
    {
        ptr = head;
    }
    char* ptr;
};
class VideoProtocol
{
public:
    int type;
    VideoProtocol(int t):type(t){}
    virtual void Read(OctetsStream& s)
    {
        try {
            type = s.Read<int>();
        } catch (OctetsException ex) {
            type = -1;
        }
    }
    virtual void Write(OctetsStream& s)
    {
        s.Write(type);
    }
};


#endif
