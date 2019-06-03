#pragma once


typedef struct tagVector3
{
	float x, y, z;

	tagVector3() {}

	tagVector3(float fx, float fy)
		:x(fx), y(fy), z(0) {};

}Vector3;



typedef struct tagTrasnform
{
	Vector3 Position;
	Vector3 Scale;


}Trasnform;




typedef struct tagObject
{
	char* pName;

	Trasnform Tranpos;

	tagObject() {};

}Object;




