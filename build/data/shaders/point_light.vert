varying vec3 N;
varying vec3 v;
varying vec2 texture_coordinate;

void main(void)
{
   v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   texture_coordinate = vec2(gl_MultiTexCoord0);
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}