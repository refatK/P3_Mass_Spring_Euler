#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 a_position;
attribute vec2 a_texcoord;

uniform mat4 mvp_matrix;

varying vec2 v_texcoord;

uniform vec2 SC;

void main(){

	// Output position of the vertex, in clip space
	// map [0..800][0..600] to [-1..1][-1..1]

        vec4 tpos = mvp_matrix * vec4(a_position, 1);

        vec2 pos;
        pos[0] = tpos[0];//a_position[0];
        pos[1] = tpos[1];//a_position[1];

//        pos[0] = a_position[0];
//        pos[1] = a_position[1];

        vec2 vertexPosition_homoneneousspace = pos - SC;
// [0..800][0..600] -> [-400..400][-300..300]

	vertexPosition_homoneneousspace /= SC;

        //gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);
        gl_Position = tpos;

        v_texcoord = a_texcoord;
}

