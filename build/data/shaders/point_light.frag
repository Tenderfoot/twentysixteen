
varying vec3 N;
varying vec3 v;
varying vec2 texture_coordinate; 
uniform sampler2D my_color_texture;

// Its a GLight, because I'm Grant.
// Yeah I know I didn't invent this but fuck you
struct GLight
{
  float x;
  float y;
  float z;
  float r;
  float g;
  float b;
  float radius;
};

// I'm going to support 10 total lights to start...
uniform GLight scene_lights[10];
uniform float num_lights;

void main(void)
{

	vec3 light_pos;
	vec3 L;   
	vec4 Idiff;  
	vec4 diff_total;

	int i;
	for(i=0; i<num_lights; i++)
	{
		light_pos = vec3(scene_lights[i].x,scene_lights[i].y,scene_lights[i].z);
		L = normalize(light_pos - v);   
		Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);  
		Idiff = clamp(Idiff, 0.0, 1.0); 
	
		float dist = distance(v, light_pos);
		if(dist<scene_lights[i].radius)
		{
			float multiplier = (1-(dist/scene_lights[i].radius));
			Idiff.r = Idiff.r*multiplier*scene_lights[i].r;
			Idiff.g = Idiff.g*multiplier*scene_lights[i].g;
			Idiff.b = Idiff.b*multiplier*scene_lights[i].b;
		}

		if(texture2D(my_color_texture, texture_coordinate).a == 0)
		{
			Idiff.a = 0;
		}
		else
		{
			if(dist>scene_lights[i].radius)
			{
				Idiff.r = 0.0;
			}

			if( dist>scene_lights[i].radius)
			{
				Idiff.g = 0.0;
			}

			if(dist>scene_lights[i].radius)
			{
				Idiff.b = 0.0;
			}

			Idiff.a = 1;
		}

		diff_total += Idiff;
	}

	// ambient light
	if(diff_total.r<0.0)
	{
		diff_total.r = 0.0;
	}
	if(diff_total.g<0.0)
	{
		diff_total.g = 0.0;
	}
	if(diff_total.b<0.0)
	{
		diff_total.b = 0.0;
	}

   gl_FragColor = diff_total*texture2D(my_color_texture, texture_coordinate);
}
        