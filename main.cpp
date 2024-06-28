#include "model.h"
Game game;

int createMainWindow() 
{
    initwindow(800, 800, "��������� �����");
    setbkcolor(WHITE);
    setfillstyle(SOLID_FILL, RGB(255, 250, 250));
    game.maxX = getmaxx();      
    game.maxY = getmaxy();
    return 0;
}

// ������� ��������� ������ � ���� � ����
void showAbout() 
{
   cleardevice() ;
   IMAGE* image = loadBMP("window_about.bmp") ; // �������� �������� 
   if(image!=NULL)
   {
      putimage(0, 0, image, COPY_PUT);
      freeimage(image);
   }
   outtextxy(300,700, "������� ����� ������") ; // ��������� ����� 
   getch() ;
   game.typeScena = ts_first;   // ����� ����� �� ����
}

// ������� ��������� ����� ������������ �����
int showScena()
{
    switch (game.typeScena) {
    case ts_first:                      // ���� ����� ���� 
        showFirstMenu();             // ���������� ������� ������� ������������ ����� ����
        break;
 
    case ts_settings:                  //����������
        showSettings();
        break;

    case ts_game:
        startGame();
        break;

    case ts_about:
        showAbout();
        break;
    }
    return 1;
}


int main()
{
   
   createMainWindow();
   while (true) {
        showScena();            // �������� ������� �����
        if (game.typeScena == ts_finish) { // ���� ��� ����� ����� ����������� ���� 
            break;
        }
    }
    closegraph();

    return 0;
}
