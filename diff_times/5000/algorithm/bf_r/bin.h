#include <iostream>
#include <utility>
#include <vector>
#include "ts.h"
using namespace std;

class Bin {
public:
	vector<vector<int> > table;
	double alpha;
	double beta;
	int solutionNum;
	Bin() {alpha = 1; beta = 0.0; solutionNum = 0;}
	Bin(int row, int col) {
		alpha = 1.0;
		beta = 0.0;
		solutionNum = 0;
		table.resize(row, vector<int>());
		for (int i = 0; i < row; i ++)
				table[i].resize(col, 0);
	}
	void clear() {
		for (int i = 0; i < table.size(); i ++)
			for (int j = 0; j < table[0].size(); j ++)
				table[i][j] = 0;
	}

	bool canPlace(TsHandled ts, vector<pair<int, int> > &rc) {
        	for (int i = 0; i < ts.epochPeroid; i ++)
               		for (int j = 0; j < table[0].size() - ts.occupy + 1; j ++) {
                       		if (table[i][j] == 0) {
                                	for (int s = i; s < table.size(); s += ts.epochPeroid)
                                        	for (int k = j; k < j + ts.occupy; k ++)
                                                	if (table[s][k] != 0) {
                                                        	goto PLACENEXT;
                                                	}
                                	rc.push_back(make_pair(i, j));
                        	PLACENEXT: ;
                        	}
        	}
	        if (rc.size() > 0)
        	        return true;
      	 	return false;
	}

	void place(TsHandled ts, int row, int col) {
        	for (int i = row; i < table.size(); i += ts.epochPeroid)
                	for (int j = col; j < col + ts.occupy; j ++)
                        	table[i][j] = ts.id;
	}

	void disPlace(TsHandled ts, int row, int col) {
        	for (int i = row; i < table.size(); i += ts.epochPeroid)
                	for (int j = col; j < col + ts.occupy; j ++)
                        	table[i][j] = 0;
	}

	void outPutTable() {
		cout << "Bin table result: " << endl;
		for (int i = 0; i < table.size(); i ++) {
			for (int j = 0; j < table[0].size(); j ++) {
				cout << table[i][j] << " ";
			}
			cout << endl;
		}
	}

        double evaluate() {
                double totalOccupy = 0.0;
                vector<double> lineOccupy(table.size(), 0.0);
                for (int i = 0; i < table.size(); i ++) {
                        for (int j = 0; j < table[0].size(); j ++) {
                                if (table[i][j] != 0)
                                        lineOccupy[i] += 1.0;
                        }
                        totalOccupy += lineOccupy[i];
                }
                double varTotal = 0.0;
                double averageLine = totalOccupy / lineOccupy.size();
                for (int i = 0; i < lineOccupy.size(); i ++) {
                        varTotal += (lineOccupy[i] - averageLine) * (lineOccupy[i] - averageLine);
                }

                double varLine = 0.0;
                for (int i = 0; i < table.size(); i ++) {
                        vector<double> w;
                        int difNum = 0;
                        int whiteNum = 0;
                        int prevNum = INT_MAX;
                        int j = 0;
                        while (j < table[i].size() && table[i][j] == 0)
                                j ++;
                        int start = j;
                        for (; j < table[i].size(); j ++) {
                                if (prevNum == table[i][j]) {
                                        continue;
                                }
                                if (table[i][j] == 0) {
                                        whiteNum ++;
                                }
                                else {
                                        if (j != start)
                                                w.push_back(whiteNum);
                                        whiteNum = 0;
                                        prevNum = table[i][j];
                                }
                        }
                        if ((start != 0 || whiteNum != 0) && w.size() == 0) {
                                if (start != 0)
                                        w.push_back(start);
                                if (whiteNum != 0)
                                        w.push_back(whiteNum);
                        }

                        double sum = 0;
                        for (int k = 0; k < w.size(); k ++)
                                sum += w[k];
                        double var = 0.0;
                        sum /= w.size();
                        for (int k = 0; k < w.size(); k ++)
                                var = var + (sum - w[k]) * (sum - w[k]);
                        varLine += var;
                }

                return this->alpha * varTotal + this->beta * varLine;

        }

	int countRow(int row) {
		int count = 0;
		for (int i = 0; i < this->table[row].size(); i ++)
			if (this->table[row][i] != 0)
				count ++;
		return count;
	}
	bool findSolution(vector<TsHandled> &v, int start, vector<OutTs> &ots) {
        	if (start == v.size()) {                //find a solution
                	//cout << "find a solution" << endl;
                	solutionNum ++;
                	//outPutTable();
			return true;
        	}
        	else {
                        	int row = 0;
                        	int col = 0;
                        	vector<pair<int, int> > rc;
                        	if (canPlace(v[start], rc)) {
					pair<int, int> best = rc[0];
					row = best.first;
					col = best.second;
				//	place(v[start], row, col);
					double tempBest = countRow(row);
				//	disPlace(v[start], row, col);
                                	for (int m = 1; m < rc.size(); m ++) {
						row = rc[m].first;
						col = rc[m].second;
				//		place(v[start], row, col);
						double temp = countRow(row);
						if (temp < tempBest) {
							tempBest = temp;
							best = rc[m];
						}
                                  //      	disPlace(v[start], row, col);
                                	}
                                        	row = best.first;
                                        	col = best.second;
                                        	place(v[start], row, col);
                                        	OutTs oTemp;
                                        	oTemp.id = v[start].id;
                                        	oTemp.slot = col;
                                        	oTemp.offset = row;
                                        	oTemp.occupy = v[start].occupy;
                                        	oTemp.epochPeroid = v[start].epochPeroid;
                                        	ots[start] = oTemp;
                                        	int nextStart = start + 1;
                                                if (findSolution(v, nextStart, ots))
							return true;
                                        	disPlace(v[start], row, col);
                        	}
				return false;
        	}
	}
	void adjuest() {
		vector<vector<int> > newTable(table.size(), vector<int>(table[0].size(), 0));
		int lineSpaceCount = 0;
		vector<pair<int, int> > line;
		int start = 0;
		for (int i = 0; i < table[0].size(); i ++) {
			bool fullSpace = true;
			bool edge = true;
			if (i == table[0].size() - 1) {
				int j = 0;
				for (j = 0; j < table.size(); j ++)
					if (table[j][i] != 0)
						break;
				if (j == table.size())
					lineSpaceCount ++;
				else {
					line.push_back(pair<int, int>(start, i));
				}
				break;
			}
			for (int j = 0; j < table.size(); j ++) {
				if (table[j][i] != 0)
					fullSpace = false;
				if (table[j][i] == 0 && table[j][i + 1] == 0)
					continue;
				if (table[j][i] == table[j][i + 1])
					edge = false;
			}
			if (fullSpace) {
				lineSpaceCount ++;
				start = i + 1;
			}
			else if (edge) {
				line.push_back(pair<int, int>(start, i));
				start = i + 1;
			}
		}
		if (line.size() <= 1)
			return ;
		vector<int> v(line.size() - 1, lineSpaceCount / (line.size() - 1));
		lineSpaceCount = lineSpaceCount % v.size();
		for (int i = 0; i < lineSpaceCount; i ++)
			v[i] ++;
		int k = 0;
		for (int i = 0; i < line.size(); i ++) {
			for (int j = line[i].first; j <= line[i].second; j ++) {
				for (int s = 0; s < newTable.size(); s ++) {
					newTable[s][k] = table[s][j];
				}
				k ++;
			}	
			if (i == v.size())
				break;
			while (v[i] --)
				k ++;
		}
		table.swap(newTable);
	}

};
