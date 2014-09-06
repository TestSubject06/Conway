#ifndef MYLIB_H
#define MYLIB_H
typedef unsigned short u16;
typedef unsigned int   u32;

#define RGB(r,g,b) ((r) | ((g)<<5) | ((b)<<10))
#define REG_DISPCNT 	*(u16*)0x4000000
#define MODE3 		3
#define MODE4		4
#define BG2_ENABLE 	(1<<10)
#define OBJ_ENABLE	(1<<12)
#define SPR_MAP_1D	(1<<6)
#define REG_VCOUNT 	*(volatile unsigned short*)0x4000006
#define OVRAM_BNK2 	(u32*)0x6014000
#define MODE4_BUFFER_0 (u16*)0x6000000
#define MODE4_BUFFER_1 (u16*)0x600A000
#define PAGE_FLIP	(1<<4)

typedef struct DMA_REC{
	const volatile void *src;
	const volatile void *dst;
	u32 cnt;
} DMA_REC;

#define REG_DMA ((volatile DMA_REC*)0x40000B0)
#define DMA_ON 1<<0x1f
#define DMA_32 1<<0x1A
#define DMA_SOURCE_FIXED 2<<0x17

#define REG_BUTTONS	*(volatile unsigned short*) 0x4000130
#define BTN_A 		1
#define BTN_B 		(1<<1)
#define BTN_SELC 	(1<<2)
#define BTN_STRT 	(1<<3)
#define BTN_RGHT 	(1<<4)
#define BTN_LEFT 	(1<<5)
#define BTN_UP 		(1<<6)
#define BTN_DOWN 	(1<<7)
#define BTN_R 		(1<<8)
#define BTN_L 		(1<<9)

#define SPRITE_RENDER_NORMAL 	(0<<8)
#define SPRITE_RENDER_AFFINE 	(1<<8)
#define SPRITE_RENDER_HIDDEN 	(2<<8)
#define SPRITE_RENDER_DUBAFFINE (3<<8)

#define SPRITE_COLORMODE_4BPP 	(0<<13)
#define SPRITE_COLORMODE_8BPP 	(1<<13)

#define SPRITE_SHAPE_SQUARE 	(0<<14)
#define SPRITE_SHAPE_WIDE	 	(1<<14)
#define SPRITE_SHAPE_TALL	 	(2<<14)
#define SPRITE_SHAPE_FORBIDDEN 	(3<<14)

#define SPRITE_SIZE_SMALL	 	(0<<14)
#define SPRITE_SIZE_EXTEND	 	(1<<14)
#define SPRITE_SIZE_MEDIUM	 	(2<<14)
#define SPRITE_SIZE_LARGE	 	(3<<14)
//SIZE TABLE	00		01		10		11
//	00			8x8		16x16	32x32	64x64
//	01			16x8	32x8	32x16	64x32
//	10			8x16	8x32	16x32	32x64
//	11			X		X		X		X

#define SPRITE_RENDER_PRIORITY0 	(0<<10)
#define SPRITE_RENDER_PRIORITY1 	(1<<10)
#define SPRITE_RENDER_PRIORITY2 	(2<<10)
#define SPRITE_RENDER_PRIORITY3 	(3<<10)

#define SPRITE_RENDER_PALETTE0	 	(0<<12)
#define SPRITE_RENDER_PALETTE1	 	(1<<12)
#define SPRITE_RENDER_PALETTE2	 	(2<<12)
#define SPRITE_RENDER_PALETTE3	 	(3<<12)
#define SPRITE_RENDER_PALETTE4	 	(4<<12)
#define SPRITE_RENDER_PALETTE5	 	(5<<12)
#define SPRITE_RENDER_PALETTE6	 	(6<<12)
#define SPRITE_RENDER_PALETTE7	 	(7<<12)
#define SPRITE_RENDER_PALETTE8	 	(8<<12)
#define SPRITE_RENDER_PALETTE9	 	(9<<12)
#define SPRITE_RENDER_PALETTE10	 	(10<<12)
#define SPRITE_RENDER_PALETTE11	 	(11<<12)
#define SPRITE_RENDER_PALETTE12	 	(12<<12)
#define SPRITE_RENDER_PALETTE13	 	(13<<12)
#define SPRITE_RENDER_PALETTE14	 	(14<<12)
#define SPRITE_RENDER_PALETTE15	 	(15<<12)

extern u16* videoBuffer;
extern u16* obj_attr_mem;
extern u16* sprite_palette;
extern u16* background_palette;
extern u16* backBuffer;

void setPixel(int x, int y, u16 color);

void drawRect(int x, int y, int width, int height, u16 color);

void drawHollowRect(int x, int y, int width, int height, u16 color);
void dma_cpy(const volatile void *dst, const volatile void *src, u32 count, u16 ch);
void dma_cpy32(const volatile void *dst, const volatile void *src, u32 count, u16 ch);
void flipPage();
void vid_vsync();
#endif
