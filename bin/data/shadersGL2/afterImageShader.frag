#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform float alpha;
uniform float blur;
uniform vec2 resolution;
uniform vec2 center;

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

// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// The texture2D() call accepts a mip level offset as an optional parameter, which
// allows one to sample from different LODs of the texture. Besides being handy in
// some special situations, it also allows you to fake (box) blur of textures without
// having to perform a blur youtself. This has been traditionally used in demos and
// games to fake deph ot field and other similar effects in a very cheap way.

//void main( void )
//{
//	vec2 uv = gl_FragCoord.xy / iResolution.xy;
//	
//	float lod = (5.0 + 5.0*sin( iGlobalTime ))*step( uv.x, 0.5 );
//	
//	vec3 col = texture2D( iChannel0, vec2(uv.x,1.0-uv.y), lod ).xyz;
//	
//	gl_FragColor = vec4( col, 1.0 );
//}

// https://www.shadertoy.com/view/XslGz8
//This is a blur, facilitated by a barrel distortion/fisheye function I found somewhere (that's a pretty generic way of doing a lens distort). Changing warp amount and layering=blur

//vec2 barrelDistortion(vec2 coord, float amt) {
//	vec2 cc = coord - 0.5;
//	float dist = dot(cc, cc);
//	//return coord + cc * (dist*dist)  * amt;
//	return coord + cc * dist * amt;
//    
//}
//
//void main()
//{
//	//vec2 uv=(gl_FragCoord.xy/iResolution.xy);
//    
//	vec2 uv=(gl_FragCoord.xy/iResolution.xy*.5)+.25;
//	//uv.y +=.1;
//	vec4 a1=texture2D(iChannel0, barrelDistortion(uv,0.0));
//	vec4 a2=texture2D(iChannel0, barrelDistortion(uv,0.2));
//	vec4 a3=texture2D(iChannel0, barrelDistortion(uv,0.4));
//	vec4 a4=texture2D(iChannel0, barrelDistortion(uv,0.6));
//	
//	vec4 a5=texture2D(iChannel0, barrelDistortion(uv,0.8));
//	vec4 a6=texture2D(iChannel0, barrelDistortion(uv,1.0));
//	vec4 a7=texture2D(iChannel0, barrelDistortion(uv,1.2));
//	vec4 a8=texture2D(iChannel0, barrelDistortion(uv,1.4));
//	
//	vec4 a9=texture2D(iChannel0, barrelDistortion(uv,1.6));
//	vec4 a10=texture2D(iChannel0, barrelDistortion(uv,1.8));
//	vec4 a11=texture2D(iChannel0, barrelDistortion(uv,2.0));
//	vec4 a12=texture2D(iChannel0, barrelDistortion(uv,2.2));
//    
//	vec4 tx=(a1+a2+a3+a4+a5+a6+a7+a8+a9+a10+a11+a12)/12.;
//	gl_FragColor = vec4(tx.rgb,1.);
//}


//vec2 barrelDistortion(vec2 coord, float amt) {
//	vec2 cc = coord - 0.5;
//	float dist = dot(cc, cc);
////	return coord + cc * (dist*dist)  * amt;
//	return coord + cc * dist * amt;
//
//}


vec4 blurX(in sampler2DRect tx, in vec2 uv, in float amt) {
    vec4 color;
	
    color += 1.0 * texture2DRect(tx, uv + vec2(amt * -4.0, 0.0));
    color += 2.0 * texture2DRect(tx, uv + vec2(amt * -3.0, 0.0));
    color += 3.0 * texture2DRect(tx, uv + vec2(amt * -2.0, 0.0));
    color += 4.0 * texture2DRect(tx, uv + vec2(amt * -1.0, 0.0));
    
    color += 5.0 * texture2DRect(tx, uv + vec2(amt, 0));
	
    color += 4.0 * texture2DRect(tx, uv + vec2(amt * 1.0, 0.0));
    color += 3.0 * texture2DRect(tx, uv + vec2(amt * 2.0, 0.0));
    color += 2.0 * texture2DRect(tx, uv + vec2(amt * 3.0, 0.0));
    color += 1.0 * texture2DRect(tx, uv + vec2(amt * 4.0, 0.0));
	
    color /= 25.0;
    
    return color;
}

void main()
{
    
    vec4 color = blurX(tex0, texCoordVarying.xy, blur);
    
//    vec2 position = (gl_FragCoord.xy / resolution.xy) - center; //vec2(center.x, (1.0-center.y));//- vec2(0.5);

    float avg = 0.299*color.r + 0.587*color.g + 0.114*color.b;
    
    vec4 bluish = vec4(avg*0.5, avg*0.5, avg, color.a*alpha);
    
    gl_FragColor = bluish;

}



//vec4 vignette(col, pos, rad, soft, opacity) {
//    float len = length(position);
//    float vig = smoothstep(rad, rad - soft, len);
//    col.rgb = mix(color.rgb, color.rgb * vig, opacity);
//    
//}
//
//float smoothstep(in float edge0, in float edge1, in float x) {
//	float t;
//    t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
//    return t * t * (3.0 - 2.0 * t);
//}

