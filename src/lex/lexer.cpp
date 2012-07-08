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

#include"lexer.h"
using namespace pl0;

std::map<std::string, Token::Kind> Lexer::keyword_map_;

Lexer::Lexer(const std::string& file_data)
    : buffer_(file_data)
{
    buffer_ptr_ = buffer_.getBufferBegin();
    location_.setLineNumber(1);

    initKeywordMap(keyword_map_);
}

void Lexer::lex(Token& result)
{
    result.startToken();
    lexTokenInternal(result);
}

void Lexer::setFileData(const std::string& file_data)
{
    buffer_.setBufferData(file_data);
    buffer_ptr_ = buffer_.getBufferBegin();
    location_.setLineNumber(1);
}

std::string Lexer::getFileData()const
{
    return buffer_.getBufferData();
}

void Lexer::lexTokenInternal(Token& result)
{
    const char* cur_ptr = buffer_ptr_;
    unsigned line_number = location_.getLineNumber();

    if (*cur_ptr == ' ' || *cur_ptr == '\t' || *cur_ptr == '\n') {
        if (*cur_ptr == '\n') {
            line_number++;
        }
        cur_ptr++;
        while (*cur_ptr == ' ' || *cur_ptr == '\t' || *cur_ptr == '\n') {
            if (*cur_ptr == '\n') {
                line_number++;
            }
            cur_ptr++;
        }
        buffer_ptr_ = cur_ptr;
    }

    location_.setLineNumber(line_number);

    Token::Kind kind = Token::Kind::unknown;
    switch (*cur_ptr++) {
        case 0:
            if (cur_ptr - 1 == buffer_.getBufferEnd()) {
                formTokenWithChars(result, cur_ptr, Token::Kind::eof);
                buffer_ptr_ = cur_ptr - 1;
                return;
            }
            kind = Token::Kind::unknown;
            break;
        case '+':
            kind = Token::Kind::plus;
            break;
        case '-':
            kind = Token::Kind::minus;
            break;
        case '*':
            kind = Token::Kind::star;
            break;
        case '/':
            kind = Token::Kind::slash;
            break;
        case '<':
            if (*cur_ptr == '=') {
                kind = Token::Kind::lessequal;
                cur_ptr++;
            } else {
                kind = Token::Kind::less;
            }
            break;
        case '>':
            if (*cur_ptr == '=') {
                kind = Token::Kind::greaterequal;
                cur_ptr++;
            } else {
                kind = Token::Kind::greater;
            }
            break;
        case '=':
            kind = Token::Kind::equal;
            break;
        case '#':
            kind = Token::Kind::pound;
            break;
        case ':':
            if (*cur_ptr == '=') {
                kind = Token::Kind::colonequal;
                cur_ptr++;
            } else {
                kind = Token::Kind::unknown;
            }
            break;
        case ';':
            kind = Token::Kind::semi;
            break;
        case ',':
            kind = Token::Kind::comma;
            break;
        case '(':
            kind = Token::Kind::l_paren;
            break;
        case ')':
            kind = Token::Kind::r_paren;
            break;
        case '.':
            kind = Token::Kind::dot;
            break;
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            lexNumber(result, cur_ptr);
            return;
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
            lexIdentifier(result, cur_ptr);
            return;
        default:
            kind = Token::Kind::unknown;
            break;
    }

    formTokenWithChars(result, cur_ptr, kind);
}

void Lexer::lexIdentifier(Token& result, const char* cur_ptr)
{
    while (isIdentifierBody(*cur_ptr)) {
        cur_ptr++;
    }

    auto iter = keyword_map_.find(std::string(buffer_ptr_, cur_ptr));
    if (iter != keyword_map_.end()) {
        formTokenWithChars(result, cur_ptr, iter->second);
        return;
    }

    formTokenWithChars(result, cur_ptr, Token::Kind::identifier);
}

void Lexer::lexNumber(Token& result, const char* cur_ptr)
{
    while (isNumberBody(*cur_ptr)) {
        cur_ptr++;
    }

    formTokenWithChars(result, cur_ptr, Token::Kind::number);
}

void Lexer::formTokenWithChars(Token& result, const char* tok_end, Token::Kind kind)
{
    result.setKind(kind);
    result.setRawData(std::string(buffer_ptr_, tok_end));
    result.setSourceLocation(location_);

    buffer_ptr_ = tok_end;
}

bool Lexer::isIdentifierBody(char ch)const
{
    return std::isalpha(ch);
}

bool Lexer::isNumberBody(char ch)const
{
    return std::isdigit(ch);
}

void Lexer::initKeywordMap(std::map<std::string, Token::Kind>& keyword_map)
{
    if (!keyword_map.empty()) {
        return;
    }

    keyword_map["begin"] = Token::Kind::kw_begin;
    keyword_map["call"] = Token::Kind::kw_call;
    keyword_map["const"] = Token::Kind::kw_const;
    keyword_map["do"] = Token::Kind::kw_do;
    keyword_map["end"] = Token::Kind::kw_end;
    keyword_map["if"] = Token::Kind::kw_if;
    keyword_map["procedure"] = Token::Kind::kw_procedure;
    keyword_map["read"] = Token::Kind::kw_read;
    keyword_map["then"] = Token::Kind::kw_then;
    keyword_map["var"] = Token::Kind::kw_var;
    keyword_map["while"] = Token::Kind::kw_while;
    keyword_map["write"] = Token::Kind::kw_write;
    keyword_map["odd"] = Token::Kind::kw_odd;
}
