// schnitte.cpp : Defines the entry point for the console application.
//


#ifndef COMMON_INC
#include "common.h"
#endif

#include "capture.h"
#include<conio.h>
#include<SDL.h>

long hr = 0;
int samnum=2048;
int main(int argc, char* argv[]) {
	

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("SDL2 line drawing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 960, 0);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

	printf("S' geht los\n");
	printf("\n\n");
	capture test=capture();
	snippet* t = new snippet(samnum, (WAVEFORMATEXTENSIBLE*)test.format);
	kiss_fft_cfg config=kiss_fft_alloc(samnum,0,NULL,NULL);
	test.printFotmat();
	kiss_fft_cpx* transf = new kiss_fft_cpx[samnum];

	

	char c = 0;
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		//SDL_Delay(10);
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
			// TODO input handling code goes here
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		test.getSnippet(t);
		//t->print();
		t->getInts();
		float freq=0;
		kiss_fft(config,t->idata,transf);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		int wc = 0;
		int width, height;
		SDL_GetWindowSize(window, &width, &height);
		for (int i = samnum -1;i > (samnum -200);i--,wc++) {
			float freq =((samnum -i*1.0) / samnum)*test.format->nSamplesPerSec;
			float euc = sqrt(pow(transf[i].r,2)+ pow(transf[i].i, 2));
			int w = 5;
			int h = 5;
			SDL_Rect rect;
			rect.x = height-w*wc;
			rect.y = 0;
			rect.w = w-1;
			rect.h = -(euc * h);
			SDL_RenderDrawLine(renderer,0,wc*w,euc,wc*w);
			//SDL_RenderDrawRect(renderer, &rect);
			
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

