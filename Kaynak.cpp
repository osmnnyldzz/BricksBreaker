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


// Bloklarý oluþturacaðýmýz özellikleri tutan Struct.
struct Brick
{
	int lives;				//Bloklarýn canlarýný(1 veya 2) belirlemek için gerekli deðiþken.
	float x1;				//
	float x2;				//
	float y1;				//Bloðun ekran üzerindeki konumlarý
	float y2;				//
	float centerX;			//Topun hangi kenara çarptýðýný anlamak için gereken merkez noktalarý.
	float centerY;			//
	ALLEGRO_COLOR color;	//Blok renklerini tuttuðumuz deðiþken
};

// Toplarý oluþturacaðýmýz özellikleri tutan Struct.
struct Ball
{
	float x;				// Topun baþlangýç X ve Y deðeri
	float y;				//
	float radius;			// Topun yarý çapýný tutacaðýmýz deðiþken
	float speed;			// Topun hýz deðerini tutacaðýmýz deðiþken.
	float dx;
	float dy;
	float angle;
};

int main()
{
	al_init();



	al_init_primitives_addon(); //Kare,Top gibi görsel öðeler için al_init_primitives_addon(); kod parçacýðýný ekliyoruz.
	al_init_font_addon();		//Yazý desenleri için al_init_font_addon();	 kod parçacýðýný ekliyoruz.
	al_init_ttf_addon();		//Yazý desenleri için al_init_ttf_addon();	 kod parçacýðýný ekliyoruz.
	al_init_acodec_addon();		//Ses efekti kullanmak için al_init_acodec_addon();	kod parçacýðýný ekliyoruz.
	al_init_image_addon();		//Background resim koymak için al_init_image_addon(); kod parçacýðýný ekliyoruz.

	al_install_mouse();			//
	al_install_keyboard();		// Ses,Fare ve Klavyeyi aktif etmek için kullandýðýmýz kod parçacýðý.
	al_install_audio();			//

	ALLEGRO_DISPLAY* display = NULL;							// ALLEGRO_DISPLAY ile display'i tanýmladýk.Ekraný bilgisayara tanýtmak için.
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue(); // Fare ve Klavye eventleri için event_queue tanýmladýk ve al_create metodu ile eventi oluþturduk.


	/*       RENK KODLARINI deðiþken isimlerine tanýmlayarak top ve bloklara renk verme kolaylýðý saðladýk.        */
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


	/* Bloklara çarpma sesi verebilmek için ses dosyasýný projemize dahil ettik.*/
	////////////////////////////////// >>>> SES AYARLARI <<<<< /////////////////////////////////////////
	al_reserve_samples(10);
	ALLEGRO_SAMPLE* food = al_load_sample("sounds/food.wav");


	/* Font kullanabilmek için ttf dosyalarýný projemize dahil ettik. */
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
	int width = 600;			// Ekran boyutu geniþlik.
	int heigth = 500;			// Ekran boyutu yükseklik.
	int gameZone = 50;			// Oyun alaný belirlemek için tanýmladýk.
	int gameOver = 0;			// Oyun bitirme kodlarý için deðer tutacaðýmýz deðiþken.

	ALLEGRO_BITMAP* bgImage = NULL;
	bgImage = al_load_bitmap("image/bg.jpg"); //Backgorund ekledik.

	/////////////////////////////////  >>>> Oyun AYARLARI <<<<< //////////////////////////////////////////
	const int ballCount = 3;			// Top sayýsýnýn tuttulduðu deðiþken.
	const int brickCount = 24;			// Blok sayýsýnýn tuttulduðu deðiþken.

	////////////////////////////////// >>>> BRICK AYARLARI <<<<< /////////////////////////////////////////
	struct Brick bricks[brickCount];	//Struct üzerinden 24 tane blok oluþturduk.
	for (int x = 0; x < brickCount; x++)
	{
		// 1.sýradaki 8 bloðu tanýmladýk.
		if (x < 8)
		{
			bricks[x].color = pink;
			bricks[x].x1 = 100 + (x * 51);						// Bloklarýn yanyana gelmesi için +51 deðer ekliyoruz.
			bricks[x].y1 = 75;
			bricks[x].x2 = 150 + (x * 51);
			bricks[x].y2 = 125;
			bricks[x].centerX = (bricks[x].x1 + bricks[x].x2) / 2;
			bricks[x].centerY = (bricks[x].y1 + bricks[x].y2) / 2;
			bricks[x].lives = (1 + rand() % 2);
		}
		// 2.sýradaki 8 bloðu tanýmladýk.
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
		// 3.sýradaki 8 bloðu tanýmladýk.
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
	float angle = 0;													//Topun ilk açýsý sýfýr olarak tanýmladýk.
	for (int topIndex = 0; topIndex < ballCount; topIndex++)			//Static olarak tanýmladýðýmýz top sayýsý kadar top olu
	{
		ball[topIndex].radius = 10;										//Topun çapý.				
		ball[topIndex].x = width / 2;									//Top baþlangýç noktasý.
		ball[topIndex].y = heigth - ball[topIndex].radius - gameZone;	//Topun oyun alanýnýn en altýnda olmasý için uyguladýðýmýz iþlemler.
		ball[topIndex].angle = angle;									//Toplarýn ilk açýsýný verdik.
		ball[topIndex].dx = cos(ball[topIndex].angle);					//Toplarýn hangi açýyla nereye gideceðini hesaplayan kod.(Sadece baþlangýç olarak atanmýþ deðerler)
		ball[topIndex].dy = sin(ball[topIndex].angle);
		ball[topIndex].speed = 0.3;										//Topun hýzý.
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	display = al_create_display(width, heigth);							//Ekraný oluþturduk.

	al_register_event_source(event_queue, al_get_mouse_event_source());				//Eventleri mouse,klavye için tanýttýk.
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));


	al_hide_mouse_cursor(display);										// Mouse imlecini görünmez yaptýk.

	int ballRangeCounter = 0;			// Toplarý arka arkaya atarken araya mesafe koymak için counter tanýmladýk.		
	int topIn = 1;

	float topX = 900;					// Mouse'un X konumunu bu deðiþken de tutacaðýz.
	float topY = 900;					// Mouse'un Y konumunu bu deðiþken de tutacaðýz.

	bool mainLoop = true;				// Oyun baþlatan Bool
	bool isLocated = false;				// Topun konumlandýrýlmasýný kontrol eden Bool
	bool canMove = false;				// Topun hareketini kontrol eden Bool
	bool levelComplete = false;			// Bloklarýn bir alt satýra inmesini kontrol eden Bool
	bool success = false;				// Oyunun Win mi Lose mu olduðunu kontrol eden Bool 

	int ballStoped = 0;					// Yere çarptýðýnda duran toplarýn sayýsýný belirlemek.
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

	// Talimatlarý bitiren kod--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while (1) {
		ALLEGRO_EVENT events;						// events'i ALLEGRO kütüphanesi ile tanýmlýyoruz ve event_queue'nin events beklemesini saðlýyoruz.
		al_wait_for_event(event_queue, &events);

		if (events.keyboard.keycode == ALLEGRO_KEY_ENTER)	//Eðer Enter'a týklanýrsa talimatlarý geçmesini saðladýðýmýz kýsým.
		{
			break;
		}
	}

	//******************************************** OYUNUN BAÞLADIÐI WHILE DÖNGÜSÜ ********************************************
	while (mainLoop)
	{

		al_draw_bitmap(bgImage, 100, 100, 0);

		/* Bricks oluþturduðumuz kýsým */
		for (int i = 0; i < brickCount; i++)
		{
			al_draw_filled_rectangle(bricks[i].x1, bricks[i].y1, bricks[i].x2, bricks[i].y2, bricks[i].color);
			al_draw_filled_rectangle(bricks[i].x1 + 3, bricks[i].y1 + 3, bricks[i].x2 - 3, bricks[i].y2 - 3, black);
			al_draw_textf(nunitoFont, white, bricks[i].centerX - 10, bricks[i].centerY - 10, NULL, "%d", bricks[i].lives);
		}

		/* ÜST VE ALT ÇÝZGÝLER */
		al_draw_line(0, gameZone, width, gameZone, pink, 2);
		al_draw_line(0, heigth - gameZone, width, heigth - gameZone, pink, 2);

		/* EKRAN DA GÖRÜNEN YAZILAR */
		al_draw_text(font25, white, width - 465, heigth - 485, NULL, " BRICK BREAKER GAME");
		al_draw_textf(font20, gold, width - 585, heigth - 35, NULL, " Heart: x%d", heart);

		/* Ball oluþturduðumuz kýsým */
		for (int topIndex = 0; topIndex < ballCount; topIndex++)
		{
			al_draw_filled_circle(ball[topIndex].x, ball[topIndex].y, ball[topIndex].radius, ballColor);
			if (topIn != ballCount)
				al_draw_textf(nunitoFont, white, ball[ballCount - 1].x + 10, ball[ballCount - 1].y - 10, NULL, "%d", ballCount - topIn + 1);
		}


		//Toplar hareket etmiyorsa (canMove) ve sabit duruyorlarsa (isLocated) toplarý atarken çýkan küçük toplarýn oluþturulmasý
		if (!canMove && isLocated)
		{
			for (int i = 0; i < 20; i++)
			{
				al_draw_filled_circle(ball[0].x - ((ball[0].x - topX) / 20) * i, ball[0].y - ((ball[0].y - topY) / 20) * i, 3, antiqueGold);
			}
			al_draw_filled_circle(topX, topY, 3, antiqueGold);
		}




		// Topu SAÐ-SOL hareket ettirmek için kullandýðýmýz kod parçacýðý.
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
					ball[topIndex].x = ev.mouse.x;	// Topu sadece X yönünde hareket ettirmemiz için gerekli mouse eventi.
				}

			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (ev.mouse.button & 1)
				{
					topX = ev.mouse.x;				// Toplarýn yerini belirlemek için mouse ile 1 kere týkladýðýmýzda mouse'un
					topY = ev.mouse.y;				// konumlarýný, topX ve topY deðiþkenlerine atama iþlemi
					isLocated = true;				// Topu sabitlediðimizi belirtiyoruz ve yukarýdaki açýkladýðým kodda niþan almamýzý kolaylaþtýracak toplarý oluþturuyoruz.
				}
			}
		}

		// Toplarýn niþan alma kýsmýnda kullandýðýmýz eventler
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
				topX = ev.mouse.x;			// Niþan alma yardýmýnýn X ve Y kordinatlarýný topX ve topY deðiþkenlerine atýyoruz.
				topY = ev.mouse.y;
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (ev.mouse.button & 1)
				{
					canMove = true;			// Mouse'a bir kere daha týklandýðý zaman toplarýn hareketine baþlamasýna izin verdiðimiz kod.

					for (int topIndex = 0; topIndex < ballCount; topIndex++)
					{
						ball[topIndex].dx = (topX - ball[0].x) / sqrt(pow(topX - ball[0].x, 2) + pow(topY - ball[0].y, 2));		// Toplarýn matematiksel iþlemler ile dx ve dy konumlarýný belirliyoruz.
						ball[topIndex].dy = (topY - ball[0].y) / sqrt(pow(topX - ball[0].x, 2) + pow(topY - ball[0].y, 2));
					}
				}
			}
		}


		al_flip_display();

		// Eðer toplar hareket ederse çalýþan kod.
		if (canMove)
		{
			if (ballRangeCounter > 250)				// wait komutu bulamadýðým için toplar arasýndaki mesafeyi ayarlamak için kendi Waitimizi kodladýðýmýz kýsým.
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
				ball[topIndex].x += ball[topIndex].dx * ball[topIndex].speed;		// Toplarýn x yönündeki hareketi.
				ball[topIndex].y += ball[topIndex].dy * ball[topIndex].speed;		// Toplarýn y yönündeki hareketi.

				if (ball[topIndex].x < ball[topIndex].radius) ball[topIndex].dx = -ball[topIndex].dx;				// SOL DUVAR
				if (ball[topIndex].y < ball[topIndex].radius + gameZone) ball[topIndex].dy = -ball[topIndex].dy;	// ÜST DUVAR
				if (ball[topIndex].x + ball[topIndex].radius > width) ball[topIndex].dx = -ball[topIndex].dx;		// SAÐ DUVAR
				if (ball[topIndex].y + ball[topIndex].radius == heigth - gameZone)									// ALT DUVAR
				{
					ball[topIndex].dx = 0;				//Eðer top yere çarparsa dur komutu.
					ball[topIndex].dy = 0;


					while (ball[ballStoped].dx == 0 && ball[ballStoped].dy == 0) // Her iki yönde de durduðu zaman Duran top sayýsýný artýrýyoruz.
					{
						ballStoped++;
					}

					if (ballStoped == ballCount)			// Eðer duran top sayýsý, normal top sayýsýna eþit ise. Hareket ettirme komutunu False yapýyoruz.
					{
						canMove = false;
						for (int i = 0; i < ballCount; i++)
						{
							ball[i].x = ball[0].x;			// Ýlk düþen topun konumunu diðer toplarýn konumuna veriyoruz böylece ayný noktada duruyorlar.
							ball[i].y = ball[0].y;
							topIn = 1;
							ballStoped = 0;					// Tekrar atýþ yapýlmasý halinde deðiþkenlerimizi sýfýrlýyoruz.
							ballRangeCounter = 0;
						}
						heart--;							// Toplar her yere çarpýp durduðunda can bir eksilmekte.
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

				if (xdiff < ball[topIndex].radius + 25 && ydiff < ball[topIndex].radius + 25)				// Topun karelere yaklaþtýðýný ve temas ettiðini kontrol ediyoruz.
				{
					if (ydiff > xdiff)																		// Sol ve Sað için kontrol
					{
						al_play_sample(food, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						bricks[z].color = electricBlue;
						bricks[z].lives--;

						ball[topIndex].dy = -ball[topIndex].dy;							// Topun geldiði açýyý eksiye çeviriyoruz.
						ball[topIndex].x += ball[topIndex].dx * ball[topIndex].speed;	// Topun hýzýný tekrar belirliyoruz.
						ball[topIndex].y += ball[topIndex].dy * ball[topIndex].speed;
					}
					else																// Alt ve Üst için kontrol
					{
						al_play_sample(food, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						bricks[z].color = electricBlue;
						bricks[z].lives--;

						ball[topIndex].dx = -ball[topIndex].dx;							// Topun geldiði açýyý eksiye çeviriyoruz.
						ball[topIndex].x += ball[topIndex].dx * ball[topIndex].speed;	// Topun hýzýný tekrar belirliyoruz.
						ball[topIndex].y += ball[topIndex].dy * ball[topIndex].speed;
					}

				}

				if (bricks[z].lives <= 0 && bricks[z].x1 != -2000)						// Toplarýn sýfýrladýðý bloklarý yok ettiðimiz kýsým.
				{
					bricks[z].x1 = -2000;
					bricks[z].x2 = -2000;												// Bloklarý silmek yerine daha basit yol olan uzaya gönderme
					bricks[z].y1 = -2000;												// iþlemi ile ekrandan bloklarý kaldýrýyoruz.
					bricks[z].y2 = -2000;
					bricks[z].centerX += -2000;
					bricks[z].centerY += -2000;

					gameOver++;
				}
			}

		}

		//Her levelde bloklar 1 sýra aþaðýya inmektedir. 
		if (levelComplete) {
			for (int i = 0; i < brickCount; i++)
			{
				bricks[i].y1 += 51;												// 1 sýra aþaðýya indirmek için +50 ekliyoruz (sadece y1 ve y2)
				bricks[i].y2 += 51;
				bricks[i].centerX = (bricks[i].x1 + bricks[i].x2) / 2;
				bricks[i].centerY = (bricks[i].y1 + bricks[i].y2) / 2;

			}


			levelComplete = false;												// Yeni level baþlangýcý için False yaptýk.
		}
		for (int i = 0; i < brickCount; i++)
		{
			if (bricks[i].y2 >= heigth - gameZone) {				//Eðer bloklar yere çarparsa oyunu kaybetme komutu.

				success = false;
				mainLoop = false;

			}
		}


		if (heart == 0) {										// Eðer can biterse oyunu kaybetme komutu.

			success = false;
			mainLoop = false;


		}
		if (gameOver == brickCount)								//Eðer tüm bloklar silinmiþse oyunu kazanma komutu.
		{
			success = true;
			mainLoop = false;

		}



		al_clear_to_color(al_map_rgb(0, 0, 0));


	}

	while (true)
	{
		if (success)		// Ekrana SUCCESFUL yazdýrma komutu
		{
			al_clear_to_color(black);
			al_draw_text(font25, diamond, width / 2, heigth / 2, ALLEGRO_ALIGN_CENTRE, " SUCCESFUL! "); al_flip_display();
		}
		else				// Ekrana Game Over yazdýrma komutu
		{
			al_clear_to_color(black);
			al_draw_text(font25, diamond, width / 2, heigth / 2 - 25, ALLEGRO_ALIGN_CENTRE, " GAME OVER! "); al_flip_display();
			al_draw_text(font25, diamond, width / 2, heigth / 2 + 25, ALLEGRO_ALIGN_CENTRE, "PRESS ENTER FOR TRY AGAIN "); al_flip_display();
		}

		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		if (events.keyboard.keycode == ALLEGRO_KEY_ENTER)
		{
			main();			// Eðer oyuncu oyuna tekrar baþlamak isterse Enter tuþu ile tekrar baþlayabilme eventi.
		}

	}
	al_destroy_bitmap(bgImage);
	al_destroy_display(display);

	return 0;
}

