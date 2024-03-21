#ifndef _LED_GPIO_H
#define _LED_GPIO_H

#include "sys.h"

#define SCPII  PEout(14)		//
#define LCPII  PEout(15)		//

#define R1  	PCout(7)  	//
#define R2  	PGout(8)		//
#define G1  	PCout(8)		//
#define G2  	PGout(7)		//
#define B1		PCout(6)  	//
#define B2  	PGout(6)		//

#define R3  	PDout(14)		//
#define R4  	PDout(12)		//
#define G3  	PDout(15)		//
#define G4  	PAout(8)		//
#define B3		PDout(13)		//
#define B4  	PCout(9)		//

#define R5  	PGout(4)		//
#define R6 	 	PDout(11)		//
#define G5  	PGout(5)		//
#define G6  	PGout(2)		//
#define B5		PGout(3)		//
#define B6  	PDout(10)		//

#define R7  	PDout(8)		//
#define R8  	PBout(13)		//
#define G7  	PDout(9)		//
#define G8  	PBout(14)		//
#define B7		PBout(15)		//
#define B8  	PBout(12)		//

#define R9  	PGout(1)		//
#define R10  	PFout(14)		//
#define G9  	PGout(0)		//
#define G10  	PFout(13)		//
#define B9		PFout(15)		//
#define B10  	PFout(12)		//

#define R11  	PEout(12)		//
#define R12  	PEout(9)		//
#define G11  	PEout(11)		//
#define G12  	PEout(8)		//
#define B11		PEout(10)		//
#define B12  	PEout(7)		//

void LED_GPIO_Init(void);

#endif
