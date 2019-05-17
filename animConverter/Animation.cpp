#include "stdafx.h"

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
    std::cout << "Cleaning ..." << std::endl;

    m_bones.clear();
    m_duration = 0U;
    m_fps = 0U;
    m_bonesCount = 0U;
    m_loaded = false;
}

bool Animation::Load(const std::string &f_path)
{
    if(!m_loaded)
    {
        try
        {
            std::ifstream l_file;
            l_file.open(f_path);
            if(l_file.fail()) throw std::exception("Unable to open file");

            std::string l_data((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
            l_file.close();

            sajson::document l_document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(l_data.size(), const_cast<char*>(l_data.data())));
            size_t l_errorLine = l_document.get_error_line();
            if(l_errorLine) throw std::exception("JSON parsing error");

            sajson::value l_documentRoot = l_document.get_root();
            if(l_documentRoot.get_type() != sajson::TYPE_OBJECT) throw std::exception("JSON root isn't an object");

            size_t l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("animations"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No animation node");
            sajson::value l_animContNode = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_animContNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Animation node isn't an array");

            size_t l_animCount = l_animContNode.get_length();
            if(l_animCount == 0U) throw std::exception("Animation node is empty");

            std::vector<std::pair<size_t, std::string>> l_animNamesVector;
            for(size_t i = 0U; i < l_animCount; i++)
            {
                sajson::value l_animNode = l_animContNode.get_array_element(i);
                if(l_animNode.get_type() == sajson::TYPE_OBJECT)
                {
                    size_t l_animNodeLength = l_animNode.get_length();
                    if(l_animNodeLength > 0U)
                    {
                        size_t l_animNameIndex = l_animNode.find_object_key(sajson::literal("name"));
                        if(l_animNameIndex != l_animNodeLength)
                        {
                            sajson::value l_animNameValue = l_animNode.get_object_value(l_animNameIndex);
                            if(l_animNameValue.get_type() == sajson::TYPE_STRING) l_animNamesVector.push_back(std::make_pair(i, l_animNameValue.as_string()));
                        }
                    }
                }
            }

            std::cout << "Available animations list:" << std::endl;
            for(auto &iter : l_animNamesVector) std::cout << iter.first << ". " << iter.second << std::endl;
            std::cout << "Enter animation index" << std::endl << "> ";

            size_t l_selectedAnim = 0U;
            std::cin >> l_selectedAnim;
            bool l_selectionCheck = false;
            for(auto &iter : l_animNamesVector)
            {
                if(iter.first == l_selectedAnim)
                {
                    l_selectionCheck = true;
                    break;
                }
            }
            if(!l_selectionCheck) throw std::exception("Invalid animation list index");

            sajson::value l_animNode = l_animContNode.get_array_element(l_selectedAnim);
            l_nodeIndex = l_animNode.find_object_key(sajson::literal("fps"));
            if(l_nodeIndex == l_animNode.get_length()) throw std::exception("No FPS node");
            sajson::value l_fpsNode = l_animNode.get_object_value(l_nodeIndex);
            if(l_fpsNode.get_type() != sajson::TYPE_INTEGER) throw std::exception("FPS node isn't an integer value");
            m_fps = l_fpsNode.get_integer_value();
            std::cout << "FPS is " << m_fps << std::endl;
            if(m_fps == 0U) throw std::exception("FPS is invalid");

            l_nodeIndex = l_animNode.find_object_key(sajson::literal("length"));
            if(l_nodeIndex == l_animNode.get_length()) throw std::exception("No length node");
            sajson::value l_durationNode = l_animNode.get_object_value(l_nodeIndex);
            if(l_durationNode.get_type() != sajson::TYPE_INTEGER && l_durationNode.get_type() != sajson::TYPE_DOUBLE) throw std::exception("Length node isn't a number value");
            m_duration = static_cast<unsigned int>((l_durationNode.get_type() == sajson::TYPE_INTEGER) ? l_durationNode.get_integer_value()*m_fps : l_durationNode.get_double_value()*static_cast<double>(m_fps)) + 1U;
            std::cout << "Animation duration is " << m_duration << " frames" << std::endl;
            if(m_duration == 0U) throw std::exception("Animation duration is invalid");

            l_nodeIndex = l_animNode.find_object_key(sajson::literal("hierarchy"));
            if(l_nodeIndex == l_animNode.get_length()) throw std::exception("No hierarchy node");
            sajson::value l_hierarchyNode = l_animNode.get_object_value(l_nodeIndex);
            if(l_hierarchyNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Hierarchy node isn't an array");
            m_bonesCount = static_cast<unsigned int>(l_hierarchyNode.get_length());
            std::cout << "Bones count is " << m_bonesCount << std::endl;
            if(m_bonesCount == 0U) throw std::exception("Bones array is empty");
            m_bones.resize(static_cast<size_t>(m_bonesCount));

            for(size_t i = 0U; i < static_cast<size_t>(m_bonesCount); i++)
            {
                std::cout << "Bone " << i << " parsing ..." << std::endl;
                sajson::value l_boneNode = l_hierarchyNode.get_array_element(i);
                if(l_boneNode.get_type() != sajson::TYPE_OBJECT) throw std::exception("Bone node isn't an object");
                if(l_boneNode.get_length() == 0U) throw std::exception("Bone node is empty");
                l_nodeIndex = l_boneNode.find_object_key(sajson::literal("keys"));
                if(l_nodeIndex == l_boneNode.get_length()) throw std::exception("Bone keys node doesn't exist");
                sajson::value l_dataNode = l_boneNode.get_object_value(l_nodeIndex);
                if(l_dataNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Bone keys node isn't an array");

                size_t l_dataNodeLength = l_dataNode.get_length();
                std::cout <<  l_dataNodeLength << " keyframes " << std::endl;
                if(l_dataNodeLength < 2U) throw std::exception("Bone has less than 2 keyframes");

                keyframeData l_previousKeyframe;
                for(size_t j = 0U; j < l_dataNodeLength; j++)
                {
                    std::cout << "Keyframe " << j << " parsing ..." << std::endl;
                    keyframeData l_keyframeData;
                    sajson::value b_node = l_dataNode.get_array_element(j);
                    if(b_node.get_type() != sajson::TYPE_OBJECT) throw std::exception("Bone frame isn't an object");
                    if(b_node.get_length() == 0U) throw std::exception("Bone frame is empty");
                    l_nodeIndex = b_node.find_object_key(sajson::literal("pos"));
                    if(l_nodeIndex != b_node.get_length())
                    {
                        sajson::value l_node1 = b_node.get_object_value(l_nodeIndex);
                        if(l_node1.get_type() != sajson::TYPE_ARRAY) throw std::exception("Bone frame position node isn't an array");
                        if(l_node1.get_length() != 3U) throw std::exception("Bone frame position node size isn't equal 3");

                        for(size_t k = 0U; k < 3U; k++)
                        {
                            sajson::value l_valueNode = l_node1.get_array_element(k);
                            size_t l_nodeType = l_valueNode.get_type();
                            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("Bone frame position node value isn't a number");
                            l_keyframeData.m_position[k] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value() : l_valueNode.get_double_value());
                        }
                    }
                    else
                    {
                        if(j == 0U) throw std::exception("Bone frame position node doesn't exist");
                        else l_keyframeData.m_position = l_previousKeyframe.m_position;
                    }

                    l_nodeIndex = b_node.find_object_key(sajson::literal("rot"));
                    if(l_nodeIndex != b_node.get_length())
                    {
                        sajson::value l_node2 = b_node.get_object_value(l_nodeIndex);
                        if(l_node2.get_type() != sajson::TYPE_ARRAY) throw std::exception("Bone frame rotation node isn't an array");
                        if(l_node2.get_length() != 4U) throw std::exception("Bone frame rotation node size isn't equal 4");
                        for(size_t k = 0U; k < 4U; k++)
                        {
                            sajson::value l_valueNode = l_node2.get_array_element(k);
                            size_t l_nodeType = l_valueNode.get_type();
                            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("Bone frame rotation node value isn't a number");
                            l_keyframeData.m_rotation[k] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value() : l_valueNode.get_double_value());
                        }
                    }
                    else
                    {
                        if(j == 0U) throw std::exception("Bone frame rotation node doesn't exist");
                        else l_keyframeData.m_rotation = l_previousKeyframe.m_rotation;
                    }

                    l_nodeIndex = b_node.find_object_key(sajson::literal("scl"));
                    if(l_nodeIndex != b_node.get_length())
                    {
                        sajson::value l_node3 = b_node.get_object_value(l_nodeIndex);
                        if(l_node3.get_type() != sajson::TYPE_ARRAY) throw std::exception("Bone frame scale node isn't an array");
                        if(l_node3.get_length() != 3U) throw std::exception("Bone frame scale node size isn't equal 3");
                        for(size_t k = 0U; k < 3U; k++)
                        {
                            sajson::value l_valueNode = l_node3.get_array_element(k);
                            size_t l_nodeType = l_valueNode.get_type();
                            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("Bone frame scale node value isn't a number");
                            l_keyframeData.m_scale[k] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_valueNode.get_integer_value() : l_valueNode.get_double_value());
                        }
                    }
                    else
                    {
                        if(j == 0U) throw std::exception("Bone frame scale node doesn't exist");
                        else l_keyframeData.m_scale = l_previousKeyframe.m_scale;
                    }

                    l_nodeIndex = b_node.find_object_key(sajson::literal("time"));
                    if(l_nodeIndex == b_node.get_length()) throw std::exception("Bone frame time node doesn't exist");
                    sajson::value l_node4 = b_node.get_object_value(l_nodeIndex);
                    if(l_node4.get_type() != sajson::TYPE_INTEGER && l_node4.get_type() != sajson::TYPE_DOUBLE) throw std::exception("Bone frame time node type isn't a number");
                    l_keyframeData.m_frameIndex = static_cast<int>(l_node4.get_type() == sajson::TYPE_INTEGER ? l_node4.get_integer_value()*m_fps : l_node4.get_double_value()*static_cast<double>(m_fps));

                    m_bones[i].m_keyframes.push_back(l_keyframeData);
                    l_previousKeyframe = l_keyframeData;
                }
            }
            std::cout << "Animation " << f_path << " loaded successfully" << std::endl;
            m_loaded = true;
        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            Clean();
        }
    }
    return m_loaded;
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
                int l_keyframesCount = static_cast<int>(iter.m_keyframes.size());
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

            std::cout << "Animation has been converted to " << f_path << std::endl;
            l_result = true;
        }
        else std::cout << "Unable to create file " << f_path << std::endl;
    }
    return l_result;
}
