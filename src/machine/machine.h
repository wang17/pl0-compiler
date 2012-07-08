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

#ifndef MACHINE_H_
#define MACHINE_H_

#include<array>
#include<vector>
#include"../basic/pcode.h"

namespace pl0 {

class Machine {

public:
    explicit Machine(const std::vector<PCode>& codes);

    void setCodes(const std::vector<PCode>& codes);

    typedef std::vector<PCode>::iterator codes_iterator;
    codes_iterator getCodesBegin();
    codes_iterator getCodesEnd();

    typedef std::vector<PCode>::const_iterator const_codes_iterator;
    const_codes_iterator getCodesBegin()const;
    const_codes_iterator getCodesEnd()const;

    void start();

private:
    void init();
    void exec(const PCode& I);
    unsigned base(unsigned level_diff)const;

private:
    std::vector<PCode> codes_;

    unsigned program_counter_;
    unsigned base_register_;
    unsigned stack_register_;
    std::array<int, 1024> stack_;

private:
    Machine(const Machine&) = delete;
    void operator=(const Machine&) = delete;

};

}   //end namespace pl0

#endif // MACHINE_H_
