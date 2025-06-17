///
/// @file ArraySliceInstruction.h
/// @brief 数组切片指令类
///

#pragma once

#include "Instruction.h"
#include "ArrayType.h"

///
/// @brief 数组切片指令，用于表示数组的部分访问
/// 这个指令的类型是数组类型，但实际上包含的是指针地址
///
class ArraySliceInstruction : public Instruction {

public:
    ///
    /// @brief 构造函数
    /// @param _func 所属函数
    /// @param _address 数组切片的地址
    /// @param _arrayType 数组切片的类型
    ///
    ArraySliceInstruction(Function * _func, Value * _address, ArrayType * _arrayType);

    ///
    /// @brief 转换成字符串显示
    /// @param str 转换后的字符串
    ///
    void toString(std::string & str) override;

    ///
    /// @brief 获取数组切片的地址
    /// @return 地址值
    ///
    Value * getAddress() const;

    ///
    /// @brief 重写getIRName方法，返回地址的名称
    /// @return IR名称
    ///
    std::string getIRName() const override;
};
