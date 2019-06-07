#include "Headers.h"


//** 전역변수
SCENEID eSCENEID = SCENEID_MENU;
DWORD dwMonsterTime = 0;

int Collision = 0;
int iSleep = 0;
int iScore = 0;
int iGameover = 0;
int iLogoCheck = 0;


float fPlayerSpeed = 2;
int iPlayerCount = 0;
DWORD FrameTime = 250;

DWORD MenuFrame;
DWORD MenuCheck = 0;



//** 함수 전방선언
void SetScene(Object* _pPlayer[], Object* _pMonster[]);

Object* CreateObject();


void InitPlayer(Object* _pPlayer);
void PlayerProgress(Object* _pPlayer[], Object* _pMonster[]);
void PlayerRender(Object* _pPlayer[]);

void InitMonster(Object* _pMonster);
void MonsterProgress(Object* _pMonster[]);
void MonsterRender(Object* _pMonster[]);
void CreateMonster(Object* _pMonster[]);

void BackGroundRender();

void SetCursorPosiotion(int _ix, int _iy, char* _str, int _iColor);
void SetCursorPosiotionInteger(int _ix, int _iy, int _value);
void SetCursorColor(int _iColor);
DWORD InputKey();



int main(void) {
	//** rand Initialize
	srand(GetTickCount64());

	

	//** Create Player
	Object* pPlayer[PLAYER_MAX];

	//** MonsterList Initialize (= NULL)
	Object* pMonster[MONSTER_MAX];

	//** FrameTime Initialize
	DWORD dwTime = GetTickCount64();

	//** Create Monster Time Initialize
	dwMonsterTime = GetTickCount64();

	MenuFrame = GetTickCount64();


	while(true)
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

		if (GetAsyncKeyState(VK_RETURN))
			eSCENEID = SCENEID_STAGE;


		/*for (int i = 0; i < MONSTER_MAX; i++)
		{
			//각각을 동적할당
			_pMonster[i] = CreateObject();
			//초기화
			InitMonster(_pMonster[i]);
			_pMonster[i]->TransPos.Position = Vector3(float(rand() % (WINSIZEX - 5) + 2), 1.f);

		}*/
		
		if (GetAsyncKeyState(VK_RETURN))
			eSCENEID = SCENEID_STAGE;

		for (int i = 0; i < PLAYER_MAX; i++)
			_pPlayer[i] = NULL;

		_pPlayer[0] = CreateObject();
		InitPlayer(_pPlayer[0]);

		_pPlayer[0]->pName = (char*)"◎";

		iGameover = 0;
		iPlayerCount = 0;
		FrameTime = 280;

		for (int i = 0; i < MONSTER_MAX; i++)
			_pMonster[i] = NULL;

		break;
		
		

		eSCENEID = SCENEID_STAGE;

		break;
	case SCENEID_STAGE:
		BackGroundRender();

		//** Progress
		PlayerProgress(_pPlayer, _pMonster);
		MonsterProgress(_pMonster);

		//** Render
		PlayerRender(_pPlayer);
		MonsterRender(_pMonster);


		if (iGameover)
			eSCENEID = SCENEID_MENU;

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

	
	}


}


Object* CreateObject()
{
	Object* pObj = (Object*)malloc(sizeof(Object));

	return pObj;
}



void InitMonster(Object* _pMonster)
{
	

	_pMonster->pName = (char*)"★";
	
	
	//초기 위치(0,0)으로 설정
	_pMonster->TransPos.Position = Vector3(0.f, 0.f);
	
	//들어온 문자의 크기만큼 크기 초기화
	_pMonster->TransPos.Scale = Vector3((float)strlen(_pMonster->pName), 0.f);

	_pMonster->TransPos.eDirection = DIRID_DOWN;

}
void MonsterProgress(Object* _pMonster[])
{
	//** 일정 시간 마다..
	if (dwMonsterTime + 1500 < GetTickCount())
	{
		dwMonsterTime = GetTickCount();

		//** 몬스터를 생성.
		CreateMonster(_pMonster);


	}

}

void CreateMonster(Object* _pMonster[])
{

	//** 모든 몬스터 리스트를 확인.
	for (int i = 0; i < MONSTER_MAX; i++)
	{
		//** 만약에 몬스터가 없다면.....
		if (!_pMonster[i])
		{
			//** 몬스터를 생성.
			_pMonster[i] = CreateObject();

			//** 생성된 몬스터 초기화.
			InitMonster(_pMonster[i]);

			//** 몬스터의 위치를 X좌표가 랜덤하게 변경
			_pMonster[i]->TransPos.Position = Vector3(float(rand() % (WINSIZEX - 5) + 2), 1.f);
			

			//** 모든작업 종료된후 구문 탈출.
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


void InitPlayer(Object* _pPlayer)
{
	_pPlayer->pName = (char*)"■";
	_pPlayer->TransPos.eDirection = DIRID_RIGHT;
	_pPlayer->TransPos.Position = Vector3(2.f, 1.f);
	_pPlayer->TransPos.Scale = Vector3((float)strlen(_pPlayer->pName), 1.f);
}

void PlayerProgress(Object* _pPlayer[], Object* _pMonster[])
{
	DWORD dwKey = InputKey();

	if (dwKey & KEYID_UP && _pPlayer[0]->TransPos.eDirection != DIRID_DOWN)
		_pPlayer[0]->TransPos.eDirection = DIRID_UP;

	if (dwKey & KEYID_DOWN && _pPlayer[0]->TransPos.eDirection != DIRID_UP)
		_pPlayer[0]->TransPos.eDirection = DIRID_DOWN;

	if (dwKey & KEYID_LEFT && _pPlayer[0]->TransPos.eDirection != DIRID_RIGHT)
		_pPlayer[0]->TransPos.eDirection = DIRID_LEFT;

	if (dwKey & KEYID_RIGHT && _pPlayer[0]->TransPos.eDirection != DIRID_LEFT)
		_pPlayer[0]->TransPos.eDirection = DIRID_RIGHT;




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


void SetCursorPosiotion(int _ix, int _iy, char* _str, int _iColor)
{
	//** 좌표를 셋팅.
	COORD pos = { (SHORT)_ix, (SHORT)_iy };

	//** 셋팅된 자표로 콘솔 커서를 이동시킴.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	SetCursorColor(_iColor);

	//** 이동된 위치에 문자열 출력.
	cout << _str;
}

//** 입력된 (x, y)좌표에 정수 출력.
void SetCursorPosiotionInteger(int _ix, int _iy, int _value)
{
	//** 좌표를 셋팅.
	COORD pos = { (SHORT)_ix, (SHORT)_iy };

	//** 셋팅된 자표로 콘솔 커서를 이동시킴.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);



	//** 이동된 위치에 문자열 출력.
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
			SetCursorPosiotion(0, y, (char*)"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■", 15);
		}
		else
		{
			SetCursorPosiotion(0, y, (char*)"■", 15);
			SetCursorPosiotion(WINSIZEX - 2, y, (char*)"■", 15);

		}
	}
	SetCursorPosiotion(5, WINSIZEY - 1, (char*)"SCORE : ", 15);
	SetCursorPosiotionInteger(14, WINSIZEY - 1, iScore);
}