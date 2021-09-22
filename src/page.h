#ifndef PAGE_H
#define PAGE_H

#include"logger.h"
/**
 * @brief The Page object is the main memory representation of a physical page
 * (equivalent to a block). The page class and the page.h header file are at the
 * bottom of the dependency tree when compiling files. 
 *<p>
 * Do NOT modify the Page class. If you find that modifications
 * are necessary, you may do so by posting the change you want to make on Moodle
 * or Teams with justification and gaining approval from the TAs. 
 *</p>
 */

class Page{

    string containerName;
    string pageIndex;
    int columnCount; // for tables
    int rowCount; // for tables
    int elementCount; // for matrices
    int pageType; // 0 - table page; 1 - matrix page
    bool ofSparseMatrix; // if a page corresponds to a sparse matrix
    vector<vector<int>> rows; // for rows
    vector<int> all_elements; // for non-sparse matrices
    map<int, int> non_zero_elements; // for sparse matrices
    map<int, int>::iterator nz_itr; // non zero elements map iterator

    public:

    string pageName = "";
    Page();
    Page(string containerName, int pageIndex, int pageType, bool ofSparseMatrix);
    Page(string containerName, int pageIndex, vector<vector<int>> rows, int rowCount);
    Page(string containerName, int pageIndex, vector<int> all_elements, int elementCount);
    Page(string containerName, int pageIndex, map<int, int> non_zero_elements, int elementCount);
    vector<int> getRow(int rowIndex);
    vector<int> getNonSparseMatrixElement(int elementIndex);
    vector<int> getNextSparseMatrixElement();
    void initializeMapPointer();
    void writePage();
    string getPageName(string containerName, int pageIndex);
};

#endif