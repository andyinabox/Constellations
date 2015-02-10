#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;
uniform float alpha;

void main()
{
    
    vec4 outColor = texture2DRect(tex0, texCoordVarying);
    
    outColor.a = alpha;
    
    
    gl_FragColor = outColor;
}
