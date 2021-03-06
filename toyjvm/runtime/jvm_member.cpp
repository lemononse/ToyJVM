//
// Created by cyhone on 18-4-19.
//

#include <toyjvm/runtime/jvm_member.h>
#include <toyjvm/utilities/method_descriptor.h>

namespace jvm {
    JvmField::JvmField(JvmClass *this_class, FieldInfo *field_info)
            : JvmMember(this_class, field_info)
    {
        auto constValueAttr = field_info->constantValueAttr();
        const_value_index_ = constValueAttr != nullptr ? constValueAttr->constValueIndex() : -1;
    }

    JvmMethod::JvmMethod(JvmClass *this_class, MethodInfo *method_info)
            : JvmMember(this_class, method_info)
    {

        // 计算该方法所需的参数个数
        auto method_descriptor = parseMethodDescriptor(descriptor_);
        calcArgsSlotCount(method_descriptor.param_types);

        if (access_flags_.isNative()) {
            injectCodeAttribute(method_descriptor.return_type);
        } else {
            copyCodeAttr(method_info);
        }
    }

    void JvmMethod::injectCodeAttribute(const std::string &return_type)
    {
        max_stack_ = 4;
        max_locals_ = args_slot_count_;
        switch (return_type[0]) {
            case 'V': {
                code_ = bytes{0xfe, 0xb1};
                return;
            }
            case 'D': {
                code_ = bytes{0xfe, 0xaf};
                return;
            }
            case 'F': {
                code_ = bytes{0xfe, 0xae};
                return;
            }
            case 'J': {
                code_ = bytes{0xfe, 0xad};
                return;
            }
            case '[':
            case 'L': {
                code_ = bytes{0xfe, 0xb0};
                return;
            }
            default: {
                code_ = bytes{0xfe, 0xac};
                return;
            }
        }
    }

    void JvmMethod::calcArgsSlotCount(std::vector<std::string> &param_types)
    {
        for (const auto &param_type : param_types) {
            args_slot_count_++;
            if (param_type == "J" || param_type == "D") {
                args_slot_count_++;
            }
        }
        if (!access_flags_.isStatic()) {
            args_slot_count_++;
        }
    }

    void JvmMethod::copyCodeAttr(jvm::MethodInfo *method_info)
    {
        AttrCode *codeAttr = method_info->codeAttr();
        auto instance_klass = dynamic_cast<JvmClass *>(this_class_);
        if (codeAttr != nullptr) {
            max_stack_ = codeAttr->maxStack();
            max_locals_ = codeAttr->maxLocals();
            code_ = std::move(codeAttr->moveCode());

            exception_table_ = new ExceptionTable(codeAttr->exceptionTable(),
                                                  dynamic_cast<RuntimeConstPool *>(&(instance_klass->runtimeConstPool()))
            );

            line_number_table_ = codeAttr->lineNumberTable();
        }
    }

    int JvmMethod::findExceptionHandler(jvm::JvmClass *jclass, int pc) const
    {
        if (exception_table_ != nullptr) {
            auto handler = exception_table_->findExceptionHandler(jclass, pc);
            return handler == boost::none ? -1 : handler.get().handler_pc;
        }

        return -1;
    }

    int JvmMethod::getLineNumber(int pc)
    {
        if (access_flags_.isNative()) {
            return -2;
        }

        if (line_number_table_ == nullptr) {
            return -1;
        }

        return line_number_table_->getLineNumber(pc);
    }
}