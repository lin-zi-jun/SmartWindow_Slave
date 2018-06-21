#ifndef LCDCONF_H
#define LCDCONF_H

/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE           (320)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE           (240)   /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL    (16)

#define LCD_CONTROLLER      -1
#define LCD_FIXEDPALETTE    565
#define LCD_SWAP_RB         1
#define LCD_SWAP_XY         0
#define LCD_MIRROR_Y        0
#define LCD_MIRROR_X        0
#define LCD_INIT_CONTROLLER() ili93xx_Initializtion(); 
#endif
