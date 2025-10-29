#include <string>
#include <unordered_map>

struct StdStringHash
{
    std::size_t operator()(const std::string& str) const { return std::hash<std::string>()(str); }
};