/*   ###################################################################
**     设置系统时钟，从EXTCLK口输出系统总线时钟（bus clock）
**     Copyright : RoRoceline Songar Studio All Rights Reserved.
**     从MCUinit 中备注或去掉备注，设置时钟初始化，基于8MHz晶振
**     mail   : //undefined
** ###################################################################
*/
#include <MC9S12G128.h>                /* I/O map for MC9S12G128MLH */
#include "MCUinit.h"

void CLK_Ini_6_25MHz(void);
void CLK_Ini_12_5MHz(void);
void CLK_Ini_8MHz(void);
void CLK_Ini_24MHz(void);
void LedBlkPin(void);


/*
** ===================================================================
**     Method      :  MCU_init (component MC9S12G128_64)
**     Description :
** ===================================================================
*/       

void MCU_init(void)
  {
    // initial CLK resouce and prediviation
    //       CLK_Ini_6_25MHz();
             CLK_Ini_8MHz() ;
    //        CLK_Ini_12_5MHz();      
    //       CLK_Ini_24MHz(); 
   
  LedBlkPin();
    asm CLI;                           
  } 
   
 /*
** ===================================================================
**     Method      :  LedBlkPin(void)
**     Description : LED port initial
** ===================================================================
*/
 void LedBlkPin(void)
    {
     // SET blink port to PJ0     
       PIEJ = 0x00U;                                      
       PIFJ = 0xFFU;                                      
       DDRJ |= (unsigned char)0x01U;                  
    }

/*
** ===================================================================
**     Method      :  CLK_Ini_6_25MHz(void)
**     Description : Initial CLK with internal reference 
** ===================================================================
*/
void CLK_Ini_6_25MHz(void)
  {
    
         /* CPMUPROT: PROT=0 */
        CPMUPROT = 0x26U;                    /* Disable protection of clock configuration registers */
        /* CPMUCLKS: PSTP=0 */
        CPMUCLKS &= (unsigned char)~(unsigned char)0x40U;                     
        /* CPMUCLKS: PLLSEL=1 */
        CPMUCLKS |= (unsigned char)0x80U;    /* Enable the PLL to allow write to divider registers */
        /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=1,SYNDIV3=1,SYNDIV2=0,SYNDIV1=0,SYNDIV0=0 */
        CPMUSYNR = 0x58U;                    /* Set the multiplier register */
        /* CPMUPOSTDIV: POSTDIV4=0,POSTDIV3=0,POSTDIV2=0,POSTDIV1=1,POSTDIV0=1 */
        CPMUPOSTDIV = 0x03U;                 /* Set the post divider register */
        /* CPMUPLL: FM1=0,FM0=0 */
        CPMUPLL = 0x00U;                     /* Set the PLL frequency modulation */
         while(CPMUFLG_LOCK == 0U) 
          {          /* Wait until the PLL is within the desired tolerance of the target frequency */
          }
        /* CPMUPROT: PROT=0 */
        CPMUPROT = 0x00U;                    /* Enable protection of clock configuration registers */
        /* CPMUCOP: RSBCK=0,WRTMASK=0 */
        CPMUCOP &= (unsigned char)~(unsigned char)0x60U;                     
        /* Common initialization of the CPU registers */
        /* CPMUINT: LOCKIE=0,OSCIE=0 */
        CPMUINT &= (unsigned char)~(unsigned char)0x12U;                     
        /* CPMULVCTL: LVIE=0 */
        CPMULVCTL &= (unsigned char)~(unsigned char)0x02U;                     
        /* IRQCR: IRQEN=0 */
        IRQCR &= (unsigned char)~(unsigned char)0x40U;                     
        /* ### Init_GPIO init code */
  }
 

  
  /*
** ===================================================================
**     Method      :  CLK_Ini_12_5MHz(void)
**     Description : PEI -  12.5MHz Bus CLK, 1MHz Internal Clock 
** ===================================================================
 */
  void CLK_Ini_12_5MHz(void)
    {
            /* CPMUPROT: PROT=0 */
           CPMUPROT = 0x26U;                    /* Disable protection of clock configuration registers */
            /* CPMUCLKS: PSTP=0 */
           CPMUCLKS &= (unsigned char)~(unsigned char)0x40U;                     
            /* CPMUCLKS: PLLSEL=1 */
            CPMUCLKS |= (unsigned char)0x80U;    /* Enable the PLL to allow write to divider registers */
            /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=1,SYNDIV3=1,SYNDIV2=0,SYNDIV1=0,SYNDIV0=0 */   
           CPMUSYNR = 0x58;                /* SYNDIV = 24, VCO frequency 48 - 64 MHz */

           CPMUPOSTDIV = 0x01;             /* POSTDIV = 1 */

           CPMUCLKS_PLLSEL = 1;            /* PLLSEL = 1 */

            while (!CPMUCLKS_PLLSEL);       /* Verify CPMUCLKS configuration */

           while(!CPMUFLG_LOCK);	        /* wait for PLL to lock */
       }

 /*
** ===================================================================
**     Method      :  CLK_Ini_8MHz(void)
**     Description : PBE - 16 MHz Crystal, 4MHz Bus CLK (32MHz VCO, 4MHz PLL) 
** ===================================================================
*/ 

void CLK_Ini_8MHz(void)
  {
    
         /* CPMUPROT: PROT=0 */
        CPMUPROT = 0x26U;                    /* Disable protection of clock configuration registers */
        /* CPMUCLKS: PSTP=0 */
        CPMUCLKS &= (unsigned char)~(unsigned char)0x40U;                     
        /* CPMUCLKS: PLLSEL=1 */
        CPMUCLKS |= (unsigned char)0x80U;    /* Enable the PLL to allow write to divider registers */
        /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=1,SYNDIV3=1,SYNDIV2=0,SYNDIV1=0,SYNDIV0=0 */    
        CPMUSYNR = 0x01;                /* SYNDIV = 1, VCO frequency 32 - 48 MHz */       
       
        CPMUREFDIV = 0xC0;              /* REFDIV = 0, REFCLK frequency 6 - 12 MHz */       

        CPMUPOSTDIV = 0x07;             /* POSTDIV = 7 */

        CPMUCLKS_PLLSEL = 1;            /* PLLSEL = 1 */
     
        while (!CPMUCLKS_PLLSEL);       /* Verify CPMUCLKS configuration */

        CPMUOSC_OSCE = 1;               /* Enable Oscillator OSCE = 1 */

         while(!CPMUFLG_UPOSC);          /* wait for OSC to stabilise */

         CPMUCLKS_PLLSEL = 0;            /* PLLSEL = 0 */

         while (CPMUCLKS_PLLSEL);        /* Verify CPMUCLKS configuration */
                  
         CPMUPROT = 0x00U;                    /* Enable protection of clock configuration registers */
    }       

 /*
** ===================================================================
**     Method      :  CLK_Ini_24MHz(void)
**     Description : PEE - 48MHz VCO, 24MHz Bus CLK, from 16MHz Crystal 
** ===================================================================
 */
  void CLK_Ini_24MHz(void)
    {
        CPMUPROT = 0x26U;                    /* Disable protection of clock configuration registers */
        /* CPMUCLKS: PSTP=0 */
        CPMUCLKS &= (unsigned char)~(unsigned char)0x40U;                     
        /* CPMUCLKS: PLLSEL=1 */
        CPMUCLKS |= (unsigned char)0x80U;    /* Enable the PLL to allow write to divider registers */
        /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=1,SYNDIV3=1,SYNDIV2=0,SYNDIV1=0,SYNDIV0=0 */    
        CPMUREFDIV = 0x81U;                  /* Set the divider register */
        /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=0,SYNDIV5=0,SYNDIV4=0,SYNDIV3=0,SYNDIV2=0,SYNDIV1=1,SYNDIV0=0 */
        CPMUSYNR = 0x02U;                    /* Set the multiplier register */
        /* CPMUPOSTDIV: POSTDIV4=0,POSTDIV3=0,POSTDIV2=0,POSTDIV1=0,POSTDIV0=0 */
        CPMUPOSTDIV = 0x00U;                 /* Set the post divider register */
        /* CPMUOSC: OSCE=1,OSCBW=0,OSCPINS_EN=0,OSCFILT=0 */

        /* CPMUPLL: FM1=0,FM0=0 */
        CPMUPLL = 0x00U;                     /* Set the PLL frequency modulation */


         while (!CPMUCLKS_PLLSEL);       /* Verify CPMUCLKS configuration */
         CPMUOSC_OSCE = 1;               /* Enable Oscillator OSCE = 1 */
         while(!CPMUFLG_UPOSC);          /* wait for OSC to stabilise */
         CPMUPROT = 0x00U;                    /* Enable protection of clock configuration registers */
 
    }
    
    
    /* NOTE:
            CPMUCLKS_PLLSEL 设为“1" 时，使能PLL, 设为0 时，时钟会变成8MHz，为OSC/2
 */
    
   