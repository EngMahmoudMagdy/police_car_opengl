/* »”„ «··Â «·—Õ„‰ «·—ÕÌ„

„À«· ⁄·Ï  Õ„Ì· «·„Ã”„« 
@purpose : wavefront obj loader ..
@properties: support vertex loading + texture coordinates loading 
			+ normal coordinates loading .
@missing: -there is no supporting for material loading .
		  -ignore "groups" propery,in obj files .

@Abdullah AlShammari - aboshammar@windowslive.com

*/
#pragma once
#include<vector>
#include<string>
#include<fstream>
using namespace std; 

struct Vertex
{
	double x,y,z;
};

struct Normal
{
	double xn,yn,zn;
};

struct TextCoord
{
	double s , t , r ;
};

struct FaceIndices
{
	// [0] index of vertex
	// [1] index of textcoord
	// [2] index of normalcoord
	int firstVertex[3]; 
	int secondVertex[3];
	int thirdVertex[3];
};

class ObjModel
{
	enum WavefrontTag
	{
		EWT_VERTEX,EWT_TEXTURE,EWT_NORMAL,EWT_FACE
	};
	ifstream inf;
	int parseInt(const std::string &s);//convert string to integer
	double parseDouble(const std::string &s);//convert string to double 
	void openFile();//open a file
	void readFile();//read a file line by line .
	
	/*-----------------------------
	   @func name   :  getType
	   @description : recognize the line type(v,vt,vn or f)
	   @return value: line type(EWT_VERTEX,EWT_TEXTURE,EWT_NORMAL or EWT_FACE)
	 -----------------------------*/ 
	int getType(const string &str);

	/*-----------------------------
	   @func name   :  toVertex
	   @description : convert line data to numbers and store them into vectorOfVertex
	   @return value: none .
	 -----------------------------*/ 
	void toVertex(const string &s);

	/*-----------------------------
	   @func name   :  toTexture
	   @description : convert line data to numbers and store them into vectorOfTexture
	   @return value: none .
	 -----------------------------*/ 
	void toTexture(const string &s);

	/*-----------------------------
	   @func name   :  toNormal
	   @description : convert line data to numbers and store them into vectorOfNormal
	   @return value: none .
	 -----------------------------*/ 
	void toNormal(const string &s);
	
	/*-----------------------------
	   @func name   :  toFace
	   @description : convert line data to numbers and store them into vectorOfFace
	   @return value: none .
	 -----------------------------*/ 
	void toFace(const string &s);
	bool hasNorCoord,hasTexCoord;

public:
	vector<Vertex>         vectorOfVertex;
	vector<Normal>         vectorOfNormal;
	vector<TextCoord>	   vectorOfTexture;
	vector<FaceIndices>    vectorOfFace;
	ObjModel();
	~ObjModel();
	bool load(const char*fileName);//start from here 
	inline bool hasNormalCoord(){return hasNorCoord;}
	inline bool hasTextureCoord(){return hasTexCoord;}

};