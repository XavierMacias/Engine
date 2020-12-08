#version 330
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv0;
layout(location = 2) in vec3 normals;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 uv0;
out vec3 fragment_normal;

void main()
{
 gl_Position = proj*view*model*vec4(vertex_position, 1.0);
 fragment_normal = transpose(inverse(mat3(model)))*normals;
 uv0 = vertex_uv0;
}

