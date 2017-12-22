/*   ###################################################################
**     设置系统时钟，从EXTCLK口输出系统总线时钟（bus clock）
**     Copyright : RoRoceline All Rights Reserved.
**     从MCUinit 中备注或去掉备注，设置时钟初始化，基于16MHz晶振
**     mail   : //undefined
** ###################################################################
*/
/* MODULE MCUinit */

#include <MC9S12G128.h>                /* I/O map for MC9S12G128MLH */
#include "MCUinit.h"

#define CGM_DELAY  0x0CFFU

/* User declarations and definitions */
/*   Code, declarations and definitions here will be preserved during code generation */
/* End of user declarations and definitions */

/*
** ===================================================================
**     Method      :  MCU_init (component MC9S12G128_64)
**
**     Description :
**         Device initialization code for selected peripherals.
** ===================================================================
*/

void CLK_Ini_6_25MHz(void);
void CLK_Ini_12_5MHz(void);
void CLK_Cry_8MHz(void);
void CLK_Cry_24MHz(void);
void CLK_Cry_25MHz(void);  
void LedBlkPin(void);


void Led_Ini(void);
void Key_Ini(void); 

void SCI2_init(void);
unsigned long Baud_Rate = 19200;
unsigned int BaudRatePrescaler;

    /*     unsigned long Baud_Rate = 19200; */
    /*     unsigned long Baud_Rate = 38400; */
    /*     unsigned long Baud_Rate = 40000; */
    /*     unsigned long Baud_Rate = 57600; */

// ===================================================================
//     Led_Ini(void)
//     Description : LED port initial
// ===================================================================
void Led_Ini(void) 
  {
      /* PIEJ: PIEJ7=0,PIEJ6=0,PIEJ5=0,PIEJ4=0,PIEJ3=0,PIEJ2=0,PIEJ1=0,PIEJ0=0 */
  PIEJ = 0x00U;                                      
    /* Clear interrupt flags */
  /* PIFJ: PIFJ7=1,PIFJ6=1,PIFJ5=1,PIFJ4=1,PIFJ3=1,PIFJ2=1,PIFJ1=1,PIFJ0=1 */
  PIFJ = 0xFFU;                                      
  /* PPSJ: PPSJ7=0,PPSJ6=0,PPSJ5=0,PPSJ4=0,PPSJ3=0,PPSJ2=0,PPSJ1=0,PPSJ0=0 */
  PPSJ = 0x00U;                                      
  /* PERJ: PERJ7=1,PERJ6=1,PERJ5=1,PERJ4=1,PERJ3=1,PERJ2=1,PERJ1=1,PERJ0=1 */
  PERJ = 0xFFU;                                      
  /* DDRJ: DDRJ7=1,DDRJ6=1,DDRJ5=1,DDRJ4=1,DDRJ3=1,DDRJ2=1,DDRJ1=1,DDRJ0=1 */
  DDRJ = 0xFFU;  
  
  PTJ =0XFF;
  }
 
 
// ===================================================================
//     Key_Ini(void)
//     Description : T port KEY initial
// =================================================================== 
 void Key_Ini(void)  
  {
  PPST &= (unsigned char)~(unsigned char)3U;                     
  /* PERT: PERT1=1,PERT0=1 */
  PERT |= (unsigned char)3U;                      
  /* DDRT: DDRT1=0,DDRT0=0 */
  DDRT &= (unsigned char)~(unsigned char)3U;                     
  /* ### */
  /* Initial interrupt priority */
  /*lint -save  -e950 Disable MISRA rule (1.1) checking. */
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
**     Method      :  CLK_Cry_8MHz(void)
**     Description : PBE - 16 MHz Crystal, 4MHz Bus CLK (32MHz VCO, 4MHz PLL) 
** ===================================================================
*/ 

void CLK_Cry_8MHz(void)
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
**     Method      :  CLK_Cry_24MHz(void)
**     Description : PEE - 48MHz VCO, 24MHz Bus CLK, from 16MHz Crystal 
** ===================================================================
 */
  void CLK_Cry_24MHz(void)
    {
        CPMUPROT = 0x26U;  /* Disable protection of clock configuration registers */
        CPMUCLKS &= (unsigned char)~(unsigned char)0x40U;   /* CPMUCLKS: PLLSEL=1 */
        CPMUCLKS |= (unsigned char)0x80U;   /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=1,SYNDIV3=1,SYNDIV2=0,SYNDIV1=0,SYNDIV0=0 */    
        CPMUREFDIV = 0x81U; /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=0,SYNDIV5=0,SYNDIV4=0,SYNDIV3=0,SYNDIV2=0,SYNDIV1=1,SYNDIV0=0 */
        CPMUSYNR = 0x02U;   /* CPMUPOSTDIV: POSTDIV4=0,POSTDIV3=0,POSTDIV2=0,POSTDIV1=0,POSTDIV0=0 */
        CPMUPOSTDIV = 0x00U;  /* CPMUOSC: OSCE=1,OSCBW=0,OSCPINS_EN=0,OSCFILT=0 */
                              /* CPMUPLL: FM1=0,FM0=0 */
        CPMUPLL = 0x00U;                     /* Set the PLL frequency modulation */


         while (!CPMUCLKS_PLLSEL);       /* Verify CPMUCLKS configuration */
         CPMUOSC_OSCE = 1;               /* Enable Oscillator OSCE = 1 */
         while(!CPMUFLG_UPOSC);          /* wait for OSC to stabilise */
         CPMUPROT = 0x00U;                    /* Enable protection of clock configuration registers */
 
    }         

    
 /*
** ===================================================================
**     Method      :  CLK_Cry_25MHz(void)
**     Description : PEE - 50MHz VCO, 24MHz Bus CLK, from 16MHz Crystal 
** ===================================================================
 */
  void CLK_Cry_25MHz(void)       
  {      
    CPMUPROT = 0x26U; 
    CPMUCLKS &= (unsigned char)~(unsigned char)0x40U;   /* CPMUCLKS: PLLSEL=1 */
    CPMUCLKS |= (unsigned char)0x80U;    /* CPMUREFDIV: REFFRQ1=0,REFFRQ0=0,REFDIV3=1,REFDIV2=1,REFDIV1=1,REFDIV0=1 */
    CPMUREFDIV = 0x0FU;  /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=1,SYNDIV3=1,SYNDIV2=0,SYNDIV1=0,SYNDIV0=0 */
    CPMUSYNR = 0x58U;                    /* Set the multiplier register */
    CPMUPOSTDIV = 0x00U;                 /* Set the post divider register */
    CPMUOSC = 0x80U;                      /* CPMUPLL: FM1=0,FM0=0 */
    CPMUPLL = 0x00U;                     /* Set the PLL frequency modulation */
    
    while (!CPMUCLKS_PLLSEL);       /* Verify CPMUCLKS configuration */
      CPMUOSC_OSCE = 1;               /* Enable Oscillator OSCE = 1 */
    while(!CPMUFLG_UPOSC);          /* wait for OSC to stabilise */
      CPMUPROT = 0x00U;                    /* Enable protection of clock configuration registers */
 
  }
  
      /* NOTE:
            CPMUCLKS_PLLSEL 设为“1" 时，使能PLL, 设为0 时，时钟会变成8MHz，为OSC/2
 */




void SCI2_init(void)

{   CPMUCOP &= (unsigned char)~(unsigned char)0x60U;                     
  /* Common initialization of the CPU registers */
  /* WOMM: WOMM3=0,UART port configuration */
  WOMM &= (unsigned char)~(unsigned char)0x08U;                     
  /* CPMUINT: LOCKIE=0,OSCIE=0 */
  CPMUINT &= (unsigned char)~(unsigned char)0x12U;                     
  /* CPMULVCTL: LVIE=0 */
  CPMULVCTL &= (unsigned char)~(unsigned char)0x02U;                     
  /* IRQCR: IRQEN=0 */
  IRQCR &= (unsigned char)~(unsigned char)0x40U;                     
  /* ### Init_SCI init code */
  /* SCI2CR2: TIE=0,TCIE=0,RIE=0,ILIE=0,TE=0,RE=0,RWU=0,SBK=0 */
  SCI2CR2 = 0x00U;                     /* Disable the SCI2 module */
  (void)(SCI2SR1 == 0);                /* Dummy read of the SCI2SR1 registr to clear flags */
  (void)(SCI2DRL == 0);                /* Dummy read of the SCI2DRL registr to clear flags */
  /* SCI2SR2: AMAP=0,TXPOL=0,RXPOL=0,BRK13=0,TXDIR=0,RAF=0 */
  SCI2SR2 = 0x00U;                                      
  /* SCI2BD: IREN=0,TNP1=0,TNP0=0,SBR12=0,SBR11=0,SBR10=0,SBR9=0,SBR8=0,SBR7=1,SBR6=0,SBR5=1,SBR4=0,SBR3=0,SBR2=0,SBR1=1,SBR0=1 */
  SCI2BD = 0xA3U;                            
  /* SCI2CR1: LOOPS=0,SCISWAI=0,RSRC=0,M=0,WAKE=0,ILT=0,PE=0,PT=0 */
  SCI2CR1 = 0x00U;                                      
  /* SCI2SR2: AMAP=1 */
  SCI2SR2 |= (unsigned char)0x80U;     /* Switch to the alternative register set */
  /* SCI2ASR1: RXEDGIF=1,BERRV=0,BERRIF=1,BKDIF=1 */
  SCI2ASR1 = 0x83U;                    /* Clear interrupt flags */
  /* SCI2ACR1: RXEDGIE=0,BERRIE=0,BKDIE=0 */
  SCI2ACR1 = 0x00U;                                      
  /* SCI2ACR2: BERRM1=0,BERRM0=0,BKDFE=0 */
  SCI2ACR2 = 0x00U;                                      
  /* SCI2SR2: AMAP=0 */
  SCI2SR2 &= (unsigned char)~(unsigned char)0x80U; /* Switch to the normal register set */
  /* SCI2CR2: TIE=0,TCIE=0,RIE=0,ILIE=0,TE=1,RE=1,RWU=0,SBK=0 */
  SCI2CR2 = 0x0CU;                                      
  /* ### Init_GPIO init code */     
  
  //Set BaudRate
   BaudRatePrescaler = (unsigned int)(25000000 / (16 * Baud_Rate));

    SCI2BDH = (unsigned char)((BaudRatePrescaler>>8));
    SCI2BDL = (unsigned char)(BaudRatePrescaler); 
}
