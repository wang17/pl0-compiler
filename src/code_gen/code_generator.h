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

#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include<vector>
#include"../ast/ast.h"
#include"../basic/pcode.h"
#include"../symbol/symbol_table.h"

namespace pl0 {

class CodeGenerator {

public:
    CodeGenerator(Block* program);

    const Block* getProgram()const;

    std::vector<PCode> getProgramCode();

private:
    void genProgramCode(Block* block);

    void genBlockCode(Block* block);

    void genConstDeclaractionCode(ConstantDecl* decl);
    void genVariableDeclaractionCode(VariableDecl* decl);
    void genProcedureDeclaractionCode(ProcedureDecl* decl);

    void genStatementCode(Stmt* stmt);
    void genAssignStatementCode(AssignStmt* stmt);
    void genCallStatementCode(CallStmt* stmt);
    void genBeginEndStatementCode(BeginEndStmt* stmt);
    void genIfThenStatementCode(IfThenStmt* stmt);
    void genWhileDoStatementCode(WhileDoStmt* stmt);
    void genReadStatementCode(ReadStmt* stmt);
    void genWriteStatementCode(WriteStmt* stmt);

    void genConditionExpressionCode(Condition* cond);

    void genExpressionCode(Expr* expr);
    void genConstantExpressionCode(ConstantExpr* expr);
    void genVariableExpressionCode(VariableExpr* expr);
    void genUnaryExpressionCode(UnaryExpr* expr);
    void genBinaryExpression(BinaryExpr* expr);

private:
    unsigned genCode(PCode::OpCode f, unsigned l, unsigned a);
    unsigned nextAddress()const;
    void backPatchAddress(unsigned index, unsigned address);

private:
    Block* program_;
    std::vector<PCode> code_;
    SymbolTable table_;

private:
    CodeGenerator(const CodeGenerator&) = delete;
    void operator=(const CodeGenerator&) = delete;

};

}   //end namespace pl0

#endif // CODE_GENERATOR_H_
