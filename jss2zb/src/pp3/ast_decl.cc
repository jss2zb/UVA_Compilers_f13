/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "errors.h"      
         
Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this); 
}

VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}

void VarDecl::Build(Tree *tree)
{
  Decl *temp;
  if((temp = tree->LocalLookup(id->GetName())) == NULL)
    {
      tree->InsertValue(this);
    }
  else
    {
      ReportError::DeclConflict(this,temp);
    }
}

void VarDecl::Check(Tree *tree)
{
  //Eventually do Type Check
}
 
void VarDecl::PrintChildren(int indentLevel) { 
   type->Print(indentLevel+1);
   id->Print(indentLevel+1);
}

ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<NamedType*> *imp, List<Decl*> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && imp != NULL && m != NULL);     
    extends = ex;
    if (extends) extends->SetParent(this);
    (implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);
}

void ClassDecl::Build(Tree *tree)
{
  Decl *temp;
  temp = tree->LocalLookup(id->GetName());
  if(temp != NULL)
    {
      ReportError::DeclConflict(this,temp);
    }
  else
    {
      tree->InsertValue(this);
    }
  Tree *theTree = new Tree(tree);
  for(int i=0; i < members->NumElements(); i++)
    {
      (members->Nth(i))->Build(theTree);
    }
  tree->InsertChild(theTree);
}

void ClassDecl::Check(Tree *tree)
{
  if(extends)
    {
      Decl* myDecl = tree->Lookup((extends->GetName())->GetName());
      if(myDecl == NULL)
	{
	  reasonT t = LookingForClass;
	  ReportError::IdentifierNotDeclared(extends->GetName(), t);
	}
    }
  for(int i = 0; i < implements->NumElements(); i++)
    {
      Decl* myDecl = tree->Lookup((implements->Nth(i))->GetName()->GetName());
      if(myDecl == NULL)
	{
	  reasonT t = LookingForInterface;
	  ReportError::IdentifierNotDeclared((implements->Nth(i))->GetName(),t);
	}
    }
}

void ClassDecl::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
    if (extends) extends->Print(indentLevel+1, "(extends) ");
    implements->PrintAll(indentLevel+1, "(implements) ");
    members->PrintAll(indentLevel+1);
}


InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl*> *m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    (members=m)->SetParentAll(this);
}

void InterfaceDecl::Build(Tree *tree)
{
  Decl *temp;
  temp = tree->LocalLookup(id->GetName());
  if(temp != NULL)
    {
      ReportError::DeclConflict(this,temp);
    }
  else
    {
      tree->InsertValue(this);
    }
  Tree *theTree = new Tree(tree);
  for(int i = 0; i < members->NumElements(); i++)
    {
      (members->Nth(i))->Build(theTree);
    }
  tree->InsertChild(theTree);
}

void InterfaceDecl::Check(Tree *tree)
{
  //
}

void InterfaceDecl::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
    members->PrintAll(indentLevel+1);
}
	
FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
}

void FnDecl::Build(Tree *tree)
{
  Decl *temp;
  temp = tree->LocalLookup(id->GetName());
  if(temp != NULL)
    {
      ReportError::DeclConflict(this,temp);
    }
  else
    {
      tree->InsertValue(this);
    }
    
  Tree *theTree = new Tree(tree);
  for(int i = 0; i < formals->NumElements(); i++)
    {
      (formals->Nth(i))->Build(theTree);
    }
  if(body)
    {
      body->Build(theTree);
    }
  tree->InsertChild(theTree);
}

void FnDecl::Check(Tree *tree)
  {
    int index = 0;
    for(int i = 0; i < formals->NumElements(); i++)
      {
        Decl* myDecl = tree->Lookup((((formals->Nth(i)))->GetName())->GetName());
        if(myDecl == NULL)
          {
            reasonT t = LookingForType;
	    ReportError::IdentifierNotDeclared((formals->Nth(i))->GetName(), t);
	  }
      }
    //Check return?
  }

void FnDecl::SetFunctionBody(Stmt *b) { 
    (body=b)->SetParent(this);
}

void FnDecl::PrintChildren(int indentLevel) {
    returnType->Print(indentLevel+1, "(return type) ");
    id->Print(indentLevel+1);
    formals->PrintAll(indentLevel+1, "(formals) ");
    if (body) body->Print(indentLevel+1, "(body) ");
}


