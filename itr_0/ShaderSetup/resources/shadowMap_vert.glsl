varying vec3 vN; 	// vertex normal
varying vec3 vV;	// vertex position in eye space, seen from 0,0,0 
varying vec4 vVEye; // vertex position in eye space

varying vec3 vL0;	// direction pos light0
varying vec3 vL1;	// direction pos light1

varying vec4 vSc0;	// coord of shadow 0 in eyespace
varying vec4 vSc1;	// coord of shadow 1 in eyespace

uniform mat4 uShadow0Matrix; // input shadow matrix of light 0
uniform mat4 uShadow1Matrix; // input shadow matrix of light 1

varying vec4 	vLightIntensity;

void main(void){
	vVEye = gl_ModelViewMatrix * gl_Vertex;

	vV  = normalize( -vVEye.xyz );
	vN  = normalize( gl_NormalMatrix * gl_Normal);
    
	vL0 = normalize( gl_LightSource[0].position.xyz - vVEye.xyz );
	vL1 = normalize( gl_LightSource[1].position.xyz - vVEye.xyz );

	vSc0 = uShadow0Matrix * vVEye;
	vSc1 = uShadow1Matrix * vVEye;
    gl_Position = ftransform();
}
