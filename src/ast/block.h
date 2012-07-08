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

#ifndef BLOCK_H_
#define BLOCK_H_

#include<vector>
#include<memory>

namespace pl0 {

class Stmt;
class ConstantDecl;
class VariableDecl;
class ProcedureDecl;

class Block {

public:
    Block(ConstantDecl* c_decl, VariableDecl* v_decl,
          const std::vector<ProcedureDecl*>& p_decls, Stmt* body)
        : c_decl_(c_decl), v_decl_(v_decl),
          p_decls_(), body_(body), p_decls_helper_(p_decls)
    {
        for (ProcedureDecl* d: p_decls_helper_) {
            p_decls_.push_back(std::shared_ptr<ProcedureDecl>(d));
        }
    }

    ~Block()
    {}

    ConstantDecl* getConstantDecl()
    {
        return c_decl_.get();
    }

    const ConstantDecl* getConstantDecl()const
    {
        return c_decl_.get();
    }

    VariableDecl* getVariableDecl()
    {
        return v_decl_.get();
    }

    const VariableDecl* getVariableDecl()const
    {
        return v_decl_.get();
    }

    typedef std::vector<ProcedureDecl*>::iterator procedure_decls_iterator;

    procedure_decls_iterator getProcedureDeclsBegin()
    {
        return p_decls_helper_.begin();
    }

    procedure_decls_iterator getProcedureDeclsEnd()
    {
        return p_decls_helper_.end();
    }

    typedef std::vector<ProcedureDecl*>::const_iterator const_procedure_decls_iterator;

    const_procedure_decls_iterator getProcedureDeclsBegin()const
    {
        return p_decls_helper_.cbegin();
    }

    const_procedure_decls_iterator getProcedureDeclsEnd()const
    {
        return p_decls_helper_.cend();
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
    std::shared_ptr<ConstantDecl> c_decl_;
    std::shared_ptr<VariableDecl> v_decl_;
    std::vector<std::shared_ptr<ProcedureDecl>> p_decls_;
    std::shared_ptr<Stmt> body_;

    std::vector<ProcedureDecl*> p_decls_helper_;

private:
    Block(const Block&) = delete;
    void operator=(const Block&) = delete;

};

}   //end namespace pl0

#endif // BLOCK_H_
