#version 330

in vec4 vColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 directionalLightSpacePos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light {
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
};

struct DirectionalLight {
  Light base;
  vec3 direction;
};

struct PointLight {
  Light base;
  vec3 position;
  float constant;
  float linear;
  float exponent;
};

struct SpotLight {
  PointLight base;
  vec3 direction;
  float edge;
};

struct Material {
  float specularIntensity;
  float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;

uniform Material material;

uniform vec3 eyePosition;

float calcDirectionalShadowFactor(DirectionalLight light) {
  vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
  projCoords = (projCoords * 0.5) + 0.5;

  float currentDepth = projCoords.z;

  vec3 temp_normal = normalize(normal);
  vec3 lightDir = normalize(light.direction);

  float bias = max(0.05 * (1 - dot(temp_normal, lightDir)), 0.005);

  float shadow = 0.0;

  vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
  for (int x = -1; x <= 1; x ++) {
    for (int y = -1; y <= 1; y ++) {
      float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    }
  }

  shadow /= (3.0 * 3.0);

  if (projCoords.z > 1.0) {
    shadow = 0.0;
  }

  return shadow;
}

vec4 calcLightByDirection(Light light, vec3 direction, float shadowFactor) {
  vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

  float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
  vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

  vec4 specularColor = vec4(0, 0, 0, 0);
  
  if (diffuseFactor > 0.0f) {
    vec3 fragToEye = normalize(eyePosition - fragPos);
    vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));
    float specularFactor = dot(fragToEye, reflectedVertex);
    if (specularFactor > 0.0f) {
      specularFactor = pow(specularFactor, material.shininess);
      specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
    }
  }
  return (ambientColor + (1.0f - shadowFactor) * (diffuseColor + specularColor));
}

vec4 calcDirectionalLight() {
  float shadowFactor = calcDirectionalShadowFactor(directionalLight);
  return calcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 calcPointLight(PointLight pLight) {
  vec3 direction = fragPos - pLight.position;
  float distance = length(direction);
  direction = normalize(direction);

  vec4 color = calcLightByDirection(pLight.base, direction, 0.0f);
  float attenuation = pLight.exponent * distance * distance +
    pLight.linear * distance +
    pLight.constant;
  return (color / attenuation);
}

vec4 calcSpotLight(SpotLight sLight) {
  vec3 rayDirection = normalize(fragPos - sLight.base.position);
  float sLightFactor = dot(rayDirection, sLight.direction);

  if (sLightFactor > sLight.edge) {
    vec4 color = calcPointLight(sLight.base);
    return color * (1.0f  - (1.0f - sLightFactor) * (1.0f / (1.0f - sLight.edge)));
  } else {
    return vec4(0, 0, 0, 0);
  }
}

vec4 calcPointLights() {
  vec4 totalColor = vec4(0, 0, 0, 0);
  for (int i = 0; i < pointLightCount; i ++) {
    totalColor += calcPointLight(pointLights[i]);
  }
  return totalColor;
}

vec4 calcSpotLights() {
  vec4 totalColor = vec4(0, 0, 0, 0);
  for (int i = 0; i < spotLightCount; i ++) {
    totalColor += calcSpotLight(spotLights[i]);
  }
  return totalColor;
}

void main() {
  vec4 finalColor = calcDirectionalLight();
  finalColor += calcPointLights();
  finalColor += calcSpotLights();
  
  color = texture(theTexture, texCoord) * finalColor;
}
