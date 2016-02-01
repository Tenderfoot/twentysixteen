
varying vec3 N;
varying vec3 v;
varying vec2 texture_coordinate; 
uniform sampler2D my_color_texture;

void main(void)
{
   vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);  
   Idiff = clamp(Idiff, 0.0, 1.0); 
	
if(texture2D(my_color_texture, texture_coordinate).a == 0)
{
Idiff.a = 0;
}
else
{
if(Idiff.r<0.2)
{
Idiff.r = 0.2;
Idiff.g = 0.2;
Idiff.b = 0.2;
}
Idiff.a = 1;
}


   gl_FragColor = Idiff*texture2D(my_color_texture, texture_coordinate);
}
        