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

Object* CreateObject();


void InitPlayer(Object* _pPlayer);
void PlayerProgress(Object* _pPlayer[],Object* _pBullet[]);
void PlayerRender(Object* _pPlayer[]);

void InitMonster(Object* _pMonster);
void MonsterProgress(Object* _pMonster[]);
void MonsterRender(Object* _pMonster[]);
void CreateMonster(Object* _pMonster[]);


void InitBullet(Object* _pBullet);
void CreateBullet(Object* _pBullet[], Object* _pPlayer[]);
void BulletRender(Object* _pBullet[]);
void BulletProgress(Object* _pBullet[], Object* _pMonster[]);



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

			// Collision == 1 �̸�(�浹������)
			if (Collision)
			{
				//0���� �ٲ��ش�.
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
		
		//�÷��̾� ���� ����
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			_pPlayer[i] = NULL;
			_pPlayer[i] = CreateObject();
			
		}
		//ù��° Player �ʱ�ȭ
		InitPlayer(_pPlayer[0]);
		_pPlayer[0]->pName = (char*)"��";
		

		iGameover = 0;
		iPlayerCount = 0;
		FrameTime = 280;


		//���� ���� ����
		for (int i = 0; i < MONSTER_MAX; i++)
			_pMonster[i] = NULL;

		//�Ѿ� ���� ����
		for (int i = 0; i < BULLET_MAX; i++)
			_pBullet[i] = NULL;

		break;

	case SCENEID_STAGE:
		BackGroundRender();

		//** Progress
		BulletProgress(_pBullet, _pMonster);
		PlayerProgress(_pPlayer,_pBullet);
		MonsterProgress(_pMonster);
		

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


//���� ����
Object* CreateObject()
{
	Object* pObj = (Object*)malloc(sizeof(Object));

	return pObj;
}

//�÷��̾� �ʱ�ȭ
void InitPlayer(Object* _pPlayer)
{
	_pPlayer->pName = (char*)"��";
	_pPlayer->TransPos.eDirection = DIRID_CENTER;
	_pPlayer->TransPos.Position = Vector3(15.f, 45.f);
	_pPlayer->TransPos.Scale = Vector3((float)strlen(_pPlayer->pName), 1.f);
}

//�÷��̾� �̵� 
void PlayerProgress(Object* _pPlayer[], Object* _pBullet[])
{
	DWORD dwKey = InputKey();


	//�Ѿ� �߻�
	if (dwKey & KEYID_SPACE)
		CreateBullet(_pBullet, _pPlayer);
	
	//��
	if (dwKey & KEYID_UP)
	{
		//1���� Ŭ������ �� �� ����
		if (_pPlayer[0]->TransPos.Position.y > 1)
			int(_pPlayer[0]->TransPos.Position.y -= fPlayerSpeed / 2);
	}
	
	//�Ʒ�
	if (dwKey & KEYID_DOWN)
	{
		if (_pPlayer[0]->TransPos.Position.y < (WINSIZEY - 3))
			int(_pPlayer[0]->TransPos.Position.y += fPlayerSpeed / 2);
	}
		
	//��
	if (dwKey & KEYID_LEFT)
	{
		if (_pPlayer[0]->TransPos.Position.x > 2)
			int(_pPlayer[0]->TransPos.Position.x -= fPlayerSpeed);
	}
	//��
	if (dwKey & KEYID_RIGHT)
	{
		if (_pPlayer[0]->TransPos.Position.x < (WINSIZEX - 4))
			int(_pPlayer[0]->TransPos.Position.x += fPlayerSpeed);
	}



	
}

//�÷��̾� �׷���
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

//���� �ʱ�ȭ 
void InitMonster(Object* _pMonster)
{
	_pMonster->pName = (char*)"��";
	_pMonster->TransPos.Position = Vector3(0.f, 0.f);
	
	_pMonster->TransPos.Scale = Vector3((float)strlen(_pMonster->pName), 1.f);
	_pMonster->TransPos.eDirection = DIRID_DOWN;
}

//���� �ð����� ���� ����
void MonsterProgress(Object* _pMonster[])
{
	//** ���� �ð� ����..
	if (dwMonsterTime + 1600 < GetTickCount64())
	{
		dwMonsterTime = GetTickCount64();

		//** ���͸� ����.
		CreateMonster(_pMonster);
	}


}

//���͸� �׷��� + ������ ������ 0.5�� ���� ������ ���� �������� �ϰ� �װŸ� int������ �� ��ȯ ���� �� ������ �浹
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
				_pMonster[i]->pName, 2);

			//���͸� ��� ������ ���� �Ѵ�.
			_pMonster[i]->TransPos.Position.y += 0.5;

			//���Ͱ� ������ �� �������� ������
			if (_pMonster[i]->TransPos.Position.y == WINSIZEY-2)
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
			//** ���͸� ����. (�� ������ ������ش�.)
			_pMonster[i] = CreateObject();

			//** ������ ���͸� �ʱ�ȭ. (������� ������ �ʱⰪ�� �ʱ�ȭ ���ش�.)
			InitMonster(_pMonster[i]);

			//** ������ ��ġ�� ������ ��ǥ�� ����.
			//_pMonster[i]->TransPos.Position = Vector3(float(rand() % (WINSIZEX - 5) + 2),30.f);
			_pMonster[i]->TransPos.Position = Vector3(15.f, 10.f);


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

//Ŀ�� ǥ�ø� �Ⱥ��̰� ���ش�.
void DisableCursor()
{
	CONSOLE_CURSOR_INFO Info;

	Info.bVisible = false;
	Info.dwSize = 1;

	SetConsoleCursorInfo(
		GetStdHandle(STD_OUTPUT_HANDLE), &Info
	);
}

//�Ѿ� �ʱ�ȭ
void InitBullet(Object* _pBullet)
{
	_pBullet->pName = (char*)"��";
	_pBullet->TransPos.eDirection = DIRID_CENTER;
	_pBullet->TransPos.Position = Vector3(0.f, 0.f);
	_pBullet->TransPos.Scale = Vector3(float(strlen(_pBullet->pName)), 1.f);

}

//�Ѿ� ����
void CreateBullet(Object* _pBullet[], Object* _pPlayer[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�Ѿ��� ������
		if (!_pBullet[i])
		{
			//�Ѿ��� �� ������ ������ش�.
			_pBullet[i] = CreateObject();

			//������� ������ ���� �ʱ�ȭ ���ش�.
			InitBullet(_pBullet[i]);

			//�Ѿ��� �����Ǵ� ��ġ�� Player������ �Ѵ�.
			_pBullet[i]->TransPos.Position = _pPlayer[0]->TransPos.Position;
		
			break;
		}
	}
	
}

//�Ѿ� �׷���
void BulletRender(Object* _pBullet[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�Ѿ��� ������
		if (_pBullet[i])
		{
			//�׷��ش�.
			SetCursorPosiotion( int(_pBullet[i]->TransPos.Position.x),
								int(_pBullet[i]->TransPos.Position.y-1),
							   _pBullet[i]->pName,4);
		}
	}
}


//for�������� �ؼ�  �ι�° for������ ���� �浹�� �Ͼ�� üũ�� �ϴ� ������ �ϳ� ������ش�.
//i�� �ٷ� ���� ���� ���� for�� ó�� ���� �� ���� check�� ���� �����ش�.
//�Ѿ� �̵� & �浹 ó��
void BulletProgress(Object* _pBullet[], Object* _pMonster[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�Ѿ��� �ִٸ�
		if (_pBullet[i])
		{
			//�Ѿ��� ���� �������.
			_pBullet[i]->TransPos.Position.y -= 0.5;

			//������ ���� �Ѿ��� �����ش�.
			if (_pBullet[i]->TransPos.Position.y == 1)
			{
				free(_pBullet[i]);
				_pBullet[i] = NULL;
			}

			//���Ϳ� �Ѿ��� ����Ʈ�� �ִٸ�
			if (_pMonster[i] && _pBullet[i])
			{
					// �Ѿ��� ������ �κ� > ������ ����
				if (_pBullet[i]->TransPos.Position.x  + _pBullet[i]->TransPos.Scale.x  > _pMonster[i]->TransPos.Position.x &&
					// �Ѿ��� ���� �κ� < ���� ������
					_pMonster[i]->TransPos.Position.x + _pMonster[i]->TransPos.Scale.x > _pBullet[i]->TransPos.Position.x &&
					_pMonster[i]->TransPos.Position.y + _pMonster[i]->TransPos.Scale.y + 0.5f > _pBullet[i]->TransPos.Position.y &&
					_pMonster[i]->TransPos.Position.y < _pBullet[i]->TransPos.Position.y + _pBullet[i]->TransPos.Scale.y + 0.5f)
				{
					//�Ѿ˰� ���͸� �����ش�.

					free(_pBullet[i]);
					_pBullet[i] = NULL;

					free(_pMonster[i]);
					_pMonster[i] = NULL;

					//�浹���� �ÿ� 1
					Collision = 1;

					//������ ��� ���߰� �Ѵ�.
					iSleep = 50;

					//���� ���ϱ�
					iScore += 5;
				}

			}
		}

	}
}

