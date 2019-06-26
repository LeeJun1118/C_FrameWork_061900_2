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
DWORD FrameTime = 280;

DWORD MenuFrame;
DWORD MenuCheck = 0;


//** 함수 전방선언
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
	//콘솔창에 커서 표시 없애줌
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

			// Collision == 1 이면(충돌했으면)
			if (Collision)
			{
				//0으로 바꿔준다.
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
		
		//플레이어 공간 생성
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			_pPlayer[i] = NULL;
			_pPlayer[i] = CreateObject();
			
		}
		//첫번째 Player 초기화
		InitPlayer(_pPlayer[0]);
		_pPlayer[0]->pName = (char*)"♠";
		

		iGameover = 0;
		iPlayerCount = 0;
		FrameTime = 280;


		//몬스터 공간 생성
		for (int i = 0; i < MONSTER_MAX; i++)
			_pMonster[i] = NULL;

		//총알 공간 생성
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


//공간 생성
Object* CreateObject()
{
	Object* pObj = (Object*)malloc(sizeof(Object));

	return pObj;
}

//플레이어 초기화
void InitPlayer(Object* _pPlayer)
{
	_pPlayer->pName = (char*)"■";
	_pPlayer->TransPos.eDirection = DIRID_CENTER;
	_pPlayer->TransPos.Position = Vector3(15.f, 45.f);
	_pPlayer->TransPos.Scale = Vector3((float)strlen(_pPlayer->pName), 1.f);
}

//플레이어 이동 
void PlayerProgress(Object* _pPlayer[], Object* _pBullet[])
{
	DWORD dwKey = InputKey();


	//총알 발사
	if (dwKey & KEYID_SPACE)
		CreateBullet(_pBullet, _pPlayer);
	
	//위
	if (dwKey & KEYID_UP)
	{
		//1보다 클때까지 갈 수 있음
		if (_pPlayer[0]->TransPos.Position.y > 1)
			int(_pPlayer[0]->TransPos.Position.y -= fPlayerSpeed / 2);
	}
	
	//아래
	if (dwKey & KEYID_DOWN)
	{
		if (_pPlayer[0]->TransPos.Position.y < (WINSIZEY - 3))
			int(_pPlayer[0]->TransPos.Position.y += fPlayerSpeed / 2);
	}
		
	//좌
	if (dwKey & KEYID_LEFT)
	{
		if (_pPlayer[0]->TransPos.Position.x > 2)
			int(_pPlayer[0]->TransPos.Position.x -= fPlayerSpeed);
	}
	//우
	if (dwKey & KEYID_RIGHT)
	{
		if (_pPlayer[0]->TransPos.Position.x < (WINSIZEX - 4))
			int(_pPlayer[0]->TransPos.Position.x += fPlayerSpeed);
	}



	
}

//플레이어 그려줌
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

//몬스터 초기화 
void InitMonster(Object* _pMonster)
{
	_pMonster->pName = (char*)"★";
	_pMonster->TransPos.Position = Vector3(0.f, 0.f);
	
	_pMonster->TransPos.Scale = Vector3((float)strlen(_pMonster->pName), 1.f);
	_pMonster->TransPos.eDirection = DIRID_DOWN;
}

//일정 시간마다 몬스터 생성
void MonsterProgress(Object* _pMonster[])
{
	//** 일정 시간 마다..
	if (dwMonsterTime + 1600 < GetTickCount64())
	{
		dwMonsterTime = GetTickCount64();

		//** 몬스터를 생성.
		CreateMonster(_pMonster);
	}


}

//몬스터를 그려줌 + 밑으로 움직임 0.5씩 서로 밑으로 위로 떨어지게 하고 그거를 int형으로 형 변환 햇을 때 같으면 충돌
void MonsterRender(Object* _pMonster[])
{
	//** 모든 몬스터 리스트를 확인.
	for (int i = 0; i < MONSTER_MAX; i++)
	{
		//** 만약에 몬스터 리스트에 몬스터가 있다면.....
		if (_pMonster[i])
		{
			SetCursorPosiotion(
				int(_pMonster[i]->TransPos.Position.x),
				int(_pMonster[i]->TransPos.Position.y),
				_pMonster[i]->pName, 2);

			//몬스터를 계속 밑으로 가게 한다.
			_pMonster[i]->TransPos.Position.y += 0.5;

			//몬스터가 밑으로 다 내려가면 없어짐
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
	//** 모든 몬스터 리스트를 확인.
	for (int i = 0; i < MONSTER_MAX; i++)
	{
		//** 만약에 몬스터 리스트에 몬스터가 없다면.....
		if (!_pMonster[i])
		{
			//** 몬스터를 생성. (들어갈 공간을 만들어준다.)
			_pMonster[i] = CreateObject();

			//** 생성된 몬스터를 초기화. (만들어진 공간에 초기값을 초기화 해준다.)
			InitMonster(_pMonster[i]);

			//** 몬스터의 위치를 랜덤한 좌표로 변경.
			//_pMonster[i]->TransPos.Position = Vector3(float(rand() % (WINSIZEX - 5) + 2),30.f);
			_pMonster[i]->TransPos.Position = Vector3(15.f, 10.f);


			//** 모든 작업이 종료된 후 구문 탈출.
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
			SetCursorPosiotion(0, y, (char*)"■■■■■■■■■■■■■■■■■■■■■■■■■", 15);//25개
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

//** 입력된 (x, y)좌표에 문자열 출력.
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

//커서 표시를 안보이게 해준다.
void DisableCursor()
{
	CONSOLE_CURSOR_INFO Info;

	Info.bVisible = false;
	Info.dwSize = 1;

	SetConsoleCursorInfo(
		GetStdHandle(STD_OUTPUT_HANDLE), &Info
	);
}

//총알 초기화
void InitBullet(Object* _pBullet)
{
	_pBullet->pName = (char*)"Λ";
	_pBullet->TransPos.eDirection = DIRID_CENTER;
	_pBullet->TransPos.Position = Vector3(0.f, 0.f);
	_pBullet->TransPos.Scale = Vector3(float(strlen(_pBullet->pName)), 1.f);

}

//총알 생성
void CreateBullet(Object* _pBullet[], Object* _pPlayer[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//총알이 없으면
		if (!_pBullet[i])
		{
			//총알이 들어갈 공간을 만들어준다.
			_pBullet[i] = CreateObject();

			//만들어준 공간의 값을 초기화 해준다.
			InitBullet(_pBullet[i]);

			//총알이 생성되는 위치를 Player앞으로 한다.
			_pBullet[i]->TransPos.Position = _pPlayer[0]->TransPos.Position;
		
			break;
		}
	}
	
}

//총알 그려줌
void BulletRender(Object* _pBullet[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//총알이 있으면
		if (_pBullet[i])
		{
			//그려준다.
			SetCursorPosiotion( int(_pBullet[i]->TransPos.Position.x),
								int(_pBullet[i]->TransPos.Position.y-1),
							   _pBullet[i]->pName,4);
		}
	}
}


//for이중으로 해서  두번째 for문들어가기 전에 충돌이 일어나면 체크를 하는 변수를 하나 만들어준다.
//i를 바로 지울 수는 없다 for이 처음 돌고 난 다음 check를 보고 없애준다.
//총알 이동 & 충돌 처리
void BulletProgress(Object* _pBullet[], Object* _pMonster[])
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//총알이 있다면
		if (_pBullet[i])
		{
			//총알이 위로 쏘아진다.
			_pBullet[i]->TransPos.Position.y -= 0.5;

			//끝까지 가면 총알을 없애준다.
			if (_pBullet[i]->TransPos.Position.y == 1)
			{
				free(_pBullet[i]);
				_pBullet[i] = NULL;
			}

			//몬스터와 총알이 리스트에 있다면
			if (_pMonster[i] && _pBullet[i])
			{
					// 총알의 오른쪽 부분 > 몬스터의 왼쪽
				if (_pBullet[i]->TransPos.Position.x  + _pBullet[i]->TransPos.Scale.x  > _pMonster[i]->TransPos.Position.x &&
					// 총알의 왼쪽 부분 < 몬스터 오른쪽
					_pMonster[i]->TransPos.Position.x + _pMonster[i]->TransPos.Scale.x > _pBullet[i]->TransPos.Position.x &&
					_pMonster[i]->TransPos.Position.y + _pMonster[i]->TransPos.Scale.y + 0.5f > _pBullet[i]->TransPos.Position.y &&
					_pMonster[i]->TransPos.Position.y < _pBullet[i]->TransPos.Position.y + _pBullet[i]->TransPos.Scale.y + 0.5f)
				{
					//총알과 몬스터를 없애준다.

					free(_pBullet[i]);
					_pBullet[i] = NULL;

					free(_pMonster[i]);
					_pMonster[i] = NULL;

					//충돌햇을 시에 1
					Collision = 1;

					//맞으면 잠깐 멈추게 한다.
					iSleep = 50;

					//점수 더하기
					iScore += 5;
				}

			}
		}

	}
}

