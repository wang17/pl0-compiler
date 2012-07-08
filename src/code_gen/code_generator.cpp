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

#include"code_generator.h"
using namespace pl0;

CodeGenerator::CodeGenerator(Block* program)
    : program_(program), code_(), table_()
{}

const Block* CodeGenerator::getProgram()const
{
    return program_;
}

std::vector<PCode> CodeGenerator::getProgramCode()
{
    if (code_.empty()) {
        genProgramCode(program_);
    }
    return code_;
}

void CodeGenerator::genProgramCode(Block* block)
{
    table_.beginScope();
    genCode(PCode::OpCode::Int, 0, 3);
    genBlockCode(block);
    genCode(PCode::OpCode::Opr, 0, 0);
    table_.endScope();
}

void CodeGenerator::genBlockCode(Block* block)
{
    if (block->getConstantDecl() != 0) {
        genConstDeclaractionCode(block->getConstantDecl());
    }

    if (block->getVariableDecl() != 0) {
        genVariableDeclaractionCode(block->getVariableDecl());
    }

    auto begin_iter = block->getProcedureDeclsBegin();
    auto end_iter = block->getProcedureDeclsEnd();
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
        genProcedureDeclaractionCode(*iter);
    }

    if (block->getBody() != 0) {
        genStatementCode(block->getBody());
    }
}

void CodeGenerator::genConstDeclaractionCode(ConstantDecl* decl)
{
    auto begin_iter = decl->getBodyBegin();
    auto end_iter = decl->getBodyEnd();
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
        table_.insertSymbol(new Symbol(Symbol::Kind::Constant,
                                       iter->first,
                                       iter->second));
    }
}

void CodeGenerator::genVariableDeclaractionCode(VariableDecl* decl)
{
    unsigned level = table_.scopeLevel();
    unsigned address = 3;
    auto begin_iter = decl->getBodyBegin();
    auto end_iter = decl->getBodyEnd();
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
        genCode(PCode::OpCode::Int, 0, 1);
        table_.insertSymbol(new Symbol(Symbol::Kind::Variable,
                                       *iter,
                                       level,
                                       address));
        address++;
    }
}

void CodeGenerator::genProcedureDeclaractionCode(ProcedureDecl* decl)
{
    unsigned back_patch_address = genCode(PCode::OpCode::Jmp, 0, 0);
    table_.insertSymbol(new Symbol(Symbol::Kind::Procedure,
                                   decl->getName(),
                                   table_.scopeLevel(),
                                   back_patch_address + 1));
    table_.beginScope();
    genCode(PCode::OpCode::Int, 0, 3);
    genBlockCode(decl->getBody());
    genCode(PCode::OpCode::Opr, 0, 0);
    table_.endScope();
    backPatchAddress(back_patch_address, nextAddress());
}

void CodeGenerator::genStatementCode(Stmt* stmt)
{
    if (stmt == 0) {
        return ;
    }

    switch (stmt->getStmtClass()) {
        case Stmt::StmtClass::AssignStmt:
            genAssignStatementCode(static_cast<AssignStmt*>(stmt));
            return;
        case Stmt::StmtClass::CallStmt:
            genCallStatementCode(static_cast<CallStmt*>(stmt));
            return;
        case Stmt::StmtClass::BeginEndStmt:
            genBeginEndStatementCode(static_cast<BeginEndStmt*>(stmt));
            return;
        case Stmt::StmtClass::IfThenStmt:
            genIfThenStatementCode(static_cast<IfThenStmt*>(stmt));
            return;
        case Stmt::StmtClass::WhileDoStmt:
            genWhileDoStatementCode(static_cast<WhileDoStmt*>(stmt));
            return;
        case Stmt::StmtClass::ReadStmt:
            genReadStatementCode(static_cast<ReadStmt*>(stmt));
            return;
        case Stmt::StmtClass::WriteStmt:
            genWriteStatementCode(static_cast<WriteStmt*>(stmt));
            return;
        default:
            return;
    }
}

void CodeGenerator::genAssignStatementCode(AssignStmt* stmt)
{
    genExpressionCode(stmt->getExpr());
    Symbol* symbol = table_.findSymbol(stmt->getName());
    genCode(PCode::OpCode::Sto,
            table_.scopeLevel() - symbol->getLevel(),
            symbol->getAddress());
}

void CodeGenerator::genCallStatementCode(CallStmt* stmt)
{
    Symbol* symbol = table_.findSymbol(stmt->getName());
    genCode(PCode::OpCode::Cal,
            table_.scopeLevel() - symbol->getLevel(),
            symbol->getAddress());
}

void CodeGenerator::genBeginEndStatementCode(BeginEndStmt* stmt)
{
    auto begin_iter = stmt->getBodyBegin();
    auto end_iter = stmt->getBodyEnd();
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
        genStatementCode(*iter);
    }
}

void CodeGenerator::genIfThenStatementCode(IfThenStmt* stmt)
{
    genConditionExpressionCode(stmt->getCond());
    unsigned back_patch_address = genCode(PCode::OpCode::Jpc, 0, 0);
    genStatementCode(stmt->getThen());
    backPatchAddress(back_patch_address, nextAddress());
}

void CodeGenerator::genWhileDoStatementCode(WhileDoStmt* stmt)
{
    unsigned condition_address = nextAddress();
    genConditionExpressionCode(stmt->getCond());
    unsigned back_patch_address = genCode(PCode::OpCode::Jpc, 0, 0);
    genStatementCode(stmt->getBody());
    genCode(PCode::OpCode::Jmp, 0, condition_address);
    backPatchAddress(back_patch_address, nextAddress());
}

void CodeGenerator::genReadStatementCode(ReadStmt* stmt)
{
    Symbol* symbol = 0;
    auto begin_iter = stmt->getBodyBegin();
    auto end_iter = stmt->getBodyEnd();
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
        symbol = table_.findSymbol(*iter);
        genCode(PCode::OpCode::Opr, 0, 16);
        genCode(PCode::OpCode::Sto,
                table_.scopeLevel() - symbol->getLevel(),
                symbol->getAddress());
    }
}

void CodeGenerator::genWriteStatementCode(WriteStmt* stmt)
{
    auto begin_iter = stmt->getBodyBegin();
    auto end_iter = stmt->getBodyEnd();
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
        genExpressionCode(*iter);
        genCode(PCode::OpCode::Opr, 0, 14);
        genCode(PCode::OpCode::Opr, 0, 15);
    }
}

void CodeGenerator::genConditionExpressionCode(Condition* cond)
{
    genExpressionCode(cond->getLHS());
    if (cond->getOpCode() == Condition::OpCode::Odd) {
        genCode(PCode::OpCode::Opr, 0, 6);
        return;
    }

    genExpressionCode(cond->getRHS());
    switch (cond->getOpCode()) {
        case Condition::OpCode::Eq:
            genCode(PCode::OpCode::Opr, 0, 8);
            return;
        case Condition::OpCode::Ne:
            genCode(PCode::OpCode::Opr, 0, 9);
            return;
        case Condition::OpCode::Lt:
            genCode(PCode::OpCode::Opr, 0, 10);
            return;
        case Condition::OpCode::Le:
            genCode(PCode::OpCode::Opr, 0, 13);
            return;
        case Condition::OpCode::Gt:
            genCode(PCode::OpCode::Opr, 0, 12);
            return;
        case Condition::OpCode::Ge:
            genCode(PCode::OpCode::Opr, 0, 11);
            return;
        default:
            return;
    }
}

void CodeGenerator::genExpressionCode(Expr* expr)
{
    if (expr == 0) {
        return;
    }

    switch (expr->getExprClass()) {
        case Expr::ExprClass::ConstantExpr:
            genConstantExpressionCode(static_cast<ConstantExpr*>(expr));
            return;
        case Expr::ExprClass::VariableExpr:
            genVariableExpressionCode(static_cast<VariableExpr*>(expr));
            return;
        case Expr::ExprClass::UnaryExpr:
            genUnaryExpressionCode(static_cast<UnaryExpr*>(expr));
            return;
        case Expr::ExprClass::BinaryExpr:
            genBinaryExpression(static_cast<BinaryExpr*>(expr));
            return;
        default:
            return;
    }
}

void CodeGenerator::genConstantExpressionCode(ConstantExpr* expr)
{
    genCode(PCode::OpCode::Lit, 0, std::stoi(expr->getValue()));
}

void CodeGenerator::genVariableExpressionCode(VariableExpr* expr)
{
    Symbol* symbol = table_.findSymbol(expr->getName());
    if (symbol->getKind() == Symbol::Kind::Constant) {
        genCode(PCode::OpCode::Lit, 0, std::stoi(symbol->getValue()));
        return;
    }
    if (symbol->getKind() == Symbol::Kind::Variable) {
        genCode(PCode::OpCode::Lod,
                table_.scopeLevel() - symbol->getLevel(),
                symbol->getAddress());
        return;
    }
}

void CodeGenerator::genUnaryExpressionCode(UnaryExpr* expr)
{
    genExpressionCode(expr->getRHS());
    switch (expr->getOpCode()) {
        case UnaryExpr::OpCode::Pos:
            return;
        case UnaryExpr::OpCode::Neg:
            genCode(PCode::OpCode::Opr, 0, 1);
            return;
        default:
            return;
    }
}

void CodeGenerator::genBinaryExpression(BinaryExpr* expr)
{
    genExpressionCode(expr->getLHS());
    genExpressionCode(expr->getRHS());
    switch (expr->getOpCode()) {
        case BinaryExpr::OpCode::Add:
            genCode(PCode::OpCode::Opr, 0, 2);
            return;
        case BinaryExpr::OpCode::Sub:
            genCode(PCode::OpCode::Opr, 0, 3);
            return;
        case BinaryExpr::OpCode::Mul:
            genCode(PCode::OpCode::Opr, 0, 4);
            return;
        case BinaryExpr::OpCode::Div:
            genCode(PCode::OpCode::Opr, 0, 5);
            return;
        default:
            return;
    }
}

unsigned CodeGenerator::genCode(PCode::OpCode f, unsigned l, unsigned a)
{
    PCode c;
    c.f = f;
    c.l = l;
    c.a = a;
    code_.push_back(c);
    return code_.size() - 1;
}

unsigned CodeGenerator::nextAddress()const
{
    return code_.size();
}

void CodeGenerator::backPatchAddress(unsigned index, unsigned address)
{
    code_[index].a = address;
}
