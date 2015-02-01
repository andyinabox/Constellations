#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform vec3 color;
uniform float alpha;

void main()
{
    
    vec4 outColor = texture2DRect(tex0, texCoordVarying);
    
    outColor.rgb = color.rgb;
    outColor.a = alpha;
    
    gl_FragColor = outColor;
}
