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

#ifndef TOKEN_H_
#define TOKEN_H_

#include<string>
#include"../basic/source_location.h"

namespace pl0 {

class Token {

public:
    enum class Kind: size_t {
#define TOK(X) X,
#include"token_kind.def"
        NUM_TOKENS
    };

public:
    Token();

    void startToken();

    void setKind(Kind k);
    Kind getKind()const;

    bool is(Kind k)const;
    bool isNot(Kind k)const;

    bool isIdentifier()const;
    bool isNumber()const;

    void setRawData(const std::string& data);
    std::string getRawData()const;

    void setSourceLocation(const SourceLocation& location);
    SourceLocation getSourceLocation()const;

    const char* getTokenKindName()const;

private:
    Kind kind_;
    std::string raw_data_;
    SourceLocation location_;

};

}   //end namespace pl0

#endif // TOKEN_H_
