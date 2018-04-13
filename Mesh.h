#ifndef MESH_H
#define MESH_H
#include<d3d11.h>
#include<SimpleMath.h>
#include<vector>
#include<string>

class Mesh
{
public:

	struct Vertex
	{
		DirectX::SimpleMath::Vector3 v_xyz;
		DirectX::SimpleMath::Vector3 n_xyz;
		//DirectX::SimpleMath::Vector2 uv;
	};

	std::vector<int> indices;
	std::vector<Vertex> vertices;

private:
	DirectX::SimpleMath::Vector3 cDiffuse;
	DirectX::SimpleMath::Vector3 cAmbient;
	DirectX::SimpleMath::Vector3 cSpecular;
	DirectX::SimpleMath::Vector3 cEmissive;

	std::string diffuse_tPath;
	std::string normal_tPath;
	std::string objectName;

	float specularExponent;
	float opacity;
	
public:
	Mesh();
	~Mesh();

	void setObjectName(const std::string &in_string);
	void setDiffuse_tPath(const std::string &in_string);
	void setNormal_tPath(const std::string &in_string);
	void setSpecularExponent(const float &in_value);
	void setOpacity(const float &in_value);
	void setDiffuse(const DirectX::SimpleMath::Vector3 &in_vec);
	void setAmbient(const DirectX::SimpleMath::Vector3 &in_vec);
	void setSpecular(const DirectX::SimpleMath::Vector3 &in_vec);
	void setEmissive(const DirectX::SimpleMath::Vector3 &in_vec);

	DirectX::SimpleMath::Vector3 get_cDiffuse() const;
	DirectX::SimpleMath::Vector3 get_cAmbient() const;
	DirectX::SimpleMath::Vector3 get_cSpecular() const;
	DirectX::SimpleMath::Vector3 get_cEmissive() const;
	std::string getDiffuse_tPath() const;
	std::string getNormal_tPath() const;
	float getSpecularExp() const;
	float getOpacity() const;


};
#endif // !MESH_H
