#include "global.h"

/**
 * @brief Construct a new Matrix:: Matrix object
 *
 */
Matrix::Matrix()
{
    logger.log("Matrix::Matrix");
}

/**
 * @brief Construct a new Matrix:: Matrix object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param matrixName 
 */
Matrix::Matrix(string matrixName)
{
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + matrixName + ".csv";
    this->matrixName = matrixName;
}

/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into blocks.
 *
 * @return true if the matrix has been successfully loaded 
 * @return false if an error occurred 
 */
bool Matrix::load()
{
    logger.log("Matrix::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    this->getDimensionAndSparsityDetails();
    return this->blockify();
}

/**
 * @brief This function performs in-place transpose to the 
 * given matrix
 */
void Matrix::transpose()
{

}

/**
 * @brief This function obtains dimension of the matrix and also obtains info
 * about sparsity of the matrix
 */
void Matrix::getDimensionAndSparsityDetails()
{
    logger.log("Matrix::getDimensionAndSparsityDetails");
    ifstream fin(this->sourceFileName, ios::in);
    string line;
    getline(fin, line);
    string word;
    stringstream s(line);
    while (getline(s, word, ','))
    {
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        this->n++;
    }
    this->checkSparsity();
    int tuple_size = 1;
    if(this->isSparse)
        tuple_size = 2;
    this->maxElementsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * tuple_size));
}

/**
 * @brief This function traverses through the matrix and check if it is sparse 
 */
void Matrix::checkSparsity()
{
    logger.log("Matrix::checkSparsity");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    int zeroes = 0;
    while (getline(fin, line))
    {
        stringstream s(line);
        for (int i = 0; i < this->n; i++)
        {
            if (!getline(s, word, ','))
                return;
            if (stoi(word) == 0)
                zeroes++;
        }
    }
    if((5 * zeroes) >= (3 * this->n * this->n))
        this->isSparse = true;
    this->isSparse = false;
}

/**
 * @brief This function splits the matrix into elements and stores them 
 * in multiple files of one block size. 
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Matrix::blockify()
{
    logger.log("Matrix::blockify");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    int pageCounter = 0, lastElement = 0, rowCount = 0, columnsCount = 0;
    map<int, int> nonZeroElementsInPage;
    vector<int> elementsInPage;
    while (getline(fin, line))
    {
        stringstream s(line);
        while(getline(s, word, ','))
        {
            columnsCount++;
            if(!this->isSparse)
            {
                elementsInPage.emplace_back(stoi(word));
                pageCounter++;
                if (pageCounter == this->maxElementsPerBlock)
                {
                    bufferManager.writePage(this->matrixName, this->blockCount, elementsInPage, pageCounter);
                    this->blockCount++;
                    this->elementsPerBlockCount.emplace_back(pageCounter);
                    this->lastCellNumberInBlock.emplace_back(lastElement);
                    pageCounter = 0;
                    elementsInPage.clear();
                }
            }
            else
            {
                if(stoi(word) != 0)
                {
                    nonZeroElementsInPage.insert(make_pair(lastElement, stoi(word)));
                    pageCounter++;
                    if (pageCounter == this->maxElementsPerBlock)
                    {
                        bufferManager.writePage(this->matrixName, this->blockCount, nonZeroElementsInPage, pageCounter);
                        this->blockCount++;
                        this->elementsPerBlockCount.emplace_back(pageCounter);
                        this->lastCellNumberInBlock.emplace_back(lastElement);
                        pageCounter = 0;
                        nonZeroElementsInPage.clear();
                    }
                }
            }
            lastElement++;
        }
        if(columnsCount != this->n)
            return false;
        rowCount++;
        columnsCount = 0;
    }
    if (pageCounter)
    {
        if(this->isSparse)
            bufferManager.writePage(this->matrixName, this->blockCount, nonZeroElementsInPage, pageCounter);
        else
            bufferManager.writePage(this->matrixName, this->blockCount, elementsInPage, pageCounter);
        this->blockCount++;
        this->elementsPerBlockCount.emplace_back(pageCounter);
        this->lastCellNumberInBlock.emplace_back(lastElement);
        lastElement++;
        pageCounter = 0;
        nonZeroElementsInPage.clear();
        elementsInPage.clear();
    }
    if(rowCount != this->n)
        return false;
    return true;
}

/**
 * @brief Function prints the first few rows of the matrix. If the matrix contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Matrix::print()
{
    logger.log("Matrix::print");
    uint count = min((long long)PRINT_COUNT, (long long)this->n);
    Cursor cursor(this->matrixName, 0, 1, this->isSparse);
    vector<int> current_row;
    vector<int> current_element;
    int previous_element = -1, elementNumber = 0, rows_done = 0;
    if(!this->isSparse)
    {
        while(true)
        {
            current_element = cursor.getNext();
            if(current_element.empty())
                break;
            current_row.emplace_back(current_element);
            if(current_row.size() % this->n == 0)
            {
                this->writeRow(current_row, cout);
                rows_done++;
                if(rows_done == count)
                    break;
                current_row.clear();
            }
        }
    }
    else
    {
        while(true)
        {
            current_element = cursor.getNext();
            if(current_element.empty())
                break;
            elementNumber = current_element[0];
            for(int i=previous_element+1;i<elementNumber;i++)
            {
                current_row.emplace_back(0);
                if(current_row.size() % this->n == 0)
                {
                    this->writeRow(current_row, cout);
                    rows_done++;
                    if(rows_done == count)
                        break;
                    current_row.clear();
                }                
            }
            current_row.emplace_back(current_element[1]);
            if(current_row.size() % this->n == 0)
            {
                this->writeRow(current_row, cout);
                rows_done++;
                if(rows_done == count)
                    break;
                current_row.clear();
            }
            previous_element = elementNumber;
        }
        if(rows_done < count)
        {
            for(int i=previous_element+1;i<this->n*this->n;i++)
            {
                current_row.emplace_back(0);
                if(current_row.size() % this->n == 0)
                {
                    this->writeRow(current_row, cout);
                    rows_done++;
                    if(rows_done == count)
                        break;
                    current_row.clear();
                }            
            }
        }
    }
    printRowCount(this->n);
}

/**
 * @brief This function returns one row of the matrix using the cursor object. It
 * returns an empty row is all rows have been read.
 *
 * @param cursor 
 * @return vector<int> 
 */
void Matrix::getNextPage(Cursor *cursor)
{
    logger.log("Matrix::getNext");
    if (cursor->pageIndex < this->blockCount - 1)
    {
        cursor->nextPage(cursor->pageIndex+1);
    }
}

/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */
void Matrix::makePermanent()
{
    logger.log("Matrix::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->matrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);
    Cursor cursor(this->matrixName, 0, 1, this->isSparse);
    vector<int> current_row;
    vector<int> current_element;
    int previous_element = -1, elementNumber = 0;
    if(!this->isSparse)
    {
        while(true)
        {
            current_element = cursor.getNext();
            if(current_element.empty())
                break;
            current_row.emplace_back(current_element);
            if(current_row.size() % this->n == 0)
            {
                this->writeRow(current_row, cout);
                current_row.clear();
            }
        }
    }
    else
    {
        while(true)
        {
            current_element = cursor.getNext();
            if(current_element.empty())
                break;
            elementNumber = current_element[0];
            for(int i=previous_element+1;i<elementNumber;i++)
            {
                current_row.emplace_back(0);
                if(current_row.size() % this->n == 0)
                {
                    this->writeRow(current_row, cout);
                    current_row.clear();
                }                
            }
            current_row.emplace_back(current_element[1]);
            if(current_row.size() % this->n == 0)
            {
                this->writeRow(current_row, cout);
                current_row.clear();
            }
            previous_element = elementNumber;
        }
        for(int i=previous_element+1;i<this->n*this->n;i++)
        {
            current_row.emplace_back(0);
            if(current_row.size() % this->n == 0)
            {
                this->writeRow(current_row, cout);
                current_row.clear();
            }            
        }
    }   
    fout.close();
}

/**
 * @brief Function to check if matrix is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Matrix::isPermanent()
{
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->matrixName + ".csv")
        return true;
    return false;
}

/**
 * @brief The unload function removes the matrix from the database by deleting
 * all temporary files created as part of this matrix
 *
 */
void Matrix::unload(){
    logger.log("Matrix::~unload");
    for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
        bufferManager.deleteFile(this->matrixName, pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}

/**
 * @brief Function that returns a cursor that reads rows from this matrix
 * 
 * @return Cursor 
 */
Cursor Matrix::getCursor()
{
    logger.log("Matrix::getCursor");
    Cursor cursor(this->matrixName, 0, 1, this->isSparse);
    return cursor;
}
