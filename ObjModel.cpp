#include"ObjModel.h"
#include<iostream>


ObjModel::ObjModel()
{
	 hasNorCoord=false;hasTexCoord=false;
}

ObjModel::~ObjModel()
{
	vectorOfFace.clear();
	vectorOfTexture.clear();
	vectorOfNormal.clear();
	vectorOfVertex.clear();
}

bool ObjModel::load(const char *fileName)
{
	try
	{
		inf.open(fileName,ios::in);
		readFile();
		inf.close();
		return true;
	}
	catch(std::exception e)
	{
		cout<<e.what()<<endl;
		inf.close();
		return false;
	}
}

void ObjModel::readFile()
{
	string s;
	while(getline(inf,s))
	{
		switch(getType(s))
		{

			case ObjModel::EWT_VERTEX :
				toVertex(s.substr(0,s.size()));
			break;

			case ObjModel::EWT_TEXTURE:
				 toTexture(s.substr(0,s.size()));
			break;

			case ObjModel::EWT_NORMAL:
			   	 toNormal(s.substr(0,s.size()));
			break;

			case ObjModel::EWT_FACE:
				 toFace(s.substr(0,s.size()));
			break;
		}
	}

}

int ObjModel::getType(const std::string &str)
{
	string temp;
	int x =str.find(" ");
	temp=str.substr(0,x);
	
	if(temp == "v")
		return ObjModel::EWT_VERTEX;
	else if(temp== "vt")
	{
		hasTexCoord=true;
		return ObjModel::EWT_TEXTURE;
	}
	else if(temp== "vn")
	{
		hasNorCoord=true;
		return ObjModel::EWT_NORMAL;
	}
	else if(temp== "f")
		return ObjModel::EWT_FACE;
	else
		return -1;
}

int ObjModel::parseInt(const std::string &s)
{
	int result = 0;
	result = atoi(s.c_str());
	return result ;
}

double ObjModel::parseDouble(const std::string &s)
{
	double result = 0;
	result = atof(s.c_str());
	return result ;
}

void ObjModel::toVertex(const std::string &s)
{
	/*
	@example:
	@(if s = "v 15.543 -2.543 0.43432)
	@after processing :
		v.x = 15.543
		v.y = -2.543
		v.z = 0.43432
	and push v into vectorOfVertex
	*/
	
	Vertex v;
	int min = s.find_first_not_of(" ",1);
	int max = s.find_first_of(" ",min);
	v.x = parseDouble(s.substr(min,max-min));

	for(int i=0;i<2;i++)
	{
		min = s.find_first_not_of(" ",max);
		max = s.find_first_of(" ",min);
		if(i==0)
			v.y = parseDouble(s.substr(min,max-min));
		else
			v.z = parseDouble(s.substr(min,max-min));
		

	}

	vectorOfVertex.push_back(v);

	
}


void ObjModel::toNormal(const std::string &s)
{
	/*
	@example:
	@(if s = "vn 1.0 -1.0 0)
	@after processing :
		n.xn = 1
		v.yn = -1
		v.z = 0
	and push n into vectorOfNormal
	*/

	Normal n;
	int min = s.find_first_not_of(" ",2);
	int max = s.find_first_of(" ",min);
	n.xn = parseDouble(s.substr(min,max-min));

	for(int i=0;i<2;i++)
	{
		min = s.find_first_not_of(" ",max);
		max = s.find_first_of(" ",min);
		if(i==0)
			n.yn = parseDouble(s.substr(min,max-min));
		else
			n.zn = parseDouble(s.substr(min,max-min));
		

	}

	vectorOfNormal.push_back(n);
}
void ObjModel::toTexture(const std::string &s)
{

	/*
	@example:
	@(if s = "vt 1.0 -1.0 0)
	@after processing :
		tc.s = 1
		tc.t = -1
		tc.r = 0
	and push tc into vectorOfTexture
	*/
	TextCoord tc;
	int min = s.find_first_not_of(" ",2);
	int max = s.find_first_of(" ",min);
	tc.s = parseDouble(s.substr(min,max-min));

	for(int i=0;i<2;i++)
	{
		min = s.find_first_not_of(" ",max);
		max = s.find_first_of(" ",min);
		if(i==0)
			tc.t = parseDouble(s.substr(min,max-min));
		else
			tc.r = parseDouble(s.substr(min,max-min));
		

	}

	vectorOfTexture.push_back(tc);

}

void ObjModel::toFace(const std::string &s)
{

	/*
	@example:
	@(if s = "f 1/2/4 5/6/7 8/9/10)
	@after processing :
		f.firstVertex[0] = 1//index of vertex
		f.firstVertex[1] = 2//index of texture
		f.firstVertex[2] = 3//index of normal
		f.secondVertex[0] = 5//index of vertex
		f.secondVertex[1] = 6//index of texture
		f.secondVertex[2] = 7//index of normal
		f.thirdVertex[0] = 8//index of vertex
		f.thirdVertex[1] = 9//index of texture
		f.thirdVertex[2] = 10//index of normal
	@else if s = "f 1//4 5//7 8//10")
	@after processing:
		f.firstVertex[0] = 1//index of vertex
		f.firstVertex[1] = 0//index of texture
		f.firstVertex[2] = 3//index of normal
		f.secondVertex[0] = 5//index of vertex
		f.secondVertex[1] = 0//index of texture
		f.secondVertex[2] = 7//index of normal
		f.thirdVertex[0] = 8//index of vertex
		f.thirdVertex[1] = 0//index of texture
		f.thirdVertex[2] = 10//index of normal
	@else if s = "f 1 5 8")
	@after processing:
		f.firstVertex[0] = 1//index of vertex
		f.firstVertex[1] = 0//index of texture
		f.firstVertex[2] = 0//index of normal
		f.secondVertex[0] = 5//index of vertex
		f.secondVertex[1] = 0//index of texture
		f.secondVertex[2] = 0//index of normal
		f.thirdVertex[0] = 8//index of vertex
		f.thirdVertex[1] = 0//index of texture
		f.thirdVertex[2] = 0//index of normal
	
	@finally: push f into vectorOfFace
	*/
		FaceIndices f;
		int min = s.find_first_not_of(" ",1);
		int max = s.find_first_of(" ",min);
		string temp = s.substr(min,max-min);
		int min2=0,max2=0;
	if(hasTexCoord  && hasNorCoord )// s="f x/y/z"
		{
 			min2 = temp.find_first_of("/",0);
			f.firstVertex[0] = atoi(temp.substr(0,min2).c_str()) - 1;

			max2 = temp.find_first_of("/",min2+1);
			f.firstVertex[1] = atoi(temp.substr(min2+1,max2).c_str()) - 1;

			f.firstVertex[2] = atoi(temp.substr(max2+1,temp.size()-1).c_str()) -1;
		}
	else if(!hasTexCoord  && hasNorCoord )	// s="f x//y"
		{

			 //v//n
			min2 = temp.find_first_of("/",0);
			f.firstVertex[0] = atoi(temp.substr(0,min2).c_str()) -1;
			max2 = temp.find_last_of("/",min2+1);
			f.firstVertex[1] = 0;
			f.firstVertex[2] = atoi(temp.substr(max2+1,temp.size()-1).c_str()) -1;
		}

	else // s="f x y z"
		{

			 //v 
			min2 = temp.find_first_of(" ",0);
			f.firstVertex[0] = atoi(temp.substr(0,min2).c_str()) -1;
			f.firstVertex[1] = 0;
			f.firstVertex[2] = 0;
		}

		min = s.find_first_not_of(" ",max);
		max = s.find_first_of(" ",min);
		temp = s.substr(min,max-min);
		min2=max2=0;
	if(hasTexCoord  && hasNorCoord )
		{
			// v/t/n
			min2 = temp.find_first_of("/",0);
			f.secondVertex[0] = atoi(temp.substr(0,min2).c_str()) -1;

			max2 = temp.find_first_of("/",min2+1);
			f.secondVertex[1] = atoi(temp.substr(min2+1,max2).c_str()) -1;

			f.secondVertex[2] = atoi(temp.substr(max2+1,temp.size()-1).c_str()) -1;
		}
		else if(!hasTexCoord  && hasNorCoord )	// s="f x//y"
		{
			 //v//n
			min2 = temp.find_first_of("/",0);
			f.secondVertex[0] = atoi(temp.substr(0,min2).c_str()) -1;
			max2 = temp.find_last_of("/",min2+1);
			f.secondVertex[1] = 0;
			f.secondVertex[2] = atoi(temp.substr(max2+1,temp.size()-1).c_str()) -1;
		}
	else // s="f x y z"
		{

			 //v 
			min2 = temp.find_first_of(" ",0);
			f.secondVertex[0] = atoi(temp.substr(0,min2).c_str()) -1;
			f.secondVertex[1] = 0;
			f.secondVertex[2] = 0;
		}

		min = s.find_first_not_of(" ",max);
		max = s.find_first_of(" ",min);
		temp = s.substr(min,max-min);
		min2=max2=0;
		if(hasTexCoord && hasNorCoord)
		{
		// v/t/n
			min2 = temp.find_first_of("/",0);
			f.thirdVertex[0] = atoi(temp.substr(0,min2).c_str()) -1;

			max2 = temp.find_first_of("/",min2+1);
			f.thirdVertex[1] = atoi(temp.substr(min2+1,max2).c_str()) -1;

			f.thirdVertex[2] = atoi(temp.substr(max2+1,temp.size()-1).c_str()) -1;
		}
		else if(!hasTexCoord  && hasNorCoord )	// s="f x//y"
		{
			 //v//n
			min2 = temp.find_first_of("/",0);
			f.thirdVertex[0] = atoi(temp.substr(0,min2).c_str()) -1;
			max2 = temp.find_last_of("/",min2+1);
			f.thirdVertex[1] = 0;
			f.thirdVertex[2] = atoi(temp.substr(max2+1,temp.size()-1).c_str()) -1;
		}
		else // s="f x y z"
		{

			 //v 
			min2 = temp.find_first_of(" ",0);
			f.thirdVertex[0] = atoi(temp.substr(0,min2).c_str()) -1;
			f.thirdVertex[1] = 0;
			f.thirdVertex[2] = 0;
		}

	vectorOfFace.push_back(f);



}

