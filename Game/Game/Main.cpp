#include "Headers.h"


//** ��������
SCENEID eSCENEID = SCENEID_MENU;
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


//** �Լ� ���漱��
void SetScene(Object* _pPlayer[], Object* _pMonster[], Object* _pBullet[]);
char* SetName();
Object* CreateObject();


void InitPlayer(Object* _pPlayer);
void PlayerProgress(Object* _pPlayer[], Object* _pMonster[]);
void PlayerRender(Object* _pPlayer[]);

void InitMonster(Object* _pMonster);
void MonsterProgress(Object* _pMonster[]);
void MonsterRender(Object* _pMonster[]);
void CreateMonster(Object* _pMonster[]);


void InitBullet(Object* _pBullet);
void CreateBullet(Object* _pBullet[], Object* _pPlayer[]);
void BulletRender(Object* _pBullet[]);
void BulletProgress(Object* _pBullet[], Object* _pPlayer[]);

void SetBulletPosition(Object* _pBullet, DIRID _eDir);


void BackGroundRender();
void DisableCursor();

void SetCursorPosiotion(int _ix, int _iy, char* _str, int _iColor);
void SetCursorPosiotionInteger(int _ix, int _iy, int _value);
void SetCursorColor(int _iColor);
DWORD InputKey();




int main(void)
{
	//�ܼ�â�� Ŀ�� ǥ�� ������
	DisableCursor();


	//** rand Initialize
	srand(GetTickCount64());

	
	//** Create Player
	Object* pPlayer[PLAYER_MAX];

	//** MonsterList Initialize (= NULL)
	Object* pMonster[MONSTER_MAX];

	Object* pBullet[BULLET_MAX];

	//** FrameTime Initialize
	DWORD dwTime = GetTickCount64();

	//** Create Monster Time Initialize
	dwMonsterTime = GetTickCount64();

	MenuFrame = GetTickCount64();

	while (true)
	{
		if (dwTime + FrameTime + iSleep < GetTickCount64())
		{
			dwTime = GetTickCount64();
			system("cls");

			if (Collision)
			{
				Collision = 0;
				iSleep = 0;
			}

			SetScene(pPlayer, pMonster,pBullet);
		}
	}

	system("pause");

	return 0;
}


void SetScene(Object* _pPlayer[], Object* _pMonster[], Object* _pBullet[])
{
	SetCursorColor(15);

	switch (eSCENEID)
	{
	case SCENEID_LOGO:
		

		eSCENEID = SCENEID_MENU;

		

		

		break;

	case SCENEID_MENU:
		printf_s("SCENEID_MENU");
		
		if (GetAsyncKeyState(VK_RETURN))
			eSCENEID = SCENEID_STAGE;

		for (int i = 0; i < PLAYER_MAX; i++)
			_pPlayer[i] = NULL;

		

		iGameover = 0;
		iPlayerCount = 0;
		FrameTime = 280;


		for (int i = 0; i < MONSTER_MAX; i++)
			_pMonster[i] = NULL;

		for (int i = 0; i < BULLET_MAX; i++)
			_pBullet[i] = NULL;

		break;

	case SCENEID_STAGE:
		BackGroundRender();

		//** Progress
		PlayerProgress(_pPlayer, _pMonster);
		MonsterProgress(_pMonster);
		BulletProgress(_pBullet, _pPlayer);

		//** Render
		PlayerRender(_pPlayer);
		MonsterRender(_pMonster);
		BulletRender(_pBullet);

		if (iGameover)
			eSCENEID = SCENEID_MENU;

		break;

	case SCENEID_STORE:
		printf_s("SCENEID_STORE\n");
		system("pause");
		break;

	case SCENEID_OPTION:
		printf_s("SCENEID_OPTION\n");
		system("pause");
		break;

	case SCENEID_EXIT:
		printf_s("SCENEID_EXIT\n");
		system("pause");
		break;
	}
}


char* SetName()
{
	char str[128] = "";

	printf_s("����� �����ΰ��� ?\n");	Sleep(1000);
	printf_s("�Է� : ");
	scanf_s("%s", str, 128);

	char* pName = (char*)malloc(strlen(str));
	strcpy(pName, str);

	return pName;
}

Object* CreateObject()
{
	Object* pObj = (Object*)malloc(sizeof(Object));

	return pObj;
}

void InitPlayer(Object* _pPlayer)
{
	_pPlayer->pName = (char*)"��";
	_pPlayer->TransPos.eDirection = DIRID_RIGHT;
	_pPlayer->TransPos.Position = Vector3(15.f, 45.f);
	_pPlayer->TransPos.Scale = Vector3((float)strlen(_pPlayer->pName), 1.f);
}

void PlayerProgress(Object* _pPlayer[], Object* _pMonster[])
{
	DWORD dwKey = InputKey();

	if (dwKey & KEYID_UP)
		_pPlayer[0]->TransPos.eDirection = DIRID_UP;

	if (dwKey & KEYID_DOWN)
		_pPlayer[0]->TransPos.eDirection = DIRID_DOWN;

	if (dwKey & KEYID_LEFT)
		_pPlayer[0]->TransPos.eDirection = DIRID_LEFT;

	if (dwKey & KEYID_RIGHT)
		_pPlayer[0]->TransPos.eDirection = DIRID_RIGHT;

	//�밢��



	for (int i = iPlayerCount; i > 0; i--)
	{
		if (_pPlayer[i])
		{
			_pPlayer[i]->TransPos.Position = _pPlayer[i - 1]->TransPos.Position;
			_pPlayer[i]->TransPos.eDirection = _pPlayer[i - 1]->TransPos.eDirection;
		}
	}




	switch (_pPlayer[0]->TransPos.eDirection)
	{
	case DIRID_UP:
		if (_pPlayer[0]->TransPos.Position.y > 1)
			int(_pPlayer[0]->TransPos.Position.y -= fPlayerSpeed / 2);
		else
			iGameover = 1;
		break;
	case DIRID_DOWN:
		if (_pPlayer[0]->TransPos.Position.y < (WINSIZEY - 3))
			int(_pPlayer[0]->TransPos.Position.y += fPlayerSpeed / 2);
		else
			iGameover = 1;
		break;
	case DIRID_LEFT:
		if (_pPlayer[0]->TransPos.Position.x > 2)
			int(_pPlayer[0]->TransPos.Position.x -= fPlayerSpeed);
		else
			iGameover = 1;
		break;
	case DIRID_RIGHT:
		if (_pPlayer[0]->TransPos.Position.x < (WINSIZEX - 4))
			int(_pPlayer[0]->TransPos.Position.x += fPlayerSpeed);
		else
			iGameover = 1;
		break;
	}


	for (int i = 0; i < MONSTER_MAX; i++)
	{
		if (_pMonster[i])
		{
			if (int(_pPlayer[0]->TransPos.Position.x + _pPlayer[0]->TransPos.Scale.x) > (int)_pMonster[i]->TransPos.Position.x &&
				int(_pMonster[i]->TransPos.Position.x + _pMonster[i]->TransPos.Scale.x) > (int)_pPlayer[0]->TransPos.Position.x &&
				(int)_pMonster[i]->TransPos.Position.y == (int)_pPlayer[0]->TransPos.Position.y)
			{
				free(_pMonster[i]);
				_pMonster[i] = NULL;

				if (FrameTime > 40)
					FrameTime -= 20;


				Collision = 1;
				iSleep = 50;
				iScore += 5;
			}
		}
	}
}

void PlayerRender(Object* _pPlayer[])
{
	for (int i = 0; i < iPlayerCount + 1; i++)
	{
		if (_pPlayer[i])
		{
			SetCursorPosiotion(
				int(_pPlayer[i]->TransPos.Position.x),
				int(_pPlayer[i]->TransPos.Position.y),
				_pPlayer[i]->pName, 13);
		}
	}
}


void InitMonster(Object* _pMonster)
{
	_pMonster->pName = (char*)"��";
	_pMonster->TransPos.Position = Vector3(0.f, 0.f);
	
	_pMonster->TransPos.Scale = Vector3((float)strlen(_pMonster->pName), 0.f);
	_pMonster->TransPos.eDirection = DIRID_DOWN;
}

void MonsterProgress(Object* _pMonster[])
{
	//** ���� �ð� ����..
	if (dwMonsterTime + 1500 < GetTickCount64())
	{
		dwMonsterTime = GetTickCount64();

		//** ���͸� ����.
		CreateMonster(_pMonster);
	}


}

void MonsterRender(Object* _pMonster[])
{
	//** ��� ���� ����Ʈ�� Ȯ��.
	for (int i = 0; i < MONSTER_MAX; i++)
	{
		//** ���࿡ ���� ����Ʈ�� ���Ͱ� �ִٸ�.....
		if (_pMonster[i])
		{
			SetCursorPosiotion(
				int(_pMonster[i]->TransPos.Position.x),
				int(_pMonster[i]->TransPos.Position.y),
				_pMonster[i]->pName, 13);

			//���͸� ��� ������ ���� �Ѵ�.
			_pMonster[i]->TransPos.Position.y++;

			//���Ͱ� ������ �� �������� ������
			if (_pMonster[i]->TransPos.Position.y == WINSIZEY)
			{
				free(_pMonster[i]);
				_pMonster[i] = NULL;
			}
				
		}
		
	}
}

void CreateMonster(Object* _pMonster[])
{
	//** ��� ���� ����Ʈ�� Ȯ��.
	for (int i = 0; i < MONSTER_MAX; i++)
	{
		//** ���࿡ ���� ����Ʈ�� ���Ͱ� ���ٸ�.....
		if (!_pMonster[i])
		{
			//** ���͸� ����.
			_pMonster[i] = CreateObject();

			//** ������ ���͸� �ʱ�ȭ.
			InitMonster(_pMonster[i]);

			//** ������ ��ġ�� ������ ��ǥ�� ����.
			_pMonster[i]->TransPos.Position = Vector3(float(rand() % (WINSIZEX - 5) + 2),30.f);

			//** ��� �۾��� ����� �� ���� Ż��.
			break;
		}
	}
}

void BackGroundRender()
{
	for (int y = 0; y < WINSIZEY - 1; y++)
	{
		if (y == 0 || y == (WINSIZEY - 2))
		{
			SetCursorPosiotion(0, y, (char*)"��������������������������", 15);//25��
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

//** �Էµ� (x, y)��ǥ�� ���ڿ� ���.
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

void DisableCursor()
{
	CONSOLE_CURSOR_INFO Info;

	Info.bVisible = false;
	Info.dwSize = 1;

	SetConsoleCursorInfo(
		GetStdHandle(STD_OUTPUT_HANDLE), &Info
	);
}

void InitBullet(Object* _pBullet)
{
	_pBullet->pName = (char*)"l";
	_pBullet->TransPos.eDirection = DIRID_CENTER;
	_pBullet->TransPos.Position = Vector3(0.f, 0.f);
	_pBullet->TransPos.Scale = Vector3(float(strlen(_pBullet->pName)), 1.f);

}

void CreateBullet(Object* _pBullet[],Object* _pPlayer[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (_pBullet[i])
		{
			_pBullet[i] = CreateObject();
			InitBullet(_pBullet[i]);

			_pBullet[i]->TransPos.Position = _pPlayer[i]->TransPos.Position;
			SetBulletPosition(_pBullet[i], _pBullet[i]->TransPos.eDirection);

			break;
		}
	}
	
}

void BulletRender(Object* _pBullet[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (_pBullet[i])
		{
			SetCursorPosiotion( int(_pBullet[i]->TransPos.Position.x),
							int(_pBullet[i]->TransPos.Position.y),
							   _pBullet[i]->pName,4);
		}
	}
}


void BulletProgress(Object* _pBullet[],Object* _pPlayer[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (_pBullet[i])
		{
			switch (_pBullet[i]->TransPos.eDirection)
			{
			case DIRID_UP:
				_pBullet[i]->TransPos.Position.y--;
				break;
			case DIRID_DOWN:
				_pBullet[i]->TransPos.Position.y++;
				break;
			case DIRID_LEFT:
				_pBullet[i]->TransPos.Position.x--;
				break;
			case DIRID_RIGHT:
				_pBullet[i]->TransPos.Position.x++;
				break;
			}
		}

	}
}

void SetBulletPosition(Object* _pBullet, DIRID _eDir)
{
	_pBullet->TransPos.eDirection = _eDir;
}