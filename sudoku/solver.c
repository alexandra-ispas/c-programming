#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_header.h"
#include "constants.h"
#include "cJSON.h"

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Pixels;

bmp_fileheader* alloc_fileheader(){
	bmp_fileheader *fileheader = (bmp_fileheader*)malloc(1 * sizeof(bmp_fileheader));
	if(fileheader == NULL){
		printf("Allocation error\n");
		exit(EXIT_FAILURE);
	}
	return fileheader;
}

bmp_infoheader* alloc_infoheader(){
	bmp_infoheader *infoheader = (bmp_infoheader*)malloc(1 * sizeof(bmp_infoheader));
	if(infoheader == NULL){
		printf("Allocation error\n");
		exit(EXIT_FAILURE);
	}
	return infoheader;
}

Pixels **alloc_bitmap(int height, int width){
	Pixels **bitmap = (Pixels**)malloc(height * sizeof(Pixels*));
	if(bitmap == NULL){
		printf("Allocation error\n");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < width; i++){
		bitmap[i] = (Pixels*)malloc(width * sizeof(Pixels));
		if(bitmap[i] == NULL){
			printf("Allocation error\n");
			for(int j = 0; j < i; j++)
				free(bitmap[j]);
			free(bitmap);
			exit(EXIT_FAILURE);
		}
	}
	return bitmap;
}

void freeMemory(bmp_fileheader *fileheader, bmp_infoheader *infoheader, Pixels **bitmap){
	free(fileheader);
	for(int i = 0; i < infoheader->height; i++){
		free(bitmap[i]);
	}
	free(bitmap);
	free(infoheader);
}

void read_infoheader(bmp_infoheader **infoheader, cJSON *info){

	(*infoheader)->biSize = (unsigned int)cJSON_GetObjectItem(info, BISIZE)->valueint;
	(*infoheader)->width = (signed int)cJSON_GetObjectItem(info, WIDTH)->valueint;
	(*infoheader)->height = (signed int)cJSON_GetObjectItem(info, HEIGHT)->valueint;
	(*infoheader)->planes = (unsigned short)cJSON_GetObjectItem(info, PLANES)->valueint;
	(*infoheader)->bitPix = (unsigned short)cJSON_GetObjectItem(info, BITPIX)->valueint;
	(*infoheader)->biCompression = (unsigned int)cJSON_GetObjectItem(info, BITCOMPRESSION)->valueint;
	(*infoheader)->biSizeImage = (unsigned int)cJSON_GetObjectItem(info, BISIZEIMAGE)->valueint;
	(*infoheader)->biXPelsPerMeter = cJSON_GetObjectItem(info, BIXPELSPERMETER)->valueint;
	(*infoheader)->biYPelsPerMeter = cJSON_GetObjectItem(info, BIYPELSPERMETER)->valueint;
	(*infoheader)->biClrUsed = (unsigned int)cJSON_GetObjectItem(info, BICLRUSED)->valueint;
	(*infoheader)->biClrImportant = (unsigned int)cJSON_GetObjectItem(info, BICLRIMPORTANT)->valueint;
}

void read_fileheader(bmp_fileheader **fileheader, cJSON *file){
	char *signatureFile = (char*)cJSON_GetObjectItem(file, SIGNATURE)->valuestring;
	(*fileheader)->fileMarker1 = signatureFile[0];
	(*fileheader)->fileMarker2 = signatureFile[1];
	(*fileheader)->bfSize = (unsigned int)cJSON_GetObjectItem(file, BFSIZE)->valueint;
	(*fileheader)->unused1 = 0;
	(*fileheader)->unused2 = 0;
	(*fileheader)->imageDataOffset = (unsigned int)cJSON_GetObjectItem(file, IMAGEDATAOFFSET)->valueint;
}

//writes the matrix of pixels based on the task 
void Pixeliwriting(FILE *task, bmp_infoheader *infoheader, Pixels **pixeli
, int padding, int taskNo) {

    int i, j;
    unsigned char p = 0;

	if(taskNo == 1){
		if (padding == 4) {
			for (i = infoheader->height - 1; i >= 0 ; i--) {
				for (j = 0; j < infoheader->width; j++) {
					fwrite(&pixeli[i][j], sizeof(Pixels), 1, task);
				}
			}
		} else {
			for (i = infoheader->height - 1; i >= 0 ; i--) {
				for (j = 0; j < infoheader->width; j++) {
					fwrite(&pixeli[i][j], sizeof(Pixels), 1, task);
				}
				for (int pad = 0; pad < padding; pad ++) {
					fwrite(&p, sizeof(unsigned char), 1, task);
				}
			}
		}
		return;
	}

	//rotated the numbers inside each square
	if(taskNo == 2){
		if (padding == 4) {

			for (i = 0; i < infoheader->height; i++) {
				fwrite(&pixeli[i][0], sizeof(Pixels), 1, task);
				
				for(int k = 1; k < infoheader->width; k += 8){
					for(int j = k + 6; j >= k; j--){
						fwrite(&pixeli[i][j], sizeof(Pixels), 1, task);
					}
					fwrite(&pixeli[i][k+7], sizeof(Pixels), 1, task);
				}
			}
		} else {
			for (i = 0; i < infoheader->height; i++) {
				fwrite(&pixeli[i][0], sizeof(Pixels), 1, task);
				
				for(int k = 1; k < infoheader->width; k += 8){
					for(int j = k + 6; j >= k; j--){
						fwrite(&pixeli[i][j], sizeof(Pixels), 1, task);
					}
					fwrite(&pixeli[i][k+7], sizeof(Pixels), 1, task);
				}
				for (int pad = 0; pad < padding; pad ++) {
					fwrite(&p, sizeof(unsigned char), 1, task);
				}
			}
		}
		return;
	}
	if(taskNo == 4){
		if (padding == 4) {
			for (i = 0; i < infoheader->height; i++) {
				for (j = 0; j < infoheader->width; j++) {
					fwrite(&pixeli[i][j], sizeof(Pixels), 1, task);
				}
			}
		} else {
			for (i = 0; i < infoheader->height; i++) {
				for (j = 0; j < infoheader->width; j++) {
					fwrite(&pixeli[i][j], sizeof(Pixels), 1, task);
				}
				for (int pad = 0; pad < padding; pad ++) {
					fwrite(&p, sizeof(unsigned char), 1, task);
				}
			}
		}
		return;
	}
}

//creates the ouput file name for each task
char *getOutputFileName(char input_file[], const char task[]){

	char *outputPath = (char*)calloc(100, sizeof(char));
	if(outputPath == NULL){
		printf("Allocation error\n");
		exit(EXIT_FAILURE);
	}
	strcpy(outputPath, "");
	strcpy(outputPath, task);
	if(strcmp(task, OUTTASK2) == 0){
		strncat(outputPath, input_file+(strlen(input_file)-6), 2);
		strcat(outputPath, BMP);
	} else if(strcmp(task, OUTTASK1) == 0){
		strncat(outputPath, input_file+(strlen(input_file)-7), 2);
		strcat(outputPath, BMP);
	} else if(strcmp(task, OUTTASK3) == 0){
		strncat(outputPath, input_file+(strlen(input_file)-6), 2);
		strcat(outputPath, JSON);
	} else if(strcmp(task, OUTTASK4) == 0){
		strncat(outputPath, input_file+(strlen(input_file)-6), 2);
		strcat(outputPath, BMP);
	} else if(strcmp(outputPath, BONUSOUT) == 0){
		strncat(outputPath, input_file+(strlen(input_file)-6), 2);
		strcat(outputPath, BMP);
	}
	return outputPath;
}

void writeImage(char *outputPath, bmp_infoheader *infoheader, bmp_fileheader *fileheader, 
Pixels **bitmap, int task){
	FILE *out = fopen(outputPath, "wb");
	if(out == NULL){
		printf("Failed to open file\n");
		exit(EXIT_FAILURE);
	}
	int padding = 4 - ((infoheader->width * 3) % 4);
	fwrite(fileheader, sizeof(bmp_fileheader), 1, out);
    fwrite(infoheader, sizeof(bmp_infoheader), 1, out);
	Pixeliwriting(out, infoheader, bitmap, padding, task);
	fclose(out);
}

void readJSONFile(FILE* in, bmp_infoheader **infoheader, bmp_fileheader **fileheader, 
Pixels ***bitmap){

	fseek(in, 0, SEEK_END);
	long len = ftell(in);
	fseek(in, 0, SEEK_SET);

	char *buffer = (char*)calloc(len+1, sizeof(char));
	fread(buffer, sizeof(char), len, in);
	fclose(in);

	cJSON* root = cJSON_Parse(buffer);
	if(!cJSON_IsObject(root)){
		printf("Is not an object\n");
		exit(EXIT_FAILURE);
	}
	free(buffer);

	*infoheader = alloc_infoheader();
	cJSON *infoheaderJSON = cJSON_GetObjectItem(root, INFOHEADER);
	read_infoheader(infoheader, infoheaderJSON);

	*fileheader = alloc_fileheader();
	cJSON *fileheaderJSON = cJSON_GetObjectItem(root, FILEHEADER);
	read_fileheader(fileheader, fileheaderJSON);

	cJSON *bitmapJSON = cJSON_GetObjectItem(root,BITMAP);
	int size = cJSON_GetArraySize(bitmapJSON);
	*bitmap = alloc_bitmap((*infoheader)->height, (*infoheader)->width);

	int idx1 = 0, idx2 = 0;
	for(int i = 0; i < size; i += 3){

		(*bitmap)[idx1][idx2].b = cJSON_GetArrayItem(bitmapJSON, i)->valueint;
		(*bitmap)[idx1][idx2].g = cJSON_GetArrayItem(bitmapJSON, i+1)->valueint;
		(*bitmap)[idx1][idx2].r = cJSON_GetArrayItem(bitmapJSON, i+2)->valueint;
		idx2++;
		//the line is finished
		if(idx2 == (*infoheader)->width){
			idx2 = 0;
			idx1++;
		}
	}
	cJSON_Delete(root);
}

//reads a BMP file
void readImage(FILE *image, bmp_fileheader **fileheader, bmp_infoheader **infoheader, Pixels ***bitmap){
	
	*fileheader = alloc_fileheader();
	*infoheader = alloc_infoheader();
	fread(*fileheader, sizeof(bmp_fileheader), 1, image);
    fread(*infoheader, sizeof(bmp_infoheader), 1, image);

	int padding = 4 - (((*infoheader)->width * 3) % 4);
    int i, j;
    
    *bitmap = alloc_bitmap((*infoheader)->height, (*infoheader)->width );
    
    fseek(image, (*fileheader)->imageDataOffset, SEEK_SET);
   
    if (padding == 4) {
        for (i = 0; i < (*infoheader)->height; i++) {
            for (j = 0; j < (*infoheader)->width; j++) {
                fread( &((*bitmap)[i][j].b), sizeof(unsigned char), 1, image);
                fread( &((*bitmap)[i][j].g), sizeof(unsigned char), 1, image);
                fread( &((*bitmap)[i][j].r), sizeof(unsigned char), 1, image);

            }
        }
    } else {
        for (i = 0; i < (*infoheader)->height; i++) {
            for (j = 0; j < (*infoheader)->width; j++) {
        
                fread( &((*bitmap)[i][j].b), sizeof(unsigned char), 1, image);
                fread( &((*bitmap)[i][j].g), sizeof(unsigned char), 1, image);
                fread( &((*bitmap)[i][j].r), sizeof(unsigned char), 1, image);
            }
            fseek(image, padding, SEEK_CUR);
        }
    }

	fclose(image);
}

int checkPinkPixel(Pixels p1){
	if( p1.r == 255 && p1.g == 175 && p1.b == 175){
		return 1;
	} else { 
		return 0;
	}
}

//turns the picture into a 9x9 matrix of numbers
int **getNumbers(Pixels **bitmap, bmp_infoheader *infoheader, bmp_fileheader *fileheader){

	int **numbers = (int**)calloc(9, sizeof(int*));
	if(numbers == NULL){
		printf("Allocation error\n");
		exit(EXIT_FAILURE);
	}
	

	for(int i = 0; i < 9; i++){
		numbers[i] = (int*)calloc(9,sizeof(int));
		if(numbers[i] == NULL){
			for(int j = 0; j < i; j++){
				free(numbers[j]);
			}
			free(numbers);
			printf("Allocation error\n");
			exit(EXIT_FAILURE);
		}
	}
	int col, lin = infoheader->height - 3;
	int linMatrice = 0;
	while(lin >= 0){

		col = 2;
		int nrCels = 0;

		while(nrCels < 9){

			int pixelsNo[5][2];
			int idx = 0;
			for(int i = lin; i >= lin-4; i--){

				int colIterator = col;
				int stop = col + 4;
				int nrOfPinkPixels = 0;
				int pozForOnePixelOnLine = 0;

				while(colIterator < infoheader->width && colIterator <= stop){
					if(checkPinkPixel(bitmap[i][colIterator])){
						nrOfPinkPixels++;
						pozForOnePixelOnLine = colIterator;
					}
					colIterator++;
				}
				pixelsNo[idx][0] = nrOfPinkPixels;
				if(pozForOnePixelOnLine != col){
					pixelsNo[idx++][1] = 1;	//dreapta
				} else {
					pixelsNo[idx++][1] = 0; //stanga
				}
			}
			if(pixelsNo[0][0] == 1){
				numbers[linMatrice][nrCels] = 1;

			} else if( pixelsNo[0][0] == 2){
				numbers[linMatrice][nrCels] = 4;
			} else if( pixelsNo[0][0] == 5){
				if(pixelsNo[1][0] == 1){

					if(pixelsNo[2][0] == 1){
						numbers[linMatrice][nrCels] = 7;

					} else if(pixelsNo[1][1] == 1){		//2 sau 3

						if(pixelsNo[3][1] == 1){
							numbers[linMatrice][nrCels] = 3;
						} else {
							numbers[linMatrice][nrCels] = 2;
						}
					} else {					//5 sau 6
						if(pixelsNo[3][0] == 2){
							numbers[linMatrice][nrCels] = 6;
						} else {
							numbers[linMatrice][nrCels] = 5;
						}
					}
				} else if(pixelsNo[1][0] == 2){		//8 sau 9
					if(pixelsNo[3][0] == 2){
						numbers[linMatrice][nrCels] = 8;
					} else {
						numbers[linMatrice][nrCels] = 9;
					}
				}
			} else {
				numbers[linMatrice][nrCels] = -1;
			}

			nrCels++;
			col = nrCels * 8 + 2;
		}

		linMatrice++;
		lin -= 8;
	}

	return numbers;
}

//check if the game is finished
int checkTable(int **numbers){

	// check special character or empty space
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(numbers[i][j] == -1){
				return 0;
			}
		}
	}

	for(int i = 0; i < 9; i++){
		for(int digit = 1; digit <= 9; digit++){
			int ok = 0;
			for(int j = 0; j < 9; j++){
				if(numbers[i][j] == digit){
					ok++;
				}
			}

			//there is no digit on a line
			//or there are 2 identical
			if(ok != 1){ 
				return 0;
			}
		}
	}

	for(int j = 0; j < 9; j++){
		for(int digit = 1; digit <= 9; digit++){
			int ok = 0;
			for(int i = 0; i < 9; i++){
				if(numbers[i][j] == digit){
					ok++;
				}
			}
			//there is no digit on a column
			//or there are 2 identical
			if(ok != 1){
				return 0;
			}
		}
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
		
			for(int digit = 1; digit <= 9; digit++){
				int ok = 0;
				for(int ii = i*3; ii < 3 * (i+1); ii++){
					for(int jj = j * 3; jj < 3 * (j+1); jj++){
						if(numbers[ii][jj] == digit){
							ok++;
						}
					}
				}

				if(ok != 1){
					return 0;
				}
			}
		}
	}

	return 1;
}

void writeJSON(const char *filename, int answer, char* inputFile){
	FILE* out = fopen(filename, "wt");
	if(out == NULL){
		printf("Failed to open file\n");
		exit(EXIT_FAILURE);
	}

	cJSON *root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "input_file", cJSON_CreateString(inputFile));
	if(answer == 1){
		cJSON_AddItemToObject(root, "game_state", cJSON_CreateString("Win!")); 
	} else {
		cJSON_AddItemToObject(root, "game_state", cJSON_CreateString("Loss :("));
	}

	char *outString = cJSON_Print(root);
	outString = (char*)realloc(outString, (strlen(outString) + 2 ) * sizeof(char));
	strcat(outString, "\n");
	fputs(outString, out);
	fclose(out);
	free(outString);
	cJSON_Delete(root);
}

//write a number at a specified location
void writeNumber(int lin, int col, int number, Pixels ***bitmap){
	Pixels magenta;
	magenta.r = 255;
	magenta.g = 0;
	magenta.b = 255;


	if(number == 1){
		
		for(int i = lin * 8 + 2; i <= lin * 8 + 6; i++){
			(*bitmap)[i][col * 8 + 6] = magenta;
		}
	}
	if(number == 2 || number == 3 || number == 5 || number == 6 ||
		number == 8 || number == 9){

		for(int j = col * 8 + 2; j <= col * 8 + 6; j++){
			(*bitmap)[lin * 8 + 2][j] = magenta;
			(*bitmap)[lin * 8 + 4][j] = magenta;
			(*bitmap)[lin * 8 + 6][j] = magenta;

		}

		if(number == 2){
			(*bitmap)[lin*8+5][col * 8 + 6] = magenta;
			(*bitmap)[lin*8+3][col * 8 + 2] = magenta;
		}
		if(number == 3){
			(*bitmap)[lin*8+3][col * 8 + 6] = magenta;
			(*bitmap)[lin*8+5][col * 8 + 6] = magenta;
		}
		if(number == 5){
			(*bitmap)[lin*8+5][col * 8 + 2] = magenta;
			(*bitmap)[lin*8+3][col * 8 + 6] = magenta;
		}
		if(number == 6){
			(*bitmap)[lin*8+3][col * 8 + 2] = magenta;
			(*bitmap)[lin*8+3][col * 8 + 6] = magenta;
			(*bitmap)[lin*8+5][col * 8 + 2] = magenta;
		}
		if(number == 8){
			(*bitmap)[lin*8+3][col * 8 + 2] = magenta;
			(*bitmap)[lin*8+3][col * 8 + 6] = magenta;
			(*bitmap)[lin*8+5][col * 8 + 2] = magenta;
			(*bitmap)[lin*8+5][col * 8 + 6] = magenta;

		}
		if(number == 9){
			(*bitmap)[lin*8+5][col * 8 + 2] = magenta;
			(*bitmap)[lin*8+5][col * 8 + 6] = magenta;
			(*bitmap)[lin*8+3][col * 8 + 6] = magenta;
		}
		

	}
	if(number == 4){
		(*bitmap)[lin*8+6][col * 8 + 2] = magenta;
		(*bitmap)[lin*8+6][col * 8 + 6] = magenta;
		(*bitmap)[lin*8+5][col * 8 + 2] = magenta;
		(*bitmap)[lin*8+5][col * 8 + 6] = magenta;


		(*bitmap)[lin*8+3][col * 8 + 6] = magenta;
		(*bitmap)[lin*8+2][col * 8 + 6] = magenta;

		for(int j = col * 8 + 2; j <= col * 8 + 6; j++){
			(*bitmap)[lin*8+4][j] = magenta;
		}
	}
	if(number == 7){
		for(int j = col * 8 + 2; j <= col * 8 + 6; j++){
			(*bitmap)[lin*8+6][j] = magenta;
		}

		for(int i = lin * 8 + 2; i <= lin * 8 + 5; i++){
			(*bitmap)[i][col * 8 + 6] = magenta;
		}
	}

}

int *getMissingOnLine(int **numbers, int line, int *size){

	int *misisngNumbers = (int*)malloc(9 * sizeof(int));
	if(misisngNumbers == NULL){
		printf("Allocation error\n");
		exit(EXIT_FAILURE);
	}


	int poz = 0; 
	for(int digit = 1; digit <= 9; digit++){

		int ok = 0;

		for(int i = 0; i < 9; i++){

			if(numbers[line][i] == digit){
				ok = 1;
			}

		}

		if(ok == 0){
			misisngNumbers[poz++] = digit;
		}
	}

	*size = poz;

	return misisngNumbers;
}

int *getMissingOnColumn(int **numbers, int col, int *size){

	int *misisngNumbers = (int*)malloc(9 * sizeof(int));
	if(misisngNumbers == NULL){
		printf("Allocation error\n");
		exit(EXIT_FAILURE);
	}

	int poz = 0; 
	for(int digit = 1; digit <= 9; digit++){

		int ok = 0;

		for(int i = 0; i < 9; i++){

			if(numbers[i][col] == digit){
				ok = 1;
			}

		}

		if(ok == 0){
			misisngNumbers[poz++] = digit;
		}
	}

	*size = poz;

	return misisngNumbers;

}

void completeTableTask4(Pixels ***bitmap, int **numbers){

	for(int i = 0; i < 9; i++){

		for(int j = 0; j < 9; j++){

			if(numbers[i][j] == -1){
				
				int sizeLine, sizeCol;
				int *missingLine = getMissingOnLine(numbers, i, &sizeLine);
				int *missingCol = getMissingOnColumn(numbers, j, &sizeCol);

				int ok = 0;
				
				for(int ii = sizeLine - 1; ii >= 0 && ok == 0; ii--){

					for(int jj = 0; jj < sizeCol && ok == 0; jj++){
						
						if(missingLine[ii] == missingCol[jj]){
							
							writeNumber(8 - i, j, missingLine[ii], bitmap);
							numbers[i][j] = missingLine[ii];
							ok = 1;
						}
						
					}
				}

				free(missingLine);
				free(missingCol);

			}
		}
	}

}

//check if a half full table has duplicate numbers on line/column
int checkTableTask4(int **numbers){

	//check if there are duplicates on line
	for(int i = 0; i < 9; i++){

		for(int j = 0; j < 8; j++){

			for(int k = j + 1; k < 9; k++){

				if(numbers[i][j] == numbers[i][k] && numbers[i][j] != -1){
					return 0;
				}
			}
			for(int k = j - 1; k >= 0; k--){

				if(numbers[i][j] == numbers[i][k] && numbers[i][j] != -1){
					return 0;
				}
			}
		}
	}

	//check if there are duplicates on column
	for(int j = 0; j < 9; j++){

		for(int i = 0; i < 8; i++){
			for(int k = i + 1; k < 9; k++){

				if(numbers[i][j] == numbers[k][j] && numbers[i][j] != -1){
					return 0;
				}
			}
			for(int k = i-1; k >= 0; k--){
				if(numbers[i][j] == numbers[k][j] && numbers[i][j] != -1){
					return 0;
				}
			}
		}
	}
	return 1;
}

void completeTableWithX(Pixels ***bitmap){

	Pixels red, white;
	red.r = 255;
	red.g = 0; 
	red.b = 0;

	white.r = 255;
	white.g = 255;
	white.b = 255;

	for(int i = 0; i < 9; i++){

		for(int j = 0; j < 9; j++){

			for(int lin = 8 * i + 2; lin <= 8*i+6; lin ++){

				for(int col = 8*j+2; col <= 8*j + 6; col++){

					if(lin%8 == col%8 || (lin%8 + col%8) == 8){
						(*bitmap)[lin][col] = red;
					} else {
						(*bitmap)[lin][col] = white;
					}
				}

			}
		}
	}
}

char *Task1(char *inputTask1){
	
	FILE *in = fopen(inputTask1, "r");
	if(in == NULL){
		printf("Failed to open input file\n");
		exit(EXIT_FAILURE);
	}

	bmp_infoheader *infoheader;
	bmp_fileheader *fileheader;
	Pixels **bitmap;

	readJSONFile(in, &infoheader, &fileheader, &bitmap);
	char *outputPath = getOutputFileName(inputTask1, OUTTASK1);

	writeImage(outputPath, infoheader, fileheader, bitmap, 1);

	freeMemory(fileheader, infoheader, bitmap);

	return outputPath;
}

char *Task2(char* input_file){
	
	FILE *in = fopen(input_file, "rb");
	if(in == NULL){
		printf("Failed to open input file\n");
		exit(EXIT_FAILURE);
	}

	bmp_fileheader *fileheader;
	bmp_infoheader *infoheader;
	Pixels **bitmap;

	readImage(in, &fileheader, &infoheader, &bitmap);

	char *outputPath = getOutputFileName(input_file, OUTTASK2);

	writeImage(outputPath, infoheader, fileheader, bitmap, 2);

	freeMemory(fileheader, infoheader, bitmap);

	return outputPath;
}

void Task3(char *input_file){

	FILE *in = fopen(input_file, "rb");
	if(in == NULL){
		printf("Failed to open input file\n");
		exit(EXIT_FAILURE);
	}

	bmp_fileheader *fileheader;
	bmp_infoheader *infoheader;
	Pixels **bitmap;

	readImage(in, &fileheader, &infoheader, &bitmap);

	int **nrs = getNumbers(bitmap, infoheader, fileheader);

	int answer = checkTable(nrs);

	char *outputPath = getOutputFileName(input_file, OUTTASK3);

	char boardName[100];

	strcpy(boardName, outputPath + 13);

	writeJSON(outputPath, answer, boardName);

	freeMemory(fileheader, infoheader, bitmap);

	free(outputPath);

	for(int i = 0; i < 9; i++)
		free(nrs[i]);

	free(nrs);
}

void Task4(char *input_file){

	FILE *in = fopen(input_file, "rb");
	if(in == NULL){
		printf("Failed to open input file\n");
		exit(EXIT_FAILURE);
	}

	bmp_fileheader *fileheader;
	bmp_infoheader *infoheader;
	Pixels **bitmap;

	readImage(in, &fileheader, &infoheader, &bitmap);

	int **nrs = getNumbers(bitmap, infoheader, fileheader);

	completeTableTask4(&bitmap, nrs);

	char *outputPath = getOutputFileName(input_file, OUTTASK4);

	writeImage(outputPath, infoheader, fileheader, bitmap, 4);

	free(outputPath);
	freeMemory(fileheader, infoheader, bitmap);

	for(int i = 0; i < 9; i++)
		free(nrs[i]);

	free(nrs);
}

int validCell(int **nrs, int row, int col, int number)
{
	int i;
	int newRow = (row / 3) * 3;
	int newCol = (col / 3) * 3;

	for (i = 0; i < 9; i++)	{
		if (nrs[row][i] == number) return 0;
        if (nrs[i][col] == number) return 0;
        if (nrs[newRow + (i % 3)][newCol + (i / 3)] == number) return 0;
	}

	return 1;
}

int solver(int ***nrs, int row, int col)
{
	int next_row = row + (col + 1) / 9;
	int next_col = (col + 1) % 9;
	int number;

	if ((*nrs)[row][col] != -1)
		return (next_row >= 9) || solver(nrs, next_row, next_col);

	for (number = 1; number <= 9; number++)
		if (validCell((*nrs), row, col, number))
		{
			(*nrs)[row][col] = number;

			if (next_row >= 9 || solver(nrs, next_row, next_col))
				return 1;

			(*nrs)[row][col] = -1;
		}
	
	return 0;
}

void bonus(char *input_file){
	FILE *in = fopen(input_file, "rb");
	if(in == NULL){
		printf("Failed to open input file\n");
		exit(EXIT_FAILURE);
	}
	bmp_fileheader *fileheader;
	bmp_infoheader *infoheader;
	Pixels **bitmap;
	readImage(in, &fileheader, &infoheader, &bitmap);

	int **nrs = getNumbers(bitmap, infoheader, fileheader);
	int copy[9][9];
	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++){
			copy[i][j] = nrs[i][j];
		}
	}
	if(checkTableTask4(nrs) == 0){
		completeTableWithX(&bitmap);
	} else {
		solver(&nrs, 0, 0);
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				if(copy[i][j] == -1){
					writeNumber(8 - i, j, nrs[i][j], &bitmap);
				}
			}
		}
	}
	char *outputPath = getOutputFileName(input_file, BONUSOUT);
	writeImage(outputPath, infoheader, fileheader, bitmap, 4);
	freeMemory(fileheader, infoheader, bitmap);
	free(outputPath);
	for(int i = 0; i < 9; i++)
		free(nrs[i]);
	free(nrs);
}

int main(int argc, char *argv[]){
	if(strstr(argv[1], TASK1) != 0){
		char *inputTask1 = strdup(argv[1]);
		char *inputTask2 = Task1(inputTask1);
		char *inputTask3 = Task2(inputTask2);
		Task3(inputTask3);
		free(inputTask1);
		free(inputTask2);
		free(inputTask3);
	} else if(strstr(argv[1], TASK4) != 0){
		Task4(argv[1]);
	} else if(strstr(argv[1], BONUS) != 0){
		bonus(argv[1]);
	}
	return 0;
}
