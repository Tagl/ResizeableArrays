// ResizeableArrays.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <vector>
#include "HAT.h"
#include "ResizeableArray.h"
#include "TarjanZwick.h"
#include <cassert>

using namespace std;

int main()
{
	vector<int> vec{};
	TarjanZwick<int> arr{};
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		//int x;
		//cin >> x;
		vec.push_back(i);
		arr.push_back(i);
	}

	for (uint64_t i = 0; i < vec.size(); i++)
	{
		cout << vec[i] << " ";
	}
	cout << endl;

	for (uint64_t i = 0; i < arr.size(); i++)
	{
		cout << arr[i] << " ";
		assert(arr[i] == vec[i]);
	}

	cout << endl;

	/*while (arr.size() > 0)
	{
		arr.pop_back();
	}*/

	return 0;
}
