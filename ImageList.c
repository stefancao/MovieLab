/* ImageList.c */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageList.h"
#include "Image.h"
#include "Movie.h"

/* allocate a new image list */
ILIST *NewImageList(void)
{
	ILIST *l;
	l = malloc(sizeof(ILIST));
	if (!l)
	{
		perror("Out of memory! Aborting...");
		exit(10);
	}
	l->Length = 0;
	l->First = NULL;
	l->Last = NULL;
	return l;	
}

/* delete an image list (and all entries) */
void DeleteImageList(ILIST *l)
{
	IENTRY *e, *n;
	IMAGE *image;

	assert(l);
	
	e = l->First;
	while(e)
	{
		n = e->Next;
		image = DeleteImageEntry(e);	
		DeleteImage(image);
		e = n;
	}
	free(l);
}

/* allocate a new image entry */
IENTRY *NewImageEntry(IMAGE *image)
{
	IENTRY *e;
	e = malloc(sizeof(IENTRY));
	if(!e)
	{
		perror("Out of memory! Aborting...");
		exit(10);
	}
	e->List = NULL;
	e->Next = NULL;
	e->Prev = NULL;
	e->Image = image;
	return e; 
}
 
/* delete an image entry */
IMAGE *DeleteImageEntry(IENTRY *e)
{
	IMAGE *image;
	assert(e);
	
	image = e->Image;
	free(e);
	return image;
}
 
/* insert a frame into a list at the end */
void AppendImage(ILIST *l, IMAGE *image)
{
	IENTRY *e = NULL;
	
	assert(l);
	assert(image);

	e = NewImageEntry(image); 
	if (l->Last)
	{
		e->List = l;
		e->Next = NULL;
		e->Prev = l->Last;
		l->Last->Next = e;
		l->Last = e;
	}
	else
	{
		e->List = l;
		e->Next = NULL;
		e->Prev = NULL;
		l->First = e;
		l->Last = e;
	}
	l->Length++;
}
 
/* reverse an image list */
void ReverseImageList(ILIST *l)
{
	assert(l);
	IENTRY *pCurrent;
	IENTRY *pPrev;
	IENTRY *pNext;
	pCurrent = l->First;
	pPrev = NULL;
	pNext = l->First->Next;

	while (pNext)
	{
		pCurrent->Next = pPrev;
		pCurrent->Prev = pNext;
		pPrev = pCurrent;
		pCurrent = pNext;
		pNext = pNext->Next;
		pCurrent->Next = pPrev;
		pCurrent->Prev = pNext;  
	}

	l->Last = l->First;
	l->First = pCurrent;
}
 
/* Crop frames from the list starting from SFrame to EFrame */
void CropImageList(ILIST *l, unsigned int SFrame, unsigned int EFrame)
{
	assert(l);
	
	int i, num_frames;
	num_frames = l->Length;
	
	ILIST *temp;
	
	for (i = 0; i < SFrame; i++)
	{
		temp = l->First->Next;
		DeleteImage(l->First->Image);
		DeleteImageEntry(l->First);
		l->First = temp;
	}

	for (i = 1; i < (num_frames-EFrame); i++)
	{
		temp = l->Last->Prev;
		DeleteImage(l->Last->Image);
		DeleteImageEntry(l->Last);
		l->Last = temp;
	}

	l->Last->Next = NULL;
}
 
/* Resize the images in the list */
void ResizeImageList(ILIST *l, unsigned int percentage)
{
	IENTRY *frame;
     
     	frame = l->First;
     	while(frame)
     	{
	
     		YUV2RGBImage(frame->Image);
     
    		unsigned int    x, y, x1, y1, x2, y2, i, j, tmpR, tmpG, tmpB;
    	 	unsigned int 	NEW_WIDTH, NEW_HEIGHT;
    	 	unsigned int WIDTH;
     		unsigned int HEIGHT;
     		IMAGE *image_tmp;
     
     
    	 	WIDTH 	= (frame->Image->Width);
     		HEIGHT = (frame->Image->Height);
     
     		if(percentage <= 0 || percentage > 500)
     		{
     			printf("Resizing percentage %d%% is out of the range, Sorry! \n", percentage);
     
     		}
     
     		NEW_WIDTH = WIDTH * percentage / 100;
     		NEW_HEIGHT = HEIGHT * percentage / 100;

     		image_tmp = CreateImage(NEW_WIDTH, NEW_HEIGHT);
     
     		for(x = 0; x < NEW_WIDTH; x ++){
     			for(y = 0; y < NEW_HEIGHT; y++){
     				x1 = (unsigned int)(x / (percentage / 100.00));
     				y1 = (unsigned int)(y / (percentage / 100.00));
   	 			x2 = (unsigned int)((x + 1) / (percentage / 100.00));
    	 			y2 = (unsigned int)((y + 1) / (percentage / 100.00));
     				tmpR = tmpB = tmpG = 0;
     
     				if(percentage < 100){
					for(i = x1; i < x2; i ++){
     						for(j = y1; j < y2; j ++){
     							tmpR += GetPixelR(frame->Image, i, j);
    							tmpG += GetPixelG(frame->Image, i, j);
     							tmpB += GetPixelB(frame->Image, i, j);
     						}
					}
					SetPixelR(image_tmp, x, y, tmpR / ((x2 - x1) * (y2 - y1)));
					SetPixelG(image_tmp, x, y, tmpG / ((x2 - x1) * (y2 - y1)));
					SetPixelB(image_tmp, x, y, tmpB / ((x2 - x1) * (y2 - y1)));
     
     				}
     				else{
					SetPixelR(image_tmp, x, y, GetPixelR(frame->Image, x1, y1));
					SetPixelG(image_tmp, x, y, GetPixelG(frame->Image, x1, y1));
					SetPixelB(image_tmp, x, y, GetPixelB(frame->Image, x1, y1));
     				}
     			}
     		}
     		(frame->Image->Width) = image_tmp->Width;
     		(frame->Image->Height) = image_tmp->Height;
     
     		for (y = 0; y < (image_tmp->Height); y++)
     		{
     			for (x = 0; x < image_tmp->Width; x++)
     			{
     				SetPixelR((frame->Image), x, y, GetPixelR(image_tmp, x, y));
     				SetPixelG((frame->Image), x, y, GetPixelG(image_tmp, x, y));
     				SetPixelB((frame->Image), x, y, GetPixelB(image_tmp, x, y));
     			}
     		}

    		DeleteImage(image_tmp);
          	
     		RGB2YUVImage(frame->Image);
     		frame = frame->Next;
     	}
}
 
/* Fast forward the video */
unsigned int FastImageList(ILIST *l, unsigned int ff_factor)
{
	assert(l);

	int i;
	
	ILIST *temp;	

	IENTRY *pCurrent;
	IENTRY *pNext;
	IENTRY *pPrev;
	pCurrent = l->First;
	pPrev = NULL;
	pNext = l->First->Next;

	for (i = 1; i < ff_factor; i++)
	{
		temp = pNext->Next;
		DeleteImage(pNext->Image);
		DeleteImageEntry(pNext);
		pNext = temp;
	}
	
	while(pNext)
	{
		pCurrent->Next = pNext;
		pCurrent->Prev = pPrev;
		pPrev = pCurrent;
		pCurrent = pNext;
		
		pNext = pNext->Next;
		for (i = 1; i < ff_factor; i++)
		{
			temp = pNext->Next;	
			DeleteImage(pNext->Image);
			DeleteImageEntry(pNext);
			pNext = temp;
		}		
		pCurrent->Next = pNext;
		pCurrent->Prev = pNext; 
	}

	

	return 0;
}



