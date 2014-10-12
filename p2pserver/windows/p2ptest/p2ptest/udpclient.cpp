#include "udpclient.h"
int main()
{
	char server_ip[256];
	int server_port;
	cout<<"ip:";
	cin>>server_ip;
	cout<<"port;";
	cin>>server_port;
	P2PClient client(server_ip, server_port);
	char text[102400];
	cout<<">>";
	cin.getline(text, 102400);
	while(strcmp(text, "quit") != 0)
	{
		if(text[0] == 's')	//send
		{
			char* name = text+2, *txt;
			int ind = (int)(strstr(name, " ")-name);
			if(ind > 0)
			{
				txt = name + ind+1;
				name[ind] = 0;
				cout<<"send to peer "<<name<<":"<<txt<<endl;
				client.SendToPeer(name, txt);
			}
		}
		else if(text[0] == 'c')
		{
			if(client.QueryPeerInfo(text+2))
				cout<<"sent query peer " <<(text+2)<<"  to server"<<endl;
			else
				cout<<"query faled, please add peer first!"<<endl;
		}
		else if(text[0] == 'a')
		{
			cout<<"added peer!"<<endl;
			cout<<"new peer:"<<client.AddPeer()<<endl;
		}
		else if(text[0] == 'b')
		{
			client.SendToPeer(text+2, "bye!~");
			cout<<"peer byed!"<<endl;
		}
		else if(text[0] == 'r')
		{
			client.HandleIdelFD(1000);
			client.HandleInUseFD(1000);
		}
		else if(text[0] == 'n')
		{
			cout<<"text"<<text<<endl;
			client.SetName(text + 2);
		}
		cout<<">>";
		cin.getline(text, 102400);
	}

	return 1;
}
