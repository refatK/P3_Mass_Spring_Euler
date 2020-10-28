#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 v_texcoord;

uniform vec3 color;

void main(){

    vec4 fc  =  texture2D(texture, v_texcoord);
    vec4 color4 = vec4(color, 1);
    gl_FragColor = color4 * fc;
	
}
