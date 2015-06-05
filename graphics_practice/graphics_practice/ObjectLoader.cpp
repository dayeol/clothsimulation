#include"ObjectLoader.h"

void ObjectLoader::readObjFile(string fileName)
{
	ifstream fin;
	vector<vec4> vertexs;
	vector<vec2> textures;
	vector<vec3> normals;

	string line, word;
	cout << " ? " << endl;
	fin.open(fileName);
	if (!fin)
	{
		cout << "Read obj error !" << endl;
		//exit(0);
	}
	int j = 1;
	while (getline(fin, line))
	{
		if (j == 26950)
		{
			j--;
			j++;
		}
		
		if (line.size() == 0 || line[0] == '#') { j++; continue; }
		istringstream is(line);
		is >> word;
		if (word == "v")
		{
			vec3 p;
			is >> p.x >> p.y >> p.z;
			vertexs.push_back(vec4(p.x, p.y, p.z, 1));
		}
		else
			if (word == "vt")
			{
				vec2 p;
				is >> p.x >> p.y;
				textures.push_back(p);
			}
			else
				if (word == "vn")
				{
					vec3 p;
					is >> p.x >> p.y >> p.z;
					normals.push_back(p);
				}
				else
					if (word == "f")
					{
						int c = 0;
						
						vector<long> vertexNum;
						vector<long> normalNum;
						vector<long> textureNum;

						int i = 0;
						
						while (is >> word)
						{
							c = count(word.begin(), word.end(), '/');
							if (c == 0)
							{
								vertexNum.push_back(atoi(word.c_str()));
							}
							else
								if (c == 1)
								{
									vertexNum.push_back(atoi(string(word.begin(), word.begin() + word.find("/")).c_str()));
									int tex = atoi(string(word.begin() + word.find("/") + 1, word.end()).c_str());
									if (tex != 0)
											textureNum.push_back(tex);
								}
								else
									if (c == 2)
									{
										int a = word.find("/");
										int b = word.find("/", a + 1);
										
										int tex = atoi(string(word.begin() + a + 1, word.begin() + b).c_str());
										int norm = atoi(string(word.begin() + b + 1, word.end()).c_str());
										
										vertexNum.push_back(atoi(string(word.begin(), word.begin() + a).c_str()));
										//if (tex != 0)
											textureNum.push_back(tex);
										//if (norm != 0)
											normalNum.push_back(norm);
									}
							i++;
						}
						
						switch (c)
						{
						case 0:
							for (int a = 1; a <= i - 2; a++)
							{
								Face face;
								face.set(vertexs, vertexNum, a); 
								faces.push_back(face);
							}
							break;
						case 1:
							for (int a = 1; a <= i - 2; a++)
							{
								Face face;
								face.set(vertexs, textures, vertexNum, textureNum, a);
								faces.push_back(face);
							}
							break;
						case 2:
							for (int a = 1; a <= i - 2; a++)
							{
								Face face;
								face.set(vertexs, textures, normals, vertexNum, textureNum, normalNum, a);
								faces.push_back(face);
							}
							break;
						default:break;
						}
					}
		j++;
	}
	cout << "Load " + fileName + "is done!" << endl;
	fin.close();
}


inline int power_of_two(int n)
{
	if (n <= 0) return 0;
	return (n&(n - 1)) == 0;
}

GLuint ObjectLoader::readTexture(const char *file_name)
{
	GLint width, height, total_bytes;
	GLuint last_texture_ID, texture_ID = 0;
	GLubyte *pixels = NULL;
	FILE *pFile;

	if ((pFile = fopen(file_name, "rb")) == NULL)
	{
		cout << "Read texture error" << endl;
		return 0;
	}

	fseek(pFile, 18, SEEK_SET);
	fread(&width, sizeof(width), 1, pFile);
	fread(&height, sizeof(height), 1, pFile);
	fseek(pFile, 54, SEEK_SET);

	total_bytes = (width * 3 + (4 - width * 3 % 4) % 4)*height;

	if ((pixels = (GLubyte *)malloc(total_bytes)) == NULL)
	{
		fclose(pFile);
		return 0;
	}

	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width) || !power_of_two(height) || width>max || height>max)
		{
			const GLint new_width = 1024;
			const GLint new_height = 1024;
			GLint new_total_bytes;
			GLubyte *new_pixels = NULL;

			new_total_bytes = (new_width * 3 + (4 - new_width * 3 % 4) % 4)*new_height;
			new_pixels = (GLubyte *)malloc(new_total_bytes);
			if (new_pixels == NULL)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels, new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	/*glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);
	*/
	free(pixels);
	fclose(pFile);
	cout << "read texture is done" << endl;
	return texture_ID;
}

void ObjectLoader::addVerticesList()
{
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		vertices.push_back(faces[i].getVertex1());
		vertices.push_back(faces[i].getVertex2());
		vertices.push_back(faces[i].getVertex3());
		//vertexList[idx] = vert1; idx++;
		//vertexList[idx] = vert2; idx++;
		//vertexList[idx] = vert3; idx++;
	}
	printf("done loading # face: %d\n", faces.size());
	cout << "Vertex Adding is done!" << endl;
}

void ObjectLoader::addVerticesList(vector<vec4> &vertList)
{
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		vertList.push_back(faces[i].getVertex1());
		vertList.push_back(faces[i].getVertex2());
		vertList.push_back(faces[i].getVertex3());
		//vertexList[idx] = vert1; idx++;
		//vertexList[idx] = vert2; idx++;
		//vertexList[idx] = vert3; idx++;
	}

	cout << "Vertex Adding is done!" << endl;
}

void ObjectLoader::addTextureCoord()
{
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		textures.push_back(faces[i].getTexture1());
		textures.push_back(faces[i].getTexture2());
		textures.push_back(faces[i].getTexture3());
	}

	cout << "Texture Adding is done!" <<endl;
}

void ObjectLoader::addTextureCoord(vector<vec2> &texList)
{
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		texList.push_back(faces[i].getTexture1());
		texList.push_back(faces[i].getTexture2());
		texList.push_back(faces[i].getTexture3());
	}

	cout << "Texture Adding is done!" << endl;
}

void ObjectLoader::addNormals()
{
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		normals.push_back(faces[i].getNormal1());
		normals.push_back(faces[i].getNormal2());
		normals.push_back(faces[i].getNormal3());
	}

	cout << "Normal Adding is done!" << endl;
}

void ObjectLoader::addNormals(vector<vec3> &normList)
{
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		normList.push_back(faces[i].getNormal1());
		normList.push_back(faces[i].getNormal2());
		normList.push_back(faces[i].getNormal3());
	}

	cout << "Normal Adding is done!" << endl;
}


void Face::set(vector<vec4> &vertexs, vector<long> vertexNum, int offset)
{
	vertex1 = vertexs[vertexNum[0] - 1];
	vertex2 = vertexs[vertexNum[offset] - 1];
	vertex3 = vertexs[vertexNum[offset + 1] - 1];

	texture1 = vec2(0, 0);
	texture2 = vec2(0, 0);
	texture3 = vec2(0, 0);

	normal1 = vec3(0, 0, 0);
	normal2 = vec3(0, 0, 0);
	normal3 = vec3(0, 0, 0);
}

void Face::set(vector<vec4> &vertexs, vector<vec2> &textures, vector<long> vertexNum, vector<long> textureNum, int offset)
{
	vertex1 = vertexs[vertexNum[0] - 1];
	vertex2 = vertexs[vertexNum[offset] - 1];
	vertex3 = vertexs[vertexNum[offset + 1] - 1];

	// texture is not used
	
	texture1 = textures[textureNum[0] - 1];
	texture2 = textures[textureNum[offset] - 1];
	texture3 = textures[textureNum[offset + 1] - 1];

	normal1 = vec3(0, 0, 0);
	normal2 = vec3(0, 0, 0);
	normal3 = vec3(0, 0, 0);
}

void Face::set(vector<vec4> &vertexs, vector<vec2> &textures, vector<vec3> &normals, vector<long> vertexNum, vector<long> textureNum, vector<long> normalNum, int offset)
{
	vertex1 = vertexs[vertexNum[0] - 1];
	vertex2 = vertexs[vertexNum[offset] - 1];
	vertex3 = vertexs[vertexNum[offset + 1] - 1];

	// texture is not used
	//cout << textureNum.at(offset) << endl;
	texture1 = (textureNum[0] > 0)? textures[textureNum[0] - 1]: vec2(0);
	texture2 = (textureNum[offset] > 0)? textures[textureNum[offset] - 1]: vec2(0);
	texture3 = (textureNum[offset + 1] > 0) ? textures[textureNum[offset + 1] - 1] : vec2(0);
	
	
	normal1 = (normalNum[0] > 0)? normals[normalNum[0] - 1]: vec3(0);
	normal2 = (normalNum[offset] >0)? normals[normalNum[offset] - 1]: vec3(0);
	normal3 = (normalNum[offset + 1] >0)? normals[normalNum[offset + 1] - 1]: vec3(0);
}