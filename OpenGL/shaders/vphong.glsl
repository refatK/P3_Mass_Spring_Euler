#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat4 mv_matrix;


attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;


varying vec3 N;
varying vec3 v;
//varying vec2 v_texcoord;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(a_position, 1);
    
    v = vec3(mv_matrix * vec4(a_position, 1));
    N = normalize(vec3(mv_matrix * vec4(a_normal, 0)));

}
