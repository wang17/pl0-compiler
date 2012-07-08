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

#include"diagnostic.h"
using namespace pl0;

Diagnostic::Diagnostic(Code c, const SourceLocation& location, const std::string& token_value)
    : code_(c), location_(location), token_value_(token_value)
{
    size_t code_number = static_cast<size_t>(code_);
    if (code_number < 100) {
        kind_ = Kind::Error;
    } else {
        kind_ = Kind::None;
    }
}

Diagnostic::Kind Diagnostic::getKind()const
{
    return kind_;
}

const char* Diagnostic::getKindName()const
{
    switch (kind_) {
        case Kind::Error:
            return "Error";
        case Kind::None:
            return "None";
        default:
            return "";
    }
}

Diagnostic::Code Diagnostic::getCode()const
{
    return code_;
}

const char* Diagnostic::getCodeName()const
{
    static const char* const code_name[] = {
#define DIAG(X, Y) #X,
#include"diagnostic_kind.inc"
        0
    };

    return code_name[static_cast<size_t>(code_)];
}

const char* Diagnostic::getCodeDescription()const
{
    static const char* const code_description[] = {
#define DIAG(X, Y) Y,
#include"diagnostic_kind.inc"
        0
    };

    return code_description[static_cast<size_t>(code_)];
}

SourceLocation Diagnostic::getSourceLocation()const
{
    return location_;
}

std::string Diagnostic::getTokenValue()const
{
    return token_value_;
}
