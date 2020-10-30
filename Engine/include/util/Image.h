#pragma once

#include <string>

#include <math/vector/Vector2.h>

class Image {
    public:
        Image();
        ~Image();

        void loadFile(const std::string& p_file);

        unsigned char* getPixels() const;

        const Vector2u& getSize() const;

        void setVerticalFlip(bool flip = true);

        int getChannels() const;

    private:
        unsigned char* m_data;

        Vector2u m_size;

        bool m_flipped = false;

        int m_channels = 0;
};
