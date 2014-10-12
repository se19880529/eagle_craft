#include "udpheader.h"
#include <stack>
template<typename T>
class DictionaryTree
{
public:
	bool Get(char* key, T& val)
	{
		Node* n = &root;
		while(*key && n)
		{
			n = n->childs[*(key++) - 32];
		}
		if(!*key && n && n->settled)
		{
			val = n->val;
			return true;
		}
		else
			return false;
	}
	void Set(char* key, T val)
	{
		Node* n = &root;
		while(*key)
		{
			if(!n->childs[*key-32])
			{
				n->childs[*key-32] = new Node();
			}
			n = n->childs[*key-32];
			key++;
		}
		n->settled = true;
		n->val = val;
	}
	void Dump()
	{
		std::stack<Node*> q;
		char key[256];
		int keyid = 0;
		q.push(&root);
		cout<<"Dump DictionaryTree:"<<endl;
		while(!q.empty())
		{
			Node* n = q.top();
			q.pop();
			if(n == NULL)
			{
				keyid--;
				key[keyid] = 0;
			}
			else if((unsigned long)n < 256)
			{
				key[keyid] = *(char*)&n;
				keyid++;
				key[keyid] = 0;
			}
			else
			{
				if(n->settled)
				{
					cout<<key<<":"<<n->val<<endl;
				}	
				for(int i = 0; i < 256; i++)
				{
					if(n->childs[i] != NULL)
					{
						q.push(NULL);
						q.push(n->childs[i]);
						q.push((Node*)(i+32));
					}
				}
			}
		}
	}
private:
	class Node
	{
	public:
		Node()
		{
			for(int i = 0; i < 256; i++)
				childs[i] = NULL;
			settled = false;
		}
		Node* childs[256];
		T val;
		bool settled;
	};
	Node root;
};
class P2PServer
{
public:
	P2PServer()
	{
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		sockaddr_in addr;
		addr.sin_port = htons(8887);
		addr.sin_addr.s_addr = inet_addr("10.211.55.2");
		bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in));
	}
	void Send(char* ip, unsigned short port, P2PProtocol& p)
	{
		sockaddr_in addr;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);
		char buffer[4096];
		char* end = p.Write(buffer, buffer+4095);
		cout<<"send data to "<<ip<<":"<<port<<endl;
		BufferDump(buffer, end-buffer);
		sendto(sock, buffer, end-buffer,0, (const sockaddr*)&addr, sizeof(sockaddr_in)); 
	}
	void HandleProtocol()
	{
		char buffer[4096];
		sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int bufferlen = recvfrom(sock, buffer, 4096, 0, (sockaddr*)&addr, &len);
		cout<<"received data size:"<<bufferlen<<endl;
		BufferDump(buffer, bufferlen);
		if(bufferlen > 4)
			cout<<"protocol type:"<<P2PProtocol::PeekType(buffer, bufferlen)<<endl;
		P2PProtocol* p = NULL;
		switch(P2PProtocol::PeekType(buffer, bufferlen))
		{
		case AddPeerProtocol::TYPE:
			p = new AddPeerProtocol();
			break;
		case QueryPeerProtocol::TYPE:
			p = new QueryPeerProtocol();
			break;
		case MakeHoleAgree::TYPE:
			p = new MakeHoleAgree();
			break;	
		}
		if(p)
		{
			cout<<"find protocol:"<<p->type<<endl;
			p->Read(buffer, bufferlen);
			OnProtocol(*p, sock, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
			delete p;
		}
	}
	void OnProtocol(P2PProtocol& p, int socket, char* ip, unsigned short port)
	{
		switch(p.type)
		{
		case AddPeerProtocol::TYPE:
		{
			AddPeerProtocol& peer = *(AddPeerProtocol*)&p;	
			if(peer.id < 0)
			{
				peer.id = _sessionSeed++;
			}
			if(peer.name[0])
			{
				_dict.Set(peer.name, peer.id);
				_nameMap[peer.id] = peer.name;
				cout<<"dump _dict"<<endl;
				_dict.Dump();
			}
			_cache.AddPeer(peer.id, ip, port);
			cout<<"add peer request from"<<ip<<":"<<port<<" with name "<<peer.name<<endl;
			AddPeerResultProtocol p;
			p.retcode = 0;
			p.id = peer.id;	
			Send(ip, port, p);
		}
			break;
		case QueryPeerProtocol::TYPE:
		{
			QueryPeerProtocol& query = *(QueryPeerProtocol*)&p;
			long id;
			cout<<"query peer request from "<<_nameMap[query.id]<<" in "<<ip<<":"<<port<<" for "<<query.user_query<<endl;
			if(_dict.Get(query.user_query, id))
			{
				PeerInfo* info = _cache.GetPeerInfo(id);
				if(info != NULL)
				{
					MakeHoleRequest req;
					strcpy(req.ip, ip);
					strcpy(req.name, _nameMap[query.id].c_str());
					req.port = port;
					req.type = MakeHoleRequest::TYPE;
					Send(info->ip, info->port, req);
					cout<<"query peer request success, find peer "<<info->ip<<":"<<info->port<<endl;
				}
			}
		}
			break;
		case MakeHoleAgree::TYPE:
		{
			MakeHoleAgree& re = *(MakeHoleAgree*)&p;
			QueryPeerReProtocol p;
			strcpy(p.ip, ip);
			strcpy(p.name, re.name);
			p.port = port;
			Send(re.ip, re.port, p);
			cout<<"peer hole made for "<<re.ip<<":"<<re.port<<" at "<<ip<<":"<<port<<endl;
		}
			break;	
		}
	}
private:
	int sock;
	P2PCache _cache;
	long _sessionSeed;
	map<long, string> _nameMap;
	DictionaryTree<long> _dict;
};

int main()
{
	P2PServer server;
	while(true)
	{
		server.HandleProtocol();
	}

	return 0;
}
