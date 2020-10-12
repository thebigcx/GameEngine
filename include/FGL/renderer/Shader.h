#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
};

class Shader {
    public:
        Shader(const std::string& vsPath, const std::string& fsPath);
        ~Shader();

        void bind() const;

        void setUniform(std::string p_name, int p_value);
        void setUniform(std::string p_name, bool p_value);
        void setUniform(std::string p_name, float p_value);
        void setUniform(std::string p_name, glm::mat4 p_value);

        unsigned int getId() const;

    private:
        unsigned int m_id;

        static ShaderSource parseShader(const std::string& vsPath, const std::string& fsPath);

        bool compileShader(const ShaderSource& source);
};