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
	map<string, int> name;
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
*/		int score = num;
		if (v[0].val >= 0) {
			name[al[v[0].index]] += score;
			for (int i = 1; i < num; i ++) {
				if (v[i].val == v[i - 1].val)
					name[al[v[i].index]] += score;
				else {
					score = num - i;
					name[al[v[i].index]] += score;
				}
			}
		}
	}
	f.close();
	for (int i = 0; i < al.size(); i ++)
		cout << al[i] << "        " ;
	cout << endl;
	for (int i = 0; i < al.size(); i ++)
		cout << name[al[i]] - 12 << "        " ;   // 因为不知道为什么多读了一行 而且哪行全是0
	cout << endl;
	
	return 0;
}
