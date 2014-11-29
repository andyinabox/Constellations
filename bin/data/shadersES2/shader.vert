precision highp float;

attribute vec4 position;
attribute vec2 texcoord;
uniform mat4 modelViewProjectionMatrix;


varying vec2 texCoordVarying;

uniform float time;


void main()
{

    gl_Position = modelViewProjectionMatrix * position;
    texCoordVarying = texcoord;
}