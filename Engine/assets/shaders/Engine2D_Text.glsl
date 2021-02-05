#shader vertex

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out Params
{
    vec2 texCoord;
} vsOutput;

layout (std140, binding = 2) uniform matrices
{
    mat4 uProjection;
    mat4 uTransform;
};

void main()
{
    vsOutput.texCoord = aTexCoord;
    gl_Position = uProjection * uTransform * vec4(aPos, 0.0, 1.0);
}

#shader fragment

in Params
{
    vec2 texCoord;
} fsInput;

layout (location = 0) out vec4 color;

uniform sampler2D uFontAtlas;
uniform vec4 uTextColor;

void main()
{
    vec4 sampled = vec4(1, 1, 1, texture(uFontAtlas, fsInput.texCoord).r);
    if (sampled.a < 0.1)
    {
        discard;
    }
    color = sampled * vec4(uTextColor.rgb, 1);
}