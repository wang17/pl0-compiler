/*
 * This file is a part of pl0-compiler.
 *
 * Copyright (C) 2012 xStone <chen_fu_hua@126.com>
 *
 * pl0-compiler is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include<list>
#include<string>
#include<unordered_map>
#include"symbol.h"

namespace pl0 {

class SymbolTable {

public:
    SymbolTable();
    ~SymbolTable();

    bool insertSymbol(Symbol* symbol);

    Symbol* findSymbol(const std::string& name, bool local_only = false);
    const Symbol* findSymbol(const std::string& name, bool local_only = false)const;

    void beginScope();
    void endScope();

    unsigned scopeLevel()const;

    void clear();

private:
    typedef std::unordered_map<std::string, Symbol*> symbol_map;
    std::list<symbol_map*> symbol_map_list_;

private:
    SymbolTable(const SymbolTable&) = delete;
    void operator=(const SymbolTable&) = delete;

};

}   //end namespace pl0

#endif // SYMBOL_TABLE_H_
