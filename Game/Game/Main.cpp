#include "Headers.h"


//** ��������
SCENEID eSCENEID = SCENEID_STAGE;
DWORD dwMonsterTime = 0;

int Collision = 0;
int iSleep = 0;
int iScore = 0;
int iGameover = 0;
int iLogoCheck = 0;


float fPlayerSpeed = 2;
int iPlayerCount = 0;
DWORD FrameTime = 280;

DWORD MenuFrame;
DWORD MenuCheck = 0;

Logo* pLogo[13];


//�Լ� ���� ����
void SetScene(Object* _pPlayer[], Object* _pMonster[]);

Object* CreateObject();

void InitMonster(Object* _pMonster);
void MonsterProgress(Object* _pMonster[]);
void CreateMonster(Object* _pMonster[]);
void MonsterRender(Object* _pMonster[]);

void SetCursorPosiotion(int _ix, int _iy, char* _str, int _iColor);
void SetCursorPosiotionInteger(int _ix, int _iy, int _value);
void SetCursorColor(int _iColor);
void BackGroundRender();
DWORD InputKey();

int main(void) {
	//** rand Initialize
	srand(GetTickCount());

	//** Create Logo
	for (int i = 0; i < 13; i++)
		pLogo[i] = (Logo*)malloc(sizeof(Logo));

	//** Logo Initialize
	for (int i = 0; i < 13; i++)
		pLogo[i]->TransPos.Position = Vector3(20.f, int(5.f + i));


	pLogo[0]->pName = (char*)"##                       #";
	pLogo[1]->pName = (char*)" ##                     ##";
	pLogo[2]->pName = (char*)" ##          ##        ##";
	pLogo[3]->pName = (char*)" ##          ##        ##         ###     #### #   ###  ##";
	pLogo[4]->pName = (char*)" ##          ##        ##   ####   ### ######  ## #### ####";
	pLogo[5]->pName = (char*)" ##          ##        ##  ######   #####       ##  ####  ##";
	pLogo[6]->pName = (char*)" ##          ##        ## ##    ##  ##          ##   ##   ##";
	pLogo[7]->pName = (char*)" ##          ##        ## ##    ##  ##          ##   ##   ##";
	pLogo[8]->pName = (char*)"  ##         ##        ## ##    ##  ##          ##   ##   ##";
	pLogo[9]->pName = (char*)"   ##        ##        #  ##    ##  ##          ##   ##   ##";
	pLogo[10]->pName = (char*)"    ###     ####      #   ##    ##  ##          ##   ##   ##";
	pLogo[11]->pName = (char*)"     ########  #######     ######   ###         ###  ###  ###";
	pLogo[12]->pName = (char*)"       ####     ####        ####    ###         ###  ###  ###";




	//** Create Player
	Object* pPlayer[PLAYER_MAX];

	//** MonsterList Initialize (= NULL)
	Object* pMonster[MONSTER_MAX];

	//** FrameTime Initialize
	DWORD dwTime = GetTickCount();

	//** Create Monster Time Initialize
	dwMonsterTime = GetTickCount();

	MenuFrame = GetTickCount();

	for (int i = 0; i < MONSTER_MAX; i++)
	{
		pMonster[i] = CreateObject();
		InitMonster(pMonster[i]);

	}

	while(true)
	{
		

		if (dwTime + FrameTime + iSleep < GetTickCount())
		{
		
			dwTime = GetTickCount();
			system("cls");

			if (Collision)
			{
				Collision = 0;
				iSleep = 0;
			}
	
			SetScene(pPlayer, pMonster);
		}
	}
	system("pause");
}

void SetScene(Object* _pPlayer[], Object* _pMonster[])
{
	SetCursorColor(15);

	switch (eSCENEID)
	{
	case SCENEID_LOGO:
		printf_s("SCENEID_LOGO\n");

		eSCENEID = SCENEID_MENU;

		break;
	case SCENEID_MENU:
		printf_s("SCENEID_MENU\n");

		eSCENEID = SCENEID_STAGE;

		break;
	case SCENEID_STAGE:
		BackGroundRender();
		printf_s("SCENEID_STAGE\n");


		MonsterProgress(_pMonster);
		
		MonsterRender(_pMonster);
		
		if (iGameover)
			eSCENEID = SCENEID_MENU;

		eSCENEID = SCENEID_MENU;

		break;
	case SCENEID_STORE:
		printf_s("SCENEID_STORE\n");

		break;
	case SCENEID_OPTION:
		printf_s("SCENEID_OPTION\n");
		break;

	
	}


}


Object* CreateObject()
{
	Object* pObj = (Object*)malloc(sizeof(Object));

	return pObj;
}



void InitMonster(Object* _pMonster)
{
	printf_s("InitMonster");

	_pMonster->pName = (char*)"*";
	
	//�ʱ� ��ġ(0,0)���� ����
	_pMonster->TransPos.Position = Vector3(0.f, 0.f);

	//���� ������ ũ�⸸ŭ ũ�� �ʱ�ȭ
	_pMonster->TransPos.Scale = Vector3((float)strlen(_pMonster->pName), 0.f);

	//�̵� ���� �ʱ�ȭ
	_pMonster->TransPos.eDirection = DIRID_DOWN;


}
void MonsterProgress(Object* _pMonster[])
{
	printf_s("MonsterProgress");

	//** ���� �ð� ����..
	if (dwMonsterTime + 1500 < GetTickCount())
	{
		dwMonsterTime = GetTickCount();

		//** ���͸� ����.
		CreateMonster(_pMonster);
	}
	

}

void CreateMonster(Object* _pMonster[])
{
	printf_s("CreateMonster");
	//���� ����Ʈ�� �ϳ��ϳ� Ȯ���ؾ��Ѵ�.
	for (int i = 0; i < MONSTER_MAX; i++)
	{
		//printf_s("CreateMonsterd�� if�� ��");
		//���� ����Ʈ�� ���Ͱ� ���ٸ�
		if (!_pMonster[i])
		{
			printf_s("CreateMonsterd�� if�� ��");
			//���͸� ���� ������ ����
			_pMonster[i] = CreateObject();

			//���� ����,������� �ʱ�ȭ
			printf_s("InitMonster��");
			InitMonster(_pMonster[i]);

			//Init���� (0,0)���� �ʱ�ȭ�� ���� ��ġ�� ������ ��ǥ�� ����
			_pMonster[i]->TransPos.Position = Vector3(
				float(rand() % (WINSIZEX -5 ) + 2), 
				float(rand() % (WINSIZEX - 3) + 1));
		
			break;
		
		}


	}

}

void MonsterRender(Object* _pMonster[])
{
	
	for (int i = 0; i < MONSTER_MAX; i++)
	{
		if (_pMonster[i])
		{
			SetCursorPosiotion(
				int(_pMonster[i]->TransPos.Position.x),
				int(_pMonster[i]->TransPos.Position.y),
				_pMonster[i]->pName, 13);
							   
		}

	}
	
}


void SetCursorPosiotion(int _ix, int _iy, char* _str, int _iColor)
{
	//** ��ǥ�� ����.
	COORD pos = { (SHORT)_ix, (SHORT)_iy };

	//** ���õ� ��ǥ�� �ܼ� Ŀ���� �̵���Ŵ.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	SetCursorColor(_iColor);

	//** �̵��� ��ġ�� ���ڿ� ���.
	cout << _str;
}

//** �Էµ� (x, y)��ǥ�� ���� ���.
void SetCursorPosiotionInteger(int _ix, int _iy, int _value)
{
	//** ��ǥ�� ����.
	COORD pos = { (SHORT)_ix, (SHORT)_iy };

	//** ���õ� ��ǥ�� �ܼ� Ŀ���� �̵���Ŵ.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);



	//** �̵��� ��ġ�� ���ڿ� ���.
	cout << _value;
}

void SetCursorColor(int _iColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _iColor);


}
DWORD InputKey()
{
	DWORD dwInput = 0;

	if (GetAsyncKeyState(VK_UP))
		dwInput |= KEYID_UP;

	if (GetAsyncKeyState(VK_DOWN))
		dwInput |= KEYID_DOWN;

	if (GetAsyncKeyState(VK_LEFT))
		dwInput |= KEYID_LEFT;

	if (GetAsyncKeyState(VK_RIGHT))
		dwInput |= KEYID_RIGHT;

	if (GetAsyncKeyState(VK_SPACE))
		dwInput |= KEYID_SPACE;

	return dwInput;
}


void BackGroundRender()
{
	for (int y = 0; y < WINSIZEY - 1; y++)
	{
		if (y == 0 || y == (WINSIZEY - 2))
		{
			SetCursorPosiotion(0, y, (char*)"���������������������������������������������������", 15);
		}
		else
		{
			SetCursorPosiotion(0, y, (char*)"��", 15);
			SetCursorPosiotion(WINSIZEX - 2, y, (char*)"��", 15);

		}
	}
	SetCursorPosiotion(5, WINSIZEY - 1, (char*)"SCORE : ", 15);
	SetCursorPosiotionInteger(14, WINSIZEY - 1, iScore);
}