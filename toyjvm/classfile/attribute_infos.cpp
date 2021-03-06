//
// Created by cyhone on 18-4-14.
//

#include <toyjvm/classfile/attribute_infos.h>
#include <toyjvm/classfile/attribute_table.h>

namespace jvm {
    void AttrConstantValue::read(jvm::ByteReader &reader)
    {
        constant_value_index_ = reader.read<u2>();
    }

    AttrCode::AttrCode(const std::string &attr_type, const ConstPool &const_pool)
            : BaseAttrInfo(attr_type, const_pool),
              attr_table_(new AttrTable())
    {}

    AttrLineNumberTable *AttrCode::lineNumberTable()
    {
        return attr_table_->getFirst<AttrLineNumberTable>("LineNumberTable");
    }

    void AttrCode::read(jvm::ByteReader &reader)
    {
        max_stack_ = reader.read<u2>();
        max_locals_ = reader.read<u2>();
        code_ = reader.batchRead<u4, u1>();

        auto exception_len = reader.read<u2>();
        exception_table_.reserve(exception_len);
        for (int i = 0; i < exception_len; ++i) {
            ExceptionInfo e{};
            e.start_pc = reader.read<u2>();
            e.end_pc = reader.read<u2>();
            e.handler_pc = reader.read<u2>();
            e.catch_type = reader.read<u2>();

            exception_table_.push_back(e);
        }
        attr_table_->read(reader, const_pool_);
    }

    void AttrExceptions::read(jvm::ByteReader &reader)
    {
        exception_index_table_ = reader.batchRead<u2, u2>();
    }

    void AttrInnerClasses::read(jvm::ByteReader &reader)
    {
        auto class_num = reader.read<u2>();
        inner_classes_.reserve(class_num);
        for (int i = 0; i < class_num; ++i) {
            InnerClassInfo c{};
            c.inner_class_info_index = reader.read<u2>();
            c.outer_class_info_index = reader.read<u2>();
            c.inner_name_index = reader.read<u2>();
            c.inner_class_access_flags = reader.read<u2>();

            inner_classes_.push_back(c);
        }
    }

    void AttrEnclosingMethod::read(jvm::ByteReader &reader)
    {
        class_index_ = reader.read<u2>();
        method_index_ = reader.read<u2>();
    }

    void AttrSignature::read(jvm::ByteReader &reader)
    {
        signature_index_ = reader.read<u2>();
    }

    void AttrSourceFile::read(jvm::ByteReader &reader)
    {
        sourcefile_index_ = reader.read<u2>();
    }

    std::string AttrSourceFile::sourceFileName() const
    {
        return const_pool_.stringAt(sourcefile_index_);
    }

    void AttrLineNumberTable::read(jvm::ByteReader &reader)
    {
        auto table_len = reader.read<u2>();
        line_number_table_.reserve(table_len);

        for (int i = 0; i < table_len; ++i) {
            LineNumberInfo l{};
            l.start_pc = reader.read<u2>();
            l.line_number = reader.read<u2>();

            line_number_table_.push_back(l);
        }
    }

    int AttrLineNumberTable::getLineNumber(int pc) const
    {
        for (int i = line_number_table_.size(); i > 0; --i) {
            if (pc >= line_number_table_[i].start_pc) {
                return line_number_table_[i].line_number;
            }
        }

        return -1;
    }

    void AttrLocalVariableTable::read(jvm::ByteReader &reader)
    {
        auto count = reader.read<u2>();
        for (int i = 0; i < count; ++i) {
            LocalVariableInfo item{};
            item.start_pc = reader.read<u2>();
            item.length = reader.read<u2>();
            item.name_index = reader.read<u2>();
            item.descriptor_index = reader.read<u2>();
            item.index = reader.read<u2>();
            local_variable_table_.push_back(item);
        }
    }

    void AttrLocalVariableTypeTable::read(jvm::ByteReader &reader)
    {
        auto count = reader.read<u2>();
        for (int i = 0; i < count; ++i) {
            LocalVariableTypeInfo item{};
            item.start_pc = reader.read<u2>();
            item.length = reader.read<u2>();
            item.name_index = reader.read<u2>();
            item.signature_index = reader.read<u2>();
            item.index = reader.read<u2>();
            local_variable_type_table_.push_back(item);
        }
    }

    void AttrBootstrapMethods::read(jvm::ByteReader &reader)
    {
        auto count = reader.read<u2>();
        for (int i = 0; i < count; ++i) {
            BootStrapMethod item{};
            item.bootstrap_method_ref = reader.read<u2>();
            item.bootstrap_arguments = reader.batchRead<u2, u2>();
            bootStrap_methods_.push_back(item);
        }
    }

    void AttrUnparsed::read(jvm::ByteReader &reader)
    {
        data_ = reader.batchRead<u1>(attr_len_);
    }
}