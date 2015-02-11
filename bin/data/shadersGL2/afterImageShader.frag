#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform float alpha;
uniform float blur;
uniform float threshold;

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
    
    // run through horizontal blur & get color
    vec4 color = blurX(tex0, texCoordVarying.xy, blur);
    
    // thresholding
    color.rgb = mix(color.rgb, vec3(1.0), color.r+color.g+color.b/3-threshold);

    // get grey point
    float grey = 0.299*color.r + 0.587*color.g + 0.114*color.b;
    
    // set bluish tone, set alpha
    vec4 bluish = vec4(grey*0.5, grey*0.5, grey, color.a*alpha);
    
    gl_FragColor = bluish;

}

