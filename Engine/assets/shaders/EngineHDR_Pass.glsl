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

uniform sampler2D uColorBuffer;
uniform float uExposure = 1;

const float gamma = 2.2;

void main()
{   
    vec3 hdrColor = texture(uColorBuffer, fsInput.texCoord).rgb;

    vec3 mapped = vec3(1.0) - exp(-hdrColor * uExposure);

    mapped = pow(mapped, vec3(1.0 / gamma));

    color = vec4(mapped, 1.0);
}