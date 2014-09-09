#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>

using namespace std;
struct PeerInfo
{
	char ip[256];
	unsigned short port;
};
struct PeerInfoSet
{
	long id;
	vector<PeerInfo> infos;
};
void BufferDump(const char* buffer, int size)
{
	const char* end = buffer+size;
	const int cols = 16;
	int time = 1;
	do
	{
		for(int i = 0; i < cols; i++)
		{
			if(i != cols-1)
				printf("%2d ", i);
			else
				printf("%2d+", i);
		}
	} while(time-->0);
	printf("\n");
	for(int i = 0; i < (size+cols-1)/cols;i++)
	{
		const char* rowstart = buffer;
		for(int j = 0; j < cols; j++, buffer++)
		{
			if(buffer < end)
			{
				if(j != cols-1)
					printf("%2x ", (unsigned char)*buffer);
				else
					printf("%2x|", *buffer);
			}
			else
			{
				if(j != cols-1)
					printf("   ");
				else
					printf("  |");
			}
		}
		buffer = rowstart;
		for(int j = 0; j < cols; j++, buffer++)
		{
			if(buffer < end && *buffer>32)
			{
				if(j != cols-1)
					printf(" %1c ", *buffer);
				else
					printf(" %1c|", *buffer);
			}
			else
			{
				if(j != cols-1)
					printf("   ");
				else
					printf("  |");
			}
		}
		printf("\n");
	}
}

class P2PCache
{
public:
	int AddPeer(long id, char* ip, unsigned short port)
	{
		if(NULL == ip)
			return -1;
		PeerInfo pi;
		strcpy(pi.ip, ip);
		pi.port = port;
		map<long, PeerInfoSet>::iterator iter = _peerMap.find(id);
		if(iter == _peerMap.end())
		{
			PeerInfoSet pis;
			pis.id = id;
			pis.infos.push_back(pi);
			_peerMap[id] = pis;
		}
		else
		{
			iter->second.infos.push_back(pi);
		}
		return 0;
	}
	int RemovePeer(long id, char* ip, unsigned short port)
	{
		if(NULL == ip)		//means delete all
		{
			_peerMap.erase(id);
			return 0;
		}
		else
		{
			map<long, PeerInfoSet>::iterator iter = _peerMap.find(id);
			if(_peerMap.end() == iter)
			{
				return 0;
			}
			else
			{
				vector<PeerInfo>::iterator iter2 = iter->second.infos.begin();
				while(iter2 != iter->second.infos.end())
				{
					if(strcmp(iter2->ip, ip) == 0 && iter2->port == port)
					{
						iter2 = iter->second.infos.erase(iter2);
					}
					else
						iter2++;
				}
				return 0;
			}
		}
	}
	PeerInfo* GetPeerInfo(long id)
	{
		cout<<"search for peer "<<id<<endl;
		map<long, PeerInfoSet>::iterator iter = _peerMap.find(id);
		if(iter != _peerMap.end())
		{
			cout<<"found!"<<endl;
			if(iter->second.infos.end() != iter->second.infos.begin())
			{
				return (PeerInfo*)&(*(iter->second.infos.begin()));
			}
		}
		return NULL;
	}
private:
	map<long, PeerInfoSet> _peerMap;
};
class Protocol
{
public:
	int type;
	Protocol(int t):type(t){}
	static int PeekType(char* buffer, int size)
	{
		if(size > 4)
		{
			return *(int*)buffer;
		}
		else
		{
			return -1;		//a invalid value
		}
	}
	static Protocol* Create(int type);
	virtual size_t Read(char* buffer, size_t size)
	{
		if(size > 4)
		{
			type = *(int*)buffer;
			buffer+=4;
			return 4;
		}
		return 0;
	}
	virtual char* Write(char* buffer, char* end)
	{
		if(end - buffer > 4)
		{
			*(int*)buffer = type;
			buffer += 4;
		}
		return buffer;
	}
};

class SessionProtocol : public Protocol
{
public:
	long id;
	SessionProtocol(int t) : Protocol(t){}
	size_t Read(char* buffer, size_t size)
	{
		size_t used = Protocol::Read(buffer, size);
		if(size-used > 8)
		{
			id = *(long*)(buffer+used);
			used+=8;;
		}
		return used;
	}
	char* Write(char* buffer, char* end)
	{
		buffer = Protocol::Write(buffer, end);
		if(end - buffer > 8)
		{
			*(long*)buffer = id;
			buffer += 8;
		}
		return buffer;
	}
};

class AddPeerProtocol : public SessionProtocol
{
public:
	const static int TYPE = 1;
	char name[256];
	AddPeerProtocol():SessionProtocol(TYPE){}
	size_t Read(char* buffer, size_t size)
	{
		size_t used = SessionProtocol::Read(buffer, size);
		if(size-used > 2 && size-used>=2+*(unsigned short*)(buffer+used))
		{
			memcpy(name, buffer+2+used, *(unsigned short*)(buffer+used));
			name[*(unsigned short*)(buffer+used)] = 0;
			used += 2+*(unsigned short*)(buffer+used);
		}
		return used;
	}
	char* Write(char* buffer, char* end)
	{
		char* begin = buffer;
		buffer = SessionProtocol::Write(buffer, end);
		if(end - buffer >= 2+strlen(name))
		{
			*(unsigned char*)buffer = strlen(name);
			buffer += 2;
			memcpy(buffer, name, strlen(name));
			buffer += strlen(name);
		}
		return buffer;
	}
};

class AddPeerResultProtocol : public Protocol
{
public:
	const static int TYPE = 2;
	unsigned char retcode;
	long id;
	AddPeerResultProtocol():Protocol(TYPE){}
	size_t Read(char* buffer, size_t size)
	{
		size_t used = Protocol::Read(buffer, size);
		if(size-used >= 9)
		{
			retcode = *(unsigned char*)(buffer+used);
			used+=1;
			id = *(long*)(buffer+used);
			used+=8;
		}
		return used;
	}
	char* Write(char* buffer, char* end)
	{
		buffer = Protocol::Write(buffer, end);
		if(end - buffer >= 9)
		{
			*(unsigned char*)buffer = retcode;
			buffer += 1;
			*(long*)buffer = id;
			buffer += 8;
		}
		return buffer;
	}
};

class QueryPeerProtocol : public SessionProtocol
{
public:
	const static int TYPE = 3;
	char user_query[256];
	QueryPeerProtocol():SessionProtocol(TYPE){}
	size_t Read(char* buffer, size_t size)
	{
		size_t used = SessionProtocol::Read(buffer, size);
		if(size-used>2 && size-used >= *(unsigned short*)(buffer+used) + 2)
		{
			int len = *(unsigned short*)(buffer+used);
			used+=2;
			memcpy(user_query, buffer+used, len);
			user_query[len] = 0;
			used+=strlen(user_query);
		}
		return used;
	}
	char* Write(char* buffer, char* end)
	{
		buffer = SessionProtocol::Write(buffer, end);
		if(end - buffer > 2+strlen(user_query))
		{
			*(unsigned short*)buffer = strlen(user_query);
			buffer+=2;
			memcpy(buffer, user_query, strlen(user_query));
			buffer += strlen(user_query);
		}
		return buffer;
	}
};

class QueryPeerReProtocol : public Protocol
{
public:
	const static int TYPE = 4;
	char ip[256];
	unsigned short port;
	char name[256];
	QueryPeerReProtocol():Protocol(TYPE){}
	size_t Read(char* buffer, size_t size)
	{
		size_t used = Protocol::Read(buffer, size);
		buffer += used;
		if(size - used > 2 && size - used > *(unsigned short*)buffer+6 && size - used >= *(unsigned short*)buffer+6+*(unsigned short*)(buffer+*(unsigned short*)(buffer)+4))
		{
			int len = *(unsigned short*)buffer;
			memcpy(ip, buffer + 2, len);
			ip[len] = 0;
			buffer += len + 2;
			used += len+2;
			port = *(unsigned short*)buffer;
			buffer += 2;
			used += 2;
			len = *(unsigned short*)buffer;
			buffer += 2;
			used += 2;
			memcpy( name,buffer, len);
			name[len] = 0;
			used += len;
		}
		return used;
	}
	char* Write(char* buffer, char* end)
	{
		buffer = Protocol::Write(buffer, end);
		if(end - buffer >= strlen(ip)+6+strlen(name))
		{
			*(unsigned short*)buffer = strlen(ip);
			buffer += 2;
			memcpy(buffer, ip, strlen(ip));
			buffer += strlen(ip);
			*(unsigned short*)buffer = port;
			buffer += 2;
			*(unsigned short*)buffer = strlen(name);
			buffer += 2;
			memcpy(buffer,name, strlen(name));
			buffer += strlen(name);
		}
		return buffer;
	}
};

class MakeHoleRequest : public Protocol
{
public:
	const static int TYPE = 5;
	char ip[256];
	unsigned short port;
	char name[256];
	MakeHoleRequest():Protocol(TYPE){}
	size_t Read(char* buffer, size_t size)
	{
		size_t used = Protocol::Read(buffer, size);
		buffer += used;
		if(size - used > 2 && size - used > *(unsigned short*)buffer+6 && size - used >= *(unsigned short*)buffer+6+*(unsigned short*)(buffer+*(unsigned short*)(buffer)+4))
		{
			int len = *(unsigned short*)buffer;
			memcpy(ip, buffer + 2, len);
			ip[len] = 0;
			buffer += len + 2;
			used += len+2;
			port = *(unsigned short*)buffer;
			buffer += 2;
			used += 2;
			len = *(unsigned short*)buffer;
			buffer += 2;
			used += 2;
			memcpy( name,buffer, len);
			name[len] = 0;
			used += len;
		}
		return used;
	}
	char* Write(char* buffer, char* end)
	{
		buffer = Protocol::Write(buffer, end);
		if(end - buffer >= strlen(ip)+6+strlen(name))
		{
			*(unsigned short*)buffer = strlen(ip);
			buffer += 2;
			memcpy(buffer, ip, strlen(ip));
			buffer += strlen(ip);
			*(unsigned short*)buffer = port;
			buffer += 2;
			*(unsigned short*)buffer = strlen(name);
			buffer += 2;
			memcpy(buffer,name, strlen(name));
			buffer += strlen(name);
		}
		return buffer;
	}
};

class MakeHoleReject : public SessionProtocol
{
public:
	const static int TYPE = 6;
	char ip[256];
	unsigned short port;
	char name[256];
	MakeHoleReject():SessionProtocol(TYPE){}
	size_t Read(char* buffer, size_t size)
	{
		size_t used = Protocol::Read(buffer, size);
		buffer += used;
		if(size - used > 2 && size - used > *(unsigned short*)buffer+6 && size - used >= *(unsigned short*)buffer+6+*(unsigned short*)(buffer+*(unsigned short*)(buffer)+4))
		{
			int len = *(unsigned short*)buffer;
			memcpy(ip, buffer + 2, len);
			ip[len] = 0;
			buffer += len + 2;
			used += len+2;
			port = *(unsigned short*)buffer;
			buffer += 2;
			used += 2;
			len = *(unsigned short*)buffer;
			buffer += 2;
			used += 2;
			memcpy( name,buffer, len);
			name[len] = 0;
			used += len;
		}
		return used;
	}
	char* Write(char* buffer, char* end)
	{
		buffer = Protocol::Write(buffer, end);
		if(end - buffer >= strlen(ip)+6+strlen(name))
		{
			*(unsigned short*)buffer = strlen(ip);
			buffer += 2;
			memcpy(buffer, ip, strlen(ip));
			buffer += strlen(ip);
			*(unsigned short*)buffer = port;
			buffer += 2;
			*(unsigned short*)buffer = strlen(name);
			buffer += 2;
			memcpy(buffer,name, strlen(name));
			buffer += strlen(name);
		}
		return buffer;
	}
};

class MakeHoleAgree : public SessionProtocol
{
public:
	const static int TYPE = 7;
	char ip[256];
	unsigned short port;
	char name[256];
	MakeHoleAgree():SessionProtocol(TYPE){}
	size_t Read(char* buffer, size_t size)
	{
		size_t used = Protocol::Read(buffer, size);
		buffer += used;
		if(size - used > 2 && size - used > *(unsigned short*)buffer+6 && size - used >= *(unsigned short*)buffer+6+*(unsigned short*)(buffer+*(unsigned short*)(buffer)+4))
		{
			int len = *(unsigned short*)buffer;
			memcpy(ip, buffer + 2, len);
			ip[len] = 0;
			buffer += len + 2;
			used += len+2;
			port = *(unsigned short*)buffer;
			buffer += 2;
			used += 2;
			len = *(unsigned short*)buffer;
			buffer += 2;
			used += 2;
			memcpy( name,buffer, len);
			name[len] = 0;
			used += len;
		}
		return used;
	}
	char* Write(char* buffer, char* end)
	{
		buffer = Protocol::Write(buffer, end);
		if(end - buffer >= strlen(ip)+6+strlen(name))
		{
			*(unsigned short*)buffer = strlen(ip);
			buffer += 2;
			memcpy(buffer, ip, strlen(ip));
			buffer += strlen(ip);
			*(unsigned short*)buffer = port;
			buffer += 2;
			*(unsigned short*)buffer = strlen(name);
			buffer += 2;
			memcpy(buffer,name, strlen(name));
			buffer += strlen(name);
		}
		return buffer;
	}
};
