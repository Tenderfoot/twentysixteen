varying vec2 texture_coordinate;

uniform float Time;

void main()
{
    // Transforming The Vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex + (sin(Time*1.3)*(gl_Vertex.x/2)*(gl_Vertex.y/2));
 
    // Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader
    texture_coordinate = vec2(gl_MultiTexCoord0);
}
