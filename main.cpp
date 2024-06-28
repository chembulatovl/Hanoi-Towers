#include "model.h"
Game game;

int createMainWindow() 
{
    initwindow(800, 800, "Ханойские башни");
    setbkcolor(WHITE);
    setfillstyle(SOLID_FILL, RGB(255, 250, 250));
    game.maxX = getmaxx();      
    game.maxY = getmaxy();
    return 0;
}

// функция выполняет работу в окне о игре
void showAbout() 
{
   cleardevice() ;
   IMAGE* image = loadBMP("window_about.bmp") ; // загрузка картинки 
   if(image!=NULL)
   {
      putimage(0, 0, image, COPY_PUT);
      freeimage(image);
   }
   outtextxy(300,700, "Нажмите любую кнопку") ; // выводится текст 
   getch() ;
   game.typeScena = ts_first;   // смена сцены на меню
}

// функция выполняет вызов определенной сцены
int showScena()
{
    switch (game.typeScena) {
    case ts_first:                      // если сцена меню 
        showFirstMenu();             // вызывается функция которая отрисовывает сцену меню
        break;
 
    case ts_settings:                  //аналогично
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
        showScena();            // отрисуем текущую сцену
        if (game.typeScena == ts_finish) { // если тип сцены финиш заканчиваем игру 
            break;
        }
    }
    closegraph();

    return 0;
}
