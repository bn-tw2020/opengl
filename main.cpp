#include<cmath>
#include<iostream>
#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>

#define CUBE_SIZE 30.0f //소스큐브의 크기(한변의 길이)
#define Robot_x 400.0f //로봇의 중심좌표 x
#define Robot_y 0.0f //로봇의 중심좌표 y
#define RB_BODY_SIZE 120.0f //로봇 몸통 크기
#define RB_JOINT_GAP 20.0f //로봇관절 틈의 크기
#define RB_LEG_LENGTH 120.0f //로봇 다리 길이
#define RB_ARM_LENGTH 140.0f // 로봇 다리 길이
#define VIEW_SIZE 700.0f //가시부피 크기

static double time = 0; //애니메이션에 사용할 time
GLfloat RB_HEAD_SIZE = 50.0f; //로봇의 머리 크기
GLfloat center_Y = 0.0f; //화면 중심 y축의 각 변수
GLfloat left_arm_X = 0.0f; //왼팔 x축의 각도
GLfloat right_arm_X = 0.0f; //오른쪽 x축의 각도
GLfloat left_up_leg_X = 0.0f; //왼쪽다리 관절 x축
GLfloat right_up_leg_X = 0.0f; //오른쪽 다리 관절 x축
GLfloat left_down_leg_X = 0.0f; //왼쪽 무릎 관절
GLfloat right_down_leg_X = 0.0f; //오른쪽 무르 관절
GLfloat body_Y = 0.0f; //몸 중심축
GLfloat head_Y = 0.0f; //머리 중심축
GLfloat color = 0.3f; //색깔의 변화를 위해서
GLfloat pos_ball = 0.0f; //공의 위치 변화를 하기 위해서
GLfloat rot_ball = 0.0f; //공의 회전을 하기 위해서
GLfloat zoom = 0.5; //줌인 기능을 위해서
GLfloat moved = 0;	// 좌 우로도 작동하기 위해서
int flag; //로봇의 형태를 바꾸기 위해서(원통 로봇, 큐브로봇)

void Init() {
	glEnable(GL_DEPTH_TEST); //은면 제거
	glEnable(GL_NORMALIZE); //정규화 
	glEnable(GL_SMOOTH); //물체의 부드럽게 구로 셰이딩
	glEnable(GL_LIGHTING); //조명 활성화
	
	GLfloat ambientLight[] = { 0.3f,0.3f,0.3f,1.0f }; //주변광 설정
	GLfloat diffuseLight[] = { 0.7f,0.7f,0.7f,1.0f }; //확산광 설정
	GLfloat specular[] = { 1.0f,1.0f,1.0f,1.0f }; //경면사광 설정
	GLfloat specref[] = { 1.0f,1.0f,1.0f,1.0f }; //하이라이트를 위해
	GLfloat position[] = { 0.2f,0.3f,0.4f,1.0f }; //조명 위치

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight); //위에서 설정한 변수들을 넣어주므로 조명을 정한다.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position); //조명의 위치는 0.2, 0.3, 0.4으로 설정
	glEnable(GL_LIGHT0); //LIGHT0 활성화
	 
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128); //빛을 받는 각도에 따라 로봇 색깔의 변화를 위한 설정
	glClearColor(1.0, 1.0, 1.0, 1.0); //초기화 배경을 흰색

	glMatrixMode(GL_PROJECTION); //투상행렬
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE); //평행투상으로 뷰사이즈로 설정

}

void drawstick(GLfloat angle) {
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-10, 100, 0);
	glVertex3f(10, 100, 0);
	glVertex3f(10, -420, 0);
	glVertex3f(-10, -420, 0);
	glEnd();
}

void drawArm() {
	//왼팔
	glColor3f(color, color + 0.1, (color + 1.0) - 0.1); //마우스 클릭시 화면 색상을 변경하기 위하여
	glTranslatef(RB_BODY_SIZE / 2.0f + RB_JOINT_GAP, RB_BODY_SIZE*0.9, 0.0f);
	glRotatef(15, 0, 0, 1);	//팔축을 중심으로 한 팔의 현재 움직임.
	glRotatef(left_arm_X, 1, 0, 0); //팔을 x축 기준으로 회전
	glRotatef(-30, 0, 1, 0);

	glScalef(RB_ARM_LENGTH / 4.0 / CUBE_SIZE, RB_ARM_LENGTH / CUBE_SIZE, RB_ARM_LENGTH / 4.0 / CUBE_SIZE);//팔크기처럼 만들기 위해서
	glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f); //
	if (flag == 1) glutSolidCube(CUBE_SIZE); //큐브 로봇 왼팔 그리기
	else glutSolidSphere(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE); //원형 로봇 왼팔그리기

	glPopMatrix();
	glPushMatrix();
	//오른팔
	glTranslatef(-(RB_BODY_SIZE / 2.0f + RB_JOINT_GAP), RB_BODY_SIZE*0.9, 0.0f);
	glRotatef(-15, 0, 0, 1);	//팔축을 중심으로 한 팔의 현재 움직임.
	glRotatef(right_arm_X, 1, 0, 0); //팔을 x축 기준으로 흔들기(회전)
	glRotatef(30, 0, 1, 0);

	glScalef(RB_ARM_LENGTH / 4.0 / CUBE_SIZE, RB_ARM_LENGTH / CUBE_SIZE,RB_ARM_LENGTH / 4.0 / CUBE_SIZE); //팔크기처럼 만들기 위해서
	glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);
	if (flag == 1) glutSolidCube(CUBE_SIZE); //큐브 로봇 오른팔 그리기
	else glutSolidSphere(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE); //원형 로봇 오른팔 그리기
	
}

void drawLeg() {
	glColor3f((color + 1.0) - 0.1, color, color + 0.1); //마우스 클릭시 화면 색상을 변경하기 위하여
	glTranslatef(RB_BODY_SIZE / 4.0f, -RB_JOINT_GAP, 0.0f); //윗 다리 중심
	glRotatef(2, 0, 0, 1); //몸통과 다리 각도
	glRotatef(left_up_leg_X, 1, 0, 0); //다리 흔들기(회전)
	glScalef(RB_LEG_LENGTH / 2.5 / CUBE_SIZE, RB_LEG_LENGTH / CUBE_SIZE, RB_LEG_LENGTH / 2.5 / CUBE_SIZE); //다리크기와 비슷하게 스케일설정
	glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);
	if (flag == 1)glutSolidCube(CUBE_SIZE); //왼쪽 무릎다리까지 그리기(큐브 로봇)
	else glutSolidSphere(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);//왼쪽 무릎 다리까지 그리기(원형 로봇)

	glScalef(CUBE_SIZE*2.5 / RB_LEG_LENGTH, CUBE_SIZE / RB_LEG_LENGTH, CUBE_SIZE*2.5 / RB_LEG_LENGTH); //스케일 원상 복구

	glTranslatef(0.0f, -RB_LEG_LENGTH / 2.0f - RB_JOINT_GAP, 0.0f); //밑다리 중심
	glRotatef(left_down_leg_X, 1, 0, 0); //무릎 각도 및 흔들기
	glScalef(RB_LEG_LENGTH / 2.5 / CUBE_SIZE, RB_LEG_LENGTH / CUBE_SIZE, RB_LEG_LENGTH / 2.5 / CUBE_SIZE);
	glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);
	if (flag == 1) glutSolidCube(CUBE_SIZE); //왼쪽다리 끝까지 그리기(큐브 로봇)
	else glutSolidSphere(CUBE_SIZE - 10, CUBE_SIZE - 10, CUBE_SIZE - 10); //왼쪽다리 끝까지 그리기(원형 로봇)

	glPopMatrix();
	glPushMatrix();

	glTranslatef(-RB_BODY_SIZE / 4.0f, -RB_JOINT_GAP, 0.0f); //윗다리 중심
	glRotatef(-2, 0, 0, 1); //몸통과 다리 각도
	glRotatef(right_up_leg_X, 1, 0, 0); //다리흔들기
	glScalef(RB_LEG_LENGTH / 2.5 / CUBE_SIZE, RB_LEG_LENGTH / CUBE_SIZE, RB_LEG_LENGTH / 2.5 / CUBE_SIZE); //다리크기와 비슷하게 스케일설정
	glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);
	if (flag == 1) glutSolidCube(CUBE_SIZE); //오른쪽 다리 무릎까지 그리기(큐브 로봇)
	else glutSolidSphere(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE); //오른쪽 다리무릎까지 그리기(원형 로봇)

	glScalef(CUBE_SIZE*2.5 / RB_LEG_LENGTH, CUBE_SIZE / RB_LEG_LENGTH, CUBE_SIZE*2.5 / RB_LEG_LENGTH); //스케일원상복구
	glTranslatef(0.0f, -RB_LEG_LENGTH / 2.0f - RB_JOINT_GAP, 0.0f);//밑다리 중심
	glRotatef(right_down_leg_X, 1, 0, 0);//무릎 각도, 무릎 흔들기
	glScalef(RB_LEG_LENGTH / 2.5 / CUBE_SIZE, RB_LEG_LENGTH / CUBE_SIZE, RB_LEG_LENGTH / 2.5 / CUBE_SIZE);
	glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);
	if (flag == 1) glutSolidCube(CUBE_SIZE); //오른쪽 다리끝까지그리기(큐브 로봇)
	else glutSolidSphere(CUBE_SIZE - 10, CUBE_SIZE - 10, CUBE_SIZE - 10); //오른쪽 다리끝까지 그리기(원형 로봇)
}

void drawBody() {
	glColor3f(color, (color + 1.0) - 0.1, color + 0.1); //마우스 클릭시 색상 변경을 위하여
	glTranslatef(Robot_x, Robot_y, 0.0f); //로봇 중심
	glRotatef(0, 0, 1, 0); //로봇을 돌리기 위한 회전각
	glPushMatrix(); //로봇의 각 부위를 그릴 때 중심이 되는 푸시 매트릭스

	glScalef(RB_BODY_SIZE / CUBE_SIZE, RB_BODY_SIZE / CUBE_SIZE, RB_BODY_SIZE / CUBE_SIZE); //몸통 크기 설정
	glTranslatef(0.0f, CUBE_SIZE / 2, 0.0f);
	glRotatef(body_Y, 0, 1, 0); //몸통 흔들기
	if (flag == 1)glutSolidCube(CUBE_SIZE); //큐브 로봇 몸통그리기
	else glutSolidSphere(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE); //원형 로봇 몸통그리기
}

void drawHead() {
	glColor3f(color + 1, color + 1, color - 1); //마우스 클릭시 색상 변경을 위하여
	glTranslatef(0.0f, RB_BODY_SIZE + RB_JOINT_GAP + (RB_HEAD_SIZE / 2.0f), 0.0f); //얼굴 중심
	glRotatef(head_Y, 0, 1, 0);//얼굴 움직임 
	glScalef(RB_HEAD_SIZE / CUBE_SIZE, RB_HEAD_SIZE / CUBE_SIZE, RB_HEAD_SIZE / CUBE_SIZE); //얼굴 크기 설정
	if (flag == 1) glutSolidCube(CUBE_SIZE); //큐브 로봇 얼굴
	else glutSolidSphere(CUBE_SIZE + 10, CUBE_SIZE + 10, CUBE_SIZE + 10); //원형 로봇 얼굴
}

void drawBoard() {
	glBegin(GL_TRIANGLE_FAN); //운동장을 그리기 위해
	glColor3f(0.8f, 0.8f, 1.8f); //밝은 파란색으로 설정
	glNormal3f(0, 1, 0);
	glVertex3f(0.0f, -278, 0.0f); //운동장 중심점
	float angle;//GL_TRIANGLE_FAN으로 그리기 때문에 인터넷 검색을 통해 원을 그리는 법을 찾아서 합침.
	for (angle = 0.0f; angle < (2.0f*3.14159); angle += (3.14159 / 360.0f)) {
		glVertex3f((500 * cos(angle)), -278.0f, (500 * sin(angle)));
	}
	glEnd();
}

void drawball() {
	glColor3f(color + 0.3, color + 0.5, color - 0.4); //공 색상도 같이 변하게 하기 위해서
	glTranslatef(pos_ball + 400.0f, -RB_LEG_LENGTH / 2 - 150.0f - RB_JOINT_GAP, pos_ball + 70.0f);//밑다리 중심으로 공을 그림.
	glRotatef(rot_ball - 100, 0, 1, 0);
	glScalef(50.0f / CUBE_SIZE, 50.0f / CUBE_SIZE, 50.0f / CUBE_SIZE); //공의 크기 설정
	glutSolidSphere(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE); //원형의 공을 그린다.
}


void robot_Display() {//디스플레이 콜백함수
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //모델뷰행렬로 설정
	glPushMatrix();

	glScalef(zoom, zoom, zoom);	//줌인 아웃 기능
	glTranslatef(moved, 0, 0); //좌우 기능
	gluLookAt(50, 40, 80, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //시점
	
	drawBoard();	//운동장 그리기

	glRotatef(center_Y, 0, 1, 0.0); //앵글 축으로 좌표 값을 변환해주어 로봇이 회전하게함.
	glPushMatrix();
	drawstick(50.0f); //회전축

	drawBody(); //몸통 그리기
	glPopMatrix();
	glPushMatrix();

	drawHead(); //머리 그리기
	glPopMatrix();
	glPushMatrix();

	drawLeg(); //다리 그리기
	glPopMatrix();
	glPushMatrix();

	drawArm(); //팔 그리기
	glPopMatrix();
	glPopMatrix();

	drawball(); //공 그리기
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers(); //이중 버퍼를 쓰기 때문에 Flush 대신 SwapBuffers을 이용해야함.
}

void timerFunction(int value) {//타이머 콜백 함수
	time = time + 0.1; //시간증가 변수
	center_Y -= 1; //축에서 값을 계속 변환시켜서 회전 효과를 주기 위해서

	head_Y = sin(time) * 5; //머리는 5도 각도로 움직임
	body_Y = sin(time) * 10; //몸통은 10도 각도로 움직임

	left_arm_X = sin(time) * 20; //왼팔은 20도 각도로 움직임
	right_arm_X = -left_arm_X; //오른쪽팔도 20도 각도로 움직임

	right_up_leg_X = sin(time) * 20; //오른쪽 다리는 20도 각도로 움직임
	left_up_leg_X = -right_up_leg_X; //왼쪽 다리는 오른쪽 각도와 똑같이

	right_down_leg_X = abs(right_up_leg_X / 1.2f); //오른쪽 무릎은 다리 각도를 1.2로 나눈 값
	left_down_leg_X = abs(left_up_leg_X / 1.2f); //왼쪽 무릎은 다리 각도를 1.2로 나눈값

	rot_ball = sin(time) * 20; //공은 20도 각도로 회전

	glutPostRedisplay(); //애니메이션을 설정했으므로 다시 디스플레이 콜백함수 호출

	glutTimerFunc(22, timerFunction, 1); //타이머콜백함수는 재귀호출방식으로 다음 타이머설정
}

void MyMouseClick(int Button, int State, int X, int Y) { //마우스 콜백함수
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) { //왼쪽 마우스 버튼이 눌리면
		color += 0.1; //로봇이 함수에서 설정한 color들의 색상을 0.1씩 증가
		if (color > 1.0) color = 0.0; //로봇의 색상이 하얀색을 넘어서면 다시 0으로 설정
	}
	glutPostRedisplay(); //디스플레이 콜백함수 호출
}

void MyKeyboard(unsigned char key, int x, int y) { //키보드 콜백 함수
	switch (key) {
	case 'b': //b를 누를 때
		RB_HEAD_SIZE += 10.0f; //로봇의 머리가 커짐
		glutPostRedisplay();
		break;
	case 's': //s를 누를 때
		RB_HEAD_SIZE -= 10.0f; //로봇의 머리가 작아짐
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void spkey(int key, int x, int y) { // 스폐셜 콜백 함수
	switch (key) {
	case GLUT_KEY_UP: //화살표 위 방향키를 누를 때
		zoom += 0.005; //줌인
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN: //화살표 아래 방향키를 누를 때
		zoom -= 0.005; //줌 아웃
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT: //화살표 오른쪽을 누를 때
		moved += 5; //우로 이동
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT: //화살표 왼쪽을 누를 때
		moved -= 5; //좌로 이동
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void MyMainMenu(int entryID) {// 메뉴 콜백 함수
	if (entryID == 0) flag = 1; //큐브 로봇을 위해서
	else flag = 0; //원형 로봇을 위해서
	glutPostRedisplay(); //메뉴를 선택하면 그것에 따라 디스플레이 콜백 함수 호출
}

int main(int argc, char **argv) { //메인 함수
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //디스플레이 모드를 이중 버퍼, 깊이버퍼, 색상버퍼 설정
	glutInitWindowSize(700, 700); // 윈도우의 크기를 700 x 700으로 설정
	glutInitWindowPosition(0, 0); //윈도우의 위치를 컴퓨터 좌상단 0, 0에 위치
	glutCreateWindow("20164045"); //윈도우 제목을 필자의 학번인 20164045로 설정
	glutTimerFunc(1000, timerFunction, 1); //이벤트에 따라 타이머 콜백 함수를 1000당 호출
	glutDisplayFunc(robot_Display); //이벤트에 따라 디스플레이 콜백 함수 호출
	glutMouseFunc(MyMouseClick);// 이벤트에 따라 마우스 콜백 함수 호출
	glutKeyboardFunc(MyKeyboard); // 이벤트에 따라 키보드 콜백 함수 호출
	glutSpecialFunc(spkey); //이벤트에 따라 키보드 스페셜 콜백 함수 호출
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu); //메뉴 콜백 함수 설정
	glutAddMenuEntry("Cube Robot", 0); //메뉴을 추가(큐브로봇)
	glutAddMenuEntry("Sphere Robo1", 1);//메뉴을 추가(원형로봇)
	glutAttachMenu(GLUT_RIGHT_BUTTON);//오른쪽 마우스 버튼을 클릭시 메뉴가 보임
	Init(); //초기화 함수 호출
	glutMainLoop();	//이벤트를 무한루프를 돌면서 gl진행

}
