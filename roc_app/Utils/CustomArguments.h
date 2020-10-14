#pragma once
#include "Interfaces/ICustomArguments.h"
#include "Utils/CustomArgument.h"

namespace ROC
{

class CustomArguments final : public ICustomArguments
{
    std::vector<CustomArgument> m_arguments;
    size_t m_argumentsCount;

    CustomArguments(const CustomArguments &that) = delete;
    CustomArguments& operator=(const CustomArguments &that) = delete;

    const ICustomArgument* GetArgument(size_t f_index) const;
public:
    CustomArguments();
    ~CustomArguments();

    template<typename T> void Push(T f_val);
    void Clear();

    size_t GetArgumentsCount() const;
    const std::vector<CustomArgument>& GetArguments() const;
};

}

template<typename T> void ROC::CustomArguments::Push(T f_val)
{
    m_arguments.emplace_back(f_val);
    m_argumentsCount++;
}
