/**************************************************************************
 
  Name: Jonathan Harjono             NetID: jh4482
  Course: CSE 4714              
  Part 3: Building the Parse tree
  Programming Environment: Visual Studio Code with WSL Ubuntu 20.04 using g++
  Purpose of File: Parses through a TIPS program, checks for syntax errors, and build the parse tree.

***************************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H

#include <iostream>
#include "parse_tree_nodes.h"

// Somehow removing using namespace std makes it work

extern set<string> symbolTable;
int nextToken = 0;  // token returned from yylex
int level = 0;  // used to indent output to approximate parse tree


extern "C"
{
	// Instantiate global variables used by flex
	extern char* yytext;       // text of current lexeme
	extern int   yylex();      // the generated lexical analyzer
}

// Forward declarations of production parsing functions
ProgramNode* program();
BlockNode* block();
ExpressionNode* expression();
SimpleExpressionNode* simpleExpression();
AssignmentNode* assignment();
StatementNode* statement();
CompoundNode* compound();
TermNode* term();
FactorNode* factor();
WriteNode* write();
ReadNode* read();
IfThenNode* if_then();
WhileNode* while_loop();


// Forward declarations of functions that check whether the current token is
// in the first set of a production rule
bool first_of_program();
bool firstBlock();
bool firstAssignment();
bool firstExpression();
bool firstStatement();
bool firstIf();
bool firstFactor();
bool firstTerm();
bool firstCompound();


int lex() {
  nextToken = yylex();
  if (nextToken == TOK_EOF){
      yytext[0] = 'E';
      yytext[1] = 'O';
      yytext[2] = 'F';
      yytext[3] = 0;
  }

  return nextToken;
}

inline void indent(){
    for (int i = 0; i<level; i++)
        cout << ("    ");
}

void output(){
    indent();
    cout << "-->found " << yytext << endl;
}


ProgramNode* program() { // change to return programNode*

    if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";
    
    indent();
    cout << "enter <program>" << endl;
    ++level;

    
    // TODO: finish this parsing function...
    output();
    lex();
    if (nextToken != TOK_IDENT) {
        throw "2: identifier expected";
    }
    ProgramNode* prog = new ProgramNode(yytext, nullptr);
    output();
    lex();
    
    if (nextToken != TOK_SEMICOLON) {
        throw "14: ';' expected";
    }
    
    output();
    lex();
    prog->block = block();
    
    --level;
    indent();
    cout << "exit <program>" << endl;
    return prog;
    
}

AssignmentNode* assignment() { // change to return assignmentNode*
    if (!firstAssignment()) {
            throw "2: identifier expected";
        }

        indent();
        cout << "enter <assignment>" << endl;
        ++level;

        AssignmentNode* assigning = new AssignmentNode;
        if (symbolTable.count(yytext) < 1) { // Checks if identifier is in the symbol Table
            throw "104: identifier not declared";
        }
        
        assigning->identifier = new string(yytext);
        output();
        lex();
        

        if(nextToken != TOK_ASSIGN) {
            throw "51: ':=' expected";
        }
        output();
        lex();

        assigning->expression = expression();

        --level;
        indent();
        cout << "exit <assignment>" << endl;
        return assigning;
}

IfThenNode* if_then() {
    if (!firstIf()) {
        throw "900: illegal type of statement";
    }

    indent();
    cout << "enter <if statement>" << endl;
    ++level;

    IfThenNode* ifState = new IfThenNode();

    output();
    lex();
    ifState->expressioned = expression();

    if (nextToken != TOK_THEN) {
        throw "52: 'THEN' expected";
    }
    output();
    lex();
    ifState->thenState = statement();

    if (nextToken == TOK_ELSE) {
        output();
        lex();
        ifState->elseState = statement();
    }

    --level;
    indent();
    cout << "exit <if statement>" << endl;
    return ifState;
}

WhileNode* while_loop() {
    indent();
    cout << "enter <while statement>" << endl;
    ++level;

    WhileNode* whileLoop = new WhileNode;

    output();
    lex();

    whileLoop->expression =  expression();
    whileLoop->body = statement();

    --level;
    indent();
    cout << "exit <while statement>" << endl;
    return whileLoop;
}

ReadNode* read() {
    indent();
    cout << "enter <read>" << endl;
    ++level;

    ReadNode* reader = new ReadNode();
    output();
    lex();

    if (nextToken != TOK_OPENPAREN) {
        throw "9: '(' expected";
    }
    output();
    lex();

    if (nextToken != TOK_IDENT) {
        throw "2: identifier expected";
    }
    while (nextToken == TOK_IDENT) {
        if (symbolTable.count(yytext) < 1) { //Checks if identifier is in symbolTable
            throw "104: identifier not declared";
        }
        reader->identifier = new string("Value ");
        reader->identifier->append(yytext);
        output();
        lex();
    }

    if (nextToken != TOK_CLOSEPAREN) {
        throw "4: ')' expected";
    }
    output();
    lex();

    --level;
    indent();
    cout << "exit <read>" << endl;
    return reader;
}

WriteNode* write() {
    indent();
    cout << "enter <write>" << endl;
    ++level;
    WriteNode* writeNode = new WriteNode;

    output();
    lex();

    if (nextToken != TOK_OPENPAREN) {
        throw "9: '(' expected";
    }
    output();
    lex();

    if (nextToken != TOK_IDENT && nextToken != TOK_STRINGLIT) {
        throw "134: illegal type of operand(s)";
    }
    
    if (nextToken == TOK_IDENT) {
        writeNode->identifier = new string("Value ");
        writeNode->identifier->append(yytext);
    }
    else if (nextToken == TOK_STRINGLIT) {
        writeNode->identifier = new string("String ");
        writeNode->identifier->append(yytext);
    }
    
    output();
    lex();

    if (nextToken != TOK_CLOSEPAREN) {
        throw "4: ')' expected";
    }
    output();
    lex();

    --level;
    indent();
    cout << "exit <write>" << endl;
    return writeNode;
}

BlockNode* block() { // change to return blockNode*
    if(!firstBlock()) {
        throw "18: error in declaration part OR 17: 'BEGIN' expected";
    }
    
    indent();
    cout << "enter <block>" << endl;
    ++level;
    
    BlockNode* blocked = new BlockNode(nullptr);

    string name = yytext;
    if (nextToken == TOK_VAR) {
        output();
        lex();

        if (nextToken != TOK_IDENT) {
            throw "2: identifier expected";
        }

        while (nextToken == TOK_IDENT) {
            if (symbolTable.count(yytext) >= 1) { // Checks how many times the identifier has been declared
                throw "101: identifier declared twice";
            }
            symbolTable.insert(yytext);
            output();
            lex();

            if (nextToken != TOK_COLON) {
                throw "5: ':' expected";
            }
            output();
            lex();

            if (nextToken != TOK_INTEGER && nextToken != TOK_REAL) {
                throw "10: error in type";
            }
            output();
            lex();

            if (nextToken != TOK_SEMICOLON) {
                throw "14: ';' expected";
            }
            output();
            lex();
            cout << endl;
        }
    }

    if (nextToken == TOK_BEGIN) {
        blocked->compound = compound();
    }
    else {
        throw "17: 'BEGIN' expected";
    }
    
    --level;
    indent();
    cout << "exit <block>" << endl;

    return  blocked;
}

StatementNode* statement() { // change to return statementNode*
    if (!firstStatement()) {
        throw "900: illegal type of statement";
    }
    
    indent();
    cout << "enter <statement>" << endl;
    ++level;

    StatementNode* statementPtr = nullptr; 
    
    if (nextToken == TOK_IDENT) { //assignment
        statementPtr = assignment();
    }
    else if (nextToken == TOK_BEGIN) { // compound
        statementPtr = compound();
    }
    else if (nextToken == TOK_IF) {
        statementPtr = if_then();
    }
    else if (nextToken == TOK_WHILE) {
        statementPtr = while_loop();
    }
    else if (nextToken == TOK_READ) {
        statementPtr = read();
    }
    else if (nextToken == TOK_WRITE) {
        statementPtr = write();
    }
    else {
        throw "999: an error has occured";
    }
    
    --level;
    indent();
    cout << "exit <statement>" << endl;
    return statementPtr;

}

ExpressionNode* expression() { // change to return expressionNode*
    if (!firstExpression()) {
        throw "144: illegal type of expression";
    }

    indent();
    cout << "enter <expression>" << endl;
    ++level;
    
    ExpressionNode* expressionPtr = new ExpressionNode;
    expressionPtr->expressions.push_back(simpleExpression());

    if (nextToken == TOK_EQUALTO || nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_NOTEQUALTO) {
        //May change order if output is not right
        expressionPtr->operators.push_back(nextToken); // Pushes in the =,<,>,!= to the vectors
        output();
        lex();
        expressionPtr->expressions.push_back(simpleExpression()); // Pushes in the simple expressions
    }


    --level;
    indent();
    cout << "exit <expression>" << endl;
    return expressionPtr;
}

SimpleExpressionNode* simpleExpression() {
    if (!firstExpression()) {
        "901: illegal type of simple expression";
    }
    
    indent();
    cout << "enter <simple expression>" << endl;
    ++level;
    SimpleExpressionNode* simpleExpressionPtr = new SimpleExpressionNode;
    simpleExpressionPtr->termCollection.push_back(term());

    if (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR) {
        simpleExpressionPtr->termOperator.push_back(nextToken); // Pushes in the +.-.,OR operators to the vector
        output(); 
        lex();
        simpleExpressionPtr->termCollection.push_back(term()); // Pushes in the terms
    }
    
    --level;
    indent();
    cout << "exit <simple expression>" << endl;
    return simpleExpressionPtr;
}

TermNode* term() { // change to return termNode* 
    if (!firstTerm()) {
        throw "902: illegal type of term";
    }
    
    indent();
    cout << "enter <term>" << endl;
    ++level;

    TermNode* termPtr = new TermNode;
    termPtr->factorCollection.push_back(factor());
    
    while (nextToken) {

        if ((nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND) && symbolTable.count(yytext) >= 0) {
            //May need to copy output & lex to outside
            output();
            termPtr->factorOperator.push_back(nextToken); // Pushes in the *,/,AND to the vector
            lex();
            termPtr->factorCollection.push_back(factor()); // Pushes in the factors
        }
        else {
            break;
        }
        
    }

    --level;
    indent();
    cout << "exit <term>" << endl;
    return termPtr;
}

FactorNode* factor() { // change to return factorNode* 
    if (!firstFactor()) {
        throw "903: illegal type of factor";
    }
    
    indent();
    cout << "enter <factor>" << endl;
    ++level;
    FactorNode* factorPtr = nullptr;

    if (nextToken == TOK_INTLIT) {
        factorPtr = new IntLitNode(atoi(yytext)); // atoi changes yytext to int
        output();
        lex();
    }
    else if (nextToken == TOK_FLOATLIT) {
        factorPtr = new FloatLitNode(atof(yytext)); // atof changes yytext to float
        output();
        lex();
    }
    else if (nextToken == TOK_IDENT) {
        if (symbolTable.count(yytext) < 1) { //Checks if the identifier is in the symbolTable
            throw "104: identifier not declared";
        }
        factorPtr = new IdentifierNode(yytext);
        output();
        lex();
    }
    else if (nextToken == TOK_OPENPAREN) {
        output();
        lex();

        factorPtr = new NestedExpressionNode(expression());

        if (nextToken != TOK_CLOSEPAREN) {
            throw "4: ')' expected";
        }
        else if (nextToken == TOK_CLOSEPAREN) {
            output();
            lex();
        }
    }
    else if (nextToken == TOK_NOT) {
        int notToken = nextToken;
        output();
        lex();
        factorPtr = new NestedFactorNode(notToken, factor());
    }
    else if (nextToken == TOK_MINUS) {
        int minusTok = nextToken;
        output();
        lex();
        factorPtr = new NestedFactorNode(minusTok, factor());
    }

    --level;
    indent();
    cout << "exit <factor>" << endl;
    return factorPtr;
}

CompoundNode* compound() {
    if (!firstCompound()) {
        throw "17: 'BEGIN' expected";
    }
    
    indent();
    cout << "enter <compound_statement>" << endl;
    ++level;

    CompoundNode* compounding = new CompoundNode;
    output();
    lex();
    
    compounding->statements.push_back(statement());

    while (nextToken == TOK_SEMICOLON) {
        output();
        lex();
        compounding->statements.push_back(statement());
    }

    if (nextToken != TOK_END) {
        throw "13: 'END' expected";
    }
    output();
    lex();

    --level;
    indent();
    cout << "exit <compound_statement>" << endl;
    return compounding;
}


//*****************************************************************************

bool first_of_program(void) {
    if (nextToken == TOK_PROGRAM) {
        return true;
    }
    return false;
}

bool firstBlock(void) {
    if (nextToken == TOK_VAR || nextToken == TOK_BEGIN) {
        return true;
    }
    return false;
}

bool firstStatement(void) {
    if (nextToken == TOK_IDENT || nextToken == TOK_BEGIN || nextToken == TOK_IF || nextToken == TOK_WHILE || nextToken == TOK_READ || nextToken == TOK_WRITE) {
        return true;
    }
    return false;
}

bool firstAssignment(void) {
    if (nextToken == TOK_IDENT) {
        return true;
    }
    return false;
}

bool firstCompound(void) {
    if (nextToken == TOK_BEGIN) {
        return true;
    }
    return false;
}

bool firstIf(void) {
    if (nextToken == TOK_IF) {
        return true;
    }
    return false;
}

bool firstWhile(void) {
    if (nextToken == TOK_WHILE) {
        return true;
    }
    return false;
}

bool firstRead(void) {
    if (nextToken == TOK_READ) {
        return true;
    }
    return false;
}

bool firstWrite(void) {
    if (nextToken == TOK_WRITE) {
        return true;
    }
    return false;
}

bool firstExpression(void) {
    if(nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS) {
        return true;
    }
    return false;
}

bool firstTerm(void) {
    if(nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS) {
        return true;
    }
    return false;
}

bool firstFactor(void) {
    if(nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS) {
        return true;
    }
    return false;
}

#endif
