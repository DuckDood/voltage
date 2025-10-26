#include <vector>
#include <GL/glew.h>
#include "shader.h"

std::vector<float> LoadObjFile(std::string obj);

std::vector<float> LoadObjByName(std::string path);

std::vector<float> load3dCache(std::string path);

// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
// i couldve figured it out but its easier to copy just for easy comparison logic
class boundingBox {
	public:
	float minX;
	float maxX;

	float minY;
	float maxY;

	float minZ;
	float maxZ;
  
	glm::vec3 position = glm::vec3(0.,0.,0.);
	glm::vec3 scale = glm::vec3(1.,1.,1.);

	void Update();
};
bool intersect(boundingBox a, boundingBox b);

class Hitbox : public boundingBox {
	public:
		std::vector<Hitbox*> *hitboxes;
		int id = 0;
		bool isOverlapped();

};

class Framebuffer {
	public:
	unsigned int framebuffer;
	unsigned int renderbuffer;
	unsigned int colorBuffer;

	int width, height;

	Framebuffer(int inWidth, int inHeight);

	void Start(int inWidth, int inHeight);

	void End();
};

#if HITBOX_VIEW
class HitboxRenderer {
	public:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	Hitbox* hit;
	HitboxRenderer();
	void Update();
	void Update(Hitbox* next);
};
#endif

class Transform {
	public: 
	float pitch = 0, yaw = 0, roll = 0;
	glm::vec3 position = glm::vec3(0,0,0);
	glm::vec3 scale = glm::vec3(1,1,1);

	glm::mat4 transformMatrix = glm::mat4(1.f);
	glm::mat4 invTransformMatrix = glm::mat4(1.f);
	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::mat4 translationMatrix = glm::mat4(1.f);
	glm::mat4 scaleMatrix = glm::mat4(1.f);

	Transform* parent = NULL;

	void UpdateRotation();
};

class Camera : public Transform {
	public:
	float fov;
	Camera();
};

struct Material {
	GLuint diffuseTex;
	GLuint specularTex;
	GLuint normal;
	float shininess;
	bool useLighting = 1;

	// praying i dont forget to add these later and not even religious
	// i didnt forget to add them later :33
	glm::vec4 specularColor = glm::vec4(1., 1., 1., 1.);
	glm::vec4 diffuseColor = glm::vec4(1., 1., 1., 1.);
	
	bool useDiffuseTex = 1;
	bool useSpecularTex = 1;
	bool useNormalMap = 0;
};
enum CULL {
	NONE = 0,
	BACK = 1,
	FRONT = 2,
};

class Model : public Transform {
	public:
	unsigned int VAO;
	unsigned int VBO;
	bool usable = false;
	int vertCount;
	int cullType = 0;
	Material mat;

		Model(std::vector<float> obj, Material material);
		Model(Model *m);
	
		void Unload();
};

class Object : public Model {
	public:
		using Model::Model;
		Hitbox hitbox;

		void UpdateHitbox();
};

struct Light {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float innerCutOff;
	float outerCutOff;

	int type;
};

void setSceneLights(Shader shader, std::vector<Light*> lights);

void RenderModel(Model model, Shader shader);

