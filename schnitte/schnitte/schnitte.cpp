// schnitte.cpp : Defines the entry point for the console application.
//


#ifndef COMMON_INC
#include "common.h"
#endif

#include "capture.h"
#include<conio.h>
#include<SDL.h>



long hr = 0;
int samnum=1024;
int main(int argc, char* argv[]) {
	

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("SDL2 line drawing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 500, 0);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	//SDL_MaximizeWindow(window);

	SDL_SetWindowResizable(window, SDL_TRUE);

	printf("S' geht los\n");
	printf("\n\n");
	capture test=capture();
	snippet* t = new snippet(samnum, (WAVEFORMATEXTENSIBLE*)test.format);
	//test.printFotmat();

	
	int count = 0;
	bool quit = false;
	SDL_Event event;
	while (!quit) {
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
		t->getInts();
		t->transform();

		int rgb[3];
		int lim[3];
		t->getCol1(rgb,lim);
		SDL_SetRenderDrawColor(renderer, rgb[0], rgb[1], rgb[2], 255);
		SDL_RenderClear(renderer);

		float freq=0;
		int wc = 0;
		int width, height;
		SDL_GetWindowSize(window, &width, &height);
		for (int i = samnum -1;i >0;i--,wc++) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

			if (wc > lim[1]) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			if (wc > lim[2]) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

			}

			float euc = sqrt(pow(t->freq[0][i].r,2)+ pow(t->freq[0][i].i, 2));
			int w = 3;
			int h = 5;
			SDL_Rect rect;
			rect.x = height-w*wc;
			rect.y = 0;
			rect.w = w-1;
			rect.h = -(euc * h);
			SDL_RenderDrawLine(renderer,wc*w,height-euc,wc*w,height);
			//SDL_RenderDrawRect(renderer, &rect);

			//float freq =((samnum -i*1.0) / samnum)*test.format->nSamplesPerSec;
			//printf("freq %.2f:%*c<\n",freq, (int)(euc), ' ');
			//printf("%.3f %f  %f\n",freq, transf[i].r,transf[i].i);
			
		}
		SDL_RenderPresent(renderer);

		
		//c=_getch();
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 42;
}

