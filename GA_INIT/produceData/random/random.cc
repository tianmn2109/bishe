#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
using namespace std;

int main(int argc, char **argv)
{
	if (argc < 7) {
		cerr << "Usage: [proName] [outfilename] [ChannelNumber] [slot] [minPeroid] [maxPeroid] [sendOccupy]" << endl;
	}
	int channelNumber;
	int slot;
	int minPeroid;
	int maxPeroid;
	int sendOccupy;
	
	channelNumber = atoi(argv[2]);
	slot = atoi(argv[3]);
	minPeroid = atoi(argv[4]);
//	cout << minPeroid << endl;
	maxPeroid = atoi(argv[5]);
	sendOccupy = atoi(argv[6]);
	int range = maxPeroid - minPeroid + 1;

	ofstream outfile(argv[1]);
	srand((int)time(NULL));
	int num = rand() % channelNumber + 1;
	outfile << slot << " " << minPeroid / slot << endl;
	for (int i = 0; i < num; i ++) {
		outfile << i + 1 << " " << rand() % range / minPeroid * minPeroid + minPeroid << " ";
		outfile << slot * (rand() % sendOccupy + 1) << endl;
	}
	outfile.close();
	

	return 0;
	
	
}
