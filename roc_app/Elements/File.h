#pragma once
#include "Interfaces/IFile.h"
#include "Elements/Element.h"

namespace ROC
{

class File final : public Element, public virtual IFile
{
    enum FileMode : unsigned char
    {
        FM_Read = 0U,
        FM_Write,

        FM_None = 0xFFU
    };

    std::fstream *m_file;
    std::string m_path;
    FileMode m_type;

    File(const File &that) = delete;
    File& operator=(const File &that) = delete;
public:
    size_t Read(std::string &f_data, size_t f_lenght);
    size_t Write(const std::string &f_data);

    size_t GetSize();
    bool SetPosition(size_t f_pos);
    size_t GetPosition();
    bool IsEOF() const;
    void Flush();

    const std::string& GetPath() const;
protected:
    File();
    ~File();
    bool Create(const std::string &f_path);
    bool Open(const std::string &f_path, bool f_ro);

    friend class ElementManager;
};

}
