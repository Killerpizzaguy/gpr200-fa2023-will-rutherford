#version 450

uniform float iTime;
uniform vec3 iResolution;

//Uniforms to use with the ui
//Original values from shadertoy in comments

uniform vec3 _SkyTop; //0.851, 0.447, 0.541
uniform vec3 _SkyBottom; //0.482, 0.42, 0.969
uniform vec3 _TopSunColor; //1, 1, 0.9
uniform vec3 _BottomSunColor; //1.0, 0.3, 0.2
uniform float _SunRadius; //0.2
uniform float _SunSpeed; //1.0
uniform vec3 _ForegroundColor; //0.1, 0.1, 0.1

out vec4 fragColor;
vec2 fragCoord = gl_FragCoord.xy;

float circleSDF(vec2 p, float r){
    return length(p)-r;
}

void main()
{
     // Normalized pixel coordinates (from 0 to 1)
     vec2 uv = fragCoord/iResolution.xy;
     float aspect = iResolution.x / iResolution.y;
    
     //Sky gradient
     float t = sin(uv.y*1.0+iTime*_SunSpeed)*0.5 + 0.5;
     vec3 color = mix(_SkyBottom, _SkyTop,uv.y-0.4+t);
     
     //Adjust uv to do SDFs
     uv = (2.0*fragCoord-iResolution.xy)/iResolution.y;
     
     float sunBlur = 20.0*aspect/iResolution.x;
     
     //Red sun
     vec2 redSunPos = uv + vec2(-1.0, 0.6+sin(iTime*_SunSpeed)*1.0);
     color = mix(color, _BottomSunColor, smoothstep(sunBlur,-sunBlur,circleSDF(redSunPos,_SunRadius)));
     
     //Yellow sun
     vec2 yellowSunPos = uv + vec2(-0.4, 0.2+sin(iTime*_SunSpeed)*1.0);
     color = mix(color, _TopSunColor, smoothstep(sunBlur,-sunBlur,circleSDF(yellowSunPos,_SunRadius)));
     
     //Dunes
     float dunes = 1.0 - smoothstep(sin(uv.x*2.0) * 0.1 - 0.73, sin(uv.x*2.0) * 0.1 - 0.7, uv.y);
     vec3 duneColor = vec3(0.1);
     
     color = mix(color, _ForegroundColor, dunes);
     
     fragColor = vec4(color,1.0);

}