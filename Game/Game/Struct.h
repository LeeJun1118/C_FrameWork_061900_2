#pragma once


typedef struct tagVector3
{
	float x, y, z;

	tagVector3() {};

	tagVector3(float _x, float _y) :x(_x), y(_y), z(0) {};

	//³ªÁß¿¡ ¾µ²¨
	tagVector3(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z) {};

}Vector3;

typedef struct tagTransform
{
	Vector3 Position;
	Vector3 Scale;

	DIRID eDirection;

}Transform;


typedef struct tagObject
{
	char* pName;

	Transform TransPos;

	tagObject() {};

}Object;

typedef struct tagLogo
{
	char* pName;
	Transform TransPos;

	int iColor;

}Logo;
