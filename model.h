#ifndef __GAME_HANOY___
#define __GAME_HANOY___

#include <malloc.h>
#include "graphics.h"

#define COUNT_ASIX 10 //���� ���-�� �������� 10

//�������� ����������� ������ � ���� �������������� ��� ������� 
enum types_ring { tr_bar=1, tr_ellipse=2 };// ������������ ���� ����� // �������� 2 ���� ���������� � �������� ������
enum types_button { tb_settings=1, tb_exit, tb_start,tb_cancel,tb_about, tb_selectRings,tb_selectAsix,tb_selectTypeRings,tb_pause,tb_back}; //  ��� ������, ������������� ��� � ������ ������ 
enum types_scena {ts_first,ts_settings,ts_game, ts_finish, ts_about};//5 ���� ���� //����, ��������� ����, ����, ����� ����, � ���� 
enum game_stage {gs_not,gs_exec,gs_pause,gs_finish};//������ ���� // ������ �� ����������, ����, ����� , ����� ����

//�������� ������ 
typedef struct Button {
    types_button type; // ��� ������
    const char* str = NULL;
    int data = 0;       //������ ����������� � ������ � ����������� �� �� ����
    long color = RGB(0,0,0);
    int isActive = 0;
    int x; //���������� 
    int y;
    int x2;
    int y2;
} button;

// ��������� ����������� ������
typedef struct Ring {
    int size;   // ������ ������
    int x = 0 ;
    int y = 0;
    int x2 = 0 ;
    int y2 = 0 ;
    long color = 0; //���� ������
} Ring;

// ��������� ����������� ��������
typedef struct Asix {
    int number = 0;    // ����� ���
    int heightRing = 40 ;
    int countRings;    // ���������� ����� �� �������
    long color = 0; // ������� 
    int x = 0 ; //..���������� 
    int y = 0;
    int x2 = 0;
    int y2 = 0;
    int areaX = 0; //.. ���������� ������ ������� 
    int areaY = 0;
    int areaX2 = 0;
    int areaY2 = 0 ;
    void* image = NULL; // ���������� ������� ������ ��� ���������� ����������� ��� ������� �������
    Ring rings[10]; // ������ �� 8 �����
} Asix;

// ��������� ����������� ��� ����
typedef struct Game {
    game_stage stage = gs_not;
    types_scena typeScena = ts_first;
    types_ring typeRing = tr_bar ;
    int isMouseMove = 0 ;       // ������� ����, ��� �� ����� �������� � ������� ������
    Ring* ringForMove = NULL ;  // ���������� �� ��������� ������ ������� ������� 
    Asix* asixForMove = NULL;  // ��������� �� ��������� ��� ������ � ������� ������ �������
    int lastAsix =-1;
    int currentAsix=-1;
    int lastX = 0 ;
    int lastY = 0;
    int maxY = 0;
    int maxX = 0;
    int countAxis = 0;         // ������� ���� � ����
    int countRings = 0;         // ������� ����� � ����
    int counts = 0;             // ������� ����� ������� � ����
    Asix asix[COUNT_ASIX] ;     // ������ �� �������� 
} Game ;

extern Game game;//���������� ���������� game ��������� �� ������ ��������

//������� ������ ����, ���������� ���-�� ������� 
int initGame();
int backStep();
//������� ������������ ������� ������ ����
int move(Asix* asix1, Asix* asix2);
// ������� ��������� ���� 
int testGameOver();
int testMove(Asix* asix1, Asix* asix2);

//*******************************

enum section_type {st_select,st_itog,st_buttons}; // ��� ������ � ������ 

//��������� ������
typedef struct Section {
	section_type type;   //��� ������
	const char* name;  //���  
	int num; // ���������� ����� 
	int color = 0; // ����   
	int countButtons = 0; // ���-�� ������ 
	int data;//������ ����������� � ����� � ����������� �� �� ����
	int height = 150;
	Button buttons[10] ;
	int isFinish = 0 ;
	int x;
	int y;
	int x2;
} Scena_area;

// ������� ����������� ����
int showFirstMenu();
//������� ����������� ��������
int showSettings();

//**********************************
void showGame(); 
int startGame();
#endif