#include "model.h"

// ������� ��������� ���� 
int testGameOver(){
	for (int i = 1; i < game.countAxis; i++) {
		if (game.asix[i].countRings == game.countRings) {  //����� ������� �� i ������� 
			game.stage = gs_finish;
			return 1;
		}
	}
	return 0; // ����� �� ������� 
}

// ������� �������� ����������� ����������� ������� � ����� ��� �� ������ ��� 
// ��������� ����������� �������� �������� ������ ��  ������� ������ ������ ��� 
// 0 - ���������� ������ 
// 1 - ������� �������� 
int testMove(Asix* asix1, Asix* asix2) {

	if (asix1->number == asix2->number) {
		return 0;
	}
	if (asix1->countRings == 0) {  // �������� ������ ����� ����������� - ����
		return 0;	   // ������ ���������� 
	}

	if (asix2->countRings == 0) { // �������� ���� ����� ����������� - ����
		return 1;	// ��������� ������ �� ����  
	}
	Ring* r1 = &asix1->rings[asix1->countRings - 1];
	Ring* r2 = &asix2->rings[asix2->countRings - 1];

	if (r1->size < r2->size){
		return 1; // ����������� ������ ������ ������ �� ������ ��� 
	}
	return 0;
}

//������� ������� �������� ���� ����� ���������� ���� 
int backStep(){
      if(game.lastAsix<0){ // �������� �� �� ��� �� ������� �� ����� �����
         return -1;
      }
      Asix* asix2 = &game.asix[game.lastAsix]; // ��������� �� ��������� ������� // �������� �������� � �������� ����� ������
      Asix* asix1 = &game.asix[game.currentAsix]; // ��������� �� ��������� �������  // �������� ������� �� ��������� game, ���� �������� ����� 
      game.counts -=2; // ������� ����������� 
      move(asix1,asix2);//���������� ������� �������� ����� 
      game.lastAsix = -1 ; //���������� ���� 1 ����� ����� ������� ������ ���������� ������� ���� ��� ������ �� ��������� 
      return 1 ;      
}

// ������� �������� ������ � 1 ��� �� 2 
// ����� ��������� �������� �� ������� ���� 
// 0 - ����������� �� ���� �������
// 1 - ����������� ������������ 
int move(Asix* asix1, Asix* asix2) {

	if (testMove(asix1, asix2) == 0) { // �������� �� ����������� ����������� �� �������� ����  
		return 0;		// ����������� ������ ������ �� �������� ����
	}

	Ring* r1 = &asix1->rings[asix1->countRings - 1];
	Ring* r2 = &asix2->rings[asix2->countRings];

	r2->size = r1->size ;	// ������� ������� ������ �� 1 ��� �� 2  
	r1->size = 0;			// �� 1 ��� �������� 0 �� ������ ������� ������ - ���������� ������
	asix1->countRings--;
	asix2->countRings++;
	game.counts++;
        game.lastAsix = asix1->number;
        game.currentAsix = asix2->number;
	testGameOver();
	return 1;
}

//  �������������� ���� ������� �� ��������� 
int initGame() {
	game.counts = 0;				//�������� ���-�� �����  
	//game.timer = 0;					//��������� �������
	game.stage = gs_exec;                         //������ ���� ������� 
	
	Ring* rings = game.asix[0].rings;		// ������ ����� ������� �������
	for (int i = 0; i < game.countRings; i++) {
		rings[i].size = game.countRings-i ;	// ���������� ������ ��� ����� �� ������ ���
	}
	for(int i=0;i < COUNT_ASIX;i++){
		game.asix[i].number = i; // ���������� ����� ��� ������� ������� 
		game.asix[i].countRings = 0;
		game.asix[i].color = RGB(100 + i * 20, 20 * i * 20, 100 + i * 30);   //������ ���� ������� ������� 
	}

	game.asix[0].countRings = game.countRings ;// 1 �������� ����������� �������� 
	return 1;
}