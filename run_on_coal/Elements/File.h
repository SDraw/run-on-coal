#pragma once

#include "Elements/Element.h"

namespace ROC
{

class File final : public Element
{
    enum FileMode
    {
        FM_None = -1,
        FM_Read,
        FM_Write
    };
    FileMode m_type = FM_None;
    std::fstream *m_file;
    std::string m_path;

    File(const File &that);
    File& operator=(const File &that);
public:
    size_t Read(std::string &f_data, size_t f_lenght);
    size_t Write(const std::string &f_data);

    size_t GetSize();
    bool SetPosition(size_t f_pos);
    size_t GetPosition();
    inline bool IsEOF() const { return (m_file ? m_file->eof() : false); }
    void Flush();

    inline const std::string& GetPath() const { return m_path; }

    static bool Delete(const std::string &f_path);
    static bool Rename(const std::string &f_old, const std::string &f_new);
protected:
    File();
    ~File();
    bool Create(const std::string &f_path);
    bool Open(const std::string &f_path, bool f_ro);

    friend class ElementManager;
};

}
