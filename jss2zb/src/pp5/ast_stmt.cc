/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "errors.h"


Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
    scope = new Tree();
}



void Program::Emit() 
{
  CodeGenerator *cg = new CodeGenerator();
  bool error = false;

  for(int i = 0; i < decls->NumElements();i++)
    {
      if(!decls->Nth(i)->IsFunction())
        {
	  decls->Nth(i)->Emit(cg);
        }
    }

  for(int i = 0; i < decls->NumElements();i++)
    {
      if(strcmp(decls->Nth(i)->GetName()->GetName(),"main") == 0)
	{
	  error = true;
	}
      (decls->Nth(i))->Emit(scope,cg);
    }
  
  if(!error)
    {
      ReportError::NoMainFound();
    }
}

void Program::Build() 
{
  for(int i = 0; i < decls->NumElements(); i++)
    {
      (decls->Nth(i))->Build(scope);
    }
}

void Program::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    printf("\n");
}

/*
Tree Stmt::Build(Tree *tree)
{
  printf();
  return tree;
  }*/

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::Build(Tree *tree) 
{
  Tree *theTree = new Tree(tree);
  for(int i = 0; i < decls->NumElements();i++)
    {
      (decls->Nth(i))->Build(theTree);
    }
  for(int i = 0; i < stmts->NumElements(); i++)
    {
      (stmts->Nth(i))->Build(theTree);
    }
  scope = theTree;
  tree->InsertChild(theTree);
}

Location* StmtBlock::Emit(Tree *tree,CodeGenerator *cg)
{
  for(int i = 0; i < decls->NumElements();i++)
    {
      decls->Nth(i)->Emit(scope,cg);
    }
  for(int i = 0; i < stmts->NumElements(); i++)
    {
      stmts->Nth(i)->Emit(scope,cg);
    }

  return NULL;
}

void StmtBlock::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    stmts->PrintAll(indentLevel+1);
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) { 
    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

void ForStmt::Build(Tree *tree)
{
  Tree *theTree = new Tree(tree);
  init->Build(theTree);
  body->Build(theTree);
  scope = theTree;
  tree->InsertChild(theTree);
}

Location* ForStmt::Emit(Tree *tree,CodeGenerator *cg)
{
  init->Emit(scope,cg);
  char *tlabel = cg->NewLabel();
  char *elabel = cg->NewLabel();
  label = elabel;
  cg->GenLabel(tlabel);
  cg->GenIfZ(test->Emit(scope,cg),elabel);
  body->Emit(scope,cg);
  step->Emit(scope,cg);
  cg->GenGoto(tlabel);
  cg->GenLabel(elabel);
  return NULL;
}

void ForStmt::PrintChildren(int indentLevel) {
    init->Print(indentLevel+1, "(init) ");
    test->Print(indentLevel+1, "(test) ");
    step->Print(indentLevel+1, "(step) ");
    body->Print(indentLevel+1, "(body) ");
}

void WhileStmt::Build(Tree *tree)
{
  test->Build(tree);
  body->Build(tree);
}

Location* WhileStmt::Emit(Tree *tree,CodeGenerator *cg)
{
  char *tlabel = cg->NewLabel();
  char *elabel = cg->NewLabel();
  label = elabel;
  cg->GenLabel(tlabel);
  cg->GenIfZ(test->Emit(tree,cg),elabel);
  body->Emit(tree,cg);
  cg->GenGoto(tlabel);
  cg->GenLabel(elabel);
  return NULL;
}

void WhileStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

void IfStmt::Build(Tree *tree)
{
  test->Build(tree);
  body->Build(tree);
  if(elseBody) elseBody->Build(tree);
}

Location* IfStmt::Emit(Tree *tree,CodeGenerator *cg)
{
  char *eLabel = cg->NewLabel();
  cg->GenIfZ(test->Emit(tree,cg),eLabel);
  char *tLabel = cg->NewLabel();  
  body->Emit(tree,cg);
  if(elseBody)
    {
      cg->GenGoto(tLabel);
      cg->GenLabel(eLabel);
      elseBody->Emit(tree,cg);
      cg->GenLabel(tLabel);
    }
  else
    cg->GenLabel(eLabel);
  return NULL;
}

void IfStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(then) ");
    if (elseBody) elseBody->Print(indentLevel+1, "(else) ");
}

void ReturnStmt::Build(Tree *tree)
{
  expr->Build(tree);
}

Location* ReturnStmt::Emit(Tree *tree,CodeGenerator *cg)
{
  return NULL;
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

void ReturnStmt::PrintChildren(int indentLevel) {
    expr->Print(indentLevel+1);
}

void PrintStmt::Build(Tree *tree)
{
  for(int i=0; i < args->NumElements(); i++)
    {
      (args->Nth(i))->Build(tree);
    }
}

Location* PrintStmt::Emit(Tree *tree,CodeGenerator *cg)
{
  for(int i=0; i < args->NumElements(); i++)
    {
      if(strcmp(args->Nth(i)->GetType(tree)->GetIdentifier()->GetName(),"int") == 0)
	{
	  cg->GenBuiltInCall(PrintInt,args->Nth(i)->Emit(tree,cg));
	}
      else if(strcmp(args->Nth(i)->GetType(tree)->GetIdentifier()->GetName(),"bool") == 0)
	{
	  cg->GenBuiltInCall(PrintBool,args->Nth(i)->Emit(tree,cg));
	}
      else
	{
	  cg->GenBuiltInCall(PrintString,args->Nth(i)->Emit(tree,cg));
	}
    }
  return NULL;
}
  
PrintStmt::PrintStmt(List<Expr*> *a) {    
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

void PrintStmt::PrintChildren(int indentLevel) {
    args->PrintAll(indentLevel+1, "(args) ");
}

void BreakStmt::Build(Tree *tree){}

Location* BreakStmt::Emit(Tree *tree,CodeGenerator *cg)
{
  cg->GenGoto(parent->GetExitLabel());
  return NULL;
}