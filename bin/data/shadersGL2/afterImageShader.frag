#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform float alpha;

void main()
{
    
    vec4 color = texture2DRect(tex0, texCoordVarying);

    float avg = 0.299*color.r + 0.587*color.g + 0.114*color.b;
    
    vec4 gray = vec4(avg*0.5, avg*0.5, avg, color.a*alpha);
    
    gl_FragColor = gray;
}
