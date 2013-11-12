/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 */

#ifndef _H_ast_decl
#define _H_ast_decl

#include "ast.h"
#include "list.h"

class Type;
class NamedType;
class Identifier;
class Stmt;
class ReturnStmt;

class Decl : public Node 
{
  protected:
    Identifier *id;
  
  public:
    Decl(Identifier *name);
    virtual Identifier* GetName() {return id;}

    //Modified
    virtual Type* GetType() {return new Type(id->GetName());};
    virtual void Build(Tree *tree) = 0;
    virtual void Check(Tree *tree) = 0;
    virtual bool hasMembers(Identifier *id) = 0;
    virtual Tree* GetScope() = 0;
};

class VarDecl : public Decl 
{
  protected:
    Type *type;

  public:
    VarDecl(Identifier *name, Type *type);
    const char *GetPrintNameForNode() { return "VarDecl"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
    void Check(Tree *tree);
    Identifier* GetName() {return id;};
    //Modified
    Tree* GetScope() {return NULL;};
    Type* GetType() {return type;};
    bool hasMembers(Identifier *id) {return false;};
};

class ClassDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    NamedType *extends;
    List<NamedType*> *implements;
    Tree *scope;

  public:
    ClassDecl(Identifier *name, NamedType *extends, 
              List<NamedType*> *implements, List<Decl*> *members);
    const char *GetPrintNameForNode() { return "ClassDecl"; }
    void PrintChildren(int indentLevel);
    Identifier* GetName() {return id;};
    void Build(Tree *tree);
    void Check(Tree *tree);
    Tree* GetScope() {return scope;};
    Type* GetClass() {return new Type(id->GetName());}
    bool hasMembers(Identifier *id);
};

class InterfaceDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    
  public:
    InterfaceDecl(Identifier *name, List<Decl*> *members);
    const char *GetPrintNameForNode() { return "InterfaceDecl"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
    void Check(Tree *tree);
    Tree* GetScope() {return NULL;};
    Identifier* GetName() {return id;};
    bool hasMembers(Identifier *id);
};

class FnDecl : public Decl 
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;
    Tree *scope;

  public:
    FnDecl(Identifier *name, Type *returnType, List<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);
    const char *GetPrintNameForNode() { return "FnDecl"; }
    void PrintChildren(int indentLevel);
    void Build(Tree *tree);
    void Check(Tree *tree);
    Identifier* GetName() {return id;};
    Tree* GetScope() {return NULL;};
    void CheckReturn(ReturnStmt *r, Type *t);
    bool hasMembers(Identifier *id) {return false;}
};

#endif
