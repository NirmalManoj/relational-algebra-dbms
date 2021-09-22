#include "global.h"

Cursor::Cursor(string containerName, int pageIndex, int pageType, bool ofSparseMatrix)
{
    logger.log("Cursor::Cursor");
    this->page = bufferManager.getPage(containerName, pageIndex, pageType, ofSparseMatrix);
    this->pagePointer = 0;    
    this->containerName = containerName;
    this->pageIndex = pageIndex;
    this->pageType = pageType;
    this->ofSparseMatrix = ofSparseMatrix;
    if(ofSparseMatrix)
        this->page.initializeMapPointer();
}

/**
 * @brief This function reads the next row from the page. The index of the
 * current row read from the page is indicated by the pagePointer(points to row
 * in page the cursor is pointing to). This analogy is also applied to matrices
 *
 * @return vector<int> 
 */
vector<int> Cursor::getNext()
{
    logger.log("Cursor::getNext");
    if(this->pageType == 0)
    {
        vector<int> result = this->page.getRow(this->pagePointer);
        this->pagePointer++;
        if(result.empty())
        {
            tableCatalogue.getTable(this->containerName)->getNextPage(this);
            if(!this->pagePointer){
                result = this->page.getRow(this->pagePointer);
                this->pagePointer++;
            }
        }           
        return result;
    }
    else if(this->ofSparseMatrix)
    {
        vector<int> result = this->page.getNextSparseMatrixElement();
        this->pagePointer++;
        if(result.empty())
        {
            matrixCatalogue.getMatrix(this->containerName)->getNextPage(this);
            if(!this->pagePointer){
                result = this->page.getNextSparseMatrixElement();
                this->pagePointer++;
            }            
        }
        return result;
    }
    vector<int> result = this->page.getNonSparseMatrixElement(this->pagePointer);
    this->pagePointer++;
    if(result.empty())
    {
        matrixCatalogue.getMatrix(this->containerName)->getNextPage(this);
        if(!this->pagePointer){
            result = this->page.getNonSparseMatrixElement(this->pagePointer);
            this->pagePointer++;
        }
    }           
    return result;    
}

vector<int> Cursor::getElementAtIndex(int elementIndex)
{
    vector<int> result;
    if(!this->ofSparseMatrix){
        result = this->page.getNonSparseMatrixElement(elementIndex);
        return result;
    }
}

/**
 * @brief Function that loads Page indicated by pageIndex. Now the cursor starts
 * reading from the new page.
 *
 * @param pageIndex 
 */
void Cursor::nextPage(int pageIndex)
{
    logger.log("Cursor::nextPage");
    this->page = bufferManager.getPage(this->containerName, pageIndex, this->pageType, this->ofSparseMatrix);
    this->pageIndex = pageIndex;
    this->pagePointer = 0;
    if(this->ofSparseMatrix)
        this->page.initializeMapPointer();
}