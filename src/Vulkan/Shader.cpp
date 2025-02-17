#include "Shader.h"

#include <fstream>
#include <sstream>

#include "Device.h"
#include "../utils/VulkanCheckResult.h"

void Shader::Init(const std::string& path, ShaderType type)
{
    m_ShaderType = type;
    VkShaderModuleCreateInfo shaderCI{};
    shaderCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    std::vector<char> byteCode = read_shader_byte_code(path);
    shaderCI.codeSize = byteCode.size();
    shaderCI.pCode = reinterpret_cast<uint32_t*>(byteCode.data());

    if(VK_CALL(vkCreateShaderModule(VKDEVICE, &shaderCI, nullptr, &m_Module)))
        throw std::runtime_error("Cannot Create shader module!");
    else VK_LOG(get_shader_type_string(), "shader module created!");

    // Create the pipeline shader stage create info
    m_ShaderStageInfo = {};
    m_ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    switch (type) {
        case ShaderType::VERTEX_SHADER :
            m_ShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case ShaderType::FRAGMENT_SHADER :
            m_ShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        case ShaderType::COMPUTE_SHADER:
            m_ShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
            break;
        case ShaderType::MESH_SHADER:
            m_ShaderStageInfo.stage = VK_SHADER_STAGE_MESH_BIT_NV;
            break;
        case ShaderType::TASK_SHADER:
            m_ShaderStageInfo.stage = VK_SHADER_STAGE_TASK_BIT_NV;
            break;
        case ShaderType::TESSELATION_CONTROL_SHADER:
            m_ShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            break;
        case ShaderType::TESSELATION_EVALUATION_SHADER:
            m_ShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            break;
    }
    m_ShaderStageInfo.module = m_Module;
    m_ShaderStageInfo.pName = "main";
    m_ShaderStageInfo.pSpecializationInfo = nullptr;
}

void Shader::Destroy()
{
    vkDestroyShaderModule(VKDEVICE, m_Module, nullptr);
}

std::string Shader::get_shader_type_string()
{
    switch (m_ShaderType) {
        case ShaderType::VERTEX_SHADER :
            return "Vertex";
            break;
        case ShaderType::FRAGMENT_SHADER :
            return "Fragment";
            break;
        case ShaderType::COMPUTE_SHADER:
            return "Compute";
            break;
        case ShaderType::TASK_SHADER:
            return "Task";
            break;
        case ShaderType::MESH_SHADER:
            return "Mesh";
            break;
        case  ShaderType::TESSELATION_CONTROL_SHADER:
            return "Tesselation Control";
        case  ShaderType::TESSELATION_EVALUATION_SHADER:
            return "Tesselation Evaluation";
            break;
    }
}

std::vector<char> Shader::read_shader_byte_code(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    std::ostringstream error;
    error << "Cannot open the shader byte code file  " << "(" << filePath + ")";

    if(!file.is_open())
        throw std::runtime_error(error.str().c_str());

    size_t fileSize = file.tellg();
    file.seekg(0);
    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
