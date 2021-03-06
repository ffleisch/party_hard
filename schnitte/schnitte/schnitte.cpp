// schnitte.cpp : Defines the entry point for the console application.
//


#ifndef COMMON_INC

#include "common.h"
#include "arduinoSender.h"
#endif

#include "capture.h"
//#include "capturePar.h"
#include "ColCalc.h"
#include<conio.h>
#include<SDL.h>


long hr = 0;
int samnum=1024;

double fps = 0;
bool image_en = true;

std::chrono::high_resolution_clock::time_point t1;
std::chrono::high_resolution_clock::time_point t2;
int main(int argc, char* argv[]) {


	flAvg fpsAvg(1,0.07);
	t1 = std::chrono::high_resolution_clock::now();


	//arduinoSender* Ard = new arduinoSender();


	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("SDL2 line drawing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920,1080, 0);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	
	//SDL_MaximizeWindow(window);

	//SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	printf("S' geht los\n");
	printf("\n\n");
	
	//capture test=capture();
	capture test = capture();
	snippet* t = new snippet(samnum, (WAVEFORMATEXTENSIBLE*)test.format);
	//test.printFotmat();

	ColCalcBounds ColGen(t);

	int count = 0;
	
	bool quit = false;

	SDL_Event event;
	while (!quit) {

		t2 = t1;
		t1=std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t2);

		count++;
		//SDL_Delay(10);
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
			// TODO input handling code goes here
		}


		test.getSnippet(t);
		//t->print();
		//t->getInts();//moved to capture.cpp
		t->transform();

		int rgb[3];
		int lim[3];
		int avgs[3];

		ColGen.CalcRGB(rgb);
		
		//Ard->sendRGB(rgb);
		if (image_en) {

			SDL_SetRenderDrawColor(renderer, rgb[0], rgb[1], rgb[2], 255);
			SDL_RenderClear(renderer);

			float freq = 0;
			int wc = 0;
			int width, height;

			SDL_GetWindowSize(window, &width, &height);
			for (int i = samnum - 1; i > 0; i--, wc++) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

				/*if (wc > lim[1]) {
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				}
				if (wc > lim[2]) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				}*/
				float euc = sqrt(pow(t->freq[0][i].r, 2) + pow(t->freq[0][i].i, 2));
				int w = 3;
				int h = 5;
				SDL_Rect rect;
				rect.x = height - w * wc;
				rect.y = 0;
				rect.w = w - 1;
				rect.h = (int)-(euc * h);
				SDL_RenderDrawLine(renderer, wc*w,(int) height - euc, wc*w, height);
				//SDL_RenderDrawRect(renderer, &rect);

				//float freq =((samnum -i*1.0) / samnum)*test.format->nSamplesPerSec;
				//printf("freq %.2f:%*c<\n",freq, (int)(euc), ' ');
				//printf("%.3f %f  %f\n",freq, transf[i].r,transf[i].i);

			}

			for (int i = 0; i < 3; i++) {
				SDL_SetRenderDrawColor(renderer, 255 * (i == 0), 255 * (i == 1), 255 * (i == 2), 255);
				SDL_RenderDrawLine(renderer, 0, height - avgs[i], width, height - avgs[i]);
			}
		}
		//fps anzeigen
		fps = 1 / time_span.count();
		fpsAvg.add(&fps);
		printf("FPS:%.3lf\n", fpsAvg.val[0]);
		if (image_en) {
			SDL_RenderPresent(renderer);

		}
		
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 42;
}

