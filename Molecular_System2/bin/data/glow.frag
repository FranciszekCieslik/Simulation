#version 330 core

in vec2 vTexCoord;

uniform sampler2D texture0;       // Diffuse texture
uniform vec3 glowColor = vec3(1.0, 0.5, 0.0); // Base glow color (e.g., orange)
uniform float glowIntensity = 1.5;            // Intensity of glow

out vec4 fragColor;

void main() {
    vec4 baseColor = texture(texture0, vTexCoord);

    // Combine base color with emissive glow
    vec3 emissive = glowColor * glowIntensity;
    vec4 finalColor = vec4(baseColor.rgb + emissive, baseColor.a);

    fragColor = finalColor;
}
