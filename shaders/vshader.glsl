#version 150

in  vec4 vPosition;
in  vec3 vNormal;
in  vec2 vTexCoord;


uniform mat4 ModelViewEarth;
uniform mat4 Projection;
uniform mat4 NormalMatrix;

out vec4 pos;
out vec4 N;
out vec2 texCoord;



void main()
{
  
  texCoord    = vTexCoord;
  
  pos = ModelViewEarth * vPosition;

  // Transform vertex normal into eye coordinates
  N = NormalMatrix*vec4(vNormal, 0.0);
  N.w = 0.0;
  N = normalize(N);

  // Transform vertex position into eye coordinates
  gl_Position = Projection * pos;
  
}
