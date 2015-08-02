#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;

struct node {
public:
	double val;
	int index;
	node(double v, int i) {val = v; index = i;}
	bool operator < (const node & n) {
		if (val < 0)
			return false;
		if (n.val < 0)
			return true;
		return val < n.val;
	}
};

int main(int argc, char **argv) {
	ifstream f(argv[1]);
	int num = atoi(argv[2]);
	vector<string> al;
	map<string, double> name;
	for (int i = 0; i < num; i ++) {
		string n;
		f >> n;
		al.push_back(n);
		name[n] = 0;
	}
	while (!f.eof()) {
		vector<node> v;
		for (int i = 0; i < num; i ++) {
			double temp = 0;
			f >> temp;
			v.push_back(node(temp, i));
		}
		sort(v.begin(), v.end());
/*		for (int i = 0 ; i < v.size(); i ++)
			cout << v[i].val << " ";
		cout << endl;
*/		if (v[0].val >= 0) {
			for (int i = 1; i < num; i ++) {
				if (v[i].val < 0)
					break;
				name[al[v[i].index]] += fabs(v[i].val - v[0].val);
			}
		}
	}
	f.close();
	for (int i = 0; i < al.size(); i ++)
		cout << al[i] << "        " ;
	cout << endl;
	for (int i = 0; i < al.size(); i ++)
		cout << name[al[i]] << "        " ;   // 因为不知道为什么多读了一行 而且哪行全是0
	cout << endl;
	
	return 0;
}
