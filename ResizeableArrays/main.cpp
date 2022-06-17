// ResizeableArrays.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <vector>
#include "HAT.h"
#include "ResizeableArray.h"
#include "TarjanZwick.h"
#include <cassert>

using namespace std;

const constexpr int MAXN = 10000;

TarjanZwick<int> arr[MAXN];

/*
* Input is of the form:
* <n=number of arrays> <m=number of operations>
* then m operations follow of the forms
* append:	1 <whichArray> <item>
* pop:		2 <whichArray>
* print:	3 <whichArray> <arrayIndex>
*/

int main()
{
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < n; i++)
	{
		int operation, whichArray;
		cin >> operation >> whichArray;
		switch (operation)
		{
		case 1:
			int item;
			cin >> item;
			arr[whichArray].push_back(item);
			break;
		case 2:
			arr[whichArray].pop_back();
			break;
		case 3:
			uint64_t arrayIndex;
			cin >> arrayIndex;
			cout << arr[whichArray][arrayIndex] << endl;
			break;
		default:
			return 1;
		}
	}

	return 0;
}
