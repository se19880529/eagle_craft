#include <iostream>

using namespace std;
int partion(int* arr, int start, int end)
{
	int begin = start;
	start++;
	while(end >= start)
	{
		while(end >= start && arr[start] <= arr[begin])
			start++;
		while(end >= start && arr[end] > arr[begin])
			end--;
		if(end > start)
		{
			int s = arr[start];
			arr[start] = arr[end];
			arr[end] = s;
		}
	}
	end = arr[begin];
	arr[begin] = arr[start - 1];
	arr[start - 1] = end;
	return start - 1;
}

int seed = 0xffaa23af;
int random(int x, int y)
{
	seed = (seed<<7)+(seed>>7);
	if(seed < 0)
		seed = -seed;
	return x + seed % (y-x+1);
}
void qsort(int* arr, int start, int end)
{
	if(end > start)
	{
		int flag = random(start, end);
		int t = arr[flag];
		arr[flag] = arr[start];
		arr[start] = t;
		flag = partion(arr, start, end);
		qsort(arr, start, flag - 1);
		qsort(arr, flag + 1, end);
	}
}
int main()
{
	int arr[1024];
	int n = 0;
	int v = 0;
	cin>>v;
	while(v != 0)
	{
		arr[n++] = v;
		cin>>v;	
	}
	qsort(arr, 0, n-1);
	for(int i = 0; i < n; i++)
		if(i!=n-1)
			cout<<arr[i]<<",";
		else
			cout<<arr[i]<<endl;
	return 0;
}
