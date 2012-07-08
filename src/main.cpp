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

#include<cstring>
#include<fstream>
#include<iomanip>
#include<iostream>
#include<sstream>
#include<string>
#include"ast/ast.h"
#include"code_gen/code_generator.h"
#include"lex/lexer.h"
#include"machine/machine.h"
#include"parse/parser.h"
using namespace pl0;


void display_usage();
std::string read_file_data(const char* file_name);
void output_token_stream(Lexer& lexer, bool lex);
void output_ast(Parser& parser, bool parse);
void output_pcode(CodeGenerator& generator, bool analytics);
void execute_pcode(Machine& machine, bool execute);


int main(int argc, char* argv[])
{
    bool lex = false;
    bool parse = false;
    bool analytics = false;
    bool execute = false;
    const char* file_name = 0;

    char opt = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && std::strlen(argv[i]) == 2) {
            opt = argv[i][1];
        } else if (argv[i][0] != '-' && file_name == 0) {
            file_name = argv[i];
            continue;;
        } else {
            opt = '?';
        }
        switch (opt) {
            case 'l':
                lex = true;
                break;
            case 'p':
                parse = true;
                break;
            case 'a':
                analytics = true;
                break;
            case 'e':
                execute = true;
                break;
            case '?':
            default:
                display_usage();
                exit(EXIT_FAILURE);
        }
    }
    if (file_name == 0) {
        display_usage();
        exit(EXIT_FAILURE);
    }

    std::string file_data = read_file_data(file_name);

    std::cout << "Run pl0-compiler ...\n\n";

    if (!lex && !parse && !analytics && !execute) {
        std::cout << "Finish pl0-compiler ...\n\n" << std::flush;
        return 0;
    }
    Lexer lexer(file_data);
    output_token_stream(lexer, lex);

    if (!parse && !analytics && !execute) {
        std::cout << "Finish pl0-compiler ...\n\n" << std::flush;
        return 0;
    }
    Parser parser(file_data);
    output_ast(parser, parse);

    if (parser.parseProgram() == 0) {
        std::cout << "Finish pl0-compiler ...\n\n" << std::flush;
        return 0;
    }
    CodeGenerator generator(parser.parseProgram());
    output_pcode(generator, analytics);

    Machine machine(generator.getProgramCode());
    execute_pcode(machine, execute);

    std::cout << "Finish pl0-compiler ...\n\n" << std::flush;

    return 0;
}


void display_usage()
{
    std::cout << "\n"
              << "Usage: \n"
              << "pl0-compiler [ flags ] source_code_file\n"
              << "\n"
              << "Flags: \n"
              << "-l: lex source code, and output token stream.\n"
              << "-p: parse token stream, and output abstract syntax tree.\n"
              << "-a: analytics abstract syntax tree, and output pcode.\n"
              << "-e: execute pcode.\n"
              << "-?: help.\n";
}

std::string read_file_data(const char* file_name)
{
    std::ifstream ifs(file_name);
    std::string file_data;
    if (ifs) {
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        file_data = buffer.str();
    } else {
        std::cout << "Cannot open file: " << file_name << "\n\n";
        exit(EXIT_FAILURE);
    }
    ifs.close();
    return file_data;
}

void output_token_stream(Lexer& lexer, bool lex)
{
    std::cout << "Start lex source code ...\n\n";

    if (!lex) {
        return;
    }

    const char* const file_name = "token_stream.txt";
    std::ofstream ofs(file_name);
    if (!ofs) {
        std::cout << "Cannot create output file: " << file_name << "\n\n";
        exit(EXIT_FAILURE);
    }

    std::stringstream source_buffer;
    std::stringstream token_buffer;
    token_buffer.setf(std::ios::left);
    Token token;

    lexer.lex(token);
    unsigned line_number = token.getSourceLocation().getLineNumber();
    unsigned token_line_number = line_number;
    while (true) {
        token_line_number = token.getSourceLocation().getLineNumber();

        if (line_number < token_line_number) {
            ofs << line_number << ":" << source_buffer.str() << "\n";
            ofs << token_buffer.str();
            line_number = token_line_number;
            source_buffer.str("");
            token_buffer.str("");
        }

        if (token.is(Token::Kind::eof)) {
            ofs << "    " << std::setw(2) <<token_line_number << ": eof\n";
            break;
        }

        source_buffer << " " << token.getRawData();

        token_buffer << "    " << std::setw(2) << token_line_number << ": "
                     << std::setw(16) << token.getTokenKindName()
                     << token.getRawData() << "\n";

        lexer.lex(token);
    }

    std::cout << "Generate token stream file: " << file_name << " ...\n\n";
}

void output_ast(Parser& /*parser*/, bool parse)
{
    std::cout << "Start parse token stream ...\n\n";

    if (!parse) {
        return;
    }
}

void output_pcode(CodeGenerator& generator, bool analytics)
{
    std::cout << "Start analytics abstract syntax tree ...\n\n";

    if (!analytics) {
        return;
    }

    const char* const file_name = "pcode.txt";
    std::ofstream ofs(file_name);
    if (!ofs) {
        std::cout << "Cannot create output file: " << file_name << "\n\n";
        exit(EXIT_FAILURE);
    }

    std::vector<PCode> code = generator.getProgramCode();
    PCode c;
    for (size_t i = 0; i < code.size(); i++) {
        c = code[i];
        ofs << std::setw(3) << i << ":  ";
        switch (c.f) {
            case PCode::OpCode::Lit:
                ofs << "Lit";
                break;
            case PCode::OpCode::Lod:
                ofs << "Lod";
                break;
            case PCode::OpCode::Sto:
                ofs << "Sto";
                break;
            case PCode::OpCode::Cal:
                ofs << "Cal";
                break;
            case PCode::OpCode::Int:
                ofs << "Int";
                break;
            case PCode::OpCode::Jmp:
                ofs << "Jmp";
                break;
            case PCode::OpCode::Jpc:
                ofs << "Jpc";
                break;
            case PCode::OpCode::Opr:
                ofs << "Opr";
                break;
        }
        ofs << "  " << std::setw(2) << c.l
            << "  " << c.a << "\n";
    }

    std::cout << "Generate pcode file: " << file_name << " ...\n\n";
}

void execute_pcode(Machine& machine, bool execute)
{
    std::cout << "Start execute pcode ...\n\n";

    if (!execute) {
        return;
    }

    machine.start();

    std::cout << "\n";
}
