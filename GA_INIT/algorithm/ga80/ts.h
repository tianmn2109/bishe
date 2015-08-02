#include <vector>
#include <iostream>
using namespace std;

// input struct
class TsChannel {
public:
        int id;
        int peroid;
        int sendTime;
};

class TsHandled {
public:
        int id;
        int epochPeroid;
        int occupy;
        bool operator < (const TsHandled &t) const {
                return epochPeroid < t.epochPeroid; 
        }
};

class OutTs {
public:
        int id;
        int slot;
        int offset;
        int occupy;
        int epochPeroid;
	bool operator < (const OutTs & t) {
		return id < t.id;
	}
};


