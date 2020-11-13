
#include "pch.h"
#include <iostream>
#include <time.h> 
#include <cstdlib>
#include <Windows.h>

using namespace std;

//Ucagin yonunu belirten enum yapisi
enum UCAKYON {
	YON_ASAGI = 1,
	YON_YUKARI = 2
};

//Hedefin ve merminin yonunu belirten enum yapisi
enum SYON {
	YON_SOL = 1,
	YON_SAG = 2
};
//Ucak yapisi
struct Ucak {
	int x;
	int y;
	UCAKYON yon;
	char karakter;
};
//Mermi yapisi
struct Mermi {
	int x1;
	int y1;

	SYON Syon;
	char karakter;
};
//Hedef yapisi
struct Hedef {
	int x2;
	int y2;

	SYON syon;
	char karakter;
};
//Kullanilacak degiskenler
const int genislik = 80;
const int yukseklik = 20;
char sahne[genislik][yukseklik];
char tuslar[256];
int ucakboyutu = 6;
int mermiboyutu = 100;
int hedefboyutu = 10;
int SpacebasmaSayisi = 0;
bool Canli = true;
int Altbasmasayisi = 0;
int UstBasmaSayisi = 0;
int score = 0;
Ucak UcakGovde[6];
Mermi MermiGovde[500];
Hedef HedefGovde[50];

//Konsolda cerceve olusturan fonksiyon
void cercevexy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void KursoruGizle() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}
//Oyun sahnesini olusturan fonksiyon
void sahneyiCiz() {
	for (int i = 0; i < yukseklik; i++) {
		for (int j = 0; j < genislik; j++) {
			cout << sahne[j][i];
		}
		cout << endl;
	}
}
//Sahneyi bosaltarak silen fonksiyon
void sahneyiTemizle() {
	for (int i = 0; i < yukseklik; i++) {
		for (int j = 0; j < genislik; j++) {
			sahne[j][i] = ' ';
		}
	}
}
//Oyun Sinirlarini olusturan fonksiyon
void sinirlariOlustur() {
	for (int x = 0; x < genislik; x++) {
		sahne[x][0] = 27;//'▄'
		sahne[x][yukseklik - 1] = 27;//'▄'
	}
	for (int y = 0; y < yukseklik; y++) {
		sahne[0][y] = 27;//'▄'
		sahne[genislik - 1][y] = 27;//'▄'
	}
}
//Tuslari okuyarak hareket ettirmemizi saglayan ana fonksiyon
void klavyeoku(char tuslar[]) {
	for (int i = 0; i < 256; i++) {
		tuslar[i] = (char)(GetAsyncKeyState(i) >> 8);
	}
}

//Ucak Govdesinin baslangic koordinatlarini olusturan fonksiyon
void UcakOlustur() {
	UcakGovde[0].x = 1;
	UcakGovde[0].y = 1;
	UcakGovde[1].x = 2;
	UcakGovde[1].y = 2;
	UcakGovde[2].x = 3;
	UcakGovde[2].y = 3;
	UcakGovde[3].x = 2;
	UcakGovde[3].y = 4;
	UcakGovde[4].x = 1;
	UcakGovde[4].y = 5;
}
//Mermi govdemizi ve baslangiçta nereden olusacagini gosteren fonksiyon
void MermiOlustur() {
	for (int i = 0; i < mermiboyutu; i++) {
		i += SpacebasmaSayisi;
		MermiGovde[i].x1 = UcakGovde[2].x + 1;
		MermiGovde[i].y1 = UcakGovde[2].y;
	}
}
//Hedefleri rastgele olusturan fonksiyon
void HedefOlustur() {
	srand(time(NULL));
	int hedefx;
	int hedefy;
	for (int i = 0; i < hedefboyutu; i++) {
		hedefx = rand() % 100 + 1;
		hedefy = rand() % 18 + 1;
		if (hedefx < 40) hedefx = 41;
		HedefGovde[i].x2 = hedefx;
		HedefGovde[i].y2 = hedefy;
		HedefGovde[i].syon = YON_SOL;
	}
}
//ucagi oyun sahnesine yerlestiren fonksiyon
void UcagiSahneyeYerlestir() {
	for (int i = 0; i < ucakboyutu - 1; i++) {
		int x = UcakGovde[i].x;
		int y = UcakGovde[i].y;
		sahne[x][y] = 'D';// '▄';
	}
}
//Mermiyi oyun sahnesine yerlestiren fonksiyon
void MermiyiSahneyeYerlestir() {
	for (int i = 0; i < mermiboyutu - 1; i++) {
		int x = MermiGovde[i].x1;
		int y = MermiGovde[i].y1;
		sahne[x][y] = '-';
	}
}
//Hedefi oyun sahnesine yerlestiren fonksiyon
void HedefiSahneyeYerlestir() {
	for (int i = 0; i < hedefboyutu - 1; i++) {
		int x1 = HedefGovde[i].x2;
		int y1 = HedefGovde[i].y2;
		sahne[x1][y1] = 'X';//'▄'
	}
}
//Ucagi asagi hareket ettiren fonksiyon
void AsagiHareket() {
	for (int i = 0; i < ucakboyutu - 1; i++) {
		UcakGovde[i].y++;
		//MermiGovde[i].y1++;
	}
}
//Ucagi yukari hareket ettiren fonksiyon
void YukariHareket() {
	for (int i = 0; i < ucakboyutu - 1; i++) {
		UcakGovde[i].y--;
		//MermiGovde[i].y1--;
	}
}
//Mermileri saga dogru hareket ettiren fonksiyon
void MermiyiHareketEttir() {
	for (int i = 0; i < mermiboyutu; i++) {
		switch (MermiGovde[i].Syon)
		{
		case YON_SAG:
			MermiGovde[i].x1++;
			break;
		}
	}
}
//Hedefleri sola dogru hareket ettiren fonksiyon
void HedefiHareketEttir() {
	for (int i = 0; i < genislik; i++) {
		HedefGovde[i].x2--;
	}
}
//Tus takimini kullanarak oyunu kontrol edebilecegimiz ve ucaga yon verebilmemizi saglayan fonksiyon
void KlavyeKontrol() {
	klavyeoku(tuslar);
	if (tuslar[(char)38] != 0) {
		UstBasmaSayisi++;
		YukariHareket();
		for (int i = 0; i < ucakboyutu; i++)
			UcakGovde[i].yon = YON_YUKARI;
	}
	if (tuslar[(char)40] != 0) {
		Altbasmasayisi++;
		AsagiHareket();
		for (int i = 0; i < ucakboyutu; i++)
			UcakGovde[i].yon = YON_ASAGI;

	}

	if (tuslar[(char)32] != 0) {
		SpacebasmaSayisi++;
		MermiOlustur();
		for (int i = 0; i < mermiboyutu; i++)
			MermiGovde[i].Syon = YON_SAG;
	}
}
//merminin hedefi vurdugunda calisacak fonksiyon
void HedefVur() {
	for (int i = 0; i < mermiboyutu; i++) {
		for (int j = 0; j < hedefboyutu; j++) {
			if (MermiGovde[i].x1 == HedefGovde[j].x2 && MermiGovde[i].y1 == HedefGovde[j].y2)
			{
				score += 10;
				cout << "Score :" << score << endl;
				HedefOlustur();
				MermiOlustur();
			}
		}
	}
}
//Hedefin ucaga carptiginda oyunun bitmesini saglayan fonksiyon
int OyunBitti() {
	for (int i = 0; i < ucakboyutu; i++) {
		for (int j = 0; j < hedefboyutu; j++) {
			if (UcakGovde[i].x == HedefGovde[j].x2 && UcakGovde[i].y == HedefGovde[j].y2)
			{
				Canli = false;
				sahneyiTemizle();
				cout << "Oyunu Kaybettiniz!!" << "Score :" << score << endl;
				system("Pause");
				return 0;
			}
		}
	}
}


int main()
{
	setlocale(LC_ALL, "Turkish");

	UcakOlustur();
	if (Canli == true) {
		HedefOlustur();
	}
	MermiOlustur();
	KursoruGizle();


	while (true) {
		sahneyiTemizle();
		sinirlariOlustur();
		KlavyeKontrol();
		UcagiSahneyeYerlestir();

		HedefiSahneyeYerlestir();
		HedefiHareketEttir();
		MermiyiSahneyeYerlestir();
		MermiyiHareketEttir();
		cercevexy(0, 0);
		sahneyiCiz();

		Sleep(150);
		OyunBitti();
		HedefVur();
	}

}