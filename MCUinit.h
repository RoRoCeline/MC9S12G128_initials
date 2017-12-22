/*   ###################################################################
**     设置系统时钟，从EXTCLK口输出系统总线时钟（bus clock）
**     Copyright : RoRoceline All Rights Reserved.
**     从MCUinit 中备注或去掉备注，设置时钟初始化，基于16MHz晶振
**     mail   : //undefined
** ###################################################################
*/
/* MODULE MCUinit */


#pragma CODE_SEG DEFAULT



extern void CLK_Ini_6_25MHz(void);
extern void CLK_Ini_12_5MHz(void);
extern void CLK_Cry_8MHz(void);
extern void CLK_Cry_24MHz(void);
extern void CLK_Cry_25MHz(void); 


extern void Led_Ini(void);
extern void Key_Ini(void); 

extern void SCI2_init(void);
