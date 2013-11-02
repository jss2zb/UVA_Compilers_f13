/* File: ast_stmt.h
 * ----------------
 * The Stmt class and its subclasses are used to represent
 * statements in the parse tree.  For each statment in the
 * language (for, if, return, etc.) there is a corresponding
 * node class for that construct. 
 *
 * pp2: You will need to add new expression and statement node c
 * classes for the additional grammar elements (Switch/Postfix)
 */


#ifndef _H_ast_stmt
#define _H_ast_stmt

#include "list.h"
#include "ast_type.h"

class Decl;
class VarDecl;
class Expr;
  
class Program : public Node
{
  protected:
     List<Decl*> *decls;
     Tree *scope;
     
  public:
     Program(List<Decl*> *declList);
     const char *GetPrintNameForNode() { return "Program"; }
     void PrintChildren(int indentLevel);
     void Build(); 
     void Check();
};

class Stmt : public Node
{
 public:
  Stmt() : Node() {};
  Stmt(yyltype loc) : Node(loc) {};
  //virtual Tree Build(Tree *tree) {return tree;}
  void Build(Tree *tree) {};
};

class StmtBlock : public Stmt 
{
  protected:
    List<VarDecl*> *decls;
    List<Stmt*> *stmts;
    
  public:
    StmtBlock(List<VarDecl*> *variableDeclarations, List<Stmt*> *statements);
    const char *GetPrintNameForNode() { return "StmtBlock"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
};

  
class ConditionalStmt : public Stmt
{
  protected:
    Expr *test;
    Stmt *body;
  
  public:
    ConditionalStmt(Expr *test, Stmt *body);
    virtual void Build(Tree *tree) {}
};

class LoopStmt : public ConditionalStmt 
{
  public:
    LoopStmt(Expr *testExpr, Stmt *body) : ConditionalStmt(testExpr, body) {}
  virtual void Build(Tree *tree) {}
};

class ForStmt : public LoopStmt 
{
  protected:
    Expr *init, *step;
  
  public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);
    const char *GetPrintNameForNode() { return "ForStmt"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
};

class WhileStmt : public LoopStmt 
{
  public:
    WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}
    const char *GetPrintNameForNode() { return "WhileStmt"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
};

class SwitchStmt : public LoopStmt
{
 protected:
   List<Stmt*> *cases;

 public:
   SwitchStmt(Expr *test, List<Stmt*> *cases, Stmt *body); 
   const char *GetPrintNameForNode() { return "SwitchStmt";}
   void PrintChildren(int indentLevel);
   void Build(Tree *tree);
};

class CaseStmt : public Stmt
{
 protected:
  Expr *test;
  List<Stmt*> *body;

 public: 
  CaseStmt(Expr *test, List<Stmt*> *body);
  const char *GetPrintNameForNode() { return "Case"; }
  void PrintChildren(int indentLevel);
  void Build(Tree *tree);
};

class DefaultStmt : public Stmt
{
 protected:
  List<Stmt*> *body;
 public:
  DefaultStmt(List<Stmt*> *body);
  const char *GetPrintNameForNode() { return "Default"; }
  void PrintChildren(int indentLevel);
  void Build(Tree *tree);
};


class IfStmt : public ConditionalStmt 
{
  protected:
    Stmt *elseBody;
  
  public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
    const char *GetPrintNameForNode() { return "IfStmt"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
};

class BreakStmt : public Stmt 
{
  public:
 BreakStmt(yyltype loc) : Stmt(loc) {};
    const char *GetPrintNameForNode() { return "BreakStmt"; }
    void Build(Tree *tree);
};

class ReturnStmt : public Stmt  
{
  protected:
    Expr *expr;
  
  public:
    ReturnStmt(yyltype loc, Expr *expr);
    const char *GetPrintNameForNode() { return "ReturnStmt"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
};

class PrintStmt : public Stmt
{
  protected:
    List<Expr*> *args;
    
  public:
    PrintStmt(List<Expr*> *arguments);
    const char *GetPrintNameForNode() { return "PrintStmt"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
};


#endif
