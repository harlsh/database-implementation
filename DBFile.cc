#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {
    isFileOpen = 0;
    isWriting = 0;
    pageIndex = 0;
}

int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    if (isFileOpen == 1) {
        cerr << "Cannot recreate file since file already opened!" << "\n";
        return 0;
    }
    if(f_type == heap){
        diskFile.Open(0, const_cast<char *>(f_path));
        pageIndex = 0;
        isWriting = 0;
        isFileOpen = 1;
        MoveFirst();
    }
    return 1;
}

void DBFile::Load (Schema &f_schema, const char *loadpath) {
    if (isFileOpen == 0) {
        cerr << "Cannot loading while file not open!";
        return;
    }
    FILE *tableFile = fopen (loadpath, "r");
    Record temp;
    ComparisonEngine comp;

    while (temp.SuckNextRecord (&f_schema, tableFile) == 1) {
            this->Add(temp);
    }
    if (isWriting == 1)
        diskFile.AddPage(&bufferPage, pageIndex);
}

int DBFile::Open (const char *f_path) {
    if (isFileOpen == 1) {
        cerr << "File already opened!" << "\n";
        return 0;
    }
    diskFile.Open(1, const_cast<char *>(f_path));
    pageIndex = 0;
    
    isWriting = 0;
    isFileOpen = 1;
    MoveFirst();
    return 1;
}

void DBFile::MoveFirst () {
    if (isFileOpen == 0) {
        cerr << "Cannot MoveFirst while file not opening!" << "\n";
        return;
    }
    if (isWriting == 1) {
        diskFile.AddPage(&bufferPage, pageIndex);
        isWriting = 0;
    }
    pageIndex = 0;
    bufferPage.EmptyItOut();
    
    if (diskFile.GetLength() > 0) {
        diskFile.GetPage(&bufferPage, pageIndex);
    }
}
int DBFile::Close () {
    if (isFileOpen == 0) {
        cerr << "File is not opened!" << "\n";
        return 0;
    }
    if (isWriting == 1)
        diskFile.AddPage(&bufferPage, pageIndex);
    bufferPage.EmptyItOut();
    diskFile.Close();
    isFileOpen = 0;
    return 1;
}
void DBFile::Add (Record &rec) {
    if (isFileOpen == 0) {
        cerr << "Cannot writing while file not opening!" << "\n";
        return;
    }
    if (isWriting == 0) {
        bufferPage.EmptyItOut();
        if (diskFile.GetLength() > 0) {
            diskFile.GetPage(&bufferPage, diskFile.GetLength() - 2);
            pageIndex = diskFile.GetLength() - 2;
        }
        isWriting = 1;
    }
    if (bufferPage.Append(&rec) == 0) {
        diskFile.AddPage(&bufferPage, pageIndex++);
        bufferPage.EmptyItOut();
        bufferPage.Append(&rec);
    }
}

int DBFile::GetNext (Record &fetchme) {
    if (isFileOpen == 0) {
        cerr << "Cannot reading while file not opening!" << "\n";
        return 0;
    }
    
    if (isWriting == 1) {
        MoveFirst();
    }
    if (bufferPage.GetFirst(&fetchme) == 0) {
        pageIndex++;
        if (pageIndex >= diskFile.GetLength() - 1) {
            return 0;
        }
        bufferPage.EmptyItOut();
        diskFile.GetPage(&bufferPage, pageIndex);
        bufferPage.GetFirst(&fetchme);
    }
    return 1;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    ComparisonEngine comp;
    while (GetNext(fetchme) == 1) {
        if (comp.Compare(&fetchme, &literal, &cnf) == 1)
            return 1;
    }
    return 0;
}
