#include <renderer/ui/text/Glyph.h>

Glyph::Glyph(unsigned int texture, Vector2i size, Vector2i bearing, unsigned int advance)
: m_texture(texture)
, m_size(size)
, m_bearing(bearing)
, m_advance(advance)
{

}