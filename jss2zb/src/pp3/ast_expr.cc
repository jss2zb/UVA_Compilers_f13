/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "errors.h"
#include <string.h>


IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}

void IntConstant::PrintChildren(int indentLevel) { 
    printf("%d", value);
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}

void DoubleConstant::PrintChildren(int indentLevel) { 
    printf("%g", value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}

void BoolConstant::PrintChildren(int indentLevel) { 
    printf("%s", value ? "true" : "false");
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}

void StringConstant::PrintChildren(int indentLevel) { 
    printf("%s",value);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

void Operator::PrintChildren(int indentLevel) {
    printf("%s",tokenString);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) : Expr(Join(l->GetLocation(), r->GetLocation())) 
{
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) : Expr(Join(o->GetLocation(), r->GetLocation())) 
{
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

void CompoundExpr::PrintChildren(int indentLevel) 
{
   if (left) left->Print(indentLevel+1);
   op->Print(indentLevel+1);
   if (right) right->Print(indentLevel+1);
   }

  
Type* ArithmeticExpr::GetType()
{
  if(left)
    {
      if((strcmp(right->GetType()->GetIdentifier()->GetName(),"double") == 0) && (strcmp(left->GetType()->GetIdentifier()->GetName(),"double") == 0)) 
	{
	  return right->GetType();
	}
      else if((strcmp(right->GetType()->GetIdentifier()->GetName(),"int") == 0) && (strcmp(left->GetType()->GetIdentifier()->GetName(),"double") == 0))
	{
	  return left->GetType();
	}
      else if((strcmp(right->GetType()->GetIdentifier()->GetName(),"double") == 0) && (strcmp(left->GetType()->GetIdentifier()->GetName(),"int") == 0))
	{
	  return right->GetType();
	}
      else if((strcmp(right->GetType()->GetIdentifier()->GetName(),"int") == 0) && (strcmp(right->GetType()->GetIdentifier()->GetName(),"int") == 0))
	{
	  return right->GetType();
	}
      else if((strcmp(right->GetType()->GetIdentifier()->GetName(),"double") == 0) || (strcmp(right->GetType()->GetIdentifier()->GetName(),"int") == 0))
	{
	  return right->GetType();
	}
      else if((strcmp(left->GetType()->GetIdentifier()->GetName(),"double") == 0) || (strcmp(left->GetType()->GetIdentifier()->GetName(),"int") == 0))
	{
	  return left->GetType();
	}
      else 
	{
	  return new Type("error");
	}
    }
  return right->GetType();
  }

void ArithmeticExpr::Check(Tree *tree)
{
  if(left)
    {
      if(!((strcmp(left->GetType()->GetIdentifier()->GetName(),"double") == 0) || (strcmp(left->GetType()->GetIdentifier()->GetName(),"int") == 0)) || !((strcmp(right->GetType()->GetIdentifier()->GetName(),"double") == 0) || (strcmp(right->GetType()->GetIdentifier()->GetName(),"int") == 0)))
	{
	  ReportError::IncompatibleOperands(op,left->GetType(),right->GetType());
	}
    }  
}

void AssignExpr::Check(Tree *tree)
{
  left->Check(tree);
  right->Check(tree);
  
  Decl *myDecl = tree->Lookup(left->GetType()->GetIdentifier()->GetName());
  if(myDecl)
    {
      Type *lType = myDecl->GetType();
      if(!(strcmp(lType->GetIdentifier()->GetName(),right->GetType()->GetIdentifier()->GetName()) == 0))
	{
	  ReportError::IncompatibleOperands(op,lType,right->GetType());
	}
    }
}

   
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

void ArrayAccess::PrintChildren(int indentLevel) {
    base->Print(indentLevel+1);
    subscript->Print(indentLevel+1, "(subscript) ");
  }
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}

  void FieldAccess::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
  }

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

 void Call::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
    actuals->PrintAll(indentLevel+1, "(actuals) ");
  }


NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
}

void NewExpr::PrintChildren(int indentLevel) {	
    cType->Print(indentLevel+1);
}

NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this); 
    (elemType=et)->SetParent(this);
}

void NewArrayExpr::PrintChildren(int indentLevel) {
    size->Print(indentLevel+1);
    elemType->Print(indentLevel+1);
}

void FieldAccess::Check(Tree *tree)
{
  if(base == NULL)
    {
      Decl* myDecl = tree->Lookup(field->GetName());
      if(myDecl == NULL)
	{
	  reasonT t = LookingForVariable;
	  ReportError::IdentifierNotDeclared(field,t);
	}
    }
}

Type* FieldAccess::GetType()
{
  return new Type(field->GetName());
}

/*Type* ArrayAccess::GetType()
{
  return base->GetType();
  }*/
