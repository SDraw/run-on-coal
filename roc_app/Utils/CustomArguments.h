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
public:
    CustomArguments();
    ~CustomArguments();

    template<typename T> void Push(T f_val)
    {
        m_arguments.emplace_back(f_val);
        m_argumentsCount++;
    }
    void Clear();

    inline size_t GetArgumentsCount() const { return m_arguments.size(); }
    inline const std::vector<CustomArgument>& GetArgumentsVectorRef() const { return m_arguments; }
protected:
    const ICustomArgument* GetArgument(size_t f_index) const;
};

}
