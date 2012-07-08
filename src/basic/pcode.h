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

#ifndef PCODE_H_
#define PCODE_H_

namespace pl0 {

struct PCode {

enum class OpCode {
    Lit,
    Lod,
    Sto,
    Cal,
    Int,
    Jmp,
    Jpc,
    Opr
};

PCode::OpCode f;
unsigned l;
unsigned a;

};

}   //end namespace pl0

#endif // PCODE_H_
