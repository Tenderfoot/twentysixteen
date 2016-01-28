
varying vec2 texture_coordinate; 
uniform sampler2D my_color_texture;

int rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    // Sampling The Texture And Passing It To The Frame Buffer

    vec4 test = texture2D(my_color_texture, texture_coordinate);
    gl_FragColor = vec4(1-test.r, 1-test.g, 1-test.b, test.a);
}