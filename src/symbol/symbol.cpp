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

#include"symbol.h"
using namespace pl0;

Symbol::Symbol(Kind k,
               const std::string& name,
               const SourceLocation& location)
    : Symbol(k, name, "", 0, 0, location)
{}

Symbol::Symbol(Kind k,
               const std::string& name,
               const std::string& value,
               const SourceLocation& location)
    : Symbol(k, name, value, 0, 0, location)
{}

Symbol::Symbol(Kind k,
               const std::string& name,
               unsigned level, unsigned address,
               const SourceLocation& location)
    : Symbol(k, name, "", level, address, location)
{}

Symbol::Kind Symbol::getKind()const
{
    return kind_;
}

std::string Symbol::getName()const
{
    return name_;
}

void Symbol::setValue(const std::string& value)
{
    value_ = value;
}

std::string Symbol::getValue()const
{
    return value_;
}

void Symbol::setLevel(unsigned level)
{
    level_ = level;
}

unsigned Symbol::getLevel()const
{
    return level_;
}

void Symbol::setAddress(unsigned address)
{
    address_ = address;
}

unsigned Symbol::getAddress()const
{
    return address_;
}

void Symbol::setSourceLocation(const SourceLocation& location)
{
    location_ = location;
}

SourceLocation Symbol::getSourceLocation()const
{
    return location_;
}

bool Symbol::is(Kind k)const
{
    return kind_ == k;
}

bool Symbol::isNot(Kind k)const
{
    return kind_ != k;
}

bool Symbol::isConstantSymbol()const
{
    return is(Kind::Constant);
}

bool Symbol::isVariableSymbol()const
{
    return is(Kind::Variable);
}

bool Symbol::isProcedureSymbol()const
{
    return is(Kind::Procedure);
}

Symbol::Symbol(Kind k,
               const std::string& name,
               const std::string& value,
               unsigned level, unsigned address,
               const SourceLocation& location)
    : kind_(k), name_(name),
      value_(value), level_(level), address_(address), location_(location)
{}
