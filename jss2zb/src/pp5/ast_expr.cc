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
  return cg->GenLoadConstant(0);
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
      Location *le;
      Location *ri;
      if(left->InClass())
	{
	  Decl *dec;
	  if(left->IsField())
	    {
	      dec = tree->Lookup(left->GetName()->GetName());
	      if(dec->HasOffset())
		{
		  le = cg->GenLoad(CodeGenerator::ThisPtr,dec->GetOffset());
		}
	      else
		{
		  le = left->Emit(tree,cg);
		}
	    }
	  else
	    {
	      le = left->Emit(tree,cg);
	    }

	  if(right->IsField())
	    {
	      dec = tree->Lookup(right->GetName()->GetName());
	      if(dec->HasOffset())
		{
		  ri = cg->GenLoad(CodeGenerator::ThisPtr,dec->GetOffset());
		}
	      else
		{
		  ri = right->Emit(tree,cg);
		}
	    }
	  else
	    {
	      ri = right->Emit(tree,cg);
	    }
	}
      else
	{
	  le = left->Emit(tree,cg);
	  ri = right->Emit(tree,cg);
	  
	}
      loc = cg->GenBinaryOp(op->GetName(),le,ri);
    }
  else
    {
      Location *ri;
      if(right->InClass())
	{
	  if(right->IsField())
	    {
	      Decl *dec = tree->Lookup(right->GetName()->GetName());
	      if(dec->HasOffset())
		{
		  ri = cg->GenLoad(CodeGenerator::ThisPtr,dec->GetOffset());
		}
	      else
		{
		  ri = right->Emit(tree,cg);
		}
	    }
	  else
	    {
	      ri = right->Emit(tree,cg);
	    }
	}
      else
	{
	  ri = right->Emit(tree,cg);
	}
loc = cg->GenBinaryOp("-",cg->GenLoadConstant(0),ri);
    }
return loc;
}

Location* AssignExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  Location *le = left->Emit(tree,cg);
  Location *ri = right->Emit(tree,cg);
  if(right->IsArray())
    {
      ri = cg->GenLoad(ri);
    }

  if(left->IsArray())
    {
      cg->GenStore(le,ri);
    }
  else if(left->InClass() && left->IsField())
    {
      Decl *dec = tree->Lookup(left->GetName()->GetName());
      /*      if(right->IsField())
	{
	  Decl *mydec = tree->Lookup(right->GetName()->GetName());
	  if(dec->HasOffset())
	    {
	      ri = cg->GenLoad(CodeGenerator::ThisPtr,dec->GetOffset());
	    }
	    }*/
      
      if(dec->HasOffset())
	{
	  cg->GenStore(CodeGenerator::ThisPtr,ri,dec->GetOffset());
	}
      else 
	{
	  cg->GenAssign(le,ri);
	}
    }
  else
    {
      cg->GenAssign(le,ri);
    }
  return NULL;
}

Location* RelationalExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  Location *le;
  Location *ri;
  if(this->InClass())
    {
      if(left->IsField())
	{
	  Decl *dec = tree->Lookup(left->GetName()->GetName());
	  if(dec->HasOffset())
	    {
	      le = cg->GenLoad(CodeGenerator::ThisPtr,dec->GetOffset());
	    }
	  else
	    {
	      le = left->Emit(tree,cg);
	    }
	}
      else
	{
	  le = left->Emit(tree,cg);
	}

      if(right->IsField())
	{
	  Decl *dec = tree->Lookup(right->GetName()->GetName());
          if(dec->HasOffset())
            {
              ri = cg->GenLoad(CodeGenerator::ThisPtr,dec->GetOffset());
            }
          else
            {
              ri = right->Emit(tree,cg);
            }
	}
      else
	{
	  ri = right->Emit(tree,cg);
	}
    }
  else
    {
      le = left->Emit(tree,cg);
      ri = right->Emit(tree,cg);
    }
    
  if(strcmp(op->GetName(),"<=") == 0)
    {
      Location *l = cg->GenBinaryOp("<",le,ri);
      Location *r = cg->GenBinaryOp("==",le,ri);
      return cg->GenBinaryOp("||",l,r);
    }
  else if(strcmp(op->GetName(),">") == 0)
    {
      return cg->GenBinaryOp("<",ri,le);
    }
  else if(strcmp(op->GetName(),">=") == 0)
    {
      Location *l = cg->GenBinaryOp("<",ri,le);
      Location *r = cg->GenBinaryOp("==",le,ri);
      return cg->GenBinaryOp("||",l,r);
    }
  Location *locate = cg->GenBinaryOp(op->GetName(),le,ri);
  return locate;
}

Type* RelationalExpr::GetType(Tree *tree)
{
  return new Type("bool");
}

Location* LogicalExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  if(left)
    {
      Location *le;
      Location *ri;

      if(this->InClass())
	{
	  if(left->IsField())
	    {
	      Decl *dec = tree->Lookup(left->GetName()->GetName());
	      if(dec->HasOffset())
		{
		  le = cg->GenLoad(CodeGenerator::ThisPtr,dec->GetOffset());
		}
	      else
		{
		  le = left->Emit(tree,cg);
		}
	    }
	  
	  if(right->IsField())
	    {
	      Decl *dec = tree->Lookup(right->GetName()->GetName());
              if(dec->HasOffset())
                {
                  ri = cg->GenLoad(CodeGenerator::ThisPtr,dec->GetOffset());
                }
              else
                {
                  ri = left->Emit(tree,cg);
                }

	    }
	}
      else
	{
	  le = left->Emit(tree,cg);
	  ri = right->Emit(tree,cg);
	}
      
      return cg->GenBinaryOp(op->GetName(),le,ri);
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
  Location *loc;
  if(base == NULL && !this->InClass())
    {
      bool hasReturn = true;
      if(strcmp(tree->Lookup(field->GetName())->GetType()->GetIdentifier()->GetName(),"void") == 0)
	{
	  hasReturn = false;
	}
      
      List<Location*> *lactuals = new List<Location*>();
      
      for(int i = 0; i < actuals->NumElements(); i++)
	{
	  lactuals->Append(actuals->Nth(i)->Emit(tree,cg));
	}
      
      int n = actuals->NumElements();
	  
      for(int i = 0; i < actuals->NumElements(); i++)
	{
	  cg->GenPushParam(lactuals->Nth(n-i-1));
	}

      const char *c = "_";
      size_t len1 = strlen(c);
      size_t len2 = strlen(field->GetName());
      char *f = (char*)malloc(len1+len2+1);
      strcpy(f,c);
      strcat(f,field->GetName());
      f[len1+len2] = '\0';
      loc = cg->GenLCall(f,hasReturn);
      cg->GenPopParams(4*actuals->NumElements());
    }
  else if(strcmp(field->GetName(),"length") == 0)
    {
      /*Location *local;
      if(this->InClass())
	{
	  Decl *classDecl = tree->Lookup(base->GetName()->GetName());
	  if(classDecl->HasOffset())
	    {
	      local = cg->GenLoad(CodeGenerator::ThisPtr,classDecl->GetOffset());
	    }
	  else
	    {
	      local = base->Emit(tree,cg);
	    }
	}
      else
	{
	  local = base->Emit(tree,cg);
	  }*/
      loc = cg->GenLoad(base->Emit(tree,cg),-4);
    }
else if(base == NULL)
    {
      bool hasReturn = true;
      Decl *classDecl = tree->Lookup(this->GetClass()->GetIdentifier()->GetName());
      Tree *myTree = classDecl->GetScope();
      if(strcmp(myTree->Lookup(field->GetName())->GetType()->GetIdentifier()->GetName(),"void") == 0)
        {
          hasReturn = false;
        }
      List<Decl*> *members = classDecl->GetMembers();
      int offset = 0;
      for(int i = 0; i < members->NumElements(); i++)
        {
          if(members->Nth(i)->IsFunction())
            {
              if((strcmp(members->Nth(i)->GetName()->GetName(),field->GetName()) == 0) && (members->Nth(i)->GetFormals()->NumElements() == actuals->NumElements()))
                {
                  break;
                }
              else
                {
                  offset++;
                }
            }
        }
      
      List<Location*> *lactuals = new List<Location*>();
      for(int i = 0; i < actuals->NumElements(); i++)
        {
          lactuals->Append(actuals->Nth(i)->Emit(tree,cg));
        }
      Location *b = cg->GenLoad(CodeGenerator::ThisPtr);
      Location *c = cg->GenLoad(b,offset*4);
      int n = actuals->NumElements();

      for(int i = 0; i < actuals->NumElements(); i++)
        {
          cg->GenPushParam(lactuals->Nth(n-i-1));
        }
      cg->GenPushParam(CodeGenerator::ThisPtr);
      loc = cg->GenACall(c,hasReturn);
      cg->GenPopParams(4*(n+1));
    }
  else 
    {
      bool hasReturn = true;
      Decl *classDecl = tree->Lookup(base->GetType(tree)->GetIdentifier()->GetName());
      Tree *myTree = classDecl->GetScope();
      if(strcmp(myTree->Lookup(field->GetName())->GetType()->GetIdentifier()->GetName(),"void") == 0)
        {
          hasReturn = false;
        }
      List<Decl*> *members = classDecl->GetMembers();
      int offset = 0;
      for(int i = 0; i < members->NumElements(); i++)
	{
	  if(members->Nth(i)->IsFunction())
	    {
	      if((strcmp(members->Nth(i)->GetName()->GetName(),field->GetName()) == 0) && (members->Nth(i)->GetFormals()->NumElements() == actuals->NumElements()))
		{
		  break;
		}
	      else
		{
		  offset++;
		}
	    }
	}
     
      List<Location*> *lactuals = new List<Location*>();
      for(int i = 0; i < actuals->NumElements(); i++)
        {
          lactuals->Append(actuals->Nth(i)->Emit(tree,cg));
        }
      Location *b = cg->GenLoad(base->Emit(tree, cg));
      Location *c = cg->GenLoad(b,offset*4);
      int n = actuals->NumElements();

      for(int i = 0; i < actuals->NumElements(); i++)
        {
          cg->GenPushParam(lactuals->Nth(n-i-1));
        }
      cg->GenPushParam(base->Emit(tree,cg));
      loc = cg->GenACall(c,hasReturn);
      cg->GenPopParams(4*(n+1));
    }

  return loc;
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
  Location *baseLocation;

  if(this->InClass() && base->IsField())
    {
      Decl *myDecl = tree->Lookup(base->GetName()->GetName());
      baseLocation = cg->GenLoad(CodeGenerator::ThisPtr,myDecl->GetOffset());
    }
  else
    {
      baseLocation = base->Emit(tree,cg);
    }

  Location *loc = subscript->Emit(tree,cg);
  char *eLabel = cg->NewLabel();
  Location *zer = cg->GenLoadConstant(0);
  Location *res = cg->GenBinaryOp("<",loc,zer);
  Location *tes = cg->GenLoad(baseLocation,-4);

  Location *res2 = cg->GenBinaryOp("<",loc,tes);
  Location *res3 = cg->GenBinaryOp("==",res2,zer);
  Location *fin = cg->GenBinaryOp("||",res,res3);
  cg->GenIfZ(fin,eLabel);
  Location *mes = cg->GenLoadConstant("Decaf runtime error: Array subscript out of bounds\\n");
  cg->GenBuiltInCall(PrintString,mes);
  cg->GenBuiltInCall(Halt);
  cg->GenLabel(eLabel);
  Location *temp = cg->GenLoadConstant(4);
  Location *atemp = cg->GenBinaryOp("*",temp,loc);
  Location *array = cg->GenBinaryOp("+",baseLocation,atemp);
  if(strcmp(parent->GetPrintNameForNode(),"AssignExpr") == 0)
    {
      return array;
    }
  return cg->GenLoad(array);
}

Type* ArrayAccess::GetType(Tree *tree)
{
  return base->GetType(tree);
}

Location* NewArrayExpr::Emit(Tree *tree,CodeGenerator *cg)
{
  char *eLabel = cg->NewLabel();
  Location *loc = size->Emit(tree,cg);
  Location *zer = cg->GenLoadConstant(0);
  Location *res = cg->GenBinaryOp("<",loc,zer);
  cg->GenIfZ(res,eLabel);
  Location *mes = cg->GenLoadConstant("Decaf runtime error: Array size is <= 0\\n");
  cg->GenBuiltInCall(PrintString,mes);
  cg->GenBuiltInCall(Halt);
  cg->GenLabel(eLabel);
  Location *tem1 = cg->GenLoadConstant(1);
  Location *tem2 = cg->GenBinaryOp("+",tem1,loc);
  Location *con = cg->GenLoadConstant(4);
  Location *tem3 = cg->GenBinaryOp("*",tem2,con);
  Location *arr = cg->GenBuiltInCall(Alloc,tem3);
  cg->GenStore(arr,loc);
  Location *ret = cg->GenBinaryOp("+",arr,con);
  return ret;
}

Type* NewArrayExpr::GetType(Tree *tree)
{
  return new ArrayType((*location),elemType);
}

Location* This::Emit(Tree *tree,CodeGenerator *cg)
{
  return CodeGenerator::ThisPtr;
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
  Decl *d = tree->Lookup(cType->GetIdentifier()->GetName());
  int num = d->GetMemberBytes();
  Location *mBytes = cg->GenLoadConstant(num);
  Location *res = cg->GenBuiltInCall(Alloc,mBytes);
  Location *res2 = cg->GenLoadLabel(cType->GetIdentifier()->GetName());
  cg->GenStore(res,res2);
  return res;
}

Type* NewExpr::GetType(Tree *tree)
{
  return cType;
}

Location* ReadIntegerExpr::Emit(Tree *tree, CodeGenerator *cg)
{
  Location *loc = cg->GenBuiltInCall(ReadInteger);
  return loc;
}

Location* ReadLineExpr::Emit(Tree *tree, CodeGenerator *cg)
{
  Location *loc = cg->GenBuiltInCall(ReadLine);
  return loc;
}

