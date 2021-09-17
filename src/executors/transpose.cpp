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
}

void executeTRANSPOSE()
{
    logger.log("executeTRANSPOSE");
    return;
}