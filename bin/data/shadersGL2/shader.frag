#version 120

uniform sampler2DRect tex0;

varying vec2 texCoordVarying;

uniform float period;
uniform float time;
uniform int active;

void main()
{
    vec4 color = texture2DRect(tex0, texCoordVarying);

    if((color.r + color.g + color.b)/3 > 0.1) {
 	   color.a = (cos(time/period) + 1) / 2;
    }
    color.r = color.r * ((sin(time) + 1) / 2);
    color.b = color.b * ((cos(time) + 1) / 2);

    if(active < 1) {
    	color.a = 0.0;
    }

    gl_FragColor = color;
}