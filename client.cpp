#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

#define MY_PORT 1111
#define SERVERADDR "127.0.0.1"

using namespace std;

const int N = 11;
int corX, corY, winValue;// Переменные которые будут являться координатами наших кораблей и переменная для проверки выигрыша.
int fieldPlayer[N][N];
int fieldComp[N][N];//Двумерные массивы. 1 для нашего игрового поля, 2 для соперника.

void showEmptyField(int numberArray);//Функция отрисовки игрового поля.

void enterLinkor();// Функция для размещения на игровом поле 4-ех палубного корабля.
void enterCruiser();// Функция для размещения на игровом поле 3-ех палубного корабля.
void enterDestroyer();// Функция для размещения на игровом поле 2-ух палубного корабля.
void enterBoat();// Функция для размещения на игровом поле однопалубного корабля.

int ClientHandler(); // Функция для получения сообщения с координатами хода противника и отправки сообщения с изменившимися координатми на игровом поле и проверкой попадания или промаха

int getValue();//Функция проверки введенных пользователем данных

int shoting();// Функция для отправки координат стрельбы и получения результата

void checkWounded(); // Функция для проверки раненых кораблей и их перезаписи в уничтоженных

int winGame();// Функция проверки проигрыша или выигрыша игрока
SOCKET Connection;// Подключаем сокет для работе по сети.

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

	}//Очищаем поля (0 – пустая клетка, 1 – целый корабль, 2 – промах , 3 – раненый корабль, 4 – уничтоженный корабль).
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			fieldComp[i][j] = 0;
		}

	}
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr; //Структура для хранения адреса
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(SERVERADDR);//выбираем адрес для соединения
	addr.sin_port = htons(MY_PORT);//выбираем порт для соединения
	addr.sin_family = AF_INET;//Семейство протоколов создаваемого сокета

	Connection = socket(AF_INET, SOCK_STREAM, NULL);//Соедиеняемся с сервером
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		cout << "Error: failed connect to server.\n";
		return 1;
	}
	else
	{
		cout << "Player found!\n";
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
	// Выполняем цикл, который будет работать пока не выполнится условие победы или проигрыша
	do
	{
		do
		{
			temp = ClientHandler();
			checkWounded();
			if (winValue != 1)
			{
				break;
			}
		} while (temp != 2 );
		if (winValue != 1)
		{
			break;
		}
		do
		{
			buff = shoting();
			checkWounded();
			if (winValue != 1)
			{
				break;
			}
		} while (buff != 2 );
		winValue = winGame();
	} while (winValue != 1);

	closesocket(Connection);
	WSACleanup();
	return 0;
}

void showEmptyField(int numberArray)
{
	switch (numberArray)
	{
	case 1:
		system("cls");
		cout << "Our playing field:" << endl;
		for (int i = 0; i < 13; i++)//верхние координаты 1-10
		{
			if (i == 0 || i == 1 || i == 2)
			{
				cout << ' ';
				continue;
			}
			cout << i - 2;
		}
		cout << endl;
		cout << ' ' << ' ' << (char)201;
		for (int i = 0; i < 10; i++)// псевдографика верхней границы
		{
			cout << (char)205;
		}
		cout << (char)187 << endl;
		for (int i = 1; i < 10; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				if (j == 0)
				{
					cout << i;
				}
				else if (j == 1)
				{
					cout << ' ';
				}
				else if (j == 2)
				{
					cout << (char)186;
				}
				else if (j == 13)
				{
					cout << (char)186;
				}
				else if (fieldPlayer[i - 1][j - 3] == 1)
				{
					cout << (char)219;
				}
				else if (fieldPlayer[i - 1][j - 3] == 0)
				{
					cout << ' ';
				}
				else if (fieldPlayer[i - 1][j - 3] == 2)
				{
					cout << (char)42;
				}
				else if (fieldPlayer[i - 1][j - 3] == 3)
				{
					cout << (char)176;
				}
				else if (fieldPlayer[i - 1][j - 3] == 4)
				{
					cout << (char)88;
				}
			}
			cout << endl;
		}
		for (int i = 0; i < 14; i++)
		{
			if (i == 0)
			{
				cout << 1;
			}
			else if (i == 1)
			{
				cout << 0;
			}
			else if (i == 2)
			{
				cout << (char)186;
			}
			else if (i == 13)
			{
				cout << (char)186;
			}
			else if (fieldPlayer[9][i - 3] == 1)
			{
				cout << (char)219;
			}
			else if (fieldPlayer[9][i - 3] == 0)
			{
				cout << ' ';
			}
			else if (fieldPlayer[9][i - 3] == 2)
			{
				cout << (char)42;
			}
			else if (fieldPlayer[9][i - 3] == 3)
			{
				cout << (char)176;
			}
			else if (fieldPlayer[9][i - 3] == 4)
			{
				cout << (char)88;
			}
		}
		break;
	case 2:
		cout << "Opponent's playing field:" << endl;
		for (int i = 0; i < 13; i++)//верхние координаты 1-10
		{
			if (i == 0 || i == 1 || i == 2)
			{
				cout << ' ';
				continue;
			}
			cout << i - 2;
		}
		cout << endl;
		cout << ' ' << ' ' << (char)201;
		for (int i = 0; i < 10; i++)// псевдографика верхней границы
		{
			cout << (char)205;
		}
		cout << (char)187 << endl;
		for (int i = 1; i < 10; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				if (j == 0)
				{
					cout << i;
				}
				else if (j == 1)
				{
					cout << ' ';
				}
				else if (j == 2)
				{
					cout << (char)186;
				}
				else if (j == 13)
				{
					cout << (char)186;
				}
				else if (fieldComp[i - 1][j - 3] == 1)
				{
					cout << (char)219;
				}
				else if (fieldComp[i - 1][j - 3] == 0)
				{
					cout << ' ';
				}
				else if (fieldComp[i - 1][j - 3] == 2)
				{
					cout << (char)42;
				}
				else if (fieldComp[i - 1][j - 3] == 3)
				{
					cout << (char)176;
				}
				else if (fieldComp[i - 1][j - 3] == 4)
				{
					cout << (char)88;
				}
			}
			cout << endl;
		}
		for (int i = 0; i < 14; i++)
		{
			if (i == 0)
			{
				cout << 1;
			}
			else if (i == 1)
			{
				cout << 0;
			}
			else if (i == 2)
			{
				cout << (char)186;
			}
			else if (i == 13)
			{
				cout << (char)186;
			}
			else if (fieldComp[9][i - 3] == 1)
			{
				cout << (char)219;
			}
			else if (fieldComp[9][i - 3] == 0)
			{
				cout << ' ';
			}
			else if (fieldComp[9][i - 3] == 2)
			{
				cout << (char)42;
			}
			else if (fieldComp[9][i - 3] == 3)
			{
				cout << (char)176;
			}
			else if (fieldComp[9][i - 3] == 4)
			{
				cout << (char)88;
			}
		}
	}
	cout << endl;
	cout << ' ' << ' ' << (char)200;
	for (int i = 0; i < 10; i++)// псевдографика нижней границы
	{
		cout << (char)205;
	}
	cout << (char)188 << endl;
}

void enterLinkor()
{
	showEmptyField(1);
enter:
	cout << "Enter linkor coordinates(4 cells):" << endl;
	int X, Y, A, B, X1, Y1, A1, B1, h;
	do
	{
		cout << "Enter the first coordinate of the horizontal:"; X = getValue(); cout << endl;
		cout << "Enter the first vertical coordinate:"; Y = getValue(); cout << endl;
	} while (X < 1 || X > 10 || Y < 1 || Y>10);
enter1:
	do {
		cout << "Enter the second horizontal coordinate:"; A = getValue(); cout << endl;
		cout << "Enter the second vertical coordinate:"; B = getValue(); cout << endl;
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
		cout << "Enter the third horizontal coordinate:";  X1 = getValue(); cout << endl;
		cout << "Enter the third vertical coordinate:"; Y1 = getValue(); cout << endl;
	} while (X1 < 1 || X1 > 10 || Y1 < 1 || Y1>10);
	h = abs((X1 + Y1) - (A + B));
	if (h != 1)
	{
		cout << "You have entered the coordinates of a neighboring square." << endl;
		goto enter2;
	}
enter3:
	do {
		cout << "Enter the fourth horizontal coordinate:"; A1 = getValue(); cout << endl;
		cout << "Enter the fourth vertical coordinate:"; B1 = getValue(); cout << endl;
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
		cout << "Enter the first coordinate of the horizontal:";  X = getValue(); cout << endl;
		cout << "Enter the first vertical coordinate:"; Y = getValue(); cout << endl;
	} while (X < 1 || X > 10 || Y < 1 || Y>10);
enter1:
	do {
		cout << "Enter the second horizontal coordinate:"; A = getValue(); cout << endl;
		cout << "Enter the second vertical coordinate:"; B = getValue(); cout << endl;
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
		cout << "Enter the third horizontal coordinate:"; X1 = getValue(); cout << endl;
		cout << "Enter the third vertical coordinate:"; Y1 = getValue(); cout << endl;
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
		cout << "Enter the first coordinate of the horizontal:";  X = getValue(); cout << endl;
		cout << "Enter the first vertical coordinate:";  Y = getValue(); cout << endl;
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
		cout << "Enter the second horizontal coordinate:"; A = getValue(); cout << endl;
		cout << "Enter the second vertical coordinate:";  B = getValue(); cout << endl;
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
	cout << "Enter the coordinates of the boat (1 cell):" << endl;
	int X, Y;
enter:
	do
	{
		cout << "Enter the first coordinate of the horizontal:";  X = getValue(); cout << endl;
		cout << "Enter the first vertical coordinate:";  Y = getValue(); cout << endl;
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
	recv(Connection, msg, sizeof(msg), NULL);
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
	send(Connection, msg1, sizeof(msg1), NULL);
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
	cout << "Enter the coordinates for shooting" << '\n';
	cout << "Horizontal coordinate: "; corX=getValue(); cout << '\n';
	cout << "Vertical coordinate: "; corY=getValue(); cout << '\n';
	msg1[0] = (char)corX;
	msg1[1] = (char)corY;
	send(Connection, msg1, sizeof(msg1), NULL);
	Sleep(10);

	char msg[256];
	recv(Connection, msg, sizeof(msg), NULL);
	fieldComp[corX - 1][corY - 1] = (int)msg[0];
	system("cls");
	showEmptyField(1);
	showEmptyField(2);
	return fieldComp[corX - 1][corY - 1];
}

void checkWounded()
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
		cout << "You lost." << endl;
		return 1;
	}
	if (winChek == 20)
	{
		cout << "You won!" << endl;
		return 1;
	}
}

int getValue()
{
	while (true)
	{
		int value;
		cin >> value;

		if (cin.fail())
		{
			cout << "Wrong value!" << '\n';
			cin.clear();
			cin.ignore(32767, '\n');
		}
		else
			return value;
	}
}
