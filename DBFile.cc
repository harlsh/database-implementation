#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile()
{
    pageWritten = true;
}

DBFile::~DBFile()
{
    delete pFile;
    delete pRecord;
    delete pPage;
}

int DBFile::Create(const char *f_path, fType f_type, void *startup)
{
    pFile->Open(0, f_path);
    return pFile->GetMyFileDes() != 0;
}

void DBFile::Load(Schema &f_schema, const char *loadpath)
{
    File *tempFile = fopen(loadpath, "r");
    if (tempFile == NULL)
    {
        cerr << "Given File Path " << loadpath << " is invalid." << endl;
        exit(1);
    }

    Record tempRecord;
    while (tempRecord.SuckNextRecord(&f_schema, tempFile) != 0)
        Add(tempRecord);
    fclose(tempFile);
}

int DBFile::Open(const char *f_path)
{
    return pFile->Open(1, f_path) != 0;
}

void DBFile::MoveFirst()
{
    if (pFile->GetLength() == 0)
    {
        cerr << "File is Empty" << endl;
        exit(1);
    }
    pFile->GetPage(pPage, 0);
}

int DBFile::Close()
{
    if (pageWritten == false)
    {
        off_t tempIndex = pFile->GetLength();
        if (tempIndex == 0)
        {
            currentPageIndex = 0;
        }
        else
        {
            currentPageIndex = tempIndex - 1;
        }
        pFile->AddPage(myPage, tempIndex);
        pPage->EmptyItOut();
    }
    pageWritten = true;
    int temp1 = pFile->Close();
    int closeValue = pFile->GetmyFilDes();
    if (closeValue == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void DBFile::Add(Record &rec)
{
}

int DBFile::GetNext(Record &fetchme)
{
    while (pPage->GetFirst(&fetchme) != 1)
    {
        off_t fileLength;
        if (pFile->GetLength() == 0)
        {
            fileLength = 0;
        }
        else
        {
            fileLength = pFile->GetLength() - 1;
        }
        if (++currentPageIndex < fileLength)
        {
            pFile->GetPage(pPage, currentPageIndex);
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int DBFile::GetNext(Record &fetchme, CNF &cnf, Record &literal)
{
    ComparisonEngine cEngine;
    while (GetNext(fetchme))
    {
        if (cEngine.Compare(&fetchme, &literal, &cnf))
        {
            return 1;
        }
    }
    return 0;
}

File *DBFile::GetFile()
{
    return pFile;
}

Page *DBFile::GetCurrentPage()
{
    return pPage;
}