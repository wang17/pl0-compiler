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

#ifndef STMT_H_
#define STMT_H_

#include<memory>
#include<string>
#include<vector>

namespace pl0 {

class Expr;
class Condition;

class Stmt {

public:
    enum class StmtClass {
        AssignStmt,
        CallStmt,
        BeginEndStmt,
        IfThenStmt,
        WhileDoStmt,
        ReadStmt,
        WriteStmt
    };

public:
    explicit Stmt(StmtClass sc)
        : stmt_class_(sc)
    {}

    virtual ~Stmt()
    {}

    StmtClass getStmtClass()const
    {
        return stmt_class_;
    }

private:
    StmtClass stmt_class_;

private:
    Stmt(const Stmt&) = delete;
    void operator=(const Stmt&) = delete;

};


class AssignStmt: public Stmt {

public:
    AssignStmt(const std::string& name, Expr* e)
        : Stmt(StmtClass::AssignStmt),
          name_(name), expr_(e)
    {}

    virtual ~AssignStmt()
    {}

    std::string getName()const
    {
        return name_;
    }

    Expr* getExpr()
    {
        return expr_.get();
    }

    const Expr* getExpr()const
    {
        return expr_.get();
    }

private:
    std::string name_;
    std::shared_ptr<Expr> expr_;

};


class CallStmt: public Stmt {

public:
    explicit CallStmt(const std::string& name)
        : Stmt(StmtClass::CallStmt),
          name_(name)
    {}

    virtual ~CallStmt()
    {}

    std::string getName()const
    {
        return name_;
    }

private:
    std::string name_;

};


class BeginEndStmt: public Stmt {

public:
    explicit BeginEndStmt(const std::vector<Stmt*>& body)
        : Stmt(StmtClass::BeginEndStmt),
          body_(), body_helper_(body)
    {
        for (Stmt* s: body_helper_) {
            body_.push_back(std::shared_ptr<Stmt>(s));
        }
    }

    virtual ~BeginEndStmt()
    {}

    typedef std::vector<Stmt*>::iterator body_iterator;

    body_iterator getBodyBegin()
    {
        return body_helper_.begin();
    }

    body_iterator getBodyEnd()
    {
        return body_helper_.end();
    }

    typedef std::vector<Stmt*>::const_iterator const_body_iterator;

    const_body_iterator getBodyBegin()const
    {
        return body_helper_.cbegin();
    }

    const_body_iterator getBodyEnd()const
    {
        return body_helper_.cend();
    }

private:
    std::vector<std::shared_ptr<Stmt>> body_;

    std::vector<Stmt*> body_helper_;

};


class IfThenStmt: public Stmt {

public:
    IfThenStmt(Condition* cond, Stmt* then)
        : Stmt(StmtClass::IfThenStmt),
          cond_(cond), then_(then)
    {}

    virtual ~IfThenStmt()
    {}

    Condition* getCond()
    {
        return cond_.get();
    }

    const Condition* getCond()const
    {
        return cond_.get();
    }

    Stmt* getThen()
    {
        return then_.get();
    }

    const Stmt* getThen()const
    {
        return then_.get();
    }

private:
    std::shared_ptr<Condition> cond_;
    std::shared_ptr<Stmt> then_;

};


class WhileDoStmt: public Stmt {

public:
    WhileDoStmt(Condition* cond, Stmt* body)
        : Stmt(StmtClass::WhileDoStmt),
          cond_(cond), body_(body)
    {}

    virtual ~WhileDoStmt()
    {}

    Condition* getCond()
    {
        return cond_.get();
    }

    const Condition* getCond()const
    {
        return cond_.get();
    }

    Stmt* getBody()
    {
        return body_.get();
    }

    const Stmt* getBody()const
    {
        return body_.get();
    }

private:
    std::shared_ptr<Condition> cond_;
    std::shared_ptr<Stmt> body_;

};


class ReadStmt: public Stmt {

public:
    explicit ReadStmt(const std::vector<std::string>& body)
        : Stmt(StmtClass::ReadStmt),
          body_(body)
    {}

    virtual ~ReadStmt()
    {}

    typedef std::vector<std::string>::iterator body_iterator;

    body_iterator getBodyBegin()
    {
        return body_.begin();
    }

    body_iterator getBodyEnd()
    {
        return body_.end();
    }

    typedef std::vector<std::string>::const_iterator const_body_iterator;

    const_body_iterator getBodyBegin()const
    {
        return body_.cbegin();
    }

    const_body_iterator getBodyEnd()const
    {
        return body_.cend();
    }

private:
    std::vector<std::string> body_;

};


class WriteStmt: public Stmt {

public:
    explicit WriteStmt(const std::vector<Expr*> body)
        : Stmt(StmtClass::WriteStmt),
          body_(), body_helper_(body)
    {
        for (Expr* e: body_helper_) {
            body_.push_back(std::shared_ptr<Expr>(e));
        }
    }

    virtual ~WriteStmt()
    {}

    typedef std::vector<Expr*>::iterator body_iterator;

    body_iterator getBodyBegin()
    {
        return body_helper_.begin();
    }

    body_iterator getBodyEnd()
    {
        return body_helper_.end();
    }

    typedef std::vector<Expr*>::const_iterator const_body_iterator;

    const_body_iterator getBodyBegin()const
    {
        return body_helper_.cbegin();
    }

    const_body_iterator getBodyEnd()const
    {
        return body_helper_.cend();
    }

private:
    std::vector<std::shared_ptr<Expr>> body_;

    std::vector<Expr*> body_helper_;

};

}   //end namespace pl0

#endif // STMT_H_
