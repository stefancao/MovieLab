/*********************************************************************/
/* MovieLab.c: homework assignment #5, for EECS 22,	Fall 2014    */
/* Name: Stefan Cao						     */
/* ID#: 79267250					             */
/* Course: EECS 22						     */
/*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "DIPs.h"
#include "Image.h"
#include "ImageList.h"
#include "Movie.h"


/*save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie);

/*read one frame from the movie */
IMAGE* ReadOneFrame(const char* fname, int nFrame, unsigned int W, unsigned H);

/*read the movie frames from the input file */
int ReadMovie(const char *fname, int nFrame, unsigned int W, unsigned H, MOVIE *movie);

/* Print the command line parameter usage of the program*/
void PrintUsage();

/* type define the function pointer to the DIP function	*/
typedef void MOP_F(IMAGE *image);

/* the function for perform DIP operations on the movie*/
void Movie_DIP_Operation(MOVIE *movie, MOP_F *MovieOP);



/* Main Function */
int main(int argc, char *argv[])
{
	int x = 0;
	char *fin = NULL, *fout = NULL;


	/* initialize and define variables */
	int num_frames = 0;

	unsigned int Width, Height = 0;
	
	int bw_op = 0; 
	int vflip_op = 0;
	int hmirror_op = 0;
	int poster_op = 0; 
	int edge_op = 0;

	int start = 0;
	int end = 0;

	int cut_op = 0;
	int resize_op = 0;
	int fast_op = 0;
	int rvs_op = 0;

	int resize_factor = 0;
	int fast_factor = 0;

/* entering while loop to check options entered */
	while(x < argc)
	{
		/*specify input video*/
		if(0 == strcmp(&argv[x][0], "-i")) 
		{
			if(x < argc - 1)
			{
				fin = (char *)malloc(sizeof(char) * (strlen(&argv[x + 1][0]) + strlen(".yuv") + 1));
				strcpy(fin, argv[x + 1]);
				strcat( fin, ".yuv");
			}/*fi*/
			else
			{
				printf("Missing argument for input name!");
				free(fin);
				free(fout);
				return 5;
			}/*esle*/
			x += 2;
			continue;
		}/*fi*/

		/*specify output video*/
		if(0 == strcmp(&argv[x][0], "-o")) 
		{
			if(x < argc - 1)
			{
				fout = (char *)malloc(sizeof(char) * (strlen(&argv[x + 1][0]) + strlen(".yuv") + 1));
				strcpy(fout, argv[x + 1]);
				strcat( fout, ".yuv");
			}/*fi*/
			else
			{
				printf("Missing argument for output name!");
				free(fin);
				free(fout);
				return 5;
			}/*esle*/
			x += 2;
			continue;
		}/*fi*/

		/* determine number of frames ti read from input stream */
		if(0 == strcmp(&argv[x][0], "-f"))
		{
			if(x < argc -1)
			{
				if(sscanf(argv[x+1], "%d", &num_frames) != 1)
				{
					printf("Incorrect input!\n");
					free(fin);
					free(fout);
					return 5;
				}
			}
			else
			{
				printf("Missing argument for number of frames!");
				free(fin);
				free(fout);
				return 5;
			}
			x += 2;
			continue;
		}		

		/* set the resolution of the input stream (widthxheight) */
		if(0 == strcmp(argv[x], "-s"))
		{
			if(x < argc - 1)
			{
				if(sscanf(argv[x+1], "%dx%d", &Width, &Height) != 2)
				{
					printf("Wrong input formatting!");
					free(fin);
					free(fout);
					return 5;
				}

			}
			else
			{
				printf("Missing argument for size of frame!");
				free(fin);
				free(fout);
				return 5;
			}
			x += 2;
		}
		
		/* activate the conversion to black and white */
		if (0 == strcmp(argv[x], "-bw"))
		{
			bw_op = 1;
		}

		/* activate the vertical flip */
		if (0 == strcmp(argv[x], "-vflip"))
		{
			vflip_op = 1;
		}	
	
		/* activate horizontal mirror */	
		if (0 == strcmp(argv[x], "-hmirror"))
		{
			hmirror_op = 1;
		}

		/* activate edge filter */	
		if (0 == strcmp(argv[x], "-edge"))
		{
			edge_op = 1;
		}
	
		/* activate posterize filter */	
		if (0 == strcmp(argv[x], "-poster"))
		{
			poster_op = 1;
		}

		/* to crop the frames fromt he video from frame Start to frame End */
		if(0 == strcmp(argv[x], "-cut"))
                {
                        if(x < argc - 1)
                        {
                                if(sscanf(argv[x+1], "%d-%d", &start, &end) != 2)
                                {
                                        printf("Wrong input formatting!");
                                        free(fin);
                                        free(fout);
                                        return 5;
                                }
				cut_op = 1;
                        }
                        else
                        {
                                printf("Missing argument for crop frames!");
                                free(fin);
                                free(fout);
                                return 5;
                        }                                                                    
			                                                      
                }                         

		/* resize the video with the provided factor (1-100) */
		if(0 == strcmp(argv[x], "-resize"))
                {
                        if(x < argc - 1)
                        {
				if(sscanf(argv[x+1], "%d", &resize_factor) != 1)
				{
					printf("Incorrect input!\n");
					free(fin);
					free(fout);
					return 5;
				}

                        }
                        else
                        {
                                printf("Missing argument for factor of resize!");
                                free(fin);
                                free(fout);
                                return 5;
                        }
			resize_op = 1;
                        x += 2;
                        continue;
                }
		
		/* to fast forward the video with the provided factor */
		if(0 == strcmp(argv[x], "-fast"))
                {
                        if(x < argc -1)
                        {
				if(sscanf(argv[x+1], "%d", &fast_factor) != 1)
				{
					printf("Incorrect input!\n");
					free(fin);
					free(fout);
					return 5;
				}					
                        }
                        else
                        {
                                printf("Missing argument for factor of fast!");
                                free(fin);
                                free(fout);
                                return 5;
                        }
			fast_op = 1;
                        x += 2;
                        continue;
                }
	
		/* to reverse the frame order of the input stream */
		if (0 == strcmp(argv[x], "-rvs"))
                {
                        rvs_op = 1;
                }
		

		/*help option*/
		if(0 == strcmp(&argv[x][0], "-h")) 
		{ 
			PrintUsage();
			free(fin);
			free(fout);
			return 0;
		}/*fi*/
		x++;
	}/*elihw*/

	if(!fin){
		printf("Missing argument for input name!\n");
		PrintUsage();
		free(fin);
		free(fout);
		return 5;
	}

	if(!fout){
		printf("Missing argument for output name!\n");
		PrintUsage();
		free(fin);
		free(fout);
		return 5;
	}

	/* Creating a pointer a creating a movie */	
	MOVIE *movie = NULL;
	movie = CreateMovie(num_frames, Width, Height);
 
	/* Reading Movie */
	ReadMovie(fin, num_frames, Width, Height, movie);

	/* cut, resize, fast, reverse */
	if (cut_op == 1)
	{
		CropImageList(movie->Frames, start, end);
		printf("Operation Frame Cropping is done!\n");
	}
	if (resize_op == 1)
	{
		ResizeImageList(movie->Frames, resize_factor);	
		printf("Operation Resize is done! New Width/Height = %dx%d\n", (Width * resize_factor / 100), (Height * resize_factor /100));
	}
	if (fast_op == 1)
	{
		FastImageList(movie->Frames, fast_factor);
		printf("Opearation Fast Forward is done! Number of frames = %d\n", num_frames/fast_factor);
	}
	if (rvs_op == 1)
	{
		ReverseImageList(movie->Frames);
		printf("Operation ReverseMovie is done!\n");
	}


	/* Movie operations */
	if (bw_op == 1)
	{
		Movie_DIP_Operation(movie, BlackNWhite);
		printf("Operation BlackNWhite is done!\n");
	}
	if (vflip_op == 1)
	{
		Movie_DIP_Operation(movie, VFlip);
		printf("Operation VFlip is done!\n");
	}
	if (hmirror_op == 1)
	{
		Movie_DIP_Operation(movie, HMirror);
		printf("Operation HMirror is done!\n");
	}
	if (edge_op == 1)
	{
		Movie_DIP_Operation(movie, Edge);
		printf("Operation Edge is done!\n");
	}
	if (poster_op == 1)
	{
		Movie_DIP_Operation(movie, Posterize);
		printf("Operation Posterize is done!\n");
	}

	/* Save Movie */
	SaveMovie(fout, movie);

	free(fin);
	free(fout);
	fin = NULL;
	fout = NULL;
	
	/* Delete Images and ImageEntries */
	IENTRY *pCurrent;                                                                    
        IENTRY *pPrev;                                                                       
        IENTRY *pNext;                                                                       
                                                                                              
        pCurrent = movie->Frames->First;                                                                 
        pPrev = NULL;                                                              
        pNext = movie->Frames->First->Next;                                                              
        while(pNext)                                               
        {      
                pCurrent->Next = pNext;                                                      
                pCurrent->Prev = pPrev;                                                      
                pPrev = pCurrent;
		DeleteImage(pPrev->Image);
		DeleteImageEntry(pPrev);                                                       
                pCurrent = pNext;                                                            
                pNext = pNext->Next;                                                         
                pCurrent->Next = pNext;                                                      
                pCurrent->Prev = pPrev;                                                      
        }
	pCurrent = NULL;
	pPrev = NULL;
	pNext = NULL;

	/* Delete Movie */
	DeleteMovie(movie);
	return 0;
}

void PrintUsage()
{
	printf("\nFormat on command line is:\n"
	"MovieLab [option]\n"
	"-i [file_name]    to provide the input file name\n"
	"-o [file_name]    to provide the	output file name\n"
	"-f [no_frames]    to determine how many frames desired in the input stream\n"
	"-s [WidthxHeight] to set resolution of the input stream (widthxheight)\n"
	"-j                to generate the movie with JuliaSet sequences\n"
	"-bw               to activate the conversion to black and white\n"
	"-vflip            to activate vertical flip\n"
	"-hmirror          to activate horizontal flip\n"
	"-edge             to activate edge filter\n"
	"-poster           to activate posterize filter\n"
	"-cut [Start-End]  to crop the frame from the video from frame Start to frame End\n"  
	"-resize [factor]  to resize the video with the provided factor [1-100]\n"
	"-fast   [factor]  to fast forward the video with the provided factor [1+]\n"
	"-rvs              to reverse the frame order of the input stream\n"
	"-h                to show this usage information\n"
	);
}

IMAGE* ReadOneFrame(const char *fname, int nFrame, unsigned int W, unsigned H)
{
        /*defining local variables*/
        FILE *file;

        unsigned int x, y;
        unsigned char ch;
        IMAGE* image ;

        /*checking error*/
        assert(fname);
        assert(nFrame >= 0);

        image = CreateImage(W, H) ;
        assert(image) ;

        /*opening file stream*/
        file = fopen(fname, "r");
        assert(file) ;

        /*find desired frame*/
        fseek(file, 1.5 * nFrame * W * H, SEEK_SET);

        for(y = 0; y < H; y ++){
                for(x = 0; x < W; x ++){
                        ch = fgetc(file);
                        SetPixelY(image, x, y, ch);
                }/*rof*/
        }

        for(y = 0; y < H ; y += 2){
                for(x = 0; x < W ; x += 2){
                        ch = fgetc(file);
                        SetPixelU(image, x, y, ch);
                        SetPixelU(image, x + 1, y, ch);
                        SetPixelU(image, x, y + 1, ch);
                        SetPixelU(image, x + 1, y + 1, ch);
                }
        }

        for(y = 0; y < H ; y += 2){
                for(x = 0; x < W ; x += 2){
                        ch = fgetc(file);
                        SetPixelV(image, x, y, ch);
                        SetPixelV(image, x + 1, y, ch);
                        SetPixelV(image, x, y + 1, ch);
                        SetPixelV(image, x + 1, y + 1, ch);
                }
        }

        /*checking for error*/

        assert(ferror(file) == 0) ;

        /*closing stream and terminating*/
        fclose(file);
        file = NULL;
        return image;
}


/*read the movie frames from the input file */
int ReadMovie(const char *fname, int nFrame, unsigned int W, unsigned H, MOVIE *movie)
{
	assert(movie);

	/* AppendImage by calling ReadOneFrame to read one image at a time */
	int i;
	for (i = 0; i < nFrame; i++)
	{
		AppendImage(movie->Frames, ReadOneFrame(fname, i, W, H));
	}
	printf("The movie file %s has been read successfully!\n", fname);
	
	return 0;
	
}


/* the function for perform DIP operations on the movie*/
void Movie_DIP_Operation(MOVIE *movie, MOP_F *MovieOP)
{
	IENTRY *e;
	ILIST *l;

	l = movie->Frames;
	e = l->First;
	while(e)
	{
		YUV2RGBImage(e->Image);
		MovieOP(e->Image);
		RGB2YUVImage(e->Image);
		e = e->Next;
	}
}


/*save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie)
{
	int i ; 
	int x, y;
	FILE *file;
	IENTRY *currFrame;

	 /*opening file stream*/
	 if(!(file = fopen(fname, "w"))){
			return 1;
	 }
	 
	 i = 0 ; 

	ILIST *l;
	l = movie->Frames;	

	 currFrame = l->First;
	 while(currFrame){
		for(y = 0; y < movie->Height; y ++){ 
			for(x = 0; x < movie->Width; x ++){
				fputc(GetPixelY(currFrame->Image, x, y), file);
			}
		}
		for(y = 0; y < movie->Height; y += 2){ 
			for(x = 0; x < movie->Width; x += 2){
				fputc(GetPixelU(currFrame->Image, x, y), file);
			}
		}
		for(y = 0; y < movie->Height; y += 2){ 
			for(x = 0; x < movie->Width; x += 2){
				fputc(GetPixelV(currFrame->Image, x, y), file);
			}
		}
		currFrame = currFrame->Next;	 
		i++ ; 
	 }

	 fclose(file);
	 file = NULL;

	 printf("The movie file %s has been written successfully! \n", fname);
	 printf("%d frames are written to the file %s in total \n", i, fname); 
	 return 0;
}

