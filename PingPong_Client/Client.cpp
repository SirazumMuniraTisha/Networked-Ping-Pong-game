/*
Name: Tisha, Sirazum Munira
Project: PA-1a (Regular Project Name) Networked Pingpong game
Instructor: Feng Chen
Class: cs7103-au17
LogonID: cs710307
*/
#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <ws2tcpip.h> //winsock2.h header file not works here
#include <string> //For string
#include <iostream> //For cout, endl, cin.getline
#include <time.h> //for time function needed for game
#include <conio.h> //for _getch() 
using namespace std;
enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 }; //Directions for ball
class cBall
{
private:
	int x, y;
	int originalX, originalY;
	eDir direction;
public:
	cBall(int posX, int posY)
	{
		originalX = posX;
		originalY = posY;
		x = posX; y = posY;
		direction = STOP;
	}
	void Reset()
	{
		x = originalX; y = originalY;
		direction = STOP;
	}
	void changeDirection(eDir d)
	{
		direction = d;
	}
	void randomDirection()
	{
		direction = (eDir)((rand() % 6) + 1);
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline eDir getDirection() { return direction; }

	void Move()
	{
		switch (direction)
		{
		case STOP:
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case UPRIGHT:
			x++; y--;
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		default:
			break;
		}
	}
	friend ostream & operator<<(ostream & o, cBall c)
	{
		o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]";
		return o;
	}
};
class cPaddle
{
private:
	int x, y;
	int originalX, originalY;
public:
	cPaddle()
	{
		x = y = 0;
	}
	cPaddle(int posX, int posY) : cPaddle()
	{
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}
	inline void Reset() { x = originalX; y = originalY; }
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUp() { y = y - 2; }
	inline void moveDown() { y = y + 2; }
	friend ostream & operator<<(ostream & o, cPaddle c)
	{
		o << "Paddle [" << c.x << "," << c.y << "]";
		return o;
	}
};
class cGameManger
{
public:
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2;
	bool quit;
	cBall * ball;
	cPaddle *player1;
	cPaddle *player2;
public:
	cGameManger(int w, int h)
	{
		srand(time(NULL));
		quit = false;
		up1 = 'w'; up2 = 'i';
		down1 = 's'; down2 = 'k';
		score1 = score2 = 0;
		width = w; height = h;
		ball = new cBall(w / 2, h / 2);
		player1 = new cPaddle(1, h / 2 - 3);
		player2 = new cPaddle(w - 2, h / 2 - 3);
	}
	~cGameManger()
	{
		delete ball, player1, player2;
	}
	void ScoreUp(cPaddle * player)
	{
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;
		ball->Reset();
		player1->Reset();
		player2->Reset();
	}
	void Draw(int ballx, int bally, int player1y, int player2y)//(int ballx, int bally, int paddleno, int playery)
	{
		system("cls");
		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
		cout << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int player1x = player1->getX();
				int player2x = player2->getX();

				if (j == 0)
					cout << "\xB2";
				if (ballx == j && bally == i)
					cout << "O"; //ball
				else if (player1x == j && player1y == i)
					cout << "\xDB"; //player1 paddle
				else if (player2x == j && player2y == i)
					cout << "\xDB"; //player2
				else if (player1x == j && player1y + 1 == i)
					cout << "\xDB"; //player1
				else if (player1x == j && player1y + 2 == i)
					cout << "\xDB"; //player1
				else if (player1x == j && player1y + 3 == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y + 1 == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y + 2 == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y + 3 == i)
					cout << "\xDB"; //player1
				else
					cout << " ";
				if (j == width - 1)
					cout << "\xB2";
			}
			cout << endl;
		}
		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
		cout << endl;
		cout << "Score 1: " << score1 << endl << "Score 2: " << score2 << endl << endl;

		if (score1 == 2 && score1 > score2) {
			cout << "Client 1 wins!" << endl << endl << endl;
			ball->Reset();
			player1->Reset();
			player2->Reset();
			system("pause");
		}
		else if (score2 == 2 && score2 > score1) {
			cout << "Client 2 wins!" << endl << endl << endl;
			ball->Reset();
			player1->Reset();
			player2->Reset();
			system("pause");
		}
		cout << "ballinfod: " << ball->getDirection() << endl; //printed some ball info for varifying network connection
		cout << "ballinfox: " << ballx << endl;
		cout << "ballinfoy: " << bally << endl;
	}

	void Input(char current) //current is the keystroke 
	{
		ball->Move();
		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();

		if (current == up1)
			if (player1y > 0) {
				player1->moveUp();
			}
		if (current == up2)
			if (player2y > 0)
			{
				player2->moveUp();
			}

		if (current == down1)
			if (player1y + 4 < height)
			{
				player1->moveDown();
			}

		if (current == down2)
			if (player2y + 4 < height)
			{
				player2->moveDown();
			}

		if (ball->getDirection() == STOP)
			ball->randomDirection();

		if (current == 'q')
			quit = true;

		cout << "ballinfoi: " << ball->getDirection() << endl;
		cout << "ballinfox: " << ball->getX() << endl;
		cout << "ballinfoy: " << ball->getY() << endl;

		Draw(ball->getX(), ball->getY(), player1->getY(), player2->getY()); //calling draw function
		Logic(ball->getX(), ball->getY(), player1->getY(), player2->getY()); //calling logic function
	}

	void Input1(char current) //for showing the ball info
	{

		ball->Move();

		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();
		int updateinfo[4];
		cout << "keyPressed: " << current << endl;
		cout << "leftup: " << up1 << endl;
		cout << "leftDown: " << down1 << endl;
		cout << "Rightup: " << up2 << endl;
		cout << "RightDown: " << down2 << endl;

		if (current == up1)
			if (player1y > 0) {
				player1->moveUp();
			}
		if (current == up2)
			if (player2y > 0)
			{
				player2->moveUp();
			}
		if (current == down1)
			if (player1y + 4 < height)
			{
				player1->moveDown();
			}
		if (current == down2)
			if (player2y + 4 < height)
			{
				player2->moveDown();
			}

		if (ball->getDirection() == STOP)
			ball->randomDirection();
		if (current == 'q')
			quit = true;

		cout << "ballDirection: " << ball->getDirection() << endl;
		cout << "ballPosx: " << ball->getX() << endl;
		cout << "ballPosy: " << ball->getY() << endl;
	}
	void Logic(int ballx, int bally, int player1y, int player2y)
	{
		int player1x = player1->getX();
		int player2x = player2->getX();

		//left paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player1x + 1)
				if (bally == player1y + i)
					ball->changeDirection((eDir)((rand() % 3) + 4));

		//right paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player2x - 1)
				if (bally == player2y + i)
					ball->changeDirection((eDir)((rand() % 3) + 1));

		//bottom wall
		if (bally == height - 1)
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
		//top wall
		if (bally == 0)
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
		//right wall
		if (ballx == width - 1)
			ScoreUp(player1);
		//left wall
		if (ballx == 0)
			ScoreUp(player2);
	}
	void Run(char info)
	{
		int i = 0;
		while (i<35) //run game for 35 loop nd then restart
		{
			Input(info);
			Input1(info);
			i++;

		}
	}
};

void Play(char buffer) {
	cGameManger c(40, 20); //canvas size
	c.Run(buffer);

}

SOCKET Connection;//This client's connection to the server
std::string info;
void ClientThread()
{
	int bufferlength; //Holds the length of the message we are receiving

	while (true)
	{
		recv(Connection, (char*)&bufferlength, sizeof(int), NULL); //receive buffer
		char * buffer = new char[bufferlength + 1]; //Allocate buffer
		buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
		recv(Connection, buffer, bufferlength, NULL);

		if (bufferlength == 1) { //when pressing gaming keys
			std::cout << buffer << std::endl;
			Play(buffer[0]);

		}
		else
			std::cout << buffer << std::endl; //print out buffer
		delete[] buffer; //Deallocate buffer
	}
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr.s_addr)); //address local host 
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //Failed to Connect
	}

	std::cout << "Connected!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.

	std::string userinput;
	while (true)
	{
		userinput = _getch(); // get gaming keystrocks

		int bufferlength = userinput.size();
		send(Connection, (char*)&bufferlength, sizeof(int), NULL); //Send length of buffer
		send(Connection, userinput.c_str(), bufferlength, NULL); //Send buffer

		Sleep(1);
	}

	return 0;
}

