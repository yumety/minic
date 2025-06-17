///
/// @file GotoInstruction.h
/// @brief 无条件跳转指令即goto指令
///
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-29
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// </table>
///
#pragma once

#include <string>

#include "Instruction.h"
#include "LabelInstruction.h"
#include "Function.h"

///
/// @brief 无条件跳转指令
///
class GotoInstruction final : public Instruction {

public:
    ///
    /// @brief 无条件跳转指令的构造函数
    /// @param target 跳转目标
    ///
    GotoInstruction(Function * _func, Instruction * _target);

	///
    /// @brief 有条件跳转指令的构造函数
    /// @param _func 当前函数
    /// @param _cond 条件值（i1）
    /// @param _trueTarget 条件为真时跳转目标
    /// @param _falseTarget 条件为假时跳转目标
    ///
    GotoInstruction(Function * _func,
		Value    * _cond,
		Instruction * _trueTarget,
		Instruction * _falseTarget);

    /// @brief 转换成字符串
    void toString(std::string & str) override;

    ///
    /// @brief 获取目标Label指令
    /// @return LabelInstruction*
    ///
    // [[nodiscard]] LabelInstruction * getTarget() const;
	
	/// @brief 获取真跳转目标 Label
    [[nodiscard]] LabelInstruction * getTrueTarget()  const;
    /// @brief 获取假跳转目标 Label（无条件时为 nullptr）
    [[nodiscard]] LabelInstruction * getFalseTarget() const;
    /// @brief 获取条件值（无条件时为 nullptr）
    [[nodiscard]] Value  * getCond()  const;
private:
    // ///
    // /// @brief 跳转到的目标Label指令
    // ///
    // LabelInstruction * target;

	LabelInstruction * trueTarget;   ///< 真分支目标
    LabelInstruction * falseTarget;  ///< 假分支目标
    Value * cond;         ///< 条件值
};
