#include <iostream>

void kmp_gen_next(int next[], char* str, int len)
{
	int p = 0;
	int f = -1;
	next[0] = -1;
	while(p < len)
	{
		if(f < 0 || str[f] == str[p])
		{
			f++;
			p++;
			next[p] = f;
		}
		else
		{
			f = next[f];
		}
	}
}

int kmp_find(int next[], char* pattern, int lenpattern, char* str, int startstr, int endstr)
{
	int p = startstr;
	int patternp = 0;
	while(patternp < lenpattern && p <= endstr)
	{
		if(patternp == -1 || str[p] == pattern[patternp])
		{
			p++;
			patternp++;
		}
		else
		{
			patternp = next[patternp];
		}
	}
	if(patternp == lenpattern)
		return p-lenpattern;
	else
		return -1;
}

using namespace std;
int main()
{
	while(1)
	{
		char input[255];
		int next[255];
		cin>>input;
		kmp_gen_next(next, input, 255);
		while(1)
		{
			char str[1280];
			cin>>str;
			if(strcmp(str, "end") == 0)
				break;
			cout<<kmp_find(next, input, strlen(input), str, 0, strlen(str)-1)<<endl;
		}
	}
	return 0;
}
