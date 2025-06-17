///
/// @file StoreArrayInstruction.cpp
/// @brief 数组元素存储指令
///

#include "StoreArrayInstruction.h"
#include "Function.h"

/// @brief 数组元素存储指令构造函数
/// @param _func 所属的函数
/// @param _value 要存储的值
/// @param _arrayBase 数组基址
/// @param _offset 偏移量（以元素为单位）
StoreArrayInstruction::StoreArrayInstruction(Function * _func, Value * _value, Value * _arrayBase, Value * _offset)
    : Instruction(_func, IRInstOperator::IRINST_OP_STORE_ARRAY, VoidType::getType())
{
    this->addOperand(_value);
    this->addOperand(_arrayBase);
    this->addOperand(_offset);
}

/// @brief 转换成字符串
void StoreArrayInstruction::toString(std::string & str)
{
    Value * value = getOperand(0);
    Value * arrayBase = getOperand(1);

    // 生成类似 *%t8 = %l4 的格式
    str = "*" + arrayBase->getIRName() + " = " + value->getIRName();
}

/// @brief 获取要存储的值
Value * StoreArrayInstruction::getValue() const
{
    return const_cast<StoreArrayInstruction*>(this)->getOperand(0);
}

/// @brief 获取数组基址
Value * StoreArrayInstruction::getArrayBase() const
{
    return const_cast<StoreArrayInstruction*>(this)->getOperand(1);
}

/// @brief 获取偏移量
Value * StoreArrayInstruction::getOffset() const
{
    return const_cast<StoreArrayInstruction*>(this)->getOperand(2);
}
