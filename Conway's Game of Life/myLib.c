#include "myLib.h"

u16* videoBuffer = (u16*)0x6000000;
u16* obj_attr_mem = (u16*)0x7000000;
u16* background_palette = (u16*)0x5000000;
u16* sprite_palette = (u16*)0x5000200;
u16* backBuffer = MODE4_BUFFER_1;

void setPixel(int x, int y, u16 color){
	videoBuffer[(y*240)+x] = color;
}

void drawRect(int x, int y, int width, int height, u16 color){
	//Draw horizontal lines until it's done
	for(int i = 0; i <= height; i++){
		for(int j = 0; j <= width; j++){
			videoBuffer[((y+i)*240)+(x+j)] = color;
		}
	}	
}

void drawHollowRect(int x, int y, int width, int height, u16 color){
	for(int i = 0; i <= width; i++){
		videoBuffer[(y*240) + (x+i)] = color;
		videoBuffer[((y+height)*240) + (x+i)] = color;	
	}
	for(int i = 0; i <= height; i++){
		videoBuffer[((y+i)*240) + (x)] = color;
		videoBuffer[((y+i)*240) + (x+width)] = color;	
	}
}

void flipPage(){
	if(videoBuffer == MODE4_BUFFER_0){
		videoBuffer = MODE4_BUFFER_1;
		backBuffer = MODE4_BUFFER_0;
	}else{
		videoBuffer = MODE4_BUFFER_0;
		backBuffer = MODE4_BUFFER_1;
	}
	REG_DISPCNT ^= PAGE_FLIP;
}

void dma_cpy(const volatile void *dst, const volatile void *src, u32 count, u16 ch){
	REG_DMA[ch].cnt = 0;
	REG_DMA[ch].src = (u32*)src;
	REG_DMA[ch].dst = (u32*)dst;
	REG_DMA[ch].cnt = count | (1<<0x1f);
}

void dma_cpy32(const volatile void *dst, const volatile void *src, u32 count, u16 ch){
	REG_DMA[ch].cnt = 0;
	REG_DMA[ch].src = (u32*)src;
	REG_DMA[ch].dst = (u32*)dst;
	REG_DMA[ch].cnt = count | (1<<0x1f) | (1<<0x1A);
}

void vid_vsync(){
	while(REG_VCOUNT >= 160){};
	while(REG_VCOUNT < 160){};	
}
