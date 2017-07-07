#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/compatibility.hpp"
#include "sajson.h"

#include "Animation.h"

Animation::Animation()
{
    m_duration = 0U;
    m_fps = 0U;
    m_bonesCount = 0U;
    m_loaded = false;
}
Animation::~Animation()
{
    m_bones.clear();
}

void Animation::Clean()
{
    m_bones.clear();
    m_duration = 0U;
    m_fps = 0U;
    m_bonesCount = 0U;
    m_loaded = false;
}

#define ReportError(T) { std::cout << ">>> " << T << std::endl; return false; }
#define ReportErrorAndClean(T) { std::cout << ">>> " T << std::endl; Clean(); return false; }
#define Info(T) std::cout << "> " << T << std::endl

bool Animation::Load(const std::string &f_path)
{
    if(m_loaded) ReportError("Animation has already been loaded");

    std::ifstream l_file;
    l_file.open(f_path);
    if(l_file.fail()) ReportError("Unable to open file");
    std::string l_data((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
    l_file.close();

    sajson::document l_document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(l_data.size(), const_cast<char*>(l_data.data())));
    size_t l_errorLine = l_document.get_error_line();
    if(l_errorLine) ReportError("JSON parsing error, line " << l_errorLine << ": " << l_document.get_error_message_as_string());

    sajson::value l_documentRoot = l_document.get_root();
    if(l_documentRoot.get_type() != sajson::TYPE_OBJECT) ReportError("JSON root isn't an object");

    size_t l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("animations"));
    if(l_nodeIndex == l_documentRoot.get_length()) ReportError("No animation node");
    sajson::value l_animContNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_animContNode.get_type() != sajson::TYPE_ARRAY) ReportError("Animation node isn't an array");

    size_t l_animCount = l_animContNode.get_length();
    if(l_animCount == 0U) ReportError("Animation node is empty");

    std::vector<std::pair<size_t, std::string>> l_animNamesVector;
    for(size_t i = 0; i < l_animCount; i++)
    {
        sajson::value l_animNode = l_animContNode.get_array_element(i);
        if(l_animNode.get_type() == sajson::TYPE_OBJECT)
        {
            size_t l_animNameIndex = l_animNode.find_object_key(sajson::literal("name"));
            if(l_animNameIndex != 0U)
            {
                sajson::value l_animNameValue = l_animNode.get_object_value(l_animNameIndex);
                if(l_animNameValue.get_type() == sajson::TYPE_STRING) l_animNamesVector.push_back(std::make_pair(i, l_animNameValue.as_string()));
            }
        }
    }

    Info("Available animations list:");
    for(auto &iter : l_animNamesVector) Info(iter.first << ". " << iter.second);
    Info("Enter animation index");
    std::cout << "> ";

    size_t l_selectedAnim = 0U;
    std::cin >> l_selectedAnim;
    bool l_selectionCheck = false;
    for(auto &iter : l_animNamesVector)
    {
        if(iter.first == l_selectedAnim) l_selectionCheck = true;
    }
    if(!l_selectionCheck) ReportError("Invalid animation list index");

    sajson::value l_animNode = l_animContNode.get_array_element(l_selectedAnim);
    l_nodeIndex = l_animNode.find_object_key(sajson::literal("fps"));
    if(l_nodeIndex == l_animNode.get_length()) ReportError("No FPS node");
    sajson::value l_fpsNode = l_animNode.get_object_value(l_nodeIndex);
    if(l_fpsNode.get_type() != sajson::TYPE_INTEGER) ReportError("FPS node isn't an integer value");
    m_fps = l_fpsNode.get_integer_value();
    Info("FPS is " << m_fps);
    if(m_fps == 0U) ReportError("FPS is invalid");

    l_nodeIndex = l_animNode.find_object_key(sajson::literal("length"));
    if(l_nodeIndex == l_animNode.get_length()) ReportError("No length node");
    sajson::value l_durationNode = l_animNode.get_object_value(l_nodeIndex);
    if(l_durationNode.get_type() != sajson::TYPE_INTEGER && l_durationNode.get_type() != sajson::TYPE_DOUBLE) ReportError("Length node isn't a number value");
    m_duration = static_cast<unsigned int>((l_durationNode.get_type() == sajson::TYPE_INTEGER) ? l_durationNode.get_integer_value()*m_fps : l_durationNode.get_double_value()*static_cast<double>(m_fps))+1U;
    Info("Animation duration is " << m_duration << " frames");
    if(m_duration == 0U) ReportError("Animation duration is invalid");

    l_nodeIndex = l_animNode.find_object_key(sajson::literal("hierarchy"));
    if(l_nodeIndex == l_animNode.get_length()) ReportError("No hierarchy node");
    sajson::value l_hierarchyNode = l_animNode.get_object_value(l_nodeIndex);
    if(l_hierarchyNode.get_type() != sajson::TYPE_ARRAY) ReportError("Hierarchy node isn't an array");
    m_bonesCount = l_hierarchyNode.get_length();
    Info("Bones count is " << m_bonesCount);
    if(m_bonesCount == 0U) ReportError("Bones array is empty");
    m_bones.resize(m_bonesCount);

    for(unsigned int i = 0; i < m_bonesCount; i++)
    {
        sajson::value l_boneNode = l_hierarchyNode.get_array_element(i);
        if(l_boneNode.get_type() != sajson::TYPE_OBJECT) ReportErrorAndClean("Bone " << i << " node isn't an object");
        if(l_boneNode.get_length() == 0) ReportErrorAndClean("Bone " << i << " node is empty");
        l_nodeIndex = l_boneNode.find_object_key(sajson::literal("keys"));
        if(l_nodeIndex == l_boneNode.get_length()) ReportErrorAndClean("Bone " << i << " keys node doesn't exist");
        sajson::value l_dataNode = l_boneNode.get_object_value(l_nodeIndex);
        if(l_dataNode.get_type() != sajson::TYPE_ARRAY) ReportErrorAndClean("Bone " << i << " keys node isn't an array");

        Info("Bone " << i << ", " << l_dataNode.get_length() << " keyframes ");

        keyframeData l_previousKeyframe;
        for(size_t j = 0, jj = l_dataNode.get_length(); j < jj; j++)
        {
            keyframeData l_keyframeData;
            sajson::value b_node = l_dataNode.get_array_element(j);
            if(b_node.get_type() != sajson::TYPE_OBJECT) ReportErrorAndClean("Bone " << i << " frame " << j << " isn't an object");
            if(b_node.get_length() == 0) ReportErrorAndClean("Bone " << i << " frame " << j << " is empty");
            l_nodeIndex = b_node.find_object_key(sajson::literal("pos"));
            if(l_nodeIndex != b_node.get_length())
            {
                sajson::value l_node1 = b_node.get_object_value(l_nodeIndex);
                if(l_node1.get_type() != sajson::TYPE_ARRAY) ReportErrorAndClean("Bone " << i << " frame " << j << " position node isn't an array");
                if(l_node1.get_length() != 3) ReportErrorAndClean("Bone " << i << " frame " << j << " position node size isn't equal 3");

                for(size_t k = 0; k < 3; k++)
                {
                    sajson::value l_valueNode = l_node1.get_array_element(k);
                    size_t l_nodeType = l_valueNode.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) ReportErrorAndClean("Bone " << i << " frame " << j << " position node value " << k << " isn't a number");
                    l_keyframeData.m_position[k] = float(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value() : l_valueNode.get_double_value());
                }
            }
            else
            {
                if(j == 0U) { ReportErrorAndClean("Bone " << i << " frame " << j << " position node doesn't exist"); }
                else l_keyframeData.m_position = l_previousKeyframe.m_position;
            }

            l_nodeIndex = b_node.find_object_key(sajson::literal("rot"));
            if(l_nodeIndex != b_node.get_length())
            {
                sajson::value l_node2 = b_node.get_object_value(l_nodeIndex);
                if(l_node2.get_type() != sajson::TYPE_ARRAY) ReportErrorAndClean("Bone " << i << " frame " << j << " rotation node isn't an array");
                if(l_node2.get_length() != 4) ReportErrorAndClean("Bone " << i << " frame " << j << " rotation node size isn't equal 4");
                for(size_t k = 0; k < 4; k++)
                {
                    sajson::value l_valueNode = l_node2.get_array_element(k);
                    size_t l_nodeType = l_valueNode.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) ReportErrorAndClean("Bone " << i << " frame " << j << " rotation node value " << k << " isn't a number");
                    l_keyframeData.m_rotation[k] = float(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value() : l_valueNode.get_double_value());
                }
            }
            else
            {
                if(j == 0U) { ReportErrorAndClean("Bone " << i << " frame " << j << " rotation node doesn't exist"); }
                else l_keyframeData.m_rotation = l_previousKeyframe.m_rotation;
            }

            l_nodeIndex = b_node.find_object_key(sajson::literal("scl"));
            if(l_nodeIndex != b_node.get_length())
            {
                sajson::value l_node3 = b_node.get_object_value(l_nodeIndex);
                if(l_node3.get_type() != sajson::TYPE_ARRAY) ReportErrorAndClean("Bone " << i << " frame " << j << " scale node isn't an array");
                if(l_node3.get_length() != 3) ReportErrorAndClean("Bone " << i << " frame " << j << " scale node size isn't equal 3");
                for(size_t k = 0; k < 3; k++)
                {
                    sajson::value l_valueNode = l_node3.get_array_element(k);
                    size_t l_nodeType = l_valueNode.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) ReportErrorAndClean("Bone " << i << " frame " << j << " scale node value " << k << " isn't a number");
                    l_keyframeData.m_scale[k] = float(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value() : l_valueNode.get_double_value());
                }
            }
            else
            {
                if(j == 0U) { ReportErrorAndClean("Bone " << i << " frame " << j << " scale node doesn't exist"); }
                else l_keyframeData.m_scale = l_previousKeyframe.m_scale;
            }

            l_nodeIndex = b_node.find_object_key(sajson::literal("time"));
            if(l_nodeIndex == b_node.get_length()) ReportErrorAndClean("Bone " << i << " frame " << j << " time node doesn't exist");
            sajson::value l_node4 = b_node.get_object_value(l_nodeIndex);
            if(l_node4.get_type() != sajson::TYPE_INTEGER && l_node4.get_type() != sajson::TYPE_DOUBLE) ReportErrorAndClean("Bone " << i << " frame " << j << " time node type isn't a number");
            l_keyframeData.m_frameIndex = int(l_node4.get_type() == sajson::TYPE_INTEGER ? l_node4.get_integer_value()*m_fps : float(l_node4.get_double_value())*float(m_fps));

            m_bones[i].m_keyframes.push_back(l_keyframeData);
            l_previousKeyframe = l_keyframeData;
        }
    }
    Info("Animation " << f_path << " loaded successfully");
    m_loaded = true;
    return true;
}

bool Animation::Generate(const std::string &f_path)
{
    bool l_result = false;
    if(m_loaded)
    {
        std::ofstream l_file(f_path, std::ios::out | std::ios::binary);
        if(!l_file.fail())
        {
            l_file.write(reinterpret_cast<char*>(&m_fps), sizeof(unsigned int));
            l_file.write(reinterpret_cast<char*>(&m_duration), sizeof(unsigned int));
            l_file.write(reinterpret_cast<char*>(&m_bonesCount), sizeof(unsigned int));
            for(auto &iter : m_bones)
            {
                int l_keyframesCount = iter.m_keyframes.size();
                l_file.write(reinterpret_cast<char*>(&l_keyframesCount), sizeof(int));
                for(auto &iter1 : iter.m_keyframes)
                {
                    l_file.write(reinterpret_cast<char*>(&iter1.m_position), sizeof(glm::vec3));
                    l_file.write(reinterpret_cast<char*>(&iter1.m_rotation), sizeof(glm::quat));
                    l_file.write(reinterpret_cast<char*>(&iter1.m_scale), sizeof(glm::vec3));
                    l_file.write(reinterpret_cast<char*>(&iter1.m_frameIndex), sizeof(int));
                }
            }
            l_file.flush();
            l_file.close();

            Info("Animation has been converted to " << f_path);
            l_result = true;
        }
        else Info("Unable to create file " << f_path);
    }
    else Info("Animation hasn't been loaded yet");
    return l_result;
}
