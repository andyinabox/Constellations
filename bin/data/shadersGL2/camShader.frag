#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform vec2 resolution;
uniform float radius;
uniform float softness;
uniform float opacity;
uniform vec2 center;
uniform vec3 inputColor;
uniform float colorMix;

// https://github.com/mattdesl/lwjgl-basics/wiki/ShaderLesson3
void main()
{
    vec4 color = texture2DRect(tex0, texCoordVarying);

//    float avg = 0.299*color.r + 0.587*color.g + 0.114*color.b;
//    vec4 gray = vec4(avg, avg, avg, color.a);
    
    // get center of vignette based on given position
    vec2 position = (gl_FragCoord.xy / resolution.xy) - center; //vec2(center.x, (1.0-center.y));//- vec2(0.5);

	//determine the vector length of the center position
    float len = length(position);

	//use smoothstep to create a smooth vignette
    float vignette = smoothstep(radius, radius - softness, len);

    // use greyscale color to overlay input color
    color.rgb = mix(color.rgb, color.rgb*inputColor, colorMix);
    
    // color.rgb = mix(color.rgb, color.rgb * vignette, opacity);
    color.rgb = mix(color.rgb, color.rgb * vignette, opacity);


    gl_FragColor = color;
}

// https://www.opengl.org/sdk/docs/man/html/smoothstep.xhtml
// http://en.wikipedia.org/wiki/Smoothstep
float smoothstep(in float edge0, in float edge1, in float x) {
	float t;
    t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}