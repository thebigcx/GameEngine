#shader vertex

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out Params
{
    vec2 texCoord;
} vsOutput;

void main()
{
    vsOutput.texCoord = aTexCoord;
    gl_Position = vec4(aPos.x, aPos.y, 0, 1);
}

#shader fragment

in Params
{
    vec2 texCoord;
} fsInput;

layout (location = 0) out vec4 color;

uniform sampler2D uColorBuffer;

void main()
{
    color = texture(uColorBuffer, fsInput.texCoord);
}