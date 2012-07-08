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

#include"diagnostic_reporter.h"
#include<iostream>
using namespace pl0;

DiagnosticReporter::DiagnosticReporter()
    : error_count_(0), none_count_(0)
{}

void DiagnosticReporter::reportDiagnostic(const Diagnostic& diag)
{
    switch (diag.getKind()) {
        case Diagnostic::Kind::Error:
            error_count_++;
            break;
        case Diagnostic::Kind::None:
            none_count_++;
            break;
        default:
            break;
    }

    std::cout << diag.getSourceLocation().getLineNumber() << ": "
              << diag.getKindName() << ": "
              << diag.getCodeDescription() << "\n"
              << "Unexpected token: " << diag.getTokenValue() << "\n\n";
}

void DiagnosticReporter::resetCount()
{
    error_count_ = 0;
    none_count_ = 0;
}

unsigned DiagnosticReporter::getErrorCount()const
{
    return error_count_;
}

unsigned DiagnosticReporter::getNoneCount()const
{
    return none_count_;
}
