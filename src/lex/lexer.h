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

#ifndef LEXER_H_
#define LEXER_H_

#include<map>
#include<string>
#include"buffer.h"
#include"token.h"

namespace pl0 {

class Lexer {

public:
    explicit Lexer(const std::string& file_data);

    void lex(Token& result);

    void setFileData(const std::string& file_data);
    std::string getFileData()const;

private:
    void lexTokenInternal(Token& result);

    void lexIdentifier(Token& result, const char* cur_ptr);
    void lexNumber(Token& result, const char* cur_ptr);

    void formTokenWithChars(Token& result, const char* tok_end, Token::Kind kind);

private:
    bool isIdentifierBody(char ch)const;
    bool isNumberBody(char ch)const;

    void initKeywordMap(std::map<std::string, Token::Kind>& keyword_map);

private:
    Buffer buffer_;
    const char* buffer_ptr_;
    SourceLocation location_;

private:
    static std::map<std::string, Token::Kind> keyword_map_;

private:
    Lexer(const Lexer&) = delete;
    void operator=(const Lexer&) = delete;

};

}   //end namespace pl0

#endif // LEXER_H_
