///
/// @file ArrayType.h
/// @brief 数组类型描述类
///
#pragma once

#include "Type.h"
#include "Value.h"
#include <vector>

class ArrayType : public Type {

public:
    ///
    /// @brief 数组类型构造函数
    /// @param _elementType 数组元素类型
    /// @param _dimensions 各维度大小
    ///
    ArrayType(Type * _elementType, std::vector<int32_t> _dimensions) 
        : Type(Type::ArrayTyID), elementType(_elementType), dimensions(std::move(_dimensions))
    {}

    ///
    /// @brief 获取类型的IR标识符，包含维度信息
    /// @return std::string IR标识符，如 i32[10][10]
    ///
    [[nodiscard]] std::string toString() const override
    {
        std::string typeStr = elementType->toString();
        for (int32_t dim : dimensions) {
            typeStr += "[" + std::to_string(dim) + "]";
        }
        return typeStr;
    }

    ///
    /// @brief 获取元素类型
    /// @return Type* 元素类型
    ///
    [[nodiscard]] Type * getElementType() const
    {
        return elementType;
    }

    ///
    /// @brief 获取维度信息
    /// @return const std::vector<int32_t>& 维度大小列表
    ///
    [[nodiscard]] const std::vector<int32_t>& getDimensions() const
    {
        return dimensions;
    }

    ///
    /// @brief 获得类型所占内存空间大小
    /// @return int32_t 总字节数
    ///
    [[nodiscard]] int32_t getSize() const override
    {
        int32_t totalSize = elementType->getSize();
        for (int32_t dim : dimensions) {
            totalSize *= dim;
        }
        return totalSize;
    }

private:
    ///
    /// @brief 数组元素类型
    ///
    Type * elementType;

    ///
    /// @brief 各维度大小
    ///
    std::vector<int32_t> dimensions;
};
