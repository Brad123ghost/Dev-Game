#version 330 core

in vec2 geomTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform int uDrawTexture;

void main()
{
    if (uDrawTexture == 1)
    {
        FragColor = texture(uTexture, geomTexCoord);
    }
    else
    {
        FragColor = vec4(1.0, 1.0, 1.0, 0.6);
    }
}