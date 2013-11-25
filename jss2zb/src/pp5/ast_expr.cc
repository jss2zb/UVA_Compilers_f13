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

Location* IntConstant::Emit(Tree *tree, CodeGenerator *cg)
{
  return cg->GenLoadConstant(value);
}

Location* NullConstant::Emit(Tree *tree, CodeGenerator *cg)
{
  return NULL;
}

Location* EmptyExpr::Emit(Tree *tree, CodeGenerator *cg)
{
  return NULL;
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}

Location* DoubleConstant::Emit(Tree *tree, CodeGenerator *cg)
{
  return cg->GenLoadConstant(value);
}

void DoubleConstant::PrintChildren(int indentLevel) { 
    printf("%g", value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}

Location* BoolConstant::Emit(Tree *tree, CodeGenerator *cg)
{
  return cg->GenLoadConstant(value);
}

void BoolConstant::PrintChildren(int indentLevel) { 
    printf("%s", value ? "true" : "false");
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}

Location* StringConstant::Emit(Tree *tree, CodeGenerator *cg)
{
  return cg->GenLoadConstant(value);
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

Location* ArithmeticExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  Location *loc;
  if(left)
    {
      loc = cg->GenBinaryOp(op->GetName(),left->Emit(tree,cg),right->Emit(tree,cg));
    }
  else
    {
      loc = cg->GenBinaryOp("-",cg->GenLoadConstant(0),right->Emit(tree,cg));
    }
return loc;
}

Location* AssignExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  cg->GenAssign(left->Emit(tree,cg),right->Emit(tree,cg));
  return NULL;
}

Location* RelationalExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  if(strcmp(op->GetName(),"<=") == 0)
    {
      Location *l = cg->GenBinaryOp("<",left->Emit(tree,cg),right->Emit(tree,cg));
      Location *r = cg->GenBinaryOp("==",left->Emit(tree,cg),right->Emit(tree,cg));
      return cg->GenBinaryOp("||",l,r);
    }
  else if(strcmp(op->GetName(),">") == 0)
    {
      return cg->GenBinaryOp("<",right->Emit(tree,cg),left->Emit(tree,cg));
    }
  else if(strcmp(op->GetName(),">=") == 0)
    {
      Location *l = cg->GenBinaryOp("<",right->Emit(tree,cg),left->Emit(tree,cg));
      Location *r = cg->GenBinaryOp("==",left->Emit(tree,cg),right->Emit(tree,cg));
      return cg->GenBinaryOp("||",l,r);
    }
  return cg->GenBinaryOp(op->GetName(),left->Emit(tree,cg),right->Emit(tree,cg));
}

Type* RelationalExpr::GetType(Tree *tree)
{
  return new Type("bool");
}

Location* LogicalExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  if(left)
    {
      return cg->GenBinaryOp(op->GetName(),left->Emit(tree,cg),right->Emit(tree,cg));
    }
  else
    {
      char *eLabel = cg->NewLabel();
      char *tLabel = cg->NewLabel();
      Location *l = right->Emit(tree,cg);
      cg->GenIfZ(l,eLabel);
      cg->GenAssign(l,cg->GenLoadConstant(0));
      cg->GenGoto(tLabel);
      cg->GenLabel(eLabel);
      cg->GenAssign(l,cg->GenLoadConstant(1));
      cg->GenLabel(tLabel);
      return l;
    }
  return NULL;
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

Location* Call::Emit(Tree *tree,CodeGenerator *cg)
{
  return NULL;
}

Type* Call::GetType(Tree *tree)
{
  if(base)
    {
      if(base->GetType(tree)->isArray() && strcmp(field->GetName(),"length")==0)
	{
	  return new Type("int");
	}
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

Location* FieldAccess::Emit(Tree *tree,CodeGenerator *cg)
{
  return tree->Lookup(field->GetName())->GetLabel();
}

Type* FieldAccess::GetType(Tree *tree)
{
  Decl *myDecl = tree->Lookup(field->GetName());
  Type *myT;
  if(myDecl == NULL)
    {
      myT = new Type("error");
    }
  else if(myDecl->IsFunction())
    {
      myT = new Type("error");
    }
  else
    {
      myT = myDecl->GetType();
    }
  return myT;
}

Location* ArrayAccess::Emit(Tree *tree,CodeGenerator *cg)
{
  return NULL;
}

Type* ArrayAccess::GetType(Tree *tree)
{
  return base->GetType(tree);
}

Location* NewArrayExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  return NULL;
}

Type* NewArrayExpr::GetType(Tree *tree)
{
  return new ArrayType((*location),elemType);
}

Location* This::Emit(Tree *tree,CodeGenerator *cg)
{
  return NULL;
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

Location* EqualityExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  if(strcmp(left->GetType(tree)->GetIdentifier()->GetName(),"string") == 0)
    {
      if(strcmp(op->GetName(),"==") == 0)
	{
	  return cg->GenBuiltInCall(StringEqual,left->Emit(tree,cg),right->Emit(tree,cg));
	}
      else
	{
	  char *eLabel = cg->NewLabel();
	  char *tLabel = cg->NewLabel();
	  Location *l = cg->GenBuiltInCall(StringEqual,left->Emit(tree,cg),right->Emit(tree,cg));
	  cg->GenIfZ(l,eLabel);
	  cg->GenAssign(l,cg->GenLoadConstant(0));
	  cg->GenGoto(tLabel);
	  cg->GenLabel(eLabel);
	  cg->GenAssign(l,cg->GenLoadConstant(1));
	  cg->GenLabel(tLabel);
	  return l;
	}
    }
  else
    {
      if(strcmp(op->GetName(),"==") == 0)
	{
	  return cg->GenBinaryOp(op->GetName(),left->Emit(tree,cg),right->Emit(tree,cg));
	}
      else
	{
	  char *eLabel = cg->NewLabel();
          char *tLabel = cg->NewLabel();
          Location *l = cg->GenBinaryOp("==",left->Emit(tree,cg),right->Emit(tree,cg));
          cg->GenIfZ(l,eLabel);
          cg->GenAssign(l,cg->GenLoadConstant(0));
          cg->GenGoto(tLabel);
          cg->GenLabel(eLabel);
          cg->GenAssign(l,cg->GenLoadConstant(1));
          cg->GenLabel(tLabel);
          return l;
	}
    }
  return cg->GenBinaryOp(op->GetName(),left->Emit(tree,cg),right->Emit(tree,cg));
}

Type* EqualityExpr::GetType(Tree *tree)
{
  return new Type("bool");
}

Location* NewExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  return NULL;
}

Type* NewExpr::GetType(Tree *tree)
{
  return cType;
}

Location* ReadIntegerExpr::Emit(Tree *tree, CodeGenerator *cg)
{
  return NULL;
}

Location* ReadLineExpr::Emit(Tree *tree, CodeGenerator *cg)
{
  return NULL;
}

