#include "model.h"

Section settings[10] ; // управляющие кнопки на сцене //массив структуры секции
int countSections=0;
char str[9][3] ;        // для инициализации кнопок выбора количества колец

//отрисовка кнопки 
//функция получает на входе укзатель на структуру секции и указатель на структуру кнопки    
void showButton(Section* s, Button* r)
{
   long lastColor = getbkcolor() ;
    if (r->isActive) {                                          // если кнопка активна
        setfillstyle(SOLID_FILL, RGB(0, 255, 0));        // она отрисовывается зеленым цветом
        setbkcolor(RGB(0, 255, 0));
    }
    else {
        setfillstyle(SOLID_FILL, RGB(128, 128, 128));
        setbkcolor(RGB(128, 128, 128));
    }

    bar(s->x + r->x, s->y + r->y, s->x + r->x2, s->y + r->y2);  //отрисовка самой кнопки 

    if (r->str != NULL) {
        setcolor(RGB(0, 0, 0));
        outtextxy(s->x + r->x + 20, s->y + r->y + (r->y2 - r->y)/2, (char*)r->str);
    }
    setbkcolor(lastColor);
}


//функция сравнения данных привязанных к секции в зависимости от ее типа
int verifiAllData() {
    for (int i = 0; i < countSections; i++) {
            
        if (settings[i].type == st_select) { // признак того что секция отработана(кнопка нажата) 
            if (settings[i].data==0) { // в случае что в секции не нажата кнопка 
                return 0;
            }
        }
    }
    return 1;
}

//функция отрисовывает коментарии в зависимости от выбора кнопки 
//на входе получает указатель на секцию
void showComentSection(Section* s) {
    int data;
    if (s->type == st_buttons) { 
        return;
    }
    if (s->type == st_itog) { // итоговым типом секции является признак того что все предыдущие секции были отработаны 
        data = verifiAllData(); // для этого обращаемся к функции которая анализирует 
    }
    else {
        data = s->data;
    }
    if (data > 0) {
        setcolor(RGB(0, 255, 0));
        outtextxy(s->x + 10, s->y+s->height/2 ,(char*)"OK");
    }
    else
    {
        setcolor(RGB(255, 0, 0));
        outtextxy(s->x + 10, s->y + s->height / 2, (char*)"!!!   ");
    }
}

// отрисовка секции 
//на входе получает указатель на структуру секции
void showSection(Section* s)
{
    long lastColor = getbkcolor() ;
    setfillstyle(CLOSE_DOT_FILL, s->color);
    bar(s->x, s->y, s->x2, s->y + s->height);
    setcolor(RGB(0, 0, 255));
    outtextxy(s->x + 20, s->y + 5, (char*)s->name);
    showComentSection(s); 
    for (int i = 0; i < s->countButtons; i++)
    {
        Button& b = s->buttons[i];
        showButton(s, &b);
    }
    setbkcolor(lastColor);
}

// инициализация секции 
// на вход функция получает указатель на структура секция 
void initSection(Section* s)
{
    s->x = 0;
    s->x2 = game.maxX ;
    s->y = 50 + s->height * s->num ;

    int k = s->countButtons * 2 + 1;
    int w = game.maxX / k;
    for (int i = 0; i < s->countButtons; i++)
    {
        Button& b = s->buttons[i];
        b.isActive = 0;
        b.x = 0 + w + 2 * i * w;
        b.x2 = b.x + w;

        if (s->type == st_select) {
            b.y = s->height / 3;
            b.y2 = b.y + s->height / 3;
        }
        else {

            b.y = s->height / 4;
            b.y2 = b.y + s->height / 2;        
        }
    }
}

// функция проверяет координаты кнопки
//на входе получает указатель на структуру секции и координаты 
//возвращает указатель на структуру кнопки 
Button* getButtonByCoord(Section* s,int x, int y) 
{
    for (int i = 0; i < s->countButtons; i++) 
    {
        Button* b = &s->buttons[i];
        if (s->x+b->x <= x && s->x+b->x2 >= x && s->y+b->y <= y && s->y + s->height+b->y2 >= y)
        {
            return b;
        }
    }
    return NULL; // если на кнопку не попали
}

//проверка на попадание в секцию 
// получает на входе координаты 
//возвращает указатель на структуру сеции 
Section* getSectionByCoord(int x, int y) 
{
    for (int i = 0; i < countSections; i++)
    {
        Section* s = &settings[i];
        if (s->x <= x && s->x2 >= x && s->y <= y && s->y + s->height >= y) //проверка по координатам
        {
            return s;
        }
    }

    return NULL;
}

// функция выполняет проверку на нажате мыши в сцене настроек
void sets_onLeftBtnClick(int x, int y) {
    if (game.typeScena != ts_settings && game.typeScena != ts_first) { //функция выполняется только на сцене настроек и меню 
        return;
    }
    Section* s = getSectionByCoord(x, y); // узнать попали ли мы в область сеции  мышью
    if (s != NULL) { // попали в секцию
        Button* b = getButtonByCoord(s, x, y); //вызываем функцию на проверку нажатия по кнопке 
        if (b != NULL) { // если нажали на кнопку 
            switch (b->type)  // 
            {
            case tb_start: 
                if (verifiAllData() == 1) {
                    game.countAxis  = settings[0].data;
                    game.countRings = settings[1].data;
                    game.typeRing = (types_ring)settings[2].data;
                    game.typeScena = ts_game;
                }
                break;
            case tb_cancel: // нажали на назад 
                game.typeScena = ts_first; // сцена ставится меню 
                break;

            case tb_about: // нажали о игре 
               game.typeScena = ts_about; // сцена ставится о игре 
                break;

            case tb_exit: // тип кнопки если выход 
                game.typeScena = ts_finish; // 
                break;

            case tb_settings:
                game.typeScena = ts_settings;
                break;

            default:
                Button* b2 = NULL;
                int lastData = 0;
                for (int i = 0; i < s->countButtons; i++)
                {
                    // отработаем случай повторного нажатия на зеленую кнопку
                    b2 = &s->buttons[i] ;
                    if (b2->isActive == 1) {
                        b2->isActive = 0;                      // сбрасываем признак активности кнопки
                        lastData = b2->data;                 // чтобы сейчас обратно не активировать эту кнопку
                        s->data = 0 ;                           // признак того, что в секции ничего не выбрали
                        showButton(s, b2);                    // отрисовываем кнопку (станет обратно серой)
                        break;
              
                    }
                 }
               // проверяем, что кликнули по ранее неактивной кнопке
                if (b->data != lastData) {      
                    b->isActive = 1;                            // делаем эту кнопку активной
                    s->data = b->data;                       // данные этой кнопки передаем в секцию
                    showButton(s, b);                          // отрисовываем кнопку (станет зеленой)
                }
                showComentSection(s);                      // выводим признак присутствия данных в секции (зеленое ОК или красные !!!)
                showComentSection(&settings[countSections - 1]);        // выводим итоги в самом низу (возможность старта игры)
            }
        }
    }
}

// для сцены настройки обращаемся к массиву структур для инициализации кнопок в соответствующей секции 
void initSettings()
{
   //заводим кол-во секций 
    countSections = 4;
    settings[0] = { st_select,"Выберите количество осей", 0, RGB(0,255,0),   3, 0}; //...,2-строка, 3-порядковый номер, 4-цвет, 5 - сколько кнопок содержит,данные  
    settings[0].buttons[0] = { tb_selectAsix, "3", 3 } ; // для соответствующей кнопки 1 секции кнопки выставляем 1-стержень, 2-строку, 3 - ее значение(data)
    settings[0].buttons[1] = { tb_selectAsix, "4", 4 };
    settings[0].buttons[2] = { tb_selectAsix, "5", 5 };
   // для каждой секции и кнопки переданы соответствующие значения 
    settings[1] = { st_select,"Выберите количество колец", 1, RGB(0,128,128),  9, 0};
   
   
   for(int i=0 ; i<=8 ; i++) { 
     sprintf(str[i], "%d", i+2);
     settings[1].buttons[i] = { tb_selectRings, str[i], i+2 };
   }
   
    settings[2] = { st_select,"Выберите тип кольца", 2, RGB(128,128,0),   2, 0 };
    settings[2].buttons[0] = { tb_selectTypeRings, "Эллипс", tr_ellipse };
    settings[2].buttons[1] = { tb_selectTypeRings, "Прямоугольник", tr_bar };

    settings[3] = { st_itog,"", 3, RGB(255,255,255),  2};
    settings[3].buttons[0] = { tb_start, "СТАРТ !"};
    settings[3].buttons[1] = { tb_cancel, "ВЫХОД" };

    for (int i = 0; i < countSections; i++) {
        initSection(&settings[i]);
    }
    registermousehandler(WM_LBUTTONDOWN, sets_onLeftBtnClick); // привязываем события мыши
}

//функция инициализирует для сцены меню кнопки через секции 
void initFirstMenu()
{
   // 3 кнопки- 3 секции
    countSections = 3 ;
   
   //для первой секции 
    settings[0] = { st_buttons,"", 0, RGB(255,255,255),   1, 0, 250 }; // тип секции,имя(пустое),порядковый номер, цвет, кол-во кнопок, данные, размер, высота кнопки 
    settings[0].buttons[0] = { tb_settings, "НАЧАТЬ ИГРУ",0, RGB(0,0,0)}; // тип кнопки, строка, данные и цвет 

    settings[1] = { st_buttons,"", 1, RGB(255,255,255),   1, 0, 250 };
    settings[1].buttons[0] = { tb_about, "ПОМОЩЬ"};

    settings[2] = { st_buttons,"", 2, RGB(255,255,255),   1, 0, 250 };
    settings[2].buttons[0] = { tb_exit, "ВЫХОД"};

    for (int i = 0; i < countSections; i++) {
        initSection(&settings[i]); 
    }
    registermousehandler(WM_LBUTTONDOWN, sets_onLeftBtnClick); // привязываем событие 
}


// функция показывает меню игры 
int showFirstMenu()
{
    if (game.typeScena != ts_first) { 
        return 1; //возврат 1 если сцена любая кроме меню 
    }

    initFirstMenu(); // иницициализация меню
    cleardevice();
    for (int i = 0; i < countSections; i++) {
        showSection(&settings[i]);
    }

    while (1) {
        delay(100); //проверяем по времени что мы не перешли на другую сцену 
        if (game.typeScena != ts_first) {
            break;
        }
    }
    return 1;
}

//функция показывает настройки игры 
int showSettings()
{
    if (game.typeScena != ts_settings) { // проверка на то что мы находимся на сцене настройки 
        return 1;
    }

    initSettings(); // инициализация настроек 
    cleardevice();
    for (int i = 0; i < countSections; i++) {
        showSection(&settings[i]);
    }

    while (1) {
        delay(100);
        if (game.typeScena != ts_settings) {
            break;   // переходим на другую сцену
        }
    }
    return 1;
}
