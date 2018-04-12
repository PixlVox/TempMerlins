#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::setObjectName(const std::string & in_string)
{
	this->objectName = in_string;
}

void Mesh::setDiffuse_tPath(const std::string & in_string)
{
	this->diffuse_tPath = in_string;
}

void Mesh::setNormal_tPath(const std::string & in_string)
{
	this->normal_tPath = in_string;
}

void Mesh::setSpecularExponent(const float &in_value)
{
	this->specularExponent = in_value;
}

void Mesh::setOpacity(const float & in_value)
{
	this->opacity = in_value;
}

void Mesh::setDiffuse(const DirectX::SimpleMath::Vector3 &in_vec)
{
	this->cDiffuse = in_vec;
}

void Mesh::setAmbient(const DirectX::SimpleMath::Vector3 & in_vec)
{
	this->cAmbient = in_vec;
}

void Mesh::setSpecular(const DirectX::SimpleMath::Vector3 & in_vec)
{
	this->cSpecular = in_vec;
}

void Mesh::setEmissive(const DirectX::SimpleMath::Vector3 & in_vec)
{
	this->cEmissive = in_vec;
}

DirectX::SimpleMath::Vector3 Mesh::get_cDiffuse() const
{
	return this->cDiffuse;
}

DirectX::SimpleMath::Vector3 Mesh::get_cAmbient() const
{
	return this->cAmbient;
}

DirectX::SimpleMath::Vector3 Mesh::get_cSpecular() const
{
	return cSpecular;
}

DirectX::SimpleMath::Vector3 Mesh::get_cEmissive() const
{
	return this->cEmissive;
}

std::string Mesh::getDiffuse_tPath() const
{
	return this->diffuse_tPath;
}

std::string Mesh::getNormal_tPath() const
{
	return this->normal_tPath;
}

float Mesh::getSpecularExp() const
{
	return this->specularExponent;
}

float Mesh::getOpacity() const
{
	return this->opacity;
}
