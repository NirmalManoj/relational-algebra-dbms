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
        parsedQuery.transposeRelationName = tokenizedQuery[1];
        logger.log(parsedQuery.transposeRelationName);
        return true;
    }
    cout << "SYNTAX ERROR" << endl;
    return false;
}

bool semanticParseTRANSPOSE()
{
    logger.log("semanticParseTRANSPOSE");
    if (!matrixCatalogue.isMatrix(parsedQuery.transposeRelationName))
    {
        cout << "SEMANTIC ERROR: Matrix doesn't exist" << endl;
        logger.log(parsedQuery.transposeRelationName);
        return false;
    }
    return true;
}

void executeTRANSPOSE()
{
    logger.log("executeTRANSPOSE");
    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.transposeRelationName);
    matrix->transpose();
    return;
}