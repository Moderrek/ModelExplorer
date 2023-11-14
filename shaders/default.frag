#version 330 core

out vec4 FragColor;

in vec3 currentPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 point_light() {
    float ambient = 0.2f;
    float specularLight = 0.5f;
    
    vec3 light_vec = lightPos - currentPos;
    float dist = length(light_vec);
    float a = 3.0f;
    float b = 0.7f;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    
    vec3 normal = normalize(Normal);
    vec3 light_direction = normalize(light_vec);

    float diffuse = max(dot(normal, light_direction), 0.0f);
    vec3 viewDirection = normalize(camPos - currentPos);
    vec3 reflectionDirection = reflect(-light_direction, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16.0f);
    float specular = specAmount * specularLight;
    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 directional_light() {
    float ambient = 0.20f;
    float specularLight = 0.50f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    
    vec3 viewDirection = normalize(camPos - currentPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

float near = 0.1f;
float far = 250.0f;

float linearize_depth(float depth) {
    return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logistic_depth(float depth, float steepness, float offset) {
    float z_val = linearize_depth(depth);
    return (1 / (1 + exp(-steepness * (z_val - offset))));
}

vec4 spot_light() {
    float ambient = 0.2f;
    float outerCone = 0.9f;
    float innerCone = 0.95f;
    float specularLight = 0.5f;

    vec3 light_vec = lightPos - currentPos;

    
    vec3 normal = normalize(Normal);
    vec3 light_direction = normalize(light_vec);

    float diffuse = max(dot(normal, light_direction), 0.0f);
    vec3 viewDirection = normalize(camPos - currentPos);
    vec3 reflectionDirection = reflect(-light_direction, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16.0f);
    float specular = specAmount * specularLight;

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -light_direction);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main() {
    float depth = logistic_depth(gl_FragCoord.z, 0.02f, 32.0f);
    FragColor = directional_light() * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.90f), 1.0f);
}