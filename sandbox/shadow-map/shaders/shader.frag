#version 460 core
#include ../../common/shaders/uniforms.glsl

in vec3 position;
in vec3 normal;
in vec2 texCoords;
in vec4 positionLightSpace;

out vec4 fragColor;

uniform vec3 camPos;
uniform sampler2DShadow depthMap;
uniform float depthBias;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float rand(in vec4 seed4)
{
  float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
  return fract(sin(dot_product) * 43758.5453);  
}

// Blinn-Phong reflection model
vec3 blinnPhong(in vec3 viewDir, in vec3 normal, in vec3 lightDir, in vec3 kd, in vec3 ks, in float shininess) {
  vec3 diffuse = max(dot(lightDir, normal), 0.0) * kd;

  vec3 h = normalize(lightDir + viewDir); // half-vector
  vec3 specular = pow(max(dot(h, normal), 0.0), shininess) * ks;

  return diffuse + specular;
}

float testShadow(in vec4 positionLightSpace) {
  // projection
  vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;

  // convert from NDC to [0, 1]
  projCoords = projCoords * 0.5 + 0.5;

  // if position is far than zFar, do not shadow
  float currentDepth = projCoords.z;
  if(currentDepth > 1.0) {
    return 0.0;
  }

  // shadow bias
  float bias = clamp(depthBias * tan(acos(dot(normal, directionalLight.direction))), 0.0, 0.01);

  // float visibility = 1.0;
  // for(int i = 0; i < 4; ++i) {
  //   int index = int(16.0 * rand(vec4(gl_FragCoord.xyy, i)))%16;
  //   visibility -= 0.25 * clamp(1.0 - texture(depthMap, vec3(projCoords.xy + poissonDisk[index]/700.0, currentDepth - bias)), 0.0, 1.0);
  // }
  float visibility = 1.0 - clamp(1.0 - texture(depthMap, vec3(projCoords.xy, currentDepth - bias)), 0.0, 1.0);

  return visibility;
}

void main() {
  // view direction
  vec3 viewDir = normalize(camPos - position);

  vec3 kd = texture(diffuseMap, texCoords).xyz + material.kd;
  vec3 ks = texture(specularMap, texCoords).xyz + material.ks;

  vec3 color = vec3(0);

  // shadow test
  float visibility = testShadow(positionLightSpace);

  // directional light
  color += visibility * blinnPhong(viewDir, normal, directionalLight.direction, kd, ks, material.shininess) * directionalLight.ke;

  // point light
  vec3 lightDir = normalize(pointLight.position - position);
  float dist = max(distance(pointLight.position, position) - pointLight.radius, 0.0);
  color += visibility * blinnPhong(viewDir, normal, lightDir, kd, ks, material.shininess) * pointLight.ke / pow(dist, 2.0);

  // ambient
  color += 0.01 * (1.0 - visibility) * kd;

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  fragColor = vec4(color, 1.0);
}