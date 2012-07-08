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

#include"token.h"
using namespace pl0;

Token::Token()
{
    startToken();
}

void Token::startToken()
{
    kind_ = Kind::unknown;
    raw_data_.clear();
    location_.setLineNumber(0);
}

void Token::setKind(Kind k)
{
    kind_ = k;
}

Token::Kind Token::getKind()const
{
    return kind_;
}

bool Token::is(Kind k)const
{
    return kind_ == k;
}

bool Token::isNot(Kind k)const
{
    return kind_ != k;
}

bool Token::isIdentifier()const
{
    return is(Kind::identifier);
}

bool Token::isNumber()const
{
    return is(Kind::number);
}

void Token::setRawData(const std::string& data)
{
    raw_data_ = data;
}

std::string Token::getRawData()const
{
    return raw_data_;
}

void Token::setSourceLocation(const SourceLocation& location)
{
    location_ = location;
}

SourceLocation Token::getSourceLocation()const
{
    return location_;
}

const char* Token::getTokenKindName()const
{
    static const char* const token_kind_name[] = {
#define TOK(X) #X,
#include"token_kind.def"
        0
    };

    return token_kind_name[static_cast<size_t>(kind_)];
}
