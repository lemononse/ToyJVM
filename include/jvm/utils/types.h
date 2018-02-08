#ifndef MY_JVM_UTILS_COMMON_H
#define MY_JVM_UTILS_COMMON_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>
namespace cyh {
class JReference;

using u8 = uint64_t;
using u4 = uint32_t;
using u2 = uint16_t;
using u1 = uint8_t;
using bytes = std::vector<u1>;

using j_byte = int8_t;
using j_char = uint16_t;
using j_short = int16_t;
using j_int = int32_t;
using j_float = float;
using j_long = int64_t;
using j_double = double;
using j_ref = JReference*;
/* 
	* type map:
	* Java  C++
	* byte  int8_t
	* char  uint8_t
	* short int16_t
	* int   int32_t
	* float float
	* 
	* long  int64_t
	* double double
 	*/
const std::unordered_map<std::string, std::string> PrimitiveTypes = {
    { "void", "V" },
    { "boolean", "Z" },
    { "byte", "B" },
    { "short", "S" },
    { "int", "I" },
    { "long", "J" },
    { "char", "C" },
    { "float", "F" },
    { "double", "D" }
};
}

#endif /* ifndef  */
