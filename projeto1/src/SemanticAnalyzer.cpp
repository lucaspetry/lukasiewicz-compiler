#include "SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer() {
}

SemanticAnalyzer::~SemanticAnalyzer() {
}

void SemanticAnalyzer::analyzeBinaryOperation(BinaryOperation* binaryOp) {
    TreeNode* left = binaryOp->left;
    BinaryOperation::Type op = binaryOp->operation;
    TreeNode* right = binaryOp->right;
            
    // Adiciona uma conversão int -> float obrigatória caso necessário
    switch(left->dataType()) {
        case Data::FLOAT:
            if(right->dataType() == Data::INTEGER) {
                binaryOp->right = new TypeCasting(Data::FLOAT, right);
            }
            break;
        case Data::INTEGER:
            if(right->dataType() == Data::FLOAT && op != BinaryOperation::ASSIGN) {
                binaryOp->left = new TypeCasting(Data::FLOAT, left);
            }
            break;
        default:
            break;
    }
    
    // Define o tipo da operação binária de acordo com o operando esquerdo
    if(binaryOp->left->dataType() != Data::UNKNOWN)
        binaryOp->setType(binaryOp->left->dataType());
    
    // Se algum dos tipos é desconhecido, a análise de operandos não se aplica
    if(binaryOp->left->dataType() == Data::UNKNOWN || binaryOp->right->dataType() == Data::UNKNOWN)
        return;
    
    // Se os operandos diferem, gera erro semântico
    if(binaryOp->left->dataType() != binaryOp->right->dataType())
        yyerror("semantic error: %s operation expected %s but received %s\n",
            BinaryOperation::operationName(op),
            dataTypeToString(binaryOp->left->dataType()).c_str(),
            dataTypeToString(binaryOp->right->dataType()).c_str());
}

TreeNode* SemanticAnalyzer::declareVariable(std::string id, TreeNode::ClassType dataType) {
    if(symbolTable.existsVariable(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, false)); // Adds variable to symbol table
    
    return new Variable(id, classToDataType(dataType)); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::assignVariable(std::string id, TreeNode::ClassType assignedType) {
    if(!symbolTable.existsVariable(id)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else {
        symbolTable.setInitializedVariable(id);
        return new Variable(id, symbolTable.getSymbolType(id));
    }
    
}

TreeNode* SemanticAnalyzer::declareAssignVariable(std::string id, TreeNode::ClassType dataType, TreeNode::ClassType assignedType) {
    if(symbolTable.existsVariable(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, false)); // Adds variable to symbol table
    
    symbolTable.setInitializedVariable(id);
    
    return new Variable(id, classToDataType(dataType));
}

TreeNode* SemanticAnalyzer::useVariable(std::string id) {
    if(!symbolTable.existsVariable(id)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else if(!symbolTable.isVariableInitialized(id)) {
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    }
    return new Variable(id, symbolTable.getSymbolType(id));
}

Data::Type SemanticAnalyzer::classToDataType(TreeNode::ClassType type) const {
    switch(type) {
        case TreeNode::BOOLEAN:
            return Data::BOOLEAN;
        case TreeNode::FLOAT:
            return Data::FLOAT;
        case TreeNode::INTEGER:
            return Data::INTEGER;
        default:
            return Data::UNKNOWN;
    }
}

std::string SemanticAnalyzer::classToString(TreeNode::ClassType type) const {
    return dataTypeToString(classToDataType(type));
}

std::string SemanticAnalyzer::dataTypeToString(Data::Type type) const {
    switch(type) {
        case Data::BOOLEAN:
            return "boolean";
        case Data::FLOAT:
            return "float";
        case Data::INTEGER:
            return "integer";
        default:
            return "unknown";
    }    
}