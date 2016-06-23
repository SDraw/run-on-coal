#include "stdafx.h"
#include "classes/Animation.h"
#include "classes/Frame.h"
#include "classes/Utils.h"

Animation::Animation()
{
    m_duration = 0U;
    m_fps = 0U;
    m_bonesValue = 0U;
    m_loaded = false;
}
Animation::~Animation()
{
    for(auto iter : m_frameVector) delete iter;
    m_frameVector.clear();
}

void Animation::Clean()
{
    for(auto iter : m_frameVector) delete iter;
    m_duration = 0U;
    m_fps = 0U;
    m_bonesValue = 0U;
    m_loaded = false;
}
#define ReportError(first) { std::cout << first << std::endl; return false; }
#define ReportErrorAndClean(first) { std::cout << first << std::endl; Clean(); return false; }
bool Animation::LoadFromJSON(std::string &f_path)
{
    if(m_loaded) ReportError("Animation has already been loaded");
    std::string l_data;
    if(!Utils::ReadFile(f_path,l_data)) ReportError("Unable to read file " << f_path);
    if(!l_data.length()) ReportError("Empty file");
    sajson::document l_document = sajson::parse(sajson::literal(l_data.c_str()));
    if(l_document.get_error_line()) ReportError("JSON parsing error: " << l_document.get_error_message().c_str());

    sajson::value l_documentRoot = l_document.get_root();
    if(l_documentRoot.get_type() != sajson::TYPE_OBJECT) ReportError("JSON root isn't an object");

    size_t l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("animations"));
    if(l_nodeIndex == l_documentRoot.get_length()) ReportError("No animation node");
    sajson::value l_animContNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_animContNode.get_type() != sajson::TYPE_ARRAY) ReportError("Animation node isn't an array");
    if(l_animContNode.get_length() == 0) ReportError("Animation node is empty");
    sajson::value l_animNode = l_animContNode.get_array_element(0);
    if(l_animNode.get_type() != sajson::TYPE_OBJECT) ReportError("Animation subnode doesn't exist");

    l_nodeIndex = l_animNode.find_object_key(sajson::literal("length"));
    if(l_nodeIndex == l_animNode.get_length()) ReportError("No length node");
    sajson::value l_durationNode = l_animNode.get_object_value(l_nodeIndex);
    if(l_durationNode.get_type() != sajson::TYPE_INTEGER) ReportError("Length node isn't an integer value");
    m_duration = l_durationNode.get_integer_value()+1;
    std::cout << "Animation duration is " << m_duration << " frames" << std::endl;
    if(m_duration < 1) ReportError("Animation duration less than one frame");

    l_nodeIndex = l_animNode.find_object_key(sajson::literal("fps"));
    if(l_nodeIndex == l_animNode.get_length()) ReportError("No FPS node");
    sajson::value l_fpsNode = l_animNode.get_object_value(l_nodeIndex);
    if(l_fpsNode.get_type() != sajson::TYPE_INTEGER) ReportError("FPS node isn't an integer value");
    m_fps = l_fpsNode.get_integer_value();
    std::cout << "FPS is " << m_fps << std::endl;
    if(m_fps < 1) ReportError("FPS is lower than one frame");

    l_nodeIndex = l_animNode.find_object_key(sajson::literal("hierarchy"));
    if(l_nodeIndex == l_animNode.get_length()) ReportError("No hierarchy node");
    sajson::value l_hierarchyNode = l_animNode.get_object_value(l_nodeIndex);
    if(l_hierarchyNode.get_type() != sajson::TYPE_ARRAY) ReportError("Hierarchy node isn't an array");
    m_bonesValue = l_hierarchyNode.get_length();
    std::cout << "Bones value is " << m_bonesValue << std::endl;
    if(m_bonesValue == 0) ReportError("Bones array is empty");
    for(size_t i=0; i < m_duration; i++)
    {
        Frame *l_frame = new Frame();
        m_frameVector.push_back(l_frame);
    }
    for(size_t i=0; i < m_frameVector.size(); i++)
    {
        for(size_t j=0; j < m_bonesValue; j++)
        {
            m_frameVector[i]->m_position.push_back(glm::vec3(0.f));
            m_frameVector[i]->m_rotation.push_back(glm::quat(1.f,0.f,0.f,0.f));
            m_frameVector[i]->m_scale.push_back(glm::vec3(1.f));
        }
    }
    for(size_t i=0; i < m_bonesValue; i++)
    {
        sajson::value l_boneNode = l_hierarchyNode.get_array_element(i);
        if(l_boneNode.get_type() != sajson::TYPE_OBJECT) ReportErrorAndClean("Bone " << i << " node isn't an object");
        if(l_boneNode.get_length() == 0) ReportErrorAndClean("Bone " << i << " node is empty");
        l_nodeIndex = l_boneNode.find_object_key(sajson::literal("keys"));
        if(l_nodeIndex == l_boneNode.get_length()) ReportErrorAndClean("Bone " << i << " keys node doesn't exist");
        sajson::value l_dataNode = l_boneNode.get_object_value(l_nodeIndex);
        if(l_dataNode.get_type() != sajson::TYPE_ARRAY) ReportErrorAndClean("Bone " << i << " keys node isn't an array");
        if(l_dataNode.get_length() != m_duration) ReportErrorAndClean("Bone " << i << " keys value isn't equal to animation duration");
        for(size_t j=0; j < m_duration; j++)
        {
            sajson::value b_node = l_dataNode.get_array_element(j);
            if(b_node.get_type() != sajson::TYPE_OBJECT) ReportErrorAndClean("Bone " << i << " frame " << j << " isn't an object");
            if(b_node.get_length() == 0) ReportErrorAndClean("Bone " << i << " frame " << j << " is empty");
            l_nodeIndex = b_node.find_object_key(sajson::literal("pos"));
            if(l_nodeIndex == b_node.get_length()) ReportErrorAndClean("Bone " << i << " frame " << j << " position node doesn't exist");
            sajson::value l_node1 = b_node.get_object_value(l_nodeIndex);
            if(l_node1.get_type() != sajson::TYPE_ARRAY) ReportErrorAndClean("Bone " << i << " frame " << j << " position node isn't an array");
            if(l_node1.get_length() != 3) ReportErrorAndClean("Bone " << i << " frame " << j << " position node size isn't equal 3");

            std::vector<float> l_vectorData;

            for(size_t k=0; k < 3; k++)
            {
                sajson::value l_valueNode = l_node1.get_array_element(k);
                size_t l_nodeType = l_valueNode.get_type();
                if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) ReportErrorAndClean("Bone " << i << " frame " << j << " position node value " << k << " isn't a number");
                l_vectorData.push_back(float(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value():l_valueNode.get_double_value()));
            }
            m_frameVector[j]->m_position[i] = glm::vec3(l_vectorData[0],l_vectorData[1],l_vectorData[2]);
            l_vectorData.clear();

            l_nodeIndex = b_node.find_object_key(sajson::literal("rot"));
            if(l_nodeIndex == b_node.get_length()) ReportErrorAndClean("Bone " << i << " frame " << j << " rotation node doesn't exist");
            sajson::value l_node2 = b_node.get_object_value(l_nodeIndex);
            if(l_node2.get_type() != sajson::TYPE_ARRAY) ReportErrorAndClean("Bone " << i << " frame " << j << " rotation node isn't an array");
            if(l_node2.get_length() != 4) ReportErrorAndClean("Bone " << i << " frame " << j << " rotation node size isn't equal 4");
            for(size_t k=0; k < 4; k++)
            {
                sajson::value l_valueNode = l_node2.get_array_element(k);
                size_t l_nodeType = l_valueNode.get_type();
                if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) ReportErrorAndClean("Bone " << i << " frame " << j << " rotation node value " << k << " isn't a number");
                l_vectorData.push_back(float(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value():l_valueNode.get_double_value()));
            }
            m_frameVector[j]->m_rotation[i] = glm::quat(l_vectorData[3],l_vectorData[0],l_vectorData[1],l_vectorData[2]);
            l_vectorData.clear();

            l_nodeIndex = b_node.find_object_key(sajson::literal("scl"));
            if(l_nodeIndex == b_node.get_length()) ReportErrorAndClean("Bone " << i << " frame " << j << " scale node doesn't exist");
            sajson::value l_node3 = b_node.get_object_value(l_nodeIndex);
            if(l_node3.get_type() != sajson::TYPE_ARRAY) ReportErrorAndClean("Bone " << i << " frame " << j << " scale node isn't an array");
            if(l_node3.get_length() != 3) ReportErrorAndClean("Bone " << i << " frame " << j << " scale node size isn't equal 3");
            for(size_t k=0; k < 3; k++)
            {
                sajson::value l_valueNode = l_node3.get_array_element(k);
                size_t l_nodeType = l_valueNode.get_type();
                if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) ReportErrorAndClean("Bone " << i << " frame " << j << " sacle node value " << k << " isn't a number");
                l_vectorData.push_back(float(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value():l_valueNode.get_double_value()));
            }
            m_frameVector[j]->m_scale[i] = glm::vec3(l_vectorData[0],l_vectorData[1],l_vectorData[2]);
        }
    }
    std::cout << "Animation " << f_path.c_str() << " loaded successfully" << std::endl;
    m_loaded = true;
    return true;
}

bool Animation::GenerateBinary(std::string &f_path)
{
    if(!m_loaded)
    {
        std::cout << "Animation hasn't been loaded yet" << std::endl;
        return false;
    }
    std::ofstream l_file(f_path,std::ios::out|std::ios::binary);
    if(l_file.fail())
    {
        std::cout << "Unable to create file " << f_path.c_str() << std::endl;
        return false;
    }
    l_file.write((char*)&m_fps,sizeof(unsigned int));
    l_file.write((char*)&m_duration,sizeof(unsigned int));
    l_file.write((char*)&m_bonesValue,sizeof(unsigned int));
    for(size_t i=0, ii=m_frameVector.size(); i < ii; i++)
    {
        for(size_t j=0; j < m_bonesValue; j++)
        {
            boneData l_data;
            l_data.m_position = m_frameVector[i]->m_position[j];
            l_data.m_rotation = m_frameVector[i]->m_rotation[j];
            l_data.m_scale = m_frameVector[i]->m_scale[j];
            l_file.write((char*)&l_data,sizeof(boneData));
        }
    }
    l_file.flush();
    l_file.close();
    std::cout << "Animation successfully has been generated to " << f_path.c_str() << " file" << std::endl;
    return true;
}
