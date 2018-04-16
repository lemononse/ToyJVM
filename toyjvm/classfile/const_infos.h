//
// Created by cyhone on 18-2-8.
//

#ifndef TOYJVM_CONST_INFOS_H
#define TOYJVM_CONST_INFOS_H

#include <toyjvm/common/basereader.h>
#include <toyjvm/common/jvm_types.h>
#include <toyjvm/classfile/const_pool.h>
#include <boost/noncopyable.hpp>
#include <string>

namespace jvm {
    class BaseConstInfo : boost::noncopyable {
    public:
        BaseConstInfo(ConstPool::ConstType tag, const ConstPool &const_pool)
                : tag_(tag), const_pool_(const_pool)
        {}

        virtual ~BaseConstInfo() = default;

        virtual void read(BaseReader &)
        {}

    protected:
        ConstPool::ConstType tag_;
        const ConstPool &const_pool_;
    };


    class ConstClassInfo : public BaseConstInfo {
    public:
        explicit ConstClassInfo(ConstPool &const_pool)
                : BaseConstInfo(ConstPool::ConstType::Class, const_pool)
        {}

        void read(BaseReader &) override;

        const std::string &className();

    private:
        u2 name_index_;
        std::string class_name_;
    };

    template<ConstPool::ConstType tag>
    class BaseMemberRefInfo : public BaseConstInfo {
    public:
        explicit BaseMemberRefInfo(ConstPool &const_pool)
                : BaseConstInfo(tag, const_pool)
        {}

        void read(jvm::BaseReader &reader) override
        {
            class_index_ = reader.read<u2>();
            name_and_type_index_ = reader.read<u2>();
        }

    private:
        u2 class_index_;
        u2 name_and_type_index_;
    };

    using ConstFieldRefInfo = BaseMemberRefInfo<ConstPool::ConstType::FieldRef>;
    using ConstMethodRefInfo = BaseMemberRefInfo<ConstPool::ConstType::MethodRef>;
    using ConstInterfaceMethodRefInfo = BaseMemberRefInfo<ConstPool::ConstType::InterfaceMethodRef>;

    class ConstStringInfo : public BaseConstInfo {
    public:
        explicit ConstStringInfo(ConstPool &const_pool)
                : BaseConstInfo(ConstPool::ConstType::String, const_pool)
        {}

        void read(BaseReader &) override;

    private:
        u2 string_index_;
    };

    template<ConstPool::ConstType tag, typename T>
    class BaseNumberInfo : public BaseConstInfo {
    public:
        explicit BaseNumberInfo(ConstPool &const_pool)
                : BaseConstInfo(tag, const_pool)
        {}

        void read(BaseReader &reader) override
        {
            val_ = reader.read<T>();
        }

        T val() const
        {
            return val_;
        }

    private:
        T val_;
    };

    using ConstFloatInfo = BaseNumberInfo<ConstPool::ConstType::Float, jfloat>;
    using ConstDoubleInfo = BaseNumberInfo<ConstPool::ConstType::Double, jdouble>;
    using ConstIntegerInfo = BaseNumberInfo<ConstPool::ConstType::Integer, jint>;
    using ConstLongInfo = BaseNumberInfo<ConstPool::ConstType::Long, jlong>;

    class ConstNameAndTypeInfo : public BaseConstInfo {
    public:
        explicit ConstNameAndTypeInfo(ConstPool &const_pool)
                : BaseConstInfo(ConstPool::ConstType::NameAndType, const_pool)
        {}

        void read(BaseReader &) override;

    private:
        u2 name_index_;
        u2 descriptor_index_;
    };

    class ConstUtf8Info : public BaseConstInfo {
    public:
        explicit ConstUtf8Info(ConstPool &const_pool)
                : BaseConstInfo(ConstPool::ConstType::Utf8, const_pool)
        {}

        void read(BaseReader &) override;

        std::string asString();

    private:
        bytes data_;
    };

    class ConstMethodHandleInfo : public BaseConstInfo {
    public:
        explicit ConstMethodHandleInfo(ConstPool &const_pool)
                : BaseConstInfo(ConstPool::ConstType::MethodHandle, const_pool)
        {}

        void read(BaseReader &) override;

    private:
        u1 reference_kind_;
        u2 reference_index_;
    };

    class ConstMethodTypeInfo : public BaseConstInfo {
    public:
        explicit ConstMethodTypeInfo(ConstPool &const_pool)
                : BaseConstInfo(ConstPool::ConstType::MethodType, const_pool)
        {}

        void read(BaseReader &) override;

    private:
        u2 descriptor_index_;
    };

    class ConstInvokeDynamicInfo : public BaseConstInfo {
    public:
        explicit ConstInvokeDynamicInfo(ConstPool &const_pool)
                : BaseConstInfo(ConstPool::ConstType::InvokeDynamic, const_pool)
        {}

        void read(BaseReader &) override;

    private:
        u2 boostrap_method_attr_index_;
        u2 name_and_type_index_;
    };
}
#endif //TOYJVM_CONST_INFO_H