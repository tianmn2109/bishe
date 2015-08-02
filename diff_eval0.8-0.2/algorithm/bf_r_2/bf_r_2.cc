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

vector<TsHandled> vv;
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
	vector<OutTs> resOts(ts.size());
	int each = 0;
	int left = 0;
	int times = 1;
        //sort(v.begin(), v.end());
	CAL:
	bool hasSolution = false;
	double minEval = DBL_MAX;
	for (int i = 0; i < 10000; i ++) {
		shuffle(v);
		vv.clear();
		vv = v;
		int l = left;
		for (int j = 0; j < vv.size(); j ++) {
			vv[j].occupy += each;
			if (l > 0) {
				vv[j].occupy ++;
				l --;
			}
		}
		bin.clear();
  		if (bin.findSolution(vv, 0, ots)) {
//			cout << "---------------------" << endl;
//			bin.outPutTable();

			hasSolution = true;
			double eval = bin.evaluate();
			if (eval < minEval) {
				minEval = eval;
				res = bin;
				resOts = ots;
			}
		}
	}
	if (hasSolution) {
		//cout << "minEval value = " << minEval;
		//res.outPutTable();
		if (times == 1) {
			int whiteSpaceLine = res.spaceNum();
			each = whiteSpaceLine / v.size();
			left = whiteSpaceLine % v.size();
			times ++;
			goto CAL;
		}
		if (times == 2) {
			for (int i = 0; i < resOts.size(); i ++) {
				for (int j = 0; j < v.size(); j ++) 
					if (resOts[i].id == v[j].id) {
						resOts[i].occupy = v[j].occupy;
						break;
					}
			}
			vector<vector<int> > t(res.table.size(), vector<int>(res.table[0].size(), 0));
			for (int i = 0; i < resOts.size(); i ++) {
				for (int k = resOts[i].offset; k < t.size(); k += resOts[i].epochPeroid)
					for (int s = resOts[i].slot; s < resOts[i].occupy + resOts[i].slot; s ++)
						t[k][s] = resOts[i].id;
			}
		        //for (int i = 0; i < t.size(); i ++) {
                        	//for (int j = 0; j < t[0].size(); j ++) {
                      	          //cout << t[i][j] << " ";
       		                //}
                        //cout << endl;
	                //}
			res.table = t;
			//res.outPutTable();
			res.alpha = 0.8;
			res.beta = 0.2;
			res.adjuest();
			//res.outPutTable();
			cout << "minEval value = " << res.evaluate();
	
		}
		
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

