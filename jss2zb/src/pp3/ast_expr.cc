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

  
Type* ArithmeticExpr::GetType(Tree *tree)
{
  if(left)
    {
      if((strcmp(right->GetType(tree)->GetIdentifier()->GetName(),"double") == 0) && (strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"double") == 0)) 
	{
	  return right->GetType(tree);
	}
      else if((strcmp(right->GetType(tree)->GetIdentifier()->GetName(),"int") == 0) && (strcmp(right->GetType(tree)->GetIdentifier()->GetName(),"int") == 0))
	{
	  return right->GetType(tree);
	}
      else if((strcmp(right->GetType(tree)->GetIdentifier()->GetName(),"double") == 0) || (strcmp(right->GetType(tree)->GetIdentifier()->GetName(),"int") == 0))
	{
	  return right->GetType(tree);
	}
      else if((strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"double") == 0) || (strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"int") == 0))
	{
	  return left->GetType(tree);
	}
      else 
	{
	  return new Type("error");
	}
    }
  return right->GetType(tree);
  }

void ArithmeticExpr::Check(Tree *tree)
{
  
  right->Check(tree);
  
  if(left)
    {
      left->Check(tree);
      if(!(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),right->GetType(tree)->GetIdentifier()->GetName()) == 0))
	{
	  ReportError::IncompatibleOperands(op,left->GetType(tree),right->GetType(tree));
	}
      else if(!(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"double") == 0) && !(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"int") == 0))
	{
	  ReportError::IncompatibleOperands(op,left->GetType(tree),right->GetType(tree));
	}      
    }  
  else
    {
      if(!(strcmp(right->GetType(tree)->GetIdentifier()->GetName(),"double") == 0) && !(strcmp(right->GetType(tree)->GetIdentifier()->GetName(),"int") == 0))
        {
	  ReportError::IncompatibleOperand(op,right->GetType(tree));
        }
    }
}

void AssignExpr::Check(Tree *tree)
{
  left->Check(tree);
  right->Check(tree);
  
  if(!(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),right->GetType(tree)->GetIdentifier()->GetName()) == 0))
    {
      Type *lType = left->GetType(tree);
      Type *rType = right->GetType(tree);
      if(lType->isArray())
	{
	  lType = new Type(strcat(lType->GetIdentifier()->GetName(),"[]"));
	}
      
      if(rType->isArray())
        {
          rType = new Type(strcat(rType->GetIdentifier()->GetName(),"[]"));
        }
      ReportError::IncompatibleOperands(op,lType,rType);

    }
}

void RelationalExpr::Check(Tree *tree)
{
  left->Check(tree);
  right->Check(tree);
  
  if(!(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),right->GetType(tree)->GetIdentifier()->GetName()) == 0))
    {
	  ReportError::IncompatibleOperands(op,left->GetType(tree),right->GetType(tree));
    }
  else if(!(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"int")==0) && !(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"double")==0))
    {
      ReportError::IncompatibleOperands(op,left->GetType(tree),right->GetType(tree));
    }
}

Type* RelationalExpr::GetType(Tree *tree)
{
  return new Type("bool");
}

void LogicalExpr::Check(Tree *tree)
{
  printf("HERE!\n");
  if(left) left->Check(tree);
  right->Check(tree);
  
  if(left)
    {
      if(!(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),right->GetType(tree)->GetIdentifier()->GetName()) == 0))
	{
	  ReportError::IncompatibleOperands(op,left->GetType(tree),right->GetType(tree));
	}
      else if(!(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"bool")==0))
	{
	  ReportError::IncompatibleOperands(op,left->GetType(tree),right->GetType(tree));
	}
    }
  else
    {
      printf("UNARY\n");
      if(!(strcmp(right->GetType(tree)->GetIdentifier()->GetName(),"bool")==0))
	{
	  ReportError::IncompatibleOperand(op,right->GetType(tree));
	}
    }

}

Type* LogicalExpr::GetType(Tree *tree)
{
  return new Type("bool");
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

void Call::Check(Tree *tree)
{
  for(int i = 0; i < actuals->NumElements();i++)
    {
      actuals->Nth(i)->Check(tree);
    }

  if(base)
    {
      Decl *myDecl = tree->Lookup(base->GetType(tree)->GetIdentifier()->GetName());
      if(myDecl == NULL)
	{
	  reasonT t = LookingForClass;
	  ReportError::IdentifierNotDeclared(base->GetType(tree)->GetIdentifier(),t);
	}
      else
	{
	  if(!myDecl->hasMembers(field))
	    {
	      ReportError::FieldNotFoundInBase(field,base->GetType(tree));
	    }
	}
    }
  else
    {
      
      Decl *myDecl = tree->Lookup(field->GetName());
      
      if(myDecl == NULL)
	{
	  reasonT t = LookingForFunction;
	  ReportError::IdentifierNotDeclared(field,t);
	}
    }
}

Type* Call::GetType(Tree *tree)
{
  if(base)
    {
      Decl *myDecl = tree->Lookup(base->GetType(tree)->GetIdentifier()->GetName());
      if(myDecl != NULL)
	{
	  if(myDecl->hasMembers(field))
	    {
	      myDecl = myDecl->GetScope()->Lookup(field->GetName());
	      if(myDecl != NULL)
		{
		  return myDecl->GetType();
		}
	    }
	}
    }
  else
    {
      Decl *myDecl = tree->Lookup(field->GetName());
      if(myDecl != NULL)
	{
	  return myDecl->GetType();
	}
    }
  return new Type("error");
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
  else
    {
      printf("%s\n",base->GetType(tree)->GetIdentifier()->GetName());
      Decl* myDecl = tree->Lookup(base->GetType(tree)->GetIdentifier()->GetName());
      if(myDecl)
	{
	  if(!myDecl->hasMembers(base->GetType(tree)->GetIdentifier()))
	    {
	      ReportError::FieldNotFoundInBase(field,base->GetType(tree));
	    }
	}
      else
	{
	  Type *lType = base->GetType(tree);
	  if(lType->isArray())
	    {
	      lType = new Type(strcat(lType->GetIdentifier()->GetName(),"[]"));
	    }
	  ReportError::FieldNotFoundInBase(field,lType);
	}
    }
}

Type* FieldAccess::GetType(Tree *tree)
{
  Decl *myDecl = tree->Lookup(field->GetName());
  Type *myT;
  if(myDecl == NULL)
    {
      myT = new Type("error");
    }
  else
    {
      myT = myDecl->GetType();
    }
  return myT;
}

void ArrayAccess::Check(Tree *tree)
{
  base->Check(tree);
  subscript->Check(tree);
  if(!(strcmp(subscript->GetType(tree)->GetIdentifier()->GetName(),"int")==0))
    {
      ReportError::SubscriptNotInteger(subscript);
    }
  
  if(!(base->GetType(tree)->isArray()))
    {
      ReportError::BracketsOnNonArray(base);
    }
}

Type* ArrayAccess::GetType(Tree *tree)
{
  return base->GetType(tree);
}

void NewArrayExpr::Check(Tree *tree)
{
  size->Check(tree);
  
  if(!(strcmp(size->GetType(tree)->GetIdentifier()->GetName(),"int")==0))
    {
      ReportError::NewArraySizeNotInteger(size);
    }
}

Type* NewArrayExpr::GetType(Tree *tree)
{
  return new ArrayType((*location),elemType);
}

void This::Check(Tree *tree)
{
  if(this->GetClass() == NULL)
    {
      ReportError::ThisOutsideClassScope(this);
    }
}

Type* This::GetType(Tree *tree)
{
  Type *tType = this->GetClass();
  if(tType == NULL)
    {
      tType = new Type("error");
    }
  return tType;
  
}
