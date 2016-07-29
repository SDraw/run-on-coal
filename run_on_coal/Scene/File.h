#pragma once

namespace ROC
{

class File
{
    std::fstream *m_file;
    std::string m_path;
    enum FileMode { NoneMode = -1, ReadMode, WriteMode };
    FileMode m_type;
public:
    size_t Read(std::string &f_data, size_t f_lenght);
    size_t Write(std::string &f_data);
    size_t GetSize();
    bool SetPosition(size_t f_pos);
    inline size_t GetPosition() { return static_cast<size_t>((m_type == FileMode::ReadMode) ? m_file->tellg() : m_file->tellp()); }
    inline void GetPath(std::string &f_string) { f_string.append(m_path); }
    inline bool IsEOF() { return m_file->eof(); }
    static bool Delete(Core *f_core, std::string &f_path);
    static bool Rename(Core *f_core, std::string &f_old, std::string &f_new);
protected:
    File();
    ~File();
    bool Create(std::string &f_path, std::string &f_rPath);
    bool Open(std::string &f_path, std::string &f_rPath, bool f_ro);
    friend class ElementManager;
};

}
