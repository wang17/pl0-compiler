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

#include"source_location.h"
using namespace pl0;

SourceLocation::SourceLocation(unsigned line_number)
    : line_number_(line_number)
{}

void SourceLocation::setLineNumber(unsigned line_number)
{
    line_number_ = line_number;
}

unsigned SourceLocation::getLineNumber()const
{
    return line_number_;
}
