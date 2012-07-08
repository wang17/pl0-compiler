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

#ifndef DIAGNOSTIC_REPORTER_H_
#define DIAGNOSTIC_REPORTER_H_

#include"diagnostic.h"

namespace pl0 {

class DiagnosticReporter {

public:
    DiagnosticReporter();

    void reportDiagnostic(const Diagnostic& diag);

    void resetCount();
    unsigned getErrorCount()const;
    unsigned getNoneCount()const;

private:
    unsigned error_count_;
    unsigned none_count_;

private:
    DiagnosticReporter(const DiagnosticReporter&) = delete;
    void operator=(const DiagnosticReporter&) = delete;

};

}   //end namespace pl0

#endif // DIAGNOSTIC_REPORTER_H_
