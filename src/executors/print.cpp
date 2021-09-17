#include "global.h"
/**
 * @brief 
 * SYNTAX: PRINT <relation_name> OR  OR PRINT MATRIX <matrix_name>
 */
bool syntacticParsePRINT()
{
    if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "MATRIX")
    {
        logger.log("syntacticParsePRINT_MATRIX");
        parsedQuery.queryType = PRINT_MATRIX;
        parsedQuery.loadRelationName = tokenizedQuery[2];
        return true;
    }
    logger.log("syntacticParsePRINT");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = PRINT;
    parsedQuery.printRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParsePRINT()
{
    logger.log("semanticParsePRINT");
    if (!tableCatalogue.isTable(parsedQuery.printRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    return true;
}

void executePRINT()
{
    logger.log("executePRINT");
    Table* table = tableCatalogue.getTable(parsedQuery.printRelationName);
    table->print();
    return;
}
