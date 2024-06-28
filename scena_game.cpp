#include "model.h"

//массив из цветов для каждого конуса
long colors[10] = { RGB(255,69,0), RGB(106,90,205) , RGB(255,0,0) , RGB(60,179,113), RGB(255,160,122) , RGB(255,20,147) , RGB(255,215,0) , RGB(55,215,1000), RGB(255,165,0), RGB(155,165,12) };

Button ctrl_buttons[10];        // управляющие кнопки на сцене
int ctrl_counts = 0;

// вычисление координат чтобы нарисовать кольцо
void calcCoordRings(Asix& asix) {
    int maxW = 0.9 * (asix.areaX2 - asix.areaX) ; 
    for (int i = 0; i < asix.countRings; i++) {    // Цикл по количеству колец 
        Ring* r = &asix.rings[i];
        int w = maxW * r->size / game.countRings; // высота будет делиться на 8 частей под каждое кольцо 
        r->x = asix.areaX + ((asix.areaX2 - asix.areaX) - w) / 2;// определяется центр стержня чтобы положить кольцо
        r->x2 = r->x + w;
        r->y2 = asix.areaY2 - asix.heightRing * i ;//высота определяется по кол-ву колец на стержне 
        r->y = r->y2 - asix.heightRing;
        r->color = colors[r->size-1];//сразу задаем кольцу цвет 
    }
}
//функция получает на входе указатель на структуру кольца
//выпроняет отрисовку 
void showRing(Ring* r)
{
    long lastColor = getbkcolor() ;
    setfillstyle(SOLID_FILL, r->color);
    
    switch (game.typeRing) {
    case tr_bar:
        bar(r->x, r->y, r->x2, r->y2);  //отрисовываем кольцо типа прямоугольник если выбран тип соответствующий 
        break;
    case tr_ellipse:
        int cX = r->x + (r->x2 - r->x) / 2;
        int cY = r->y + (r->y2 - r->y) / 2;
        fillellipse(cX, cY, (r->x2 - r->x) / 2, (r->y2 - r->y) / 2);
        break;
    }

    char str[5];
    sprintf_s(str, "%d", r->size);
    setcolor(RGB(0, 0, 0));
    setbkcolor(r->color);
    outtextxy(r->x + (r->x2 - r->x)/2 - 5, r->y + (r->y2 - r->y) / 2, str);
    setbkcolor(lastColor);

}

//отрисока колец на оси
// функция получает на входе стержень
void showRings(Asix& asix) {
    calcCoordRings(asix);//вызывается функция с вычислением координат кольца

    if (asix.image != NULL) { // если ранее был выделен участок памяти под верхнее кольцо 
        free(asix.image); //то мы ее освобождаем, т.к новое верхнее кольцо может быть другого размера  
        asix.image = NULL;
    }

    for (int i = 0; i < asix.countRings; i++) {
        Ring* r = &asix.rings[i];
        if (i == asix.countRings - 1) {
            int memsize = imagesize(r->x, r->y, r->x2, r->y2); //кол-во байт которое потребуется для хранения области под кольцом 
            asix.image = malloc(memsize); // выделяем память 
            getimage(r->x, r->y, r->x2, r->y2, asix.image);
        }
        showRing(r);
    }
}

// вычисление границ и обалсти стержня 
//на входе получает ссылку на структуру стержня, ширину и высоту экрана
void __initAsix(Asix& asix, int w, int h) { // функция получает на входе координаты и стержень
    asix.areaX = asix.number * w + w / 2; //вычисляются координаты области стержня
    asix.areaX2 = asix.areaX + w;
    asix.x = asix.number * w + w - 5; //координаты ширины стержня
    asix.x2 = asix.x + 10;

    asix.y = h/2 ; //высота стержня - половина экрана
    asix.y2 = h-10 ; // отступ от нижней границы

    asix.areaY = asix.y - h / (game.countAxis +1);//отступ от границ экрана 
    asix.areaY2 = asix.y2;
 }

//вычисление координт положения стержней 
void calcCoordAsix() {
    int w = game.maxX / (game.countAxis+1) ;//кол-во областей на 1 больше чем осей 
    int h = game.maxY ;//высота 

    for (int i = 0; i < game.countAxis; i++) {
        __initAsix(game.asix[i], w, h); //вызывается функцию с расчетами
    }
}

// отрисовка области границ стержней 
void showAsixes() {
    //за один цикл отрисовывается сам стержень и область стержня 
    for (int i = 0; i < game.countAxis; i++) {

        setfillstyle(CLOSE_DOT_FILL, game.asix[i].color); //устанавливаем цвет и стиль заливки для области стержня  
        bar(game.asix[i].areaX, game.asix[i].areaY, game.asix[i].areaX2, game.asix[i].areaY2);//рисуем область 

        setfillstyle(SOLID_FILL, game.asix[i].color); //устанавливаем цвет и стиль заливки для стержня
        bar(game.asix[i].x, game.asix[i].y, game.asix[i].x2, game.asix[i].y2);// рисуем стержень
    }
}
// функция получает координаты на входе 
// возвращает указатель на структуру стержня 
Asix* getAsixByCoord(int x, int y) {
    for (int i = 0; i < game.countAxis; i++) {
        Asix* asix = &game.asix[i];
        if (asix->areaX <= x && asix->areaX2 >= x && asix->areaY <= y && asix->areaY2 >= y) {
            return asix;  //возвращает стержень 
        }
    }
    return NULL ; // если мы не попадаем координатами в область стержня возврат NULL
}

// функция воозвращает номер кольца на стержне 
// получает указатель на структуру стержня , координаты 
int getRingByCoord(Asix* asix, int x, int y) {
    for (int i = 0; i < asix->countRings; i++) {
        Ring* r = &asix->rings[i];
        if (r->x <= x && r->x2 >= x && r->y <= y && r->y2 >= y) {
            return i ; //номер кольца на стержне
        }
    }
    return -1 ; // колец нет на стержне 
}
//вывод на экран счетчика 
void showCounter() {
    char str[25];
    long lastColor = getbkcolor() ;    
    sprintf_s(str, "Количество ходов: %d", game.counts);
    setbkcolor(WHITE);
    setcolor(BLACK);
    outtextxy(100,50,str);
    setbkcolor(lastColor);
}

//функция при переходе стадии сцены в игре изменяет текст в игре 
void showStage() {
    char str[25];
    switch (game.stage) {
    case gs_exec:// если идет игра
        sprintf(str, "Игра выполняется...");
        setcolor(RGB(0, 255, 0));
        break;
    case gs_finish:// если игра закончена 
        sprintf(str, "Игра окончена"); 
        setcolor(RGB(255, 0, 0));
        break;
    case gs_pause: // если пауза 
        sprintf(str, "Пауза");
        setcolor(RGB(0, 0,255));
        break;
    }
    outtextxy(100, 100, str);
}

// функция выполяет вывод на экран кнопок на сцене игры 
void showCtrlButtons() {
    Button* b=NULL;
    long lastColor = getbkcolor() ;    
    for (int i = 0; i < ctrl_counts; i++) {
        b = &ctrl_buttons[i];
        if(b->isActive) {
           setcolor(RGB(0, 0, 0));
           setfillstyle(SOLID_FILL, b->color);
           setbkcolor(BLACK);
           setcolor(WHITE);
           bar(b->x, b->y, b->x2, b->y2);
           outtextxy(b->x + 5, b->y + 5, (char*)b->str);
        }
    }
    setbkcolor(lastColor);
}

// функция получает на входе координаты самой игры 
// возвращает указатель на стуктуру кнопки 
Button* getCtrlButtonByCoord(int x, int y) {
    for (int i = 0; i < ctrl_counts; i++) {
        Button* b = &ctrl_buttons[i];
        if (b->x <= x && b->x2 >= x && b->y <= y && b->y2 >= y) { // проверка на вхождение в координаты 
            return b; // возврат кнопки 
        }
    }
    return NULL;
}

// нажате мыши в самой игре 
void game_onLeftBtnClick(int x, int y) {
    if (game.typeScena != ts_game) {            // выполняется только когда сцена игра
        return;
    }

    Button* b = getCtrlButtonByCoord(x, y); // вызывется функция попадания в кнопку самой игры 
    if (b != NULL) { // попали в кнопу 
        switch (b->type) {                         // проверка типа кнопки 
           
        case tb_back:                                 //возварт хода 
           if( game.stage == gs_exec){          //проверка того что стадия игры находится не в паузе 
               backStep();                              //вызываем функцию
               showGame();                              // отрисовываем сцену
           }
           break;
        case tb_cancel:                                 // если тип кнопки установлен на отмену 
            game.typeScena = ts_first;              // покидаем сцена меню 
            break;
        }
        return ;
    }

    if (game.stage != gs_exec) {                    
        return;
    }

    game.isMouseMove = 0;  //мышь отжата 
    Asix* asix = getAsixByCoord(x, y); 
    if (asix != NULL) {
        //точно попали в область оси

        int ring = getRingByCoord(asix, x, y);//узнаем номер кольца в которое ткнули мышкой 

        if (ring>=0 && ring == asix->countRings - 1) { //если это самое верхнее кольцо на оси 
            Ring* r = &asix->rings[ring];
            game.lastX = x;        // запоминаем координаты в момент нажатия мыши
            game.lastY = y;       // в onMouseMove будем узнавать дельту в момент перемещения  
            game.isMouseMove = 1; // оказалось что ткнули в верхнее кольцо и его можно двигать 
            game.ringForMove = r; //какое кольцо двигаем 
            game.asixForMove = asix;// с какого стержня начали двигать 
        }
    }
}

// отжали левую кнопку
//функция получает на входе координаты  
void game_onLeftBtnUp(int x, int y) {
    if (game.typeScena != ts_game) {
        return;
    }                                                // если сцена не игровая и стадия игры соответственно дальше не проходим   
    if (game.stage != gs_exec) {
        return;
    }

    if (game.isMouseMove == 1) { //мышь пока нажата  
        Asix* asix = getAsixByCoord(x, y);//проверяем область в какой находимся 
        if (asix != NULL) {
            if (move(game.asixForMove, asix) == 1) { // кольцо перенесено 

            }
        }
    } else {    
       return;  //не реагируем на отжатие кнопки мыши 
    }
    
    game.isMouseMove = 0; // мышь отжали 
    game.ringForMove = NULL;
    game.asixForMove = NULL;
    showGame();
}

// двигаем мышь 
void game_onMouseMove(int x, int y) {
    if (game.typeScena != ts_game) {
        return;
    }
    if (game.stage != gs_exec) {
        return;
    }

    if (game.isMouseMove == 1) {                //нажали мышь 
        int dx = x - game.lastX;;
        int dy = y - game.lastY;        //узнаем дельту от предыдущего перемещения 
        
        Asix* asix = game.asixForMove;
        Ring* r = game.ringForMove;
        putimage(r->x, r->y, asix->image, putimage_ops::COPY_PUT);
        r->x += dx ;        // меняем координаты кольца с учетом перемещения мыши
        r->x2 += dx ;
        r->y += dy;
        r->y2 += dy;
        getimage(r->x, r->y, r->x2, r->y2, asix->image);//перед отрисовкой кольца на новом месте, запомним изображения под ним 
        showRing(r);

        game.lastX = x; // теперь запоминаем новые координаты мыши как опорные для следующего события перемещения 
        game.lastY = y; 
    }
}

// идет отрисовка всей сцены 
void showGame() {
    cleardevice();//отчистка сцены 

    if (game.typeScena != ts_game) {
        return;
    }

    ctrl_buttons[0].isActive = game.lastAsix>=0 ; //кнопка скрывается если нет оси с которой сняли кольцо 
    
    setbkcolor(WHITE);
    showAsixes();// показываем стержни 
    for (int i = 0; i < game.countAxis; i++) {
        showRings(game.asix[i]);//кольца на каждом стержне 
    }
    showCounter();//счетчик
    //showTimer();//таймер 
    showStage();
    showCtrlButtons();
}

//функция заполняет кнопки в самой игре 
void initCtrlButtons() {
    
    ctrl_counts = 2;    // длинна массива 
   //массив структуры кнопка всего их 3 на экране 
    ctrl_buttons[0] = { tb_back,"Назад", 0, 0, 0 };   //массив структуры кнопки // 1 -тип кнопки, 2 - название,3 - данные4- цвет 5- активность кнопки
    //ctrl_buttons[1] = { tb_pause,"Пауза" , 0, 0, 1}; //массив структуры кнопки // 1 -тип кнопки, 2 - название,3 - данные4- цвет 5- активность кнопки  
    ctrl_buttons[1] = { tb_cancel,"Завершить" , 0, 0, 1};
   
    int w = 100; 
    int h = 30;
    int x0 = 450;
    int y0 = 40;

    for (int i = 0; i < ctrl_counts; i++) {
        Button* b = &ctrl_buttons[i];
        b->x = x0 + i * w + i * 20; // кнопки будут выставлены на расстоянии 20 друг от друга 
        b->x2 = b->x + w;
        b->y = y0;
        b->y2 = b->y + h;
    }
}


int startGame() {
    if (initGame() == 0) {  // по условию можем начать игру  
        return -1;
    }
    calcCoordAsix();    // расчет координат стержней 
    initCtrlButtons();          // расчет координат кнопок на экране самой игры 
    bar(0, 0, game.maxX, game.maxY);
    //выставляем события на мышь 
    registermousehandler(WM_LBUTTONDOWN, game_onLeftBtnClick);   //отработка игрового сценария
    registermousehandler(WM_LBUTTONUP, game_onLeftBtnUp);
    registermousehandler(WM_MOUSEMOVE, game_onMouseMove);

    showGame();

    //int k = 0;
    while (1) 
    {
        // идет работа с таймером 
        delay(100);
        if (game.typeScena != ts_game) {
            break;      // покидаем сцену игры
        }
    }
    return 1;
}
