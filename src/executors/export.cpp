#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT <relation_name> OR EXPORT MATRIX <matrix_name>
 */

bool syntacticParseEXPORT()
{
    if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "MATRIX")
    {
        logger.log("syntacticParseEXPORT_MATRIX");
        parsedQuery.queryType = EXPORT_MATRIX;
        parsedQuery.loadRelationName = tokenizedQuery[2];
        return true;
    }
    logger.log("syntacticParseEXPORT");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = EXPORT;
    parsedQuery.exportRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParseEXPORT()
{
    logger.log("semanticParseEXPORT");
    //Table should exist
    if (tableCatalogue.isTable(parsedQuery.exportRelationName))
        return true;
    cout << "SEMANTIC ERROR: No such relation exists" << endl;
    return false;
}

void executeEXPORT()
{
    logger.log("executeEXPORT");
    Table* table = tableCatalogue.getTable(parsedQuery.exportRelationName);
    table->makePermanent();
    return;
}