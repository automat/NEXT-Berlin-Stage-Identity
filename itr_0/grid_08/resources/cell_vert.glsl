varying vec3 vN; 	// vertex normal
varying vec3 vV;	// vertex position in eye space, seen from 0,0,0
varying vec4 vVEye; // vertex position in eye space

varying vec3 vL0;	// direction pos light0
varying vec3 vL1;	// direction pos light1

void main(void){
	vVEye = gl_ModelViewMatrix * gl_Vertex;
    
	vV  = normalize( -vVEye.xyz );
	vN  = normalize( gl_NormalMatrix * gl_Normal);
    
	vL0 = normalize( gl_LightSource[0].position.xyz - vVEye.xyz );
	vL1 = normalize( gl_LightSource[1].position.xyz - vVEye.xyz );

    gl_Position = ftransform();
}
