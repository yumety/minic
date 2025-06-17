///
/// @file GotoInstruction.cpp
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

#include "VoidType.h"

#include "GotoInstruction.h"

///
/// @brief 无条件跳转指令的构造函数
/// @param target 跳转目标
///
GotoInstruction::GotoInstruction(Function * _func, Instruction * _target)
    : Instruction(_func, IRInstOperator::IRINST_OP_GOTO, VoidType::getType())
{
    // 真假目标一样，则无条件跳转
    trueTarget = static_cast<LabelInstruction *>(_target);
	falseTarget = nullptr;
	cond = nullptr;
}

/// 有条件跳转构造
GotoInstruction::GotoInstruction(Function * _func,
	Value    * _cond,
	Instruction * _trueTarget,
	Instruction * _falseTarget)
	: Instruction(_func, IRInstOperator::IRINST_OP_GOTO, VoidType::getType())
{
	trueTarget = static_cast<LabelInstruction*>(_trueTarget);
	falseTarget = static_cast<LabelInstruction*>(_falseTarget);
	cond = _cond;
}

/// @brief 转换成IR指令文本
void GotoInstruction::toString(std::string & str)
{
    // str = "br label " + target->getIRName();
	if (!cond) {
        // br label .Lx
        str = "br label " + trueTarget->getIRName();
    } else {
        // bc %cond, label .Ltrue, label .Lfalse
        str = "bc "
            + cond->getIRName()
            + ", label "  + trueTarget->getIRName()
            + ", label "  + falseTarget->getIRName();
    }
}

///
/// @brief 获取目标Label指令
/// @return LabelInstruction* label指令
///
// LabelInstruction * GotoInstruction::getTarget() const
// {
//     return target;
// }
LabelInstruction * GotoInstruction::getTrueTarget() const {
    return trueTarget;
}
LabelInstruction * GotoInstruction::getFalseTarget() const {
    return falseTarget;
}
Value * GotoInstruction::getCond() const {
    return cond;
}