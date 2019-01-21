#version 150

uniform vec4 ambient;
uniform vec4 LightPosition;

in vec4 pos;
in vec4 N;
in vec2 texCoord;

uniform mat4 ModelViewLight;

uniform sampler2D textureEarth;
uniform sampler2D textureNight;
uniform sampler2D textureCloud;
uniform sampler2D texturePerlin;

uniform float animate_time;


out vec4 fragColor;




void main()
{

  
  vec4 L = normalize( (ModelViewLight*LightPosition) - pos );




  float Kd = 1.0;
  
  float bright = dot(N, L) / (length(L) * length(N));
  bright = clamp(bright, 0.0, 1.0);

  vec4 diffuse_color = texture(textureEarth, texCoord );
  vec4 diffuse_cloud = texture(textureCloud, texCoord );
  vec4 night_text = texture(textureNight, texCoord);
  vec4 noise_text = texture(texturePerlin, texCoord );

  //vec2 newCoord = noise2(texCoord);
  //vec4 noise_text = texture(texturePerlin, newCoord );


  diffuse_color = (diffuse_cloud + diffuse_color + noise_text);
  //diffuse_color = (noise_text * Kd);





  diffuse_color = Kd*diffuse_color * bright; 
  night_text = Kd * night_text;


  

  
  //fragColor = vec4(1.0,0.0,0.0,1.0); 
  fragColor = (ambient + diffuse_color) + (night_text * (.90-bright));




  fragColor = clamp(fragColor, 0.0, 1.0);
  fragColor.a = 1.0;
}