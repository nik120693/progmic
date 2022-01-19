/* 
 * **********************************************************
 * Alternative definition for Multiple vector mode Interrupt with respect to INTEnableSystemMultiVectoredInt() used if plib.h are included
 * call the MACRO "macro_enable_interrupts()"
 * **********************************************************
 */
#define macro_enable_interrupts()\
{  unsigned int val = 0;\
   asm volatile("mfc0 %0,$13":"=r"(val));\ 
   val |= 0x00800000;\
   asm volatile("mtc0 %0,$13" : "+r"(val));\
   INTCONbits.MVEC = 1;\
   __builtin_enable_interrupts();}
   