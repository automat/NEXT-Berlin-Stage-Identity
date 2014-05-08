varying vec3  vNormal;
varying float vDepth;

const float farClip = 5.0;

void main(){
    vDepth  = -(gl_ModelViewMatrix * gl_Vertex).z / farClip;
    vNormal = normalize(( gl_ModelViewMatrix * vec4(gl_Normal.xyz,0.0)).xyz);
    gl_Position = ftransform();
}