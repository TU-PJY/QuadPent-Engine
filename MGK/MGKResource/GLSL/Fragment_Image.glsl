#version 330 core

in vec3           fragPos;
in vec2           TexCoord;

out vec4          fragColor;
uniform vec3      viewPos;

uniform sampler2D outTexture;
uniform float     transparency;
uniform vec3      objectColor;

uniform bool      UseBlur;
uniform float     Radius; 
uniform vec2      TexelSize;

vec4              BlurResult;
vec4              DefaultResult;
vec4              FinalResult;

vec4 ComputeBlur() {
    vec4 BlurResultValue = vec4(0.0);
    vec4 totalColor = vec4(0.0);
    float totalAlpha = 0.0;
    float totalWeight = 0.0;
    int BlurNum = int(Radius);
    int kernelSize = (BlurNum * 2 + 1) * (BlurNum * 2 + 1);

    for (int y = -BlurNum; y <= BlurNum; ++y) {
        for (int x = -BlurNum; x <= BlurNum; ++x) {
            vec2 offset = vec2(float(x), float(y)) * TexelSize * 3.0;
            vec4 sample = texture(outTexture, TexCoord + offset);
            
            float weight = sample.a;
            totalColor += sample * weight;
            totalAlpha += sample.a;
            totalWeight += weight;
        }
    }

    vec4 weightedColor = vec4(totalColor.rgb / totalWeight, totalAlpha / float(kernelSize));
    BlurResultValue = mix(vec4(0.0), weightedColor, step(0.0, totalWeight));

    return BlurResultValue;
}

void main() {
    BlurResult = mix(vec4(0.0), ComputeBlur(), float(UseBlur));
    DefaultResult = mix(texture(outTexture, TexCoord), vec4(0.0), float(UseBlur));

   FinalResult = mix(DefaultResult, BlurResult, float(UseBlur));
   fragColor = vec4(FinalResult.rgb + objectColor.rgb, FinalResult.a * transparency);
}