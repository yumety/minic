///
/// @file IRGenerator.h
/// @brief AST遍历产生线性IR的头文件
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

#include <unordered_map>

#include "AST.h"
#include "Module.h"
#include "LabelInstruction.h"

/// @brief AST遍历产生线性IR类
class IRGenerator {

public:
    /// @brief 构造函数
    /// @param root
    /// @param _module
    IRGenerator(ast_node * root, Module * _module);

    /// @brief 析构函数
    ~IRGenerator() = default;

    /// @brief 运行产生IR
    bool run();

protected:
    /// @brief 编译单元AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_compile_unit(ast_node * node);

    /// @brief 函数定义AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_function_define(ast_node * node);

    /// @brief 形式参数AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_function_formal_params(ast_node * node);

    /// @brief 函数调用AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_function_call(ast_node * node);

    /// @brief 语句块（含函数体）AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_block(ast_node * node);

    /// @brief 整数加法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_add(ast_node * node);

	/// @brief 整数减法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_sub(ast_node * node);

    /// @brief 整数乘法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_mul(ast_node * node);

    /// @brief 整数除法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_div(ast_node * node);

    /// @brief 整数取余AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_mod(ast_node * node);

    /// @brief 单目负号AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_neg(ast_node * node);

    /// @brief 小于比较AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_lt(ast_node * node);

    /// @brief 大于比较AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_gt(ast_node * node);

    /// @brief 小于等于比较AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_le(ast_node * node);

    /// @brief 大于等于比较AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_ge(ast_node * node);

    /// @brief 等于比较AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_eq(ast_node * node);

    /// @brief 不等于比较AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_ne(ast_node * node);

    /// @brief 逻辑与AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_and(ast_node * node);

    /// @brief 逻辑或AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_or(ast_node * node);

    /// @brief 逻辑非AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_not(ast_node * node);

    /// @brief 将i1类型的布尔值转换为i32类型的整数值
    /// @param node AST节点
    /// @param boolValue i1类型的布尔值
    /// @return i32类型的整数值
    Value* convertBoolToInt(ast_node *node, Value *boolValue);

    /// @brief 赋值AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_assign(ast_node * node);

    /// @brief return节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_return(ast_node * node);

    /// @brief if节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_if(ast_node * node);

    /// @brief while节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_while(ast_node * node);

    /// @brief break节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_break(ast_node * node);

    /// @brief continue节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_continue(ast_node * node);

    /// @brief 类型叶子节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_leaf_node_type(ast_node * node);

    /// @brief 标识符叶子节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_leaf_node_var_id(ast_node * node);

    /// @brief 无符号整数字面量叶子节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_leaf_node_uint(ast_node * node);

    /// @brief 变量声明语句节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_declare_statment(ast_node * node);

    /// @brief 变量定声明节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_variable_declare(ast_node * node);

    /// @brief 处理普通变量定义
    /// @param node 变量声明节点
    /// @param defNode 变量定义节点
    /// @param ty 变量类型
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_simple_var_def(ast_node *node, ast_node *defNode, Type *ty);

    /// @brief 处理数组变量定义
    /// @param node 变量声明节点
    /// @param defNode 数组定义节点
    /// @param ty 元素类型
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_array_var_def(ast_node *node, ast_node *defNode, Type *ty);

    /// @brief 获取数组维度信息（公共函数）
    /// @param node 父节点，用于添加指令
    /// @param dimsNode 维度节点列表
    /// @param dimValues 输出：维度值列表
    /// @param dimConstants 输出：维度常量列表
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_get_array_dimensions(ast_node *node, ast_node *dimsNode,
                                std::vector<Value *> &dimValues,
                                std::vector<int32_t> &dimConstants);

    /// @brief 计算数组访问的线性偏移
    /// @param node 父节点，用于添加指令
    /// @param indices 各维度索引值
    /// @param fullDimensions 数组的完整维度信息
    /// @param accessedDims 访问的维度数量
    /// @param isFullAccess 是否为完整访问
    /// @param remainingDims 剩余维度信息
    /// @return 线性偏移值
    Value* ir_calculate_array_offset(ast_node *node, const std::vector<Value *> &indices,
                                   const std::vector<int32_t> &fullDimensions,
                                   size_t accessedDims, bool isFullAccess,
                                   const std::vector<int32_t> &remainingDims);

    /// @brief 生成数组访问指令
    /// @param node 父节点，用于添加指令
    /// @param arrayNameNode 数组名节点
    /// @param linearOffset 线性偏移值
    /// @param elementType 元素类型
    /// @param isFullAccess 是否为完整访问
    /// @param remainingDims 剩余维度信息
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_generate_array_access_instructions(ast_node *node, ast_node *arrayNameNode,
                                              Value *linearOffset, Type *elementType,
                                              bool isFullAccess, const std::vector<int32_t> &remainingDims);

    /// @brief 数组访问AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_array_access(ast_node * node);

    /// @brief 未知节点类型的节点处理
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_default(ast_node * node);

    /// @brief 根据AST的节点运算符查找对应的翻译函数并执行翻译动作
    /// @param node AST节点
    /// @return 成功返回node节点，否则返回nullptr
    ast_node * ir_visit_ast_node(ast_node * node);

    /// @brief AST的节点操作函数
    typedef bool (IRGenerator::*ast2ir_handler_t)(ast_node *);

    /// @brief AST节点运算符与动作函数关联的映射表
    std::unordered_map<ast_operator_type, ast2ir_handler_t> ast2ir_handlers;

private:
    /// @brief 抽象语法树的根
    ast_node * root;

    /// @brief 符号表:模块
    Module * module;

    /// @brief 循环条件标签栈
	std::vector<LabelInstruction *> loopCondStack;

    /// @brief 循环结束标签栈
    std::vector<LabelInstruction *> loopEndStack;
};
