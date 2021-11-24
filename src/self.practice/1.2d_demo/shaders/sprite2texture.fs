#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 spriteColor;
uniform float angle;

//uv代表坐标 a是角度
vec2 rota2(vec2 uv, float a)
{
    return vec2(uv.x * cos(a) - uv.y * sin(a), uv.y * cos(a) + uv.x * sin(a));
}

void main()
{
    vec2 maskTexCoord = rota2(TexCoords - vec2(0.5, 0.5), radians(angle)) + vec2(0.5, 0.5);
    float maskalpha = texture(texture1, maskTexCoord).a;
    color = vec4(spriteColor, 1.0) * texture(texture0, TexCoords) * maskalpha;
}