#include <iostream>
#include <vector>

using namespace std;
class CharSet
{
	public virtual bool IsInCharset(char elem)
	{
		return false;
	}
}

class SingleCharSet : public CharSet
{
	char c;
	SingleCharSet(char ch)
	{
		c = ch;
	}
	public bool IsInCharset(char elem)
	{
		return elem == c;
	}
}

class NumberCharSet : public CharSet
{
	public bool IsInCharset(char elem)
	{
		return elem >= '0' && elem <= '9';
	}
}

class WordCharSet : public CharSet
{
	bool isLower;
	bool ignoreCase;
	WordCharSet(bool low, bool case)
	{
		isLower = low;
		ignoreCase = case;
	}
	public bool IsInCharset(char elem)
	{
		bool in = false;
		if(ignoreCase || !isLower)
		{
			in = (elem <= 'Z' && elem >= 'A');
		}
		if(!in && (ignoreCase || isLower))
		{
			in = (elem <= 'z' && elem >= 'a');
		}
	}
}

class AllCharSet : public CharSet
{
	public bool IsInCharset(char elem)
	{
		return elem != 0;
	}
}

class Status
{
	vector<CharSet> sets;
	vector<int>	nextStatus;
	public int Move(char c)
	{
		vector<CharSet>::iterator iter = sets.begin();
		vector<int>::iterator iterIndex = nextStatus.begin();
		while(iter != sets.end())
		{
			if(iter->IsInCharset(elem))
			{
				return *iterIndex;
			}
			iterIndex++;
			iter++;
		}
		return -1;
	}
	public void AddEdge(CharSet& charset, int status)
	{
		sets.push_back(charset);
		nextStatus.push_back(status);
	}
}

class Regex
{
	vector<Status> statuses;
}
