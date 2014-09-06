#include "myLib.h"
#include "LIFE.h"
#include "CURSOR.h"
#include "PULSAR.h"
#include "EARTH.h"
#include "WINTER.h"
#include "GOSPER.h"
#include "METROID.h"
#include <stdlib.h>
#include <debugging.h>

char numNeighbors;

char livePixels(char* pixels){
	numNeighbors = 0;
	numNeighbors += pixels[0];
	numNeighbors += pixels[1];
	numNeighbors += pixels[2];
	numNeighbors += pixels[3];
	numNeighbors += pixels[5];
	numNeighbors += pixels[6];
	numNeighbors += pixels[7];
	numNeighbors += pixels[8];
	return numNeighbors;
}

char liveNeighbors(char x, char y, char odd){
	numNeighbors = 0;
	if(odd){
		//Up
		if(y != 0 && (videoBuffer[(y-1)*120+x] & 0xFF) >= 1) numNeighbors++;
		//Left
		if((videoBuffer[(y)*120+(x)] & 0xFF00) >= 1) numNeighbors++;
		//Right
		if(x != 119 && (videoBuffer[(y)*120+(x+1)] & 0xFF00) >= 1) numNeighbors++;
		//Down
		if(y != 159 && (videoBuffer[(y+1)*120+x] & 0xFF) >= 1) numNeighbors++;
		//UpLeft
		if(y != 0 && (videoBuffer[(y-1)*120+x] & 0xFF00) >= 1) numNeighbors++;
		//UpRight
		if(y != 0 && x != 119 && (videoBuffer[(y-1)*120+(x+1)] & 0xFF00) >= 1) numNeighbors++;
		//DownLeft
		if(y != 159 && (videoBuffer[(y+1)*120+x] & 0xFF00) >= 1) numNeighbors++;
		//DownRight
		if(y != 159 && x != 119 && (videoBuffer[(y+1)*120+(x+1)] & 0xFF00) >= 1) numNeighbors++;
	}else{
		//Up
		if(y != 0 && (videoBuffer[(y-1)*120+x] & 0xFF00) >= 1) numNeighbors++;
		//Left
		if(x != 0 && (videoBuffer[(y)*120+(x-1)] & 0xFF) >= 1) numNeighbors++;
		//Right
		if((videoBuffer[(y)*120+x] & 0xFF) >= 1) numNeighbors++;
		//Down
		if(y != 159 && (videoBuffer[(y+1)*120+x] & 0xFF00) >= 1) numNeighbors++;
		//UpLeft
		if(y != 0 && x != 0 && (videoBuffer[(y-1)*120+(x-1)] & 0xFF) >= 1) numNeighbors++;
		//UpRight
		if(y != 0 && (videoBuffer[(y-1)*120+x] & 0xFF) >= 1) numNeighbors++;
		//DownLeft
		if(y != 159 && x != 0 && (videoBuffer[(y+1)*120+(x-1)] & 0xFF) >= 1) numNeighbors++;
		//DownRight
		if(y != 159 && (videoBuffer[(y+1)*120+x] & 0xFF) >= 1) numNeighbors++;
	}
	return numNeighbors;
}



int main(){
	REG_DISPCNT = BG2_ENABLE | MODE4 | OBJ_ENABLE | SPR_MAP_1D;

	
	//Intro screen, count frames for a while
	//Setup palette
	dma_cpy(background_palette, LIFE_palette, 2, 3);
	dma_cpy(sprite_palette, CURSOR_palette, 2, 3);

	//Draw the image
	dma_cpy(backBuffer, LIFE, 19200, 3);
	dma_cpy(OVRAM_BNK2, CURSOR, 96, 3);

	flipPage();
	int frameCount = 0;
	while((~(REG_BUTTONS) & BTN_STRT) == 0){
		frameCount++;
	}

	//Use those frames as a seed for the game of life
	srand(frameCount);
	for(int i = 0; i < 160; i++){
		for(int k = 0; k < 120; k++){
			backBuffer[i*120+k] = ((rand()%2)<<8)|(rand()%2);
		}
	}
	flipPage();

	short pixel1;
	short pixel2;
	char livePixels1;
	char livePixels2;

	char cursorAnim[4] = {2, 0, 4, 0};
	//Y position 0, 256 color mode, wide sprite
	u16 ATTR_0 = 30 | SPRITE_RENDER_NORMAL | SPRITE_COLORMODE_8BPP | SPRITE_SHAPE_SQUARE;
	//X position of 25, Largest sprite (for a 64x32 sprite)
	u16 ATTR_1 = 25 | SPRITE_SIZE_SMALL;
	//Tile index of 512(The beginning of charblock 2), no other flags to set
	u16 ATTR_2 = 512;

	short cursorY = 0;
	short cursorX = 0;

	//while(1);
	char pixelMatrix[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	short pointerIndex = 0;
	const void* imagePointers[5] = {GOSPER, WINTER, PULSAR, EARTH, METROID};
	u16 previousREG_BUTTONS = 0xFFFF;
	short newPixel = 0;
	char editMode = 0;
	while(1){
		background_palette[1] = 0x7FFF;
		while(editMode == 0){
			for(int i = 0; i < 160; i++){

				if(editMode){
					break;
				}

				pixelMatrix[0] = 0;
				pixelMatrix[1] = 0;
				pixelMatrix[2] = 0;
				pixelMatrix[3] = 0;
				pixelMatrix[4] = 0;
				pixelMatrix[5] = 0;
				pixelMatrix[6] = 0;
				pixelMatrix[7] = 0;
				pixelMatrix[8] = 0;

				//Build a 3x3 matrix array of pixel data, not addresses.
				//Then iterate horizontally by removing the first row, and adding the next row.
				//The matrix follows the pattern [0] Top-Left [1] Left [2] Bottom-Left ...
				//Possible optimizations for this?
			
				//If we're not on the top row, build the top row.
				if(i!=0){
					//pixelMatrix[0] = (videoBuffer[(i-1)*120+(-1)] & 0xFF); Nothing on the left will lever be nonzero here
					pixelMatrix[3] = (videoBuffer[(i-1)*120] & 0xFF);
					pixelMatrix[6] = (videoBuffer[(i-1)*120] & 0xFF00)>>8;
				}

				//Build the middle
				//pixelMatrix[1] = (videoBuffer[(i)*120+(-1)] & 0xFF);
				pixelMatrix[4] = (videoBuffer[(i)*120] & 0xFF);
				pixelMatrix[7] = (videoBuffer[(i)*120] & 0xFF00)>>8;

				if(i!=159){
					//pixelMatrix[2] = (videoBuffer[(i+1)*120+(-1)] & 0xFF);
					pixelMatrix[5] = (videoBuffer[(i+1)*120] & 0xFF);
					pixelMatrix[8] = (videoBuffer[(i+1)*120] & 0xFF00)>>8;
				}
			
				newPixel = 0;
				for(int k = 0; k < 120; k++){
					pixel1 = videoBuffer[(i)*120+(k)] & 0xFF;
					pixel2 = videoBuffer[(i)*120+(k)] & 0xFF00;

					livePixels1 = livePixels(pixelMatrix);
					//Shift the matrix to the right by one
					pixelMatrix[0] = pixelMatrix[3];
					pixelMatrix[1] = pixelMatrix[4];
					pixelMatrix[2] = pixelMatrix[5];
					pixelMatrix[3] = pixelMatrix[6];
					pixelMatrix[4] = pixelMatrix[7];
					pixelMatrix[5] = pixelMatrix[8];
					if(k==119){
						pixelMatrix[6] = 0;
						pixelMatrix[7] = 0;
						pixelMatrix[8] = 0;
					}else{
						if(i!=0){
							pixelMatrix[6] = (videoBuffer[(i-1)*120+(k+1)] & 0xFF);
						}
						pixelMatrix[7] = (videoBuffer[(i)*120+(k+1)] & 0xFF);
						if(i!=159){
							pixelMatrix[8] = (videoBuffer[(i+1)*120+(k+1)] & 0xFF);
						}
					}

					livePixels2 = livePixels(pixelMatrix);
					//Shift the matrix to the right by one
					pixelMatrix[0] = pixelMatrix[3];
					pixelMatrix[1] = pixelMatrix[4];
					pixelMatrix[2] = pixelMatrix[5];
					pixelMatrix[3] = pixelMatrix[6];
					pixelMatrix[4] = pixelMatrix[7];
					pixelMatrix[5] = pixelMatrix[8];
					if(k==119){
						pixelMatrix[6] = 0;
						pixelMatrix[7] = 0;
						pixelMatrix[8] = 0;
					}else{
						if(i!=0){
							pixelMatrix[6] = (videoBuffer[(i-1)*120+(k+1)] & 0xFF00)>>8;
						}
						pixelMatrix[7] = (videoBuffer[(i)*120+(k+1)] & 0xFF00)>>8;
						if(i!=159){
							pixelMatrix[8] = (videoBuffer[(i+1)*120+(k+1)] & 0xFF00)>>8;
						}
					}

					if(pixel1 >= 1){
						if(livePixels1 <= 1 || livePixels1 > 3){
							newPixel = 0;
						}else{
							newPixel = 1;
						}
					}else{
						if(livePixels1 == 3){
							newPixel = 1;
						}else{
							newPixel = 0;
						}
					}

					if(pixel2 >= 1){
						if(livePixels2 <= 1 || livePixels2 > 3){
							newPixel = newPixel | 0<<8;
						}else{
							newPixel = newPixel | 1<<8;
						}
					}else{
						if(livePixels2 == 3){
							newPixel = newPixel | 1<<8;
						}else{
							newPixel = newPixel | 0<<8;
						}
					}
					backBuffer[i*120+k] = newPixel;

					//Check to see if it's time to go to edit mode
					if((~(REG_BUTTONS) & BTN_SELC)){
						editMode = 1;
						break;
					}
				}
			}
			if(editMode == 0){
				vid_vsync();
				flipPage();
			}
		}

		while((~(REG_BUTTONS) & BTN_SELC)){
			//Wait for the user to let go of the select button
		}
		//Make the cursor visible, and set it's location to the center of the screen.
		
		//Y position 0, 256 color mode, wide sprite
		ATTR_0 = 30 | SPRITE_RENDER_NORMAL | SPRITE_COLORMODE_8BPP | SPRITE_SHAPE_SQUARE;
		//X position of 25, Largest sprite (for a 64x32 sprite)
		ATTR_1 = 25 | SPRITE_SIZE_SMALL;
		//Tile index of 512(The beginning of charblock 2), no other flags to set
		ATTR_2 = 512;

		obj_attr_mem[0] = ATTR_0;
		obj_attr_mem[1] = ATTR_1;
		obj_attr_mem[2] = ATTR_2;

		background_palette[1] = RGB(8, 8, 8);

		//We're in edit mode
		while(editMode){
			frameCount++;
			sprite_palette[1] = (rand()&0xFFFF);
			vid_vsync();

			if((~(REG_BUTTONS) & BTN_LEFT)){
				cursorX -= 1;
				if(cursorX < -3){
					cursorX = -3;
				}
			}
			if((~(REG_BUTTONS) & BTN_RGHT)){
				cursorX += 1;
				if(cursorX > 236){
					cursorX = 236;
				}
			}
			if((~(REG_BUTTONS) & BTN_UP)){
				cursorY -= 1;
				if(cursorY < -3){
					cursorY = -3;
				}
			}
			if((~(REG_BUTTONS) & BTN_DOWN)){
				cursorY += 1;
				if(cursorY > 156){
					cursorY = 156;
				}
			}
			if((~(REG_BUTTONS) & BTN_A)){
				//Even pixel, or odd pixel?
				char odd = (cursorX & 1);
				if(odd){
					videoBuffer[(cursorY+3)*120+((cursorX+3)>>1)] = (videoBuffer[(cursorY+3)*120+((cursorX+3)>>1)] & 0xFF00) | 1;
				}else{
					videoBuffer[(cursorY+3)*120+((cursorX+3)>>1)] = (videoBuffer[(cursorY+3)*120+((cursorX+3)>>1)] & 0xFF) | (1<<8);
				}
			}
			if((~(REG_BUTTONS) & BTN_B)){
				//Even pixel, or odd pixel?
				char odd = (cursorX & 1);
				if(odd){
					videoBuffer[(cursorY+3)*120+((cursorX+3)>>1)] = (videoBuffer[(cursorY+3)*120+((cursorX+3)>>1)] & 0xFF00);
				}else{
					videoBuffer[(cursorY+3)*120+((cursorX+3)>>1)] = (videoBuffer[(cursorY+3)*120+((cursorX+3)>>1)] & 0xFF);
				}
			}
			if((~(REG_BUTTONS) & BTN_STRT) && (~(previousREG_BUTTONS) & BTN_STRT)==0){
				volatile int black = 0;
				REG_DMA[3].src = &black;
				REG_DMA[3].dst = backBuffer;
				REG_DMA[3].cnt = 9600 | DMA_ON | DMA_32 | DMA_SOURCE_FIXED;
				vid_vsync();
				flipPage();
			}
			if((~(REG_BUTTONS) & BTN_R) && (~(previousREG_BUTTONS) & BTN_R)==0){
				vid_vsync();
				flipPage();
			}
			if((~(REG_BUTTONS) & BTN_L) && (~(previousREG_BUTTONS) & BTN_L)==0){
				dma_cpy(backBuffer, imagePointers[pointerIndex], 19200, 3);
				pointerIndex++;
				if(pointerIndex > 4){
					pointerIndex = 0;
				}
				vid_vsync();
				flipPage();
			}
			if((~(REG_BUTTONS) & BTN_SELC) && (~(previousREG_BUTTONS) & BTN_SELC)==0){
				editMode = 0;
				ATTR_0 = SPRITE_RENDER_HIDDEN;
			}
			if(frameCount > 15){
				frameCount = 0;
			}
			obj_attr_mem[0] = (ATTR_0 & 0xFF00) | (cursorY&0xFF);
			obj_attr_mem[1] = (ATTR_1 & 0xFE00) | (cursorX&0x01FF);
			obj_attr_mem[2] = 512 + cursorAnim[frameCount>>2];
			previousREG_BUTTONS = REG_BUTTONS;
		}
		while((~(REG_BUTTONS) & BTN_SELC)){
			//Wait for the user to let go of the select button
		}
	}
	//Flip that onto the screen
	//Clear the back buffer
	//Simulate the next tick using the front buffer

	//Figure out the edit mode later on.

	//EDIT MODE TIME
	//Controls:
	//Arrows move the cursor* Done
	//R calls flipPage() which is basically an undo for clearing the screen* Done
	//L cycles through pre-made test cases* Done
	//A sets the pixel under the cursor ON* Done
	//B sets the pixel under the cursor OFF* Done
	//Select EXITS Edit Mode* Done
	//Start Clears the screen* Done
}
