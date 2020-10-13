#version 150

#define PI 3.1415926538

out vec4 fragColor;

in vec2 v_texcoord;
in vec3 v_viewSpaceNormal;
in vec3 v_viewSpacePosition;

uniform vec4 lightPosition;
uniform vec4 lightColor;
uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform float matShininess;


// These are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform sampler2DRect specularTexture;

void pointLight(in vec3 normal, in vec3 surfacePosition, inout vec3 diffuse, inout vec3 specular){
    float nDotVP;       // normal . light direction
    float nDotHV;       // normal . light half vector
    vec3  halfVector;   // direction of maximum highlights

    // Compute direction from surface point to the eye
    // Note: eye is at (0,0,0) in view space
    vec3 eyeDir = normalize(-surfacePosition);

    // Compute direction from surface to light position
    vec4 viewSpaceLightPos = viewMatrix * lightPosition;
    vec3 lightDir = normalize(viewSpaceLightPos.xyz - surfacePosition);

    // Compute the half vector between lightDir and eyeDir
    halfVector = normalize(lightDir + eyeDir);

    // Compute dot products for use in calculations
    nDotHV = max(0.0, dot(normal, halfVector));
    nDotVP = max(0.0, dot(normal, lightDir));

    // Calculate diffuse contribution of light to the surface
    diffuse += lightColor.rgb * nDotVP;

    // Calculate specular contribution of light to the surface
    vec3 specularReflection = vec3(lightColor.rgb) * matSpecular.rgb * pow(nDotHV, matShininess);
    specular += mix(vec3(0.0), specularReflection, step(0.0000001, nDotVP));
}


void main (void) {
    vec3 ambient = vec3(0,0,0);
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);

    // Re-normalize v_viewSpaceNormal to avoid interpollation artefacts
    vec3 normal = normalize(v_viewSpaceNormal);

    // Get the contribution of the light to the diffuse and specular light on the surface
    pointLight(normal, v_viewSpacePosition, diffuse, specular);

    // Combine the lighting contributions to calculate the fragment color
    fragColor = matAmbient + vec4(diffuse,1.0) * matDiffuse + vec4(specular,1.0) * matSpecular;
    

}
