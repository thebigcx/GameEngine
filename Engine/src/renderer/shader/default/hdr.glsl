#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out DATA
{
    vec2 texCoord;
} vs_out;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0, 1);
    vs_out.texCoord = aTexCoord;
}

#shader fragment

out vec4 FragColor;

in DATA
{
    vec2 texCoord;
} fs_in;

uniform sampler2D Texture2D;
uniform float exposure = 1;

void main()
{   
    const float gamma = 2.2;
    vec3 hdrColor = texture(Texture2D, fs_in.texCoord).rgb;

    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}