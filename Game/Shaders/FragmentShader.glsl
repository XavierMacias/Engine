#version 330 core
out vec4 color;

in vec2 uv0;
in vec3 surface_position;
in vec3 fragment_normal;

uniform sampler2D mytexture;
uniform vec3 light_dir;
uniform vec3 view_pos;
uniform vec3 ambient_color;
uniform vec3 light_color;
uniform float kd;
uniform float ks;
uniform float n;

void main()
{
 vec3 normal = normalize(fragment_normal);
 vec3 nligh_dir = normalize(light_dir);

 vec3 rRefl = reflect(nligh_dir,normal);
 float nDotL = max(dot(normal, nligh_dir), 0.0);
 vec3 v = normalize(view_pos - surface_position);

 float vDotR = max(dot(v,rRefl),0.0);
 float specular = pow(vDotR, n);
 vec3 diffuse_color = texture2D(mytexture, uv0).rgb;
 vec3 ambient = ambient_color*diffuse_color;
	
 color.rgb = ambient + vec3(kd*diffuse_color*light_color*nDotL) + vec3(ks*light_color*specular);
 color.a = 1.0;

}