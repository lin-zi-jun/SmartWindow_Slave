/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : MainTask.c
Purpose     : Init GUI & call of GUI-demo
--------------------END-OF-HEADER-------------------------------------
*/
#include "headers.h"
extern OS_SEM mainSem;
//extern u8 SHOW_IMAGE_FLAG;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void UITask(void) {
//   delay_ms(20000);

//   show_image_power(10,10);
//    while(1)
//   {           
//       show_image(10,10);
// //      create_image_file();
//       OSSemPost(&mainSem, 0, NULL);
//   } 



 	while(1) {
 		UI_Init();
 
      }         
}
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
