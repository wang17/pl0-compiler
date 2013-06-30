#############################################################################
#
# Makefile for building: pl0-compiler
#
#############################################################################

####### Compiler, tools and options

CXX           = g++
LINK          = g++
CXXFLAGS      = -std=c++0x
LFLAGS        =
LIBS          =
DEL_FILE      = rm -f

####### Files

OBJECTS       = ./main.o \
                ./source_location.o \
                ./code_generator.o \
                ./buffer.o \
                ./lexer.o \
                ./token.o \
                ./machine.o \
                ./diagnostic.o \
                ./diagnostic_reporter.o \
                ./parser.o \
                ./symbol.o \
                ./symbol_table.o
TARGET        = pl0-compiler

####### Build rules

$(TARGET): $(OBJECTS)
	$(LINK) $(OBJECTS) -o $(TARGET) $(LFLAGS) $(LIBS)

main.o: ../src/main.cpp
	$(CXX) -c $(CXXFLAGS) -o ./main.o ../src/main.cpp

source_location.o: ../src/basic/source_location.cpp
	$(CXX) -c $(CXXFLAGS) -o ./source_location.o ../src/basic/source_location.cpp

code_generator.o: ../src/code_gen/code_generator.cpp
	$(CXX) -c $(CXXFLAGS) -o ./code_generator.o ../src/code_gen/code_generator.cpp

buffer.o: ../src/lex/buffer.cpp
	$(CXX) -c $(CXXFLAGS) -o ./buffer.o ../src/lex/buffer.cpp

lexer.o: ../src/lex/lexer.cpp
	$(CXX) -c $(CXXFLAGS) -o ./lexer.o ../src/lex/lexer.cpp

token.o: ../src/lex/token.cpp
	$(CXX) -c $(CXXFLAGS) -o ./token.o ../src/lex/token.cpp

machine.o: ../src/machine/machine.cpp
	$(CXX) -c $(CXXFLAGS) -o ./machine.o ../src/machine/machine.cpp

diagnostic.o: ../src/parse/diagnostic.cpp
	$(CXX) -c $(CXXFLAGS) -o ./diagnostic.o ../src/parse/diagnostic.cpp

diagnostic_reporter.o: ../src/parse/diagnostic_reporter.cpp
	$(CXX) -c $(CXXFLAGS) -o ./diagnostic_reporter.o ../src/parse/diagnostic_reporter.cpp

parser.o: ../src/parse/parser.cpp
	$(CXX) -c $(CXXFLAGS) -o ./parser.o ../src/parse/parser.cpp

symbol.o: ../src/symbol/symbol.cpp
	$(CXX) -c $(CXXFLAGS) -o ./symbol.o ../src/symbol/symbol.cpp

symbol_table.o: ../src/symbol/symbol_table.cpp
	$(CXX) -c $(CXXFLAGS) -o ./symbol_table.o ../src/symbol/symbol_table.cpp

clean:
	$(DEL_FILE) $(OBJECTS)
