#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aTexIndex;

out Params
{
    vec2 texCoord;
    vec4 color;
    flat float texIndex;
} vsOutput;

layout (std140, binding = 2) uniform matrices
{
    mat4 uProjection;
    mat4 uTransform;
};

void main()
{
    vsOutput.texCoord = aTexCoord;
    vsOutput.color = aColor;
    vsOutput.texIndex = aTexIndex;
    gl_Position = uProjection * uTransform * vec4(aPos, 1.0);
}

#shader fragment
#define MAX_TEXTURE_SLOTS 32

in Params
{
    vec2 texCoord;
    vec4 color;
    float texIndex;
} fsInput;

layout (location = 0) out vec4 color;

uniform sampler2D uTextures[MAX_TEXTURE_SLOTS];

void main()
{
    color = fsInput.color;
    for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
    {
        if (i == int(fsInput.texIndex))
        {
            color *= texture(uTextures[i], fsInput.texCoord);
            break;
        }
    }

    if (color.a < 0.1)
    {
        discard;
    }
}
