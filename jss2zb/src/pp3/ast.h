/* File: ast.h
 * ----------- 
 * This file defines the abstract base class Node and the concrete 
 * Identifier and Error node subclasses that are used through the tree as 
 * leaf nodes. A parse tree is a hierarchical collection of ast nodes (or, 
 * more correctly, of instances of concrete subclassses such as VarDecl,
 * ForStmt, and AssignExpr).
 * 
 * Location: Each node maintains its lexical location (line and columns in 
 * file), that location can be NULL for those nodes that don't care/use 
 * locations. The location is typcially set by the node constructor.  The 
 * location is used to provide the context when reporting semantic errors.
 *
 * Parent: Each node has a pointer to its parent. For a Program node, the 
 * parent is NULL, for all other nodes it is the pointer to the node one level
 * up in the parse tree.  The parent is not set in the constructor (during a 
 * bottom-up parse we don't know the parent at the time of construction) but 
 * instead we wait until assigning the children into the parent node and then 
 * set up links in both directions. The parent link is typically not used 
 * during parsing, but is more important in later phases.
 *
 * Printing: The only interesting behavior of the node classes for pp2 is the 
 * bility to print the tree using an in-order walk.  Each node class is 
 * responsible for printing itself/children by overriding the virtual 
 * PrintChildren() and GetPrintNameForNode() methods. All the classes we 
 * provide already implement these methods, so your job is to construct the
 * nodes and wire them up during parsing. Once that's done, printing is a snap!

 */

#ifndef _H_ast
#define _H_ast

#include <stdlib.h>   // for NULL
#include "location.h"
#include "list.h"
#include "hashtable.h"

class Identifier;
class Type;
class Decl;
class ReturnStmt;

class MyType
{
 protected:
  Identifier *id;
  Type *type;

 public:
  MyType(Type *t,Identifier *i);

  const char *GetPrintNameForNode() {return "MyType"; }
  void PrintChildren(int indentLevel);
  char* GetName();
  yyltype* GetLocation();
};

class Tree
{
 protected:
  Hashtable<Decl*> *hash;
  Tree *parent;
  List<Tree*> *children;

 public:
  Tree();
  Tree(Tree *tree);

  List<Tree*>* GetChildren();
  void InsertValue(Decl *type);
  void InsertChild(Tree *tree);
  Decl* Lookup(char *key);
  Decl* LocalLookup(char *key);
  void printAll();

};

class Node 
{
  protected:
    yyltype *location;
    Node *parent;

  public:
    Node(yyltype loc);
    Node();
    
    yyltype *GetLocation()   { return location; }
    void SetParent(Node *p)  { parent = p; }
    Node *GetParent()        { return parent; }
    virtual bool IsInLoop() 
    {
      if(parent)
	{
	  return parent->IsInLoop();
	}
      return false;
    }
    
    virtual Type* GetClass()
    {
      if(parent)
	{
	  parent->GetClass();
	}
      return NULL;
    }
    
    virtual void CheckReturn(ReturnStmt *r, Type *t)
    {
      if(parent)
	{
	  return parent->CheckReturn(r,t);
	}
    }

    virtual const char *GetPrintNameForNode() = 0;
    
    // Print() is deliberately _not_ virtual
    // subclasses should override PrintChildren() instead
    void Print(int indentLevel, const char *label = NULL); 
    virtual void PrintChildren(int indentLevel)  {}
    //virtual void Build(Tree *tree) {}
    //virtual void Check(Tree *tree) {}
};

class Identifier : public Node 
{
  protected:
    char *name;
    
  public:
    Identifier(yyltype loc, const char *name);
    //Modified
    Identifier(const char *name);

    const char *GetPrintNameForNode()   { return "Identifier"; }
    void PrintChildren(int indentLevel);
    char *GetName() { return name; };
    void Build(Tree *tree);
    void Check(Tree *tree);
    yyltype* GetLocation() {return location;}
};

class Type : public Node
{
 protected:
  char *typeName;

 public :
  static Type *intType, *doubleType, *boolType, *voidType,
    *nullType, *stringType, *errorType;

 Type(yyltype loc) : Node(loc) {}
  Type(const char *str);

  const char *GetPrintNameForNode() { return "Type"; }
  void PrintChildren(int indentLevel);
  //Modified
  virtual Identifier* GetIdentifier() 
    {       
      char* tName = typeName;
      return new Identifier(tName);
    }
  virtual bool isArray() {return false;}
};


// This node class is designed to represent a portion of the tree that 
// encountered syntax errors during parsing. The partial completed tree
// is discarded along with the states being popped, and an instance of
// the Error class can stand in as the placeholder in the parse tree
// when your parser can continue after an error.
class Error : public Node
{
  public:
    Error() : Node() {}
    const char *GetPrintNameForNode()   { return "Error"; }
};


#endif
