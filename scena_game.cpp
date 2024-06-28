#include "model.h"

//������ �� ������ ��� ������� ������
long colors[10] = { RGB(255,69,0), RGB(106,90,205) , RGB(255,0,0) , RGB(60,179,113), RGB(255,160,122) , RGB(255,20,147) , RGB(255,215,0) , RGB(55,215,1000), RGB(255,165,0), RGB(155,165,12) };

Button ctrl_buttons[10];        // ����������� ������ �� �����
int ctrl_counts = 0;

// ���������� ��������� ����� ���������� ������
void calcCoordRings(Asix& asix) {
    int maxW = 0.9 * (asix.areaX2 - asix.areaX) ; 
    for (int i = 0; i < asix.countRings; i++) {    // ���� �� ���������� ����� 
        Ring* r = &asix.rings[i];
        int w = maxW * r->size / game.countRings; // ������ ����� �������� �� 8 ������ ��� ������ ������ 
        r->x = asix.areaX + ((asix.areaX2 - asix.areaX) - w) / 2;// ������������ ����� ������� ����� �������� ������
        r->x2 = r->x + w;
        r->y2 = asix.areaY2 - asix.heightRing * i ;//������ ������������ �� ���-�� ����� �� ������� 
        r->y = r->y2 - asix.heightRing;
        r->color = colors[r->size-1];//����� ������ ������ ���� 
    }
}
//������� �������� �� ����� ��������� �� ��������� ������
//��������� ��������� 
void showRing(Ring* r)
{
    long lastColor = getbkcolor() ;
    setfillstyle(SOLID_FILL, r->color);
    
    switch (game.typeRing) {
    case tr_bar:
        bar(r->x, r->y, r->x2, r->y2);  //������������ ������ ���� ������������� ���� ������ ��� ��������������� 
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

//�������� ����� �� ���
// ������� �������� �� ����� ��������
void showRings(Asix& asix) {
    calcCoordRings(asix);//���������� ������� � ����������� ��������� ������

    if (asix.image != NULL) { // ���� ����� ��� ������� ������� ������ ��� ������� ������ 
        free(asix.image); //�� �� �� �����������, �.� ����� ������� ������ ����� ���� ������� �������  
        asix.image = NULL;
    }

    for (int i = 0; i < asix.countRings; i++) {
        Ring* r = &asix.rings[i];
        if (i == asix.countRings - 1) {
            int memsize = imagesize(r->x, r->y, r->x2, r->y2); //���-�� ���� ������� ����������� ��� �������� ������� ��� ������� 
            asix.image = malloc(memsize); // �������� ������ 
            getimage(r->x, r->y, r->x2, r->y2, asix.image);
        }
        showRing(r);
    }
}

// ���������� ������ � ������� ������� 
//�� ����� �������� ������ �� ��������� �������, ������ � ������ ������
void __initAsix(Asix& asix, int w, int h) { // ������� �������� �� ����� ���������� � ��������
    asix.areaX = asix.number * w + w / 2; //����������� ���������� ������� �������
    asix.areaX2 = asix.areaX + w;
    asix.x = asix.number * w + w - 5; //���������� ������ �������
    asix.x2 = asix.x + 10;

    asix.y = h/2 ; //������ ������� - �������� ������
    asix.y2 = h-10 ; // ������ �� ������ �������

    asix.areaY = asix.y - h / (game.countAxis +1);//������ �� ������ ������ 
    asix.areaY2 = asix.y2;
 }

//���������� �������� ��������� �������� 
void calcCoordAsix() {
    int w = game.maxX / (game.countAxis+1) ;//���-�� �������� �� 1 ������ ��� ���� 
    int h = game.maxY ;//������ 

    for (int i = 0; i < game.countAxis; i++) {
        __initAsix(game.asix[i], w, h); //���������� ������� � ���������
    }
}

// ��������� ������� ������ �������� 
void showAsixes() {
    //�� ���� ���� �������������� ��� �������� � ������� ������� 
    for (int i = 0; i < game.countAxis; i++) {

        setfillstyle(CLOSE_DOT_FILL, game.asix[i].color); //������������� ���� � ����� ������� ��� ������� �������  
        bar(game.asix[i].areaX, game.asix[i].areaY, game.asix[i].areaX2, game.asix[i].areaY2);//������ ������� 

        setfillstyle(SOLID_FILL, game.asix[i].color); //������������� ���� � ����� ������� ��� �������
        bar(game.asix[i].x, game.asix[i].y, game.asix[i].x2, game.asix[i].y2);// ������ ��������
    }
}
// ������� �������� ���������� �� ����� 
// ���������� ��������� �� ��������� ������� 
Asix* getAsixByCoord(int x, int y) {
    for (int i = 0; i < game.countAxis; i++) {
        Asix* asix = &game.asix[i];
        if (asix->areaX <= x && asix->areaX2 >= x && asix->areaY <= y && asix->areaY2 >= y) {
            return asix;  //���������� �������� 
        }
    }
    return NULL ; // ���� �� �� �������� ������������ � ������� ������� ������� NULL
}

// ������� ����������� ����� ������ �� ������� 
// �������� ��������� �� ��������� ������� , ���������� 
int getRingByCoord(Asix* asix, int x, int y) {
    for (int i = 0; i < asix->countRings; i++) {
        Ring* r = &asix->rings[i];
        if (r->x <= x && r->x2 >= x && r->y <= y && r->y2 >= y) {
            return i ; //����� ������ �� �������
        }
    }
    return -1 ; // ����� ��� �� ������� 
}
//����� �� ����� �������� 
void showCounter() {
    char str[25];
    long lastColor = getbkcolor() ;    
    sprintf_s(str, "���������� �����: %d", game.counts);
    setbkcolor(WHITE);
    setcolor(BLACK);
    outtextxy(100,50,str);
    setbkcolor(lastColor);
}

//������� ��� �������� ������ ����� � ���� �������� ����� � ���� 
void showStage() {
    char str[25];
    switch (game.stage) {
    case gs_exec:// ���� ���� ����
        sprintf(str, "���� �����������...");
        setcolor(RGB(0, 255, 0));
        break;
    case gs_finish:// ���� ���� ��������� 
        sprintf(str, "���� ��������"); 
        setcolor(RGB(255, 0, 0));
        break;
    case gs_pause: // ���� ����� 
        sprintf(str, "�����");
        setcolor(RGB(0, 0,255));
        break;
    }
    outtextxy(100, 100, str);
}

// ������� �������� ����� �� ����� ������ �� ����� ���� 
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

// ������� �������� �� ����� ���������� ����� ���� 
// ���������� ��������� �� �������� ������ 
Button* getCtrlButtonByCoord(int x, int y) {
    for (int i = 0; i < ctrl_counts; i++) {
        Button* b = &ctrl_buttons[i];
        if (b->x <= x && b->x2 >= x && b->y <= y && b->y2 >= y) { // �������� �� ��������� � ���������� 
            return b; // ������� ������ 
        }
    }
    return NULL;
}

// ������ ���� � ����� ���� 
void game_onLeftBtnClick(int x, int y) {
    if (game.typeScena != ts_game) {            // ����������� ������ ����� ����� ����
        return;
    }

    Button* b = getCtrlButtonByCoord(x, y); // ��������� ������� ��������� � ������ ����� ���� 
    if (b != NULL) { // ������ � ����� 
        switch (b->type) {                         // �������� ���� ������ 
           
        case tb_back:                                 //������� ���� 
           if( game.stage == gs_exec){          //�������� ���� ��� ������ ���� ��������� �� � ����� 
               backStep();                              //�������� �������
               showGame();                              // ������������ �����
           }
           break;
        case tb_cancel:                                 // ���� ��� ������ ���������� �� ������ 
            game.typeScena = ts_first;              // �������� ����� ���� 
            break;
        }
        return ;
    }

    if (game.stage != gs_exec) {                    
        return;
    }

    game.isMouseMove = 0;  //���� ������ 
    Asix* asix = getAsixByCoord(x, y); 
    if (asix != NULL) {
        //����� ������ � ������� ���

        int ring = getRingByCoord(asix, x, y);//������ ����� ������ � ������� ������ ������ 

        if (ring>=0 && ring == asix->countRings - 1) { //���� ��� ����� ������� ������ �� ��� 
            Ring* r = &asix->rings[ring];
            game.lastX = x;        // ���������� ���������� � ������ ������� ����
            game.lastY = y;       // � onMouseMove ����� �������� ������ � ������ �����������  
            game.isMouseMove = 1; // ��������� ��� ������ � ������� ������ � ��� ����� ������� 
            game.ringForMove = r; //����� ������ ������� 
            game.asixForMove = asix;// � ������ ������� ������ ������� 
        }
    }
}

// ������ ����� ������
//������� �������� �� ����� ����������  
void game_onLeftBtnUp(int x, int y) {
    if (game.typeScena != ts_game) {
        return;
    }                                                // ���� ����� �� ������� � ������ ���� �������������� ������ �� ��������   
    if (game.stage != gs_exec) {
        return;
    }

    if (game.isMouseMove == 1) { //���� ���� ������  
        Asix* asix = getAsixByCoord(x, y);//��������� ������� � ����� ��������� 
        if (asix != NULL) {
            if (move(game.asixForMove, asix) == 1) { // ������ ���������� 

            }
        }
    } else {    
       return;  //�� ��������� �� ������� ������ ���� 
    }
    
    game.isMouseMove = 0; // ���� ������ 
    game.ringForMove = NULL;
    game.asixForMove = NULL;
    showGame();
}

// ������� ���� 
void game_onMouseMove(int x, int y) {
    if (game.typeScena != ts_game) {
        return;
    }
    if (game.stage != gs_exec) {
        return;
    }

    if (game.isMouseMove == 1) {                //������ ���� 
        int dx = x - game.lastX;;
        int dy = y - game.lastY;        //������ ������ �� ����������� ����������� 
        
        Asix* asix = game.asixForMove;
        Ring* r = game.ringForMove;
        putimage(r->x, r->y, asix->image, putimage_ops::COPY_PUT);
        r->x += dx ;        // ������ ���������� ������ � ������ ����������� ����
        r->x2 += dx ;
        r->y += dy;
        r->y2 += dy;
        getimage(r->x, r->y, r->x2, r->y2, asix->image);//����� ���������� ������ �� ����� �����, �������� ����������� ��� ��� 
        showRing(r);

        game.lastX = x; // ������ ���������� ����� ���������� ���� ��� ������� ��� ���������� ������� ����������� 
        game.lastY = y; 
    }
}

// ���� ��������� ���� ����� 
void showGame() {
    cleardevice();//�������� ����� 

    if (game.typeScena != ts_game) {
        return;
    }

    ctrl_buttons[0].isActive = game.lastAsix>=0 ; //������ ���������� ���� ��� ��� � ������� ����� ������ 
    
    setbkcolor(WHITE);
    showAsixes();// ���������� ������� 
    for (int i = 0; i < game.countAxis; i++) {
        showRings(game.asix[i]);//������ �� ������ ������� 
    }
    showCounter();//�������
    //showTimer();//������ 
    showStage();
    showCtrlButtons();
}

//������� ��������� ������ � ����� ���� 
void initCtrlButtons() {
    
    ctrl_counts = 2;    // ������ ������� 
   //������ ��������� ������ ����� �� 3 �� ������ 
    ctrl_buttons[0] = { tb_back,"�����", 0, 0, 0 };   //������ ��������� ������ // 1 -��� ������, 2 - ��������,3 - ������4- ���� 5- ���������� ������
    //ctrl_buttons[1] = { tb_pause,"�����" , 0, 0, 1}; //������ ��������� ������ // 1 -��� ������, 2 - ��������,3 - ������4- ���� 5- ���������� ������  
    ctrl_buttons[1] = { tb_cancel,"���������" , 0, 0, 1};
   
    int w = 100; 
    int h = 30;
    int x0 = 450;
    int y0 = 40;

    for (int i = 0; i < ctrl_counts; i++) {
        Button* b = &ctrl_buttons[i];
        b->x = x0 + i * w + i * 20; // ������ ����� ���������� �� ���������� 20 ���� �� ����� 
        b->x2 = b->x + w;
        b->y = y0;
        b->y2 = b->y + h;
    }
}


int startGame() {
    if (initGame() == 0) {  // �� ������� ����� ������ ����  
        return -1;
    }
    calcCoordAsix();    // ������ ��������� �������� 
    initCtrlButtons();          // ������ ��������� ������ �� ������ ����� ���� 
    bar(0, 0, game.maxX, game.maxY);
    //���������� ������� �� ���� 
    registermousehandler(WM_LBUTTONDOWN, game_onLeftBtnClick);   //��������� �������� ��������
    registermousehandler(WM_LBUTTONUP, game_onLeftBtnUp);
    registermousehandler(WM_MOUSEMOVE, game_onMouseMove);

    showGame();

    //int k = 0;
    while (1) 
    {
        // ���� ������ � �������� 
        delay(100);
        if (game.typeScena != ts_game) {
            break;      // �������� ����� ����
        }
    }
    return 1;
}
