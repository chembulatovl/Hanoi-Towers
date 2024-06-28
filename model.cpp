#include "model.h"

// функция окончания игры 
int testGameOver(){
	for (int i = 1; i < game.countAxis; i++) {
		if (game.asix[i].countRings == game.countRings) {  //башня собрана на i стержне 
			game.stage = gs_finish;
			return 1;
		}
	}
	return 0; // башня не собрана 
}

// функция проверки возможности перемещения конусов с одной оси на другую ось 
// проверяет возможность переноса верхнего кольца на  верхнее кольцо другой оси 
// 0 - переносить нельзя 
// 1 - перенос возможен 
int testMove(Asix* asix1, Asix* asix2) {

	if (asix1->number == asix2->number) {
		return 0;
	}
	if (asix1->countRings == 0) {  // стержень откуда хотим передвинуть - пуст
		return 0;	   // нечего переносить 
	}

	if (asix2->countRings == 0) { // стержень куда хотим передвинуть - пуст
		return 1;	// проверять ничего не надо  
	}
	Ring* r1 = &asix1->rings[asix1->countRings - 1];
	Ring* r2 = &asix2->rings[asix2->countRings - 1];

	if (r1->size < r2->size){
		return 1; // переносимое кольцо меньше кольца на второй оси 
	}
	return 0;
}

//введена функция возврата хода после очередного шага 
int backStep(){
      if(game.lastAsix<0){ // проверка на то что со стержня не сняли конус
         return -1;
      }
      Asix* asix2 = &game.asix[game.lastAsix]; // указатель на структуру стержня // является стержень с которого сняли кольцо
      Asix* asix1 = &game.asix[game.currentAsix]; // указатель на структуру стержня  // является адресом на структуру game, куда положили конус 
      game.counts -=2; // счетчик уменьшается 
      move(asix1,asix2);//вызывается функция переноса колец 
      game.lastAsix = -1 ; //реализован лишь 1 откат назад поэтому заново выставляем признак того что кольцо не перенесли 
      return 1 ;      
}

// перенос верхнего кольца с 1 оси на 2 
// перед переносом проверка на правила игры 
// 0 - перемещение не было сделано
// 1 - перемещение осуществлено 
int move(Asix* asix1, Asix* asix2) {

	if (testMove(asix1, asix2) == 0) { // проверка на возможность перемещения по правилам игры  
		return 0;		// перемещение делать нельзя по правилам игры
	}

	Ring* r1 = &asix1->rings[asix1->countRings - 1];
	Ring* r2 = &asix2->rings[asix2->countRings];

	r2->size = r1->size ;	// перенос размера кольца из 1 оси на 2  
	r1->size = 0;			// на 1 оси помещаем 0 на старую позицию кольца - отсутствие кольца
	asix1->countRings--;
	asix2->countRings++;
	game.counts++;
        game.lastAsix = asix1->number;
        game.currentAsix = asix2->number;
	testGameOver();
	return 1;
}

//  инициализируем игру задавая ей параметры 
int initGame() {
	game.counts = 0;				//обнуляем кол-во ходов  
	//game.timer = 0;					//обнуление таймера
	game.stage = gs_exec;                         //стадия игры игровая 
	
	Ring* rings = game.asix[0].rings;		// массив колец первого стержня
	for (int i = 0; i < game.countRings; i++) {
		rings[i].size = game.countRings-i ;	// изначально кольца все лежат на первой оси
	}
	for(int i=0;i < COUNT_ASIX;i++){
		game.asix[i].number = i; // порядковый номер для каждого стержня 
		game.asix[i].countRings = 0;
		game.asix[i].color = RGB(100 + i * 20, 20 * i * 20, 100 + i * 30);   //задаем цвет каждого стержня 
	}

	game.asix[0].countRings = game.countRings ;// 1 стержень заполняется кольцами 
	return 1;
}