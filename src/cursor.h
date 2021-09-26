#ifndef CURSOR_H
#define CURSOR_H

#include"bufferManager.h"
/**
 * @brief The cursor is an important component of the system. To read from a
 * table, you need to initialize a cursor. The cursor reads rows from a page one
 * at a time.
 *
 */
class Cursor{
    public:
    Page page;
    int pageIndex;
    string containerName;
    int pagePointer;
    int pageType;
    bool ofSparseMatrix;

    public:
    Cursor(string containerName, int pageIndex, int pageType, bool ofSparseMatrix);
    vector<int> getNext();
    vector<int> getElementAtIndex(int elementIndex);
    void nextPage(int pageIndex);
};

#endif