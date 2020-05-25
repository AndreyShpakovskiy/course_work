﻿//Игра морской бой. Выполнили: Фролов Д.В. , Бардин В.С. . Версия 1.0

#include <iostream>
//#include "pch.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;
const int N = 11;
int corX, corY, winValue;
int fieldPlayer[N][N];
int fieldComp[N][N];
SOCKET newConnection;

void showEmptyField(int numberArray);

void enterLinkor();
void enterCruiser();
void enterDestroyer();
void enterBoat();

int ClientHandler();

int shoting();

void chekWounded();

int winGame();

int main()
{
	int temp = 0;
	int buff = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			fieldPlayer[i][j] = 0;
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			fieldComp[i][j] = 0;
		}
	}
	enterLinkor();
	enterCruiser();
	enterCruiser();
	enterDestroyer();
	enterDestroyer();
	enterDestroyer();
	enterBoat();
	enterBoat();
	enterBoat();
	enterBoat();
	showEmptyField(1);

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("192.168.88.138");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0)
	{
		std::cout << "Error #2\n";
	}
	do
	{
		do
		{
			buff = shoting();
			chekWounded();
		} while (buff != 2);
		do
		{
			temp = ClientHandler();
			chekWounded();
		} while (temp != 2);
		winValue = winGame();
	} while (winValue != 1);
}

void showEmptyField(int numberArray)
{
	switch (numberArray)
	{
	case 1:
		system("cls");
		std::cout << '\t' << "Our playing field:" << endl;
		for (int i = 0; i < 13; i++)//верхние координаты 1-10
		{
			if (i == 0 || i == 1 || i == 2)
			{
				std::cout << ' ';
				continue;
			}
			std::cout << i - 2;
		}
		std::cout << endl;
		std::cout << ' ' << ' ' << (char)201;
		for (int i = 0; i < 10; i++)// псевдографика верхней границы
		{
			std::cout << (char)205;
		}
		std::cout << (char)187 << endl;
		for (int i = 1; i < 10; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				if (j == 0)
				{
					std::cout << i;
				}
				else if (j == 1)
				{
					std::cout << ' ';
				}
				else if (j == 2)
				{
					std::cout << (char)186;
				}
				else if (j == 13)
				{
					std::cout << (char)186;
				}
				else if (fieldPlayer[i - 1][j - 3] == 1)
				{
					std::cout << (char)219;
				}
				else if (fieldPlayer[i - 1][j - 3] == 0)
				{
					std::cout << ' ';
				}
				else if (fieldPlayer[i - 1][j - 3] == 2)
				{
					std::cout << (char)42;
				}
				else if (fieldPlayer[i - 1][j - 3] == 3)
				{
					std::cout << (char)176;
				}
				else if (fieldPlayer[i - 1][j - 3] == 4)
				{
					std::cout << (char)88;
				}
			}
			std::cout << endl;
		}
		for (int i = 0; i < 14; i++)
		{
			if (i == 0)
			{
				std::cout << 1;
			}
			else if (i == 1)
			{
				std::cout << 0;
			}
			else if (i == 2)
			{
				std::cout << (char)186;
			}
			else if (i == 13)
			{
				std::cout << (char)186;
			}
			else if (fieldPlayer[9][i - 3] == 1)
			{
				std::cout << (char)219;
			}
			else if (fieldPlayer[9][i - 3] == 0)
			{
				std::cout << ' ';
			}
			else if (fieldPlayer[9][i - 3] == 2)
			{
				std::cout << (char)42;
			}
			else if (fieldPlayer[9][i - 3] == 3)
			{
				std::cout << (char)176;
			}
			else if (fieldPlayer[9][i - 3] == 4)
			{
				std::cout << (char)88;
			}
		}
		break;
	case 2:
		std::cout << "Opponent's playing field:" << endl;
		for (int i = 0; i < 13; i++)//верхние координаты 1-10
		{
			if (i == 0 || i == 1 || i == 2)
			{
				std::cout << ' ';
				continue;
			}
			std::cout << i - 2;
		}
		std::cout << endl;
		std::cout << ' ' << ' ' << (char)201;
		for (int i = 0; i < 10; i++)// псевдографика верхней границы
		{
			std::cout << (char)205;
		}
		std::cout << (char)187 << endl;
		for (int i = 1; i < 10; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				if (j == 0)
				{
					std::cout << i;
				}
				else if (j == 1)
				{
					std::cout << ' ';
				}
				else if (j == 2)
				{
					std::cout << (char)186;
				}
				else if (j == 13)
				{
					std::cout << (char)186;
				}
				else if (fieldComp[i - 1][j - 3] == 1)
				{
					std::cout << (char)219;
				}
				else if (fieldComp[i - 1][j - 3] == 0)
				{
					std::cout << ' ';
				}
				else if (fieldComp[i - 1][j - 3] == 2)
				{
					std::cout << (char)42;
				}
				else if (fieldComp[i - 1][j - 3] == 3)
				{
					std::cout << (char)176;
				}
				else if (fieldComp[i - 1][j - 3] == 4)
				{
					std::cout << (char)88;
				}
			}
			std::cout << endl;
		}
		for (int i = 0; i < 14; i++)
		{
			if (i == 0)
			{
				std::cout << 1;
			}
			else if (i == 1)
			{
				std::cout << 0;
			}
			else if (i == 2)
			{
				std::cout << (char)186;
			}
			else if (i == 13)
			{
				std::cout << (char)186;
			}
			else if (fieldComp[9][i - 3] == 1)
			{
				std::cout << (char)219;
			}
			else if (fieldComp[9][i - 3] == 0)
			{
				std::cout << ' ';
			}
			else if (fieldComp[9][i - 3] == 2)
			{
				std::cout << (char)42;
			}
			else if (fieldComp[9][i - 3] == 3)
			{
				std::cout << (char)176;
			}
			else if (fieldComp[9][i - 3] == 4)
			{
				std::cout << (char)88;
			}
		}
	}
	std::cout << endl;
	std::cout << ' ' << ' ' << (char)200;
	for (int i = 0; i < 10; i++)// псевдографика нижней границы
	{
		std::cout << (char)205;
	}
	std::cout << (char)188 << endl;
}

void enterLinkor()
{
	showEmptyField(1);
enter:
	cout << "Enter linkor coordinates(4 cells):" << endl;
	int X, Y, A, B, X1, Y1, A1, B1, h;
	do
	{
		cout << "Enter the first coordinate of the horizontal:"; cin >> X; cout << endl;
		cout << "Enter the first vertical coordinate:"; cin >> Y; cout << endl;
	} while (X < 1 || X > 10 || Y < 1 || Y>10);
enter1:
	do {
		cout << "Enter the second horizontal coordinate:"; cin >> A; cout << endl;
		cout << "Enter the second vertical coordinate:"; cin >> B; cout << endl;
	} while (A < 1 || A>10 || B < 1 || B>10);
	h = abs((X + Y) - (A + B));
	if (h != 1)
	{
		cout << "You have entered the coordinates of a neighboring square." << endl;
		goto enter1;
	}
enter2:
	do
	{
		cout << "Enter the third horizontal coordinate:"; cin >> X1; cout << endl;
		cout << "Enter the third vertical coordinate:"; cin >> Y1; cout << endl;
	} while (X1 < 1 || X1 > 10 || Y1 < 1 || Y1>10);
	h = abs((X1 + Y1) - (A + B));
	if (h != 1)
	{
		cout << "You have entered the coordinates of a neighboring square." << endl;
		goto enter2;
	}
enter3:
	do {
		cout << "Enter the fourth horizontal coordinate:"; cin >> A1; cout << endl;
		cout << "Enter the fourth vertical coordinate:"; cin >> B1; cout << endl;
	} while (A1 < 1 || A1>10 || B1 < 1 || B1>10);
	h = abs((X1 + Y1) - (A1 + B1));
	if (h != 1)
	{
		cout << "You have entered the coordinates of a neighboring square." << endl;
		goto enter3;
	}
	if ((X == X1 && X1 == A && A == A1) || (Y == Y1 && Y1 == B && B == B1))
	{
		fieldPlayer[A1 - 1][B1 - 1] = 1;
		fieldPlayer[A - 1][B - 1] = 1;
		fieldPlayer[X - 1][Y - 1] = 1;
		fieldPlayer[X1 - 1][Y1 - 1] = 1;
	}
	else
	{
		goto enter;
	}
	system("cls");
	showEmptyField(1);
}

void enterCruiser()
{
enter:
	cout << "Enter the coordinates of the cruiser(3 cells):" << endl;
	int X, Y, A, B, X1, Y1, h;
	do
	{
		cout << "Enter the first coordinate of the horizontal:"; cin >> X; cout << endl;
		cout << "Enter the first vertical coordinate:"; cin >> Y; cout << endl;
	} while (X < 1 || X > 10 || Y < 1 || Y>10);
enter1:
	do {
		cout << "Enter the second horizontal coordinate:"; cin >> A; cout << endl;
		cout << "Enter the second vertical coordinate:"; cin >> B; cout << endl;
	} while (A < 1 || A>10 || B < 1 || B>10);
	h = abs((X + Y) - (A + B));
	if (h != 1)
	{
		cout << "You have entered the coordinates of a neighboring square." << endl;
		goto enter1;
	}
enter2:
	do
	{
		cout << "Enter the third horizontal coordinate:"; cin >> X1; cout << endl;
		cout << "Enter the third vertical coordinate:"; cin >> Y1; cout << endl;
	} while (X1 < 1 || X1 > 10 || Y1 < 1 || Y1>10);
	h = abs((X1 + Y1) - (A + B));
	if (h != 1)
	{
		cout << "You have entered the coordinates of a neighboring square." << endl;
		goto enter2;
	}
	if (X == X1 && X1 == A || Y == Y1 && Y == B)
	{
		fieldPlayer[A - 1][B - 1] = 1;
		fieldPlayer[X - 1][Y - 1] = 1;
		fieldPlayer[X1 - 1][Y1 - 1] = 1;
	}
	else
	{
		goto enter;
	}
	system("cls");
	showEmptyField(1);
}

void enterDestroyer()
{
	cout << "Enter the coordinates of the destroyer(2 cells):" << endl;
	int X, Y, A, B, h;
enter:
	do
	{
		cout << "Enter the first coordinate of the horizontal:"; cin >> X; cout << endl;
		cout << "Enter the first vertical coordinate:"; cin >> Y; cout << endl;
	} while (X < 1 || X > 10 || Y < 1 || Y>10);
	if (fieldPlayer[X][Y] == 1 || fieldPlayer[X - 1][Y] == 1 || fieldPlayer[X - 2][Y] == 1 || fieldPlayer[X][Y - 1] == 1 || fieldPlayer[X][Y - 2] == 1 || fieldPlayer[X - 1][Y - 2] == 1 || fieldPlayer[X - 2][Y - 2] == 1 || fieldPlayer[X - 2][Y - 1] == 1)
	{
		goto enter;
	}
	if (fieldPlayer[X - 1][Y - 1] != 0)
	{
		goto enter;
	}

enter1:
	do {
		cout << "Enter the second horizontal coordinate:"; cin >> A; cout << endl;
		cout << "Enter the second vertical coordinate:"; cin >> B; cout << endl;
	} while (A < 1 || A>10 || B < 1 || B>10);
	h = abs((X + Y) - (A + B));
	if (h != 1)
	{
		cout << "You have entered the coordinates of a neighboring square." << endl;
		goto enter1;
	}
	if (fieldPlayer[A][B] == 1 || fieldPlayer[A - 1][B] == 1 || fieldPlayer[A - 2][B] == 1 || fieldPlayer[A][B - 1] == 1 || fieldPlayer[A][B - 2] == 1 || fieldPlayer[A - 1][B - 2] == 1 || fieldPlayer[A - 2][B - 2] == 1 || fieldPlayer[A - 2][B - 1] == 1)
	{
		goto enter1;
	}
	if (fieldPlayer[A - 1][B - 1] == 0)
	{
		fieldPlayer[A - 1][B - 1] = 1;
	}
	else
	{
		goto enter1;
	}
	fieldPlayer[X - 1][Y - 1] = 1;
	system("cls");
	showEmptyField(1);
}

void enterBoat()
{
	cout << "Enter the coordinates of the boat(1 cells):" << endl;
	int X, Y, h;
enter:
	do
	{
		cout << "Enter the first coordinate of the horizontal:"; cin >> X; cout << endl;
		cout << "Enter the first vertical coordinate:"; cin >> Y; cout << endl;
	} while (X < 1 || X > 10 || Y < 1 || Y>10);
	if (fieldPlayer[X][Y] == 1 || fieldPlayer[X - 1][Y] == 1 || fieldPlayer[X - 2][Y] == 1 || fieldPlayer[X][Y - 1] == 1 || fieldPlayer[X][Y - 2] == 1 || fieldPlayer[X - 1][Y - 2] == 1 || fieldPlayer[X - 2][Y - 2] == 1 || fieldPlayer[X - 2][Y - 1] == 1)
	{
		goto enter;
	}
	if (fieldPlayer[X - 1][Y - 1] == 0)
	{
		fieldPlayer[X - 1][Y - 1] = 1;
	}
	else
	{
		goto enter;
	}
	system("cls");
	showEmptyField(1);
}

int ClientHandler()
{
	char msg1[256];
	char msg[256];
	recv(newConnection, msg, sizeof(msg), NULL);
	int X, Y;
	X = (int)msg[0];
	Y = (int)msg[1];
	if (fieldPlayer[X - 1][Y - 1] == 0)
	{
		fieldPlayer[X - 1][Y - 1] = 2;
	}
	if (fieldPlayer[X - 1][Y - 1] == 1)
	{
		if (fieldPlayer[X - 2][Y - 2] == 1 || fieldPlayer[X - 2][Y - 1] == 1 || fieldPlayer[X - 2][Y] == 1 || fieldPlayer[X - 1][Y - 2] == 1 || fieldPlayer[X - 1][Y] == 1 || fieldPlayer[X][Y - 2] == 1 || fieldPlayer[X][Y - 1] == 1 || fieldPlayer[X][Y] == 1)
		{
			fieldPlayer[X - 1][Y - 1] = 3;
		}
	}
	if (fieldPlayer[X - 1][Y - 1] == 1)
	{
		if (fieldPlayer[X - 2][Y - 2] != 1 || fieldPlayer[X - 2][Y - 1] != 1 || fieldPlayer[X - 2][Y] != 1 || fieldPlayer[X - 1][Y - 2] != 1 || fieldPlayer[X - 1][Y] != 1 || fieldPlayer[X][Y - 2] != 1 || fieldPlayer[X][Y - 1] != 1 || fieldPlayer[X][Y] != 1)
		{
			fieldPlayer[X - 1][Y - 1] = 4;
		}
	}
	msg1[0] = (char)fieldPlayer[X - 1][Y - 1];
	send(newConnection, msg1, sizeof(msg1), NULL);
	system("cls");
	showEmptyField(1);
	showEmptyField(2);
	return fieldPlayer[X - 1][Y - 1];
}

int shoting()
{
	showEmptyField(1);
	showEmptyField(2);
	char msg1[256];
	cout << "Enter the coordinates for shooting" << endl;
	cout << "Vertical coordinate"; cin >> corX; cout << endl;
	cout << "Horizontal coordinate"; cin >> corY; cout << endl;
	msg1[0] = (char)corX;
	msg1[1] = (char)corY;
	send(newConnection, msg1, sizeof(msg1), NULL);
	Sleep(10);

	char msg[256];
	recv(newConnection, msg, sizeof(msg), NULL);
	fieldComp[corX - 1][corY - 1] = (int)msg[0];
	system("cls");
	showEmptyField(1);
	showEmptyField(2);
	return fieldComp[corX - 1][corY - 1];
}

void chekWounded()
{
	for (int b = 0; b < 4; b++)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (fieldPlayer[i][j] == 4)
				{
					if (fieldPlayer[i - 1][j] == 3)
					{
						fieldPlayer[i - 1][j] = 4;
					}
					if (fieldPlayer[i + 1][j] == 3)
					{
						fieldPlayer[i + 1][j] = 4;
					}
					if (fieldPlayer[i][j - 1] == 3)
					{
						fieldPlayer[i][j - 1] = 4;
					}
					if (fieldPlayer[i][j + 1] == 3)
					{
						fieldPlayer[i][j + 1] = 4;
					}
				}

			}
		}
	}
	for (int b = 0; b < 4; b++)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (fieldComp[i][j] == 4)
				{
					if (fieldComp[i - 1][j] == 3)
					{
						fieldComp[i - 1][j] = 4;
					}
					if (fieldComp[i + 1][j] == 3)
					{
						fieldComp[i + 1][j] = 4;
					}
					if (fieldComp[i][j - 1] == 3)
					{
						fieldComp[i][j - 1] = 4;
					}
					if (fieldComp[i][j + 1] == 3)
					{
						fieldComp[i][j + 1] = 4;
					}
				}
			}
		}
	}
}

int winGame()
{
	int loseChek = 0;
	int winChek = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (fieldPlayer[i][j] == 4)
			{
				loseChek++;
			}
			if (fieldComp[i][j] == 4)
			{
				winChek++;
			}
		}
	}
	if (loseChek == 20)
	{
		cout << "You lose . " << endl;
		return 1;
	}
	if (winChek == 20)
	{
		cout << "You win . " << endl;
		return 1;
	}
}
