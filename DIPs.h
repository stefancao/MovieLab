/* DIPs.h */

#ifndef DIPS_H
#define DIPS_H

#include "Image.h"

/* change color image to black & white */
void BlackNWhite(IMAGE *image);

/* mirror image horizontally */
void HMirror(IMAGE *image);

/* flip image vertically */
void VFlip(IMAGE *image);

/* edge detection */
void Edge(IMAGE *image);

/*Posterize  Image*/
void Posterize(IMAGE *image);

#endif
