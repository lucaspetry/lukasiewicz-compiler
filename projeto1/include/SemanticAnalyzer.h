#ifndef SEMANTICANALYZER_H_
#define SEMANTICANALYZER_H_

#include "SymbolTable.h"
#include <string>
#include <vector>

class SemanticAnalyzer;

extern SemanticAnalyzer SEMANTIC_ANALYZER;
extern void yyerror(const char* s, ...);

/**
 * Analisador semântico.
 */
class SemanticAnalyzer {

    public:
        SemanticAnalyzer();
        virtual ~SemanticAnalyzer();

        void newScope();
        void returnScope();
        void analyzeBinaryOperation(ConditionalOperation* conditionalOp);
        void analyzeBinaryOperation(LoopDeclaration* loop);
        void analyzeBinaryOperation(BinaryOperation* binaryOp);
        TreeNode* declareVariable(std::string varId, TreeNode::ClassType dataType, int size = 0);
        TreeNode* assignVariable(std::string varId, TreeNode::ClassType assignedType, TreeNode * n = NULL);
        TreeNode* declareAssignVariable(std::string id, TreeNode::ClassType dataType, TreeNode::ClassType assignedType);
        TreeNode* useVariable(std::string varId, TreeNode* index = NULL);

    private:
        Data::Type classToDataType(TreeNode::ClassType type) const;
        std::string classToString(TreeNode::ClassType type) const;
        std::string dataTypeToString(Data::Type type) const;
        SymbolTable symbolTable;
        std::vector<SymbolTable> scopes;

};

#endif
