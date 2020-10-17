varying vec4 uv;
uniform sampler2D texture;
uniform vec4 alphaColor;


void main() {
    // Transparence
//    vec4 color;
//    color = texture2D(texture, uv.st);
    // a r g b
//    color.a *= alphaColor[0];
//    color.r *= alphaColor[1];
//    color.g *= alphaColor[2];
//    color.b *= alphaColor[3];
//    gl_FragColor = color;
    gl_FragColor = texture2D(texture, uv.st);;
}
