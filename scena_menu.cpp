#include "model.h"

Section settings[10] ; // ����������� ������ �� ����� //������ ��������� ������
int countSections=0;
char str[9][3] ;        // ��� ������������� ������ ������ ���������� �����

//��������� ������ 
//������� �������� �� ����� �������� �� ��������� ������ � ��������� �� ��������� ������    
void showButton(Section* s, Button* r)
{
   long lastColor = getbkcolor() ;
    if (r->isActive) {                                          // ���� ������ �������
        setfillstyle(SOLID_FILL, RGB(0, 255, 0));        // ��� �������������� ������� ������
        setbkcolor(RGB(0, 255, 0));
    }
    else {
        setfillstyle(SOLID_FILL, RGB(128, 128, 128));
        setbkcolor(RGB(128, 128, 128));
    }

    bar(s->x + r->x, s->y + r->y, s->x + r->x2, s->y + r->y2);  //��������� ����� ������ 

    if (r->str != NULL) {
        setcolor(RGB(0, 0, 0));
        outtextxy(s->x + r->x + 20, s->y + r->y + (r->y2 - r->y)/2, (char*)r->str);
    }
    setbkcolor(lastColor);
}


//������� ��������� ������ ����������� � ������ � ����������� �� �� ����
int verifiAllData() {
    for (int i = 0; i < countSections; i++) {
            
        if (settings[i].type == st_select) { // ������� ���� ��� ������ ����������(������ ������) 
            if (settings[i].data==0) { // � ������ ��� � ������ �� ������ ������ 
                return 0;
            }
        }
    }
    return 1;
}

//������� ������������ ���������� � ����������� �� ������ ������ 
//�� ����� �������� ��������� �� ������
void showComentSection(Section* s) {
    int data;
    if (s->type == st_buttons) { 
        return;
    }
    if (s->type == st_itog) { // �������� ����� ������ �������� ������� ���� ��� ��� ���������� ������ ���� ���������� 
        data = verifiAllData(); // ��� ����� ���������� � ������� ������� ����������� 
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

// ��������� ������ 
//�� ����� �������� ��������� �� ��������� ������
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

// ������������� ������ 
// �� ���� ������� �������� ��������� �� ��������� ������ 
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

// ������� ��������� ���������� ������
//�� ����� �������� ��������� �� ��������� ������ � ���������� 
//���������� ��������� �� ��������� ������ 
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
    return NULL; // ���� �� ������ �� ������
}

//�������� �� ��������� � ������ 
// �������� �� ����� ���������� 
//���������� ��������� �� ��������� ����� 
Section* getSectionByCoord(int x, int y) 
{
    for (int i = 0; i < countSections; i++)
    {
        Section* s = &settings[i];
        if (s->x <= x && s->x2 >= x && s->y <= y && s->y + s->height >= y) //�������� �� �����������
        {
            return s;
        }
    }

    return NULL;
}

// ������� ��������� �������� �� ������ ���� � ����� ��������
void sets_onLeftBtnClick(int x, int y) {
    if (game.typeScena != ts_settings && game.typeScena != ts_first) { //������� ����������� ������ �� ����� �������� � ���� 
        return;
    }
    Section* s = getSectionByCoord(x, y); // ������ ������ �� �� � ������� �����  �����
    if (s != NULL) { // ������ � ������
        Button* b = getButtonByCoord(s, x, y); //�������� ������� �� �������� ������� �� ������ 
        if (b != NULL) { // ���� ������ �� ������ 
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
            case tb_cancel: // ������ �� ����� 
                game.typeScena = ts_first; // ����� �������� ���� 
                break;

            case tb_about: // ������ � ���� 
               game.typeScena = ts_about; // ����� �������� � ���� 
                break;

            case tb_exit: // ��� ������ ���� ����� 
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
                    // ���������� ������ ���������� ������� �� ������� ������
                    b2 = &s->buttons[i] ;
                    if (b2->isActive == 1) {
                        b2->isActive = 0;                      // ���������� ������� ���������� ������
                        lastData = b2->data;                 // ����� ������ ������� �� ������������ ��� ������
                        s->data = 0 ;                           // ������� ����, ��� � ������ ������ �� �������
                        showButton(s, b2);                    // ������������ ������ (������ ������� �����)
                        break;
              
                    }
                 }
               // ���������, ��� �������� �� ����� ���������� ������
                if (b->data != lastData) {      
                    b->isActive = 1;                            // ������ ��� ������ ��������
                    s->data = b->data;                       // ������ ���� ������ �������� � ������
                    showButton(s, b);                          // ������������ ������ (������ �������)
                }
                showComentSection(s);                      // ������� ������� ����������� ������ � ������ (������� �� ��� ������� !!!)
                showComentSection(&settings[countSections - 1]);        // ������� ����� � ����� ���� (����������� ������ ����)
            }
        }
    }
}

// ��� ����� ��������� ���������� � ������� �������� ��� ������������� ������ � ��������������� ������ 
void initSettings()
{
   //������� ���-�� ������ 
    countSections = 4;
    settings[0] = { st_select,"�������� ���������� ����", 0, RGB(0,255,0),   3, 0}; //...,2-������, 3-���������� �����, 4-����, 5 - ������� ������ ��������,������  
    settings[0].buttons[0] = { tb_selectAsix, "3", 3 } ; // ��� ��������������� ������ 1 ������ ������ ���������� 1-��������, 2-������, 3 - �� ��������(data)
    settings[0].buttons[1] = { tb_selectAsix, "4", 4 };
    settings[0].buttons[2] = { tb_selectAsix, "5", 5 };
   // ��� ������ ������ � ������ �������� ��������������� �������� 
    settings[1] = { st_select,"�������� ���������� �����", 1, RGB(0,128,128),  9, 0};
   
   
   for(int i=0 ; i<=8 ; i++) { 
     sprintf(str[i], "%d", i+2);
     settings[1].buttons[i] = { tb_selectRings, str[i], i+2 };
   }
   
    settings[2] = { st_select,"�������� ��� ������", 2, RGB(128,128,0),   2, 0 };
    settings[2].buttons[0] = { tb_selectTypeRings, "������", tr_ellipse };
    settings[2].buttons[1] = { tb_selectTypeRings, "�������������", tr_bar };

    settings[3] = { st_itog,"", 3, RGB(255,255,255),  2};
    settings[3].buttons[0] = { tb_start, "����� !"};
    settings[3].buttons[1] = { tb_cancel, "�����" };

    for (int i = 0; i < countSections; i++) {
        initSection(&settings[i]);
    }
    registermousehandler(WM_LBUTTONDOWN, sets_onLeftBtnClick); // ����������� ������� ����
}

//������� �������������� ��� ����� ���� ������ ����� ������ 
void initFirstMenu()
{
   // 3 ������- 3 ������
    countSections = 3 ;
   
   //��� ������ ������ 
    settings[0] = { st_buttons,"", 0, RGB(255,255,255),   1, 0, 250 }; // ��� ������,���(������),���������� �����, ����, ���-�� ������, ������, ������, ������ ������ 
    settings[0].buttons[0] = { tb_settings, "������ ����",0, RGB(0,0,0)}; // ��� ������, ������, ������ � ���� 

    settings[1] = { st_buttons,"", 1, RGB(255,255,255),   1, 0, 250 };
    settings[1].buttons[0] = { tb_about, "������"};

    settings[2] = { st_buttons,"", 2, RGB(255,255,255),   1, 0, 250 };
    settings[2].buttons[0] = { tb_exit, "�����"};

    for (int i = 0; i < countSections; i++) {
        initSection(&settings[i]); 
    }
    registermousehandler(WM_LBUTTONDOWN, sets_onLeftBtnClick); // ����������� ������� 
}


// ������� ���������� ���� ���� 
int showFirstMenu()
{
    if (game.typeScena != ts_first) { 
        return 1; //������� 1 ���� ����� ����� ����� ���� 
    }

    initFirstMenu(); // ��������������� ����
    cleardevice();
    for (int i = 0; i < countSections; i++) {
        showSection(&settings[i]);
    }

    while (1) {
        delay(100); //��������� �� ������� ��� �� �� ������� �� ������ ����� 
        if (game.typeScena != ts_first) {
            break;
        }
    }
    return 1;
}

//������� ���������� ��������� ���� 
int showSettings()
{
    if (game.typeScena != ts_settings) { // �������� �� �� ��� �� ��������� �� ����� ��������� 
        return 1;
    }

    initSettings(); // ������������� �������� 
    cleardevice();
    for (int i = 0; i < countSections; i++) {
        showSection(&settings[i]);
    }

    while (1) {
        delay(100);
        if (game.typeScena != ts_settings) {
            break;   // ��������� �� ������ �����
        }
    }
    return 1;
}
