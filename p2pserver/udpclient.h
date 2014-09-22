#include "udpheader.h"
#include <sys/select.h>
class P2PClientListener
{
public:
	virtual void OnAddPeer(const char* name) = 0;
	virtual void OnDelPeer(const char* name) = 0;
	virtual void OnRecieveData(const char* name, const char* data, int size) = 0;
};
class P2PClient
{
public:
	P2PClient(const char* ip, unsigned short port)
	{
		server_addr.sin_addr.s_addr = inet_addr(ip);
		server_addr.sin_port = htons(port);
		session_id = -1;
	}

	void SetListener(P2PClientListener* lis)
	{
		listener = lis;
	}

	void SetName(char* nm)
	{
		strcpy(name, nm);
		cout<<"set name to "<<nm<<" settled "<<name<<endl;
	}

	void SendToServer(int sock, SessionProtocol& p)
	{
		char buffer[4096];
		p.id = session_id;
		char* end = p.Write(buffer, buffer + 4095);
		int sz = sendto(sock, buffer, end - buffer, 0, (const sockaddr*)&server_addr, sizeof(sockaddr_in));
		cout<<sock<<":sent type "<<p.type<<" to server, data size:"<<sz<<endl;
	}

	bool QueryPeerInfo(const char* nm)
	{
		if(fds_idel.size() > 0)
		{
			int sock = *fds_idel.begin();
			QueryPeerProtocol p;
			strcpy(p.user_query, nm);
			SendToServer(sock, p);
			return true;
		}
		else
			return false;
	}

	void SendPeerInfo(int sock)
	{
		AddPeerProtocol p;
		strcpy(p.name, name);
		SendToServer(sock, p);
	}

	int AddPeer()
	{
		int sock = socket(AF_INET, SOCK_DGRAM, 0);
		fds_idel.push_back(sock);
		SendPeerInfo(sock);
		return sock;
	}

	bool SendToPeer(const char* name, const char* text)
	{
		map<string, int>::iterator iter = peer_socket_map.begin();
		while(peer_socket_map.end() != iter)
		{
			if(strcmp(name, iter->first.c_str()) == 0)
			{
				int sock = iter->second;
				sockaddr_in addr = addr_bind_map[sock];
				cout<<"ok, now send to peer:"<<inet_ntoa(addr.sin_addr)<<":"<<ntohs(addr.sin_port)<<endl;
				sendto(sock, text, strlen(text)>102399?102399:strlen(text), 0, (const sockaddr*)&addr, sizeof(addr));
				return true;
			}
			iter++;
		}
		cout<<"peer "<<name<<" not found!"<<endl;
		return false;
	}
	
	void HandleIdelFD(long msec)
	{
		if(fds_idel.size() > 0)
		{
			timeval tv;
			tv.tv_sec = msec / 1000;
			tv.tv_usec = msec % 1000 * 1000;
			fd_set set;
			fd_set set_error;
			FD_ZERO(&set_error);
			FD_ZERO(&set);
			vector<int>::iterator iter = fds_idel.begin();
			int maxfd = -1;
			while(iter != fds_idel.end())
			{
				FD_SET(*iter, &set);
				FD_SET(*iter, &set_error);
				cout<<"put in idel socket:"<<*iter<<endl;
				if(maxfd < *iter)
					maxfd = *iter;
				iter++;
			}
			int ret = select(maxfd+1, &set, NULL, &set_error, &tv);
			if(ret < 0)
			{
				//error happened
				cout<<"select error happened!"<<ret<<endl;
			}
			else if(ret == 0)
			{
				//time out
				cout<<"select time out"<<endl;
			}
			else
			{
				char buffer[4096];
				sockaddr_in addr;
				socklen_t len = sizeof(addr);
				iter = fds_idel.begin();
				while(iter != fds_idel.end())
				{
					cout<<"socket "<<*iter<<" has data!"<<endl;
					if(FD_ISSET(*iter, &set))
					{
						int recvlen = recvfrom(*iter, buffer, 4096, 0, (sockaddr*)&addr, &len);
						BufferDump(buffer,recvlen);
						if(addr.sin_addr.s_addr == server_addr.sin_addr.s_addr && addr.sin_port == server_addr.sin_port)
						{
							cout<<"recieve protocol from server:"<<Protocol::PeekType(buffer, 4096)<<endl;
							//so we talk to him
							switch(Protocol::PeekType(buffer, 4096))
							{
							case MakeHoleRequest::TYPE:
							{
								MakeHoleRequest req;
								req.Read(buffer, recvlen);
								cout<<"hole request from "<<req.name<<" with addr:"<<req.ip<<":"<<req.port<<endl;
								sockaddr_in addr_peer;

								addr_peer.sin_addr.s_addr = inet_addr(req.ip);
								addr_peer.sin_port = htons(req.port);
								char hole = 'F';
								sendto(*iter, &hole, 1, 0, (sockaddr*)&addr_peer, sizeof(addr_peer));
								MakeHoleAgree agree;
								strcpy(agree.ip, req.ip);
								agree.port = req.port;
								strcpy(agree.name, name);
								char* end = agree.Write(buffer, buffer + 4095);
								sendto(*iter, buffer, end-buffer, 0, (sockaddr*)&server_addr, sizeof(server_addr));
								cout<<"confirm hole to server"<<endl;
								BufferDump(buffer, end-buffer);
								OnAddPeer(*iter, addr_peer, req.name); 
								iter = fds_idel.erase(iter);
								continue;
							}
								break;
							case QueryPeerReProtocol::TYPE:
							{
								QueryPeerReProtocol p;
								p.Read(buffer, recvlen);
								sockaddr_in addr_peer;
								addr_peer.sin_addr.s_addr = inet_addr(p.ip);
								addr_peer.sin_port = htons(p.port);
								cout<<"successfully connected "<<p.ip<<":"<<p.port<<endl;
								char hello[256 + 100] = "hello, thanks for allow my visiting, I'm ";
								strcat(hello, name);
								sendto(*iter, hello, strlen(hello), 0, (sockaddr*)&addr_peer, sizeof(addr_peer));
								OnAddPeer(*iter, addr_peer, p.name); 
								cout<<p.name<<" allowed your visit"<<endl;
								iter = fds_idel.erase(iter);
								continue;
							}
								break;
							case AddPeerResultProtocol::TYPE:
							{
								AddPeerResultProtocol p;
								p.Read(buffer, recvlen);
								if(p.retcode == 0)
								{
									cout<<"add peer success!id="<<p.id<<endl;
									session_id = p.id;
								}
								else
								{
									cout<<"add peer failed at "<<p.retcode<<endl;
								}
							}
								break;
							}
						}
					}
					else if(FD_ISSET(*iter, &set_error))
					{
						//error happend
						recvfrom(*iter, buffer, 102400, 0, (sockaddr*)&addr, &len);
						cout<<"error happend with "<<inet_ntoa(addr_bind_map[*iter].sin_addr)<<":"<<ntohs(addr_bind_map[*iter].sin_port)<<endl;
					}
					iter++;
				}
			}
		}
	}

	void HandleInUseFD(long msec)
	{
		if(fds_inuse.size() > 0)
		{
			timeval tv;
			int maxFd = 0;
			tv.tv_sec = msec / 1000;
			tv.tv_usec = msec % 1000 * 1000;
			fd_set set;
			fd_set set_error;
			FD_ZERO(&set_error);
			FD_ZERO(&set);
			vector<int>::iterator iter = fds_inuse.begin();
			while(iter != fds_inuse.end())
			{
				FD_SET(*iter, &set);
				FD_SET(*iter, &set_error);
				if(*iter > maxFd)
					maxFd = *iter;
				cout<<"put in inuse socket:"<<*iter<<endl;
				iter++;
			}
			int ret = select(maxFd+1, &set, NULL, &set_error, &tv);
			if(ret < 0)
			{
				//error happened
			}
			else if(ret == 0)
			{
				//time out
			}
			else
			{
				char buffer[102400];
				sockaddr_in addr;
				socklen_t len = sizeof(addr);
				iter = fds_inuse.begin();
				while(iter != fds_inuse.end())
				{
					if(FD_ISSET(*iter, &set))
					{
						int recvlen = recvfrom(*iter, buffer, 102400, 0, (sockaddr*)&addr, &len);
						cout<<"data from "<<inet_ntoa(addr.sin_addr)<<":"<<ntohs(addr.sin_port)<<endl;
						BufferDump(buffer, recvlen);
						if(addr.sin_port == addr_bind_map[*iter].sin_port)
						{
							if(listener)
								listener->OnRecieveData(socket_peer_map[*iter].c_str(), buffer, recvlen);
							//so we talk to him
							buffer[recvlen] = 0;
							cout<<buffer<<endl;
							if(strcmp(buffer, "bye!~") == 0)
							{
								sendto(*iter, "bye!~", 5,0, (sockaddr*)&addr, len);
								OnDelPeer(*iter);
								iter = fds_inuse.erase(iter);
								cout<<"peer disconnected!"<<endl;
								continue;
							}
						}
					}
					else if(FD_ISSET(*iter, &set_error))
					{
						//error happend
						recvfrom(*iter, buffer, 102400, 0, (sockaddr*)&addr, &len);
						cout<<"error happend with "<<inet_ntoa(addr_bind_map[*iter].sin_addr)<<":"<<ntohs(addr_bind_map[*iter].sin_port)<<endl;
					}
					iter++;
				}
			}
		}
	}

	void OnAddPeer(int sock, sockaddr_in addr, string name)
	{
		addr_bind_map[sock] = addr;
		peer_socket_map[name] = sock;
		socket_peer_map[sock] = name;
		fds_inuse.push_back(sock);
		if(listener)
			listener->OnAddPeer(name.c_str());
	}

	void OnDelPeer(int sock)
	{
		addr_bind_map.erase(sock);
		map<string, int>::iterator iter = peer_socket_map.begin();
		while(peer_socket_map.end() != iter)
		{
			if(iter->second == sock)
				iter = peer_socket_map.erase(iter);
			else
				iter++;
		}
		fds_idel.push_back(sock);
		if(listener)
			listener->OnDelPeer(socket_peer_map[sock].c_str());
		socket_peer_map.erase(sock);
	}
private:
	P2PClientListener* listener;
	long session_id;
	sockaddr_in server_addr;
	map<int, sockaddr_in> addr_bind_map;
	map<string, int> peer_socket_map;
	map<int, string> socket_peer_map;
	char name[256];
	vector<int> fds_inuse;
	vector<int> fds_idel;
};
