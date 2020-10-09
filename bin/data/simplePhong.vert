#version 150

#define MAX_LIGHTS 1

out vec2 v_texcoord;
out vec3 v_viewSpaceNormal;
out vec3 v_viewSpacePosition;

in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;

// These are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

void main (void){
	mat4 normalMatrix =  transpose(inverse(modelViewMatrix));
    v_viewSpaceNormal = normalize((normalMatrix * normal).xyz);
	v_viewSpacePosition = (modelViewMatrix * position).xyz;
    v_texcoord = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
    gl_Position = modelViewProjectionMatrix * position;
}
