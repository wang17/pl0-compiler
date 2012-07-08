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

#ifndef CONDITION_H_
#define CONDITION_H_

#include<memory>

namespace pl0 {

class Expr;

class Condition {

public:
    enum class OpCode {
        Odd,
        Eq,
        Ne,
        Lt,
        Le,
        Gt,
        Ge
    };

public:
    Condition(OpCode op, Expr* rhs)
        : op_(op), lhs_(0), rhs_(rhs)
    {}

    Condition(OpCode op, Expr* lhs, Expr* rhs)
        : op_(op), lhs_(lhs), rhs_(rhs)
    {}

    ~Condition()
    {}

    OpCode getOpCode()const
    {
        return op_;
    }

    Expr* getLHS()
    {
        return lhs_.get();
    }

    const Expr* getLHS()const
    {
        return lhs_.get();
    }

    Expr* getRHS()
    {
        return rhs_.get();
    }

    const Expr* getRHS()const
    {
        return rhs_.get();
    }

private:
    OpCode op_;
    std::shared_ptr<Expr> lhs_;
    std::shared_ptr<Expr> rhs_;

private:
    Condition(const Condition&) = delete;
    void operator=(const Condition&) = delete;

};

}   //end namespace pl0

#endif // CONDITION_EXPR_H_
