attribute vec4 posAttr;
attribute vec4 colAttr;
varying vec4 col;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {

   col = colAttr;
   mat4 modelView = viewMatrix * modelMatrix;

   gl_Position = projectionMatrix * modelView * posAttr;
}

