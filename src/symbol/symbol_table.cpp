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

#include"symbol_table.h"
#include<cassert>
using namespace pl0;

SymbolTable::SymbolTable()
    : symbol_map_list_()
{}

SymbolTable::~SymbolTable()
{
    clear();
}

bool SymbolTable::insertSymbol(Symbol* symbol)
{
    assert(scopeLevel() > 0);

    auto result = symbol_map_list_.front()->insert(symbol_map::value_type(symbol->getName(), symbol));
    return result.second;
}

Symbol* SymbolTable::findSymbol(const std::string& name, bool local_only)
{
    assert(scopeLevel() > 0);

    if (local_only) {
        symbol_map::iterator iter = symbol_map_list_.front()->find(name);
        if (iter != symbol_map_list_.front()->end()) {
            return iter->second;
        }
        return 0;
    }

    for (const symbol_map* map: symbol_map_list_) {
        auto iter = map->find(name);
        if (iter != map->end()) {
            return iter->second;
        }
    }
    return 0;
}

const Symbol* SymbolTable::findSymbol(const std::string& name, bool local_only)const
{
    return findSymbol(name, local_only);
}

void SymbolTable::beginScope()
{
    symbol_map_list_.push_front(new symbol_map());
}

void SymbolTable::endScope()
{
    assert(scopeLevel() > 0);

    for (auto ele: *symbol_map_list_.front()) {
        delete ele.second;
    }
    delete symbol_map_list_.front();
    symbol_map_list_.pop_front();
}

unsigned SymbolTable::scopeLevel()const
{
    return symbol_map_list_.size();
}

void SymbolTable::clear()
{
    while (scopeLevel() > 0) {
        endScope();
    }
}
