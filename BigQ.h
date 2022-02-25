#ifndef BIGQ_H
#define BIGQ_H
#include <pthread.h>
#include <iostream>
#include <queue>
#include "Pipe.h"
#include "File.h"
#include "Record.h"

using namespace std;

class BigQ {

public:

	BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
	~BigQ ();

};

//Class run represent run used for merging
class Run {

public:
	Run(File* file, int startPage, int runLength);
	int UpdateTopRecord();
	Record *topRecord; 

private: 
	File* fileBase;
	Page bufferPage;
	int startPage;
	int runLength;
	int curPage;
};

class RecordComparer {

public:
	bool operator () (Record* left, Record* right);
	RecordComparer(OrderMaker *order);

private:
	OrderMaker *order;

};

class RunComparer {

public:
	bool operator () (Run* left, Run* right);
	RunComparer(OrderMaker *order);

private:
	OrderMaker *order;

};

typedef struct {
	
	Pipe *in;
	Pipe *out;
	OrderMaker *order;
	int runlen;
	
} Payload;

void* WorkerThread(void* arg);

void* recordQueueToRun(priority_queue<Record*, vector<Record*>, RecordComparer>& recordQueue, 
    priority_queue<Run*, vector<Run*>, RunComparer>& runQueue, File& file, Page& bufferPage, int& pageIndex);

#endif
