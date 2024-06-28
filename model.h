#ifndef __GAME_HANOY___
#define __GAME_HANOY___

#include <malloc.h>
#include "graphics.h"

#define COUNT_ASIX 10 //макс кол-во стержней 10

//варианты отображения кольца в виде прямоугольника или эллипса 
enum types_ring { tr_bar=1, tr_ellipse=2 };// перечисление вида колец // заведено 2 вида квадратные и овальные кольца
enum types_button { tb_settings=1, tb_exit, tb_start,tb_cancel,tb_about, tb_selectRings,tb_selectAsix,tb_selectTypeRings,tb_pause,tb_back}; //  тип кнопки, перечисляется тип к каждой кнопке 
enum types_scena {ts_first,ts_settings,ts_game, ts_finish, ts_about};//5 вида сцен //меню, настройки игры, игра, конец игры, о игре 
enum game_stage {gs_not,gs_exec,gs_pause,gs_finish};//стадии игры // ничего не происходит, игра, пауза , конец игры

//стуктура кнопки 
typedef struct Button {
    types_button type; // тип кнопки
    const char* str = NULL;
    int data = 0;       //данные привязанные к кнопке в зависимости от ее типа
    long color = RGB(0,0,0);
    int isActive = 0;
    int x; //координаты 
    int y;
    int x2;
    int y2;
} button;

// структура описывающая кольцо
typedef struct Ring {
    int size;   // размер кольца
    int x = 0 ;
    int y = 0;
    int x2 = 0 ;
    int y2 = 0 ;
    long color = 0; //цвет кольца
} Ring;

// структура описывающая стержень
typedef struct Asix {
    int number = 0;    // номер оси
    int heightRing = 40 ;
    int countRings;    // количество колец на стержне
    long color = 0; // стержня 
    int x = 0 ; //..координаты 
    int y = 0;
    int x2 = 0;
    int y2 = 0;
    int areaX = 0; //.. координаты границ стержня 
    int areaY = 0;
    int areaX2 = 0;
    int areaY2 = 0 ;
    void* image = NULL; // выделенный участок памяти для сохранения изображения под верхним кольцом
    Ring rings[10]; // массив из 8 колец
} Asix;

// структура описывающая всю игру
typedef struct Game {
    game_stage stage = gs_not;
    types_scena typeScena = ts_first;
    types_ring typeRing = tr_bar ;
    int isMouseMove = 0 ;       // признак того, что мы мышью схватили и двигаем кольцо
    Ring* ringForMove = NULL ;  // указаатель на структуру кольца которое двигаем 
    Asix* asixForMove = NULL;  // указатель на структуру оси кольцо с которой начали двигать
    int lastAsix =-1;
    int currentAsix=-1;
    int lastX = 0 ;
    int lastY = 0;
    int maxY = 0;
    int maxX = 0;
    int countAxis = 0;         // сколько осей в игре
    int countRings = 0;         // сколько колец в игре
    int counts = 0;             // сколько шагов сделано в игре
    Asix asix[COUNT_ASIX] ;     // массив из стержней 
} Game ;

extern Game game;//глобальная переменная game участвует во многих функциях

//функция старта игры, передается кол-во конусов 
int initGame();
int backStep();
//функция передвижения конусов откуда куда
int move(Asix* asix1, Asix* asix2);
// функция окончания игры 
int testGameOver();
int testMove(Asix* asix1, Asix* asix2);

//*******************************

enum section_type {st_select,st_itog,st_buttons}; // тип данных о секции 

//структура секции
typedef struct Section {
	section_type type;   //тип секции
	const char* name;  //имя  
	int num; // порядковый номер 
	int color = 0; // цвет   
	int countButtons = 0; // кол-во кнопок 
	int data;//данные привязанные к сцене в зависимости от ее типа
	int height = 150;
	Button buttons[10] ;
	int isFinish = 0 ;
	int x;
	int y;
	int x2;
} Scena_area;

// функция отображения меню
int showFirstMenu();
//функция отображения настроек
int showSettings();

//**********************************
void showGame(); 
int startGame();
#endif