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

#ifndef EXPR_H_
#define EXPR_H_

#include<memory>
#include<string>

namespace pl0 {

class Expr {

public:
    enum class ExprClass {
        ConstantExpr,
        VariableExpr,
        UnaryExpr,
        BinaryExpr
    };

public:
    explicit Expr(ExprClass ec)
        : expr_class_(ec)
    {}

    virtual ~Expr()
    {}

    ExprClass getExprClass()const
    {
        return expr_class_;
    }

private:
    ExprClass expr_class_;

private:
    Expr(const Expr&) = delete;
    void operator=(const Expr&) = delete;

};


class ConstantExpr: public Expr {

public:
    explicit ConstantExpr(const std::string& value)
        : Expr(ExprClass::ConstantExpr),
          value_(value)
    {}

    virtual ~ConstantExpr()
    {}

    std::string getValue()const
    {
        return value_;
    }

private:
    std::string value_;

};


class VariableExpr: public Expr {

public:
    explicit VariableExpr(const std::string& name)
        : Expr(ExprClass::VariableExpr),
          name_(name)
    {}

    virtual ~VariableExpr()
    {}

    std::string getName()const
    {
        return name_;
    }

private:
    std::string name_;

};


class UnaryExpr: public Expr {

public:
    enum class OpCode {
        Pos,
        Neg
    };

public:
    UnaryExpr(OpCode op, Expr* rhs)
        : Expr(ExprClass::UnaryExpr),
          op_(op), rhs_(rhs)
    {}

    virtual ~UnaryExpr()
    {}

    OpCode getOpCode()const
    {
        return op_;
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
    std::shared_ptr<Expr> rhs_;

};


class BinaryExpr: public Expr {

public:
    enum class OpCode {
        Add,
        Sub,
        Mul,
        Div
    };

public:
    BinaryExpr(OpCode op, Expr* lhs, Expr* rhs)
        : Expr(ExprClass::BinaryExpr),
          op_(op), lhs_(lhs), rhs_(rhs)
    {}

    virtual ~BinaryExpr()
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

};

}   //end namespace pl0

#endif // EXPR_H_
