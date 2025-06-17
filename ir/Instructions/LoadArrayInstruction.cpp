///
/// @file LoadArrayInstruction.cpp
/// @brief 数组元素加载指令
///

#include "LoadArrayInstruction.h"
#include "Function.h"

/// @brief 数组元素加载指令构造函数
/// @param _func 所属的函数
/// @param _arrayBase 数组基址
/// @param _offset 偏移量（以元素为单位）
/// @param _elementType 数组元素类型
LoadArrayInstruction::LoadArrayInstruction(Function * _func, Value * _arrayBase, Value * _offset, Type * _elementType)
    : Instruction(_func, IRInstOperator::IRINST_OP_LOAD_ARRAY, _elementType)
{
    this->addOperand(_arrayBase);
    this->addOperand(_offset);
}

/// @brief 转换成字符串
void LoadArrayInstruction::toString(std::string & str)
{
    Value * arrayBase = getOperand(0);

    // 生成类似 %l4 = *%t8 的格式
    // 注意：这里不生成 declare 语句，因为 Function::toString 会自动处理
    str = getIRName() + " = *" + arrayBase->getIRName();
}

/// @brief 获取数组基址
Value * LoadArrayInstruction::getArrayBase() const
{
    return const_cast<LoadArrayInstruction*>(this)->getOperand(0);
}

/// @brief 获取偏移量
Value * LoadArrayInstruction::getOffset() const
{
    return const_cast<LoadArrayInstruction*>(this)->getOperand(1);
}
