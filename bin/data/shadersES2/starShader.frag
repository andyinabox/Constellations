#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;
uniform float alpha;

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

vec4 blurY(in sampler2DRect tx, in vec2 uv, in float amt) {
    vec4 color;
	
    color += 1.0 * texture2DRect(tx, uv + vec2(0.0, amt * -4.0));
    color += 2.0 * texture2DRect(tx, uv + vec2(0.0, amt * -3.0));
    color += 3.0 * texture2DRect(tx, uv + vec2(0.0, amt * -2.0));
    color += 4.0 * texture2DRect(tx, uv + vec2(0.0, amt * -1.0));
    
    color += 5.0 * texture2DRect(tx, uv + vec2(0.0, amt));
	
    color += 4.0 * texture2DRect(tx, uv + vec2(0.0, amt * 1.0));
    color += 3.0 * texture2DRect(tx, uv + vec2(0.0, amt * 2.0));
    color += 2.0 * texture2DRect(tx, uv + vec2(0.0, amt * 3.0));
    color += 1.0 * texture2DRect(tx, uv + vec2(0.0, amt * 4.0));
	
    color /= 25.0;
    
    return color;
}

void main()
{
    
//    vec4 outColor = blurX(tex0, texCoordVarying.xy, 0.5);
    vec4 xBlur = blurX(tex0, texCoordVarying.xy, 0.4);
    vec4 yBlur = blurY(tex0, texCoordVarying.xy, 0.4);

    vec4 outColor = mix(xBlur, yBlur, 0.5);
    
    outColor.a = alpha;
    
    
    gl_FragColor = outColor;
}

