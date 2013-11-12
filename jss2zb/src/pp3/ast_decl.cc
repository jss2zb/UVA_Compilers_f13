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
  Identifier* name = type->GetIdentifier();
  if(strcmp(name->GetName(), "int") != 0 && strcmp(name->GetName(),"double") != 0 && strcmp(name->GetName(),"string") != 0 && strcmp(name->GetName(),"bool") != 0)
    {
      Decl* myDecl = tree->Lookup(type->GetIdentifier()->GetName());
      if(myDecl == NULL)
	{
	  reasonT t = LookingForType;
	  ReportError::IdentifierNotDeclared(type->GetIdentifier(),t);
	}
    }
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

  if(extends)
    {
      temp = tree->LocalLookup(extends->GetIdentifier()->GetName());
      List<Decl*> *t = temp->GetMembers();
      for(int i = 0; i < t->NumElements();i++)
	{
	  t->Nth(i)->Build(theTree);
	  if(t->Nth(i)->IsFunction())
	    {
	      members->Append(t->Nth(i));
	    }
	}
    }
  scope = theTree;
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
  for(int i = 0; i < members->NumElements(); i++)
    {
      members->Nth(i)->Check(scope);
    }
}

bool ClassDecl::hasMembers(Identifier *id)
{
  for(int i = 0; i < members->NumElements(); i++)
    {
      if(strcmp(members->Nth(i)->GetName()->GetName(),id->GetName()) == 0)
	{
	  return true;
	}
    }
  return false;
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

bool InterfaceDecl::hasMembers(Identifier *id)
{
  for(int i = 0; i < members->NumElements(); i++)
    {
      if(strcmp(members->Nth(i)->GetName()->GetName(),id->GetName()) == 0)
        {
          return true;
        }
    }
  return false;
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
  scope = theTree;
  tree->InsertChild(theTree);
}

void FnDecl::Check(Tree *tree)
  {
    int index = 0;
    for(int i = 0; i < formals->NumElements(); i++)
      {
	Type* t = formals->Nth(i)->GetType();
	Identifier* name = t->GetIdentifier();
	
	if(strcmp(name->GetName(), "int") != 0 && strcmp(name->GetName(),"double") != 0 && strcmp(name->GetName(),"string") != 0 && strcmp(name->GetName(),"bool") != 0)
	  {
	
	    Decl* myDecl = scope->Lookup(formals->Nth(i)->GetType()->GetIdentifier()->GetName());
	    if(myDecl == NULL)
	      {
		reasonT t = LookingForType;
		ReportError::IdentifierNotDeclared(formals->Nth(i)->GetType()->GetIdentifier(), t);
	      }
	  }
      }
    
    if(body)
      {
	body->Check(scope);
      }
  }

void FnDecl::CheckReturn(ReturnStmt *r, Type *t)
{
  if(!(strcmp(t->GetIdentifier()->GetName(),returnType->GetIdentifier()->GetName()) == 0))
    {
      ReportError::ReturnMismatch(r,t,returnType);
    }

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


