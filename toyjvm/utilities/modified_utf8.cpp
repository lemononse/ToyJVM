//
// Created by cyhone on 18-4-14.
//

#include <toyjvm/utilities/modified_utf8.h>

namespace jvm {
    std::string parseMutf8AsString(bytes &raw_data)
    {
        std::string str(raw_data.begin(), raw_data.end());
        return str;
    }

    size_t ModifiedUTF8::rawLen() const
    {
        return raw_utf8_.size();
    }

    const bytes &ModifiedUTF8::rawUTF8() const
    {
        return raw_utf8_;
    }

    size_t ModifiedUTF8::unicodeLen() const
    {
        int num_chars = raw_utf8_.size();
        for (int i = 0; i < raw_utf8_.size(); i++) {
            if ((raw_utf8_[i] & 0xC0) == 0x80) {
                --num_chars;
            }
        }
        return num_chars;
    }

    ModifiedUTF8 &ModifiedUTF8::operator=(jvm::bytes data)
    {
        raw_utf8_ = std::move(data);
    }

    bool ModifiedUTF8::operator==(const jvm::bytes &data) const
    {
        if (raw_utf8_.size() != data.size()) {
            return false;
        }

        return std::equal(raw_utf8_.begin(), raw_utf8_.end(), data.begin());
    }

    bool ModifiedUTF8::operator==(const jvm::ModifiedUTF8 &other) const
    {
        return *this == other.raw_utf8_;
    }

    bool ModifiedUTF8::operator==(const std::string &str) const
    {
        bytes data(str.begin(), str.end());
        return *this == data;
    }

    std::tuple<jchar, int> ModifiedUTF8::next(int start_index) const
    {
        unsigned char ch, ch2, ch3;
        int length = -1;              /* bad length */
        jchar result;
        switch ((ch = raw_utf8_[start_index]) >> 4) {
            case 0x8:
            case 0x9:
            case 0xA:
            case 0xB:
            case 0xF:
                /* Shouldn't happen. */
                break;
            case 0xC:
            case 0xD:
                /* 110xxxxx  10xxxxxx */
                if (((ch2 = raw_utf8_[start_index + 1]) & 0xC0) == 0x80) {
                    unsigned char high_five = ch & 0x1F;
                    unsigned char low_six = ch2 & 0x3F;
                    result = (high_five << 6) + low_six;
                    length = 2;
                    break;
                }
                break;
            case 0xE:
                /* 1110xxxx 10xxxxxx 10xxxxxx */
                if (((ch2 = raw_utf8_[start_index + 1]) & 0xC0) == 0x80) {
                    if (((ch3 = raw_utf8_[start_index + 2]) & 0xC0) == 0x80) {
                        unsigned char high_four = ch & 0x0f;
                        unsigned char mid_six = ch2 & 0x3f;
                        unsigned char low_six = ch3 & 0x3f;
                        result = (((high_four << 6) + mid_six) << 6) + low_six;
                        length = 3;
                    }
                }
                break;
            default:
                result = ch;
                length = 1;
                break;
        }
        if (length <= 0) {
            return std::make_tuple(raw_utf8_[start_index], 1);
        }
        return std::make_tuple(result, length);
    }
    std::vector<jchar> ModifiedUTF8::asJCharArr() const
    {
        unsigned char ch;
        int index = 0;
        auto unicode_length = unicodeLen();

        std::vector<jchar> raw_utf16(unicode_length);

        /* ASCII case loop optimization */
        for (; index < unicode_length; index++) {
            if ((ch = raw_utf8_[index]) > 0x7F) { break; }
            raw_utf16[index] = ch;
        }

        while (index < unicode_length) {
            auto result = next(index);
            raw_utf16[index] = std::get<0>(result);
            index += std::get<1>(result);
        }

        return raw_utf16;
    }

    std::string ModifiedUTF8::asString() const
    {
        std::string str(raw_utf8_.begin(), raw_utf8_.end());
        return str;
    }

    const std::vector<jchar> &UTF16::rawUTF16() const
    {
        return raw_utf16_;
    }
}