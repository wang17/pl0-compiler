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

#include"machine.h"
#include<iostream>
using namespace pl0;

Machine::Machine(const std::vector<PCode>& codes)
    : codes_(codes), program_counter_(0),
      base_register_(0), stack_register_(0), stack_()
{}

void Machine::setCodes(const std::vector<PCode>& codes)
{
    codes_ = codes;
    init();
}

Machine::codes_iterator Machine::getCodesBegin()
{
    return codes_.begin();
}

Machine::codes_iterator Machine::getCodesEnd()
{
    return codes_.end();
}

Machine::const_codes_iterator Machine::getCodesBegin()const
{
    return codes_.cbegin();
}

Machine::const_codes_iterator Machine::getCodesEnd()const
{
    return codes_.cend();
}

void Machine::start()
{
    PCode I;

    init();

    do {
        I = codes_[program_counter_++];
        exec(I);
    } while (program_counter_ != 0);
}

void Machine::init()
{
    program_counter_ = 0;
    base_register_ = 0;
    stack_register_ = 0;
    stack_.fill(0);
}

void Machine::exec(const PCode& I)
{
    switch (I.f) {
        case PCode::OpCode::Lit:
            stack_[stack_register_++] = I.a;
            break;
        case PCode::OpCode::Lod:
            stack_[stack_register_++] = stack_[base(I.l) + I.a];
            break;
        case PCode::OpCode::Sto:
            stack_[base(I.l) + I.a] = stack_[--stack_register_];
            break;
        case PCode::OpCode::Cal:
            stack_[stack_register_] = base(I.l);
            stack_[stack_register_ + 1] = base_register_;
            stack_[stack_register_ + 2] = program_counter_;
            base_register_ = stack_register_;
            program_counter_ = I.a;
            break;
        case PCode::OpCode::Int:
            stack_register_ += I.a;
            break;
        case PCode::OpCode::Jmp:
            program_counter_ = I.a;
            break;
        case PCode::OpCode::Jpc:
            if (stack_[--stack_register_] == 0) {
                program_counter_ = I.a;
            }
            break;
        case PCode::OpCode::Opr:
            switch (I.a) {
                case 0:
                    stack_register_ = base_register_;
                    base_register_ = stack_[stack_register_ + 1];
                    program_counter_ = stack_[stack_register_ + 2];
                    break;
                case 1:
                    stack_[stack_register_ - 1] =- stack_[stack_register_ - 1];
                    break;
                case 2:
                    stack_register_--;
                    stack_[stack_register_ - 1] += stack_[stack_register_];
                    break;
                case 3:
                    stack_register_--;
                    stack_[stack_register_ - 1] -= stack_[stack_register_];
                    break;
                case 4:
                    stack_register_--;
                    stack_[stack_register_ - 1] *= stack_[stack_register_];
                    break;
                case 5:
                    stack_register_--;
                    stack_[stack_register_ -1 ] /= stack_[stack_register_];
                    break;
                case 6:
                    if (stack_[stack_register_ - 1] % 2 == 0) {
                        stack_[stack_register_ - 1] = 0;
                    } else {
                        stack_[stack_register_ - 1] = 1;
                    }
                    break;
                case 8:
                    stack_register_--;
                    if (stack_[stack_register_ - 1] == stack_[stack_register_]) {
                        stack_[stack_register_ - 1] = 1;
                    } else {
                        stack_[stack_register_ - 1] = 0;
                    }
                    break;
                case 9:
                    stack_register_--;
                    if (stack_[stack_register_ - 1] != stack_[stack_register_]) {
                        stack_[stack_register_ - 1] = 1;
                    } else {
                        stack_[stack_register_ - 1] = 0;
                    }
                    break;
                case 10:
                    stack_register_--;
                    if (stack_[stack_register_ - 1] < stack_[stack_register_]) {
                        stack_[stack_register_ - 1] = 1;
                    } else {
                        stack_[stack_register_ - 1] = 0;
                    }
                    break;
                case 11:
                    stack_register_--;
                    if (stack_[stack_register_ - 1] >= stack_[stack_register_]) {
                        stack_[stack_register_ - 1] = 1;
                    } else {
                        stack_[stack_register_ - 1] = 0;
                    }
                    break;
                case 12:
                    stack_register_--;
                    if (stack_[stack_register_ - 1] > stack_[stack_register_]) {
                        stack_[stack_register_ - 1] = 1;
                    } else {
                        stack_[stack_register_ - 1] = 0;
                    }
                    break;
                case 13:
                    stack_register_--;
                    if (stack_[stack_register_ - 1] <= stack_[stack_register_]) {
                        stack_[stack_register_ - 1] = 1;
                    } else {
                        stack_[stack_register_ - 1] = 0;
                    }
                    break;
                case 14:
                    std::cout << stack_[--stack_register_];
                    break;
                case 15:
                    std::cout << '\n';
                    break;
                case 16:
                    std::cout << "???\t: ";
                    std::cin >> stack_[stack_register_++];
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

unsigned Machine::base(unsigned level_diff)const
{
    unsigned base = base_register_;
    while (level_diff > 0) {
        base = stack_[base];
        level_diff--;
    }
    return base;
}
