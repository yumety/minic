///
/// @file IRGenerator.cpp
/// @brief AST遍历产生线性IR的源文件
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
#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "AST.h"
#include "Common.h"
#include "Function.h"
#include "IRCode.h"
#include "IRGenerator.h"
#include "Module.h"
#include "EntryInstruction.h"
#include "LabelInstruction.h"
#include "ExitInstruction.h"
#include "FuncCallInstruction.h"
#include "BinaryInstruction.h"
#include "MoveInstruction.h"
#include "LoadArrayInstruction.h"
#include "StoreArrayInstruction.h"
#include "ArrayType.h"
#include "ConstInt.h"
#include "PointerType.h"
#include "ArraySliceInstruction.h"
#include <map>
#include <typeinfo>

// 全局映射：保存数组访问节点的地址信息
static std::map<ast_node*, Value*> g_arrayAccessAddresses;
#include "GotoInstruction.h"
#include "ArgInstruction.h"

/// @brief 构造函数
/// @param _root AST的根
/// @param _module 符号表
IRGenerator::IRGenerator(ast_node *_root, Module *_module) :
    root(_root), module(_module) {
    /* 叶子节点 */
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_LITERAL_UINT] = &IRGenerator::ir_leaf_node_uint;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_VAR_ID] = &IRGenerator::ir_leaf_node_var_id;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_TYPE] = &IRGenerator::ir_leaf_node_type;

    /* 表达式运算 */
    ast2ir_handlers[ast_operator_type::AST_OP_SUB] = &IRGenerator::ir_sub;
    ast2ir_handlers[ast_operator_type::AST_OP_ADD] = &IRGenerator::ir_add;
    ast2ir_handlers[ast_operator_type::AST_OP_NEG] = &IRGenerator::ir_neg;
    ast2ir_handlers[ast_operator_type::AST_OP_MUL] = &IRGenerator::ir_mul;
    ast2ir_handlers[ast_operator_type::AST_OP_DIV] = &IRGenerator::ir_div;
    ast2ir_handlers[ast_operator_type::AST_OP_MOD] = &IRGenerator::ir_mod;

    ast2ir_handlers[ast_operator_type::AST_OP_LT] = &IRGenerator::ir_lt;
    ast2ir_handlers[ast_operator_type::AST_OP_GT] = &IRGenerator::ir_gt;
    ast2ir_handlers[ast_operator_type::AST_OP_LE] = &IRGenerator::ir_le;
    ast2ir_handlers[ast_operator_type::AST_OP_GE] = &IRGenerator::ir_ge;
    ast2ir_handlers[ast_operator_type::AST_OP_EQ] = &IRGenerator::ir_eq;
    ast2ir_handlers[ast_operator_type::AST_OP_NE] = &IRGenerator::ir_ne;

    ast2ir_handlers[ast_operator_type::AST_OP_AND] = &IRGenerator::ir_and;
    ast2ir_handlers[ast_operator_type::AST_OP_OR] = &IRGenerator::ir_or;
    ast2ir_handlers[ast_operator_type::AST_OP_NOT] = &IRGenerator::ir_not;

    ast2ir_handlers[ast_operator_type::AST_OP_IF] = &IRGenerator::ir_if;
    ast2ir_handlers[ast_operator_type::AST_OP_WHILE] = &IRGenerator::ir_while;
    ast2ir_handlers[ast_operator_type::AST_OP_BREAK] = &IRGenerator::ir_break;
    ast2ir_handlers[ast_operator_type::AST_OP_CONTINUE] = &IRGenerator::ir_continue;

    /* 语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_ASSIGN] = &IRGenerator::ir_assign;
    ast2ir_handlers[ast_operator_type::AST_OP_RETURN] = &IRGenerator::ir_return;

    /* 函数调用 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_CALL] = &IRGenerator::ir_function_call;

    /* 函数定义 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_DEF] = &IRGenerator::ir_function_define;
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS] = &IRGenerator::ir_function_formal_params;
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_CALL] = &IRGenerator::ir_function_call;

    /* 变量定义语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_DECL_STMT] = &IRGenerator::ir_declare_statment;
    ast2ir_handlers[ast_operator_type::AST_OP_VAR_DECL] = &IRGenerator::ir_variable_declare;
    ast2ir_handlers[ast_operator_type::AST_OP_ARRAY_ACCESS] = &IRGenerator::ir_array_access;
    // ast2ir_handlers[ast_operator_type::AST_OP_ARRAY_DIMS] = &IRGenerator::ir_array_dims;

    /* 语句块 */
    ast2ir_handlers[ast_operator_type::AST_OP_BLOCK] = &IRGenerator::ir_block;

    /* 编译单元 */
    ast2ir_handlers[ast_operator_type::AST_OP_COMPILE_UNIT] = &IRGenerator::ir_compile_unit;
}

/// @brief 遍历抽象语法树产生线性IR，保存到IRCode中
/// @param root 抽象语法树
/// @param IRCode 线性IR
/// @return true: 成功 false: 失败
bool IRGenerator::run() {
    ast_node *node;

    // 从根节点进行遍历
    node = ir_visit_ast_node(root);

    return node != nullptr;
}

/// @brief 根据AST的节点运算符查找对应的翻译函数并执行翻译动作
/// @param node AST节点
/// @return 成功返回node节点，否则返回nullptr
ast_node *IRGenerator::ir_visit_ast_node(ast_node *node) {
    // 空节点
    if (nullptr == node) {
        return nullptr;
    }

    bool result;

    std::unordered_map<ast_operator_type, ast2ir_handler_t>::const_iterator pIter;
    pIter = ast2ir_handlers.find(node->node_type);
    if (pIter == ast2ir_handlers.end()) {
        // 没有找到，则说明当前不支持
        result = (this->ir_default)(node);
    } else {
        result = (this->*(pIter->second))(node);
    }

    if (!result) {
        // 语义解析错误，则出错返回
        node = nullptr;
    }

    return node;
}

/// @brief 未知节点类型的节点处理
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_default(ast_node *node) {
    // 未知的节点
    printf("Unkown node(%d)\n", (int)node->node_type);
    return true;
}

/// @brief 编译单元AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_compile_unit(ast_node *node) {
    module->setCurrentFunction(nullptr);

    for (auto son : node->sons) {
        // 遍历编译单元，要么是函数定义，要么是语句
        ast_node *son_node = ir_visit_ast_node(son);
        if (!son_node) {
            // TODO 自行追加语义错误处理
            return false;
        }
    }

    return true;
}

/// @brief 函数定义AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_define(ast_node *node) {
    // 确保不在函数内嵌套定义函数
    if (module->getCurrentFunction()) return false;

    // sons: 0:returnType, 1:name, 2:params, 3:block
    ast_node *type_node = node->sons[0];
    ast_node *name_node = node->sons[1];
    ast_node *param_node = node->sons[2];
    ast_node *block_node = node->sons[3];

    Function *newFunc = module->newFunction(name_node->name, type_node->type);
    if (!newFunc) return false;
    module->setCurrentFunction(newFunc);
    module->enterScope();

    InterCode &irCode = newFunc->getInterCode();
    // Entry
    irCode.addInst(new EntryInstruction(newFunc));
    // Prepare exit label
    LabelInstruction *exitLabel = new LabelInstruction(newFunc);
    newFunc->setExitLabel(exitLabel);

    // 形式参数值拷贝
    if (!ir_function_formal_params(param_node)) return false;
    node->blockInsts.addInst(param_node->blockInsts);

    // 返回值分配与初始化
    LocalVariable *retVal = nullptr;
    if (!type_node->type->isVoidType()) {
        retVal = static_cast<LocalVariable *>(module->newVarValue(type_node->type));
        newFunc->setReturnValue(retVal);
        // 初始化为0
        node->blockInsts.addInst(
            new MoveInstruction(newFunc, retVal, module->newConstInt(0)));
    }

    // 生成函数体
    block_node->needScope = false;
    if (!ir_block(block_node)) return false;
    node->blockInsts.addInst(block_node->blockInsts);

    // 添加 body 到函数IR
    irCode.addInst(node->blockInsts);

    // 添加 exit label + Exit
    irCode.addInst(exitLabel);
    irCode.addInst(new ExitInstruction(newFunc, retVal));

    module->leaveScope();
    module->setCurrentFunction(nullptr);
    return true;
}

/// @brief 形式参数AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_formal_params(ast_node *node) {
    Function *current = module->getCurrentFunction();

    InterCode &irCode = current->getInterCode();

    // 每个形参变量都创建对应的临时变量，用于表达实参转递的值
    for (size_t i = 0; i < node->sons.size(); ++i) {
         // AST_OP_FUNC_FORMAL_PARAM
        ast_node *paramAST = node->sons[i];

        // 安全地检查子节点数量
        if (paramAST->sons.size() < 2) {
            return false; // 形参至少需要类型和名称
        }

        Type *ty = paramAST->sons[0]->type;
        std::string pname = paramAST->sons[1]->name;

        // 检查是否有第三个孩子（array-dims节点）
        if (paramAST->sons.size() > 2 && paramAST->sons[2] != nullptr) {
            ast_node *dimsNode = paramAST->sons[2];

            // 处理数组参数的维度信息
            std::vector<int32_t> dimSizes;
            dimSizes.push_back(0); // 第一个维度省略，用0表示

            // 处理后续维度
            for (auto dimChild : dimsNode->sons) {
                if (dimChild != nullptr) {
                    ast_node *dimNode = ir_visit_ast_node(dimChild);
                    if (!dimNode) {
                        return false;
                    }

                    // 尝试获取常量维度大小
                    if (ConstInt *constDim = dynamic_cast<ConstInt*>(dimNode->val)) {
                        dimSizes.push_back(constDim->getVal());
                    } else {
                        // 如果不是常量，使用默认值
                        dimSizes.push_back(10);
                    }
                }
            }

            // 创建数组类型，第一维为0
            ArrayType *arrayType = new ArrayType(ty, dimSizes);
            ty = arrayType;
        }

        FormalParam *fp = new FormalParam(ty, pname);
        current->getParams().push_back(fp);
    }

    for (size_t i = 0; i < node->sons.size(); ++i) {
        ast_node *pAST = node->sons[i];
        Type *ty = current->getParams()[i]->getType(); // 使用已处理的类型
        std::string pname = pAST->sons[1]->name;

        // 而真实的形参则创建函数内的局部变量
        LocalVariable *localVar =
            static_cast<LocalVariable *>(module->newVarValue(ty, pname));
        FormalParam *fp = current->getParams()[i];

        // 然后产生赋值指令，用于把表达实参值的临时变量拷贝到形参局部变量上
        irCode.addInst(new MoveInstruction(current, localVar, fp));
    }

    return true;
}

/// @brief 函数调用AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_call(ast_node * node)
{
    std::vector<Value *> realParams;

    // 获取当前正在处理的函数
    Function * currentFunc = module->getCurrentFunction();

    // 函数调用的节点包含两个节点：
    // 第一个节点：函数名节点
    // 第二个节点：实参列表节点

    std::string funcName = node->sons[0]->name;
    int64_t lineno = node->sons[0]->line_no;

    ast_node * paramsNode = node->sons[1];

    // 根据函数名查找函数，看是否存在。若不存在则出错
    // 这里约定函数必须先定义后使用
    auto calledFunction = module->findFunction(funcName);
    if (nullptr == calledFunction) {
        minic_log(LOG_ERROR, "函数(%s)未定义或声明", funcName.c_str());
        return false;
    }

    // 当前函数存在函数调用
    currentFunc->setExistFuncCall(true);

    // 如果没有孩子，也认为是没有参数
    if (!paramsNode->sons.empty()) {

        int32_t argsCount = (int32_t) paramsNode->sons.size();

        // 当前函数中调用函数实参个数最大值统计，实际上是统计实参传参需在栈中分配的大小
        // 因为目前的语言支持的int和float都是四字节的，只统计个数即可
        if (argsCount > currentFunc->getMaxFuncCallArgCnt()) {
            currentFunc->setMaxFuncCallArgCnt(argsCount);
        }

        // 遍历参数列表，孩子是表达式
        // 这里自左往右计算表达式
        for (auto son: paramsNode->sons) {

            // 遍历Block的每个语句，进行显示或者运算
            ast_node * temp = ir_visit_ast_node(son);
            if (!temp) {
                return false;
            }

            Value *paramValue = temp->val;

            // 如果实参是数组变量（变量名，不是数组访问），需要传递数组的首地址
            if (son->node_type == ast_operator_type::AST_OP_LEAF_VAR_ID &&
                temp->val->getType()->isArrayType()) {
                // 数组名作为实参，传递数组首地址
                // 这里temp->val就是数组变量本身，可以直接使用
                paramValue = temp->val;
            }

            realParams.push_back(paramValue);
            node->blockInsts.addInst(temp->blockInsts);
        }
    }

    // TODO 这里请追加函数调用的语义错误检查，这里只进行了函数参数的个数检查等，其它请自行追加。
    if (realParams.size() != calledFunction->getParams().size()) {
        // 函数参数的个数不一致，语义错误
        minic_log(LOG_ERROR, "第%lld行的被调用函数(%s)未定义或声明", (long long) lineno, funcName.c_str());
        return false;
    }

    // 返回调用有返回值，则需要分配临时变量，用于保存函数调用的返回值
    Type * type = calledFunction->getReturnType();

    FuncCallInstruction * funcCallInst = new FuncCallInstruction(currentFunc, calledFunction, realParams, type);

    // 创建函数调用指令
    node->blockInsts.addInst(funcCallInst);

    // 函数调用结果Value保存到node中，可能为空，上层节点可利用这个值
    node->val = funcCallInst;

    return true;
}

/// @brief 语句块（含函数体）AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_block(ast_node *node) {
    // 进入作用域
    if (node->needScope) {
        module->enterScope();
    }

    std::vector<ast_node *>::iterator pIter;
    for (pIter = node->sons.begin(); pIter != node->sons.end(); ++pIter) {
        // 遍历Block的每个语句，进行显示或者运算
        ast_node *temp = ir_visit_ast_node(*pIter);
        if (!temp) {
            return false;
        }

        node->blockInsts.addInst(temp->blockInsts);
    }

    // 离开作用域
    if (node->needScope) {
        module->leaveScope();
    }

    return true;
}



/// @brief 整数加法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_add(ast_node *node) {
    ast_node *src1_node = node->sons[0];
    ast_node *src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node *left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node *right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction *addInst = new BinaryInstruction(module->getCurrentFunction(),
                                                       IRInstOperator::IRINST_OP_ADD_I,
                                                       left->val,
                                                       right->val,
                                                       IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(addInst);

    node->val = addInst;

    return true;
}

/// @brief 整数减法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_sub(ast_node *node) {
    ast_node *src1_node = node->sons[0];
    ast_node *src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node *left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node *right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction *subInst = new BinaryInstruction(module->getCurrentFunction(),
                                                       IRInstOperator::IRINST_OP_SUB_I,
                                                       left->val,
                                                       right->val,
                                                       IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(subInst);

    node->val = subInst;

    return true;
}

/// @brief 整数乘法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mul(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;

    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    BinaryInstruction *mulInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_MUL_I,
        left->val,
        right->val,
        IntegerType::getTypeInt());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(mulInst);

    node->val = mulInst;
    return true;
}

/// @brief 整数除法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_div(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;

    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    BinaryInstruction *divInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_DIV_I,
        left->val,
        right->val,
        IntegerType::getTypeInt());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(divInst);

    node->val = divInst;
    return true;
}

/// @brief 整数取余AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mod(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;

    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    BinaryInstruction *modInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_MOD_I,
        left->val,
        right->val,
        IntegerType::getTypeInt());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(modInst);

    node->val = modInst;
    return true;
}

/// @brief 单目负号AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_neg(ast_node *node) {
    ast_node *operand = ir_visit_ast_node(node->sons[0]);
    if (!operand) return false;

    Value *target = operand->val;
    Function *cur = module->getCurrentFunction();

    // 检查是否在全局作用域中（用于全局变量初始化）
    if (cur == nullptr) {
        // 全局作用域：只支持常量表达式
        if (ConstInt *constVal = dynamic_cast<ConstInt*>(target)) {
            // 对于常量，直接计算负值
            int32_t negValue = -constVal->getVal();
            node->val = module->newConstInt(negValue);
            return true;
        } else {
            // 全局变量初始化不支持非常量表达式
            printf("Error: Global variable initialization must use constant expressions\n");
            return false;
        }
    }

    // 函数作用域：生成运行时指令
    Value *zero = module->newConstInt(0);

    if (target->getType()->isInt1Byte()) {
        // 如果是 i1（布尔值），转成 i32（手动跳转转化）
        LabelInstruction *L_true = new LabelInstruction(cur);
        LabelInstruction *L_false = new LabelInstruction(cur);
        LabelInstruction *L_join = new LabelInstruction(cur);

        Value *tmpInt = module->newVarValue(IntegerType::getTypeInt());

        node->blockInsts.addInst(operand->blockInsts);
        node->blockInsts.addInst(new GotoInstruction(cur, target, L_true, L_false));

        // L_true
        node->blockInsts.addInst(L_true);
        node->blockInsts.addInst(new MoveInstruction(cur, tmpInt, module->newConstInt(1)));
        node->blockInsts.addInst(new GotoInstruction(cur, L_join));

        // L_false
        node->blockInsts.addInst(L_false);
        node->blockInsts.addInst(new MoveInstruction(cur, tmpInt, module->newConstInt(0)));
        node->blockInsts.addInst(new GotoInstruction(cur, L_join));

        // L_join:
        node->blockInsts.addInst(L_join);

        target = tmpInt;
    } else {
        // 正常 int 类型值，添加其 IR
        node->blockInsts.addInst(operand->blockInsts);
    }

    BinaryInstruction *negInst = new BinaryInstruction(
        cur,
        IRInstOperator::IRINST_OP_SUB_I,
        zero,
        target,
        IntegerType::getTypeInt());

    node->blockInsts.addInst(negInst);
    node->val = negInst;

    return true;
}

/// @brief 将i1类型的布尔值转换为i32类型的整数值
/// @param node AST节点
/// @param boolValue i1类型的布尔值
/// @return i32类型的整数值
Value* IRGenerator::convertBoolToInt(ast_node *node, Value *boolValue) {
    Function *cur = module->getCurrentFunction();
    Value *result = module->newVarValue(IntegerType::getTypeInt());
    auto *L_true = new LabelInstruction(cur);
    auto *L_false = new LabelInstruction(cur);
    auto *L_end = new LabelInstruction(cur);

    node->blockInsts.addInst(new GotoInstruction(cur, boolValue, L_true, L_false));

    // L_true: 布尔值为真
    node->blockInsts.addInst(L_true);
    node->blockInsts.addInst(new MoveInstruction(cur, result, module->newConstInt(1)));
    node->blockInsts.addInst(new GotoInstruction(cur, L_end));

    // L_false: 布尔值为假
    node->blockInsts.addInst(L_false);
    node->blockInsts.addInst(new MoveInstruction(cur, result, module->newConstInt(0)));
    node->blockInsts.addInst(new GotoInstruction(cur, L_end));

    // L_end
    node->blockInsts.addInst(L_end);

    return result;
}

/// @brief 小于比较AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_lt(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;
    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    // 生成 cmp lt 指令，结果类型为布尔 i1
    BinaryInstruction *cmpInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_LT_I,
        left->val,
        right->val,
        IntegerType::getTypeBool());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(cmpInst);

    // bool → int 转换
    node->val = convertBoolToInt(node, cmpInst);
    return true;
}

/// @brief 大于比较AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_gt(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;
    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    BinaryInstruction *cmpInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_GT_I,
        left->val,
        right->val,
        IntegerType::getTypeBool());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(cmpInst);

    // bool → int 转换
    node->val = convertBoolToInt(node, cmpInst);
    return true;
}

/// @brief 小于等于比较AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_le(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;
    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    BinaryInstruction *cmpInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_LE_I,
        left->val,
        right->val,
        IntegerType::getTypeBool());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(cmpInst);

    node->val = cmpInst;
    return true;
}

/// @brief 大于等于比较AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_ge(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;
    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    BinaryInstruction *cmpInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_GE_I,
        left->val,
        right->val,
        IntegerType::getTypeBool());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(cmpInst);

    node->val = cmpInst;
    return true;
}

/// @brief 等于比较AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_eq(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;
    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    BinaryInstruction *cmpInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_EQ_I,
        left->val,
        right->val,
        IntegerType::getTypeBool());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(cmpInst);

    node->val = cmpInst;
    return true;
}

/// @brief 不等于比较AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_ne(ast_node *node) {
    ast_node *left = ir_visit_ast_node(node->sons[0]);
    if (!left) return false;
    ast_node *right = ir_visit_ast_node(node->sons[1]);
    if (!right) return false;

    BinaryInstruction *cmpInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_NE_I,
        left->val,
        right->val,
        IntegerType::getTypeBool());

    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(cmpInst);

    node->val = cmpInst;
    return true;
}

/// @brief 逻辑与AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_and(ast_node *node) {
    ast_node *leftNode = ir_visit_ast_node(node->sons[0]);
    ast_node *rightNode = ir_visit_ast_node(node->sons[1]);
    if (!leftNode || !rightNode) return false;

    Function *cur = module->getCurrentFunction();
    // 创建标签
    auto *L_rhs = new LabelInstruction(cur);   // 左真、右评估入口
    auto *L_false = new LabelInstruction(cur); // 结果假时跳来
    auto *L_end = new LabelInstruction(cur);   // 表达式结束
    
    // 生成左操作数代码
    node->blockInsts.addInst(leftNode->blockInsts);
    Value *zero = module->newConstInt(0);
    auto *cmpL = new BinaryInstruction(
        cur,
        IRInstOperator::IRINST_OP_NE_I,
        leftNode->val,
        zero,
        IntegerType::getTypeBool());
    node->blockInsts.addInst(cmpL);

    // 短路：如果 left!=0（真），就 goto L_rhs；否则 goto L_false
    node->blockInsts.addInst(new GotoInstruction(cur, cmpL, L_rhs, L_false));

    // L_rhs: 计算右操作数
    node->blockInsts.addInst(L_rhs);
    node->blockInsts.addInst(rightNode->blockInsts);
    auto *cmpR = new BinaryInstruction(
        cur,
        IRInstOperator::IRINST_OP_NE_I,
        rightNode->val,
        zero,
        IntegerType::getTypeBool());
    node->blockInsts.addInst(cmpR);
    
    // 右操作数结果作为整个表达式的结果
    Value *result = convertBoolToInt(node, cmpR);
    node->blockInsts.addInst(new GotoInstruction(cur, L_end));

    // L_false: 左操作数为假，整个表达式为假
    node->blockInsts.addInst(L_false);
    node->blockInsts.addInst(new MoveInstruction(cur, result, module->newConstInt(0)));
    node->blockInsts.addInst(new GotoInstruction(cur, L_end));

    // L_end
    node->blockInsts.addInst(L_end);
    node->val = result;
    return true;
}

/// @brief 逻辑或AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_or(ast_node *node) {
    ast_node *leftNode = ir_visit_ast_node(node->sons[0]);
    ast_node *rightNode = ir_visit_ast_node(node->sons[1]);
    if (!leftNode || !rightNode) return false;

    Function *cur = module->getCurrentFunction();
    // 创建标签
    auto *L_rhs = new LabelInstruction(cur);   // 左假、右评估入口
    auto *L_true = new LabelInstruction(cur);  // 结果真时跳来
    auto *L_end = new LabelInstruction(cur);   // 表达式结束
    
    // 生成左操作数代码
    node->blockInsts.addInst(leftNode->blockInsts);
    Value *zero = module->newConstInt(0);
    auto *cmpL = new BinaryInstruction(
        cur,
        IRInstOperator::IRINST_OP_NE_I,
        leftNode->val,
        zero,
        IntegerType::getTypeBool());
    node->blockInsts.addInst(cmpL);

    // 短路：如果 left!=0（真），就 goto L_true；否则 goto L_rhs
    node->blockInsts.addInst(new GotoInstruction(cur, cmpL, L_true, L_rhs));

    // L_rhs: 计算右操作数
    node->blockInsts.addInst(L_rhs);
    node->blockInsts.addInst(rightNode->blockInsts);
    auto *cmpR = new BinaryInstruction(
        cur,
        IRInstOperator::IRINST_OP_NE_I,
        rightNode->val,
        zero,
        IntegerType::getTypeBool());
    node->blockInsts.addInst(cmpR);
    
    // 右操作数结果作为整个表达式的结果
    Value *result = convertBoolToInt(node, cmpR);
    node->blockInsts.addInst(new GotoInstruction(cur, L_end));

    // L_true: 左操作数为真，整个表达式为真
    node->blockInsts.addInst(L_true);
    node->blockInsts.addInst(new MoveInstruction(cur, result, module->newConstInt(1)));
    node->blockInsts.addInst(new GotoInstruction(cur, L_end));

    // L_end
    node->blockInsts.addInst(L_end);
    node->val = result;
    return true;
}

/// @brief 逻辑非AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_not(ast_node *node) {
    // 先递归生成子表达式的 IR
    ast_node *operandNode = ir_visit_ast_node(node->sons[0]);
    if (!operandNode) return false;

    Function *cur = module->getCurrentFunction();
    Value *zero = module->newConstInt(0);
    BinaryInstruction *eqInst = new BinaryInstruction(
        cur,
        IRInstOperator::IRINST_OP_EQ_I,
        operandNode->val,
        zero,
        IntegerType::getTypeBool());
    node->blockInsts.addInst(operandNode->blockInsts);
    node->blockInsts.addInst(eqInst);

    // bool → int 转换
    node->val = convertBoolToInt(node, eqInst);
    return true;
}

/// @brief 赋值AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_assign(ast_node *node) {
    ast_node *son1_node = node->sons[0];
    ast_node *son2_node = node->sons[1];

    // 赋值节点，自右往左运算

    // 赋值运算符的左侧操作数
    ast_node *left = ir_visit_ast_node(son1_node);
    if (!left) {
        // 某个变量没有定值
        // 这里缺省设置变量不存在则创建，因此这里不会错误
        return false;
    }

    // 赋值运算符的右侧操作数
    ast_node *right = ir_visit_ast_node(son2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 右侧数组访问现在默认返回值，不需要特殊处理

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    // 检查左侧是否是数组访问
    if (son1_node->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
        // 左侧是数组访问，需要使用保存的地址信息
        Value *arrayAddr = g_arrayAccessAddresses[left];

        StoreArrayInstruction *storeInst = new StoreArrayInstruction(
            module->getCurrentFunction(),
            right->val,  // 要存储的值
            arrayAddr,   // 数组元素地址
            module->newConstInt(0)  // 偏移为0，因为地址已经计算好了
        );

        // 创建临时变量保存IR的值，以及线性IR指令
        node->blockInsts.addInst(right->blockInsts);
        node->blockInsts.addInst(left->blockInsts);
        node->blockInsts.addInst(storeInst);

        node->val = storeInst;
    } else {
        // 普通变量赋值
        MoveInstruction *movInst = new MoveInstruction(module->getCurrentFunction(), left->val, right->val);

        // 创建临时变量保存IR的值，以及线性IR指令
        node->blockInsts.addInst(right->blockInsts);
        node->blockInsts.addInst(left->blockInsts);
        node->blockInsts.addInst(movInst);

        // 这里假定赋值的类型是一致的
        node->val = movInst;
    }

    return true;
}

/// @brief return节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_return(ast_node *node) {
    ast_node *right = nullptr;

    // return语句可能没有没有表达式，也可能有，因此这里必须进行区分判断
    if (!node->sons.empty()) {
        ast_node *son_node = node->sons[0];

        // 返回的表达式的指令保存在right节点中
        right = ir_visit_ast_node(son_node);
        if (!right) {
            // 某个变量没有定值
            return false;
        }
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    Function *currentFunc = module->getCurrentFunction();

    // node->blockInsts.addInst(new MoveInstruction(currentFunc, currentFunc->getReturnValue(), module->newConstInt(0)))

    // 返回值存在时则移动指令到node中
    if (right) {
        // 创建临时变量保存IR的值，以及线性IR指令
        node->blockInsts.addInst(right->blockInsts);

        // 返回值现在默认就是值，不需要特殊处理数组访问
        // 返回值赋值到函数返回值变量上，然后跳转到函数的尾部
        node->blockInsts.addInst(new MoveInstruction(currentFunc, currentFunc->getReturnValue(), right->val));

        node->val = right->val;
    } else {
        // 没有返回值
        node->val = nullptr;
    }

    // 跳转到函数的尾部出口指令上
    node->blockInsts.addInst(new GotoInstruction(currentFunc, currentFunc->getExitLabel()));

    return true;
}

bool IRGenerator::ir_if(ast_node *node) {
    // sons[0]=cond, sons[1]=thenStmt, optional sons[2]=elseStmt
    ast_node *condNode = ir_visit_ast_node(node->sons[0]);
    if (!condNode) return false;

    // then分支可能是空语句（nullptr）
    ast_node *thenNode = nullptr;
    if (node->sons[1] != nullptr) {
        thenNode = ir_visit_ast_node(node->sons[1]);
        if (!thenNode) return false;
    }

    bool hasElse = node->sons.size() > 2;
    ast_node *elseNode = nullptr;
    if (hasElse && node->sons[2] != nullptr) {
        elseNode = ir_visit_ast_node(node->sons[2]);
        if (!elseNode) return false;
    }

    Function *cur = module->getCurrentFunction();
    auto *L_then = new LabelInstruction(cur);
    auto *L_else = new LabelInstruction(cur);
    auto *L_end = new LabelInstruction(cur);

    // 1) 条件计算产生 cmp 指令
    node->blockInsts.addInst(condNode->blockInsts);

    // 2) 根据 cond 跳转到 then 或 else
    // 检查条件是否为常量
    if (ConstInt *constCond = dynamic_cast<ConstInt*>(condNode->val)) {
        // 常量条件优化
        if (constCond->getVal() != 0) {
            // if(非零常量) -> 无条件跳转到then分支
            node->blockInsts.addInst(new GotoInstruction(cur, L_then));
        } else {
            // if(0) -> 无条件跳转到else分支
            node->blockInsts.addInst(new GotoInstruction(cur, L_else));
        }
    } else {
        // 变量条件：生成条件跳转
        node->blockInsts.addInst(
            new GotoInstruction(cur,
                                condNode->val,
                                L_then,
                                L_else));
    }

    // 3) then 分支
    node->blockInsts.addInst(L_then);
    if (thenNode != nullptr) {
        node->blockInsts.addInst(thenNode->blockInsts);
    }
    // 然后无条件跳到 end
    node->blockInsts.addInst(new GotoInstruction(cur, L_end));

    // 4) else 分支
    node->blockInsts.addInst(L_else);
    if (hasElse && elseNode != nullptr) {
        node->blockInsts.addInst(elseNode->blockInsts);
    }
    // fall-through 到 end（不需要额外 goto）

    // 5) 结束标签
    node->blockInsts.addInst(L_end);
    return true;
}

bool IRGenerator::ir_while(ast_node *node) {
    Function *cur = module->getCurrentFunction();
    auto *L_cond = new LabelInstruction(cur);
    auto *L_body = new LabelInstruction(cur);
    auto *L_end = new LabelInstruction(cur);

    loopCondStack.push_back(L_cond);
    loopEndStack.push_back(L_end);

    ast_node *condNode = ir_visit_ast_node(node->sons[0]);
    if (!condNode) return false;

    // while循环体可能是空语句（nullptr）
    ast_node *bodyNode = nullptr;
    if (node->sons[1] != nullptr) {
        bodyNode = ir_visit_ast_node(node->sons[1]);
        if (!bodyNode) return false;
    }

    // 循环入口
    node->blockInsts.addInst(L_cond);
    node->blockInsts.addInst(condNode->blockInsts);

    // 检查条件是否为常量
    if (ConstInt *constCond = dynamic_cast<ConstInt*>(condNode->val)) {
        // 常量条件优化
        if (constCond->getVal() != 0) {
            // while(非零常量) -> 无条件跳转到循环体（无限循环）
            node->blockInsts.addInst(new GotoInstruction(cur, L_body));
        } else {
            // while(0) -> 直接跳转到结束（不执行循环体）
            node->blockInsts.addInst(new GotoInstruction(cur, L_end));
        }
    } else {
        // 变量条件：生成条件跳转
        node->blockInsts.addInst(new GotoInstruction(cur, condNode->val, L_body, L_end));
    }

    // 循环体
    node->blockInsts.addInst(L_body);
    if (bodyNode != nullptr) {
        node->blockInsts.addInst(bodyNode->blockInsts);
    }
    node->blockInsts.addInst(new GotoInstruction(cur, L_cond));

    // 循环结束
    node->blockInsts.addInst(L_end);

    loopCondStack.pop_back();
    loopEndStack.pop_back();
    return true;
}

bool IRGenerator::ir_break(ast_node *node) {
    Function *cur = module->getCurrentFunction();
    if (loopEndStack.empty()) {
        // error: break outside loop
        return false;
    }
    LabelInstruction *L_end = loopEndStack.back();
    node->blockInsts.addInst(new GotoInstruction(cur, L_end));
    return true;
}

bool IRGenerator::ir_continue(ast_node *node) {
    Function *cur = module->getCurrentFunction();
    if (loopCondStack.empty()) {
        // error: continue outside loop
        return false;
    }
    LabelInstruction *L_cond = loopCondStack.back();
    node->blockInsts.addInst(new GotoInstruction(cur, L_cond));
    return true;
}

/// @brief 类型叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_type(ast_node *node) {
    // 不需要做什么，直接从节点中获取即可。

    return true;
}

/// @brief 标识符叶子节点翻译成线性中间IR，变量声明的不走这个语句
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_var_id(ast_node *node) {
    Value *val;

    // 查找ID型Value
    // 变量，则需要在符号表中查找对应的值

    val = module->findVarValue(node->name);

    node->val = val;

    return true;
}

/// @brief 无符号整数字面量叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_uint(ast_node *node) {
    ConstInt *val;

    // 新建一个整数常量Value
    val = module->newConstInt((int32_t)node->integer_val);

    node->val = val;

    return true;
}

/// @brief 变量声明语句节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_declare_statment(ast_node * node)
{
    bool result = true;
    for (auto * child: node->sons) {
        // child 是 AST_OP_VAR_DECL
        result = ir_variable_declare(child);
        if (!result)
            break;
        // 串联每个声明的 IR
        node->blockInsts.addInst(child->blockInsts);
    }
    return result;
}

/// @brief 获取数组维度信息
/// @param node 父节点，用于添加指令
/// @param dimsNode 维度节点列表
/// @param dimValues 输出：维度值列表
/// @param dimConstants 输出：维度常量列表
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_get_array_dimensions(ast_node *node, ast_node *dimsNode,
                                        std::vector<Value *> &dimValues,
                                        std::vector<int32_t> &dimConstants) {
    if (!dimsNode) {
        return false;
    }

    for (auto dimChild : dimsNode->sons) {
        ast_node *dimNode = ir_visit_ast_node(dimChild);
        if (!dimNode) {
            return false;
        }

        dimValues.push_back(dimNode->val);
        node->blockInsts.addInst(dimNode->blockInsts);

        // 尝试获取常量维度值
        if (ConstInt *constDim = dynamic_cast<ConstInt*>(dimNode->val)) {
            dimConstants.push_back(constDim->getVal());
        } else {
            // 非常量维度，使用默认值10
            dimConstants.push_back(10);
        }
    }
    return true;
}

/// @brief 处理普通变量定义
/// @param node 变量声明节点
/// @param defNode 变量定义节点
/// @param ty 变量类型
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_simple_var_def(ast_node *node, ast_node *defNode, Type *ty) {
    Function *curF = module->getCurrentFunction();

    // AST_OP_VAR_DEF：子孙[0]=id, 子孙[1]=initExpr 或 nullptr
    const std::string &name = defNode->sons[0]->name;
    Value *var = module->newVarValue(ty, name);
    node->val = var;

    // 如果有初值表达式
    ast_node *initExpr = defNode->sons.size() > 1 ? defNode->sons[1] : nullptr;
    if (initExpr) {
        // 生成初值表达式的 IR
        ast_node *initNode = ir_visit_ast_node(initExpr);
        if (!initNode)
            return false;
        node->blockInsts.addInst(initNode->blockInsts);

        // 判断是全局变量还是局部变量
        if (curF) {
            // 局部变量：生成赋值指令
            node->blockInsts.addInst(new MoveInstruction(curF, static_cast<LocalVariable *>(var), initNode->val));
        } else {
            // 全局变量：设置初始化值
            GlobalVariable *globalVar = static_cast<GlobalVariable *>(var);
            globalVar->setInitializer(initNode->val);
        }
    }

    return true;
}

/// @brief 处理数组变量定义
/// @param node 变量声明节点
/// @param defNode 数组定义节点
/// @param ty 元素类型
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_array_var_def(ast_node *node, ast_node *defNode, Type *ty) {
    // AST_OP_ARRAY_DEF：子孙[0]=id, 子孙[1]=维度表达式列表, 子孙[2]=初值列表（可选）
    std::string &name = defNode->sons[0]->name;

    // 处理数组维度
    ast_node *dimsNode = defNode->sons[1];
    std::vector<Value *> dimSizes;
    std::vector<int32_t> dimConstants;

    if (!ir_get_array_dimensions(node, dimsNode, dimSizes, dimConstants)) {
        return false;
    }

    // 创建数组类型
    ArrayType *arrayType = new ArrayType(ty, dimConstants);

    // 创建数组变量（可能是局部变量或全局变量）
    Value *arrayVar = module->newVarValue(arrayType, name);
    if (!arrayVar) {
        return false;
    }

    // 保存维度信息到全局映射中，用于数组访问时的降维计算
    static std::map<std::string, std::vector<Value *>> arrayDimensions;
    if (LocalVariable *localVar = dynamic_cast<LocalVariable*>(arrayVar)) {
        arrayDimensions[localVar->getName()] = dimSizes;
    } else if (GlobalVariable *globalVar = dynamic_cast<GlobalVariable*>(arrayVar)) {
        arrayDimensions[globalVar->getName()] = dimSizes;
    }

    // 对于数组变量，node->val直接指向数组变量本身
    node->val = arrayVar;

    return true;
}

/// @brief 变量定声明节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_variable_declare(ast_node *node) {
    // node->sons[0] = 类型节点
    // node->sons[1] = AST_OP_VAR_DEF 或 AST_OP_ARRAY_DEF

    // 获取类型和定义节点
    Type *ty = node->sons[0]->type;
    ast_node *defNode = node->sons[1];

    // 根据定义节点类型进行不同处理
    if (defNode->node_type == ast_operator_type::AST_OP_VAR_DEF) {
        // 处理普通变量定义
        return ir_simple_var_def(node, defNode, ty);
    }
    else if (defNode->node_type == ast_operator_type::AST_OP_ARRAY_DEF) {
        // 处理数组定义
        return ir_array_var_def(node, defNode, ty);
    }
    else {
        // 未知的定义节点类型
        return false;
    }
}

/// @brief 计算数组访问的线性偏移
/// @param node 父节点，用于添加指令
/// @param indices 各维度索引值
/// @param fullDimensions 数组的完整维度信息
/// @param accessedDims 访问的维度数量
/// @param isFullAccess 是否为完整访问
/// @param remainingDims 剩余维度信息
/// @return 线性偏移值
Value* IRGenerator::ir_calculate_array_offset(ast_node *node, const std::vector<Value *> &indices,
                                             const std::vector<int32_t> &fullDimensions,
                                             size_t accessedDims, bool isFullAccess,
                                             const std::vector<int32_t> &remainingDims) {
    Function *curF = module->getCurrentFunction();
    Value *linearOffset = nullptr;

    if (accessedDims == 0) {
        // 没有索引，直接返回数组本身
        linearOffset = module->newConstInt(0);
    } else {
        linearOffset = indices[0];

        // 对于每个访问的维度，计算其对线性偏移的贡献
        for (size_t i = 1; i < accessedDims; ++i) {
            Value *multiplierVal = module->newConstInt(fullDimensions[i]);

            // linearOffset = linearOffset * multiplier + indices[i]
            BinaryInstruction *mulInst = new BinaryInstruction(
                curF,
                IRInstOperator::IRINST_OP_MUL_I,
                linearOffset,
                multiplierVal,
                IntegerType::getTypeInt());
            node->blockInsts.addInst(mulInst);

            BinaryInstruction *addInst = new BinaryInstruction(
                curF,
                IRInstOperator::IRINST_OP_ADD_I,
                mulInst,
                indices[i],
                IntegerType::getTypeInt());
            node->blockInsts.addInst(addInst);

            linearOffset = addInst;
        }

        // 如果是部分访问，还需要乘以剩余维度的总大小
        if (!isFullAccess) {
            int32_t remainingSize = 1;
            for (int32_t dim : remainingDims) {
                remainingSize *= dim;
            }

            if (remainingSize > 1) {
                Value *remainingSizeVal = module->newConstInt(remainingSize);
                BinaryInstruction *mulInst = new BinaryInstruction(
                    curF,
                    IRInstOperator::IRINST_OP_MUL_I,
                    linearOffset,
                    remainingSizeVal,
                    IntegerType::getTypeInt());
                node->blockInsts.addInst(mulInst);
                linearOffset = mulInst;
            }
        }
    }

    return linearOffset;
}

/// @brief 生成数组访问指令
/// @param node 父节点，用于添加指令
/// @param arrayNameNode 数组名节点
/// @param linearOffset 线性偏移值
/// @param elementType 元素类型
/// @param isFullAccess 是否为完整访问
/// @param remainingDims 剩余维度信息
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_generate_array_access_instructions(ast_node *node, ast_node *arrayNameNode,
                                                       Value *linearOffset, Type *elementType,
                                                       bool isFullAccess, const std::vector<int32_t> &remainingDims) {
    Function *curF = module->getCurrentFunction();

    // 计算字节偏移
    Value *elementSize = module->newConstInt(4);  // sizeof(int) = 4
    BinaryInstruction *byteOffsetInst = new BinaryInstruction(
        curF,
        IRInstOperator::IRINST_OP_MUL_I,
        linearOffset,
        elementSize,
        IntegerType::getTypeInt());
    node->blockInsts.addInst(byteOffsetInst);

    // 计算最终地址
    PointerType *ptrType = const_cast<PointerType*>(PointerType::get(elementType));
    BinaryInstruction *addrInst = new BinaryInstruction(
        curF,
        IRInstOperator::IRINST_OP_ADD_I,
        arrayNameNode->val,  // 数组变量
        byteOffsetInst,      // 字节偏移
        ptrType);            // 指针类型
    node->blockInsts.addInst(addrInst);

    if (isFullAccess) {
        // 完整访问：返回标量值
        LoadArrayInstruction *loadInst = new LoadArrayInstruction(
            curF,
            addrInst,            // 数组元素地址
            module->newConstInt(0),  // 偏移为0，因为地址已经计算好了
            elementType
        );
        node->blockInsts.addInst(loadInst);
        node->val = loadInst;

        // 保存地址信息，用于左值访问
        g_arrayAccessAddresses[node] = addrInst;
    } else {
        // 部分访问：返回数组切片
        // 创建剩余维度的数组类型
        ArrayType *sliceType = new ArrayType(elementType, remainingDims);

        // 创建数组切片指令，它的类型是数组类型，但包含的是地址
        ArraySliceInstruction *sliceInst = new ArraySliceInstruction(curF, addrInst, sliceType);
        node->blockInsts.addInst(sliceInst);

        node->val = sliceInst;

        // 不保存到g_arrayAccessAddresses，因为这不是左值访问
    }

    return true;
}

/// @brief 数组访问AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_array_access(ast_node *node) {
    // node->sons[0] = 数组名节点
    // node->sons[1] = array-dims节点，包含各维度索引

    // 获取数组变量
    ast_node *arrayNameNode = ir_visit_ast_node(node->sons[0]);
    if (!arrayNameNode) {
        return false;
    }

    ast_node *indicesNode = node->sons[1];

    // 处理各维度索引
    std::vector<Value *> indices;
    std::vector<int32_t> indexConstants;
    if (!ir_get_array_dimensions(node, indicesNode, indices, indexConstants)) {
        return false;
    }

    node->blockInsts.addInst(arrayNameNode->blockInsts);

    // 获取数组的完整维度信息
    std::vector<int32_t> fullDimensions;
    Type *elementType = IntegerType::getTypeInt();

    if (ArrayType *arrayType = dynamic_cast<ArrayType*>(arrayNameNode->val->getType())) {
        fullDimensions = arrayType->getDimensions();
        elementType = arrayType->getElementType();
    } else {
        // 如果不是数组类型，可能是错误
        return false;
    }

    size_t accessedDims = indices.size();
    size_t totalDims = fullDimensions.size();

    // 判断是完整访问还是部分访问
    bool isFullAccess = (accessedDims == totalDims);

    // 计算剩余维度（用于部分访问）
    std::vector<int32_t> remainingDims;
    if (!isFullAccess) {
        for (size_t i = accessedDims; i < totalDims; ++i) {
            remainingDims.push_back(fullDimensions[i]);
        }
    }

    // 计算线性偏移
    Value *linearOffset = ir_calculate_array_offset(node, indices, fullDimensions,
                                                  accessedDims, isFullAccess, remainingDims);
    if (!linearOffset) {
        return false;
    }

    // 生成数组访问指令
    return ir_generate_array_access_instructions(node, arrayNameNode, linearOffset,
                                               elementType, isFullAccess, remainingDims);
}
