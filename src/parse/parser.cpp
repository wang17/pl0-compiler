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

#include"parser.h"
using namespace pl0;

Parser::Parser(const std::string& file_data)
    : lexer_(file_data), table_(), reporter_(), program_(0), prev_location_(1)
{
    lexer_.lex(token_);
}

void Parser::setFileData(const std::string& file_data)
{
    lexer_.setFileData(file_data);
    lexer_.lex(token_);
    table_.clear();
    reporter_.resetCount();
    program_.reset();
    prev_location_.setLineNumber(1);
}

std::string Parser::getFileData()const
{
    return lexer_.getFileData();
}

/*!
 *  program -> block '.'
 */
Block* Parser::parseProgram()
{
    if (program_ != 0 || reporter_.getErrorCount() != 0) {
        return program_.get();
    }

    table_.beginScope();
    program_.reset(parseBlock());
    table_.endScope();

    if (token_.isNot(Token::Kind::dot)) {
        reportDiagnostic(Diagnostic::Code::C009, token_, true);
    } else {
        consumeToken();
    }

    if (token_.isNot(Token::Kind::eof)) {
        reportDiagnostic(Diagnostic::Code::C028, token_);
    }
    consumeToken();

    if (reporter_.getErrorCount() != 0) {
        program_.reset();
    }
    return program_.get();
}

/*!
 *  block -> [ const_declaraction ]
 *           [ var_declaraction ]
 *           { procedure_declaraction }
 *           statement
 */
Block* Parser::parseBlock()
{
    static const std::set<Token::Kind> first_set = {
        Token::Kind::kw_const, Token::Kind::kw_var,
        Token::Kind::kw_procedure, Token::Kind::identifier,
        Token::Kind::kw_call, Token::Kind::kw_begin,
        Token::Kind::kw_if, Token::Kind::kw_while,
        Token::Kind::kw_read, Token::Kind::kw_write,
        Token::Kind::dot, Token::Kind::semi
    };
    static const std::set<Token::Kind> follow_set = {
        Token::Kind::dot, Token::Kind::semi
    };

    skipUntil(first_set);
    if (isNotIn(first_set)) {
        return 0;
    }

    ConstantDecl* const_decl = 0;
    if (token_.is(Token::Kind::kw_const)) {
        const_decl = static_cast<ConstantDecl*>(parseConstDeclaraction());
    }

    VariableDecl* var_decl = 0;
    if (token_.is(Token::Kind::kw_var)) {
        var_decl = static_cast<VariableDecl*>(parseVariableDeclaraction());
    }

    std::vector<ProcedureDecl*> p_decls;
    while (token_.is(Token::Kind::kw_procedure)) {
        p_decls.push_back(static_cast<ProcedureDecl*>(parseProcedureDeclaration()));
    }

    Stmt* stmt = parseStatement();

    skipUntil(follow_set);

    return new Block(const_decl, var_decl, p_decls, stmt);
}

/*!
 *  const_declaraction -> 'const' ident '=' number { ',' ident '=' number } ';'
 */
Decl* Parser::parseConstDeclaraction()
{
    consumeToken();

    std::string name;
    std::string value;
    std::vector<std::pair<std::string, std::string>> body;

    if (token_.isIdentifier()) {
        name = token_.getRawData();
    } else {
        reportDiagnostic(Diagnostic::Code::C004, token_);
    }
    consumeToken();

    if (token_.isNot(Token::Kind::equal)) {
        if (token_.is(Token::Kind::colonequal)) {
            reportDiagnostic(Diagnostic::Code::C001, token_);
        } else {
            reportDiagnostic(Diagnostic::Code::C003, token_);
        }
    }
    consumeToken();

    if (token_.isNumber()) {
        value = token_.getRawData();
    } else {
        reportDiagnostic(Diagnostic::Code::C002, token_);
    }
    consumeToken();

    body.push_back(std::make_pair(name, value));
    table_.insertSymbol(new Symbol(Symbol::Kind::Constant, name, value));

    while (token_.is(Token::Kind::comma)) {
        consumeToken();

        if (token_.isIdentifier()) {
            name = token_.getRawData();
        } else {
            reportDiagnostic(Diagnostic::Code::C004, token_);
        }
        consumeToken();

        if (token_.isNot(Token::Kind::equal)) {
            if (token_.is(Token::Kind::colonequal)) {
                reportDiagnostic(Diagnostic::Code::C001, token_);
            } else {
                reportDiagnostic(Diagnostic::Code::C003, token_);
            }
        }
        consumeToken();

        if (token_.isNumber()) {
            value = token_.getRawData();
        } else {
            reportDiagnostic(Diagnostic::Code::C002, token_);
        }
        consumeToken();

        body.push_back(std::make_pair(name, value));
        table_.insertSymbol(new Symbol(Symbol::Kind::Constant, name, value));
    }

    if (token_.isNot(Token::Kind::semi)) {
        reportDiagnostic(Diagnostic::Code::C005, token_, true);
    } else {
        consumeToken();
    }

    return new ConstantDecl(body);
}

/*!
 *  var_declaraction -> 'var' ident { ',' ident } ';'
 */
Decl* Parser::parseVariableDeclaraction()
{
    consumeToken();

    std::string name;
    std::vector<std::string> body;

    if (token_.isIdentifier()) {
        name = token_.getRawData();
    } else {
        reportDiagnostic(Diagnostic::Code::C004, token_);
    }
    consumeToken();

    body.push_back(name);
    table_.insertSymbol(new Symbol(Symbol::Kind::Variable, name, 0, 0));

    while (token_.is(Token::Kind::comma)) {
        consumeToken();

        if (token_.isIdentifier()) {
            name = token_.getRawData();
        } else {
            reportDiagnostic(Diagnostic::Code::C004, token_);
        }
        consumeToken();

        body.push_back(name);
        table_.insertSymbol(new Symbol(Symbol::Kind::Variable, name, 0, 0));
    }

    if (token_.isNot(Token::Kind::semi)) {
        reportDiagnostic(Diagnostic::Code::C005, token_, true);
    } else {
        consumeToken();
    }

    return new VariableDecl(body);
}

/*!
 *  procedure_declaraction -> 'procedure' ident ';' block ';'
 */
Decl* Parser::parseProcedureDeclaration()
{
    consumeToken();

    std::string name;

    if (token_.isIdentifier()) {
        name = token_.getRawData();
    } else {
        reportDiagnostic(Diagnostic::Code::C004, token_);
    }
    consumeToken();
    table_.insertSymbol(new Symbol(Symbol::Kind::Procedure, name, 0, 0));

    if (token_.isNot(Token::Kind::semi)) {
        reportDiagnostic(Diagnostic::Code::C005, token_, true);
    } else {
        consumeToken();
    }

    table_.beginScope();
    Block* block = parseBlock();
    table_.endScope();

    if (token_.isNot(Token::Kind::semi)) {
        reportDiagnostic(Diagnostic::Code::C005, token_, true);
    } else {
        consumeToken();
    }

    return new ProcedureDecl(name, block);
}

/*!
 *  statement -> [ assign_statement
 *               | call_statement
 *               | begin_end_statement
 *               | if_then_statement
 *               | while_do_statement
 *               | read_statement
 *               | write_statement
 *               ]
 */
Stmt* Parser::parseStatement()
{
    static const std::set<Token::Kind> first_set = {
        Token::Kind::identifier, Token::Kind::kw_call,
        Token::Kind::kw_begin, Token::Kind::kw_if,
        Token::Kind::kw_while, Token::Kind::kw_read,
        Token::Kind::kw_write, Token::Kind::dot,
        Token::Kind::semi, Token::Kind::kw_end
    };
    static const std::set<Token::Kind> follow_set = {
        Token::Kind::dot, Token::Kind::semi,
        Token::Kind::kw_end
    };

    skipUntil(first_set);
    if (isNotIn(first_set)) {
        return 0;
    }

    Stmt* stmt = 0;
    switch (token_.getKind()) {
        case Token::Kind::identifier:
            stmt = parseAssignStatement();
            break;
        case Token::Kind::kw_call:
            stmt = parseCallStatement();
            break;
        case Token::Kind::kw_begin:
            stmt = parseBeginEndStatement();
            break;
        case Token::Kind::kw_if:
            stmt = parseIfThenStatement();
            break;
        case Token::Kind::kw_while:
            stmt = parseWhileDoStatement();
            break;
        case Token::Kind::kw_read:
            stmt = parseReadStatement();
            break;
        case Token::Kind::kw_write:
            stmt = parseWriteStatement();
            break;
        case Token::Kind::dot:
        case Token::Kind::semi:
        case Token::Kind::kw_end:
            stmt = 0 ;
            break;
        default:
            break;
    }

    skipUntil(follow_set);

    return stmt;
}

/*!
 *  assign_statement -> ident ':=' expression
 */
Stmt* Parser::parseAssignStatement()
{
    std::string name;

    name = token_.getRawData();
    Symbol* symbol = table_.findSymbol(name);
    if (symbol == 0) {
        reportDiagnostic(Diagnostic::Code::C011, token_);
    } else if (symbol->isNot(Symbol::Kind::Variable)) {
        reportDiagnostic(Diagnostic::Code::C012, token_);
    }
    consumeToken();

    if (token_.isNot(Token::Kind::colonequal)) {
        reportDiagnostic(Diagnostic::Code::C013, token_);
    }
    consumeToken();

    Expr* expr = parseExpression();

    return new AssignStmt(name, expr);
}

/*!
 *  call_statement -> 'call' ident
 */
Stmt* Parser::parseCallStatement()
{
    consumeToken();

    std::string name;

    if (token_.isIdentifier()) {
        name = token_.getRawData();
        Symbol* symbol = table_.findSymbol(name);
        if (symbol == 0) {
            reportDiagnostic(Diagnostic::Code::C011, token_);
        } else if (symbol->isNot(Symbol::Kind::Procedure)) {
            reportDiagnostic(Diagnostic::Code::C015, token_);
        }
        consumeToken();
    } else {
        reportDiagnostic(Diagnostic::Code::C014, token_);
    }

    return new CallStmt(name);
}

/*!
 *  begin_end_statement -> 'begin' statement { ';' statement } 'end'
 */
Stmt* Parser::parseBeginEndStatement()
{
    consumeToken();

    Stmt* stmt = 0;
    std::vector<Stmt*> body;

    stmt = parseStatement();
    body.push_back(stmt);

    while (token_.is(Token::Kind::semi)) {
        consumeToken();

        stmt = parseStatement();
        body.push_back(stmt);
    }

    if (token_.isNot(Token::Kind::kw_end)) {
        reportDiagnostic(Diagnostic::Code::C017, token_, true);
    } else {
        consumeToken();
    }

    return new BeginEndStmt(body);
}

/*!
 *  if_then_statement -> 'if' condition 'then' statement
 */
Stmt* Parser::parseIfThenStatement()
{
    consumeToken();

    Condition* cond = parseConditionExpression();

    if (token_.isNot(Token::Kind::kw_then)) {
        reportDiagnostic(Diagnostic::Code::C016, token_, true);
    } else {
        consumeToken();
    }

    Stmt* stmt = parseStatement();

    return new IfThenStmt(cond, stmt);
}

/*!
 *  while_do_statement -> 'while' condition 'do' statement
 */
Stmt* Parser::parseWhileDoStatement()
{
    consumeToken();

    Condition* cond = parseConditionExpression();

    if (token_.isNot(Token::Kind::kw_do)) {
        reportDiagnostic(Diagnostic::Code::C018, token_, true);
    } else {
        consumeToken();
    }

    Stmt* stmt = parseStatement();

    return new WhileDoStmt(cond, stmt);
}

/*!
 *  read_statement -> 'read' '(' ident { ',' ident } ')'
 */
Stmt* Parser::parseReadStatement()
{
    consumeToken();

    if (token_.isNot(Token::Kind::l_paren)) {
        reportDiagnostic(Diagnostic::Code::C022, token_, true);
    } else {
        consumeToken();
    }

    std::string name;
    std::vector<std::string> body;

    if (token_.isIdentifier()) {
        name = token_.getRawData();
        Symbol* symbol = table_.findSymbol(name);
        if (symbol == 0) {
            reportDiagnostic(Diagnostic::Code::C011, token_);
        } else if (symbol->isNot(Symbol::Kind::Variable)) {
            reportDiagnostic(Diagnostic::Code::C030, token_);
        }
    } else {
        reportDiagnostic(Diagnostic::Code::C010, token_);
    }
    consumeToken();

    body.push_back(name);

    while (token_.is(Token::Kind::comma)) {
        consumeToken();

        if (token_.isIdentifier()) {
            name = token_.getRawData();
            Symbol* symbol = table_.findSymbol(name);
            if (symbol == 0) {
                reportDiagnostic(Diagnostic::Code::C011, token_);
            } else if (symbol->isNot(Symbol::Kind::Variable)) {
                reportDiagnostic(Diagnostic::Code::C030, token_);
            }
        } else {
            reportDiagnostic(Diagnostic::Code::C010, token_);
        }
        consumeToken();

        body.push_back(name);
    }

    if (token_.isNot(Token::Kind::r_paren)) {
        reportDiagnostic(Diagnostic::Code::C023, token_, true);
    } else {
        consumeToken();
    }

    return new ReadStmt(body);
}

/*!
 *  write_statement -> 'write' '(' expression { ',' expression } ')'
 */
Stmt* Parser::parseWriteStatement()
{
    consumeToken();

    if (token_.isNot(Token::Kind::l_paren)) {
        reportDiagnostic(Diagnostic::Code::C022, token_, true);
    } else {
        consumeToken();
    }

    Expr* expr = 0;
    std::vector<Expr*> body;

    expr = parseExpression();
    body.push_back(expr);

    while (token_.is(Token::Kind::comma)) {
        consumeToken();

        expr = parseExpression();
        body.push_back(expr);
    }

    if (token_.isNot(Token::Kind::r_paren)) {
        reportDiagnostic(Diagnostic::Code::C023, token_, true);
    } else {
        consumeToken();
    }

    return new WriteStmt(body);
}

/*!
 *  condition -> 'odd' expression
 *             | expression ( '=' | '#' | '<' | '<=' | '>' | '>=') expression
 */
Condition* Parser::parseConditionExpression()
{
    static const std::set<Token::Kind> first_set = {
        Token::Kind::kw_odd, Token::Kind::plus,
        Token::Kind::minus, Token::Kind::identifier,
        Token::Kind::number, Token::Kind::l_paren
    };
    static const std::set<Token::Kind> follow_set = {
        Token::Kind::kw_then, Token::Kind::kw_do
    };

    skipUntil(first_set);
    if (isNotIn(first_set)) {
        return 0;
    }

    if (token_.is(Token::Kind::kw_odd)) {
        consumeToken();
        return new Condition(Condition::OpCode::Odd, parseExpression());
    }

    Expr* lhs = parseExpression();

    Condition::OpCode op = Condition::OpCode::Eq;
    switch (token_.getKind()) {
        case Token::Kind::equal:
            op = Condition::OpCode::Eq;
            break;
        case Token::Kind::pound:
            op = Condition::OpCode::Ne;
            break;
        case Token::Kind::less:
            op = Condition::OpCode::Lt;
            break;
        case Token::Kind::lessequal:
            op = Condition::OpCode::Le;
            break;
        case Token::Kind::greater:
            op = Condition::OpCode::Gt;
            break;
        case Token::Kind::greaterequal:
            op = Condition::OpCode::Ge;
            break;
        default:
            reportDiagnostic(Diagnostic::Code::C020, token_);
            break;
    }
    consumeToken();

    Expr* rhs = parseExpression();

    skipUntil(follow_set);

    return new Condition(op, lhs, rhs);
}

/*!
 *  expression -> [ '+' | '-' ] term { ( '+' | '-' ) term }
 */
Expr* Parser::parseExpression()
{
    static const std::set<Token::Kind> first_set = {
        Token::Kind::plus, Token::Kind::minus,
        Token::Kind::identifier, Token::Kind::number,
        Token::Kind::l_paren
    };
    static const std::set<Token::Kind> follow_set = {
        Token::Kind::kw_then, Token::Kind::kw_do,
        Token::Kind::equal, Token::Kind::pound,
        Token::Kind::less, Token::Kind::lessequal,
        Token::Kind::greater, Token::Kind::greaterequal,
        Token::Kind::dot, Token::Kind::semi,
        Token::Kind::kw_end, Token::Kind::comma,
        Token::Kind::r_paren
    };

    skipUntil(first_set);
    if (isNotIn(first_set)) {
        return 0;
    }

    BinaryExpr::OpCode op = BinaryExpr::OpCode::Add;
    Expr* lhs = 0;
    Expr* rhs = 0;

    if (token_.is(Token::Kind::plus)) {
        consumeToken();
        lhs = new UnaryExpr(UnaryExpr::OpCode::Pos, parseTerm());
    } else if(token_.is(Token::Kind::minus)) {
        consumeToken();
        lhs = new UnaryExpr(UnaryExpr::OpCode::Neg, parseTerm());
    } else {
        lhs = parseTerm();
    }

    while (token_.is(Token::Kind::plus) || token_.is(Token::Kind::minus)) {
        switch (token_.getKind()) {
            case Token::Kind::plus:
                op = BinaryExpr::OpCode::Add;
                break;
            case Token::Kind::minus:
                op = BinaryExpr::OpCode::Sub;
                break;
            default:
                break;
        }
        consumeToken();

        rhs = parseTerm();

        lhs = new BinaryExpr(op, lhs, rhs);
    }

    skipUntil(follow_set);

    return lhs;
}

/*!
 *  term -> factor { ( '*' | '/' ) factor }
 */
Expr* Parser::parseTerm()
{
    static const std::set<Token::Kind> first_set = {
        Token::Kind::identifier, Token::Kind::number,
        Token::Kind::l_paren
    };
    static const std::set<Token::Kind> follow_set = {
        Token::Kind::kw_then, Token::Kind::kw_do,
        Token::Kind::equal, Token::Kind::pound,
        Token::Kind::less, Token::Kind::lessequal,
        Token::Kind::greater, Token::Kind::greaterequal,
        Token::Kind::dot, Token::Kind::semi,
        Token::Kind::kw_end, Token::Kind::comma,
        Token::Kind::r_paren, Token::Kind::plus,
        Token::Kind::minus
    };

    skipUntil(first_set);
    if (isNotIn(first_set)) {
        return 0;
    }

    BinaryExpr::OpCode op = BinaryExpr::OpCode::Mul;
    Expr* lhs = 0;
    Expr* rhs = 0;

    lhs = parseFactor();

    while (token_.is(Token::Kind::star) || token_.is(Token::Kind::slash)) {
        switch (token_.getKind()) {
            case Token::Kind::star:
                op = BinaryExpr::OpCode::Mul;
                break;
            case Token::Kind::slash:
                op = BinaryExpr::OpCode::Div;
                break;
            default:
                break;
        }
        consumeToken();

        rhs = parseFactor();

        lhs = new BinaryExpr(op, lhs, rhs);
    }

    skipUntil(follow_set);

    return lhs;
}

/*!
 *  factor -> ident | number | '(' expression ')'
 */
Expr* Parser::parseFactor()
{
    static const std::set<Token::Kind> first_set = {
        Token::Kind::identifier, Token::Kind::number,
        Token::Kind::l_paren
    };
    static const std::set<Token::Kind> follow_set = {
        Token::Kind::kw_then, Token::Kind::kw_do,
        Token::Kind::equal, Token::Kind::pound,
        Token::Kind::less, Token::Kind::lessequal,
        Token::Kind::greater, Token::Kind::greaterequal,
        Token::Kind::dot, Token::Kind::semi,
        Token::Kind::kw_end, Token::Kind::comma,
        Token::Kind::r_paren, Token::Kind::plus,
        Token::Kind::minus, Token::Kind::star,
        Token::Kind::slash
    };

    skipUntil(first_set);
    if (isNotIn(first_set)) {
        return 0;
    }

    if (token_.isIdentifier()) {
        std::string name = token_.getRawData();
        Symbol* symbol = table_.findSymbol(name);
        if (symbol == 0) {
            reportDiagnostic(Diagnostic::Code::C011, token_);
        } else if (symbol->is(Symbol::Kind::Procedure)) {
            reportDiagnostic(Diagnostic::Code::C021, token_);
        }
        consumeToken();

        return new VariableExpr(name);
    }

    if (token_.isNumber()) {
        std::string value = token_.getRawData();
        consumeToken();

        return new ConstantExpr(value);
    }

    if (token_.is(Token::Kind::l_paren)) {
        consumeToken();

        Expr* expr = parseExpression();

        if (token_.isNot(Token::Kind::r_paren)) {
            reportDiagnostic(Diagnostic::Code::C023, token_, true);
        } else {
            consumeToken();
        }

        return expr;
    }

    skipUntil(follow_set);

    return 0;
}

void Parser::consumeToken()
{
    prev_location_ = token_.getSourceLocation();
    lexer_.lex(token_);
}

bool Parser::isIn(const std::set<Token::Kind>& token_kind_set)
{
    if (token_kind_set.find(token_.getKind()) != token_kind_set.end()) {
        return true;
    }
    return false;
}

bool Parser::isNotIn(const std::set<Token::Kind>& token_kind_set)
{
    if (token_kind_set.find(token_.getKind()) == token_kind_set.end()) {
        return true;
    }
    return false;
}

void Parser::skipUntil(const std::set<Token::Kind>& token_kind_set,
                       bool use_stop_set)
{
    static const std::set<Token::Kind> stop_set = {
        Token::Kind::kw_const, Token::Kind::kw_var,
        Token::Kind::kw_procedure, Token::Kind::identifier,
        Token::Kind::kw_call, Token::Kind::kw_begin,
        Token::Kind::kw_if, Token::Kind::kw_while,
        Token::Kind::kw_read, Token::Kind::kw_write
    };

    std::set<Token::Kind> sync_set = {
        Token::Kind::eof
    };
    sync_set.insert(token_kind_set.cbegin(), token_kind_set.cend());
    if (use_stop_set) {
        sync_set.insert(stop_set.cbegin(), stop_set.cend());
    }

    if (sync_set.find(token_.getKind()) != sync_set.cend()) {
        return;
    }
    reportDiagnostic(Diagnostic::Code::C029, token_);
    do {
        consumeToken();
    } while (sync_set.find(token_.getKind()) == sync_set.cend());
}

void Parser::reportDiagnostic(Diagnostic::Code c,
                              const Token& unexpected_token,
                              bool use_prev_location_)
{
    if (use_prev_location_) {
        reporter_.reportDiagnostic(Diagnostic(c,
                                              prev_location_,
                                              unexpected_token.getRawData()));
    } else {
        reporter_.reportDiagnostic(Diagnostic(c,
                                              unexpected_token.getSourceLocation(),
                                              unexpected_token.getRawData()));
    }
}
