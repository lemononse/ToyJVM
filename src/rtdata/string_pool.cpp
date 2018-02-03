#include <jvm/rtdata/class_loader.h>
#include <jvm/rtdata/jvm_reference.h>
#include <jvm/rtdata/string_pool.h>
#include <map>
namespace cyh {

std::map<std::string, JObject*> g_innered_strs;
JObject* GetStringFromPool(ClassLoader* loader, std::string str)
{
    if (g_innered_strs.find(str) != g_innered_strs.end()) {
	return g_innered_strs[str];
    }

    auto char_array_class = loader->LoadClass("[C");
    std::vector<char> char_vec(str.begin(), str.end());
    auto char_array_obj = new JArray<char>(char_vec, char_array_class);

    auto string_class = loader->LoadClass("java/lang/String");

    auto str_obj = new JObject(string_class);
    std::string name = "value";
    std::string descriptor = "[C";
    str_obj->SetRefVar(name, descriptor, char_array_obj);

    g_innered_strs[str] = str_obj;

    return str_obj;
}
}