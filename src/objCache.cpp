#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

class float2{
	public:
	float2 operator+(float2 b) {
		return float2(this->x + b.x, this->y + b.y);
	}
	float2 operator-(float2 b) {
		return float2(this->x - b.x, this->y - b.y);
	}
		#pragma omp parallel for
	float2 operator*(float2 b) {
		return float2(this->x * b.x, this->y * b.y);
	}
	float2 operator*(float b) {
		return float2(this->x * b, this->y * b);
	}
	float2 operator/(float b) {
		return float2(this->x / b, this->y / b);
	}
	float x;
	float y;
	float2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	float2() {
	}
};
class float3{
	public:
	float x = 0;
	float y = 0;
	float z = 0;
	operator float2() const {
		return float2(this->x, this->y);
	}
	float3 operator*(float b) {
		return float3(this->x * b, this->y * b, this->z * b);
	}
	float3 operator*(float3 b) {
		return float3(this->x * b.x, this->y * b.y, this->z * b.z);
	}
	float3 operator/(float b) {
		return float3(this->x / b, this->y / b, this->z / b);
	}
	float3 operator+(float3 b) {
		return float3(this->x + b.x, this->y + b.y, this->z + b.z);
	}
	/*void operator+=(float3 b) {
		*this = *this + b;
	}*/
	float3 operator-(float3 b) {
		return float3(this->x - b.x, this->y - b.y, this->z - b.z);
	}
	float3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float3() {
	}
};


void cacheObjFile(std::string obj, std::string filename) {
	std::vector<float3> allPoints;
	std::vector<float3> triPoints;
	std::vector<float2> allTexCoords;
	std::vector<float2> texCoords;
	std::vector<float3> allNormals;
	std::vector<float3> normals;

	std::stringstream lines(obj);
	std::string line;
	int a=0;

	while(std::getline(lines, line, '\n')) {
		if(line.length() < 2) continue;
		if(line.substr(0, 2) == "v ") {
			std::vector<float> axes;
			std::stringstream words(line);
			std::string word;
			int i = 2;
			while(std::getline(words, word, ' ')) {
				if(i > 2) {
					axes.push_back(std::stof(word));
				}
				i++;
			}
			allPoints.push_back({axes[0], axes[1], axes[2]});
		} else if(line.substr(0,3) == "vt ") {

			std::vector<std::string> axes;
			std::stringstream words(line);
			std::string word;
			int i = 2;
			while(std::getline(words, word, ' ')) {
				if(i > 2) {
					axes.push_back(word);
				}
				i++;
			}
			float ax1 = std::stof(axes[0]);
			float ax2 = std::stof(axes[1]);
			float2 t = float2(ax1, 1-ax2);
			allTexCoords.push_back(t);
			
		}else if(line.substr(0,3) == "vn ") {

			std::vector<std::string> axes;
			std::stringstream words(line);
			std::string word;
			int i = 2;
			while(std::getline(words, word, ' ')) {
				if(i > 2) {
					axes.push_back(word);
				}
				i++;
			}
			float3 t = float3(std::stof(axes[0]), std::stof(axes[1]), std::stof(axes[2]));
			allNormals.push_back(t);
			
		}else

		if(line.substr(0, 2) == "f ") {
			std::stringstream words(line);
			std::string word;
			std::vector<std::string> faceIndexGroups;
			int c = 2;
			while(std::getline(words, word, ' ')) {
				if(c > 2) {
					faceIndexGroups.push_back(word);
				}
				c++;
			}

			for(int i = 0; i<faceIndexGroups.size(); i++) {
				std::vector<int> indexGroup;
				std::stringstream thisFaceGroup(faceIndexGroups[i]);
				while(std::getline(thisFaceGroup, word, '/')) {
					if(word.empty()) indexGroup.push_back(0); else
					indexGroup.push_back(std::stoi(word));
				}
				int pointIndex = indexGroup[0]-1;
				int texIndex = indexGroup.size() > 1? indexGroup.at(1)-1 : -1;
				int normIndex = indexGroup.size() > 2? indexGroup.at(2)-1 : -1;
				if(i>=3) triPoints.push_back(triPoints[triPoints.size()-(3 * i - 6)]);
				if(i>=3) triPoints.push_back(triPoints[triPoints.size()-(2)]);

				if(i>=3) texCoords.push_back(texCoords[texCoords.size()-(3 * i - 6)]);
				if(i>=3) texCoords.push_back(texCoords[texCoords.size()-(2)]);

				if(i>=3) normals.push_back(normals[normals.size()-(3 * i - 6)]);
				if(i>=3) normals.push_back(normals[normals.size()-(2)]);

				triPoints.push_back(allPoints[pointIndex]);
				texCoords.push_back(texIndex >= 0? allTexCoords[texIndex] : float2(0,0));
				normals.push_back(normIndex >= 0? allNormals[normIndex] : float3(0,0,0));
			}

		}
	} 
	//return {triPoints, texCoords, normals};
	std::vector<float> value;
	for(int i = 0; i < triPoints.size(); i++) {
		value.push_back(triPoints.at(i).x);
		value.push_back(triPoints.at(i).y);
		value.push_back(triPoints.at(i).z);

		value.push_back(texCoords.at(i).x);
		value.push_back(texCoords.at(i).y);

		value.push_back(normals.at(i).x);
		value.push_back(normals.at(i).y);
		value.push_back(normals.at(i).z);
	}
	std::ofstream cacheFile(filename, std::ios::binary);
	for(float val : value) {
		cacheFile.write(reinterpret_cast<const char*>(&val), sizeof(val));
	}
}

void cacheObjFileName(std::string objName, std::string cacheName) {
	std::ifstream file(objName);
	std::string objStr = "";
	for(std::string line; std::getline(file, line); objStr += line + "\n");
	cacheObjFile(objStr, cacheName);
}

int main(int argc, char**argv) {
	if (argc < 3) {
		std::cout << "no enough args :((";
		return 1;
	}

	cacheObjFileName(argv[1], argv[2]);

	return 0;
}
