///
/// @file StoreArrayInstruction.h
/// @brief 数组元素存储指令
///
#pragma once

#include <string>

#include "Value.h"
#include "Instruction.h"
#include "VoidType.h"

class Function;

///
/// @brief 数组元素存储指令，将寄存器值存储到数组元素
///
class StoreArrayInstruction : public Instruction {

public:
    ///
    /// @brief 构造函数
    /// @param _func 所属的函数
    /// @param _value 要存储的值
    /// @param _arrayBase 数组基址
    /// @param _offset 偏移量（以元素为单位）
    ///
    StoreArrayInstruction(Function * _func, Value * _value, Value * _arrayBase, Value * _offset);

    /// @brief 转换成字符串
    void toString(std::string & str) override;

    /// @brief 获取要存储的值
    Value * getValue() const;

    /// @brief 获取数组基址
    Value * getArrayBase() const;

    /// @brief 获取偏移量
    Value * getOffset() const;
};
