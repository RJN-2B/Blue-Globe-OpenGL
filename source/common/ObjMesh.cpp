#include "common.h"


bool Mesh::loadOBJ(const char * path){
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< vec3 > temp_vertices;
  std::vector< vec2 > temp_uvs;
  std::vector< vec3 > temp_normals;
  
  hasUV = true;
  
  FILE * file = fopen(path, "r");
  if( file == NULL ){
    printf("Impossible to open the file !\n");
    return false;
  }
  
  char *line = new char[128];
  char *lineHeader = new char[128];
  
  while(true){
    memset(line, 0 , 128);
    memset(lineHeader, 0 , 128);
    
    if(fgets(line, 128, file) == NULL){ break; }
    sscanf(line, "%s ", lineHeader);
    
    if ( strcmp( lineHeader, "v" ) == 0 ){
      vec3 vertex;
      sscanf(&line[2], "%f %f %f", &vertex.x, &vertex.y, &vertex.z );
      temp_vertices.push_back(vertex);
      if(vertex.x < box_min.x){box_min.x = vertex.x; }
      if(vertex.y < box_min.y){box_min.y = vertex.y; }
      if(vertex.z < box_min.z){box_min.z = vertex.z; }
      if(vertex.x > box_max.x){box_max.x = vertex.x; }
      if(vertex.y > box_max.y){box_max.y = vertex.y; }
      if(vertex.z > box_max.z){box_max.z = vertex.z; }
    }else if ( strcmp( lineHeader, "vt" ) == 0 ){
      vec2 uv;
      sscanf(&line[3], "%f %f", &uv.x, &uv.y );
      temp_uvs.push_back(uv);
    }else if ( strcmp( lineHeader, "vn" ) == 0 ){
      vec3 normal;
      sscanf(&line[3], "%f %f %f", &normal.x, &normal.y, &normal.z );
      temp_normals.push_back(normal);
    }else if ( strcmp( lineHeader, "f" ) == 0 ){
      std::string vertex1, vertex2, vertex3;
      int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = sscanf(&line[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                           &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                           &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9){
        int matches = sscanf(&line[2], "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
                             &vertexIndex[1], &normalIndex[1],
                             &vertexIndex[2], &normalIndex[2]);
        if (matches == 6){
          hasUV = false;
        } else {
          printf("File can't be read by our simple parser : ( Try exporting with other options\n");
          return false;
        }
      }
      
      /* handle negative indices */
      /* (adjust for size during processing of each face, as per the old
       *  OBJ specification, instead of after the end of the file) */
      for (int negati = 0; negati < 3; negati++){
        if (vertexIndex[negati] < 0){
          vertexIndex[negati]+=temp_vertices.size();
          vertexIndex[negati]++; /* <- OBJ indices are one-based */
        }
        if (uvIndex[negati] < 0){
          uvIndex[negati]+=temp_uvs.size();
          uvIndex[negati]++;
        }
        if (normalIndex[negati] < 0){
          normalIndex[negati]+=temp_normals.size();
          normalIndex[negati]++;
        }
      }
      
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      if(hasUV){
        uvIndices    .push_back(uvIndex[0]);
        uvIndices    .push_back(uvIndex[1]);
        uvIndices    .push_back(uvIndex[2]);
      }
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    }
  }
  
  delete[] line;
  delete[] lineHeader;
  
  // For each vertex of each triangle
  for( unsigned int i=0; i<vertexIndices.size(); i++ ){
    unsigned int vertexIndex = vertexIndices[i];
    vec4 vertex = vec4(temp_vertices[ vertexIndex-1 ], 1.0);
    vertices.push_back(vertex);
  }
  
  if(hasUV){
    for( unsigned int i=0; i<uvIndices.size(); i++ ){
      unsigned int uvIndex = uvIndices[i];
      vec2 uv = temp_uvs[ uvIndex-1 ];
      uvs.push_back(uv);
    }
  }
  
  for( unsigned int i=0; i<normalIndices.size(); i++ ){
    unsigned int normalIndex = normalIndices[i];
    vec3 normal = temp_normals[ normalIndex-1 ];
    normals.push_back(normal);
  }
    
  center = box_min+(box_max-box_min)/2.0;
  scale = (std::max)(box_max.x - box_min.x, box_max.y-box_min.y);
  
  model_view = Scale(1.0/scale,           //Make the extents 0-1
                     1.0/scale,
                     1.0/scale)*
  Translate(-center);  //Orient Model About Center
  
  
  return true;
}

bool Mesh::makeSphere(int steps){//Can rotate earth and keep light fixed, or rotate the light.
  //TODO:  FILL IN vertices, normals, and uv vectors

	//phi (latitude) -> 0 to pi
	//Theta (spin) -> 0 to 2*pi


	//normals.push_back(vec3(0, 0, 1)); //This and next line. Initial vertex he populated for us. I moved to outside. Was inside.
	//uvs.push_back(vec2(0, 0));

	double phiStep = (M_PI) / double(steps);
	double thetaStep = (2 * M_PI) / double(steps);

	for (double phi1 = 0; phi1 < M_PI; phi1 = phi1 + phiStep) { // < or <= ??

		for (double theta1 = 0.0; theta1 <= 2 * M_PI; theta1 = theta1 + thetaStep) {

			double phi2 = phi1 + phiStep;
			double theta2 = theta1 + thetaStep;


			//POINT 1
			double x1 = sin(phi1) * cos(theta1);
			double y1 = sin(phi1) * sin(theta1);
			double z1 = cos(phi1);
			vec3 normV1 = vec3(x1, y1, z1); //Don't need to normalize since its unit sphere
			double u1 = theta1 / (2 * M_PI);
			double v1 = phi1 / M_PI;



			//POINT 2
			double x2 = sin(phi2) * cos(theta1);
			double y2 = sin(phi2) * sin(theta1);
			double z2 = cos(phi2);
			vec3 normV2 = vec3(x2, y2, z2);
			double u2 = theta1 / (2 * M_PI);
			double v2 = phi2 / M_PI;




			//POINT 3
			double x3 = sin(phi2) * cos(theta2);
			double y3 = sin(phi2) * sin(theta2);
			double z3 = cos(phi2);
			vec3 normV3 = vec3(x3, y3, z3);
			double u3 = theta2 / (2 * M_PI);
			double v3 = phi2 / M_PI;





			//POINT 4
			double x4 = sin(phi1) * cos(theta2);
			double y4 = sin(phi1) * sin(theta2);
			double z4 = cos(phi1);
			vec3 normV4 = vec3(x4, y4, z4);
			double u4 = theta2 / (2 * M_PI);
			double v4 = phi1 / M_PI;



			//Triangle 1 - Vertices 1, 2, 4

			vertices.push_back(vec4(x2, y2, z2, 1)); //Phi 2, Theta 1
			normals.push_back(normV2);
			uvs.push_back(vec2(u2, v2));

			vertices.push_back(vec4(x1, y1, z1, 1)); //Phi 1, Theta 1
			normals.push_back(normV1);
			uvs.push_back(vec2(u1, v1));

		
			vertices.push_back(vec4(x4, y4, z4, 1)); //Phi 1, Theta 2
			normals.push_back(normV4);
			uvs.push_back(vec2(u4, v4));




			//Triangle 2 - Vertices 2, 3, 4
			vertices.push_back(vec4(x2, y2, z2, 1)); //Phi 2, Theta 1
			normals.push_back(normV2);
			uvs.push_back(vec2(u2, v2));

			vertices.push_back(vec4(x3, y3, z3, 1)); //Phi 2, Theta 2
			normals.push_back(normV3);
			uvs.push_back(vec2(u3, v3));

			vertices.push_back(vec4(x4, y4, z4, 1)); //Phi 1, Theta 2
			normals.push_back(normV4);
			uvs.push_back(vec2(u4, v4));




		}
	}






  return true;
  }
