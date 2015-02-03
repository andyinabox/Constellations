#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform float alpha;
uniform vec2 resolution;

//Just a really quick, simple circular blur.Constants that you can change: CV is the radius of the blur, ST controls the quality (higher is faster but looks worse).

//#define PI2 6.283184
//
//#define CV 0.1
//#define ST 0.05

//vec4 colorat(vec2 uv) {
//	return texture2DRect(tex0, uv);
//}
//vec4 blur(vec2 uv) {
//	vec4 col = vec4(0.0);
//	for(float r0 = 0.0; r0 < 1.0; r0 += ST) {
//		float r = r0 * CV;
//		for(float a0 = 0.0; a0 < 1.0; a0 += ST) {
//			float a = a0 * PI2;
//			col += colorat(uv + vec2(cos(a), sin(a)) * r);
//		}
//	}
//	col *= ST * ST;
//	return col;
//}

//void main(void) {
//	vec2 uv = gl_FragCoord.xy / iResolution.xy;
//	gl_FragColor = blur(uv);
//}


void main()
{

//    vec2 uv = gl_FragCoord.xy / resolution.xy;
    
    vec4 color = texture2DRect(tex0, texCoordVarying);

    float avg = 0.299*color.r + 0.587*color.g + 0.114*color.b;
    
    vec4 gray = vec4(avg*0.5, avg*0.5, avg, color.a*alpha);
    
//    vec4 color = texture2DRect(tex0, vec2(texCoordVarying.y, texCoordVarying.x));
    
    gl_FragColor = gray;
//    gl_FragColor = blur(uv);
}
