#version 410

uniform float u_alpha;
flat in vec3 LightIntensity;

out vec4 fragcolor;

void main() {
    
    fragcolor = vec4(LightIntensity, 1.0);
}
