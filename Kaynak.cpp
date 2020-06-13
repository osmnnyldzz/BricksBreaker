#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include<allegro5/allegro_image.h>
#include<allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>


// Bloklar� olu�turaca��m�z �zellikleri tutan Struct.
struct Brick
{
	int lives;				//Bloklar�n canlar�n�(1 veya 2) belirlemek i�in gerekli de�i�ken.
	float x1;				//
	float x2;				//
	float y1;				//Blo�un ekran �zerindeki konumlar�
	float y2;				//
	float centerX;			//Topun hangi kenara �arpt���n� anlamak i�in gereken merkez noktalar�.
	float centerY;			//
	ALLEGRO_COLOR color;	//Blok renklerini tuttu�umuz de�i�ken
};

// Toplar� olu�turaca��m�z �zellikleri tutan Struct.
struct Ball
{
	float x;				// Topun ba�lang�� X ve Y de�eri
	float y;				//
	float radius;			// Topun yar� �ap�n� tutaca��m�z de�i�ken
	float speed;			// Topun h�z de�erini tutaca��m�z de�i�ken.
	float dx;
	float dy;
	float angle;
};

int main()
{
	al_init();



	al_init_primitives_addon(); //Kare,Top gibi g�rsel ��eler i�in al_init_primitives_addon(); kod par�ac���n� ekliyoruz.
	al_init_font_addon();		//Yaz� desenleri i�in al_init_font_addon();	 kod par�ac���n� ekliyoruz.
	al_init_ttf_addon();		//Yaz� desenleri i�in al_init_ttf_addon();	 kod par�ac���n� ekliyoruz.
	al_init_acodec_addon();		//Ses efekti kullanmak i�in al_init_acodec_addon();	kod par�ac���n� ekliyoruz.
	al_init_image_addon();		//Background resim koymak i�in al_init_image_addon(); kod par�ac���n� ekliyoruz.

	al_install_mouse();			//
	al_install_keyboard();		// Ses,Fare ve Klavyeyi aktif etmek i�in kulland���m�z kod par�ac���.
	al_install_audio();			//

	ALLEGRO_DISPLAY* display = NULL;							// ALLEGRO_DISPLAY ile display'i tan�mlad�k.Ekran� bilgisayara tan�tmak i�in.
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue(); // Fare ve Klavye eventleri i�in event_queue tan�mlad�k ve al_create metodu ile eventi olu�turduk.


	/*       RENK KODLARINI de�i�ken isimlerine tan�mlayarak top ve bloklara renk verme kolayl��� sa�lad�k.        */
	////////////////////////////////// >>>> RENK AYARLARI <<<<< ////////////////////////////////////
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR pink = al_map_rgb(255, 0, 255);
	ALLEGRO_COLOR ballColor = white;
	ALLEGRO_COLOR bricksColor = yellow;
	ALLEGRO_COLOR electricBlue = al_map_rgb(44, 117, 255);
	ALLEGRO_COLOR antiqueGold = al_map_rgb(221, 196, 136);
	ALLEGRO_COLOR deepPing = al_map_rgb(255, 20, 147);
	ALLEGRO_COLOR snakeColor = al_map_rgb(201, 0, 0);
	ALLEGRO_COLOR fireBrick = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR gold = al_map_rgb(255, 215, 0);
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR cBackgraund = al_map_rgb(115, 191, 36);
	ALLEGRO_COLOR diamond = al_map_rgb(0, 245, 255);
	ALLEGRO_COLOR screenColor = al_map_rgb(0, 0, 255);


	/* Bloklara �arpma sesi verebilmek i�in ses dosyas�n� projemize dahil ettik.*/
	////////////////////////////////// >>>> SES AYARLARI <<<<< /////////////////////////////////////////
	al_reserve_samples(10);
	ALLEGRO_SAMPLE* food = al_load_sample("sounds/food.wav");


	/* Font kullanabilmek i�in ttf dosyalar�n� projemize dahil ettik. */
	//--FONTS AYARLARI -----------------------------------------------------------------------------------
	ALLEGRO_FONT* font = al_load_font("ttfs/orbitronblack.ttf", 56, NULL);
	ALLEGRO_FONT* font20 = al_load_font("ttfs/orbitronblack.ttf", 20, NULL);
	ALLEGRO_FONT* font15 = al_load_font("ttfs/orbitronblack.ttf", 15, NULL);
	ALLEGRO_FONT* font25 = al_load_font("ttfs/orbitronblack.ttf", 25, NULL);
	ALLEGRO_FONT* livesFont = al_load_font("ttfs/orbitronblack.ttf", 18, NULL);
	ALLEGRO_FONT* smallFont = al_load_font("ttfs/orbitronblack.ttf", 10, NULL);
	ALLEGRO_FONT* nunitoFont = al_load_font("ttfs/Nunito-Black.ttf", 18, NULL);
	//----------------------------------------------------------------------------------------------------

	////////////////////////////////// >>>> EKRAN AYARLARI <<<<< /////////////////////////////////////////
	int width = 600;			// Ekran boyutu geni�lik.
	int heigth = 500;			// Ekran boyutu y�kseklik.
	int gameZone = 50;			// Oyun alan� belirlemek i�in tan�mlad�k.
	int gameOver = 0;			// Oyun bitirme kodlar� i�in de�er tutaca��m�z de�i�ken.

	ALLEGRO_BITMAP* bgImage = NULL;
	bgImage = al_load_bitmap("image/bg.jpg"); //Backgorund ekledik.

	/////////////////////////////////  >>>> Oyun AYARLARI <<<<< //////////////////////////////////////////
	const int ballCount = 3;			// Top say�s�n�n tuttuldu�u de�i�ken.
	const int brickCount = 24;			// Blok say�s�n�n tuttuldu�u de�i�ken.

	////////////////////////////////// >>>> BRICK AYARLARI <<<<< /////////////////////////////////////////
	struct Brick bricks[brickCount];	//Struct �zerinden 24 tane blok olu�turduk.
	for (int x = 0; x < brickCount; x++)
	{
		// 1.s�radaki 8 blo�u tan�mlad�k.
		if (x < 8)
		{
			bricks[x].color = pink;
			bricks[x].x1 = 100 + (x * 51);						// Bloklar�n yanyana gelmesi i�in +51 de�er ekliyoruz.
			bricks[x].y1 = 75;
			bricks[x].x2 = 150 + (x * 51);
			bricks[x].y2 = 125;
			bricks[x].centerX = (bricks[x].x1 + bricks[x].x2) / 2;
			bricks[x].centerY = (bricks[x].y1 + bricks[x].y2) / 2;
			bricks[x].lives = (1 + rand() % 2);
		}
		// 2.s�radaki 8 blo�u tan�mlad�k.
		else if (x >= 8 && x < 16)
		{
			bricks[x].color = pink;
			bricks[x].x1 = 100 + ((x - 8) * 51);
			bricks[x].y1 = 130;
			bricks[x].x2 = 150 + ((x - 8) * 51);
			bricks[x].y2 = 180;
			bricks[x].centerX = (bricks[x].x1 + bricks[x].x2) / 2;
			bricks[x].centerY = (bricks[x].y1 + bricks[x].y2) / 2;
			bricks[x].lives = (1 + rand() % 2);
		}
		// 3.s�radaki 8 blo�u tan�mlad�k.
		else if (x >= 16)
		{
			bricks[x].color = pink;
			bricks[x].x1 = 100 + ((x - 16) * 51);
			bricks[x].y1 = 185;
			bricks[x].x2 = 150 + ((x - 16) * 51);
			bricks[x].y2 = 235;
			bricks[x].centerX = (bricks[x].x1 + bricks[x].x2) / 2;
			bricks[x].centerY = (bricks[x].y1 + bricks[x].y2) / 2;
			bricks[x].lives = (1 + rand() % 2);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////// >>>> BALL AYARLARI <<<<< //////////////////////////////////////////////	
	struct Ball ball[ballCount];
	float angle = 0;													//Topun ilk a��s� s�f�r olarak tan�mlad�k.
	for (int topIndex = 0; topIndex < ballCount; topIndex++)			//Static olarak tan�mlad���m�z top say�s� kadar top olu
	{
		ball[topIndex].radius = 10;										//Topun �ap�.				
		ball[topIndex].x = width / 2;									//Top ba�lang�� noktas�.
		ball[topIndex].y = heigth - ball[topIndex].radius - gameZone;	//Topun oyun alan�n�n en alt�nda olmas� i�in uygulad���m�z i�lemler.
		ball[topIndex].angle = angle;									//Toplar�n ilk a��s�n� verdik.
		ball[topIndex].dx = cos(ball[topIndex].angle);					//Toplar�n hangi a��yla nereye gidece�ini hesaplayan kod.(Sadece ba�lang�� olarak atanm�� de�erler)
		ball[topIndex].dy = sin(ball[topIndex].angle);
		ball[topIndex].speed = 0.3;										//Topun h�z�.
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	display = al_create_display(width, heigth);							//Ekran� olu�turduk.

	al_register_event_source(event_queue, al_get_mouse_event_source());				//Eventleri mouse,klavye i�in tan�tt�k.
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));


	al_hide_mouse_cursor(display);										// Mouse imlecini g�r�nmez yapt�k.

	int ballRangeCounter = 0;			// Toplar� arka arkaya atarken araya mesafe koymak i�in counter tan�mlad�k.		
	int topIn = 1;

	float topX = 900;					// Mouse'un X konumunu bu de�i�ken de tutaca��z.
	float topY = 900;					// Mouse'un Y konumunu bu de�i�ken de tutaca��z.

	bool mainLoop = true;				// Oyun ba�latan Bool
	bool isLocated = false;				// Topun konumland�r�lmas�n� kontrol eden Bool
	bool canMove = false;				// Topun hareketini kontrol eden Bool
	bool levelComplete = false;			// Bloklar�n bir alt sat�ra inmesini kontrol eden Bool
	bool success = false;				// Oyunun Win mi Lose mu oldu�unu kontrol eden Bool 

	int ballStoped = 0;					// Yere �arpt���nda duran toplar�n say�s�n� belirlemek.
	int heart = 5;

	// Talimatlar ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	al_clear_to_color(black);
	al_draw_text(font25, electricBlue, width / 2, heigth / 2 - 200, ALLEGRO_ALIGN_CENTRE, " Instructions ");
	al_draw_text(font15, diamond, width / 2, heigth / 2 - 165, ALLEGRO_ALIGN_CENTRE, " -Game Information- ");
	al_draw_text(font15, screenColor, width / 2, heigth / 2 - 135, ALLEGRO_ALIGN_CENTRE, " -The game consists of 1 level. ");
	al_draw_text(font15, screenColor, width / 2, heigth / 2 - 105, ALLEGRO_ALIGN_CENTRE, " -If the hearts runs out, the game is over. ");
	al_draw_text(font15, screenColor, width / 2, heigth / 2 - 75, ALLEGRO_ALIGN_CENTRE, "-If the bricks fall to the ground, the game is over. ");
	al_draw_text(font15, screenColor, width / 2, heigth / 2 - 45, ALLEGRO_ALIGN_CENTRE, " -If you break all the bricks, you win. Congratulations!");


	al_draw_text(font25, electricBlue, width / 2, heigth / 2, ALLEGRO_ALIGN_CENTRE, " Gameplay ");
	al_draw_text(font15, screenColor, width / 2, heigth / 2 + 45, ALLEGRO_ALIGN_CENTRE, " -Use the mouse to move the ball left or right.");
	al_draw_text(font15, screenColor, width / 2, heigth / 2 + 75, ALLEGRO_ALIGN_CENTRE, " -Left click once to position the ball.");
	al_draw_text(font15, screenColor, width / 2, heigth / 2 + 105, ALLEGRO_ALIGN_CENTRE, " -Use the mouse to determine the direction of the ball.");
	al_draw_text(font15, screenColor, width / 2, heigth / 2 + 135, ALLEGRO_ALIGN_CENTRE, " -Left click once again to direction the ball.");

	al_draw_text(font25, diamond, width / 2, heigth / 2 + 190, ALLEGRO_ALIGN_CENTRE, " *PRESS ENTER TO SKIP* "); al_flip_display();

	// Talimatlar� bitiren kod--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while (1) {
		ALLEGRO_EVENT events;						// events'i ALLEGRO k�t�phanesi ile tan�ml�yoruz ve event_queue'nin events beklemesini sa�l�yoruz.
		al_wait_for_event(event_queue, &events);

		if (events.keyboard.keycode == ALLEGRO_KEY_ENTER)	//E�er Enter'a t�klan�rsa talimatlar� ge�mesini sa�lad���m�z k�s�m.
		{
			break;
		}
	}

	//******************************************** OYUNUN BA�LADI�I WHILE D�NG�S� ********************************************
	while (mainLoop)
	{

		al_draw_bitmap(bgImage, 100, 100, 0);

		/* Bricks olu�turdu�umuz k�s�m */
		for (int i = 0; i < brickCount; i++)
		{
			al_draw_filled_rectangle(bricks[i].x1, bricks[i].y1, bricks[i].x2, bricks[i].y2, bricks[i].color);
			al_draw_filled_rectangle(bricks[i].x1 + 3, bricks[i].y1 + 3, bricks[i].x2 - 3, bricks[i].y2 - 3, black);
			al_draw_textf(nunitoFont, white, bricks[i].centerX - 10, bricks[i].centerY - 10, NULL, "%d", bricks[i].lives);
		}

		/* �ST VE ALT ��ZG�LER */
		al_draw_line(0, gameZone, width, gameZone, pink, 2);
		al_draw_line(0, heigth - gameZone, width, heigth - gameZone, pink, 2);

		/* EKRAN DA G�R�NEN YAZILAR */
		al_draw_text(font25, white, width - 465, heigth - 485, NULL, " BRICK BREAKER GAME");
		al_draw_textf(font20, gold, width - 585, heigth - 35, NULL, " Heart: x%d", heart);

		/* Ball olu�turdu�umuz k�s�m */
		for (int topIndex = 0; topIndex < ballCount; topIndex++)
		{
			al_draw_filled_circle(ball[topIndex].x, ball[topIndex].y, ball[topIndex].radius, ballColor);
			if (topIn != ballCount)
				al_draw_textf(nunitoFont, white, ball[ballCount - 1].x + 10, ball[ballCount - 1].y - 10, NULL, "%d", ballCount - topIn + 1);
		}


		//Toplar hareket etmiyorsa (canMove) ve sabit duruyorlarsa (isLocated) toplar� atarken ��kan k���k toplar�n olu�turulmas�
		if (!canMove && isLocated)
		{
			for (int i = 0; i < 20; i++)
			{
				al_draw_filled_circle(ball[0].x - ((ball[0].x - topX) / 20) * i, ball[0].y - ((ball[0].y - topY) / 20) * i, 3, antiqueGold);
			}
			al_draw_filled_circle(topX, topY, 3, antiqueGold);
		}




		// Topu SA�-SOL hareket ettirmek i�in kulland���m�z kod par�ac���.
		if (!isLocated && canMove == false)
		{
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				mainLoop = false;
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
			{
				for (int topIndex = 0; topIndex < ballCount; topIndex++)
				{
					ball[topIndex].x = ev.mouse.x;	// Topu sadece X y�n�nde hareket ettirmemiz i�in gerekli mouse eventi.
				}

			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (ev.mouse.button & 1)
				{
					topX = ev.mouse.x;				// Toplar�n yerini belirlemek i�in mouse ile 1 kere t�klad���m�zda mouse'un
					topY = ev.mouse.y;				// konumlar�n�, topX ve topY de�i�kenlerine atama i�lemi
					isLocated = true;				// Topu sabitledi�imizi belirtiyoruz ve yukar�daki a��klad���m kodda ni�an almam�z� kolayla�t�racak toplar� olu�turuyoruz.
				}
			}
		}

		// Toplar�n ni�an alma k�sm�nda kulland���m�z eventler
		if (!canMove && isLocated)
		{
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				mainLoop = false;
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
			{
				topX = ev.mouse.x;			// Ni�an alma yard�m�n�n X ve Y kordinatlar�n� topX ve topY de�i�kenlerine at�yoruz.
				topY = ev.mouse.y;
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (ev.mouse.button & 1)
				{
					canMove = true;			// Mouse'a bir kere daha t�kland��� zaman toplar�n hareketine ba�lamas�na izin verdi�imiz kod.

					for (int topIndex = 0; topIndex < ballCount; topIndex++)
					{
						ball[topIndex].dx = (topX - ball[0].x) / sqrt(pow(topX - ball[0].x, 2) + pow(topY - ball[0].y, 2));		// Toplar�n matematiksel i�lemler ile dx ve dy konumlar�n� belirliyoruz.
						ball[topIndex].dy = (topY - ball[0].y) / sqrt(pow(topX - ball[0].x, 2) + pow(topY - ball[0].y, 2));
					}
				}
			}
		}


		al_flip_display();

		// E�er toplar hareket ederse �al��an kod.
		if (canMove)
		{
			if (ballRangeCounter > 250)				// wait komutu bulamad���m i�in toplar aras�ndaki mesafeyi ayarlamak i�in kendi Waitimizi kodlad���m�z k�s�m.
			{
				if (topIn < ballCount)
				{
					ballRangeCounter = 0;
					topIn++;
				}
			}
			else
			{
				ballRangeCounter++;
			}

			for (int topIndex = 0; topIndex < topIn; topIndex++)
			{
				ball[topIndex].x += ball[topIndex].dx * ball[topIndex].speed;		// Toplar�n x y�n�ndeki hareketi.
				ball[topIndex].y += ball[topIndex].dy * ball[topIndex].speed;		// Toplar�n y y�n�ndeki hareketi.

				if (ball[topIndex].x < ball[topIndex].radius) ball[topIndex].dx = -ball[topIndex].dx;				// SOL DUVAR
				if (ball[topIndex].y < ball[topIndex].radius + gameZone) ball[topIndex].dy = -ball[topIndex].dy;	// �ST DUVAR
				if (ball[topIndex].x + ball[topIndex].radius > width) ball[topIndex].dx = -ball[topIndex].dx;		// SA� DUVAR
				if (ball[topIndex].y + ball[topIndex].radius == heigth - gameZone)									// ALT DUVAR
				{
					ball[topIndex].dx = 0;				//E�er top yere �arparsa dur komutu.
					ball[topIndex].dy = 0;


					while (ball[ballStoped].dx == 0 && ball[ballStoped].dy == 0) // Her iki y�nde de durdu�u zaman Duran top say�s�n� art�r�yoruz.
					{
						ballStoped++;
					}

					if (ballStoped == ballCount)			// E�er duran top say�s�, normal top say�s�na e�it ise. Hareket ettirme komutunu False yap�yoruz.
					{
						canMove = false;
						for (int i = 0; i < ballCount; i++)
						{
							ball[i].x = ball[0].x;			// �lk d��en topun konumunu di�er toplar�n konumuna veriyoruz b�ylece ayn� noktada duruyorlar.
							ball[i].y = ball[0].y;
							topIn = 1;
							ballStoped = 0;					// Tekrar at�� yap�lmas� halinde de�i�kenlerimizi s�f�rl�yoruz.
							ballRangeCounter = 0;
						}
						heart--;							// Toplar her yere �arp�p durdu�unda can bir eksilmekte.
						levelComplete = true;
					}


				} // ALT

			}


		}

		for (int z = 0; z < brickCount; z++)
		{
			for (int topIndex = 0; topIndex < ballCount; topIndex++)
			{
				float xdiff = fabs(ball[topIndex].x - bricks[z].centerX);
				float ydiff = fabs(ball[topIndex].y - bricks[z].centerY);

				if (xdiff < ball[topIndex].radius + 25 && ydiff < ball[topIndex].radius + 25)				// Topun karelere yakla�t���n� ve temas etti�ini kontrol ediyoruz.
				{
					if (ydiff > xdiff)																		// Sol ve Sa� i�in kontrol
					{
						al_play_sample(food, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						bricks[z].color = electricBlue;
						bricks[z].lives--;

						ball[topIndex].dy = -ball[topIndex].dy;							// Topun geldi�i a��y� eksiye �eviriyoruz.
						ball[topIndex].x += ball[topIndex].dx * ball[topIndex].speed;	// Topun h�z�n� tekrar belirliyoruz.
						ball[topIndex].y += ball[topIndex].dy * ball[topIndex].speed;
					}
					else																// Alt ve �st i�in kontrol
					{
						al_play_sample(food, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						bricks[z].color = electricBlue;
						bricks[z].lives--;

						ball[topIndex].dx = -ball[topIndex].dx;							// Topun geldi�i a��y� eksiye �eviriyoruz.
						ball[topIndex].x += ball[topIndex].dx * ball[topIndex].speed;	// Topun h�z�n� tekrar belirliyoruz.
						ball[topIndex].y += ball[topIndex].dy * ball[topIndex].speed;
					}

				}

				if (bricks[z].lives <= 0 && bricks[z].x1 != -2000)						// Toplar�n s�f�rlad��� bloklar� yok etti�imiz k�s�m.
				{
					bricks[z].x1 = -2000;
					bricks[z].x2 = -2000;												// Bloklar� silmek yerine daha basit yol olan uzaya g�nderme
					bricks[z].y1 = -2000;												// i�lemi ile ekrandan bloklar� kald�r�yoruz.
					bricks[z].y2 = -2000;
					bricks[z].centerX += -2000;
					bricks[z].centerY += -2000;

					gameOver++;
				}
			}

		}

		//Her levelde bloklar 1 s�ra a�a��ya inmektedir. 
		if (levelComplete) {
			for (int i = 0; i < brickCount; i++)
			{
				bricks[i].y1 += 51;												// 1 s�ra a�a��ya indirmek i�in +50 ekliyoruz (sadece y1 ve y2)
				bricks[i].y2 += 51;
				bricks[i].centerX = (bricks[i].x1 + bricks[i].x2) / 2;
				bricks[i].centerY = (bricks[i].y1 + bricks[i].y2) / 2;

			}


			levelComplete = false;												// Yeni level ba�lang�c� i�in False yapt�k.
		}
		for (int i = 0; i < brickCount; i++)
		{
			if (bricks[i].y2 >= heigth - gameZone) {				//E�er bloklar yere �arparsa oyunu kaybetme komutu.

				success = false;
				mainLoop = false;

			}
		}


		if (heart == 0) {										// E�er can biterse oyunu kaybetme komutu.

			success = false;
			mainLoop = false;


		}
		if (gameOver == brickCount)								//E�er t�m bloklar silinmi�se oyunu kazanma komutu.
		{
			success = true;
			mainLoop = false;

		}



		al_clear_to_color(al_map_rgb(0, 0, 0));


	}

	while (true)
	{
		if (success)		// Ekrana SUCCESFUL yazd�rma komutu
		{
			al_clear_to_color(black);
			al_draw_text(font25, diamond, width / 2, heigth / 2, ALLEGRO_ALIGN_CENTRE, " SUCCESFUL! "); al_flip_display();
		}
		else				// Ekrana Game Over yazd�rma komutu
		{
			al_clear_to_color(black);
			al_draw_text(font25, diamond, width / 2, heigth / 2 - 25, ALLEGRO_ALIGN_CENTRE, " GAME OVER! "); al_flip_display();
			al_draw_text(font25, diamond, width / 2, heigth / 2 + 25, ALLEGRO_ALIGN_CENTRE, "PRESS ENTER FOR TRY AGAIN "); al_flip_display();
		}

		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		if (events.keyboard.keycode == ALLEGRO_KEY_ENTER)
		{
			main();			// E�er oyuncu oyuna tekrar ba�lamak isterse Enter tu�u ile tekrar ba�layabilme eventi.
		}

	}
	al_destroy_bitmap(bgImage);
	al_destroy_display(display);

	return 0;
}

