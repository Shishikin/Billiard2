
// ���� 5. ������� ������: ���� "�������"
//
// ��������� ��������� �������� ����� �� ����������� �� �������� � ������.
// 
#include <iostream>
#include <cmath>

#include <windows.h>		// ������������ ���� � ���������� ������� Windows

#include "GL/gl.h"			// ������������ ����� ���������� OpenGL �
#include "GL/glu.h"			// ��������������� ��������� (��� �����
#include "GL/glaux.h"		// ����� � �������� ������� � ����������� GL)


// ********************************************************************************
//   ��������� � ���������� ����������
// ********************************************************************************

const double PI = 3.14159265359;
const int    WINDOW_WIDTH = 400;	// ������ ��������� ���� OpenGL
const int    WINDOW_HEIGHT = 300;	// � ������ (� ��������)

class RedGreenBlue
{
private:

	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;

public:

	RedGreenBlue(unsigned char red_, unsigned char green_, unsigned char blue_)
		: red(red_), green(green_), blue(blue_) {}

	unsigned char GetRed() const
	{
		return red;
	}

	unsigned char GetGreen() const
	{
		return green;
	}

	unsigned char GetBlue() const
	{
		return blue;
	}
};

// ����������� ���������� ����� ������, ����� ����� ���� 
// ������� ����������-��������� �� ��� ����
class GraphicalObject;
class Hole;
class Wall;
class Ball;

// ���������� ����������, ��������� �������������� ������ ��������
bool   fBallMoved = false;		// ���� ������� ���� �� ������ ����������� ����
int	   saveRack = 0;			// ������� ������� �����
Wall* listOfWalls = nullptr;	// ��������� �� ������ �������� ������ ������ �����
Hole* listOfHoles = nullptr;	// ��������� �� ������ �������� ������ ���
Ball* listOfBalls = nullptr;	// ��������� �� ������ �������� ������ �����
Ball* pCueBall = nullptr;	// ��������� �� ����� ��� (� ������ �� ���� ����)



// ********************************************************************************
//   ����������� �������
// ********************************************************************************

// ��������������� ����� "�������������"
class Rect {
public:
	// ������������� ��������
	int x1, y1, x2, y2;

public:
	// ������������
	Rect() : x1(0), y1(0), x2(0), y2(0) {}
	Rect(int nx1, int ny1, int nx2, int ny2) :
		x1(nx1), y1(ny1), x2(nx2), y2(ny2) {}

	// ������������� ��������� ��������������
	void SetRect(int left, int top, int right, int bottom)
	{
		x1 = left; y1 = top; x2 = right; y2 = bottom;
	}
	// �������� ��������������
	void OffsetRect(int dx, int dy)
	{
		x1 += dx; y1 += dy; x2 += dx; y2 += dy;
	}
	// ��������� ��������� ������ ��������������
	void CenterPoint(int& x, int& y) const
	{
		x = (x1 + x2) / 2; y = (y1 + y2) / 2;
	}
	// �������� �� ���������� ��������� ����� ��������������
	bool IsRectEmpty() const { return x1 == x2 && y1 == y2; }
	// ��������� ������ ��������������
	int Width() const { return x2 - x1 + 1; }
	// ��������� ������ ��������������
	int Height() const { return y2 - y1 + 1; }
	// ������� ��������� ����������� ������� � ��������������� another
	Rect IntersectRect(const Rect& another) const;
	// ������������ ��������� ��������������
	void NormalizeRect();
};

class GraphicalObject {
public:
	GraphicalObject(int left, int top, int right, int bottom)
	{
		tregion = Rect(left, top, right, bottom);
	}
	// ��������, ����������� ������������ ��������� 
	virtual void Draw() const {}
	virtual void Update() {}
	virtual void HitBy(Ball& ball) {}
	// �������-����� ��� ������� � ���������� ������ 
	Rect GetRegion() const { return tregion; }
	const GraphicalObject* GetLink() const { return tpLink; }
	GraphicalObject* GetLink() { return tpLink; }
protected:
	Rect tregion;
	GraphicalObject* tpLink;
};


// ����� "���������� ���"
class Ball : public GraphicalObject
{
public:
	Ball(int x, int y, bool fc, Ball* pNextBall, RedGreenBlue rgb);
	// ������ ����������� ������������ � �������� ������������ 
	// � ������� ��������� ����� delete 
	Ball(const Ball&) = delete;
	Ball& operator=(const Ball&) = delete;


	// ��������� ���� � ������� ���������
	void Draw() const;



	// ��������� ��������� ���� � �������������, ��� � ������� 
	// ����������� ��������� ������ ��������� ���������� �������
	void Update();
	// ��������� ���� � ���, ��� � ���� ����� ������ ���
	void HitBy(Ball& ball);
	// ������ ���� ����� ���� OX � ������������ �� ������ ���� �� �����,
	// ��������� �� ������ ���� �� (dx, dy)
	double HitAngle(double dx, double dy) const;

	// �������-����� ��� ������� � ���������� ������
	void   SetEnergy(double v) { energy = v; }
	void   SetCenter(int newx, int newy);
	void   SetDirection(double newDir) { direction = newDir; };

	Rect   GetRegion() const { return region; }
	double GetEnergy() const { return energy; }
	void   GetCenter(int& x, int& y) const;
	double GetDirection() const { return direction; }
	bool   IsCue() const { return fCue; }

	const Ball* GetLink() const { return pLink; }
	Ball* GetLink() { return pLink; }



private:
	Ball* pLink;		// ��������� �� ��������� ��� �������� ������
	Rect   region;		// �������� �������, � ������� ������ ���
	double direction;	// ����������� �������� ���� (���� � �������� 
	// ������������ ��� OX)
	double energy;		// ������� ����
	bool   fCue;		// ������� ������ ����
	RedGreenBlue rgb = RedGreenBlue(0, 0, 255);
	//	RedGreenBlue re
};


Ball::Ball(int x, int y, bool fc, Ball* pNextBall, RedGreenBlue rgb_) :
	GraphicalObject(x - 5, y - 5, x, y),
	pLink(pNextBall),
	fCue(fc),
	rgb(rgb_)
{
	SetCenter(x, y);
	SetDirection(0);
	SetEnergy(0.0);
}


// ����� "������ ����������� �����"
class Wall {
public:
	Wall(int left, int top, int right, int bottom,
		double cf, Wall* pNextWall);

	// ��������� ������
	void Draw() const;
	// ��������� ������ � ���, ��� � ��� ����� ��� 
	void HitBy(Ball& ball);

	Rect  GetRegion() const { return region; }
	const Wall* GetLink() const { return pLink; }
	Wall* GetLink() { return pLink; }

private:
	Wall* pLink;
	Rect region;			// �������� ���������� ������
	double convertFactor;	// ��������, �� �������� ���������� 
	// ����������� ���������� ����, �����
	// ���������� ���������� ���������
private:
	// ������ ����������� ������������ � ��������� ������������ 
	// ����������� ���������� � ������ private
	Wall(const Wall&);
	Wall& operator=(const Wall&);
};



// ����� "���� ����������� �����"  
class Hole {
public:
	Hole(int x, int y, Hole* pNextHole);

	// ������ ����������� ������������ � �������� ������������ 
	// � ������� ��������� ����� delete
	Hole(const Hole&) = delete;
	Hole& operator=(const Hole&) = delete;

	// ��������� ����
	void Draw() const;
	// ��������� ���� � ���, ��� � ��� ����� ��� 
	void HitBy(Ball& ball);

	Rect GetRegion() const { return region; }
	const Hole* GetLink() const { return pLink; }
	Hole* GetLink() { return pLink; }

private:
	Hole* pLink;	// ��������� �� ��������� ���� ��� ����������� 
	// �������� ������
	Rect region;	// �������� ���������� ������� ����
};



// ********************************************************************************
//   ���������� ������ CRect
// ********************************************************************************

void Rect::NormalizeRect()
{
	int nx1 = x1, ny1 = y1, nx2 = x2, ny2 = y2;
	x1 = min(nx1, nx2);
	y1 = min(ny1, ny2);
	x2 = max(nx1, nx2);
	y2 = max(ny1, ny2);
}


Rect Rect::IntersectRect(const Rect& another) const
{
	// ��������������� ����������: ����� ��������������� ��� ������� 
	// ����������� - ��� ����� �������������. �������� ��������������,
	// ��������, �����������������, �� ����������.
	Rect r1 = *this, r2 = another;
	r1.NormalizeRect();
	r2.NormalizeRect();

	// �������� �� ������� �����������
	if ((r1.x1 >= r2.x1 && r1.x1 <= r2.x2) ||
		(r1.x2 >= r2.x1 && r1.x2 <= r2.x2))
		if ((r1.y1 >= r2.y1 && r1.y1 <= r2.y2) ||
			(r1.y2 >= r2.y1 && r1.y2 <= r2.y2))
		{
			int ix1 = max(r1.x1, r2.x1);
			int iy1 = max(r1.y1, r2.y1);
			int ix2 = min(r1.x2, r2.x2);
			int iy2 = min(r1.y2, r2.y2);
			return Rect(ix1, iy1, ix2, iy2);
		}

	// ���� ����������� ���, �� ������������ ������ �������������
	return Rect(0, 0, 0, 0);
}



// ********************************************************************************
//   ���������� ������ Wall
// ********************************************************************************

Wall::Wall(int left, int top, int right, int bottom,
	double cf, Wall* pNextWall) :
	convertFactor(cf),
	pLink(pNextWall)
{
	region.SetRect(left, top, right, bottom);
}


void Wall::Draw() const
{
	// ������ �������� � ���� �������� ��������������
	glColor3ub(0, 255, 0);
	glRecti(region.x1, region.y1, region.x2, region.y2);
}


void Wall::HitBy(Ball& ball)
{
	// ���������� ��������� ���� �� ������ 
	ball.SetDirection(convertFactor - ball.GetDirection());
}



// ********************************************************************************
//   ���������� ������ Hole
// ********************************************************************************

Hole::Hole(int x, int y, Hole* pNextHole) :
	pLink(pNextHole)
{
	// ����������� ������������� ��� ���� � ������� � ����� (x, y)
	region.SetRect(x - 5, y - 5, x + 5, y + 5);
}


void Hole::Draw() const
{
	// ���� �������� � ���� ������� �����, ���������� � ������� region
	glColor3ub(255, 255, 0);
	glPointSize(static_cast<float>(region.Width()));
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	int cx = 0, cy = 0;
	region.CenterPoint(cx, cy);
	glVertex2i(cx, cy);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
}


void Hole::HitBy(Ball& ball)
{
	// ������� ���� ����������
	ball.SetEnergy(0.0);

	// ����� ��� ����� ���������� � ��������� �������, � ����� ���� 
	// ��������� �� ����� � �������� � ������ ��� ������������ �����
	if (ball.IsCue())
		ball.SetCenter(50, 108);
	else
	{
		ball.SetCenter(10 + saveRack * 15, 250);
		saveRack++;	// ���������� ����������� �������� ������� �����
	}
}



// ********************************************************************************
//   ���������� ������ Ball
// ********************************************************************************

/*
Ball::Ball(int x, int y, bool fc, Ball* pNextBall, RedGreenBlue rgb_) :
	GraphicalObject(x-5, y-5, x, y),
	pLink(pNextBall),
	fCue(fc),
	rgb(rgb_)
{
	SetCenter(x, y);
	SetDirection(0);
	SetEnergy(0.0);
}
*/

void Ball::Draw() const
{
	// ��� �������� ��������� � ������� region ���� �����, ���� ����� ������.
	// ����� ������ �������� ���, � �������� ���������� ���� fCue. 

	if (IsCue())
		glColor3ub(255, 255, 255);	// ����� ����
	else
		glColor3ub(rgb.GetRed(), rgb.GetGreen(), rgb.GetBlue()); // ������ ����

	glPointSize(static_cast<float>(region.Width()));
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	int cx = 0, cy = 0;
	region.CenterPoint(cx, cy);
	glVertex2i(cx, cy);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
}


void Ball::SetCenter(int newx, int newy)
{
	// ������� ������������ �������������� ��� ���� � ������� � ����� (newx, newy)
	region.SetRect(newx - 5, newy - 5, newx + 5, newy + 5);
}


void Ball::GetCenter(int& x, int& y) const
{
	region.CenterPoint(x, y);
}


void Ball::Update()
{
	// ��� �������� � ���� ������ ���� ��������� ������������ �������
	if (energy <= 0.5)
		return;

	fBallMoved = true;

	// �� ������ ���� ������� ���� ����������� �� ���� ������
	energy -= 0.05;

	// �������� ���� ����������� � ������ ������������ �����������
	// ������������ ������� �� ��������. ��������� 2.0 ������� ��� 
	// ����������� �����-����� ������������� �������� �����
	int dx = static_cast<int>(2.0 * sqrt(energy) * cos(direction));
	int dy = static_cast<int>(2.0 * sqrt(energy) * sin(direction));
	region.OffsetRect(dx, dy);

	// �������� �� ��������� � ����
	Hole* hptr = listOfHoles;
	while (hptr)
	{
		Rect is = region.IntersectRect(hptr->GetRegion());
		if (!is.IsRectEmpty())
		{
			hptr->HitBy(*this);
			hptr = nullptr;
		}
		else
			hptr = hptr->GetLink();
	}

	// �������� �� ��������� � ������
	Wall* wptr = listOfWalls;
	while (wptr)
	{
		Rect is = region.IntersectRect(wptr->GetRegion());
		if (!is.IsRectEmpty())
		{
			wptr->HitBy(*this);
			wptr = nullptr;
		}
		else
			wptr = wptr->GetLink();
	}

	// �������� �� ��������� � ������ ���
	Ball* bptr = listOfBalls;
	while (bptr)
	{
		// ������ ������: ���������� ������ ��������� ����� � ������ �����
		if (bptr != this)
		{
			Rect is = region.IntersectRect(bptr->GetRegion());
			if (!is.IsRectEmpty())
			{
				bptr->HitBy(*this);
				break;
			}
		}
		bptr = bptr->GetLink();
	}
}

// ������ ���� ����� ���� OX � �������� (dx, dy)
// ������� ���������� �������� ���� �������� � ��������� (0,PI) ��� (-PI,0)
double Ball::HitAngle(double dx, double dy) const
{
	double na = 0.0;

	if (fabs(dx) < 0.05)
		na = PI / 2;
	else
		na = atan(fabs(dy / dx));

	if (dx < 0)
		na = PI - na;
	if (dy < 0)
		na = -na;

	return na;
}



void Ball::HitBy(Ball& ball)
{
	// ��������� ������� ���������� ���� �����
	ball.SetEnergy(ball.GetEnergy() / 2.0);
	// � ���������� �� � ����������� �������
	energy += ball.GetEnergy();

	// ������ ������ ����������� ��� �������� ����
	int cx1 = 0, cy1 = 0, cx2 = 0, cy2 = 0;
	GetCenter(cx1, cy1);
	ball.GetCenter(cx2, cy2);
	SetDirection(HitAngle(cx1 - cx2, cy1 - cy2));

	// ����������� ����������� ���������� ����
	double da = ball.GetDirection() - GetDirection();
	ball.SetDirection(ball.GetDirection() + da);
}


// ********************************************************************************
//   ���������� �������, �� ���������� ������� �� ������ �� �������
// ********************************************************************************

void ChooseRedGreenBlue(RedGreenBlue& rgb, int count)
{
	if (count < 5)
	{
		rgb = RedGreenBlue(0, 0, 255);
	}
	else
	{
		if (count < 10)
		{
			rgb = RedGreenBlue(0, 255, 0);
		}
		else
		{
			rgb = RedGreenBlue(255, 0, 0);
		}
	}
}

// ������� ��� ������������� ���������� ����������.
void CreateGlobals()
{
	saveRack = 0;		// ������� ������� �����
	listOfWalls = nullptr;	// ��������� �� ������ �������� ������ ������
	listOfHoles = nullptr; // ��������� �� ������ �������� ������ ���
	listOfBalls = nullptr; // ��������� �� ������ �������� ������ �����

	// �������� ������
	Wall* pNewWall = new Wall(10, 10, 300, 15, 0.0, listOfWalls);
	listOfWalls = pNewWall;
	pNewWall = new Wall(10, 200, 300, 205, 0.0, listOfWalls);
	listOfWalls = pNewWall;
	pNewWall = new Wall(10, 10, 15, 200, PI, listOfWalls);
	listOfWalls = pNewWall;
	pNewWall = new Wall(300, 10, 305, 205, PI, listOfWalls);
	listOfWalls = pNewWall;

	// �������� ���
	Hole* pNewHole = new Hole(15, 15, listOfHoles);
	listOfHoles = pNewHole;
	pNewHole = new Hole(15, 200, listOfHoles);
	listOfHoles = pNewHole;
	pNewHole = new Hole(300, 15, listOfHoles);
	listOfHoles = pNewHole;
	pNewHole = new Hole(300, 200, listOfHoles);
	listOfHoles = pNewHole;
	pNewHole = new Hole(157, 15, listOfHoles);
	listOfHoles = pNewHole;
	pNewHole = new Hole(157, 200, listOfHoles);
	listOfHoles = pNewHole;

	// �������� �����. 
	// ������� ��������� ����� ���, � ����� 15 �����. � �������
	// ������ ����� ��� ������������� ���������.
	RedGreenBlue rgb = RedGreenBlue(0, 0, 255);
	int count = 0;
	pCueBall = new Ball(50, 108, true, NULL, rgb);
	listOfBalls = pCueBall;
	for (int i = 1; i <= 5; i++)
		for (int j = 1; j <= i; j++)
		{
			ChooseRedGreenBlue(rgb, count);
			Ball* pNewBall = new Ball(190 + i * 8, 100 + 16 * j - 8 * i, false, listOfBalls, rgb);
			listOfBalls = pNewBall;
			++count;
		}
}



// ********************************************************************************
//   ������� �������� ����� OpenGL
// ********************************************************************************

// ���������� ������� "������� ����� ������ ����".
// ��� ������� ��������� ���� ���� �� ������ ����. ���� �����
// ���������, ����������� ������� �� ��������� ��������� ����.
void CALLBACK MouseButtonDown(AUX_EVENTREC* event)
{
	// ����������� ��������� ��������� ���� � ������ ������� ���������, ��������� 
	// � ������ ����� ����� ���� (� ��������� event ��� �������� � ����� ������� 
	// ���������, ��������� � ����� ������� ����� ����)
	int mouse_x = event->data[0];
	int mouse_y = WINDOW_HEIGHT - event->data[1];

	// ������ ���� ���������� ��������� ��������� �������
	pCueBall->SetEnergy(20.0);
	// � ������������� ����������� �������� "�� ��������� ����"
	int cx = 0, cy = 0;
	pCueBall->GetCenter(cx, cy);
	pCueBall->SetDirection(pCueBall->HitAngle(mouse_x - cx, mouse_y - cy));

	// ���� ������� ���� �� ������ ����������� ����
	fBallMoved = true;
}


// ������� ��� ��������� �����: ����������� ����� � �����
void CALLBACK Display()
{
	// ������� ������ � ������ (�� ������������ ��� ��������� �����)
	glClear(GL_COLOR_BUFFER_BIT);

	// ��������� ������ �����
	const Wall* pWall = listOfWalls;
	while (pWall)
	{
		pWall->Draw();
		pWall = pWall->GetLink();
	}

	// ��������� ���
	const Hole* pHole = listOfHoles;
	while (pHole)
	{
		pHole->Draw();
		pHole = pHole->GetLink();
	}

	// ��������� �����
	const Ball* pBall = listOfBalls;
	while (pBall)
	{
		pBall->Draw();
		pBall = pBall->GetLink();
	}

	// ����� ������� - ��������� ������ � 
	// ������ � ������ (� ������� ����������� ���������)
	auxSwapBuffers();
}


// ������� �������
void CALLBACK Idle()
{
	// ��������� ����� �����������, ������ ���� ���� ���� �� ���� ���������� ���
	if (!fBallMoved)
		return;

	// ���������� ��������� �����
	fBallMoved = false;
	Ball* bptr = listOfBalls;
	while (bptr)
	{
		bptr->Update();
		bptr = bptr->GetLink();
	}

	Display();		// ����������� �����
	Sleep(25);	// �������� �� 25 ��, ����� ���� �� ��������� ������� ������
}



// ********************************************************************************
//   ������� ������� ���������
// ********************************************************************************

int main()
{

	CreateGlobals();

	// ��������� ������� ����� � OpenGL: �������� ����� RGBA, ������� �����������
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGBA);

	// �������� ���� ��������������� ���������� Glaux ��� ������ ����������� OpenGL.
	// ������ ���� - WINDOW_WIDTH�WINDOW_HEIGHT ��������. ����� ������� ���� ����
	// �������� ��������� ������������ (10, 10).
	auxInitPosition(10, 10, WINDOW_WIDTH, WINDOW_HEIGHT);
	auxInitWindow(L"Billiard model");

	// ��������� ���������� OpenGL
	glClearColor(0.0, 0.0, 0.0, 0.0);		// ���� ������� ����������� (��������� ������)
	glShadeModel(GL_FLAT);					// ���������� ������ ������� ��������� (��� ��������� �������)

	// ����������� �������-����������� ������� - ��� ������ ����� ������� ����
	auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, MouseButtonDown);

	// ������� ������� � ������������ ����������, ����� ���� �� �������� ��������� 
	// �� ����������, ���� ���  �������. � �� ������� ����������� ��������. 
	auxIdleFunc(Idle);

	// ������� ������� ��������� ����. ��� ������� ����� ���������� ������
	// ���, ����� ����������� ������������ ���� �� ������ (��������, ����� 
	// ���� ����� ���������� �� ���� �����)
	auxMainLoop(Display);

	return 0;
}