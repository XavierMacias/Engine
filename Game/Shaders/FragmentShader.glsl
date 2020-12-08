#version 330 core
out vec4 color;

in vec2 uv0;
in vec3 fragment_normal;

uniform sampler2D mytexture;
uniform vec3 light_dir;
uniform vec3 view_dir;

void main()
{
 vec3 normal = normalize(fragment_normal);
 vec3 rRefl = reflect(light_dir,normal);
 float nDotL = max(dot(normal, light_dir), 0.0);
 vec3 diffuse_color = texture2D(mytexture, uv0).rgb;
	
 color.rgb = vec3(diffuse_color*nDotL);
 color.a = 1.0; 
 //color = texture2D(mytexture, uv0);
}