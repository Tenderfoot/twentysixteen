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

   vec4 test = gl_ModelViewProjectionMatrix * gl_Vertex;

   texture_coordinate = vec2(gl_MultiTexCoord0);

   if(texture_coordinate.y > 0.9)
   {
	 test.y += sin(Time)/10;
	 
	 if(texture_coordinate.x < 0.1)
	 {
		 test.x += sin(Time)/2;
	 }
	 else
	 {
		 test.x -= sin(Time)/2;
	 }

	 gl_Position = test;
   }
   else
   {
   	 gl_Position = test;
   }

}
