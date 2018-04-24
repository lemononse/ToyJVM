//
// Created by cyhone on 18-4-16.
//

#ifndef TOYJVM_JVM_FRAME_H
#define TOYJVM_JVM_FRAME_H

#include <cstddef>
#include <toyjvm/runtime/local_slots.h>
#include <toyjvm/runtime/operand_stack.h>
#include <toyjvm/runtime/jvm_member.h>

namespace jvm {
    class JvmFrame {
    public:
        JvmFrame(size_t local_slot_size, size_t max_stack_num)
                : local_slots_(local_slot_size),
                  operand_stack_(max_stack_num)
        {}

        inline LocalSlots &localSlots() const
        {
            return local_slots_;
        }

        inline OperandStack &operandStack() const
        {
            return operand_stack_;
        }

        inline std::shared_ptr<JvmMethod> method() const{
            return method_;
        }
    private:
        LocalSlots local_slots_;
        OperandStack operand_stack_;
        std::shared_ptr<JvmMethod> method_;
    };
}
#endif //TOYJVM_JVM_FRAME_H