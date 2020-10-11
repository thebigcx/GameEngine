#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        Shader(std::string vertexPath, std::string fragmentPath);

        void bind();

        void setUniform(std::string p_name, int p_value);
        void setUniform(std::string p_name, bool p_value);
        void setUniform(std::string p_name, float p_value);
        void setUniform(std::string p_name, glm::mat4 p_value);

        unsigned int getId() const;

    private:
        unsigned int m_id;
};