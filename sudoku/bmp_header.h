/* Tells the compiler not to add padding for these structs. This may
   be useful when reading/writing to binary files.
   http://stackoverflow.com/questions/3318410/pragma-pack-effect
*/
#pragma pack(1)

typedef struct 
{
    unsigned char  fileMarker1;     //singnature[0]                 /* 'B' */
    unsigned char  fileMarker2;     //singnature[1]                 /* 'M' */
    unsigned int   bfSize;          //file_size                     /* File's size */
    unsigned short unused1;                     /* Aplication specific */
    unsigned short unused2;                     /* Aplication specific */
    unsigned int   imageDataOffset; //offset             /* Offset to the start of image data */
} bmp_fileheader;

typedef struct 
{
    unsigned int   biSize;          //size                                  /* Size of the info header - 40 bytes */
    signed int     width;           //width                               /* Width of the image */
    signed int     height;          //height                               /* Height of the image */
    unsigned short planes;          //planes
    unsigned short bitPix;          //bit_count                                /* Number of bits per pixel = 3 * 8 (for each channel R, G, B we need 8 bits */
    unsigned int   biCompression;   //compression                          /* Type of compression */
    unsigned int   biSizeImage;     //image_size                          /* Size of the image data */
    int            biXPelsPerMeter; //x_pixels_per_meter
    int            biYPelsPerMeter; //y_pixels_per_meter
    unsigned int   biClrUsed;       //colors_used
    unsigned int   biClrImportant;  //colors_important
} bmp_infoheader;

#pragma pack()
