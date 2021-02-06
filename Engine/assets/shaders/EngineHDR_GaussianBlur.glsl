#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out Params
{
    vec2 texCoord;
} vsOutput;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0, 1);
    vsOutput.texCoord = aTexCoord;
}

#shader fragment

layout (location = 0) out vec4 color;

in Params
{
    vec2 texCoord;
} fsInput;

uniform sampler2D uImage;

uniform bool uHorizontal;
uniform float uWeights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 texOffset = 1.0 / textureSize(uImage, 0);
    vec3 result = texture(uImage, fsInput.texCoord).rgb * uWeights[0];

    if (uHorizontal)
    {
        for (int i = 1; i < 5; i++)
        {
            result += texture(uImage, fsInput.texCoord + vec2(texOffset.x * i, 0.0)).rgb * uWeights[i];
            result += texture(uImage, fsInput.texCoord - vec2(texOffset.x * i, 0.0)).rgb * uWeights[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; i++)
        {
            result += texture(uImage, fsInput.texCoord + vec2(0.0, texOffset.y * i)).rgb * uWeights[i];
            result += texture(uImage, fsInput.texCoord - vec2(0.0, texOffset.y * i)).rgb * uWeights[i];
        }
    }

    color = vec4(result, 1.0);
}