// https://github.com/choi303/FXAA
#version 330 core
#extension GL_EXT_gpu_shader4 : enable

in vec2 uv;

uniform sampler2D _MainTex;
uniform vec4 _MainTex_TexelSize;

out vec4 fragColor;

#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/16.0)
#define FXAA_SEARCH_STEPS        32
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/8.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (7.0/8.0)
#define FXAA_SUBPIX_TRIM         (1.0/8.0)
#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))

vec4 FxaaTexOff(sampler2D tex, vec2 uv, vec2 off) { return textureLod(tex, uv + off * _MainTex_TexelSize.xy, 0); }

float FxaaLuma(vec3 rgb) { return rgb.y * (0.587 / 0.299) + rgb.x; }

vec3 FxaaFilterReturn(vec3 rgb) { return rgb; }

vec4 FxaaTexGrad(sampler2D tex, vec2 uv, vec2 grad) { return textureGrad(tex, uv, grad, grad); }

vec3 FxaaLerp3(vec3 a, vec3 b, float amountOfA)
{
    return (vec3(-amountOfA,0,0) * b) +
        ((a * vec3(amountOfA,0,0)) + b);
}

vec4 FxaaTexLod0(sampler2D tex, vec2 uv) { return textureLod(tex, uv, 0.0); }

vec4 Fxaa(sampler2D tex, vec2 uv) {
	//SEARCH MAP
    vec3 rgbN = FxaaTexOff(tex, uv, vec2(0, -1)).xyz;
    vec3 rgbW = FxaaTexOff(tex, uv, vec2(-1, 0)).xyz;
    vec3 rgbM = FxaaTexOff(tex, uv, vec2(0, 0)).xyz;
    vec3 rgbE = FxaaTexOff(tex, uv, vec2(1, 0)).xyz;
    vec3 rgbS = FxaaTexOff(tex, uv, vec2(0, 1)).xyz;
    float lumaN = FxaaLuma(rgbN);
    float lumaW = FxaaLuma(rgbW);
    float lumaM = FxaaLuma(rgbM);
    float lumaE = FxaaLuma(rgbE);
    float lumaS = FxaaLuma(rgbS);
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
    if (range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))
        return vec4(FxaaFilterReturn(rgbM), 1.0f);

    vec3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
    
    //COMPUTE LOWPASS
    #if FXAA_SUBPIX != 0
        float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
        float rangeL = abs(lumaL - lumaM);
    #endif
    #if FXAA_SUBPIX == 1
        float blendL = max(0.0,
            (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
        blendL = min(FXAA_SUBPIX_CAP, blendL);
    #endif
    
    
    //CHOOSE VERTICAL OR HORIZONTAL SEARCH
    vec3 rgbNW = FxaaTexOff(tex, uv, vec2(-1, -1)).xyz;
    vec3 rgbNE = FxaaTexOff(tex, uv, vec2(1, -1)).xyz;
    vec3 rgbSW = FxaaTexOff(tex, uv, vec2(-1, 1)).xyz;
    vec3 rgbSE = FxaaTexOff(tex, uv, vec2(1, 1)).xyz;
    #if (FXAA_SUBPIX_FASTER == 0) && (FXAA_SUBPIX > 0)
        rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
        rgbL *= vec3(1.0 / 9.0,0,0);
    #endif
    float lumaNW = FxaaLuma(rgbNW);
    float lumaNE = FxaaLuma(rgbNE);
    float lumaSW = FxaaLuma(rgbSW);
    float lumaSE = FxaaLuma(rgbSE);
    float edgeVert =
        abs((0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE)) +
        abs((0.50 * lumaW) + (-1.0 * lumaM) + (0.50 * lumaE)) +
        abs((0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE));
    float edgeHorz =
        abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW)) +
        abs((0.50 * lumaN) + (-1.0 * lumaM) + (0.50 * lumaS)) +
        abs((0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
    bool horzSpan = edgeHorz >= edgeVert;
    float lengthSign = horzSpan ? -_MainTex_TexelSize.y : -_MainTex_TexelSize.x;
    if (!horzSpan)
        lumaN = lumaW;
    if (!horzSpan)
        lumaS = lumaE;
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;
    
    
    //CHOOSE SIDE OF PIXEL WHERE GRADIENT IS HIGHEST
    bool pairN = gradientN >= gradientS;
    if (!pairN)
        lumaN = lumaS;
    if (!pairN)
        gradientN = gradientS;
    if (!pairN)
        lengthSign *= -1.0;
    vec2 posN;
    posN.x = uv.x + (horzSpan ? 0.0 : lengthSign * 0.5);
    posN.y = uv.y + (horzSpan ? lengthSign * 0.5 : 0.0);
    
    //CHOOSE SEARCH LIMITING VALUES
    gradientN *= FXAA_SEARCH_THRESHOLD;

    //SEARCH IN BOTH DIRECTIONS UNTIL FIND LUMA PAIR AVERAGE IS OUT OF RANGE
    vec2 posP = posN;
    vec2 offNP = horzSpan ?
        vec2(_MainTex_TexelSize.x, 0.0) :
        vec2(0.0f, _MainTex_TexelSize.y);
    float lumaEndN = lumaN;
    float lumaEndP = lumaN;
    bool doneN = false;
    bool doneP = false;
    #if FXAA_SEARCH_ACCELERATION == 1
        posN += offNP * vec2(-1.0, -1.0);
        posP += offNP * vec2(1.0, 1.0);
    #endif
    for (int i = 0; i < FXAA_SEARCH_STEPS; i++)
    {
    #if FXAA_SEARCH_ACCELERATION == 1
        if (!doneN)
            lumaEndN =
                FxaaLuma(FxaaTexLod0(tex, posN.xy).xyz);
        if (!doneP)
            lumaEndP =
                FxaaLuma(FxaaTexLod0(tex, posP.xy).xyz);
    #endif
        doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
        doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);
        if (doneN && doneP)
            break;
        if (!doneN)
            posN -= offNP;
        if (!doneP)
            posP += offNP;
    }
    
    
    //HANDLE IF CENTER IS ON POSITIVE OR NEGATIVE SIDE
    float dstN = horzSpan ? uv.x - posN.x : uv.y - posN.y;
    float dstP = horzSpan ? posP.x - uv.x : posP.y - uv.y;
    bool directionN = dstN < dstP;
    lumaEndN = directionN ? lumaEndN : lumaEndP;
    
    //CHECK IF PIXEL IS IN SECTION OF SPAN WHICH GETS NO FILTERING   
    if (((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) 
        lengthSign = 0.0;
    
    float spanLength = (dstP + dstN);
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5 + (dstN * (-1.0 / spanLength))) * lengthSign;
    vec3 rgbF = FxaaTexLod0(tex, vec2(
    uv.x + (horzSpan ? 0.0 : subPixelOffset),
    uv.y + (horzSpan ? subPixelOffset : 0.0))).xyz;    
   return vec4(FxaaFilterReturn(FxaaLerp3(rgbL, rgbF, blendL)), 1.0f);
}

void main() {
	fragColor = Fxaa(_MainTex, uv);
	fragColor.a = 1;
}
