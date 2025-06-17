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

/// @brief 类型转换宏，用于安全的动态类型转换
/// @param res 转换结果变量名
/// @param type 目标类型
/// @param var 源变量
#define Instanceof(res, type, var) auto res = dynamic_cast<type>(var)

/// @brief 构造函数
/// @details 初始化CST访问器，准备进行语法树遍历
MiniCCSTVisitor::MiniCCSTVisitor() {
}

/// @brief 析构函数
/// @details 清理资源，释放访问器占用的内存
MiniCCSTVisitor::~MiniCCSTVisitor() {
}

/// @brief 遍历CST产生AST
/// @param root CST语法树的根结点
/// @return AST的根节点
/// @details 编译器的入口函数，将ANTLR4生成的具体语法树转换为抽象语法树
ast_node *MiniCCSTVisitor::run(MiniCParser::CompileUnitContext *root) {
    return std::any_cast<ast_node *>(visitCompileUnit(root));
}

/// @brief 编译单元的遍历
/// @param ctx CST上下文
/// @return 编译单元的AST节点
/// @details 处理整个编译单元，包含全局变量声明和函数定义
/// @note 当前实现先处理所有全局变量，再处理所有函数定义，这可能导致某些语义错误无法检测
/// @todo 需要增加行号检查，确保函数中使用的全局变量在函数定义之前已声明
std::any MiniCCSTVisitor::visitCompileUnit(MiniCParser::CompileUnitContext *ctx) {
    // 文法产生式：compileUnit: (funcDef | varDecl)* EOF

    // 请注意这里必须先遍历全局变量后遍历函数。肯定可以确保全局变量先声明后使用的规则，但有些情况却不能检查出。
    // 事实上可能函数A后全局变量B后函数C，这时在函数A中是不能使用变量B的，需要报语义错误，但目前的处理不会。
    // 因此在进行语义检查时，可能追加检查行号和列号，如果函数的行号/列号在全局变量的行号/列号的前面则需要报语义错误
    // TODO 请追加实现。

    ast_node *temp_node;
    ast_node *compileUnitNode = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT);

    // 第一遍：处理所有全局变量声明
    for (auto varCtx : ctx->varDecl()) {
        temp_node = std::any_cast<ast_node *>(visitVarDecl(varCtx));
        (void)compileUnitNode->insert_son_node(temp_node);
    }

    // 第二遍：处理所有函数定义
    for (auto funcCtx : ctx->funcDef()) {
        temp_node = std::any_cast<ast_node *>(visitFuncDef(funcCtx));
        (void)compileUnitNode->insert_son_node(temp_node);
    }

    return compileUnitNode;
}

/// @brief 函数定义的遍历
/// @param ctx CST上下文
/// @return 函数定义的AST节点
/// @details 处理函数定义，包括返回类型、函数名、形参列表和函数体
std::any MiniCCSTVisitor::visitFuncDef(MiniCParser::FuncDefContext *ctx) {
    // 文法产生式：funcDef : returnType T_ID T_L_PAREN formalParamList? T_R_PAREN block

    // 解析函数返回类型
    type_attr funcReturnType = std::any_cast<type_attr>(visitReturnType(ctx->returnType()));

    // 获取函数名和行号信息
    char *id = strdup(ctx->T_ID()->getText().c_str());
    var_id_attr funcId{id, (int64_t)ctx->T_ID()->getSymbol()->getLine()};

    // 处理形参列表（可选）
    ast_node *formalParamsNode = nullptr;
    if (ctx->formalParamList()) {
        formalParamsNode = std::any_cast<ast_node *>(visitFormalParamList(ctx->formalParamList()));
    }

    // 处理函数体
    auto blockNode = std::any_cast<ast_node *>(visitBlock(ctx->block()));

    // 创建函数定义节点
    // 注意：create_func_def函数内会释放funcId中指向的标识符空间，之后不要再释放
    return create_func_def(funcReturnType, funcId, blockNode, formalParamsNode);
}

/// @brief 返回类型的遍历
/// @param ctx CST上下文
/// @return 类型属性
/// @details 处理函数返回类型，支持基本类型和void类型
std::any MiniCCSTVisitor::visitReturnType(MiniCParser::ReturnTypeContext *ctx) {
    if (ctx->basicType()) {
        // 基本类型（如int）
        return std::any_cast<type_attr>(visitBasicType(ctx->basicType()));
    } else {
        // void类型
        int64_t line = ctx->T_VOID()->getSymbol()->getLine();
        return type_attr{BasicType::TYPE_VOID, line};
    }
}

/// @brief 形参列表的遍历
/// @param ctx CST上下文
/// @return 形参列表的AST节点
/// @details 处理函数的形式参数列表，支持标量参数和数组参数（包括多维数组）
/// @note 数组参数的第一维可以省略，后续维度必须指定大小
std::any MiniCCSTVisitor::visitFormalParamList(MiniCParser::FormalParamListContext *ctx) {
    // 文法产生式：formalParamList: basicType formalParam (T_COMMA basicType formalParam)*
    // formalParam: T_ID (T_L_BRACKET expr? T_R_BRACKET) (T_L_BRACKET expr T_R_BRACKET)*

    // 创建形参容器节点
    ast_node *paramsNode = create_contain_node(ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS);

    // 遍历每个参数：basicType[i] 与 formalParam[i] 配对
    for (size_t i = 0; i < ctx->basicType().size(); ++i) {
        // 1) 解析参数类型
        type_attr t = std::any_cast<type_attr>(visitBasicType(ctx->basicType(i)));
        ast_node *typeNode = create_type_node(t);

        // 2) 解析参数名
        auto *fpCtx = ctx->formalParam(i);
        char *name = strdup(fpCtx->T_ID()->getText().c_str());
        var_id_attr vid{name, (int64_t)fpCtx->T_ID()->getSymbol()->getLine()};
        ast_node *idNode = ast_node::New(vid);

        // 3) 处理数组维度（如果存在）
        size_t dimCount = fpCtx->T_L_BRACKET().size();
        auto exprList = fpCtx->expr();  // 维度表达式列表

        ast_node *dimsNode = nullptr;
        if (dimCount > 0) {
            // 创建维度容器节点
            dimsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_DIMS);

            // 处理维度表达式：第一维可能省略，从适当位置开始
            size_t start = (exprList.size() == dimCount ? 1 : 0);
            for (size_t j = start; j < exprList.size(); ++j) {
                ast_node *dimExpr = std::any_cast<ast_node *>(visitExpr(exprList[j]));
                dimsNode->insert_son_node(dimExpr);
            }
        }

        // 4) 创建单个形参节点：类型 + 标识符 + 维度信息
        ast_node *paramNode = ast_node::New(ast_operator_type::AST_OP_FUNC_FORMAL_PARAM,
                                          typeNode, idNode, dimsNode, nullptr);

        paramsNode->insert_son_node(paramNode);
    }

    return paramsNode;
}

/// @brief 语句块的遍历
/// @param ctx CST上下文
/// @return 语句块的AST节点
/// @details 处理用大括号包围的语句块，可能为空或包含多个语句项
std::any MiniCCSTVisitor::visitBlock(MiniCParser::BlockContext *ctx) {
    // 文法产生式：block : T_L_BRACE blockItemList? T_R_BRACE

    if (!ctx->blockItemList()) {
        // 空语句块：{}
        return create_contain_node(ast_operator_type::AST_OP_BLOCK);
    }

    // 非空语句块：委托给blockItemList处理
    return visitBlockItemList(ctx->blockItemList());
}

/// @brief 语句项列表的遍历
/// @param ctx CST上下文
/// @return 语句块的AST节点
/// @details 处理语句块中的语句项列表，每个语句项可以是语句或变量声明
std::any MiniCCSTVisitor::visitBlockItemList(MiniCParser::BlockItemListContext *ctx) {
    // 文法产生式：blockItemList : blockItem+
    // 正闭包：至少包含一个blockItem

    auto block_node = create_contain_node(ast_operator_type::AST_OP_BLOCK);

    // 遍历所有语句项
    for (auto blockItemCtx : ctx->blockItem()) {
        auto blockItem = std::any_cast<ast_node *>(visitBlockItem(blockItemCtx));
        (void)block_node->insert_son_node(blockItem);
    }

    return block_node;
}

/// @brief 语句项的遍历
/// @param ctx CST上下文
/// @return 语句项的AST节点
/// @details 处理单个语句项，可以是语句或变量声明
std::any MiniCCSTVisitor::visitBlockItem(MiniCParser::BlockItemContext *ctx) {
    // 文法产生式：blockItem : statement | varDecl

    if (ctx->statement()) {
        // 语句类型
        return visitStatement(ctx->statement());
    } else if (ctx->varDecl()) {
        // 变量声明类型
        return visitVarDecl(ctx->varDecl());
    }

    return nullptr;
}

/// @brief 语句的遍历（语句分发器）
/// @param ctx CST上下文
/// @return 语句的AST节点
/// @details 根据具体的语句类型分发到相应的处理函数
/// @note 支持的语句类型：赋值、返回、块、表达式、if、while、break、continue
std::any MiniCCSTVisitor::visitStatement(MiniCParser::StatementContext *ctx) {
    // 文法产生式包含多个分支：
    // statement: lVal T_ASSIGN expr T_SEMICOLON     # assignStatement
    //          | T_RETURN expr? T_SEMICOLON         # returnStatement
    //          | block                              # blockStatement
    //          | expr? T_SEMICOLON                  # expressionStatement
    //          | T_IF T_L_PAREN expr T_R_PAREN statement (T_ELSE statement)? # ifStatement
    //          | T_WHILE T_L_PAREN expr T_R_PAREN statement # whileStatement
    //          | T_BREAK T_SEMICOLON                # breakStatement
    //          | T_CONTINUE T_SEMICOLON             # continueStatement

    // 使用动态类型转换进行语句类型识别和分发
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

/// @brief 返回语句的遍历
/// @param ctx CST上下文
/// @return 返回语句的AST节点
/// @details 处理return语句，支持有返回值和无返回值两种情况
std::any MiniCCSTVisitor::visitReturnStatement(MiniCParser::ReturnStatementContext *ctx) {
    // 文法产生式：returnStatement -> T_RETURN expr? T_SEMICOLON

    ast_node *exprNode = nullptr;
    if (ctx->expr()) {
        // 有返回值的return语句
        exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));
    }
    // 无返回值的return语句：exprNode保持为nullptr

    // 创建返回节点
    return create_contain_node(ast_operator_type::AST_OP_RETURN, exprNode);
}

/// @brief 赋值语句的遍历
/// @param ctx CST上下文
/// @return 赋值语句的AST节点
/// @details 处理形如 "lVal = expr;" 的赋值语句，支持变量和数组元素赋值
std::any MiniCCSTVisitor::visitAssignStatement(MiniCParser::AssignStatementContext *ctx) {
    // 文法产生式：assignStatement: lVal T_ASSIGN expr T_SEMICOLON

    // 处理赋值左侧的左值（变量或数组元素）
    auto lvalNode = std::any_cast<ast_node *>(visitLVal(ctx->lVal()));

    // 处理赋值右侧的表达式
    auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 创建赋值节点：左值 = 表达式
    return ast_node::New(ast_operator_type::AST_OP_ASSIGN, lvalNode, exprNode, nullptr);
}

/// @brief 块语句的遍历
/// @param ctx CST上下文
/// @return 块语句的AST节点
/// @details 处理块语句，直接委托给block处理
std::any MiniCCSTVisitor::visitBlockStatement(MiniCParser::BlockStatementContext *ctx) {
    // 文法产生式：blockStatement: block
    return visitBlock(ctx->block());
}

/// @brief if语句的遍历
/// @param ctx CST上下文
/// @return if语句的AST节点
/// @details 处理if和if-else语句，支持条件分支控制
std::any MiniCCSTVisitor::visitIfStatement(MiniCParser::IfStatementContext *ctx) {
    // 文法产生式：T_IF T_L_PAREN expr T_R_PAREN statement (T_ELSE statement)?

    // 1) 处理条件表达式
    auto cond = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 2) 处理then分支语句
    auto thenResult = visitStatement(ctx->statement()[0]);
    ast_node *thenNode = nullptr;
    if (thenResult.has_value()) {
        try {
            thenNode = std::any_cast<ast_node *>(thenResult);
        } catch (const std::bad_any_cast&) {
            // 转换失败说明是空语句，thenNode保持为nullptr
        }
    }

    // 3) 处理else分支语句（可选）
    if (ctx->T_ELSE()) {
        // 有else分支
        auto elseResult = visitStatement(ctx->statement()[1]);
        ast_node *elseNode = nullptr;
        if (elseResult.has_value()) {
            try {
                elseNode = std::any_cast<ast_node *>(elseResult);
            } catch (const std::bad_any_cast&) {
                // 转换失败说明是空语句，elseNode保持为nullptr
            }
        }
        // 创建if-else节点：条件 + then分支 + else分支
        return ast_node::New(ast_operator_type::AST_OP_IF, cond, thenNode, elseNode, nullptr);
    } else {
        // 只有if分支，没有else
        return ast_node::New(ast_operator_type::AST_OP_IF, cond, thenNode, nullptr);
    }
}

/// @brief while循环语句的遍历
/// @param ctx CST上下文
/// @return while语句的AST节点
/// @details 处理while循环语句，包含条件表达式和循环体
std::any MiniCCSTVisitor::visitWhileStatement(MiniCParser::WhileStatementContext *ctx) {
    // 文法产生式：T_WHILE T_L_PAREN expr T_R_PAREN statement

    // 处理循环条件
    auto cond = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 处理循环体，可能是空语句
    auto bodyResult = visitStatement(ctx->statement());
    ast_node *body = nullptr;
    if (bodyResult.has_value()) {
        try {
            body = std::any_cast<ast_node *>(bodyResult);
        } catch (const std::bad_any_cast&) {
            // 转换失败说明是空语句，body保持为nullptr
        }
    }

    // 创建while节点：条件 + 循环体
    return ast_node::New(ast_operator_type::AST_OP_WHILE, cond, body, nullptr);
}

/// @brief break语句的遍历
/// @param ctx CST上下文
/// @return break语句的AST节点
/// @details 处理循环中的break跳出语句
std::any MiniCCSTVisitor::visitBreakStatement(MiniCParser::BreakStatementContext *ctx) {
    // 文法产生式：T_BREAK T_SEMICOLON
    return ast_node::New(ast_operator_type::AST_OP_BREAK, nullptr);
}

/// @brief continue语句的遍历
/// @param ctx CST上下文
/// @return continue语句的AST节点
/// @details 处理循环中的continue继续语句
std::any MiniCCSTVisitor::visitContinueStatement(MiniCParser::ContinueStatementContext *ctx) {
    // 文法产生式：T_CONTINUE T_SEMICOLON
    return ast_node::New(ast_operator_type::AST_OP_CONTINUE, nullptr);
}

/// @brief 表达式的遍历（表达式入口）
/// @param ctx CST上下文
/// @return 表达式的AST节点
/// @details 表达式解析的入口点，直接转发到逻辑或表达式处理
std::any MiniCCSTVisitor::visitExpr(MiniCParser::ExprContext *ctx) {
    // 文法产生式：expr: orExp
    return visitOrExp(ctx->orExp());
}

/// @brief 逻辑或表达式的遍历
/// @param ctx CST上下文
/// @return 逻辑或表达式的AST节点
/// @details 处理形如 "andExp || andExp" 的逻辑或表达式，支持左结合和短路求值
std::any MiniCCSTVisitor::visitOrExp(MiniCParser::OrExpContext *ctx) {
    // 文法产生式：orExp: andExp (T_OR andExp)*

    if (ctx->T_OR().empty()) {
        // 没有||运算符，只有一个andExp
        return visitAndExp(ctx->andExp()[0]);
    }

    // 存在||运算符，需要构建左结合的表达式树
    ast_node *left, *right;
    auto opsCtxVec = ctx->T_OR();

    // 逐个处理||运算符，构建左结合的AST
    for (int k = 0; k < (int)opsCtxVec.size(); k++) {
        ast_operator_type op = ast_operator_type::AST_OP_OR;

        if (k == 0) {
            // 第一个操作数
            left = std::any_cast<ast_node *>(visitAndExp(ctx->andExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitAndExp(ctx->andExp()[k + 1]));

        // 创建新节点作为下一次迭代的左操作数
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}

/// @brief 逻辑与表达式的遍历
/// @param ctx CST上下文
/// @return 逻辑与表达式的AST节点
/// @details 处理形如 "equalExp && equalExp" 的逻辑与表达式，支持左结合和短路求值
std::any MiniCCSTVisitor::visitAndExp(MiniCParser::AndExpContext *ctx) {
    // 文法产生式：andExp: equalExp (T_AND equalExp)*

    if (ctx->T_AND().empty()) {
        // 没有&&运算符，只有一个equalExp
        return visitEqualExp(ctx->equalExp()[0]);
    }

    // 存在&&运算符，需要构建左结合的表达式树
    ast_node *left, *right;
    auto opsCtxVec = ctx->T_AND();

    // 逐个处理&&运算符，构建左结合的AST
    for (int k = 0; k < (int)opsCtxVec.size(); k++) {
        ast_operator_type op = ast_operator_type::AST_OP_AND;

        if (k == 0) {
            // 第一个操作数
            left = std::any_cast<ast_node *>(visitEqualExp(ctx->equalExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitEqualExp(ctx->equalExp()[k + 1]));

        // 创建新节点作为下一次迭代的左操作数
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

/// @brief 左值表达式的遍历
/// @param ctx CST上下文
/// @return 左值表达式的AST节点
/// @details 处理左值表达式，包括变量名和数组访问（支持多维数组）
std::any MiniCCSTVisitor::visitLVal(MiniCParser::LValContext *ctx) {
    // 文法产生式：lVal: T_ID (T_L_BRACKET expr T_R_BRACKET)*

    // 获取变量标识符和行号信息
    auto varId = ctx->T_ID()->getText();
    int64_t lineNo = (int64_t)ctx->T_ID()->getSymbol()->getLine();
    ast_node *idNode = ast_node::New(varId, lineNo);

    // 检查是否有数组下标
    size_t dimCount = ctx->T_L_BRACKET().size();  // '[' 的个数
    auto exprList = ctx->expr();

    if (dimCount == 0) {
        // 普通变量访问
        return ast_node::New(varId, lineNo);
    }

    // 数组访问：收集所有下标表达式
    ast_node *dimsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_DIMS);

    for (size_t i = 0; i < exprList.size(); ++i) {
        // 依次处理每一维的下标表达式
        ast_node *dimExpr = std::any_cast<ast_node *>(visitExpr(exprList[i]));
        dimsNode->insert_son_node(dimExpr);
    }

    // 构造数组访问节点：变量名 + 下标表达式列表
    return ast_node::New(ast_operator_type::AST_OP_ARRAY_ACCESS,
                         idNode,
                         dimsNode,
                         nullptr);
}

/// @brief 变量声明的遍历
/// @param ctx CST上下文
/// @return 变量声明语句的AST节点
/// @details 处理变量声明语句，支持同时声明多个变量，每个变量可以是标量或数组
std::any MiniCCSTVisitor::visitVarDecl(MiniCParser::VarDeclContext *ctx) {
    // 文法产生式：varDecl: basicType varDef (T_COMMA varDef)* T_SEMICOLON

    // 创建声明语句容器节点
    ast_node *stmt_node = create_contain_node(ast_operator_type::AST_OP_DECL_STMT);

    // 解析变量类型
    type_attr typeAttr = std::any_cast<type_attr>(visitBasicType(ctx->basicType()));

    // 处理每个变量定义
    for (auto &varCtx : ctx->varDef()) {
        // 处理单个变量定义（可能是标量或数组）
        ast_node *id_node = std::any_cast<ast_node *>(visitVarDef(varCtx));

        // 为每个变量创建独立的类型节点
        ast_node *type_node = create_type_node(typeAttr);

        // 创建变量声明节点：类型 + 变量定义
        ast_node *decl_node = ast_node::New(ast_operator_type::AST_OP_VAR_DECL, type_node, id_node, nullptr);

        // 添加到声明语句中
        (void)stmt_node->insert_son_node(decl_node);
    }

    return stmt_node;
}

/// @brief 变量定义的遍历
/// @param ctx CST上下文
/// @return 变量定义的AST节点
/// @details 处理单个变量的定义，支持标量变量（可选初始化）和数组变量（指定维度）
std::any MiniCCSTVisitor::visitVarDef(MiniCParser::VarDefContext *ctx) {
    // 文法产生式：varDef: T_ID ('=' expr)? | T_ID (T_L_BRACKET expr T_R_BRACKET)*

    // 获取变量标识符和行号
    auto varId = ctx->T_ID()->getText();
    int64_t lineNo = (int64_t)ctx->T_ID()->getSymbol()->getLine();
    ast_node *idNode = ast_node::New(varId, lineNo);

    // 检查是否为数组定义
    size_t dimCount = ctx->T_L_BRACKET().size();  // '[' 的个数
    auto exprList = ctx->expr();

    if (dimCount > 0) {
        // 数组定义分支
        ast_node *dimsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_DIMS);

        // 处理每一维的大小表达式
        for (size_t i = 0; i < exprList.size(); ++i) {
            ast_node *dimExpr = std::any_cast<ast_node *>(visitExpr(exprList[i]));
            dimsNode->insert_son_node(dimExpr);
        }

        // 返回数组定义节点：标识符 + 维度信息
        return ast_node::New(ast_operator_type::AST_OP_ARRAY_DEF,
                             idNode,
                             dimsNode,
                             nullptr);
    } else {
        // 标量变量定义分支（可选初始化）
        ast_node *initNode = nullptr;
        if (!exprList.empty()) {
            // 有初始化表达式
            initNode = std::any_cast<ast_node *>(visitExpr(exprList[0]));
        }

        // 返回标量变量定义节点：标识符 + 初始化表达式（可能为nullptr）
        return ast_node::New(ast_operator_type::AST_OP_VAR_DEF,
                             idNode,
                             initNode,
                             nullptr);
    }
}

/// @brief 基本类型的遍历
/// @param ctx CST上下文
/// @return 类型属性
/// @details 处理基本数据类型，目前支持int类型
std::any MiniCCSTVisitor::visitBasicType(MiniCParser::BasicTypeContext *ctx) {
    // 文法产生式：basicType: T_INT

    type_attr attr{BasicType::TYPE_VOID, -1};
    if (ctx->T_INT()) {
        attr.type = BasicType::TYPE_INT;
        attr.lineno = (int64_t)ctx->T_INT()->getSymbol()->getLine();
    }

    return attr;
}

/// @brief 实际参数列表的遍历
/// @param ctx CST上下文
/// @return 实际参数列表的AST节点
/// @details 处理函数调用时的实际参数列表，支持多个表达式参数
std::any MiniCCSTVisitor::visitRealParamList(MiniCParser::RealParamListContext *ctx) {
    // 文法产生式：realParamList : expr (T_COMMA expr)*

    // 创建实参列表容器节点
    auto paramListNode = create_contain_node(ast_operator_type::AST_OP_FUNC_REAL_PARAMS);

    // 处理每个实际参数表达式
    for (auto paramCtx : ctx->expr()) {
        auto paramNode = std::any_cast<ast_node *>(visitExpr(paramCtx));
        paramListNode->insert_son_node(paramNode);
    }

    return paramListNode;
}

/// @brief 表达式语句的遍历
/// @param ctx CST上下文
/// @return 表达式语句的AST节点
/// @details 处理表达式语句和空语句，表达式语句以分号结尾
std::any MiniCCSTVisitor::visitExpressionStatement(MiniCParser::ExpressionStatementContext *ctx) {
    // 文法产生式：expr? T_SEMICOLON

    if (ctx->expr()) {
        // 有表达式的语句
        return visitExpr(ctx->expr());
    } else {
        // 空语句（只有分号）
        // 返回包含nullptr的std::any，避免std::bad_any_cast错误
        return std::any(static_cast<ast_node*>(nullptr));
    }
}