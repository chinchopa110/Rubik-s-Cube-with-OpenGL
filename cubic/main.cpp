#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <vector>
#include <stack>
#include <ctime>

#pragma warning(disable : 4996)

constexpr auto SPEED = 8;
constexpr auto SPEED2 = 3;
using namespace std;

float angle = 0.0;
float lx = 0.0f, lz = -1.0f;
float x = 0.0f, z = 5.0f;
static int G = 0.0;
static double sizem = 0;
int col[6] = { 0xFFFFFF, 0xFFFF00, 0x0000FF, 0x00FF00, 0xFF0000, 0xCD853F};
int x_rot = 22;
int y_rot = 36;


class small_cube{
private:
	unsigned char RGB_[3];
public:
	int color[6]; // верх(0), низ(1), перед(2), зад(3), лево(4), право(5)
	double size;

	small_cube(){

		for (int i = 0; i < 3; i++) {
			RGB_[i] = ' ';
		}

		for (int i = 0; i < 6; i++) {
			color[i] = 0;
		}

		size = 0.0;
	}


	void install(int i, int color){
		this->color[i] = color;
	}

	unsigned char* at(int i){
		RGB_[0] = color[i] >> 16;
		RGB_[1] = color[i] >> 8;
		RGB_[2] = color[i];
		return RGB_;
	}

	void draw1(){
		glPushMatrix();
		glBegin(GL_QUADS);

		// верх
		glColor3ubv(at(0));
		glNormal3f(0, 0, 1);
		glVertex3f(size, size, size);
		glVertex3f(0, size, size);
		glVertex3f(0, 0, size);
		glVertex3f(size, 0, size);

		// низ
		glColor3ubv(at(1));
		glNormal3f(0, 0, -1);
		glVertex3f(size, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, size, 0);
		glVertex3f(size, size, 0);

		// спереди
		glColor3ubv(at(2));
		glNormal3f(0, -1, 0);
		glVertex3f(size, 0, size);
		glVertex3f(0, 0, size);
		glVertex3f(0, 0, 0);
		glVertex3f(size, 0, 0);

		// сзади
		glColor3ubv(at(3));
		glNormal3f(0, 1, 0);
		glVertex3f(size, size, 0);
		glVertex3f(0, size, 0);
		glVertex3f(0, size, size);
		glVertex3f(size, size, size);

		// слева
		glColor3ubv(at(4));
		glNormal3f(-1, 0, 0);
		glVertex3f(0, size, size);
		glVertex3f(0, size, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, size);

		// справа
		glColor3ubv(at(5));
		glNormal3f(1, 0, 0);
		glVertex3f(size, size, 0);
		glVertex3f(size, size, size);
		glVertex3f(size, 0, size);
		glVertex3f(size, 0, 0);

		glEnd();
		glPopMatrix();
	}

	void rotateX(){
		int tmp = color[0];
		color[0] = color[4];
		color[4] = color[1];
		color[1] = color[5];
		color[5] = tmp;
	}

	void rotateY(){
		int tmp = color[2];
		color[2] = color[1];
		color[1] = color[3];
		color[3] = color[0];
		color[0] = tmp;
	}

	void rotateZ(){
		int tmp = color[5];
		color[5] = color[3];
		color[3] = color[4];
		color[4] = color[2];
		color[2] = tmp;
	}



	void draw(double x, double y, double z){
		glPushMatrix();
		glTranslated(x, y, z);
		draw1();
		glPopMatrix();
	}
};

//static small_cube a[3][3][3];
//static int rotate1[6];
//static double size1;
//static int color[6];
//static bool check[3][3][3];
//static int curr_gr;

class Cube{
private:
	//secind part
	//rotate!!!
	int _angle[3];
	bool check[3][3][3];
	small_cube tmp[3][3];


	//first part
	small_cube a[3][3][3];
	int rotate[6];
	double size;
	int color[6];
	int tmp1;

public:
	int curr_gr;

	Cube() {

		curr_gr = 0;

		for (int i = 0; i < 3; i++) {
			_angle[i] = 0;
		}

		for (int i = 0; i < 6; i++) {
			rotate[i] = 0;
		}

		for (int i = 0; i < 6; i++) {
			color[i] = ' ';
		}

		size = 0;

	}

	void create(double size, int* color){

		for (int i = 0; i < 6; i++) {
			rotate[i] = 0;
		}

		this->size = size;
		curr_gr = -1;

		for (int i = 0; i < 6; i++) {
			this->color[i] = color[i];
		}

		// верх
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				a[i][j][2].install(0, color[0]);
			}
		}

		// низ
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				a[i][j][0].install(1, color[1]);
			}
		}

		// спереди
		for (int k = 0; k < 3; k++) {
			for (int j = 0; j < 3; j++) {
				a[j][0][k].install(2, color[2]);
			}
		}

		// сзади
		for (int k = 0; k < 3; k++) {
			for (int j = 0; j < 3; j++) {
				a[j][2][k].install(3, color[3]);
			}
		}

		// слева
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				a[0][k][i].install(4, color[4]);
			}
		}

		// справа
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				a[2][k][i].install(5, color[5]);
			}
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					a[i][j][k].size = (size / 3.0) * 0.85;
				}
			}
		}
	}

	void draw(){

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					check[i][j][k] = true;
				}
			}
		}

		if (curr_gr != -1){
			glPushMatrix();
			int i, j, k;

			if (curr_gr == 0 || curr_gr == 1){

				k = (curr_gr & 1) * 2;
				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						check[i][j][k] = false;
					}
				}

					glTranslated(size / 2, size / 2, 0);   
					glRotatef(rotate[curr_gr], 0, 0, 1);   
					glTranslated(-size / 2, -size / 2, 0);

				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
					}
				}

			} else if (curr_gr == 2 || curr_gr == 3) {
				j = (curr_gr & 1) * 2;
				for (i = 0; i < 3; i++) {
					for (k = 0; k < 3; k++) {
						check[i][j][k] = false;
					}
				}

					glTranslated(size / 2, 0, size / 2);
					glRotatef(rotate[curr_gr], 0, 1, 0);
					glTranslated(-size / 2, 0, -size / 2);

				for (i = 0; i < 3; i++) {
					for (k = 0; k < 3; k++) {
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
					}
				}

			} else if (curr_gr == 4 || curr_gr == 5) {
				i = (curr_gr & 1) * 2;
				for (j = 0; j < 3; j++) {
					for (k = 0; k < 3; k++) {
						check[i][j][k] = false;
					}
				}


					glTranslated(0, size / 2, size / 2);
					glRotatef(rotate[curr_gr], 1, 0, 0);
					glTranslated(0, -size / 2, -size / 2);

				for (j = 0; j < 3; j++) {
					for (k = 0; k < 3; k++) {
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
					}
				}
			}
				glPopMatrix();
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (check[i][j][k]){
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
					}
				}
			}
		}
	}

	void baze_rot(int index, int sign){

		int i, j, k;

		if (sign == -1) {
			sign = 3;
		}

		while (sign--){
			if (index == 0 || index == 1) {

				k = (index & 1) * 2;

				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						tmp[j][2 - i] = a[i][j][k];
					}
				}

				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						tmp[i][j].rotateZ(), a[i][j][k] = tmp[i][j];
					}
				}

			} else if (index == 2 || index == 3) {

				j = (index & 1) * 2;

				for (i = 0; i < 3; i++) {
					for (k = 0; k < 3; k++) {
						tmp[k][2 - i] = a[i][j][k];
					}
				}

				for (i = 0; i < 3; i++) {
					for (k = 0; k < 3; k++) {
						tmp[i][k].rotateX(), a[i][j][k] = tmp[i][k];
					}
				}

			} else if (index == 4 || index == 5) {

				i = (index & 1) * 2;

				for (j = 0; j < 3; j++) {
					for (k = 0; k < 3; k++) {
						tmp[k][2 - j] = a[i][j][k];
					}
				}

				for (j = 0; j < 3; j++) {
					for (k = 0; k < 3; k++) {
						tmp[j][k].rotateY(), a[i][j][k] = tmp[j][k];
					}
				}
			}
		}
	}

	void Rotate(int index, int angle){

		if (curr_gr == -1 || curr_gr == index){
			rotate[index] += angle;

			if (rotate[index] % 90 != 0){
				curr_gr = index;
			} else {

				if ((rotate[index] < 0) ^ (curr_gr == 2 || curr_gr == 3)) {
					baze_rot(index, 1);
				} else {
					baze_rot(index, -1);
				}

				rotate[index] = 0;
				curr_gr = -1;
			}
		}
	}

};

Cube solution;

static void display(){

	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor3f(1, 0, 0);
	glTranslatef(0, 0, -30);
	glRotatef(x_rot, 1, 0, 0);
	glRotatef(y_rot, 0, 1, 0);

	glTranslatef(13 / -2.0, 13/ -2.0, 13/ -2.0);
	solution.draw();

	glPopMatrix();
	glutSwapBuffers();
}

static void reshape(int weight, int hight){

	glViewport(0, 0, weight, hight);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	GLfloat fAspect = (GLfloat)weight / (GLfloat)hight;

	gluPerspective(60, fAspect, 1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

stack<int> s;

static void keys(unsigned char key, int t1, int t2) {
	if (key >= '0' && key < '6') {
		for (int i = 0; i < 3; i++) {
			s.push(key - '0');
		}
		solution.Rotate(key - '0', 3);
		display();
	}
	
}

static void solve() {
	if(!s.empty()) {
		solution.Rotate(s.top(), 3);
		display();
		s.pop();
	}
}


time_t now = time(0);
tm* ltm = localtime(&now);

unsigned long long seed = 1975 + ltm->tm_year + ltm->tm_mon + ltm->tm_hour + ltm->tm_mday + ltm->tm_hour + ltm->tm_min + ltm->tm_sec;

static int my_rand() {
	seed = seed * 1103515245 + 12345;
	return (seed / 65536 + rand()) % 6;
}

//bool flag = true;
bool flag = false;
//bool flag2 = true;
bool flag2 = false;
int i = 0;

static void specialKeys(int key, int t1, int t2){

	if (key == GLUT_KEY_DOWN){

		x_rot -= 3;
		if (x_rot < 0) {
			x_rot += 360;
		}
		
	}

	if (key == GLUT_KEY_UP){

		x_rot += 3;
		if (x_rot >= 360) {
			x_rot -= 360;
		}
		
	}

	if (key == GLUT_KEY_RIGHT){
		
		y_rot -= 3;
		if (y_rot < 0) {
			y_rot += 360;
		}
		
	}

	if (key == GLUT_KEY_LEFT){
		y_rot += 3;
		if (y_rot >= 360) {
			y_rot -= 360;
		}
	}

	if (key == GLUT_KEY_F1) {
		flag = 1 - flag;
		i = 0;
	}

	if (key == GLUT_KEY_F2) {
		flag2 = 1 - flag2;
	}

	if (key == GLUT_KEY_F3) {
		solve();
	}

	if (key == GLUT_KEY_F4) {
		solution.create(13, col);
	}
}

static void timer(int t){
	if (solution.curr_gr != -1) {
		solution.Rotate(solution.curr_gr, 3);
	}
}


static void timer1(int t){
	//glutTimerFunc(SPEED, timer1, 0);
	if (i < 100 && flag) {
		glutTimerFunc(SPEED2, timer1, 0);
		if (solution.curr_gr == -1) {
			int pivot = my_rand();
			keys(pivot + '0', 0, 0);
			i++;
		} else {	
			solution.Rotate(solution.curr_gr, 3);
		}
	} else if (flag2 && !s.empty()) {
		glutTimerFunc(SPEED2, timer1, 0);
		if (solution.curr_gr == -1) {
			solution.Rotate(s.top(), 3);
			s.pop();
			if (s.empty()) {
				flag2 = 1 - flag2;
			}
		} else {
			solution.Rotate(solution.curr_gr, 3);
		}
	} else {
		glutTimerFunc(SPEED, timer1, 0);
		timer(0);
	}
	display();
}

int main(int argc, char** argv){

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(1, 1);

	glutCreateWindow("Cubic Rubic");

	glEnable(GL_DEPTH_TEST);
	
	solution.create(13, col);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keys);

	glutTimerFunc(SPEED, timer1, 0);
	glutSpecialFunc(specialKeys);

	glutMainLoop();

}