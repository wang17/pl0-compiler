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

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include<string>
#include"../basic/source_location.h"

namespace pl0 {

class Symbol {

public:
    enum class Kind {
        Constant,
        Variable,
        Procedure
    };

public:
    Symbol(Kind k,
          const std::string& name,
          const SourceLocation& location = SourceLocation());
    Symbol(Kind k,
           const std::string& name,
           const std::string& value,
           const SourceLocation& location = SourceLocation());
    Symbol(Kind k,
           const std::string& name,
           unsigned level, unsigned address,
           const SourceLocation& location = SourceLocation());

    Kind getKind()const;

    std::string getName()const;

    void setValue(const std::string& value);
    std::string getValue()const;

    void setLevel(unsigned level);
    unsigned getLevel()const;

    void setAddress(unsigned address);
    unsigned getAddress()const;

    void setSourceLocation(const SourceLocation& location);
    SourceLocation getSourceLocation()const;

public:
    bool is(Kind k)const;
    bool isNot(Kind k)const;

    bool isConstantSymbol()const;
    bool isVariableSymbol()const;
    bool isProcedureSymbol()const;

private:
    Symbol(Kind k,
           const std::string& name,
           const std::string& value,
           unsigned level, unsigned address,
           const SourceLocation& location);

private:
    Kind kind_;
    std::string name_;
    std::string value_;
    unsigned level_;
    unsigned address_;
    SourceLocation location_;

};

}   //end namespace pl0

#endif // SYMBOL_H_
