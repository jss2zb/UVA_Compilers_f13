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



void Program::Check() 
{
  for(int i = 0; i < decls->NumElements();i++)
    {
      (decls->Nth(i))->Check(scope);
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

void StmtBlock::Check(Tree *tree)
{
  for(int i = 0; i < decls->NumElements();i++)
    {
      (decls->Nth(i))->Check(scope);
    }
  for(int i = 0; i < stmts->NumElements(); i++)
    {
      (stmts->Nth(i))->Check(scope);
    }
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
  //  if(!test->Build(hash))
  //  {
  //    ReportError::Formatted(
  //  }
  body->Build(theTree);
  tree->InsertChild(theTree);
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

void WhileStmt::Check(Tree *tree)
{
  body->Check(tree);
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

void IfStmt::Check(Tree *tree)
{
  body->Check(tree);
  if(elseBody) elseBody->Check(tree);
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
  
PrintStmt::PrintStmt(List<Expr*> *a) {    
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

void PrintStmt::PrintChildren(int indentLevel) {
    args->PrintAll(indentLevel+1, "(args) ");
}

void BreakStmt::Build(Tree *tree){}

void BreakStmt::Check(Tree *tree)
{
  if(!(parent->IsInLoop()))
    {
      ReportError::BreakOutsideLoop(this);
    }
}
