#include <iostream>
using namespace std;


int get_top_k(int k, int arr[], int begin, int end)
{
	if(k < 0 || k > end - begin)
		return -1;
	else
	{
		int head = end, start = begin;
		int flag = arr[begin];
		while(end >= begin)
		{
			while(end >= begin && arr[end] >= flag)
				end--;
			while(end >= begin && arr[begin] < flag)
				begin++;
			if(end > begin)
			{
				int v = arr[end];
				arr[end] = arr[begin];
				arr[begin] = v;
			}
		}
		if(head - end - 1  == k)
			return end+1;
		else if(head - end - 1  > k)
			return get_top_k(k, arr, end + 2, head);
		else
			return get_top_k(k - (head - end),arr, start, end); 
	}
}

int get_top_k_heap(int k, int arr[], int begin, int end)
{
	
}

int main()
{
	int arr[1024];
	for(int i = 0; i < sizeof(arr)/sizeof(int); i++)
	{
		arr[i] = i;
	}
	while(1)
	{
		int k;
		cin>>k;
		cout<<get_top_k(k, arr, 0, 1023)<<endl;
	}
}
