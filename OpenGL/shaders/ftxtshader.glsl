#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

//uniform sampler2D texture;

varying vec3 N;
varying vec3 v;
varying vec2 v_texcoord;

void main()
{

    vec3 eye = vec3(0,0,0);

    vec3 L = normalize(eye - v);
       vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
       vec3 R = normalize(-reflect(L,N));

       //calculate Ambient Term:
       vec4 Iamb = vec4(0.1, 0.1, 0.1, 0.1);

       //calculate Diffuse Term:
       vec4 diffuse = vec4(1, 1, 1, 1);
       vec4 Idiff = diffuse * max(dot(N,L), 0.0);
       Idiff = clamp(Idiff, 0.0, 1.0);

       // calculate Specular Term:
       //vec4 Ispec = gl_FrontLightProduct[0].specular
         //           * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
      // Ispec = clamp(Ispec, 0.0, 1.0);
       // write Total Color:
      // gl_FragColor =  Iamb + Idiff;// + Ispec;

    // Set fragment color from texture
    gl_FragColor = texture2D(texture, v_texcoord);
}
