varying vec2 texture_coordinate;
varying vec3 N;
varying vec3 v;
uniform float Time;

void main()
{
    // Transforming The Vertex

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
 
    // Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader

   v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   texture_coordinate = vec2(gl_MultiTexCoord0);

   if(texture_coordinate.y > 0.9)
   {
	 gl_Vertex.y += sin(Time)/10;
	 
	 if(texture_coordinate.x < 0.1)
	 {
		 gl_Vertex.x += sin(Time)/10;
	 }
	 else
	 {
		 gl_Vertex.x -= sin(Time)/10;
	 }

	 gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   }
   else
   {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    }

}
