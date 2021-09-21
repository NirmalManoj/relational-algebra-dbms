#ifndef MATRIX_H
#define MATRIX_H

#include "cursor.h"

/**
 * @brief The Matrix class holds all information related to a loaded matrix. It
 * also implements methods that interact with the parsers, executors, cursors
 * and the buffer manager.
 *
 */
class Matrix
{
public:
    string sourceFileName = "";
    string matrixName = "";
    uint blockCount = 0;
    uint n = 0;
    uint maxElementsPerBlock = 0;
    vector<uint> elementsPerBlockCount;
    vector<uint> lastCellNumberInBlock;
    bool isSparse = false;
    
    bool blockify();
    void getDimensionAndSparsityDetails();
    void checkSparsity();
    Matrix();
    Matrix(string matrixName);
    bool load();
    void transpose();
    void print();
    void makePermanent();
    bool isPermanent();
    void getNextPage(Cursor *cursor);
    Cursor getCursor();
    void unload();

/**
 * @brief Static function that takes a vector of valued and prints them out in a
 * comma seperated format.
 *
 * @tparam T current usaages include int and string
 * @param row 
 */
template <typename T>
void writeRow(vector<T> row, ostream &fout)
{
    logger.log("Matrix::printRow");
    for (int columnCounter = 0; columnCounter < row.size(); columnCounter++)
    {
        if (columnCounter != 0)
            fout << ", ";
        fout << row[columnCounter];
    }
    fout << endl;
}

/**
 * @brief Static function that takes a vector of valued and prints them out in a
 * comma seperated format.
 *
 * @tparam T current usaages include int and string
 * @param row 
 */
template <typename T>
void writeRow(vector<T> row)
{
    logger.log("Matrix::printRow");
    ofstream fout(this->sourceFileName, ios::app);
    this->writeRow(row, fout);
    fout.close();
}
};

#endif