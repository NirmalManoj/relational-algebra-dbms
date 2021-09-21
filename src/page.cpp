#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page()
{
    this->pageName = "";
    this->containerName = "";
    this->pageIndex = -1;
    this->rowCount = 0;
    this->pageType = -1;
    this->columnCount = 0;
    this->elementCount = 0;
    this->rows.clear();
    this->ofSparseMatrix = false;
    this->all_elements.clear();
    this->non_zero_elements.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name / matrix name and page
 * index. When tables / matrices are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<containerName>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param containerName 
 * @param pageIndex 
 */
Page::Page(string containerName, int pageIndex, int pageType, bool ofSparseMatrix)
{
    logger.log("Page::Page");
    this->containerName = containerName;
    this->pageIndex = pageIndex;
    this->pageType = pageType;
    this->ofSparseMatrix = ofSparseMatrix;
    this->pageName = "../data/temp/" + this->containerName + "_Page" + to_string(pageIndex);
    ifstream fin(pageName, ios::in);
    if(pageType == 0)
    {
        Table table = *tableCatalogue.getTable(containerName);
        this->columnCount = table.columnCount;
        this->rowCount = table.rowsPerBlockCount[pageIndex];
        uint maxRowCount = table.maxRowsPerBlock;
        vector<int> row(columnCount, 0);
        this->rows.assign(maxRowCount, row);
        int number;
        for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
        {
            for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
            {
                fin >> number;
                this->rows[rowCounter][columnCounter] = number;
            }
        }
    }
    else
    {
        Matrix matrix = *matrixCatalogue.getMatrix(containerName);
        this->elementCount = matrix.elementsPerBlockCount[pageIndex];
        if(matrix.isSparse)
        {
            map<int, int> nz_elements;
            string line;
            int current_cell, current_value;
            for (uint elementCounter = 0; elementCounter < this->elementCount; elementCounter++)
            {
                getline(fin, line, ':');
                current_cell = stoi(line);
                getline(fin, line, ' ');
                current_value = stoi(line);
                nz_elements.insert(make_pair(current_cell, current_value));
            }
            this->non_zero_elements = nz_elements;
        }
        else
        {
            vector<int> all_values;
            int number;
            for (uint elementCounter = 0; elementCounter < this->elementCount; elementCounter++)
            {
                fin >> number;
                all_values.emplace_back(number);
            }
            this->all_elements = all_values;
        }
    }

    fin.close();
}

/**
 * @brief Initializes map pointer for sparse matrices (while traversing with cursor)
 */
void Page::initializeMapPointer()
{
    this->nz_itr = this->non_zero_elements.begin();
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> Page::getRow(int rowIndex)
{
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}

/**
 * @brief Get element from page indexed by elementIndex from non sparse Matrix
 * 
 * @param elementIndex 
 * @return vector<int> 
 */
vector<int> Page::getNonSparseMatrixElement(int elementIndex)
{
    vector<int>result;
    logger.log("Page::getNonSparseMatrixElement");
    if(elementIndex >= this->elementCount)
        return result;
    result.emplace_back(this->all_elements[elementIndex]);
    return result;
}

/**
 * @brief Get element from page indexed by cellNum from sparse Matrix
 * 
 * @return vector<int> 
 */
vector<int> Page::getNextSparseMatrixElement()
{
    vector<int>result;
    logger.log("Page::getNextSparseMatrixElement");
    if(this->nz_itr == this->non_zero_elements.end())
        return result;
    this->nz_itr++;
    result.emplace_back(this->nz_itr->first);
    result.emplace_back(this->nz_itr->second);
    return result;
}

// Table page creation
Page::Page(string containerName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("Page::TablePage");
    this->containerName = containerName;
    this->pageIndex = pageIndex;
    this->pageType = 0;
    this->rows = rows;
    this->ofSparseMatrix = false;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->containerName + "_Page" + to_string(pageIndex);
}

// Non Sparse matrix page creation
Page::Page(string containerName, int pageIndex, vector<int> all_elements, int elementCount)
{
    logger.log("Page::NonSparseMatrixPage");
    this->containerName = containerName;
    this->pageIndex = pageIndex;
    this->pageType = 1; // Table page is 0, Matrix page is 1
    this->ofSparseMatrix = false;
    this->all_elements = all_elements;
    this->elementCount = elementCount;
    this->pageName = "../data/temp/"+this->containerName + "_Page" + to_string(pageIndex);
}

// Sparse matrix page creation
Page::Page(string containerName, int pageIndex, map<int, int> non_zero_elements, int elementCount)
{
    logger.log("Page::SparseMatrixPage");
    this->containerName = containerName;
    this->pageIndex = pageIndex;
    this->pageType = 1;
    this->ofSparseMatrix = true;
    this->non_zero_elements = non_zero_elements;
    this->elementCount = elementCount;
    this->pageName = "../data/temp/"+this->containerName + "_Page" + to_string(pageIndex);    
}

/**
 * @brief writes current page contents to file
 * 
 */
void Page::writePage()
{
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    int rowCounter, columnCounter;
    if(this->pageType == 0)
    {
        for (rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
        {
            for (columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
            {
                if (columnCounter != 0)
                    fout << " ";
                fout << this->rows[rowCounter][columnCounter];
            }
            fout << endl;
        }
    }
    else if(this->ofSparseMatrix) 
    {
        int current_cell_number, current_cell_value;
        for (auto const& p : this->non_zero_elements)
        {
            current_cell_number = p.first;
            current_cell_value = p.second;
            fout << current_cell_number << ":" << current_cell_value << " ";
        }
    }
    else
    {
        for (auto& element: this->all_elements)
            fout << element << " ";
    }
    fout.close();
}
