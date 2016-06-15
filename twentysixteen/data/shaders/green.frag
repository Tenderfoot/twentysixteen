uniform sampler2D bgl_RenderedTexture;
uniform sampler2D bgl_DepthTexture;

void main(void)
{
  vec4 texcolor = texture2D(bgl_RenderedTexture, gl_TexCoord[0].st);
  float x =  gl_TexCoord[0].st.x - .5;
  float y =  gl_TexCoord[0].st.y - .5;   

  vec4 depth = texture2D(bgl_DepthTexture,gl_TexCoord[0].st);

  float value = 0.0015 * depth.r;

  vec4 color = texture2D(bgl_RenderedTexture, vec2(gl_TexCoord[0].st.x + value, gl_TexCoord[0].st.y + value)); // Sample area around current pixel
  color += texture2D(bgl_RenderedTexture, vec2(gl_TexCoord[0].st.x - value, gl_TexCoord[0].st.y - value));
  color += texture2D(bgl_RenderedTexture, vec2(gl_TexCoord[0].st.x + value, gl_TexCoord[0].st.y - value));
  color += texture2D(bgl_RenderedTexture, vec2(gl_TexCoord[0].st.x - value, gl_TexCoord[0].st.y + value));
  color /= 4.0;        
  gl_FragColor = color;

   
}