#version 460 core

in vec4 ocolor;

out vec4 FragColor;

void main()
{
    if (ocolor.a < 0.1)
    {
        //discard;
    }
    FragColor = ocolor;
    //FragColor = vec4(1, 1, 1, 1);
}