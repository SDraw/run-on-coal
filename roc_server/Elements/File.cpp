#include "stdafx.h"
#include "Core/Core.h"
#include "Elements/File.h"
#include "Utils/Utils.h"

ROC::File::File()
{
    m_elementType = ElementType::FileElement;
    m_file = NULL;
}
ROC::File::~File()
{
    if(m_file)
    {
        m_file->close();
        delete m_file;
    }
}

bool ROC::File::Create(const std::string &f_path, const std::string &f_rPath)
{
    m_file = new std::fstream(f_path, std::ios::out | std::ios::binary);
    if(m_file->fail()) return false;
    m_type = FileMode::WriteMode;
    m_path.assign(f_rPath);
    return true;
}
bool ROC::File::Open(const std::string &f_path, const std::string &f_rPath, bool f_ro)
{
    m_file = new std::fstream(f_path, (f_ro ? std::ios::in : std::ios::out) | std::ios::binary);
    if(m_file->fail()) return false;
    m_type = f_ro ? FileMode::ReadMode : FileMode::WriteMode;
    m_path.assign(f_rPath);
    return true;
}

size_t ROC::File::Read(std::string &f_data, size_t f_lenght)
{
    std::string l_data;
    l_data.resize(f_lenght);
    size_t l_read = static_cast<size_t>(m_file->read(const_cast<char*>(l_data.data()), f_lenght).gcount());
    f_data.insert(f_data.begin(), l_data.begin(), l_data.begin() + l_read);
    return l_read;
}
size_t ROC::File::Write(const std::string &f_data)
{
    if(m_type != FileMode::WriteMode) return 0U;
    std::streampos l_start = m_file->tellg();
    m_file->write(&f_data[0], f_data.size());
    if(m_file->fail()) return 0U;
    l_start = m_file->tellg() - l_start;
    return static_cast<size_t>(l_start);
}

size_t ROC::File::GetSize()
{
    size_t l_size = 0U;
    if(m_type == FileMode::ReadMode)
    {
        std::streampos l_last = m_file->tellg();
        m_file->seekg(SEEK_SET, std::ios::end);
        l_size = static_cast<size_t>(m_file->tellg());
        m_file->seekg(l_last);
    }
    else
    {
        std::streampos l_last = m_file->tellp();
        m_file->seekp(SEEK_SET, std::ios::end);
        l_size = static_cast<size_t>(m_file->tellp());
        m_file->seekp(l_last);
    }
    return l_size;
}
bool ROC::File::SetPosition(size_t f_pos)
{
    if(m_type == FileMode::ReadMode) m_file->seekg(f_pos);
    else m_file->seekp(f_pos);
    return !m_file->fail();
}

bool ROC::File::Delete(Core *f_core, std::string &f_path)
{
    std::string l_work, l_path;
    f_core->GetWorkingDirectory(l_work);
    Utils::AnalyzePath(f_path, l_path);
    Utils::JoinPaths(l_work, l_path);
    return !std::remove(l_work.c_str());
}
bool ROC::File::Rename(Core *f_core, std::string &f_old, std::string &f_new)
{
    std::string l_workOld, l_pathOld, l_workNew, l_pathNew;

    f_core->GetWorkingDirectory(l_workOld);
    Utils::AnalyzePath(f_old, l_pathOld);
    Utils::JoinPaths(l_workOld, l_pathOld);

    f_core->GetWorkingDirectory(l_workNew);
    Utils::AnalyzePath(f_new, l_pathNew);
    Utils::JoinPaths(l_workNew, l_pathNew);

    return !std::rename(l_workOld.c_str(), l_workNew.c_str());
}
