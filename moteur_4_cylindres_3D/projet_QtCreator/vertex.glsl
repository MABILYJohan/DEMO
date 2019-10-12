attribute highp vec4 posAttr;
attribute lowp  vec4 colAttr;
attribute mediump vec4 texAttr;
attribute highp vec3 norAttr;

varying lowp  vec4 col;
varying mediump vec4 texc;
varying highp vec3 nor;

uniform highp mat4 projMatrix;
uniform highp mat4 mvMatrix;
uniform highp mat3 norMatrix;

void main() {
   col = colAttr;
   texc = texAttr;
   gl_Position = projMatrix * mvMatrix * posAttr;
   nor = norMatrix * norAttr;
}

