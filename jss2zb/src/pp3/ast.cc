/* File: ast.cc
 * ------------
 */

#include "ast.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h> // strdup
#include <stdio.h>  // printf

Node::Node(yyltype loc) {
    location = new yyltype(loc);
    parent = NULL;
}

Node::Node() {
    location = NULL;
    parent = NULL;
}

/* The Print method is used to print the parse tree nodes.
 * If this node has a location (most nodes do, but some do not), it
 * will first print the line number to help you match the parse tree 
 * back to the source text. It then indents the proper number of levels 
 * and prints the "print name" of the node. It then will invoke the
 * virtual function PrintChildren which is expected to print the
 * internals of the node (itself & children) as appropriate.
 */
void Node::Print(int indentLevel, const char *label) { 
    const int numSpaces = 3;
    printf("\n");
    if (GetLocation()) 
        printf("%*d", numSpaces, GetLocation()->first_line);
    else 
        printf("%*s", numSpaces, "");
    printf("%*s%s%s: ", indentLevel*numSpaces, "", 
           label? label : "", GetPrintNameForNode());
   PrintChildren(indentLevel);
} 
	 
Identifier::Identifier(yyltype loc, const char *n) : Node(loc) {
    name = strdup(n);
} 

void Identifier::Build(Tree *tree){}
void Identifier::Check(Tree *tree){}

void Identifier::PrintChildren(int indentLevel) {
    printf("%s", name);
}

Tree::Tree()
{
  hash = new Hashtable<Decl*>();
  children = new List<Tree*>();
  parent = NULL;
}

 Tree::Tree(Tree *tree)
 {
   hash = new Hashtable<Decl*>();
   children = new List<Tree*>();
   parent = tree;
 }

void Tree::InsertChild(Tree *tree)
{
  children->Append(tree);
}

void Tree::InsertValue(Decl *type)
{
  hash->Enter((type->GetName())->GetName(),type,true);
}

yyltype* MyType::GetLocation()
{
  return id->GetLocation();
}

List<Tree*>* Tree::GetChildren()
{
  return children;
}

Decl* Tree::LocalLookup(char *key)
{
  Decl *temp = hash->Lookup(key);
  return temp;
}


Decl* Tree::Lookup(char *key)
{
  Decl *temp = hash->Lookup(key);
  if(temp == NULL)
    {
      if(parent != NULL)
	{
	  temp = parent->Lookup(key);
	}
    }
  return temp;
}

void Tree::printAll()
{
  //hash->Enter("here",new MyType(new Type("Cow"),i),true);
  Iterator<Decl*> myIterator = hash->GetIterator();
  Decl *mine;
  while( (mine = myIterator.GetNextValue()) != NULL)
    {
      printf("ID = %s\n",(mine->GetName())->GetName());
    }
}
