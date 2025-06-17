
// Generated from MiniC.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "MiniCVisitor.h"


/**
 * This class provides an empty implementation of MiniCVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  MiniCBaseVisitor : public MiniCVisitor {
public:

  virtual std::any visitCompileUnit(MiniCParser::CompileUnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncDef(MiniCParser::FuncDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnType(MiniCParser::ReturnTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFormalParamList(MiniCParser::FormalParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFormalParam(MiniCParser::FormalParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(MiniCParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItemList(MiniCParser::BlockItemListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItem(MiniCParser::BlockItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDecl(MiniCParser::VarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBasicType(MiniCParser::BasicTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDef(MiniCParser::VarDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStatement(MiniCParser::ReturnStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignStatement(MiniCParser::AssignStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockStatement(MiniCParser::BlockStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionStatement(MiniCParser::ExpressionStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfStatement(MiniCParser::IfStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhileStatement(MiniCParser::WhileStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBreakStatement(MiniCParser::BreakStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContinueStatement(MiniCParser::ContinueStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpr(MiniCParser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrExp(MiniCParser::OrExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAndExp(MiniCParser::AndExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqualExp(MiniCParser::EqualExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationExp(MiniCParser::RelationExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddExp(MiniCParser::AddExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMulExp(MiniCParser::MulExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqualOp(MiniCParser::EqualOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationOp(MiniCParser::RelationOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddOp(MiniCParser::AddOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMulOp(MiniCParser::MulOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryExp(MiniCParser::UnaryExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimaryExp(MiniCParser::PrimaryExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRealParamList(MiniCParser::RealParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLVal(MiniCParser::LValContext *ctx) override {
    return visitChildren(ctx);
  }


};

