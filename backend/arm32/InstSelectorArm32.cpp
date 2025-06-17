///
/// @file InstSelectorArm32.cpp
/// @brief 指令选择器-ARM32的实现
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-11-21 <td>1.0     <td>zenglj  <td>新做
/// </table>
///
#include <cstdio>

#include "Common.h"
#include "ILocArm32.h"
#include "InstSelectorArm32.h"
#include "PlatformArm32.h"

#include "PointerType.h"
#include "RegVariable.h"
#include "Function.h"

#include "LabelInstruction.h"
#include "GotoInstruction.h"
#include "FuncCallInstruction.h"
#include "MoveInstruction.h"

/// @brief 构造函数
/// @param _irCode 指令
/// @param _iloc ILoc
/// @param _func 函数
InstSelectorArm32::InstSelectorArm32(vector<Instruction *> & _irCode,
                                     ILocArm32 & _iloc,
                                     Function * _func,
                                     SimpleRegisterAllocator & allocator)
    : ir(_irCode), iloc(_iloc), func(_func), simpleRegisterAllocator(allocator)
{
    translator_handlers[IRInstOperator::IRINST_OP_ENTRY] = &InstSelectorArm32::translate_entry;
    translator_handlers[IRInstOperator::IRINST_OP_EXIT] = &InstSelectorArm32::translate_exit;

    translator_handlers[IRInstOperator::IRINST_OP_LABEL] = &InstSelectorArm32::translate_label;
    // translator_handlers[IRInstOperator::IRINST_OP_GOTO] = &InstSelectorArm32::translate_goto;
	translator_handlers[IRInstOperator::IRINST_OP_GOTO] = &InstSelectorArm32::translate_br;

    translator_handlers[IRInstOperator::IRINST_OP_ASSIGN] = &InstSelectorArm32::translate_assign;

    translator_handlers[IRInstOperator::IRINST_OP_ADD_I] = &InstSelectorArm32::translate_add_int32;
    translator_handlers[IRInstOperator::IRINST_OP_SUB_I] = &InstSelectorArm32::translate_sub_int32;
	translator_handlers[IRInstOperator::IRINST_OP_MUL_I] = &InstSelectorArm32::translate_mul_int32;
	translator_handlers[IRInstOperator::IRINST_OP_DIV_I] = &InstSelectorArm32::translate_div_int32;
	translator_handlers[IRInstOperator::IRINST_OP_MOD_I] = &InstSelectorArm32::translate_mod_int32;
	translator_handlers[IRInstOperator::IRINST_OP_LT_I] = &InstSelectorArm32::translate_lt_int32;
	translator_handlers[IRInstOperator::IRINST_OP_GT_I] = &InstSelectorArm32::translate_gt_int32;
	translator_handlers[IRInstOperator::IRINST_OP_LE_I] = &InstSelectorArm32::translate_le_int32;
	translator_handlers[IRInstOperator::IRINST_OP_GE_I] = &InstSelectorArm32::translate_ge_int32;
	translator_handlers[IRInstOperator::IRINST_OP_EQ_I] = &InstSelectorArm32::translate_eq_int32;
	translator_handlers[IRInstOperator::IRINST_OP_NE_I] = &InstSelectorArm32::translate_ne_int32;

    translator_handlers[IRInstOperator::IRINST_OP_FUNC_CALL] = &InstSelectorArm32::translate_call;
    translator_handlers[IRInstOperator::IRINST_OP_ARG] = &InstSelectorArm32::translate_arg;
}

///
/// @brief 析构函数
///
InstSelectorArm32::~InstSelectorArm32()
{}

/// @brief 指令选择执行
void InstSelectorArm32::run()
{
    for (auto inst: ir) {

        // 逐个指令进行翻译
        if (!inst->isDead()) {
            translate(inst);
        }
    }
}

/// @brief 指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate(Instruction * inst)
{
    // 操作符
    IRInstOperator op = inst->getOp();

    map<IRInstOperator, translate_handler>::const_iterator pIter;
    pIter = translator_handlers.find(op);
    if (pIter == translator_handlers.end()) {
        // 没有找到，则说明当前不支持
        printf("Translate: Operator(%d) not support", (int) op);
        return;
    }

    // 开启时输出IR指令作为注释
    if (showLinearIR) {
        outputIRInstruction(inst);
    }

    (this->*(pIter->second))(inst);
}

///
/// @brief 输出IR指令
///
void InstSelectorArm32::outputIRInstruction(Instruction * inst)
{
    std::string irStr;
    inst->toString(irStr);
    if (!irStr.empty()) {
        iloc.comment(irStr);
    }
}

/// @brief NOP翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_nop(Instruction * inst)
{
    (void) inst;
    iloc.nop();
}

/// @brief Label指令指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_label(Instruction * inst)
{
    Instanceof(labelInst, LabelInstruction *, inst);

    iloc.label(labelInst->getName());
}

// /// @brief goto指令指令翻译成ARM32汇编
// /// @param inst IR指令
// void InstSelectorArm32::translate_goto(Instruction * inst)
// {
//     Instanceof(gotoInst, GotoInstruction *, inst);

//     // 无条件跳转
//     iloc.jump(gotoInst->getTrueTarget()->getName());
// }

/// @brief bc/br 指令翻译成 ARM32 汇编
/// @param inst IR 指令（GotoInstruction）
void InstSelectorArm32::translate_br(Instruction * inst)
{
    // 确保它是我们的 GotoInstruction
    auto * brInst = static_cast<GotoInstruction*>(inst);

    Value * cond = brInst->getCond();
    // 真假目标
    auto * Ltrue  = brInst->getTrueTarget();
    auto * Lfalse = brInst->getFalseTarget();

    if (!cond) {
        // —— 无条件跳转 —— 
        // 直接 b label
        iloc.jump(Ltrue->getName());
    } else {
        // —— 有条件跳转 bc —— 
        // 1) 把 cond 装入寄存器
        int condReg = cond->getRegId();
        int loadReg;
        if (condReg == -1) {
            // cond 未在寄存器里，分配一个再 load
            loadReg = simpleRegisterAllocator.Allocate(cond);
            iloc.load_var(loadReg, cond);
        } else {
            loadReg = condReg;
        }
        std::string rCond = PlatformArm32::regName[loadReg];

        // 2) cmp rCond, #0
        iloc.inst("cmp", rCond, "#0");

        // 3) cond 为真 跳 Ltrue：bne Ltrue
        iloc.inst("bne", Ltrue->getName());

        // 4) cond 为假 跳 Lfalse：b Lfalse
        iloc.jump(Lfalse->getName());

        // 5) 如果 cond 是临时寄存器，释放掉
        simpleRegisterAllocator.free(cond);
    }
}



/// @brief 函数入口指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_entry(Instruction * inst)
{
    // 查看保护的寄存器
    auto & protectedRegNo = func->getProtectedReg();
    auto & protectedRegStr = func->getProtectedRegStr();

    bool first = true;
    for (auto regno: protectedRegNo) {
        if (first) {
            protectedRegStr = PlatformArm32::regName[regno];
            first = false;
        } else {
            protectedRegStr += "," + PlatformArm32::regName[regno];
        }
    }

    if (!protectedRegStr.empty()) {
        iloc.inst("push", "{" + protectedRegStr + "}");
    }

    // 为fun分配栈帧，含局部变量、函数调用值传递的空间等
    iloc.allocStack(func, ARM32_TMP_REG_NO);
}

/// @brief 函数出口指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_exit(Instruction * inst)
{
    if (inst->getOperandsNum()) {
        // 存在返回值
        Value * retVal = inst->getOperand(0);

        // 赋值给寄存器R0
        iloc.load_var(0, retVal);
    }

    // 恢复栈空间
    iloc.inst("mov", "sp", "fp");

    // 保护寄存器的恢复
    auto & protectedRegStr = func->getProtectedRegStr();
    if (!protectedRegStr.empty()) {
        iloc.inst("pop", "{" + protectedRegStr + "}");
    }

    iloc.inst("bx", "lr");
}

/// @brief 赋值指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_assign(Instruction * inst)
{
    Value * result = inst->getOperand(0);
    Value * arg1 = inst->getOperand(1);

    int32_t arg1_regId = arg1->getRegId();
    int32_t result_regId = result->getRegId();

    if (arg1_regId != -1) {
        // 寄存器 => 内存
        // 寄存器 => 寄存器

        // r8 -> rs 可能用到r9
        iloc.store_var(arg1_regId, result, ARM32_TMP_REG_NO);
    } else if (result_regId != -1) {
        // 内存变量 => 寄存器

        iloc.load_var(result_regId, arg1);
    } else {
        // 内存变量 => 内存变量

        int32_t temp_regno = simpleRegisterAllocator.Allocate();

        // arg1 -> r8
        iloc.load_var(temp_regno, arg1);

        // r8 -> rs 可能用到r9
        iloc.store_var(temp_regno, result, ARM32_TMP_REG_NO);

        simpleRegisterAllocator.free(temp_regno);
    }
}

/// @brief 二元操作指令翻译成ARM32汇编
/// @param inst IR指令
/// @param operator_name 操作码
/// @param rs_reg_no 结果寄存器号
/// @param op1_reg_no 源操作数1寄存器号
/// @param op2_reg_no 源操作数2寄存器号
void InstSelectorArm32::translate_two_operator(Instruction * inst, string operator_name)
{
    Value * result = inst;
    Value * arg1 = inst->getOperand(0);
    Value * arg2 = inst->getOperand(1);

    int32_t arg1_reg_no = arg1->getRegId();
    int32_t arg2_reg_no = arg2->getRegId();
    int32_t result_reg_no = inst->getRegId();
    int32_t load_result_reg_no, load_arg1_reg_no, load_arg2_reg_no;

    // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg1_reg_no == -1) {

        // 分配一个寄存器r8
        load_arg1_reg_no = simpleRegisterAllocator.Allocate(arg1);

        // arg1 -> r8，这里可能由于偏移不满足指令的要求，需要额外分配寄存器
        iloc.load_var(load_arg1_reg_no, arg1);
    } else {
        load_arg1_reg_no = arg1_reg_no;
    }

    // 看arg2是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg2_reg_no == -1) {

        // 分配一个寄存器r9
        load_arg2_reg_no = simpleRegisterAllocator.Allocate(arg2);

        // arg2 -> r9
        iloc.load_var(load_arg2_reg_no, arg2);
    } else {
        load_arg2_reg_no = arg2_reg_no;
    }

    // 看结果变量是否是寄存器，若不是则需要分配一个新的寄存器来保存运算的结果
    if (result_reg_no == -1) {
        // 分配一个寄存器r10，用于暂存结果
        load_result_reg_no = simpleRegisterAllocator.Allocate(result);
    } else {
        load_result_reg_no = result_reg_no;
    }

    // r8 + r9 -> r10
    iloc.inst(operator_name,
              PlatformArm32::regName[load_result_reg_no],
              PlatformArm32::regName[load_arg1_reg_no],
              PlatformArm32::regName[load_arg2_reg_no]);

    // 结果不是寄存器，则需要把rs_reg_name保存到结果变量中
    if (result_reg_no == -1) {

        // 这里使用预留的临时寄存器，因为立即数可能过大，必须借助寄存器才可操作。

        // r10 -> result
        iloc.store_var(load_result_reg_no, result, ARM32_TMP_REG_NO);
    }

    // 释放寄存器
    simpleRegisterAllocator.free(arg1);
    simpleRegisterAllocator.free(arg2);
    simpleRegisterAllocator.free(result);
}

/// @brief 整数加法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_add_int32(Instruction * inst)
{
    translate_two_operator(inst, "add");
}

/// @brief 整数减法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_sub_int32(Instruction * inst)
{
    translate_two_operator(inst, "sub");
}

/// @brief 整数乘法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_mul_int32(Instruction * inst)
{
    translate_two_operator(inst, "mul");
}

/// @brief 整数除法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_div_int32(Instruction * inst)
{
    translate_two_operator(inst, "sdiv");
}

/// @brief 整数取余指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_mod_int32(Instruction * inst)
{
    Value * result = inst;
    Value * arg1 = inst->getOperand(0);
    Value * arg2 = inst->getOperand(1);

    int32_t arg1_reg_no = arg1->getRegId();
    int32_t arg2_reg_no = arg2->getRegId();
    int32_t result_reg_no = inst->getRegId();
    int32_t load_result_reg_no, load_arg1_reg_no, load_arg2_reg_no;

    // arg1: 加载到寄存器
    if (arg1_reg_no == -1) {
        load_arg1_reg_no = simpleRegisterAllocator.Allocate(arg1);
        iloc.load_var(load_arg1_reg_no, arg1);
    } else {
        load_arg1_reg_no = arg1_reg_no;
    }

    // arg2: 加载到寄存器
    if (arg2_reg_no == -1) {
        load_arg2_reg_no = simpleRegisterAllocator.Allocate(arg2);
        iloc.load_var(load_arg2_reg_no, arg2);
    } else {
        load_arg2_reg_no = arg2_reg_no;
    }

    // result: 如果不是寄存器则分配一个
    if (result_reg_no == -1) {
        load_result_reg_no = simpleRegisterAllocator.Allocate(result);
    } else {
        load_result_reg_no = result_reg_no;
    }

    // 分配中间寄存器
    int32_t div_reg = simpleRegisterAllocator.Allocate(nullptr);  // 商
    int32_t mul_reg = simpleRegisterAllocator.Allocate(nullptr);  // 商 × 除数

    // sdiv div_reg, arg1, arg2
    iloc.inst("sdiv",
        PlatformArm32::regName[div_reg],
        PlatformArm32::regName[load_arg1_reg_no],
        PlatformArm32::regName[load_arg2_reg_no]);

    // mul mul_reg, div_reg, arg2
    iloc.inst("mul",
        PlatformArm32::regName[mul_reg],
        PlatformArm32::regName[div_reg],
        PlatformArm32::regName[load_arg2_reg_no]);

    // sub result_reg, arg1, mul_reg
    iloc.inst("sub",
        PlatformArm32::regName[load_result_reg_no],
        PlatformArm32::regName[load_arg1_reg_no],
        PlatformArm32::regName[mul_reg]);

    // 如果结果不是寄存器，要写回内存
    if (result_reg_no == -1) {
        iloc.store_var(load_result_reg_no, result, ARM32_TMP_REG_NO);
    }

    // 释放所有分配的寄存器
    simpleRegisterAllocator.free(arg1);
    simpleRegisterAllocator.free(arg2);
    simpleRegisterAllocator.free(result);
    simpleRegisterAllocator.free(nullptr);  // div_reg
    simpleRegisterAllocator.free(nullptr);  // mul_reg
}

/// @brief 通用比较运算翻译
/// @param inst        IR 指令（也是 Value*）
/// @param condSuffix 比较条件后缀："lt","gt","le","ge","eq","ne"
void InstSelectorArm32::translate_cmp_operator(Instruction * inst, string condSuffix) {
    // Instruction 本身也是 Value
    Value   *result = inst;
    Value   *arg1   = inst->getOperand(0);
    Value   *arg2   = inst->getOperand(1);

    // 查看它们是否已经在寄存器里
    int r1 = arg1->getRegId();
    int r2 = arg2->getRegId();
    int rd = result->getRegId();

    int load_r1, load_r2, load_rd;

    // 如果 arg1 不在寄存器，分配一个并 load
    if (r1 == -1) {
        load_r1 = simpleRegisterAllocator.Allocate(arg1);
        iloc.load_var(load_r1, arg1);
    } else {
        load_r1 = r1;
    }

    // arg2 同理
    if (r2 == -1) {
        load_r2 = simpleRegisterAllocator.Allocate(arg2);
        iloc.load_var(load_r2, arg2);
    } else {
        load_r2 = r2;
    }

    // 结果如果不在寄存器，也要分配一个暂存
    if (rd == -1) {
        load_rd = simpleRegisterAllocator.Allocate(result);
    } else {
        load_rd = rd;
    }

    // 1) 比较指令
    iloc.inst("cmp",
              PlatformArm32::regName[load_r1],
              PlatformArm32::regName[load_r2]);

    // 2) 先把结果置为 0
    iloc.inst("mov",
              PlatformArm32::regName[load_rd],
              "#0");

    // 3) 根据 condSuffix 置为 1
    //    e.g. movlt rD, #1   // if r1<r2
    iloc.inst("mov" + condSuffix,
              PlatformArm32::regName[load_rd],
              "#1");

    // 如果原来 result 不在寄存器，要把暂存寄存器存回内存
    if (rd == -1) {
        iloc.store_var(load_rd, result, ARM32_TMP_REG_NO);
    }

    // 释放分配的寄存器
    simpleRegisterAllocator.free(arg1);
    simpleRegisterAllocator.free(arg2);
    simpleRegisterAllocator.free(result);
}

/// @brief 整数小于比较翻译成ARM32汇编
void InstSelectorArm32::translate_lt_int32(Instruction * inst) {
    translate_cmp_operator(inst, "lt");
}

/// @brief 整数大于比较翻译成ARM32汇编
void InstSelectorArm32::translate_gt_int32(Instruction * inst) {
    translate_cmp_operator(inst, "gt");
}

/// @brief 整数小于等于比较翻译成ARM32汇编
void InstSelectorArm32::translate_le_int32(Instruction * inst) {
    translate_cmp_operator(inst, "le");
}

/// @brief 整数大于等于比较翻译成ARM32汇编
void InstSelectorArm32::translate_ge_int32(Instruction * inst) {
    translate_cmp_operator(inst, "ge");
}

/// @brief 整数等于比较翻译成ARM32汇编
void InstSelectorArm32::translate_eq_int32(Instruction * inst) {
    translate_cmp_operator(inst, "eq");
}

/// @brief 整数不等于比较翻译成ARM32汇编
void InstSelectorArm32::translate_ne_int32(Instruction * inst) {
    translate_cmp_operator(inst, "ne");
}

/// @brief 函数调用指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_call(Instruction * inst)
{
    FuncCallInstruction * callInst = dynamic_cast<FuncCallInstruction *>(inst);

    int32_t operandNum = callInst->getOperandsNum();

    if (operandNum != realArgCount) {

        // 两者不一致 也可能没有ARG指令，正常
        if (realArgCount != 0) {

            minic_log(LOG_ERROR, "ARG指令的个数与调用函数个数不一致");
        }
    }

    if (operandNum) {

        // 强制占用这几个寄存器参数传递的寄存器
        simpleRegisterAllocator.Allocate(0);
        simpleRegisterAllocator.Allocate(1);
        simpleRegisterAllocator.Allocate(2);
        simpleRegisterAllocator.Allocate(3);

        // 前四个的后面参数采用栈传递
        int esp = 0;
        for (int32_t k = 4; k < operandNum; k++) {

            auto arg = callInst->getOperand(k);

            // 新建一个内存变量，用于栈传值到形参变量中
            MemVariable * newVal = func->newMemVariable((Type *) PointerType::get(arg->getType()));
            newVal->setMemoryAddr(ARM32_SP_REG_NO, esp);
            esp += 4;

            Instruction * assignInst = new MoveInstruction(func, newVal, arg);

            // 翻译赋值指令
            translate_assign(assignInst);

            delete assignInst;
        }

        for (int32_t k = 0; k < operandNum && k < 4; k++) {

            auto arg = callInst->getOperand(k);

            // 检查实参的类型是否是临时变量。
            // 如果是临时变量，该变量可更改为寄存器变量即可，或者设置寄存器号
            // 如果不是，则必须开辟一个寄存器变量，然后赋值即可

            Instruction * assignInst = new MoveInstruction(func, PlatformArm32::intRegVal[k], arg);

            // 翻译赋值指令
            translate_assign(assignInst);

            delete assignInst;
        }
    }

    iloc.call_fun(callInst->getName());

    if (operandNum) {
        simpleRegisterAllocator.free(0);
        simpleRegisterAllocator.free(1);
        simpleRegisterAllocator.free(2);
        simpleRegisterAllocator.free(3);
    }

    // 赋值指令
    if (callInst->hasResultValue()) {

        // 新建一个赋值操作
        Instruction * assignInst = new MoveInstruction(func, callInst, PlatformArm32::intRegVal[0]);

        // 翻译赋值指令
        translate_assign(assignInst);

        delete assignInst;
    }

    // 函数调用后清零，使得下次可正常统计
    realArgCount = 0;
}

///
/// @brief 实参指令翻译成ARM32汇编
/// @param inst
///
void InstSelectorArm32::translate_arg(Instruction * inst)
{
    // 翻译之前必须确保源操作数要么是寄存器，要么是内存，否则出错。
    Value * src = inst->getOperand(0);

    // 当前统计的ARG指令个数
    int32_t regId = src->getRegId();

    if (realArgCount < 4) {
        // 前四个参数
        if (regId != -1) {
            if (regId != realArgCount) {
                // 肯定寄存器分配有误
                minic_log(LOG_ERROR, "第%d个ARG指令对象寄存器分配有误: %d", argCount + 1, regId);
            }
        } else {
            minic_log(LOG_ERROR, "第%d个ARG指令对象不是寄存器", argCount + 1);
        }
    } else {
        // 必须是内存分配，若不是则出错
        int32_t baseRegId;
        bool result = src->getMemoryAddr(&baseRegId);
        if ((!result) || (baseRegId != ARM32_SP_REG_NO)) {

            minic_log(LOG_ERROR, "第%d个ARG指令对象不是SP寄存器寻址", argCount + 1);
        }
    }

    realArgCount++;
}
