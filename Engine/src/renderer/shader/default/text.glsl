#shader vertex

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out DATA
{
    vec2 texCoord;
} vs_out;

layout (std140, binding = 2) uniform matrices
{
    mat4 projection;
    mat4 transform;
};

void main()
{
    vs_out.texCoord = aTexCoord;
    gl_Position = projection * transform * vec4(aPos, 0.0, 1.0);
}

#shader fragment

in DATA
{
    vec2 texCoord;
} fs_in;

out vec4 fragColor;

uniform sampler2D fontAtlas;
uniform vec4 textColor;

void main()
{
    vec4 sampled = vec4(1, 1, 1, texture(fontAtlas, fs_in.texCoord).r);
    if (sampled.a < 0.1)
    {
        discard;
    }
    fragColor = sampled * vec4(textColor.rgb, 1);
}