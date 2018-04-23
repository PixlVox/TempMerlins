#include "ObjectImporter.h"
#include<windows.h>
#include<iostream>
#include<vector>
#include<assimp\scene.h>
#include<assimp\postprocess.h>


void ObjectImporter::debugString(const LPCSTR &in_string, const aiReturn &in_param)
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

bool ObjectImporter::importModel(const std::string & fname, std::vector<Geometry>& objects){

	const aiScene* nScene = this->importer.ReadFile(fname, aiProcess_Triangulate);
	//const aiScene* nScene = this->importer.ReadFile(fname, NULL);

	//if the importer fails to read the specified file it returns NULL == 0
	if (!nScene)
	{
		OutputDebugString(reinterpret_cast<LPCSTR>(this->importer.GetErrorString()));
		return false;
	}

	for (UINT32 i = 0; i < nScene->mNumMeshes; i++)
	{

		//Current mesh
		aiMesh * cMesh = nScene->mMeshes[i];
		unsigned int mIndx = cMesh->mMaterialIndex;
		aiMaterial* mtl = nScene->mMaterials[mIndx];
	
		//Current Object
		Geometry tempO;

		//Iterate vertex values for mesh and store them
		for (UINT32 n = 0; n < cMesh->mNumVertices; n++){

			//Vertex struct
			Geometry::Vertex vertex;

			//Copy values
			vertex.pos.x = cMesh->mVertices[n].x;
			vertex.pos.y = cMesh->mVertices[n].y;
			vertex.pos.z = cMesh->mVertices[n].z;

			vertex.norm.x = cMesh->mNormals[n].x;
			vertex.norm.y = cMesh->mNormals[n].y;
			vertex.norm.z = cMesh->mNormals[n].z;
			vertex.norm.w = 1.0f;

			//vertex.uv.x = cMesh->mTextureCoords[n]->x;
			//vertex.uv.y = cMesh->mTextureCoords[n]->y;

			//Store vertex
			tempO.addVertex(vertex);
		}

		//Iterate indices and store them
		for (UINT32 k = 0; k < cMesh->mNumFaces; k++)
		{
			
			for (UINT32 j = 0; j < cMesh->mFaces[k].mNumIndices; j++)
			{
				//save indices
				tempO.addIndex(cMesh->mFaces[k].mIndices[j]);
			}
		}

		//Vertex/Face count
		tempO.setVertexCount(cMesh->mNumVertices);
		tempO.setFaceCount(cMesh->mNumFaces);

		//save current mesh name
		tempO.setName(cMesh->mName.C_Str());

		//grab colorinformation
		aiReturn rValue;
		aiString aiPath;
		DirectX::SimpleMath::Vector3 vec3;
		aiColor3D cColor(0.0f, 0.0f, 0.0f);
		float nsSpecularExponent = 0.0f;
		float dOpacity = 0.0f;
	
		rValue = mtl->Get(AI_MATKEY_SHININESS, nsSpecularExponent);
		tempO.setSpecExponent(nsSpecularExponent);
		this->debugString(LPCSTR("NS failed import"), rValue);

		mtl->Get(AI_MATKEY_OPACITY, dOpacity);
		tempO.setOpacity(dOpacity);
		this->debugString(LPCSTR("dOpacity failed import"), rValue);

		mtl->Get(AI_MATKEY_COLOR_DIFFUSE, cColor);
		vec3 = DirectX::SimpleMath::Vector3(cColor.r, cColor.g, cColor.b);
		tempO.setDiffuse(vec3);
		this->debugString(LPCSTR("cDiffuse failed import"), rValue);

		mtl->Get(AI_MATKEY_COLOR_AMBIENT, cColor);
		vec3 = DirectX::SimpleMath::Vector3(cColor.r, cColor.g, cColor.b);
		tempO.setAmbient(vec3);
		this->debugString(LPCSTR("cAmbient failed import"), rValue);

		mtl->Get(AI_MATKEY_COLOR_SPECULAR, cColor);
		vec3 = DirectX::SimpleMath::Vector3(cColor.r, cColor.g, cColor.b);
		tempO.setSpec(vec3);
		this->debugString(LPCSTR("cSpecular failed import"), rValue);

		mtl->Get(AI_MATKEY_COLOR_EMISSIVE, cColor);
		vec3 = DirectX::SimpleMath::Vector3(cColor.r, cColor.g, cColor.b);
		tempO.setEmissive(vec3);
		this->debugString(LPCSTR("cEmissive failed import"), rValue);

		mtl->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);
		tempO.setDifTexPath(aiPath.C_Str());
		this->debugString(LPCSTR("tPath diffuse failed import"), rValue);

		mtl->GetTexture(aiTextureType_NORMALS, 0, &aiPath);
		tempO.setNormTexPath(aiPath.C_Str());
		this->debugString(LPCSTR("tPath normals failed import"), rValue);

		objects.push_back(tempO);

	}
	
	//return function
	return true;
}
