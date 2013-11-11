/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>

#include "errors.h"


IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}
CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}
   
void ArithmeticExpr::Check()
{
  printf("HERE!\n");
  Type* rType = right->GetType();
  if(rType->GetName() == "Named")
    {
      Scope *scope = parent->GetScope();
      Decl *myDecl = scope->Lookup(rType->GetName
      rType = myDecl->GetDeclaredType();
    }
  if(left)
    {
      Type* lType = left->GetType();

      if(!(lType->IsEquivalentTo(Type::intType) || lType->IsEquivalentTo(Type::doubleType)) || !(rType->IsEquivalentTo(Type::intType) || rType->IsEquivalentTo(Type::doubleType)))
	{
	  ReportError::IncompatibleOperands(op,lType,rType);
	}
    }
  else
    {
      if(!(rType->IsEquivalentTo(Type::intType) || rType->IsEquivalentTo(Type::doubleType)))
	{
	  ReportError::IncompatibleOperand(op,rType);
	}
    }
}

ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}


void AssignExpr::Check()
{
  printf("HERE=!\n");
  right->Check();
  left->Check();
  if(!(left->GetType()->IsEquivalentTo(right->GetType())))
    {
      ReportError::IncompatibleOperands(op,left->GetType(),right->GetType());
    }
}


Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

void Call::Check()
{
    
  /*  Decl* myDecl = scope->Lookup(field);
  if(myDecl == NULL)
    {
      reasonT t = LookingForFunction;
      ReportError::IdentitifierNotDeclared(field,t);
    }
  */
} 

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
}


NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this); 
    (elemType=et)->SetParent(this);
}

       