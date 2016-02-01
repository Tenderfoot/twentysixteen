varying vec2 texture_coordinate;
varying vec3 N;
varying vec3 v;
uniform float Time;

void main()
{
    // Transforming The Vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex + (sin(Time*1.3)*(gl_Vertex.x/2)*(gl_Vertex.y/2));
 
    // Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader

 v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   texture_coordinate = vec2(gl_MultiTexCoord0);

}
