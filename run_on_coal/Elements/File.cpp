#include "stdafx.h"

#include "Elements/File.h"
#include "Core/Core.h"

#include "Utils/Utils.h"

ROC::File::File()
{
    m_elementType = ElementType::FileElement;
    m_file = nullptr;
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
    if(!m_file)
    {
        m_file = new std::fstream(f_path, std::ios::out | std::ios::binary);
        if(m_file->fail())
        {
            delete m_file;
            m_file = nullptr;
        }
        else
        {
            m_type = FileMode::WriteMode;
            m_path.assign(f_rPath);
        }
    }
    return (m_file != nullptr);
}
bool ROC::File::Open(const std::string &f_path, const std::string &f_rPath, bool f_ro)
{
    if(!m_file)
    {
        m_file = new std::fstream(f_path, (f_ro ? std::ios::in : std::ios::out) | std::ios::binary);
        if(m_file->fail())
        {
            delete m_file;
            m_file = nullptr;
        }
        else
        {
            m_type = f_ro ? FileMode::ReadMode : FileMode::WriteMode;
            m_path.assign(f_rPath);
        }
    }
    return (m_file != nullptr);
}

size_t ROC::File::Read(std::string &f_data, size_t f_lenght)
{
    size_t l_read = 0U;
    if(m_file)
    {
        std::string l_data;
        l_data.resize(f_lenght);
        l_read = static_cast<size_t>(m_file->read(const_cast<char*>(l_data.data()), f_lenght).gcount());
        if(l_read > 0U) f_data.assign(l_data.begin(), l_data.begin() + l_read);
    }
    return l_read;
}
size_t ROC::File::Write(const std::string &f_data)
{
    size_t l_written = 0U;
    if(m_file)
    {
        if(m_type == FileMode::WriteMode)
        {
            std::streampos l_start = m_file->tellg();
            m_file->write(&f_data[0], f_data.size());
            if(!m_file->fail()) l_written = static_cast<size_t>(m_file->tellg() - l_start);
        }
    }
    return l_written;
}

size_t ROC::File::GetSize()
{
    size_t l_size = 0U;
    if(m_file)
    {
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
    }
    return l_size;
}
size_t ROC::File::GetPosition()
{
    size_t l_pos = 0U;
    if(m_file) l_pos = static_cast<size_t>((m_type == FileMode::ReadMode) ? m_file->tellg() : m_file->tellp());
    return l_pos;
}
bool ROC::File::SetPosition(size_t f_pos)
{
    bool l_result = false;
    if(m_file)
    {
        if(m_type == FileMode::ReadMode) m_file->seekg(f_pos);
        else m_file->seekp(f_pos);
        l_result = !m_file->fail();
    }
    return l_result;
}

void ROC::File::Flush()
{
    if(m_file) m_file->flush();
}

bool ROC::File::Delete(Core *f_core, const std::string &f_path)
{
    std::string l_work, l_path(f_path);
    f_core->GetWorkingDirectory(l_work);
    Utils::Path::EscapePath(l_path);
    l_path.insert(0U, l_work);
    return !std::remove(l_work.c_str());
}
bool ROC::File::Rename(Core *f_core, const std::string &f_old, const std::string &f_new)
{
    std::string l_work, l_pathOld(f_old), l_pathNew(f_new);

    f_core->GetWorkingDirectory(l_work);
    Utils::Path::EscapePath(l_pathOld);
    l_pathOld.insert(0U, l_work);
    Utils::Path::EscapePath(l_pathNew);
    l_pathNew.insert(0U, l_work);

    return !std::rename(l_pathOld.c_str(), l_pathNew.c_str());
}
