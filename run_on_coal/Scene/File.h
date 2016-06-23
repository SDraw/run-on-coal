#pragma once

namespace ROC
{

class File
{
    std::fstream *m_file;
    std::string m_path;
    unsigned char m_type;
    enum Mode { ReadMode = 1U, WriteMode };
public:
    size_t Read(std::string &f_data, size_t f_lenght);
    size_t Write(std::string &f_data);
    size_t GetSize();
    bool SetPosition(size_t f_pos);
    size_t GetPosition();
    void GetPath(std::string &f_string);
    bool IsEOF();
    static bool Delete(std::string &f_path);
    static bool Rename(std::string &f_old, std::string &f_new);
protected:
    File();
    ~File();
    bool Create(std::string &f_path, std::string &f_rPath);
    bool Open(std::string &f_path, std::string &f_rPath, bool f_ro);
    friend class ElementManager;
};

}
