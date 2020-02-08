#include "stdafx.h"

#include "Elements/File.h"

ROC::File::File()
{
    m_elementType = ET_File;
    m_file = nullptr;
    m_type = FM_None;
}
ROC::File::~File()
{
    if(m_file)
    {
        m_file->close();
        delete m_file;
    }
}

bool ROC::File::Create(const std::string &f_path)
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
            m_type = FM_Write;
            m_path.assign(f_path);
        }
    }
    return (m_file != nullptr);
}
bool ROC::File::Open(const std::string &f_path, bool f_ro)
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
            m_type = f_ro ? FM_Read : FM_Write;
            m_path.assign(f_path);
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
        l_read = static_cast<size_t>(m_file->read(&l_data[0], f_lenght).gcount());
        if(l_read > 0U) f_data.assign(l_data.begin(), l_data.begin() + l_read);
    }
    return l_read;
}
size_t ROC::File::Write(const std::string &f_data)
{
    size_t l_written = 0U;
    if(m_file)
    {
        if(m_type == FM_Write)
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
        if(m_type == FM_Read)
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
    if(m_file) l_pos = static_cast<size_t>((m_type == FM_Read) ? m_file->tellg() : m_file->tellp());
    return l_pos;
}
bool ROC::File::SetPosition(size_t f_pos)
{
    bool l_result = false;
    if(m_file)
    {
        if(m_type == FM_Read) m_file->seekg(f_pos);
        else m_file->seekp(f_pos);
        l_result = !m_file->fail();
    }
    return l_result;
}

bool ROC::File::IsEOF() const
{
    return (m_file ? m_file->eof() : false);
}

void ROC::File::Flush()
{
    if(m_file) m_file->flush();
}

const std::string& ROC::File::GetPath() const
{
    return m_path;
}
