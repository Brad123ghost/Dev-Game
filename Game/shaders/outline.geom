#version 330
layout(triangles) in;
layout(line_strip, max_vertices = 64) out;

in vec2 fragTexCoord[];
out vec2 geomTexCoord;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    geomTexCoord = fragTexCoord[0];
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    geomTexCoord = fragTexCoord[1];
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[1].gl_Position;
    geomTexCoord = fragTexCoord[1];
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    geomTexCoord = fragTexCoord[2];
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[2].gl_Position;
    geomTexCoord = fragTexCoord[2];
    EmitVertex();
    gl_Position = gl_in[0].gl_Position;
    geomTexCoord = fragTexCoord[0];
    EmitVertex();
    EndPrimitive();
}