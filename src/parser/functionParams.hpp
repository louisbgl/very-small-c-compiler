#pragma once

#include <string>
#include <vector>

struct FunctionParameter {
    enum class ParameterType { Int };
    ParameterType type;
    std::string name;
};
