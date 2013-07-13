// POJ3133.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
class Map
{
private:
	char _len;
	char _map[9][9];
	char _map_height;
	char _map_width;
	char a[2][2];
	char b[2][2];
	bool aset;
	bool bset;
	static char _nextStep[4][2];
public:
	Map()
	{
	}
	void Resize(int row, int col)
	{
		_len = 100;
		_map_height = row;
		_map_width = col;
		aset = false;
		bset = false;
		for(int r = 0; r < _map_height; r++)
			for(int c = 0; c < _map_width; c++)
			{
				_map[r][c] = 0;
			}
	}
	void Set(int r, int c, int v)
	{
		switch(v)
		{
		case 2:
			if(!aset)
			{
				AddA(r, c);
				aset = true;
			}
			else
			{
				AddADst(r, c);
			}
			break;
		case 3:
			if(!bset)
			{
				AddB(r, c);
				bset = true;
			}
			else
			{
				AddBDst(r, c);
			}
			break;
		default:
			_map[r][c] = v;
		}
	}
	void AddB(int row, int col)
	{
		b[0][0] = row;
		b[0][1] = col;
	}
	void AddBDst(int row, int col)
	{
		b[1][0] = row;
		b[1][1] = col;
	}
	void AddA(int row, int col)
	{
		a[0][0] = row;
		a[0][1] = col;
	}
	void AddADst(int row, int col)
	{
		a[1][0] = row;
		a[1][1] = col;
	}
	int GetMin()
	{
		_map[a[0][0]][a[0][1]] = -1;
		_map[b[0][0]][b[0][1]] = -1;
		Search(a[0][0], a[0][1], 0, b[0][0], b[0][1], 0);
	//	if(_len >= 100)
	//		_len = 0;
		return _len;
	}
private:
	void Search(char ar, char ac, char astep, char br, char bc, char bstep)
	{
		char nextar, nextac, nextbr, nextbc;
		bool amove = false;
		bool bmove = false;
		bool areach = false;
		bool breach = false;
		if(ar == a[1][0] && ac == a[1][1])
		{
			areach = true;
		}
		if(br == b[1][0] && bc == b[1][1])
		{
			breach = true;
		}
		if(areach && breach && astep + bstep < _len)
		{
			_len = astep + bstep;
		}
		else
		{
			for(int i = 0; i < 4 && !areach; i++)
			{
				nextar = ar + _nextStep[i][0];
				nextac = ac + _nextStep[i][1];
				if(nextar >= 0 && nextar < _map_height && nextac >= 0 && nextac < _map_width &&
					_map[nextar][nextac] == 0)		//not a obstacle
				{
					amove = true;
					//bring a to next
					_map[nextar][nextac] = -1;
					bmove = false;
					for(int j = 0; j < 4 && !breach; j++)
					{
						nextbr = br + _nextStep[j][0];
						nextbc = bc + _nextStep[j][1];
						if(nextbr >= 0 && nextbr < _map_height && nextbc >= 0 && nextbc < _map_width &&
							_map[nextbr][nextbc] == 0)		//not a obstacle
						{
							//bring b to next
							_map[nextbr][nextbc] = -1;
							//moved
							Search(nextar, nextac, astep + 1, nextbr, nextbc, bstep + 1);
							_map[nextbr][nextbc] = 0;
							bmove = true;
						}
					}
					if(!bmove)
					{
						//only a moved
						Search(nextar, nextac, astep + 1, br, bc, bstep);
					}
					
					_map[nextar][nextac] = 0;
				}
				
			}
			if(!amove)
			{
				for(int j = 0; j < 4 && !breach; j++)
				{
					nextbr = br + _nextStep[j][0];
					nextbc = bc + _nextStep[j][1];
					if(nextbr >= 0 && nextbr < _map_height && nextbc >= 0 && nextbc >= 0 && nextbc < _map_width &&
						_map[nextbr][nextbc] == 0)		//not a obstacle
					{
						//bring b to next
						_map[nextbr][nextbc] = -1;
						//only b moved
						Search(ar, ac, astep, nextbr, nextbc, bstep + 1);
						_map[nextbr][nextbc] = 0;
					}
				}
			}
		}
	}
};

char Map::_nextStep[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main(int argc, char* argv[])
{
	int row, col;
	Map m;
	while(1)
	{
		cin>>row>>col;
		m.Resize(row, col);
		for(int c = 0; c < col; c++)
			for(int r = 0; r < row; r++)
			{
				int v;
				cin>>v;
				m.Set(r, c, v);
			}
		int len = m.GetMin();
		cout<<len<<endl;
	}
	return 0;
}

