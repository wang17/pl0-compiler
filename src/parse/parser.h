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

#ifndef PARSER_H_
#define PARSER_H_

#include<memory>
#include<set>
#include<string>
#include"../ast/ast.h"
#include"../lex/lexer.h"
#include"../symbol/symbol_table.h"
#include"diagnostic_reporter.h"

namespace pl0 {

class Parser {

public:
    explicit Parser(const std::string& file_data);

    void setFileData(const std::string& file_data);
    std::string getFileData()const;

    Block* parseProgram();

private:
    Block* parseBlock();

    Decl* parseConstDeclaraction();
    Decl* parseVariableDeclaraction();
    Decl* parseProcedureDeclaration();

    Stmt* parseStatement();
    Stmt* parseAssignStatement();
    Stmt* parseCallStatement();
    Stmt* parseBeginEndStatement();
    Stmt* parseIfThenStatement();
    Stmt* parseWhileDoStatement();
    Stmt* parseReadStatement();
    Stmt* parseWriteStatement();

    Condition* parseConditionExpression();

    Expr* parseExpression();
    Expr* parseTerm();
    Expr* parseFactor();

private:
    void consumeToken();
    bool isIn(const std::set<Token::Kind>& token_kind_set);
    bool isNotIn(const std::set<Token::Kind>& token_kind_set);
    void skipUntil(const std::set<Token::Kind>& token_kind_set, 
                   bool use_stop_set = true);

private:
    void reportDiagnostic(Diagnostic::Code c,
                          const Token& unexpected_token,
                          bool use_prev_location_ = false);

private:
    Lexer lexer_;
    Token token_;
    SymbolTable table_;
    DiagnosticReporter reporter_;
    std::shared_ptr<Block> program_;

    SourceLocation prev_location_;

private:
    Parser(const Parser&) = delete;
    void operator=(const Parser&) = delete;

};

}   //end namespace pl0

#endif // PARSER_H_
