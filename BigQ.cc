#include "BigQ.h"

void* workerMain(void* arg) {
    Payload* payload = (Payload*) arg;
    priority_queue<Run*, vector<Run*>, RunComparer> runQueue(payload->order);
    priority_queue<Record*, vector<Record*>, RecordComparer> recordQueue (payload->order);
    vector<Record* > recBuff;
    Record curRecord;

    File file;
    char* fileName = "storage.bin";
    file.Open(0, fileName);

    Page bufferPage;
    int pageIndex = 0;
    int pageCounter = 0;

    while (payload->in->Remove(&curRecord) == 1) {
        Record* tmpRecord = new Record;
        tmpRecord->Copy(&curRecord);
        if (bufferPage.Append(&curRecord) == 0) {
            pageCounter++;
            bufferPage.EmptyItOut();

            if (pageCounter == payload->runlen) {
                recordQueueToRun(recordQueue, runQueue, file, bufferPage, pageIndex);
                recordQueue = priority_queue<Record*, vector<Record*>, RecordComparer> (payload->order);
                pageCounter = 0;
            }

            bufferPage.Append(&curRecord);
        }

        recordQueue.push(tmpRecord);
    }
    if (!recordQueue.empty()) {
        recordQueueToRun(recordQueue, runQueue, file, bufferPage, pageIndex);
        recordQueue = priority_queue<Record*, vector<Record*>, RecordComparer> (payload->order);
    }
    while (!runQueue.empty()) {
        Run* run = runQueue.top();
        runQueue.pop();
        payload->out->Insert(run->topRecord);
        if (run->UpdateTopRecord() == 1) {
            runQueue.push(run);
        }
    }
    file.Close();
    payload->out->ShutDown();
    return NULL;
}

void* recordQueueToRun(priority_queue<Record*, vector<Record*>, RecordComparer>& recordQueue, 
    priority_queue<Run*, vector<Run*>, RunComparer>& runQueue, File& file, Page& bufferPage, int& pageIndex) {

    bufferPage.EmptyItOut();
    int startIndex = pageIndex;
    while (!recordQueue.empty()) {
        Record* tmpRecord = new Record;
        tmpRecord->Copy(recordQueue.top());
        recordQueue.pop();
        if (bufferPage.Append(tmpRecord) == 0) {
            file.AddPage(&bufferPage, pageIndex++);
            bufferPage.EmptyItOut();
            bufferPage.Append(tmpRecord);
        }
    }
    file.AddPage(&bufferPage, pageIndex++);
    bufferPage.EmptyItOut();
    Run* run = new Run(&file, startIndex, pageIndex - startIndex);
    runQueue.push(run);
    return NULL;
}



BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
    pthread_t worker;
    Payload* payload = new Payload;
    payload->in = &in;
    payload->out = &out;
    payload->order = &sortorder;
    payload->runlen = runlen;
    pthread_create(&worker, NULL, workerMain, (void*) payload);
    pthread_join(worker, NULL);
	out.ShutDown ();
}

BigQ::~BigQ () {

}

Run::Run(File* file, int start, int length) {
    fileBase = file;
    startPage = start;
    runLength = length;
    curPage = start;
    fileBase->GetPage(&bufferPage, startPage);
    topRecord = new Record;
    UpdateTopRecord();
}

int Run::UpdateTopRecord() {
    if (bufferPage.GetFirst(topRecord) == 0) {
        curPage++;
        if (curPage == startPage + runLength) {
            return 0;
        }
        bufferPage.EmptyItOut();
        fileBase->GetPage(&bufferPage, curPage);
        bufferPage.GetFirst(topRecord);
    }
    return 1;
}

RecordComparer::RecordComparer(OrderMaker* orderMaker) {
    order = orderMaker;
}

bool RecordComparer::operator () (Record* left, Record* right) {
    ComparisonEngine comparisonEngine;
    return (comparisonEngine.Compare(left, right, order) >= 0);
}

RunComparer::RunComparer(OrderMaker* orderMaker) {
    order = orderMaker;
}

bool RunComparer::operator () (Run* left, Run* right) {
    ComparisonEngine comparisonEngine;
    return (comparisonEngine.Compare(left->topRecord, right->topRecord, order) >= 0);
}


