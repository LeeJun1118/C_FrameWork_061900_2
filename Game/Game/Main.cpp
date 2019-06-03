#include "Headers.h"


int dwMonsterTIme = 0;


Object* CreateObject();


void InitMonster(Object* _pMonster);
void MonsterProgress(Object* _pMonster[]);
void CreateMonster(Object* _pMonster[]);

int main(void) {

	Object* pMonster[5];

	srand(GetTickCount64());


	for (int i = 0; i < 5; i++)
	{
		pMonster[i] = CreateObject();
	}
	while (true)
	{
		Sleep(1000);
		
		MonsterProgress(pMonster);

	}
	system("pause");
}



Object* CreateObject()
{
	Object* pObj = (Object*)malloc(sizeof(Object));

	return pObj;
}



void InitMonster(Object* _pMonster)
{
	_pMonster->pName = (char*)"*";

	_pMonster->Tranpos.Position = Vector3(float(rand() % WINSIZE_X), 5.f);


}



void CreateMonster(Object* _pMonster[])
{
	
	for (int i = 0; i < 5; i++)
	{
		InitMonster(_pMonster[i]);
		

		_pMonster[i]->Tranpos.Position = Vector3(100.f, 100.f);
		
		_pMonster[i]->Tranpos.Position.y++;

		printf_s("%s", _pMonster[i]->pName);

	}
}
void MonsterProgress(Object* _pMonster[])
{


	CreateMonster(_pMonster);

}