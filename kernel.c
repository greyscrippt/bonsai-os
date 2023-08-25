#define VGA_ADDR            0xb8000
#define VGA_ROWS            0x19
#define VGA_COLS            0x50

#define VGA_BLACK           0x0
#define VGA_BLUE            0x1
#define VGA_GREEN           0x2
#define VGA_CYAN            0x3
#define VGA_RED             0x4
#define VGA_MAGENTA         0x5
#define VGA_BROWN           0x6
#define VGA_LIGHTGREY       0x7
#define VGA_DARKGREY        0x8
#define VGA_LIGHTBLUE       0x9
#define VGA_LIGHTGREEN      0xA
#define VGA_LIGHTCYAN       0xB
#define VGA_LIGHTRED        0xC
#define VGA_LIGHTMAGENTA    0xD
#define VGA_YELLOW          0xE
#define VGA_WHITE           0xF

typedef signed char     int8_t;
typedef short           int16_t;
typedef long            int32_t;
typedef long long       int64_t;

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long   uint32_t;
typedef unsigned long   long uint64_t;

typedef unsigned char*  string;

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;



// Copy data of size count from address src to address dest.
uint8_t *mem_cpy(uint8_t* dest, const uint8_t* src, int count) {
  int i;
  for(i=0 ; i<count ; i++)
    dest[i] = src[i];
  return dest;
}

// Sets the values of address 'dest' into 'val' for 'count' times
uint8_t* mem_set(uint8_t* dest, uint8_t val, int count) {
  int i;
  for(i=0 ; i<count ; i++)
    dest[i] = val;
  return dest;
}

// Reads value from I/O port 'port'
uint8_t inbyte (uint16_t port) {
  uint8_t ret;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "d" (port));
  return ret;
}

// Writes value 'data' to I/O port 'port'
void outbyte(uint16_t port, uint8_t data) {
  __asm__ __volatile__ ("outb %1, %0" : : "d" (port), "a" (data));
}

// Sets a character on 'offset'.
void set_ch(char ch, int offset, uint8_t bg_color, uint8_t txt_color) {
  uint8_t* vidmem     = (uint8_t*) VGA_ADDR;
  vidmem[offset]      = ch;
  vidmem[offset + 1]  = (uint16_t) bg_color | txt_color;
}

void main(void) {
  while(1);
}
