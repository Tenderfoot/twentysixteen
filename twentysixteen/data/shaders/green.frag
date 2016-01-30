
varying vec2 texture_coordinate; 
uniform sampler2D my_color_texture;
uniform float Time;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453 * Time);
}

void main()
{
    // Sampling The Texture And Passing It To The Frame Buffer

    vec4 test = texture2D(my_color_texture, texture_coordinate);
    //gl_FragColor = vec4(abs(sin(Time)*test.r), abs(cos(Time)*test.g), abs(cos(Time)+sin(Time)), test.a);

    float s = (test.r + test.b + test.g)/3.0;

    float r = fract(Time * texture_coordinate.x);
    float g = fract(Time * texture_coordinate.y);
    float b = fract(Time * dot(texture_coordinate.xy, vec2(12.534, 343.65)));

    gl_FragColor = vec4(r, g, b, test.a);
}
