#include "ExprParser.h"
#include "ExprVisitor.h"

#include <iostream>

class EvalVisitor : public ExprVisitor {
public:

  std::any visitCalc(ExprParser::CalcContext *ctx) override {
    /// TODO
    return visit(ctx->expr());
  }

  std::any visitMulDiv(ExprParser::MulDivContext *ctx) override {
    /// TODO
    auto left = std::any_cast<int>(visit(ctx->expr(0)));
    auto right = std::any_cast<int>(visit(ctx->expr(1)));

    if (ctx->op->getType() == ExprParser::MUL) {
      return left * right;
    } else if (ctx->op->getType() == ExprParser::DIV) {
      return left / right;
    }
  }

  std::any visitAddSub(ExprParser::AddSubContext *ctx) override {
    /// TODO
    auto left=std::any_cast<int>(visit(ctx->expr(0)));
    auto right=std::any_cast<int>(visit(ctx->expr(1)));

    if(ctx->op->getType()==ExprParser::ADD) {
      return left+right;
    }
    if(ctx->op->getType()==ExprParser::SUB) {
      return left-right;
    }

  }
  std::any visitInt(ExprParser::IntContext *ctx) override {
    /// TODO
    return std::stoi(ctx->INT()->getText());
  }

  std::any visitParens(ExprParser::ParensContext *ctx) override {
    /// TODO
    return visit(ctx->expr());
  }

};
