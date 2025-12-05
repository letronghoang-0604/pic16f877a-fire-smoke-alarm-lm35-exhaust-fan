#include <16f877A.h>

#define LCD_ENABLE_PIN PIN_b2
#define LCD_RS_PIN PIN_b4
#define LCD_RW_PIN PIN_b3

#define LCD_DATA4 PIN_D7
#define LCD_DATA5 PIN_D6
#define LCD_DATA6 PIN_D5
#define LCD_DATA7 PIN_D4

#include <lcd.c>        // thu vien chinh cua lcd trong phan mem ccs
 
#define     mq2         pin_a1
#define     relay       pin_c6
#define     loa         pin_D2

void doc_lm35();
unsigned int8 i;
unsigned int32 kq0,nd;



void doc_lm35()
{
   kq0=0;              
   for(i=0;i<100;i++)
   {
      kq0 = kq0 + READ_ADC();      
      delay_ms(1);
   }
   kq0 = kq0/100;            
   kq0=kq0*5000/1023/10;     
   nd=kq0;
   lcd_gotoxy(1,1);
   lcd_putc("NHIET DO:");
   lcd_putc(nd/10+0x30);         
   lcd_putc(nd%10+0x30);        
   lcd_putc(0xdf);         
   lcd_putc("C    ");
}


void main()
{
   SET_TRIS_A(0Xff);      
   SET_TRIS_e(0Xff);
   SET_TRIS_d(0X00);
   set_tris_b(0x00);   
   set_tris_c(0x80);         
   
   lcd_init();             
   output_low(loa);
   output_low(relay);
   
   SETUP_ADC(ADC_CLOCK_DIV_32);
   SETUP_ADC_PORTS(AN0);
   SET_ADC_CHANNEL(0);
  
   delay_ms(20);

   while(true)
   {    
      doc_lm35();
      if((nd>40)&&(input(mq2)==0))        
      {
         output_high(loa);
         output_high(relay);
         lcd_gotoxy(1,2);
         lcd_putc("QUA NHIET-KHI GA");

      }
      else if(nd>40)      
      {
         output_high(loa);
         output_high(relay);
         lcd_gotoxy(1,2);
         lcd_putc("    QUA NHIET    ");

      }
      else if(input(mq2)==0)    
      {
         output_high(loa);
         output_high(relay);
         lcd_gotoxy(1,2);
         lcd_putc("     KHI GA     ");
 
      }
      else           
      {
         output_low(loa);
         output_low(relay);
         lcd_gotoxy(1,2);
         lcd_putc("  BINH THUONG   ");
      }
   }
}
/***************************************************************/
