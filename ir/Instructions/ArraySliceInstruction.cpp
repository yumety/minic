///
/// @file ArraySliceInstruction.cpp
/// @brief 数组切片指令类实现
///

#include "ArraySliceInstruction.h"
#include "Function.h"

/// @brief 构造函数
/// @param _func 所属函数
/// @param _address 数组切片的地址
/// @param _arrayType 数组切片的类型
ArraySliceInstruction::ArraySliceInstruction(Function * _func, Value * _address, ArrayType * _arrayType)
    : Instruction(_func, IRInstOperator::IRINST_OP_ASSIGN, _arrayType)
{
    this->addOperand(_address);
}

/// @brief 转换成字符串显示
/// @param str 转换后的字符串
void ArraySliceInstruction::toString(std::string & str)
{
    // 不生成任何IR指令，这个指令只是用来携带类型信息
    // 在函数调用时会使用这个类型信息
    str = "";
}

/// @brief 获取数组切片的地址
/// @return 地址值
Value * ArraySliceInstruction::getAddress() const
{
    return const_cast<ArraySliceInstruction*>(this)->getOperand(0);
}

/// @brief 重写getIRName方法，返回地址的名称
/// @return IR名称
std::string ArraySliceInstruction::getIRName() const
{
    // 直接返回地址的名称，因为我们实际上就是要使用这个地址
    return const_cast<ArraySliceInstruction*>(this)->getOperand(0)->getIRName();
}
