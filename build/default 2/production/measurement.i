
# 1 "measurement.c"

# 13 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdint.h"
typedef signed char int8_t;

# 20
typedef signed int int16_t;

# 28
typedef __int24 int24_t;

# 36
typedef signed long int int32_t;

# 52
typedef unsigned char uint8_t;

# 58
typedef unsigned int uint16_t;

# 65
typedef __uint24 uint24_t;

# 72
typedef unsigned long int uint32_t;

# 88
typedef signed char int_least8_t;

# 96
typedef signed int int_least16_t;

# 109
typedef __int24 int_least24_t;

# 118
typedef signed long int int_least32_t;

# 136
typedef unsigned char uint_least8_t;

# 143
typedef unsigned int uint_least16_t;

# 154
typedef __uint24 uint_least24_t;

# 162
typedef unsigned long int uint_least32_t;

# 181
typedef signed char int_fast8_t;

# 188
typedef signed int int_fast16_t;

# 200
typedef __int24 int_fast24_t;

# 208
typedef signed long int int_fast32_t;

# 224
typedef unsigned char uint_fast8_t;

# 230
typedef unsigned int uint_fast16_t;

# 240
typedef __uint24 uint_fast24_t;

# 247
typedef unsigned long int uint_fast32_t;

# 268
typedef int32_t intmax_t;

# 282
typedef uint32_t uintmax_t;

# 289
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;

# 15 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdbool.h"
typedef unsigned char bool;

# 19 "mybutton.h"
typedef struct mybutton{
uint8_t signals[2];
uint32_t falling_ts[2];
uint32_t rising_ts[2];

uint8_t single_click_tmps[2];
uint8_t double_click_tmps[2];
uint8_t long_press_tmps[2];

uint8_t single_clicked_flag;
uint8_t double_clicked_flag;
uint8_t long_press_flag;
}MyButton;

void init_button(MyButton *b);
void update_mybutton(MyButton *b, bool signal, uint32_t now);


bool is_single_click(MyButton *b);
bool is_double_click(MyButton *b);
bool is_long_press(MyButton *b);

void reset_single_click_flag(MyButton *b);
void reset_double_click_flag(MyButton *b);
void reset_long_press_flag(MyButton *b);

# 57
bool is_it_time_up(uint32_t *prev_tick, uint32_t ms);
bool is_it_time_over(uint32_t ms);

# 4 "/Applications/microchip/xc8/v2.40/pic/include/__size_t.h"
typedef unsigned size_t;

# 7 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdlib.h"
typedef unsigned short wchar_t;

# 15
typedef struct {
int rem;
int quot;
} div_t;
typedef struct {
unsigned rem;
unsigned quot;
} udiv_t;
typedef struct {
long quot;
long rem;
} ldiv_t;
typedef struct {
unsigned long quot;
unsigned long rem;
} uldiv_t;

# 65
extern double atof(const char *);
extern double strtod(const char *, const char **);
extern int atoi(const char *);
extern unsigned xtoi(const char *);
extern long atol(const char *);

# 73
extern long strtol(const char *, char **, int);

extern int rand(void);
extern void srand(unsigned int);
extern void * calloc(size_t, size_t);
extern div_t div(int numer, int denom);
extern udiv_t udiv(unsigned numer, unsigned denom);
extern ldiv_t ldiv(long numer, long denom);
extern uldiv_t uldiv(unsigned long numer,unsigned long denom);

# 85
extern unsigned long _lrotl(unsigned long value, unsigned int shift);
extern unsigned long _lrotr(unsigned long value, unsigned int shift);
extern unsigned int _rotl(unsigned int value, unsigned int shift);
extern unsigned int _rotr(unsigned int value, unsigned int shift);




extern void * malloc(size_t);
extern void free(void *);
extern void * realloc(void *, size_t);


# 13 "/Applications/microchip/xc8/v2.40/pic/include/c90/xc8debug.h"
#pragma intrinsic(__builtin_software_breakpoint)
extern void __builtin_software_breakpoint(void);

# 104 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdlib.h"
extern int atexit(void (*)(void));
extern char * getenv(const char *);
extern char ** environ;
extern int system(char *);
extern void qsort(void *, size_t, size_t, int (*)(const void *, const void *));
extern void * bsearch(const void *, void *, size_t, size_t, int(*)(const void *, const void *));
extern int abs(int);
extern long labs(long);

extern char * itoa(char * buf, int val, int base);
extern char * utoa(char * buf, unsigned val, int base);




extern char * ltoa(char * buf, long val, int base);
extern char * ultoa(char * buf, unsigned long val, int base);

extern char * ftoa(float f, int * status);

# 20 "monitor_sync.h"
typedef enum {
STATE_MONITORING_POWER_FOR_SYNC,
STATE_DELAY,
STATE_MEASURE,
STATE_DISPATCH,
STATE_CALIBRATE,
STATE_OUTPUT_PWM,
STATE_SAVE_TOP_CALIBRATION_POINT,
STATE_SAVE_BOTTOM_CALIBRATION_POINT,
STATE_SAVE_3RD_CALIBRATION_POINT
} States;

void monitoring_sync(States *state, MyButton *b1, MyButton *b2,
uint8_t *b1_clicked_count, uint8_t *b2_clicked_count);

void dispatch(States *state,
MyButton *b1,
MyButton *b2,
uint8_t *b1_clicked_count_ptr,
uint8_t *b2_clicked_count_ptr);

# 7 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdarg.h"
typedef void * va_list[1];

#pragma intrinsic(__va_start)
extern void * __va_start(void);

#pragma intrinsic(__va_arg)
extern void * __va_arg(void *, ...);

# 43 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdio.h"
struct __prbuf
{
char * ptr;
void (* func)(char);
};

# 29 "/Applications/microchip/xc8/v2.40/pic/include/c90/errno.h"
extern int errno;

# 12 "/Applications/microchip/xc8/v2.40/pic/include/c90/conio.h"
extern void init_uart(void);

extern char getch(void);
extern char getche(void);
extern void putch(char);
extern void ungetch(char);

extern __bit kbhit(void);

# 23
extern char * cgets(char *);
extern void cputs(const char *);

# 88 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdio.h"
extern int cprintf(char *, ...);
#pragma printf_check(cprintf)



extern int _doprnt(struct __prbuf *, const register char *, register va_list);


# 180
#pragma printf_check(vprintf) const
#pragma printf_check(vsprintf) const

extern char * gets(char *);
extern int puts(const char *);
extern int scanf(const char *, ...) __attribute__((unsupported("scanf() is not supported by this compiler")));
extern int sscanf(const char *, const char *, ...) __attribute__((unsupported("sscanf() is not supported by this compiler")));
extern int vprintf(const char *, va_list) __attribute__((unsupported("vprintf() is not supported by this compiler")));
extern int vsprintf(char *, const char *, va_list) __attribute__((unsupported("vsprintf() is not supported by this compiler")));
extern int vscanf(const char *, va_list ap) __attribute__((unsupported("vscanf() is not supported by this compiler")));
extern int vsscanf(const char *, const char *, va_list) __attribute__((unsupported("vsscanf() is not supported by this compiler")));

#pragma printf_check(printf) const
#pragma printf_check(sprintf) const
extern int sprintf(char *, const char *, ...);
extern int printf(const char *, ...);

# 20 "measurement.h"
typedef struct values {
uint16_t ph_value;
uint16_t temp_value;
} Sensor_value;

void measurement(States *state, Sensor_value *value);

# 6 "measurement.c"
void measurement(States *state, Sensor_value *value){

};

