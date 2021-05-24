/**
* implementing a threaded program that loads an image 􏰃le, applies a 􏰃lter to it, and saves it back to disk.
*
* Completion time: 15 hrs
*
* @author Brent Stockton, (Acuna, whose code you used)
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

//TODO: finish me

//UNCOMMENT BELOW LINE IF USING SER334 LIBRARY/OBJECT FOR BMP SUPPORT
//#include "BmpProcessor.h"

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096
#define BI_RGB 0

//TODO: finish me

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

//Bmp header
struct BMP_Header
{
	char signature[2];
	int size;
	short reserved1;
	short reserved2;
	int offset_pixel_array;
};

//info header
typedef struct info_header
{
	int size;
	int width;
	int height;
	short planes;
	short bits_per_pixel;
	int compression;
	int image_size_bytes;
	int pixel_per_meter_horizontal;
	int pixel_per_meter_vertical;
	int color_used;
	int color_IMP;
} BM_INFO_HEADER;

//load colour pixels
typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} COLOUR_INDEX;

COLOUR_INDEX colour_index[MAXIMUM_IMAGE_SIZE][MAXIMUM_IMAGE_SIZE];

COLOUR_INDEX blur_color[MAXIMUM_IMAGE_SIZE][MAXIMUM_IMAGE_SIZE];

typedef enum colours
{
	BLUE = 0,
	GREEN = 1,
	RED = 2
} COLOR;

//blur algo Forward dec;
unsigned char blur(int, int, COLOR);
unsigned char swissCheese(int, int, COLOR);

int global_height = 0;
int global_width = 0;

//threading

void *left_upper_blur(void *unused)
{

	int i;
	int j;

	int height = (global_height + 1) / 2;
	int width = (global_width + 1) / 2;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			blur_color[i][j].b = blur(i, j, BLUE);
			blur_color[i][j].g = blur(i, j, GREEN);
			blur_color[i][j].r = blur(i, j, RED);
		}
	}
}

void *left_lower_blur(void *unused)
{

	int i;
	int j;

	int height = global_height;
	int width = (global_width + 1) / 2;

	for (j = global_height / 2; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			blur_color[i][j].b = blur(i, j, BLUE);
			blur_color[i][j].g = blur(i, j, GREEN);
			blur_color[i][j].r = blur(i, j, RED);
		}
	}
}

void *right_upper_blur(void *unused)
{

	int i;
	int j;

	int height = (global_height + 1) / 2;
	int width = global_width;

	for (j = 0; j < height; j++)
	{
		for (i = global_width / 2; i < width; i++)
		{
			blur_color[i][j].b = blur(i, j, BLUE);
			blur_color[i][j].g = blur(i, j, GREEN);
			blur_color[i][j].r = blur(i, j, RED);
		}
	}
}

void *right_lower_blur(void *unused)
{

	int i;
	int j;

	int height = (global_height + 1) / 2;
	int width = (global_width + 1) / 2;

	for (j = global_height / 2; j < height; j++)
	{
		for (i = global_width / 2; i < width; i++)
		{
			blur_color[i][j].b = blur(i, j, BLUE);
			blur_color[i][j].g = blur(i, j, GREEN);
			blur_color[i][j].r = blur(i, j, RED);
		}
	}
}

//swiss cheese placeholders idea
void *left_upper_swiss(void *unused)
{

	int i;
	int j;

	int height = (global_height + 1) / 2;
	int width = (global_width + 1) / 2;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			blur_color[i][j].b = swissCheese(i, j, BLUE);
			blur_color[i][j].g = swissCheese(i, j, GREEN);
			blur_color[i][j].r = swissCheese(i, j, RED);
		}
	}
}

void *left_lower_swiss(void *unused)
{

	int i;
	int j;

	int height = global_height;
	int width = (global_width + 1) / 2;

	for (j = global_height / 2; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			blur_color[i][j].b = swissCheese(i, j, BLUE);
			blur_color[i][j].g = swissCheese(i, j, GREEN);
			blur_color[i][j].r = swissCheese(i, j, RED);
		}
	}
}

void *right_upper_swiss(void *unused)
{

	int i;
	int j;

	int height = (global_height + 1) / 2;
	int width = global_width;

	for (j = 0; j < height; j++)
	{
		for (i = global_width / 2; i < width; i++)
		{
			blur_color[i][j].b = swissCheese(i, j, BLUE);
			blur_color[i][j].g = swissCheese(i, j, GREEN);
			blur_color[i][j].r = swissCheese(i, j, RED);
		}
	}
}

void *right_lower_swiss(void *unused)
{

	int i;
	int j;

	int height = (global_height + 1) / 2;
	int width = (global_width + 1) / 2;

	for (j = global_height / 2; j < height; j++)
	{
		for (i = global_width / 2; i < width; i++)
		{
			blur_color[i][j].b = swissCheese(i, j, BLUE);
			blur_color[i][j].g = swissCheese(i, j, GREEN);
			blur_color[i][j].r = swissCheese(i, j, RED);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

//TODO: finish me

void main(int argc, char *argv[])
{

	//TODO: finish me
	FILE *file = fopen("test1wonderbread.bmp", "rb");
	if (NULL == file)
	{
		printf("\nCan't open the file...\n");
		exit(1);
	}

	struct BMP_Header header;
	BM_INFO_HEADER infoHeader;
	int i;
	int j;

	//read file header
	fread(&header.signature, sizeof(char) * 2, 1, file);
	fread(&header.size, sizeof(int), 1, file);
	fread(&header.reserved1, sizeof(short), 1, file);
	fread(&header.reserved2, sizeof(short), 1, file);
	fread(&header.offset_pixel_array, sizeof(int), 1, file);

	printf("signature: %c%c\n", header.signature[0], header.signature[1]);
	printf("size: %d\n", header.size);
	printf("reserved1: %d\n", header.reserved1);
	printf("reserved2: %d\n", header.reserved2);
	printf("offset_pixel_array: %d\n", header.offset_pixel_array);

	fread(&infoHeader, sizeof(BM_INFO_HEADER), 1, file);
	printf("Image size: %d x %d\n", infoHeader.width, infoHeader.height);

	global_height = infoHeader.height;
	global_width = infoHeader.width;

	fseek(file, header.offset_pixel_array, SEEK_SET);

	for (j = 0; j < infoHeader.height; j++)
	{
		for (i = 0; i < infoHeader.width; i++)
		{
			fread(&colour_index[i][j].b, sizeof(unsigned char), 1, file);
			fread(&colour_index[i][j].g, sizeof(unsigned char), 1, file);
			fread(&colour_index[i][j].r, sizeof(unsigned char), 1, file);
		}
	}
	fclose(file);

	pthread_t left_upper_thread, left_lower_thread, right_upper_thread, right_lower_thread;

	pthread_create(&left_upper_thread, NULL, &left_upper_blur, NULL);
	pthread_join(left_upper_thread, NULL);

	pthread_create(&left_lower_thread, NULL, &left_lower_blur, NULL);
	pthread_join(left_lower_thread, NULL);

	pthread_create(&right_upper_thread, NULL, &right_upper_blur, NULL);
	pthread_join(left_upper_thread, NULL);

	pthread_create(&right_lower_thread, NULL, &right_lower_blur, NULL);
	pthread_join(right_lower_thread, NULL);

	FILE *f = fopen("test2.bmp", "wb");
	if (NULL == f)
	{
		printf("\nUnable to create file...\n");
		exit(1);
	}
	fwrite(&header.signature, sizeof(char) * 2, 1, f);
	fwrite(&header.size, sizeof(int), 1, f);
	fwrite(&header.reserved1, sizeof(short), 1, f);
	fwrite(&header.reserved2, sizeof(short), 1, f);
	fwrite(&header.offset_pixel_array, sizeof(int), 1, f);
	fwrite(&infoHeader, sizeof(BM_INFO_HEADER), 1, f);

	for (j = 0; j < infoHeader.height; j++)
	{
		for (i = 0; i < infoHeader.width; i++)
		{
			fwrite(&blur_color[i][j].b, sizeof(unsigned char), 1, f);
			fwrite(&blur_color[i][j].g, sizeof(unsigned char), 1, f);
			fwrite(&blur_color[i][j].r, sizeof(unsigned char), 1, f);
		}
	}
	fclose(f);
	printf("\nCreate Blur Image Success.\n");
}
//blur function
unsigned char blur(int i, int j, COLOR colour)
{

	int k;
	int r;

	switch (colour)
	{
	case BLUE:
	{
		int count = 0;
		int sum = 0;
		int average = 0;

		for (k = i - 1; k <= i + 1; k++)
		{
			if (k < 0 || k >= global_height)
			{
				continue;
			}
			for (r = j - 1; r <= j + 1; r++)
			{
				if (r < 0 || r >= global_width)
				{
					continue;
				}
				sum = sum + colour_index[k][r].b;
				count++;
			}
		}
		if (count)
		{
			average = sum / count;
		}
		return average;
	}
	break;

	case GREEN:
	{
		int count = 0;
		int sum = 0;
		int average = 0;
		for (k = i - 1; k <= i + 1; k++)
		{
			if (k < 0 || k >= global_height)
				continue;
			for (r = j - 1; r <= j + 1; r++)
			{
				if (r < 0 || r >= global_width)
				{
					continue;
				}
				sum = sum + colour_index[k][r].g;
				count++;
			}
		}
		if (count)
		{
			average = sum / count;
		}
		return average;
	}
	break;
	case RED:
	{
		int count = 0;
		int sum = 0;
		int average = 0;
		for (k = i - 1; k <= i + 1; k++)
		{
			if (k < 0 || k >= global_height)
			{
				continue;
			}
			for (r = j - 1; r <= j + 1; r++)
			{
				if (r < 0 || r >= global_width)
				{
					continue;
				}
				sum = sum + colour_index[k][r].r;
				count++;
			}
		}
		if (count)
		{
			average = sum / count;
		}
		return average;
	}
	break;
	default:
	{
	}
	}
}

//unsigned char swissCheese(int, int, COLOR){}
//ran out of time to really work this one out.
//would have needed different params but just kind of cloned
//my other algo as a placeholder