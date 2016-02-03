
varying vec3 N;
varying vec3 v;
varying vec2 texture_coordinate; 
uniform sampler2D my_color_texture;
uniform float light_radius;

void main(void)
{
   vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * 1;

   Idiff = clamp(Idiff, 0.0, 1.0); 
	
float dist = distance(v, gl_LightSource[0].position);

if(dist<light_radius)
{
	float multiplier = (1-(dist/light_radius ));
	Idiff.r = Idiff.r*multiplier;
	Idiff.g = Idiff.g*multiplier;
	Idiff.b = Idiff.b*multiplier;
}

if(texture2D(my_color_texture, texture_coordinate).a == 0)
{
Idiff.a = 0;
}
else
{
if(Idiff.r<0.1 || dist>light_radius )
{
Idiff.r = 0.1;
Idiff.g = 0.1;
Idiff.b = 0.1;
}

Idiff.a = 1;
}

   gl_FragColor = Idiff*texture2D(my_color_texture, texture_coordinate);
}
        
