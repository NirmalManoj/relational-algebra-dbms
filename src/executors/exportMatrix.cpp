#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT MATRIX <matrix_name>
 */

bool semanticParseEXPORT_MATRIX()
{
    logger.log("semanticParseEXPORT_MATRIX");
    //Matrix should exist
    if (matrixCatalogue.isMatrix(parsedQuery.exportRelationName))
        return true;
    cout << "SEMANTIC ERROR: No such matrix exists" << endl;
    return false;
}

void executeEXPORT_MATRIX()
{
    logger.log("executeEXPORT_MATRIX");
    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.exportRelationName);
    matrix->makePermanent();
    return;
}