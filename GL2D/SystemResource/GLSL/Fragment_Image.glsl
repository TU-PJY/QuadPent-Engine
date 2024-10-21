#version 330 core

in vec3 fragPos;
in vec2 TexCoord;

out vec4 fragColor;
uniform vec3 viewPos;

uniform sampler2D outTexture;
uniform float transparency;
uniform vec3 objectColor;

uniform float BlurRadius; 
uniform vec2 TexelSize;

void main() {
    vec4 Color = vec4(0.0);
    vec4 FinalColor = vec4(0.0);
    float Total = 0.0;

    int Radius = int(BlurRadius);
    if(Radius > 0) {
        for(int y = -Radius; y <= Radius; y++) {
            for(int x = -Radius; x <= Radius; x++) {
                vec2 offset = vec2(float(x), float(y)) * TexelSize * 3.0;
                vec4 sample = texture(outTexture, TexCoord + offset);
                Color += sample;
                Total += 1.0;
            }
        }
        FinalColor = Color / Total;
    }

    else
        FinalColor = texture(outTexture, TexCoord);

    if (FinalColor.a < 0.01)
        FinalColor.a = 0.0;
    else
        fragColor = vec4(FinalColor.rgb + objectColor.rgb, FinalColor.a * transparency);
}