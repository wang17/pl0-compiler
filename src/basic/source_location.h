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

#ifndef SOURCE_LOCATION_H_
#define SOURCE_LOCATION_H_

namespace pl0 {

class SourceLocation {

public:
    explicit SourceLocation(unsigned line_number = 0);

    void setLineNumber(unsigned line_number);
    unsigned getLineNumber()const;

private:
    unsigned line_number_;

};

}   //end namespace pl0

#endif // SOURCE_LOCATION_H_
