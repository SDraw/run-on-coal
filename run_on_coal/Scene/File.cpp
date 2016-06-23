#include "stdafx.h"
#include "Core/Core.h"
#include "Scene/File.h"

ROC::File::File()
{
    m_file = NULL;
    m_type = 0U;
}
ROC::File::~File()
{
    if(m_file) delete m_file;
}

bool ROC::File::Create(std::string &f_path, std::string &f_rPath)
{
    m_file = new std::fstream(f_path,std::ios::out|std::ios::binary);
    if(m_file->fail()) return false;
    m_type = Mode::WriteMode;
    m_path.append(f_rPath);
    return true;
}
bool ROC::File::Open(std::string &f_path, std::string &f_rPath, bool f_ro)
{
    m_file = new std::fstream(f_path,(f_ro ? std::ios::in : std::ios::out) | std::ios::binary);
    if(m_file->fail()) return false;
    m_type = f_ro ? Mode::ReadMode : Mode::WriteMode;
    m_path.append(f_rPath);
    return true;
}
size_t ROC::File::Read(std::string &f_data, size_t f_lenght)
{
    std::vector<char> l_data(f_lenght);
    size_t l_read = unsigned(m_file->read(&l_data[0],f_lenght).gcount());
    f_data.append(l_data.begin(),l_data.begin()+l_read);
    return l_read;
}
size_t ROC::File::Write(std::string &f_data)
{
    if(m_type != Mode::WriteMode) return 0U;
    std::streampos l_start = m_file->tellg();
    m_file->write(&f_data[0],f_data.size());
    if(m_file->fail()) return 0U;
    l_start = m_file->tellg()-l_start;
    return size_t(l_start);
}
size_t ROC::File::GetSize()
{
    size_t l_size = 0U;
    if(m_type == Mode::ReadMode)
    {
        std::streampos l_last = m_file->tellg();
        m_file->seekg(SEEK_SET,std::ios::end);
        l_size = unsigned(m_file->tellg());
        m_file->seekg(l_last);
    }
    else
    {
        std::streampos l_last = m_file->tellp();
        m_file->seekp(SEEK_SET,std::ios::end);
        l_size = unsigned(m_file->tellp());
        m_file->seekp(l_last);
    }
    return l_size;
}

bool ROC::File::SetPosition(size_t f_pos)
{
    if(m_type == Mode::ReadMode) m_file->seekg(SEEK_SET,f_pos);
    else m_file->seekp(SEEK_SET,f_pos);
    return !m_file->fail();
}
size_t ROC::File::GetPosition()
{
    size_t l_pos = (m_type == Mode::ReadMode) ? unsigned(m_file->tellg()) : unsigned(m_file->tellp());
    return l_pos;
}

void ROC::File::GetPath(std::string &f_string)
{
    f_string.append(m_path);
}

bool ROC::File::IsEOF()
{
    return m_file->eof();
}

bool ROC::File::Delete(std::string &f_path)
{
    std::string l_path;
    Core::GetCore()->GetWorkingDirectory(l_path);
    l_path.append("/");
    l_path.append(f_path);
    return !std::remove(l_path.c_str());
}

bool ROC::File::Rename(std::string &f_old, std::string &f_new)
{
    std::string l_oPath,l_nPath;
    Core::GetCore()->GetWorkingDirectory(l_oPath);
    l_nPath.append(l_oPath);
    l_nPath.append("/");
    l_nPath.append(f_new);
    l_oPath.append("/");
    l_oPath.append(f_old);
    return !std::rename(l_oPath.c_str(),l_nPath.c_str());
}
