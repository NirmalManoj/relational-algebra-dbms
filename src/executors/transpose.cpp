#include "global.h"

/**
 * @brief 
 * SYNTAX: TRANSPOSE <matrix_name>
 */

bool syntacticParseTRANSPOSE()
{
    logger.log("syntacticParseTRANSPOSE");
    if (tokenizedQuery.size() == 2)
    {
        parsedQuery.queryType = TRANSPOSE;
        parsedQuery.loadRelationName = tokenizedQuery[1];
        return true;
    }
    cout << "SYNTAX ERROR" << endl;
    return false;
}

bool semanticParseTRANSPOSE()
{
    logger.log("semanticParseTRANSPOSE");
    if (!matrixCatalogue.isMatrix(parsedQuery.printRelationName))
    {
        cout << "SEMANTIC ERROR: Matrix doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeTRANSPOSE()
{
    logger.log("executeTRANSPOSE");
    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.printRelationName);
    matrix->transpose();
    return;
}