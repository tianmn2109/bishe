#include <fstream>
#include <cfloat>
#include <climits>
#include <cstdio>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "bin.h"
using namespace std;

int gcd(int m, int n) {
        if (m < n)
                swap(m, n);
        while (n > 0) {
                int r = m % n;
                m = n;
                n = r;
        }
        return m;
}

int MCM(int m, int n) {
        int r = gcd(m, n);
        long long mm = (long long)m;
        long long nn = (long long)n;
        long long rr = (long long)r;
        long long ret = (mm * nn) / rr;
        return (int) ret;
}

void shuffle(vector<TsHandled> &v) {
	for (int i = v.size() - 1; i > 0; i --) {
		int index = rand() % (i + 1);
		swap(v[i], v[index]);
	}
}

int schedule(vector<TsChannel> &ts, int slotTime, int slotNum) {
        vector<TsHandled> v(ts.size());
        for (int i = 0; i < ts.size(); i ++) {
                v[i].id = ts[i].id;
                v[i].epochPeroid = ts[i].peroid / (slotTime * slotNum);
                v[i].occupy = (int)ceil((double)ts[i].sendTime / (double)slotTime);
        }
        int epochNum = 1;
        for (int i = 0; i < v.size(); i ++) {
                epochNum = MCM(epochNum, v[i].epochPeroid);
        }
	srand((int)time(NULL));
	Bin bin(epochNum, slotNum);
	Bin res(epochNum, slotNum);
	vector<OutTs> ots(ts.size());
        //sort(v.begin(), v.end());
	bool hasSolution = false;
	double minEval = DBL_MAX;
	for (int i = 0; i < 10000; i ++) {
		shuffle(v);
		bin.clear();
  		if (bin.findSolution(v, 0, ots)) {
//			cout << "---------------------" << endl;
//			bin.outPutTable();

			hasSolution = true;
			double eval = bin.evaluate();
			if (eval < minEval) {
				minEval = eval;
				res = bin;
			}
		}
	}
	if (hasSolution) {
		//cout << "minEval value = " << minEval;
		//res.outPutTable();
		res.adjuest();
		//res.outPutTable();
		res.alpha = 0.9;
		res.beta = 0.1;
		cout << "minEval value = " << res.evaluate();
	}
	else 
		cout << "minEval value = " << -200;
        return 0;
}


int main(int argc, char ** argv)
{
        vector<TsChannel> vts;
/*        TsChannel t1;
        t1.id = 1;
        t1.peroid = 10;
        t1.sendTime = 1;
        TsChannel t2;
        t2.id = 2;
        t2.peroid = 15;
        t2.sendTime = 2;
        TsChannel t3;
        t3.id = 3;
        t3.peroid = 20;
        t3.sendTime = 3;
*/
	ifstream f(argv[1]);
        int slotTime = 1;
        int slotNum = 5;
	f >> slotTime >> slotNum;	
	while (!f.eof()) {
		TsChannel ts;
		f >> ts.id >> ts.peroid >> ts.sendTime;
		if (f.fail())
			break;
		vts.push_back(ts);
	}
	
	f.close();
	
//        vts.push_back(t1);
//        vts.push_back(t2);
//        vts.push_back(t3);
	clock_t start = clock();	
        schedule(vts, slotTime, slotNum);
	clock_t finish = clock();
	cout << " time = " << (double)(finish - start) / CLOCKS_PER_SEC << endl;

        return 0;
}

