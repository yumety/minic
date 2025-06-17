///
/// @file LoadArrayInstruction.h
/// @brief 数组元素加载指令
///
#pragma once

#include <string>

#include "Value.h"
#include "Instruction.h"

class Function;

///
/// @brief 数组元素加载指令，从内存加载数组元素到寄存器
///
class LoadArrayInstruction : public Instruction {

public:
    ///
    /// @brief 构造函数
    /// @param _func 所属的函数
    /// @param _arrayBase 数组基址
    /// @param _offset 偏移量（以元素为单位）
    /// @param _elementType 数组元素类型
    ///
    LoadArrayInstruction(Function * _func, Value * _arrayBase, Value * _offset, Type * _elementType);

    /// @brief 转换成字符串
    void toString(std::string & str) override;

    /// @brief 获取数组基址
    Value * getArrayBase() const;

    /// @brief 获取偏移量
    Value * getOffset() const;
};
