#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#define OPER_CLOSURE '*'
#define OPER_CHANGEMEAN '\\'
#define OPER_CLOSURE_REAL '+'
#define OPER_CAT '&'
#define OPER_LEFT_BRANCKET '('
#define OPER_RIGHT_BRANCKET ')'
#define OPER_OR '|'
#define OPER_DOT '.'
#define EMPTY '\0'
#define WORD '\1'
#define NUMBER '\2'
#define READABLE_CHAR '\3'

using namespace std;

struct Edge
{
	int start;
	int end;
	char str;
};
struct StatusNode
{
	int id;
	vector<Edge> edges;
};
struct RegexNFA
{
	int start;
	int end;	
};
class Graph
{
private:
	StatusNode* nodes;
	int nextIndex;
public:
	Graph(int size)
	{
		nodes = new StatusNode[size];
		nextIndex = 0;
	}
	void AddEdge(int id1, int id2, char ch)
	{
		Edge e;
		e.start = id1;
		e.end = id2;
		e.str = ch;
		GetNode(id1).edges.push_back(e);
	}
	StatusNode& GetNode(int id)
	{
		return nodes[id];
	}
	int CreateNode()
	{
		nodes[nextIndex].id = nextIndex;
		return nextIndex++;
	}
};

class Regex
{
public:
	Regex(const char* regex)
	{
		Create(regex);
	}
	const char* GetRegexString()
	{
		return _regex;
	}
	bool Match(const char* str)
	{
		vector<int> status;
		bool included[256];
		for(int i = 0; i < 256; i++)
			included[i] = false;
		queue<int> q;
		q.push(start);
		while(!q.empty())
		{
			vector<Edge>::iterator iter = _graph->GetNode(q.front()).edges.begin();
			while(iter != _graph->GetNode(q.front()).edges.end())
			{
				if(iter->str == EMPTY && !included[iter->end])
				{
					q.push(iter->end);
					status.push_back(iter->end);
					included[iter->end] = true;
				}
				iter++;
			}
			q.pop();
		}

		status.push_back(start);
		
		while(*str)
		{
			status = Move(status, *str);
			str++;
		}
		for(int i = 0; i < status.size(); i++)
		{
			if(status[i] == end)
				return true;
		}
		return false;
	}
	void Print()
	{
		bool visited[256];
		for(int i = 0; i < 256; i++)
			visited[i] = false;
		queue<int> tovisit;
		tovisit.push(start);
		cout<<"begin:"<<start<<",end:"<<end<<endl;
		while(!tovisit.empty())
		{
			visited[_graph->GetNode(tovisit.front()).id] = true;
			cout<<"node "<<_graph->GetNode(tovisit.front()).id<<" connected to:";
			vector<Edge>::iterator iter = _graph->GetNode(tovisit.front()).edges.begin();
			while(iter != _graph->GetNode(tovisit.front()).edges.end())
			{
				cout<<iter->end<<"("<<iter->str<<") ";
				if(!visited[iter->end])
					tovisit.push(iter->end);
				iter++;
			}
			tovisit.pop();
			cout<<endl;
		}
	}


	void Create(const char* regex)
	{
		if(NULL != _graph)
		{
			delete _graph;
		}
		_regex = regex;
		error = 0;
		errorpos = 0;
		_graph = new Graph(strlen(regex) * 2);
		while(!subRegexStack.empty())
			subRegexStack.pop();
		while(!operStack.empty())
			operStack.pop();
		bool lastNeedCat = false;

		while(*regex)
		{
			char ch = *regex;
			if(!IsOperator(ch))
			{
				if(lastNeedCat)
					operStack.push(OPER_CAT);
				BasicLink(ch);
				lastNeedCat = true;
			}
			else
			{
				switch(ch)
				{
				case OPER_CHANGEMEAN:
					regex++;
					if(!*regex)
					{
						Error(regex-1, 1);
						regex--;
					}
					else
					{
						if(lastNeedCat)
							operStack.push(OPER_CAT);
						switch(*regex)
						{
						case 'w':
							BasicLink(WORD);
							break;
						case 'd':
							BasicLink(NUMBER);
							break;
						default:
							BasicLink(*regex);
							break;		
						}
						lastNeedCat = true;
					}
					break;
				case OPER_DOT:
					if(lastNeedCat)
						operStack.push(OPER_CAT);
					BasicLink(READABLE_CHAR);
					lastNeedCat = true;
					break;
				case OPER_CLOSURE:
					Closure(false);
					break;
				case OPER_CLOSURE_REAL:
					Closure(true);
					break;
				case OPER_LEFT_BRANCKET:
					if(lastNeedCat)
						operStack.push(OPER_CAT);
					lastNeedCat=false;
					operStack.push(OPER_LEFT_BRANCKET);
					break;
				case OPER_RIGHT_BRANCKET:
					if(operStack.empty())
						Error(regex, 2);
					else
					{
						char oper = operStack.top();
						operStack.pop();
						while(oper != OPER_LEFT_BRANCKET)
						{
							switch(oper)
							{
								case OPER_CAT:
								{
									RegexNFA r = subRegexStack.top();
									subRegexStack.pop();
									RegexNFA s = subRegexStack.top();
									subRegexStack.pop();
									_graph->AddEdge(s.end, r.start, EMPTY);
									r.start = s.start;
									subRegexStack.push(r);
									start = r.start;
									end = r.end;
								}
									break;
								default:
									Error(regex, 1);
									break;

							}
							if(operStack.empty())
								break;
							oper = operStack.top();
							operStack.pop();
						}
						if(oper != OPER_LEFT_BRANCKET)
							Error(regex, 2);
					}
					break;
				}
			}
			regex++;
		}
		
	}

	//1:wrong syntax, 2: not pair left brancket
	void Error(const char* pos, int err)
	{
		error = err;
		errorpos = pos - _regex;
	}

	int GetError(){ return error; }
	int GetErrorPos(){ return errorpos;}


	void BasicLink(char ch)
	{
		StatusNode& node1 = _graph->GetNode(_graph->CreateNode());
		StatusNode& node2 = _graph->GetNode(_graph->CreateNode());
		_graph->AddEdge(node1.id, node2.id, ch);
		RegexNFA sub;
		sub.start = node1.id;
		sub.end = node2.id;
		subRegexStack.push(sub);
		start = node1.id;
		end = node2.id;
	}

	bool Closure(bool real)
	{
		if(subRegexStack.empty())
			return false;
		RegexNFA last = subRegexStack.top();
		subRegexStack.pop();
		StatusNode& node1 = _graph->GetNode(last.start);
		StatusNode& node2 = _graph->GetNode(last.end);
		if(real)
		{
			StatusNode& begin = _graph->GetNode(_graph->CreateNode());
			_graph->AddEdge(begin.id, node1.id, EMPTY);
			_graph->AddEdge(node2.id, node1.id, EMPTY);
			last.start = begin.id;
			last.end = node2.id;
		}
		else
		{
			StatusNode& begin = _graph->GetNode(_graph->CreateNode());
			StatusNode& end = _graph->GetNode(_graph->CreateNode());
			_graph->AddEdge(begin.id, node1.id, EMPTY);
			_graph->AddEdge(begin.id, end.id, EMPTY);
			_graph->AddEdge(node2.id, end.id, EMPTY);
			_graph->AddEdge(node2.id, node1.id, EMPTY);
			last.start = begin.id;
			last.end = end.id;
		}
		subRegexStack.push(last);
		start = last.start;
		end = last.end;
		return true;
	}

	bool IsOperator(char c)
	{
		return c=='+'||c=='*'||c=='('||c==')'||c=='|'||c=='\\'||c=='.';
	}
private:
	bool IsType(char ch, char type)
	{
		switch(type)
		{
			case WORD:
				return (ch<='Z' && ch>='A') || (ch<='z' && ch>='a');
			case NUMBER:
				return (ch<='9' && ch>='0');
			case READABLE_CHAR:
				return (ch<='Z' && ch>='A') || (ch<='z' && ch>='a') || (ch<='9'&&ch>='0');
			default:
				return ch == type;
		}
	}
	vector<int> Move(vector<int> old, char charactor)
	{
		bool included[256];
		for(int i = 0; i < 256; i++)
			included[i] = false;
		vector<int> res;
		vector<int>::iterator iter = old.begin();
		while(iter != old.end())
		{
			vector<Edge>::iterator edge = _graph->GetNode(*iter).edges.begin();
			while(edge != _graph->GetNode(*iter).edges.end())
			{
				if(IsType(charactor,edge->str))
				{
					included[edge->end] = true;
					res.push_back(edge->end);
					queue<int> tovisit;
					tovisit.push(edge->end);
					while(!tovisit.empty())
					{
						vector<Edge>::iterator edge2 = _graph->GetNode(tovisit.front()).edges.begin();
						while(edge2 != _graph->GetNode(tovisit.front()).edges.end())
						{
							if(edge2->str == EMPTY && !included[edge2->end])
							{
								res.push_back(edge2->end);
								tovisit.push(edge2->end);
								included[edge2->end] = true;
							}
							edge2++;
						}
						tovisit.pop();
					}
				}
				edge++;
			}
			iter++;
		}
		return res;
	}

	stack<RegexNFA> subRegexStack;
	stack<char>	operStack;
	const char* _regex;
	Graph* _graph;
	int start;
	int end;
	int error;
	int errorpos;
};

int main()
{
	char pattern[256];
	while(1)
	{
		cout<<"enter regex:";
		cin>>pattern;
		Regex r(pattern);
		if(r.GetError())
		{
			switch(r.GetError())
			{
				case 1:
					cout<<"syntax error at substring:'"<<&pattern[r.GetErrorPos()]<<"'"<<endl;
					break;
				case 2:
					cout<<"brancket at '"<<&pattern[r.GetErrorPos()]<<"' cannot be paired"<<endl;
					break;
			}
		}
		else
		{
			r.Print();
			char inputs[128] = {0};
			cout<<"input string:";
			cin>>inputs;
			while(strcmp(inputs, "quit") != 0)	
			{
				if(r.Match(inputs))
				{
					cout<<"Yes!"<<endl;
				}
				else
				{
					cout<<"No!"<<endl;
				}
				cout<<"input string:";
				cin>>inputs;
			}
		}
	}	
}


