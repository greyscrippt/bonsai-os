#define VGA_CTRL_REGISTER   0x3D4
#define VGA_DATA_REGISTER   0x3D5
#define VGA_OFFSET_LOW      0x0F
#define VGA_OFFSET_HIGH     0x0E

#define VGA_ADDR            0xB8000
#define VGA_TEXT_MODE_ADDR  0xA000
#define VGA_ROWS            25
#define VGA_COLS            80

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

uint8_t  cursor_x = 0;
uint8_t  cursor_y = 0;
uint8_t  bg_color = VGA_BLACK;
uint8_t  txt_colr = VGA_GREEN;

unsigned char port_byte_in(unsigned short port) {
  unsigned char result;
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
  __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

// +-------------------------------------------------------------------------+
// | Copies 1 byte of memory of size count from address src to address dest. |
// +-------------------------------------------------------------------------+
// @params dest   Destination address.
// @params src    Source address.
// @params count  Number of bytes to copy.
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

void set_bg_color(uint8_t color) {
  bg_color = color;
}

void set_txt_colr(uint8_t color) {
  txt_colr = color;
}

int str_len(char* str) {
  int i;
  for(i=0 ; str[i] != '\0' ; i++);
  return i;
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outbyte(0x3D4, 0x0A);
	outbyte(0x3D5, (inbyte(0x3D5) & 0xC0) | cursor_start);
 
	outbyte(0x3D4, 0x0B);
	outbyte(0x3D5, (inbyte(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor()
{
	outbyte(0x3D4, 0x0A);
	outbyte(0x3D5, 0x20);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_COLS + x;
 
	outbyte(0x3D4, 0x0F);
	outbyte(0x3D5, (uint8_t) (pos & 0xFF));
	outbyte(0x3D4, 0x0E);
	outbyte(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outbyte(0x3D4, 0x0F);
    pos |= inbyte(0x3D5);
    outbyte(0x3D4, 0x0E);
    pos |= ((uint16_t)inbyte(0x3D5)) << 8;
    return pos;
}

void setchar(unsigned char c, unsigned char forecolour, unsigned char backcolour, int x, int y)
{
     uint16_t attrib = (backcolour << 4) | (forecolour & 0x0F);
     volatile uint16_t * offset;
     offset = (volatile uint16_t *)VGA_ADDR + (y * 80 + x) ;
     *offset = c | (attrib << 8);
}

void print(char* str) {
  for(int i=0 ; i<=str_len(str) ; i++) {
    setchar(str[i], txt_colr, bg_color, cursor_x, cursor_y);

    cursor_x++;

    if(cursor_x > VGA_COLS) {
      cursor_x = 0;
      cursor_y++;
    }

    if(cursor_y == VGA_ROWS) {
      cursor_y = 0;
      cursor_x = 0;
    }
  }
}

void println(char* str) {
  print(str);
  
  cursor_y++;
  cursor_x = 0;
  
  if(cursor_y == VGA_ROWS)
    cursor_y = 0;
}

void main(void) {
  disable_cursor();
  
  while(1) {
    println("Hello");
  }
}
