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

#ifndef DECL_H_
#define DECL_H_

#include<memory>
#include<string>
#include<utility>
#include<vector>

namespace pl0 {

class Block;

class Decl {

public:
    enum class DeclClass {
        ConstantDecl,
        VariableDecl,
        ProcedureDecl
    };

public:
    explicit Decl(DeclClass dc)
        : decl_class_(dc)
    {}

    virtual ~Decl()
    {}

    DeclClass getDeclClass()const
    {
        return decl_class_;
    }

private:
    DeclClass decl_class_;

private:
    Decl(const Decl&) = delete;
    void operator=(const Decl&) = delete;

};


class ConstantDecl: public Decl {

public:
    explicit ConstantDecl(const std::vector<std::pair<std::string, std::string>>& body)
        : Decl(DeclClass::ConstantDecl), 
          body_(body)
    {}

    virtual ~ConstantDecl()
    {}

    typedef std::vector<std::pair<std::string, std::string>>::iterator body_iterator;

    body_iterator getBodyBegin()
    {
        return body_.begin();
    }

    body_iterator getBodyEnd()
    {
        return body_.end();
    }

    typedef std::vector<std::pair<std::string, std::string>>::const_iterator const_body_iterator;

    const_body_iterator getBodyBegin()const
    {
        return body_.cbegin();
    }

    const_body_iterator getBodyEnd()const
    {
        return body_.cend();
    }

private:
    std::vector<std::pair<std::string, std::string>> body_;

};


class VariableDecl: public Decl {

public:
    explicit VariableDecl(const std::vector<std::string>& body)
        : Decl(DeclClass::VariableDecl),
          body_(body)
    {}

    virtual ~VariableDecl()
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


class ProcedureDecl: public Decl {

public:
    ProcedureDecl(const std::string& name, Block* body)
        : Decl(DeclClass::ProcedureDecl),
          name_(name), body_(body)
    {}

    virtual ~ProcedureDecl()
    {}

    std::string getName()const
    {
        return name_;
    }

    Block* getBody()
    {
        return body_.get();
    }

    const Block* getBody()const
    {
        return body_.get();
    }

private:
    std::string name_;
    std::shared_ptr<Block> body_;

};

}   //end namespace pl0

#endif // DECL_H_
