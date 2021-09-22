#include "global.h"

/**
 * @brief 
 * SYNTAX: CLEAR <relation_name> 
 */

bool syntacticParseCLEAR()
{
    logger.log("syntacticParseCLEAR");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = CLEAR;
    parsedQuery.clearRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParseCLEAR()
{
    logger.log("semanticParseCLEAR");
    //Table should exist
    if (tableCatalogue.isTable(parsedQuery.clearRelationName) || matrixCatalogue.isMatrix(parsedQuery.clearRelationName))
        return true;
    cout << "SEMANTIC ERROR: No such relation exists" << endl;
    return false;
}

void executeCLEAR()
{
    logger.log("executeCLEAR");
    //Deleting table/matrix from the catalogue deletes all temporary files
    if (tableCatalogue.isTable(parsedQuery.clearRelationName))
        tableCatalogue.deleteTable(parsedQuery.clearRelationName);
    else
        matrixCatalogue.deleteMatrix(parsedQuery.clearRelationName);
    return;
}