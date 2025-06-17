///
/// @file Antlr4CSTVisitor.h
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
#pragma once

#include "AST.h"
#include "MiniCBaseVisitor.h"

/// @brief 遍历具体语法树产生抽象语法树
class MiniCCSTVisitor : public MiniCBaseVisitor {
public:
    /// @brief 构造函数
    MiniCCSTVisitor();

    /// @brief 析构函数
    virtual ~MiniCCSTVisitor();

    /// @brief 遍历CST产生AST
    /// @param root CST语法树的根结点
    /// @return AST的根节点
    ast_node *run(MiniCParser::CompileUnitContext *root);

protected:
    /* 下面的函数都是从MiniCBaseVisitor继承下来的虚拟函数，需要重载实现 */

    /// @brief 非终结运算符compileUnit的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitCompileUnit(MiniCParser::CompileUnitContext *ctx) override;

    /// @brief 非终结运算符funcDef的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitFuncDef(MiniCParser::FuncDefContext *ctx) override;

    /// @brief 非终结运算符visitReturnType的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitReturnType(MiniCParser::ReturnTypeContext *ctx) override;

    /// @brief 非终结符FormalParamList的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitFormalParamList(MiniCParser::FormalParamListContext *ctx) override;

    /// @brief 非终结运算符block的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitBlock(MiniCParser::BlockContext *ctx) override;

    /// @brief 非终结运算符blockItemList的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitBlockItemList(MiniCParser::BlockItemListContext *ctx) override;

    /// @brief 非终结运算符blockItem的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitBlockItem(MiniCParser::BlockItemContext *ctx) override;

    /// @brief 非终结运算符statement的遍历（语句分发器）
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 根据具体的语句类型分发到对应的处理函数，支持赋值、返回、块、表达式、if、while、break、continue语句
    std::any visitStatement(MiniCParser::StatementContext *ctx);

    /// @brief 非终结运算符returnStatement的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理return语句，可能包含返回表达式或无返回值
    std::any visitReturnStatement(MiniCParser::ReturnStatementContext *ctx) override;

    /// @brief 非终结运算符expr的遍历（表达式入口）
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 表达式解析的入口点，直接转发到逻辑或表达式处理
    std::any visitExpr(MiniCParser::ExprContext *ctx) override;

    /// @brief 赋值语句的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理形如 "lVal = expr;" 的赋值语句，支持变量和数组元素赋值
    std::any visitAssignStatement(MiniCParser::AssignStatementContext *ctx) override;

    /// @brief 块语句的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理用大括号包围的语句块
    std::any visitBlockStatement(MiniCParser::BlockStatementContext *ctx) override;

    /// @brief if语句的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理if和if-else语句，支持条件分支控制
    std::any visitIfStatement(MiniCParser::IfStatementContext *ctx) override;

    /// @brief while循环语句的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理while循环语句，包含条件表达式和循环体
    std::any visitWhileStatement(MiniCParser::WhileStatementContext *ctx) override;

    /// @brief break语句的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理循环中的break跳出语句
    std::any visitBreakStatement(MiniCParser::BreakStatementContext *ctx) override;

    /// @brief continue语句的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理循环中的continue继续语句
    std::any visitContinueStatement(MiniCParser::ContinueStatementContext *ctx) override;

    /// @brief 逻辑或表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理形如 "andExp || andExp" 的逻辑或表达式，支持短路求值
    std::any visitOrExp(MiniCParser::OrExpContext *ctx) override;

    /// @brief 逻辑与表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理形如 "equalExp && equalExp" 的逻辑与表达式，支持短路求值
    std::any visitAndExp(MiniCParser::AndExpContext *ctx) override;

    /// @brief 相等性表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理形如 "relationExp == relationExp" 或 "relationExp != relationExp" 的相等性比较表达式
    std::any visitEqualExp(MiniCParser::EqualExpContext *ctx) override;

    /// @brief 关系表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理形如 "addExp < addExp"、"addExp > addExp"、"addExp <= addExp"、"addExp >= addExp" 的关系比较表达式
    std::any visitRelationExp(MiniCParser::RelationExpContext *ctx) override;

    /// @brief 加法表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理形如 "mulExp + mulExp" 或 "mulExp - mulExp" 的加减法表达式，支持左结合
    std::any visitAddExp(MiniCParser::AddExpContext *ctx) override;

    /// @brief 乘法表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理形如 "unaryExp * unaryExp"、"unaryExp / unaryExp"、"unaryExp % unaryExp" 的乘除模表达式，支持左结合
    std::any visitMulExp(MiniCParser::MulExpContext *ctx) override;

    /// @brief 相等性运算符的分析
    /// @param ctx CST上下文
    /// @return 运算符类型（AST_OP_EQ 或 AST_OP_NE）
    /// @details 识别 "==" 和 "!=" 运算符，返回对应的AST运算符类型
    std::any visitEqualOp(MiniCParser::EqualOpContext *ctx) override;

    /// @brief 关系运算符的分析
    /// @param ctx CST上下文
    /// @return 运算符类型（AST_OP_LT、AST_OP_GT、AST_OP_LE、AST_OP_GE）
    /// @details 识别 "<"、">"、"<="、">=" 运算符，返回对应的AST运算符类型
    std::any visitRelationOp(MiniCParser::RelationOpContext *ctx) override;

    /// @brief 加法运算符的分析
    /// @param ctx CST上下文
    /// @return 运算符类型（AST_OP_ADD 或 AST_OP_SUB）
    /// @details 识别 "+" 和 "-" 运算符，返回对应的AST运算符类型
    std::any visitAddOp(MiniCParser::AddOpContext *ctx) override;

    /// @brief 乘法运算符的分析
    /// @param ctx CST上下文
    /// @return 运算符类型（AST_OP_MUL、AST_OP_DIV、AST_OP_MOD）
    /// @details 识别 "*"、"/"、"%" 运算符，返回对应的AST运算符类型
    std::any visitMulOp(MiniCParser::MulOpContext *ctx) override;

    /// @brief 一元表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理一元表达式，包括：基本表达式、负号表达式(-unaryExp)、逻辑非表达式(!unaryExp)、函数调用(ID(params))
    std::any visitUnaryExp(MiniCParser::UnaryExpContext *ctx) override;

    /// @brief 基本表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理最基本的表达式：整数字面量、左值表达式、带括号的表达式
    std::any visitPrimaryExp(MiniCParser::PrimaryExpContext *ctx) override;

    /// @brief 左值表达式的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理左值表达式，包括变量名和数组访问（支持多维数组）
    std::any visitLVal(MiniCParser::LValContext *ctx) override;

    /// @brief 变量声明的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理变量声明语句，支持同时声明多个变量，每个变量可以是标量或数组
    std::any visitVarDecl(MiniCParser::VarDeclContext *ctx) override;

    /// @brief 变量定义的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理单个变量的定义，支持标量变量（可选初始化）和数组变量（指定维度）
    std::any visitVarDef(MiniCParser::VarDefContext *ctx) override;

    /// @brief 基本类型的分析
    /// @param ctx CST上下文
    /// @return 类型属性
    /// @details 处理基本数据类型，目前支持int类型
    std::any visitBasicType(MiniCParser::BasicTypeContext *ctx) override;

    /// @brief 实际参数列表的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理函数调用时的实际参数列表，支持多个表达式参数
    std::any visitRealParamList(MiniCParser::RealParamListContext *ctx) override;

    /// @brief 表达式语句的分析
    /// @param ctx CST上下文
    /// @return AST的节点
    /// @details 处理表达式语句和空语句，表达式语句以分号结尾
    std::any visitExpressionStatement(MiniCParser::ExpressionStatementContext *context) override;
};
