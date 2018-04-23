#include"Geometry.h"

Geometry::Geometry() {

	//Shading values
	this->cDif = Vector3(0.0f, 0.0f, 0.0f);
	this->cAmbient = Vector3(0.0f, 0.0f, 0.0f);
	this->cSpec = Vector3(0.0f, 0.0f, 0.0f);
	this->cEmissive = Vector3(0.0f, 0.0f, 0.0f);
	this->specExponent = 0.0f;
	this->opacity = 0.0f;

	//Texture paths
	this->difTexPath = "";
	this->normTexPath = "";

	//Name
	this->name = "";

	//Buffers
	this->vBuffer = nullptr;
	this->iBuffer = nullptr;

	//World Matrix
	this->world = Matrix::Identity;
	this->world *= Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
	this->world *= Matrix::CreateScale(Vector3(1000.0f, 1000.0f, 1000.0f));

	//Checks
	this->isColliding = false;
	this->isVisisble = false;

}

Geometry::~Geometry() {

	if (this->vBuffer != nullptr) {

		this->vBuffer->Release();

	}

	if (this->iBuffer != nullptr) {

		this->iBuffer->Release();

	}

}

//////////////////////MISC/////////////////////////
void Geometry::addVertex(Vertex newVertex) {

	
	this->vertices.push_back(newVertex);

}

void Geometry::addIndex(int newIndex) {

	this->indices.push_back(newIndex);

}

void Geometry::createBuffers(ID3D11Device* device) {

	//Create Vertex Buffer
	D3D11_BUFFER_DESC vBufferDesc;
	ZeroMemory(&vBufferDesc, sizeof(vBufferDesc));

	//Flags
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof(Vertex) * this->vertexCount;
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;

	//Data
	D3D11_SUBRESOURCE_DATA vBufferData;
	ZeroMemory(&vBufferData, sizeof(vBufferData));

	vBufferData.pSysMem = this->vertices.data();
	device->CreateBuffer(&vBufferDesc, &vBufferData, &this->vBuffer);

	//Create index buffer
	D3D11_BUFFER_DESC iBufferDesc;
	ZeroMemory(&iBufferDesc, sizeof(iBufferDesc));

	//Flags
	iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	iBufferDesc.ByteWidth = sizeof(int) * (this->faceCount * 3);
	iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBufferDesc.CPUAccessFlags = 0;
	iBufferDesc.MiscFlags = 0;

	//Data
	D3D11_SUBRESOURCE_DATA iBufferData;
	ZeroMemory(&iBufferData, sizeof(iBufferData));

	iBufferData.pSysMem = this->indices.data();
	device->CreateBuffer(&iBufferDesc, &iBufferData, &this->iBuffer);

}

void Geometry::writeToFile(void) {

	std::ofstream outFile;
	outFile.open("DataCheck.txt");

	outFile << this->indices.size() << "\n";
	std::string line = "";

	for (int i = 0; i < this->indices.size(); i++) {

		outFile << this->indices.at(i) << "\n";

	}

	outFile.close();


}

///////////////////////GET///////////////////////////
int Geometry::getVertexCount(void) const{

	return this->vertexCount;

}

int Geometry::getFaceCount(void) const{

	return this->faceCount;

}

Matrix Geometry::getWorld(void) const{

	return this->world;

}

ID3D11Buffer* Geometry::getVBuffer(void) {

	return this->vBuffer;

}

ID3D11Buffer* Geometry::getIBuffer(void) {

	return this->iBuffer;

}

std::vector<Geometry::Vertex> Geometry::getVertices(void) const{

	return this->vertices;

}

std::vector<int> Geometry::getIndices(void) const{

	return this->indices;

}

std::string Geometry::getName(void) const{

	return this->name;

}

Vector3 Geometry::getSpec(void) const{

	return this->cSpec;

}

Vector3 Geometry::getDif(void) const{

	return this->cDif;

}

Vector3 Geometry::getEmissive(void) const{

	return this->cEmissive;

}

float Geometry::getSpecExponent(void) const{

	return this->specExponent;

}

float Geometry::getOpaciy(void) const{

	return this->opacity;

}

//////////////////////////SET////////////////////////////
void Geometry::setDiffuse(Vector3 dif) {

	this->cDif = dif;

}

void Geometry::setAmbient(Vector3 ambient) {

	this->cAmbient = ambient;

}

void Geometry::setSpec(Vector3 spec) {

	this->cSpec = spec;

}

void Geometry::setEmissive(Vector3 emissive) {

	this->cEmissive = emissive;

}

void Geometry::setSpecExponent(float specExponent) {

	this->specExponent = specExponent;

}

void Geometry::setOpacity(float opacity) {

	this->opacity = opacity;

}

void Geometry::setName(std::string name) {

	this->name = name;

}

void Geometry::setDifTexPath(std::string path) {

	this->difTexPath = path;

}

void Geometry::setNormTexPath(std::string path) {

	this->normTexPath = path;

}

void Geometry::setVertexCount(int vertices) {

	this->vertexCount = vertices;

}

void Geometry::setFaceCount(int faces) {

	this->faceCount = faces;

}