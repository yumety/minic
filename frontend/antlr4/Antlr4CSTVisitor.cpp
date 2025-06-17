///
/// @file Antlr4CSTVisitor.cpp
/// @brief Antlr4的具体语法树的遍历产生AST
/// @author zenglj (zenglj@live.com)
/// @version 1.1
/// @date 2024-11-23
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// <tr><td>2024-11-23 <td>1.1     <td>zenglj  <td>表达式版增强
/// </table>
///

#include <string>

#include "Antlr4CSTVisitor.h"
#include "AST.h"
#include "AttrType.h"

#define Instanceof(res, type, var) auto res = dynamic_cast<type>(var)

/// @brief 构造函数
MiniCCSTVisitor::MiniCCSTVisitor() {
}

/// @brief 析构函数
MiniCCSTVisitor::~MiniCCSTVisitor() {
}

/// @brief 遍历CST产生AST
/// @param root CST语法树的根结点
/// @return AST的根节点
ast_node *MiniCCSTVisitor::run(MiniCParser::CompileUnitContext *root) {
    return std::any_cast<ast_node *>(visitCompileUnit(root));
}

/// @brief 非终结运算符compileUnit的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitCompileUnit(MiniCParser::CompileUnitContext *ctx) {
    // compileUnit: (funcDef | varDecl)* EOF

    // 请注意这里必须先遍历全局变量后遍历函数。肯定可以确保全局变量先声明后使用的规则，但有些情况却不能检查出。
    // 事实上可能函数A后全局变量B后函数C，这时在函数A中是不能使用变量B的，需要报语义错误，但目前的处理不会。
    // 因此在进行语义检查时，可能追加检查行号和列号，如果函数的行号/列号在全局变量的行号/列号的前面则需要报语义错误
    // TODO 请追加实现。

    ast_node *temp_node;
    ast_node *compileUnitNode = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT);

    // 可能多个变量，因此必须循环遍历
    for (auto varCtx : ctx->varDecl()) {
        // 变量函数定义
        temp_node = std::any_cast<ast_node *>(visitVarDecl(varCtx));
        (void)compileUnitNode->insert_son_node(temp_node);
    }

    // 可能有多个函数，因此必须循环遍历
    for (auto funcCtx : ctx->funcDef()) {
        // 变量函数定义
        temp_node = std::any_cast<ast_node *>(visitFuncDef(funcCtx));
        (void)compileUnitNode->insert_son_node(temp_node);
    }

    return compileUnitNode;
}

/// @brief 非终结运算符funcDef的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitFuncDef(MiniCParser::FuncDefContext *ctx) {
    // 识别的文法产生式：funcDef : T_INT T_ID T_L_PAREN T_R_PAREN block;

    // 函数返回类型，终结符
    type_attr funcReturnType = std::any_cast<type_attr>(visitReturnType(ctx->returnType()));

    // 创建函数名的标识符终结符节点，终结符
    char *id = strdup(ctx->T_ID()->getText().c_str());

    var_id_attr funcId{id, (int64_t)ctx->T_ID()->getSymbol()->getLine()};

    // 形参结点目前没有，设置为空指针
    ast_node *formalParamsNode = nullptr;

    if (ctx->formalParamList()) {
        formalParamsNode = std::any_cast<ast_node *>(visitFormalParamList(ctx->formalParamList()));
    }

    // 遍历block结点创建函数体节点，非终结符
    auto blockNode = std::any_cast<ast_node *>(visitBlock(ctx->block()));

    // 创建函数定义的节点，孩子有类型，函数名，语句块和形参(实际上无)
    // create_func_def函数内会释放funcId中指向的标识符空间，切记，之后不要再释放，之前一定要是通过strdup函数或者malloc分配的空间
    return create_func_def(funcReturnType, funcId, blockNode, formalParamsNode);
}

std::any MiniCCSTVisitor::visitReturnType(MiniCParser::ReturnTypeContext *ctx) {
    if (ctx->basicType()) {
        return std::any_cast<type_attr>(visitBasicType(ctx->basicType()));
    } else {
        int64_t line = ctx->T_VOID()->getSymbol()->getLine();
        return type_attr{BasicType::TYPE_VOID, line};
    }
}

// // visitFormalParamList 创建形参列表 AST
// std::any MiniCCSTVisitor::visitFormalParamList(MiniCParser::FormalParamListContext *ctx) {
//     // formalParamList: basicType formalParam (T_COMMA basicType formalParam)*;
//     // AST_OP_FUNC_PARAMS 用于标识函数形参容器
//     ast_node *paramsNode = create_contain_node(ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS);
//     for (size_t i = 0; i < ctx->basicType().size(); ++i) {
//         // 参数类型节点
//         type_attr t = std::any_cast<type_attr>(visitBasicType(ctx->basicType(i)));
//         ast_node *typeNode = create_type_node(t);
//         // 参数标识符节点
//         char *name = strdup(ctx->T_ID(i)->getText().c_str());
//         var_id_attr vid{name, (int64_t)ctx->T_ID(i)->getSymbol()->getLine()};
//         ast_node *idNode = ast_node::New(vid);
//         // 创建单个参数节点
//         ast_node *paramNode = ast_node::New(ast_operator_type::AST_OP_FUNC_FORMAL_PARAM, typeNode, idNode, nullptr);
//         paramsNode->insert_son_node(paramNode);
//     }
//     return paramsNode;
// }

// visitFormalParamList 创建形参列表 AST
std::any MiniCCSTVisitor::visitFormalParamList(MiniCParser::FormalParamListContext *ctx) {
    // formalParamList: basicType formalParam (T_COMMA basicType formalParam)*;
    // formalParam: T_ID (T_L_BRACKET expr? T_R_BRACKET) (T_L_BRACKET expr T_R_BRACKET)*;
    // AST_OP_FUNC_PARAMS 用于标识函数形参容器
    ast_node * paramsNode =
        create_contain_node(ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS);

    // 对应 basicType[i] 与 formalParam[i]
    for (size_t i = 0; i < ctx->basicType().size(); ++i) {
        // 1) 参数类型
        type_attr t = std::any_cast<type_attr>(visitBasicType(ctx->basicType(i)));
        ast_node * typeNode = create_type_node(t);

        // 2) 参数名
        auto *fpCtx = ctx->formalParam(i);
        char * name = strdup(fpCtx->T_ID()->getText().c_str());
        var_id_attr vid{name, (int64_t)fpCtx->T_ID()->getSymbol()->getLine()};
        ast_node * idNode = ast_node::New(vid);

        // 3) 如果有下标，收集每一维的维度表达式
        size_t dimCount = fpCtx->T_L_BRACKET().size();
        auto exprList = fpCtx->expr();  // vector<ExprContext*>

        ast_node * dimsNode = nullptr;
        if (dimCount > 0) {
            dimsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_DIMS);
            // 如果 exprList.size()==dimCount，说明第一个也有 expr，要跳过
            size_t start = (exprList.size() == dimCount ? 1 : 0);
            for (size_t j = start; j < exprList.size(); ++j) {
                ast_node * dimExpr =
                    std::any_cast<ast_node *>(visitExpr(exprList[j]));
                // dimExpr 可能 nullptr（不太可能，因为 exprList 只包含真正写了 expr 的）
                dimsNode->insert_son_node(dimExpr);
            }
        }
        
        // 4) 把 type, id, dims 三个孩子组合成一个参数节点
        ast_node * paramNode =
            ast_node::New(ast_operator_type::AST_OP_FUNC_FORMAL_PARAM, typeNode, idNode, dimsNode, nullptr);

        paramsNode->insert_son_node(paramNode);
    }

    return paramsNode;
}

/// @brief 非终结运算符block的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitBlock(MiniCParser::BlockContext *ctx) {
    // 识别的文法产生式：block : T_L_BRACE blockItemList? T_R_BRACE';
    if (!ctx->blockItemList()) {
        // 语句块没有语句

        // 为了方便创建一个空的Block节点
        return create_contain_node(ast_operator_type::AST_OP_BLOCK);
    }

    // 语句块含有语句

    // 内部创建Block节点，并把语句加入，这里不需要创建Block节点
    return visitBlockItemList(ctx->blockItemList());
}

/// @brief 非终结运算符blockItemList的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitBlockItemList(MiniCParser::BlockItemListContext *ctx) {
    // 识别的文法产生式：blockItemList : blockItem +;
    // 正闭包 循环 至少一个blockItem
    auto block_node = create_contain_node(ast_operator_type::AST_OP_BLOCK);

    for (auto blockItemCtx : ctx->blockItem()) {
        // 非终结符，需遍历
        auto blockItem = std::any_cast<ast_node *>(visitBlockItem(blockItemCtx));

        // 插入到块节点中
        (void)block_node->insert_son_node(blockItem);
    }

    return block_node;
}

///
/// @brief 非终结运算符blockItem的遍历
/// @param ctx CST上下文
///
std::any MiniCCSTVisitor::visitBlockItem(MiniCParser::BlockItemContext *ctx) {
    // 识别的文法产生式：blockItem : statement | varDecl
    if (ctx->statement()) {
        // 语句识别

        return visitStatement(ctx->statement());
    } else if (ctx->varDecl()) {
        return visitVarDecl(ctx->varDecl());
    }

    return nullptr;
}

/// @brief 非终结运算符statement中的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitStatement(MiniCParser::StatementContext *ctx) {
    // 识别的文法产生式：statement: T_ID T_ASSIGN expr T_SEMICOLON  # assignStatement
    // | T_RETURN expr T_SEMICOLON # returnStatement
    // | block  # blockStatement
    // | expr ? T_SEMICOLON #expressionStatement;
    // | T_IF T_L_PAREN expr T_R_PAREN statement (T_ELSE statement)?
    // | T_WHILE T_L_PAREN expr T_R_PAREN statement
    if (Instanceof(assignCtx, MiniCParser::AssignStatementContext *, ctx)) {
        return visitAssignStatement(assignCtx);
    } else if (Instanceof(returnCtx, MiniCParser::ReturnStatementContext *, ctx)) {
        return visitReturnStatement(returnCtx);
    } else if (Instanceof(blockCtx, MiniCParser::BlockStatementContext *, ctx)) {
        return visitBlockStatement(blockCtx);
    } else if (Instanceof(exprCtx, MiniCParser::ExpressionStatementContext *, ctx)) {
        return visitExpressionStatement(exprCtx);
    } else if (Instanceof(ifCtx, MiniCParser::IfStatementContext *, ctx)) {
        return visitIfStatement(ifCtx);
    } else if (Instanceof(whileCtx, MiniCParser::WhileStatementContext *, ctx)) {
        return visitWhileStatement(whileCtx);
    } else if (Instanceof(breakCtx, MiniCParser::BreakStatementContext *, ctx)) {
        return visitBreakStatement(breakCtx);
    } else if (Instanceof(continueCtx, MiniCParser::ContinueStatementContext *, ctx)) {
        return visitContinueStatement(continueCtx);
    }

    return nullptr;
}

///
/// @brief 非终结运算符statement中的returnStatement的遍历
/// @param ctx CST上下文
///
std::any MiniCCSTVisitor::visitReturnStatement(MiniCParser::ReturnStatementContext *ctx) {
    // 识别的文法产生式：returnStatement -> T_RETURN expr T_SEMICOLON
    
	ast_node *exprNode = nullptr;
    if (ctx->expr()) {
        exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));
    }

    // 创建返回节点，其孩子为Expr
    return create_contain_node(ast_operator_type::AST_OP_RETURN, exprNode);
}

std::any MiniCCSTVisitor::visitAssignStatement(MiniCParser::AssignStatementContext *ctx) {
    // 识别文法产生式：assignStatement: lVal T_ASSIGN expr T_SEMICOLON

    // 赋值左侧左值Lval遍历产生节点
    auto lvalNode = std::any_cast<ast_node *>(visitLVal(ctx->lVal()));

    // 赋值右侧expr遍历
    auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 创建一个AST_OP_ASSIGN类型的中间节点，孩子为Lval和Expr
    return ast_node::New(ast_operator_type::AST_OP_ASSIGN, lvalNode, exprNode, nullptr);
}

std::any MiniCCSTVisitor::visitBlockStatement(MiniCParser::BlockStatementContext *ctx) {
    // 识别文法产生式 blockStatement: block

    return visitBlock(ctx->block());
}

// —— if / if-else ——
std::any MiniCCSTVisitor::visitIfStatement(MiniCParser::IfStatementContext *ctx) {
    // 识别文法产生式：T_IF T_L_PAREN expr T_R_PAREN statement (T_ELSE statement)?
    // 1) 条件 expr
    auto cond = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 2) then 分支
    auto thenResult = visitStatement(ctx->statement()[0]);
    ast_node *thenNode = nullptr;
    if (thenResult.has_value()) {
        try {
            thenNode = std::any_cast<ast_node *>(thenResult);
        } catch (const std::bad_any_cast&) {
            // 如果转换失败，说明是空语句，thenNode保持为nullptr
        }
    }

    // 3) else 分支（可选）
    if (ctx->T_ELSE()) {
        auto elseResult = visitStatement(ctx->statement()[1]);
        ast_node *elseNode = nullptr;
        if (elseResult.has_value()) {
            try {
                elseNode = std::any_cast<ast_node *>(elseResult);
            } catch (const std::bad_any_cast&) {
                // 如果转换失败，说明是空语句，elseNode保持为nullptr
            }
        }
        // 三个孩子：cond, then, else
        return ast_node::New(ast_operator_type::AST_OP_IF, cond, thenNode, elseNode, nullptr);
    } else {
        // 两个孩子：cond, then
        return ast_node::New(ast_operator_type::AST_OP_IF, cond, thenNode, nullptr);
    }
}

// while
std::any MiniCCSTVisitor::visitWhileStatement(MiniCParser::WhileStatementContext *ctx) {
    auto cond = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 处理while语句体，可能是空语句
    auto bodyResult = visitStatement(ctx->statement());
    ast_node *body = nullptr;
    if (bodyResult.has_value()) {
        try {
            body = std::any_cast<ast_node *>(bodyResult);
        } catch (const std::bad_any_cast&) {
            // 如果转换失败，说明是空语句，body保持为nullptr
        }
    }

    return ast_node::New(ast_operator_type::AST_OP_WHILE, cond, body, nullptr);
}

std::any MiniCCSTVisitor::visitBreakStatement(MiniCParser::BreakStatementContext *ctx) {
    return ast_node::New(ast_operator_type::AST_OP_BREAK, nullptr);
}

std::any MiniCCSTVisitor::visitContinueStatement(MiniCParser::ContinueStatementContext *ctx) {
    return ast_node::New(ast_operator_type::AST_OP_CONTINUE, nullptr);
}

/// @brief 非终结运算符expr的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitExpr(MiniCParser::ExprContext *ctx) {
    return visitOrExp(ctx->orExp());
}

std::any MiniCCSTVisitor::visitOrExp(MiniCParser::OrExpContext *ctx) {
    // 识别产生式：orExp: andExp (T_OR andExp)
    if (ctx->T_OR().empty()) {
        // 没有T_OR运算符，则说明闭包识别为0，只识别了第一个非终结符andExp
        return visitAndExp(ctx->andExp()[0]);
    }

    ast_node *left, *right;

    // 存在equalOp运算符，自
    auto opsCtxVec = ctx->T_OR();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int)opsCtxVec.size(); k++) {
        // 获取运算符
        ast_operator_type op = ast_operator_type::AST_OP_OR;

        if (k == 0) {
            // 左操作数
            left = std::any_cast<ast_node *>(visitAndExp(ctx->andExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitAndExp(ctx->andExp()[k + 1]));

        // 新建结点作为下一个运算符的右操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}

std::any MiniCCSTVisitor::visitAndExp(MiniCParser::AndExpContext *ctx) {
    // 识别产生式：andExp: equalExp (T_AND equalExp)*
    if (ctx->T_AND().empty()) {
        // 没有T_AND运算符，则说明闭包识别为0，只识别了第一个非终结符equalExp
        return visitEqualExp(ctx->equalExp()[0]);
    }

    ast_node *left, *right;

    // 存在andOp运算符，自
    auto opsCtxVec = ctx->T_AND();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int)opsCtxVec.size(); k++) {
        // 获取运算符
        ast_operator_type op = ast_operator_type::AST_OP_AND;

        if (k == 0) {
            // 左操作数
            left = std::any_cast<ast_node *>(visitEqualExp(ctx->equalExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitEqualExp(ctx->equalExp()[k + 1]));

        // 新建结点作为下一个运算符的右操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}

std::any MiniCCSTVisitor::visitEqualExp(MiniCParser::EqualExpContext *ctx) {
    // 识别产生式：equalExp: relationExp (equalOp relationExp)*
    if (ctx->equalOp().empty()) {
        // 没有equalOp运算符，则说明闭包识别为0，只识别了第一个非终结符relationExp
        return visitRelationExp(ctx->relationExp()[0]);
    }

    ast_node *left, *right;

    // 存在equalOp运算符，自
    auto opsCtxVec = ctx->equalOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int)opsCtxVec.size(); k++) {
        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitEqualOp(opsCtxVec[k]));

        if (k == 0) {
            // 左操作数
            left = std::any_cast<ast_node *>(visitRelationExp(ctx->relationExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitRelationExp(ctx->relationExp()[k + 1]));

        // 新建结点作为下一个运算符的右操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}

std::any MiniCCSTVisitor::visitRelationExp(MiniCParser::RelationExpContext *ctx) {
    // 识别产生式：relationExp: addExp (relationOp addExp)*
    if (ctx->relationOp().empty()) {
        // 没有relationOp运算符，则说明闭包识别为0，只识别了第一个非终结符addExp
        return visitAddExp(ctx->addExp()[0]);
    }

    ast_node *left, *right;

    // 存在equalOp运算符，自
    auto opsCtxVec = ctx->relationOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int)opsCtxVec.size(); k++) {
        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitRelationOp(opsCtxVec[k]));

        if (k == 0) {
            // 左操作数
            left = std::any_cast<ast_node *>(visitAddExp(ctx->addExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitAddExp(ctx->addExp()[k + 1]));

        // 新建结点作为下一个运算符的右操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}

/// @brief 非终结运算符addExp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitAddExp(MiniCParser::AddExpContext *ctx) {
    // 识别产生式：addExp: mulExp (addOp mulExp)*
    if (ctx->addOp().empty()) {
        // 没有addOp运算符，则说明闭包识别为0，只识别了第一个非终结符mulExp
        return visitMulExp(ctx->mulExp()[0]);
    }

    ast_node *left, *right;

    // 存在addOp运算符，自
    auto opsCtxVec = ctx->addOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int)opsCtxVec.size(); k++) {
        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitAddOp(opsCtxVec[k]));

        if (k == 0) {
            // 左操作数
            left = std::any_cast<ast_node *>(visitMulExp(ctx->mulExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitMulExp(ctx->mulExp()[k + 1]));

        // 新建结点作为下一个运算符的右操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}

/// @brief 非终结运算符mulExp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitMulExp(MiniCParser::MulExpContext *ctx) {
    // 识别的文法产生式：mulExp: unaryExp (mulOp unaryExp)*

    if (ctx->mulOp().empty()) {
        // 没有mulOp运算符，则说明闭包识别为0，只识别了第一个非终结符unaryExp
        return visitUnaryExp(ctx->unaryExp()[0]);
    }

    ast_node *left, *right;

    // 存在mulOp运算符，自
    auto opsCtxVec = ctx->mulOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int)opsCtxVec.size(); k++) {
        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitMulOp(opsCtxVec[k]));

        if (k == 0) {
            // 左操作数
            left = std::any_cast<ast_node *>(visitUnaryExp(ctx->unaryExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitUnaryExp(ctx->unaryExp()[k + 1]));

        // 新建结点作为下一个运算符的右操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}

/// @brief 非终结运算符equalOp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitEqualOp(MiniCParser::EqualOpContext *ctx) {
    // 识别的文法产生式：equalOp: T_EQ | T_NE

    if (ctx->T_EQ()) {
        return ast_operator_type::AST_OP_EQ;
    } else {
        return ast_operator_type::AST_OP_NE;
    }
}

/// @brief 非终结运算符relationOp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitRelationOp(MiniCParser::RelationOpContext *ctx) {
    // 识别的文法产生式：relationOp: T_LT | T_GT | T_LE | T_GE

    if (ctx->T_LT())
        return ast_operator_type::AST_OP_LT;
    else if (ctx->T_GT())
        return ast_operator_type::AST_OP_GT;
    else if (ctx->T_LE())
        return ast_operator_type::AST_OP_LE;
    else
        return ast_operator_type::AST_OP_GE;
}

/// @brief 非终结运算符addOp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitAddOp(MiniCParser::AddOpContext *ctx) {
    // 识别的文法产生式：addOp : T_ADD | T_SUB

    if (ctx->T_ADD()) {
        return ast_operator_type::AST_OP_ADD;
    } else {
        return ast_operator_type::AST_OP_SUB;
    }
}

/// @brief 非终结运算符mulOp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitMulOp(MiniCParser::MulOpContext *ctx) {
    // 识别的文法产生式：mulOp: T_MUL | T_DIV | T_MOD

    if (ctx->T_MUL())
        return ast_operator_type::AST_OP_MUL;
    else if (ctx->T_DIV())
        return ast_operator_type::AST_OP_DIV;
    else
        return ast_operator_type::AST_OP_MOD;
}

std::any MiniCCSTVisitor::visitUnaryExp(MiniCParser::UnaryExpContext *ctx) {
    // 识别文法产生式：unaryExp: primaryExp | T_SUB unaryExp | T_NOT unaryExp | T_ID T_L_PAREN realParamList? T_R_PAREN;

    if (ctx->primaryExp()) {
        // 普通表达式
        return visitPrimaryExp(ctx->primaryExp());
    } else if (ctx->T_SUB()) {
		// 单目负号
        auto *val = std::any_cast<ast_node *>(visitUnaryExp(ctx->unaryExp()));
        return ast_node::New(ast_operator_type::AST_OP_NEG, val, nullptr, nullptr);
    } else if (ctx->T_NOT()) {
		// 逻辑非
        ast_node *child = std::any_cast<ast_node *>(visitUnaryExp(ctx->unaryExp()));
        return ast_node::New(ast_operator_type::AST_OP_NOT,
                             child,
                             nullptr,
                             nullptr);
    } else if (ctx->T_ID()) {
        // 创建函数调用名终结符节点
        ast_node *funcname_node = ast_node::New(ctx->T_ID()->getText(), (int64_t)ctx->T_ID()->getSymbol()->getLine());

        // 实参列表
        ast_node *paramListNode = nullptr;

        // 函数调用
        if (ctx->realParamList()) {
            // 有参数
            paramListNode = std::any_cast<ast_node *>(visitRealParamList(ctx->realParamList()));
        }

        // 创建函数调用节点，其孩子为被调用函数名和实参，
        return create_func_call(funcname_node, paramListNode);
    } else {
        return nullptr;
    }
}

std::any MiniCCSTVisitor::visitPrimaryExp(MiniCParser::PrimaryExpContext *ctx) {
    // 识别文法产生式 primaryExp: T_L_PAREN expr T_R_PAREN | T_DIGIT | lVal;

    ast_node *node = nullptr;

    if (ctx->T_DIGIT()) {
        // 无符号整型字面量
        // 识别 primaryExp: T_DIGIT

        // uint32_t val = (uint32_t) stoull(ctx->T_DIGIT()->getText());
        // 添加识别8/16进制
        std::string text = ctx->T_DIGIT()->getText();
        uint32_t val = (uint32_t)strtol(text.c_str(), nullptr, 0);

        int64_t lineNo = (int64_t)ctx->T_DIGIT()->getSymbol()->getLine();
        node = ast_node::New(digit_int_attr{val, lineNo});
    } else if (ctx->lVal()) {
        // 具有左值的表达式
        // 识别 primaryExp: lVal
        node = std::any_cast<ast_node *>(visitLVal(ctx->lVal()));
    } else if (ctx->expr()) {
        // 带有括号的表达式
        // primaryExp: T_L_PAREN expr T_R_PAREN
        node = std::any_cast<ast_node *>(visitExpr(ctx->expr()));
    }

    return node;
}

std::any MiniCCSTVisitor::visitLVal(MiniCParser::LValContext *ctx) {
    // 识别文法产生式：lVal: T_ID (T_L_BRACKET expr T_R_BRACKET)*;
    // 获取ID的名字
    auto varId = ctx->T_ID()->getText();

    // 获取行号
    int64_t lineNo = (int64_t)ctx->T_ID()->getSymbol()->getLine();

    ast_node * idNode = ast_node::New(varId, lineNo);

    size_t dimCount = ctx->T_L_BRACKET().size();  // '[' 的个数

    auto exprList = ctx->expr();

    if (dimCount == 0) {
        // 普通变量
        return ast_node::New(varId, lineNo);
    }

    // 数组访问：收集所有下标表达式
    ast_node * dimsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_DIMS);

    for (size_t i = 0; i < exprList.size(); ++i) {
        // 依次遍历每一维度表达式
        ast_node * dimExpr = std::any_cast<ast_node *>(visitExpr(exprList[i]));
        dimsNode->insert_son_node(dimExpr);
    }
    
    // 构造一个数组访问节点：孩子 0 是 idNode，孩子 1 是 dimsNode
    // 后面的 nullptr 占位，下游同样可通过 sons[0]/sons[1] 访问
    return ast_node::New(ast_operator_type::AST_OP_ARRAY_ACCESS,
                             idNode,
                             dimsNode,
                             nullptr);
}

std::any MiniCCSTVisitor::visitVarDecl(MiniCParser::VarDeclContext *ctx) {
    // varDecl: basicType varDef (T_COMMA varDef)* T_SEMICOLON;

    // 声明语句节点
    ast_node *stmt_node = create_contain_node(ast_operator_type::AST_OP_DECL_STMT);

    // 类型节点
    type_attr typeAttr = std::any_cast<type_attr>(visitBasicType(ctx->basicType()));

    for (auto &varCtx : ctx->varDef()) {
        // 变量名节点
        ast_node *id_node = std::any_cast<ast_node *>(visitVarDef(varCtx));

        // 创建类型节点
        ast_node *type_node = create_type_node(typeAttr);

        // 创建变量定义节点
        ast_node *decl_node = ast_node::New(ast_operator_type::AST_OP_VAR_DECL, type_node, id_node, nullptr);

        // 插入到变量声明语句
        (void)stmt_node->insert_son_node(decl_node);
    }

    return stmt_node;
}

std::any MiniCCSTVisitor::visitVarDef(MiniCParser::VarDefContext *ctx) {
    // varDef: T_ID ('=' expr)? | T_ID (T_L_BRACKET expr T_R_BRACKET)*;

    auto varId = ctx->T_ID()->getText();

    // 获取行号
    int64_t lineNo = (int64_t)ctx->T_ID()->getSymbol()->getLine();

    // return ast_node::New(varId, lineNo);
    ast_node * idNode = ast_node::New(varId, lineNo);

    size_t dimCount = ctx->T_L_BRACKET().size();  // '[' 的个数

    auto exprList = ctx->expr();

    if (dimCount > 0) {
        // —— 数组定义分支 —— 
        // create 一个 dims 容器节点
        ast_node * dimsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_DIMS);

        // ctx->expr() 返回与 dimCount 对应的那 many expressions
        for (size_t i = 0; i < exprList.size(); ++i) {
            // 依次遍历每一维度表达式
            ast_node * dimExpr = std::any_cast<ast_node *>(visitExpr(exprList[i]));
            dimsNode->insert_son_node(dimExpr);
        }
        // 返回一个新的 AST_OP_ARRAY_DEF：idNode + dimsNode
        return ast_node::New(ast_operator_type::AST_OP_ARRAY_DEF,
                             idNode,
                             dimsNode,
                             nullptr);
    } else {
        // —— 标量（可选带初始化）分支 —— 
        ast_node * initNode = nullptr;
        if (!exprList.empty()) {
            // 只有一个初始化表达式，取第 0 个
            initNode = std::any_cast<ast_node *>(visitExpr(exprList[0]));
        }
        // 返回 AST_OP_VAR_DEF：idNode + initNode（可能为 nullptr）
        return ast_node::New(ast_operator_type::AST_OP_VAR_DEF,
                             idNode,
                             initNode,
                             nullptr);
    }
    // if (dimCount > 0) {
    //     // 是数组定义：收集每一维的表达式
    //     // ctx->expr() 会返回所有 expr：要么是 init，要么是各个下标
    //     // 因为（'=' expr）与（'[' expr ']'）是互斥的分支，这里 ctx->expr() 只包含下标
    //     ast_node * dimsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_DIMS);
    //     for (auto exprCtx : ctx->expr()) {
    //         ast_node * dimExpr = std::any_cast<ast_node *>(visitExpr(exprCtx));
    //         dimsNode->insert_son_node(dimExpr);
    //     }
    //     // AST_OP_VAR_DEF 还是保留？为了语义更清晰，最好新建一个 AST_OP_ARRAY_DEF
    //     return ast_node::New(ast_operator_type::AST_OP_ARRAY_DEF,
    //                          idNode,
    //                          dimsNode,
    //                          nullptr);
    // }
    // 
    // // 如果有初始化表达式，则递归解析
    // ast_node * initNode = nullptr;
    // if (ctx->expr()) {
    //     initNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));
    // }

    // // 返回一个 VAR_DEF 节点，孩子依次是 idNode, initNode, nullptr
    // return ast_node::New(ast_operator_type::AST_OP_VAR_DEF, idNode, initNode, nullptr);
}

std::any MiniCCSTVisitor::visitBasicType(MiniCParser::BasicTypeContext *ctx) {
    // basicType: T_INT;
    type_attr attr{BasicType::TYPE_VOID, -1};
    if (ctx->T_INT()) {
        attr.type = BasicType::TYPE_INT;
        attr.lineno = (int64_t)ctx->T_INT()->getSymbol()->getLine();
    }

    return attr;
}

std::any MiniCCSTVisitor::visitRealParamList(MiniCParser::RealParamListContext *ctx) {
    // 识别的文法产生式：realParamList : expr (T_COMMA expr)*;

    auto paramListNode = create_contain_node(ast_operator_type::AST_OP_FUNC_REAL_PARAMS);

    for (auto paramCtx : ctx->expr()) {
        auto paramNode = std::any_cast<ast_node *>(visitExpr(paramCtx));

        paramListNode->insert_son_node(paramNode);
    }

    return paramListNode;
}

std::any MiniCCSTVisitor::visitExpressionStatement(MiniCParser::ExpressionStatementContext *ctx) {
    // 识别文法产生式  expr ? T_SEMICOLON #expressionStatement;
    if (ctx->expr()) {
        // 表达式语句

        // 遍历expr非终结符，创建表达式节点后返回
        return visitExpr(ctx->expr());
    } else {
        // 空语句

        // 返回包含nullptr的std::any，而不是直接返回nullptr
        // 这样可以避免std::bad_any_cast错误
        return std::any(static_cast<ast_node*>(nullptr));
    }
}