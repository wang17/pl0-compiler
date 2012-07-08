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

#ifndef DIAGNOSTIC_H_
#define DIAGNOSTIC_H_

#include<string>
#include"../basic/source_location.h"

namespace pl0 {

class Diagnostic {

public:
    enum class Kind: size_t {
        Error,
        None
    };

    enum class Code: size_t {
#define DIAG(X, Y) X,
#include"diagnostic_kind.inc"
        NUM_DIAG
    };

public:
    Diagnostic(Code c, const SourceLocation& location, const std::string& token_value);

    Kind getKind()const;
    const char* getKindName()const;

    Code getCode()const;
    const char* getCodeName()const;
    const char* getCodeDescription()const;

    SourceLocation getSourceLocation()const;

    std::string getTokenValue()const;

private:
    Kind kind_;
    Code code_;
    SourceLocation location_;
    std::string token_value_;

};

}   //end namespace pl0

#endif // DIAGNOSTIC_H_
