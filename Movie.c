/* Movie.c */

#include "Movie.h"

#include "Image.h"
#include "ImageList.h"

/* allocate the memory space for the movie */
/* and the memory space for the frame list */
/* return the pointer to the movie */


/* Creating a Movie by allocating the size, width, height, numframes */
MOVIE *CreateMovie(unsigned int nFrames, unsigned int W, unsigned int H)
{
	MOVIE *movie;
	if (!(movie = (MOVIE *)malloc(sizeof(MOVIE))))
	{
		return NULL;
	}
	movie->Width = W;
	movie->Height = H;
	movie->NumFrames = nFrames;
	movie->Frames = NewImageList();
		
	return movie; 
}
 
/* release the memory space for the frames and the frame list */
/* release the memory space for the movie */
void DeleteMovie(MOVIE *movie)
{
	assert(movie);
	free(movie->Frames);
	free(movie);
}
 
/* convert a YUV image into an RGB image */
void YUV2RGBImage(IMAGE *Image)
{
	assert(Image);

	int C, D, E;
	int R_temp, G_temp, B_temp;
	int x, y;

	for (x = 0; x < (Image->Width); x++)
	{
		for (y = 0; y < (Image->Height); y++)
		{
			C = GetPixelY(Image, x, y) - 16;
			D = GetPixelU(Image, x, y) - 128;
			E = GetPixelV(Image, x, y) - 128;

			R_temp = ((298*C+409*E+128) >> 8);	
			G_temp = ((298*C-100*D-208*E+128) >> 8);
			B_temp = ((298*C+516*D+128) >> 8);

			/* Scaling and settting the right values for R, G, B */
			SetPixelR(Image, x, y, ((R_temp > 255)? 255 : (R_temp < 0)? 0 : R_temp));
			SetPixelG(Image, x, y, ((G_temp > 255)? 255 : (G_temp < 0)? 0 : G_temp));
			SetPixelB(Image, x, y, ((B_temp > 255)? 255 : (B_temp < 0)? 0 : B_temp));	

		}
	}

}
 
/* convert an RGB image into a YUV image */
void RGB2YUVImage(IMAGE *Image)
{
	assert(Image);

	int Y_temp, U_temp, V_temp;
	int x, y;		

	for (x = 0; x < (Image->Width); x++)
	{
		for (y = 0; y < (Image->Height); y++)
		{
			Y_temp = (((66*(GetPixelR(Image, x, y))+129*(GetPixelG(Image, x, y))+25*(GetPixelB(Image, x, y))+128) >> 8) + 16);		
			U_temp = (((-38*(GetPixelR(Image, x, y))-74*(GetPixelG(Image, x, y))+112*(GetPixelB(Image, x, y))+128) >> 8) + 128);
			V_temp = (((112*(GetPixelR(Image, x, y))-94*(GetPixelG(Image, x, y))-18*(GetPixelB(Image, x, y))+128) >> 8) + 128);
		
			/* Scaling and setting the right values for Y, U, V */
			SetPixelY(Image, x, y, ((Y_temp > 255)? 255 : (Y_temp < 0)? 0 : Y_temp));
			SetPixelU(Image, x, y, ((U_temp > 255)? 255 : (U_temp < 0)? 0 : U_temp));
			SetPixelV(Image, x, y, ((V_temp > 255)? 255 : (V_temp < 0)? 0 : V_temp));
		
		}
	}
}




