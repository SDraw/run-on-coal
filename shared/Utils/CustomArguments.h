#pragma once
#include "Utils/CustomArgument.h"

class CustomArguments final
{
    std::vector<CustomArgument> m_arguments;

    CustomArguments(const CustomArguments &that) = delete;
    CustomArguments& operator=(const CustomArguments &that) = delete;
public:
    CustomArguments();
    ~CustomArguments();

    template<typename T> void Push(T f_val)
    {
        m_arguments.emplace_back(f_val);
    }

    inline size_t GetArgumentsCount() const { return m_arguments.size(); }
    inline const std::vector<CustomArgument>& GetArgumentsVectorRef() const { return m_arguments; }

    void Clear();
};
