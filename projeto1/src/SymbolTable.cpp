#include "SymbolTable.h"

Symbol::Symbol() {
    this->dataType = Data::UNKNOWN;
    this->idType = Symbol::VARIABLE;
    this->initialized = false;
}

Symbol::Symbol(Data::Type dataType, IdentifierType idType, bool initialized) {
    this->dataType = dataType;
    this->idType = idType;
    this->initialized = initialized;
}

Symbol::~Symbol() {
}

Data::Type Symbol::getDataType() const {
    return this->dataType;
}

void Symbol::setDataType(Data::Type type) {
    this->dataType = type;
}

SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

bool SymbolTable::existsVariable(std::string varId) const {
    return entryList.find(varId) != entryList.end();
}

bool SymbolTable::isVariableInitialized(std::string varId) const {
    return entryList.at(varId).initialized;
}

Data::Type SymbolTable::getSymbolType(std::string varId) const {
    return entryList.at(varId).dataType;
}

void SymbolTable::addSymbol(std::string varId, Symbol newSymbol) {
    entryList[varId] = newSymbol;
}

void SymbolTable::setInitializedVariable(std::string varId) {
    entryList[varId].initialized = true;
}