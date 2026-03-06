/**************************************************************************
 
  Name: Jonathan Harjono             NetID: jh4482
  Course: CSE 4714              
  Part 3: Building the Parse tree
  Programming Environment: Visual Studio Code with WSL Ubuntu 20.04 using g++
  Purpose of File: Parses through a TIPS program, checks for syntax errors, and build the parse tree.

***************************************************************************/

#ifndef PARSE_TREE_NODES_H 
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

// forward declarations of classes and functions
//
// TODO: add forward declarations so that your code will compile
//
class ProgramNode;
class BlockNode;
class CompoundNode;
class AssignmentNode;
class ExpressionNode;
class SimpleExpressionNode;
class TermNode;
class FactorNode;
class IfThenNode;
class WriteNode;
class ReadNode;
class WhileNode;
class StatementNode;
class IntLitNode;
class FloatLitNode;
class IdentifierNode;
class NestedExpressionNode;
class NestedFactorNode;


// returns a string that represents the integer token value
// i.e. passing in 3000 will result in " + " being returned
//
// this will be useful when printing out the math and 
//   logical operators
//
string token_to_string(int token) {
    switch(token){
        case TOK_PLUS:
            return " + ";       
        case TOK_MINUS:
            return " - ";
        case TOK_MULTIPLY:
            return " * ";
        case TOK_DIVIDE:
            return " / ";
        case TOK_ASSIGN:
            return " := ";
        case TOK_EQUALTO:
            return " = ";
        case TOK_LESSTHAN:
            return " < ";
        case TOK_GREATERTHAN:
            return " > ";
        case TOK_NOTEQUALTO:
            return " <> ";
        case TOK_MOD:
            return " MOD ";
        case TOK_NOT:
            return " NOT ";
        case TOK_OR:
            return " OR ";
        case TOK_AND:
            return " AND ";
        default:
            return " UNKNOWN TOKEN ";
    }
}

//Abstract classes
class StatementNode {
public:
   virtual void printTo(ostream& os) = 0;
   virtual ~StatementNode(){};
};
ostream& operator<<(ostream& os, StatementNode& node){
    node.printTo(os);
    return os;
}
class FactorNode {
public:
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~FactorNode(){}; // labeling the destructor as virtual allows 
	                       // the subclass destructors to be called
};
ostream& operator<<(ostream&os, FactorNode& node) {
    node.printTo(os);
    return os;
}

//Non abstract classes
class TermNode {
public: 
    vector<int> factorOperator;
    vector<FactorNode*> factorCollection;
    
    TermNode() {};
    ~TermNode() {
        cout << "Deleting a termNode" << endl;
        int length = factorCollection.size();
        for (int i = 0; i < length; ++i) {
            delete factorCollection[i];
            factorCollection[i] = nullptr;
        }
    }
};
ostream& operator<<(ostream& os, TermNode& node) {
    os << "term( " << *(node.factorCollection[0]);

    int length = node.factorCollection.size();
    // start the index at the second simple expression
    for (int i = 1; i < length; ++i) {
        // get the operators
        int ope = node.factorOperator[i-1];
        os << token_to_string(ope) << *(node.factorCollection[i]);
    }
    os << " )";
    return os;
}

class SimpleExpressionNode {
public:
    vector<int> termOperator;
    vector<TermNode*> termCollection;

    SimpleExpressionNode() {};

    ~SimpleExpressionNode() {
        cout << "Deleting a simpleExpressionNode" << endl;
        int length = termCollection.size();
        for (int i = 0; i < length; ++i) { //cleans up the vector
            delete termCollection[i];
            termCollection[i] = nullptr;
        }
    }
};
ostream& operator<<(ostream& os, SimpleExpressionNode& node) {
    os << "simple_expression( " << *(node.termCollection[0]);
    int length = node.termCollection.size();
    // start the index at the second simple expression
    for (int i = 1; i < length; ++i) {
        // get the operators
        int ope = node.termOperator[i-1];
        os << token_to_string(ope) << *(node.termCollection[i]);
    }
    os << " )";
    return os;
}

class ExpressionNode {
public:
    vector<int> operators; 
    vector<SimpleExpressionNode*> expressions;

    ExpressionNode() {};

    ~ExpressionNode() {
        cout << "Deleting an expressionNode" << endl;
        int length = expressions.size();
        for (int i = 0; i < length; ++i) {
            delete expressions[i];
            expressions[i] = nullptr;
        }
        operators.clear();
    };
};
ostream& operator<<(ostream&os, ExpressionNode& node) {
    os << "expression( " << *(node.expressions[0]);
    int length = node.expressions.size();
    for (int i = 1; i < length; ++i) {
        int ope = node.operators[i-1];
        os << token_to_string(ope);
        os << *(node.expressions[i]);
    }
    os << " )";
    return os;
}

class WhileNode : public StatementNode 
{
public:
    ExpressionNode* expression = nullptr;
    StatementNode* body = nullptr;
    void printTo(ostream& os) {
        os << "While " << *expression << endl;
        os << "%%%%%%%% Loop Body %%%%%%%%" << endl;
        os << *body;
        os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%";
    };
    WhileNode(){};
    ~WhileNode(){
        cout << "Deleting a whileNode" << endl;
        delete expression;
        expression = nullptr;
        delete body;
        body = nullptr;
    };
};

class IfThenNode : public StatementNode 
{
public:
    ExpressionNode* expressioned = nullptr;
    StatementNode* thenState = nullptr;
    StatementNode* elseState = nullptr;

    void printTo(ostream& os) {
        os << "If " << *(expressioned) << endl;
        os << "%%%%%%%% True Statement %%%%%%%%" << endl;
        os << *(thenState);
        os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;

        if(elseState != nullptr) {
            os << "%%%%%%%% False Statement %%%%%%%%" << endl;
            os << *(elseState);
            os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
        }
    };
    IfThenNode(){};
    ~IfThenNode(){
        cout << "Deleting an ifNode" << endl;
        delete expressioned;
        expressioned = nullptr;
        delete thenState;
        thenState = nullptr;
        delete elseState;
        elseState = nullptr;
    };

};

class ReadNode : public StatementNode 
{
public: 
    string* identifier = nullptr;

    void printTo(ostream& os) {
        os << "Read " << *(identifier);
    };
    ReadNode() {}
    ReadNode(string name) {
        identifier = new string(name);
    };
    ~ReadNode( ){
        cout << "Deleting a readNode" << endl;
        delete identifier;
        identifier = nullptr;
    };

};

class WriteNode : public StatementNode 
{
public:
    string* identifier = nullptr;
    void printTo(ostream& os) {
        os << "Write " << *identifier;
    };
    WriteNode(){};
    ~WriteNode(){
        cout << "Deleting a writeNode" << endl;
        delete identifier;
        identifier = nullptr;
    };

};

class IdentifierNode : public FactorNode {
public:
    string* identifier = nullptr;
    void printTo(ostream& os) {
        os << "factor( " << *identifier << " )";
    };
    IdentifierNode(string name) {
        identifier = new string(name);
    };
    ~IdentifierNode(){
        cout << "Deleting a factorNode" << endl;
        delete identifier;
        identifier = nullptr;
    };
};

class IntLitNode : public FactorNode {
public:
    int intLiteral = 0;
    void printTo(ostream& os) {
        os << "factor( " << intLiteral << " )";
    };
    IntLitNode(int data) {
        intLiteral = data;
    };
    ~IntLitNode() {
        cout << "Deleting a factorNode" << endl;
    }
};

class FloatLitNode : public FactorNode {
public:
    float floatLit = 0.0;
    void printTo(ostream& os) {
        os << "factor( " << floatLit << " )";
    };
    FloatLitNode(float data) {
        floatLit = data;
    }
    ~FloatLitNode() {
        cout << "Deleting a factorNode" << endl;
    }
};

class NestedExpressionNode : public FactorNode {
public:
    ExpressionNode* expressionNode = nullptr;
    void printTo(ostream& os) {
        os << "nested_expression( " << *(expressionNode) << " )";
    };
    NestedExpressionNode(ExpressionNode* en) {
        expressionNode = en;
    };
    ~NestedExpressionNode() {
        cout << "Deleting a factorNode" << endl;
        delete expressionNode;
        expressionNode = nullptr;
    };
};

class NestedFactorNode : public FactorNode {
public: 
    string* ope = nullptr;
    FactorNode* factorPtr = nullptr;
    void printTo(ostream& os) {
        os << "factor(" << *ope << *(factorPtr) << " )";
    };
    NestedFactorNode(int operatorArg, FactorNode* node){
        ope = new string(token_to_string(operatorArg));
        factorPtr = node;
    };
    ~NestedFactorNode() {
        cout << "Deleting a factorNode" << endl;
        delete ope;
        ope = nullptr;
        delete factorPtr;
        factorPtr = nullptr;
    };
};

class AssignmentNode : public StatementNode {
public: 
    string* identifier = nullptr;
    ExpressionNode* expression = nullptr;
    void printTo(ostream& os) {
        os << "Assignment " << *(identifier) << " := " << *(expression);
    }

    AssignmentNode() {}

    ~AssignmentNode() {
        cout << "Deleting an assignmentNode" << endl;
        delete identifier;
        identifier = nullptr;
        delete expression;
        expression = nullptr;
    }
};

class CompoundNode : public StatementNode {
public:
    vector<StatementNode*> statements;
    void printTo(ostream& os) {
        //For when a compound statement contains a compound element
        os << "Begin Compound Statement" << endl;
        int length = statements.size();
        for (int i = 0; i < length; ++i) {
            os << *(statements[i]) << endl;
        }
        os << "End Compound Statement" << endl;
        
    }
    CompoundNode() {}
    ~CompoundNode() {
        cout << "Deleting a compoundNode" << endl;
        int length = statements.size();
        for (int i = 0; i < length; ++i) {
            delete statements[i];
            statements[i] = nullptr;
        }
    };
};

class BlockNode {
public:
    CompoundNode* compound = nullptr;

    BlockNode(CompoundNode* comp) {
        compound = comp;
    }

    ~BlockNode() {
        cout << "Deleting a blockNode" << endl;
        delete compound;
        compound = nullptr;
    };

};
ostream& operator<<(ostream& os, BlockNode& node){
    os << *(node.compound);
    return os;
}

class ProgramNode{
public:
    string* name = nullptr;
    BlockNode* block = nullptr;

    // TODO: Complete these two methods
    //programNode(string id, blockNode* blk); 
    ProgramNode(string id, BlockNode* blk) {
        name = new string(id);
        block = blk;
    }
    ~ProgramNode() {
        cout << "Deleting a programNode" << endl;
        delete name;
        name = nullptr;
        delete block;
        block = nullptr;
    };
};
// example of << for a parse tree node (no inheritance is involved)
ostream& operator<<(ostream& os, ProgramNode& node){
    os << "Program Name " << *(node.name) << endl;
    os << *(node.block) << endl;
    return os;
}

#endif