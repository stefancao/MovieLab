/* ImageList.h */

#ifndef IMAGELIST_H
#define IMAGELIST_H
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Image.h"


typedef struct ImageEntry IENTRY;
struct ImageEntry
{
	struct ILIST *List;	/* pinter to the list which this entry belongs to */
	IENTRY *Next;	/* pointer to the next entry, or NULL */
	IENTRY *Prev; 	/* pointer to the previous entry, or NULL */
	IMAGE *Image;	/* pointer to the struct for the image */	
};

typedef struct ImageList ILIST;
struct ImageList
{
	unsigned int Length;	/* Length of the list */
	IENTRY *First;		/* pointer to the first entry, or NULL */
	IENTRY *Last;		/* pointer to the last entry, or NULL */
};

/* allocate a new image list */
ILIST *NewImageList(void);

/* allocate a new image entry */
IENTRY *NewImageEntry(IMAGE *image);

/* delete an image entry */
IMAGE *DeleteImageEntry(IENTRY *e);

/* delete an image list (and all entries) */
void DeleteImageList(ILIST *l);

/* insert a frame into a list at the end */
void AppendImage(ILIST *l, IMAGE *image);

/* reverse an image list */
void ReverseImageList(ILIST *l);

/* Crop frames from the list starting from SFrame to EFrame */
void CropImageList(ILIST *l, unsigned int SFrame, unsigned int EFrame);

/* Resize the images in the list */
void ResizeImageList(ILIST *l, unsigned int percentage);

/* Fast forward the video */
unsigned int FastImageList(ILIST *l, unsigned int ff_factor);

#endif
