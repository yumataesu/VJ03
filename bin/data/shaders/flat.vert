#version 410

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int normalReverse;
uniform int u_col;
uniform vec3 lightPos;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

flat out vec3 LightIntensity;

void main() {
    
    vec3 tnorm = vec3(0.0, 0.0, 0.0);
    if(normalReverse == 0) tnorm = normalize((view * model * vec4(normal, 0.0)).xyz);
    if(normalReverse == 1) tnorm = normalize((view * model * vec4(-normal, 0.0)).xyz);
    vec4 eyeCoords = view * model * vec4(position, 1.0);
    vec3 s = normalize(vec3(lightPos - eyeCoords.xyz));
    vec3 v = normalize(-eyeCoords.xyz);
    vec3 r = reflect( -s, tnorm );
    float sDotN = max( dot(s, tnorm), 0.0 );
    vec3 ambient = vec3(0.01, 0.01, 0.01);
    
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    if(u_col == 0) diffuse = vec3(0.5, 0.1, 0.1) * sDotN;
    if(u_col == 1) diffuse = vec3(0.2, 0.1, 0.8) * sDotN;
    if(u_col == 2) diffuse = vec3(1.2, 1.1, 1.8) * sDotN;
    
    vec3 spec = vec3(1.0, 1.0, 1.0);
    if( sDotN > 0.0 ) spec = vec3(1.0, 1.0, 1.0) * pow( max( dot(r, v), 0.0 ), 20 );
    else spec = vec3(0.0, 0.0, 0.0);
    
    
    float distance = length(lightPos - eyeCoords.xyz) * 0.006;
    float attenuation = 1.0 / (distance * distance);
    
    LightIntensity = (ambient + diffuse + spec) * attenuation;
    gl_Position = projection * view * model * vec4(position, 1.0);
}

