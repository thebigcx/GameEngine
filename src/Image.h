#pragma once

#include <string>

#include "util/maths/Vector2.h"

class Image {
    public:
        Image();
        ~Image();

        void loadFile(const std::string& p_file);

        unsigned char* getPixels() const;

        const Vector2u& getSize() const;

    private:
        unsigned char* m_data;

        Vector2u m_size;
};
