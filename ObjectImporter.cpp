#include "ObjectImporter.h"
#include<windows.h>
#include<iostream>
#include<vector>
#include<assimp\scene.h>
#include<assimp\postprocess.h>


void ObjectImporter::debugString(const LPCWSTR &in_string, const aiReturn &in_param)
{
	if (AI_SUCCESS != in_param)
	{
		OutputDebugString(in_string);
	}
}

ObjectImporter::ObjectImporter()
{
}

ObjectImporter::~ObjectImporter()
{
}

bool ObjectImporter::importModel(const std::string & fname, std::vector<Mesh>* &returnMesh)
{

	const aiScene* nScene = this->importer.ReadFile(fname, aiProcess_Triangulate);

	//if the importer fails to read the specified file it returns NULL == 0
	if (!nScene)
	{
		OutputDebugString(reinterpret_cast<LPCWSTR>(this->importer.GetErrorString()));
		return false;
	}



	std::vector<Mesh>* meshes = new std::vector<Mesh>(nScene->mNumMeshes, Mesh());

	for (UINT32 i = 0; i < nScene->mNumMeshes; i++)
	{

		aiMesh * cMesh = nScene->mMeshes[i];
	
		unsigned int mIndx = cMesh->mMaterialIndex;
		aiMaterial* mtl = nScene->mMaterials[mIndx];
		

		//loop over current mesh vertices
		for (UINT32 n = 0; n < cMesh->mNumVertices; n++)
		{
			Mesh::Vertex vertex;
			//save per vertex data

			vertex.v_xyz.x = cMesh->mVertices[n].x;
			vertex.v_xyz.y = cMesh->mVertices[n].y;
			vertex.v_xyz.z = cMesh->mVertices[n].z;

			vertex.n_xyz.x = cMesh->mNormals[n].x;
			vertex.n_xyz.y = cMesh->mNormals[n].y;
			vertex.n_xyz.z = cMesh->mNormals[n].z;

			//vertex.uv.x = cMesh->mTextureCoords[n]->x;
			//vertex.uv.y = cMesh->mTextureCoords[n]->y;

			//put vertex into vector
			meshes->at(i).vertices.push_back(vertex);
		}

		//loop over faces
		for (UINT32 k = 0; k < cMesh->mNumFaces; k++)
		{
			aiFace cFace = cMesh->mFaces[k];

			for (UINT32 j = 0; j < cFace.mNumIndices; j++)
			{
				//save indices
				meshes->at(i).indices.push_back(cFace.mIndices[j]);
			}
		}

		//save current mesh name
		meshes->at(i).setObjectName(cMesh->mName.C_Str());

		//grab colorinformation
		aiReturn rValue;
		aiString aiPath;
		DirectX::SimpleMath::Vector3 vec3;
		aiColor3D cColor(0.0f, 0.0f, 0.0f);
		float nsSpecularExponent = 0.0f;
		float dOpacity = 0.0f;
	
		rValue = mtl->Get(AI_MATKEY_SHININESS, nsSpecularExponent);
		meshes->at(i).setSpecularExponent(nsSpecularExponent);
		this->debugString(LPCWSTR("NS failed import"), rValue);

		mtl->Get(AI_MATKEY_OPACITY, dOpacity);
		meshes->at(i).setOpacity(dOpacity);
		this->debugString(LPCWSTR("dOpacity failed import"), rValue);

		mtl->Get(AI_MATKEY_COLOR_DIFFUSE, cColor);
		vec3 = DirectX::SimpleMath::Vector3(cColor.r, cColor.g, cColor.b);
		meshes->at(i).setDiffuse(vec3);
		this->debugString(LPCWSTR("cDiffuse failed import"), rValue);

		mtl->Get(AI_MATKEY_COLOR_AMBIENT, cColor);
		vec3 = DirectX::SimpleMath::Vector3(cColor.r, cColor.g, cColor.b);
		meshes->at(i).setAmbient(vec3);
		this->debugString(LPCWSTR("cAmbient failed import"), rValue);

		mtl->Get(AI_MATKEY_COLOR_SPECULAR, cColor);
		vec3 = DirectX::SimpleMath::Vector3(cColor.r, cColor.g, cColor.b);
		meshes->at(i).setSpecular(vec3);
		this->debugString(LPCWSTR("cSpecular failed import"), rValue);

		mtl->Get(AI_MATKEY_COLOR_EMISSIVE, cColor);
		vec3 = DirectX::SimpleMath::Vector3(cColor.r, cColor.g, cColor.b);
		meshes->at(i).setEmissive(vec3);
		this->debugString(LPCWSTR("cEmissive failed import"), rValue);

		mtl->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);
		meshes->at(i).setDiffuse_tPath(aiPath.C_Str());
		this->debugString(LPCWSTR("tPath diffuse failed import"), rValue);

		mtl->GetTexture(aiTextureType_NORMALS, 0, &aiPath);
		meshes->at(i).setNormal_tPath(aiPath.C_Str());
		this->debugString(LPCWSTR("tPath normals failed import"), rValue);

	}

	//external pointer point to data
	returnMesh = meshes;
	
	//reset internal pointer
	meshes = nullptr;
	
	//return function
	return true;
}
