#define DATA_TYPE int
typedef struct tagIRImage
{
	DATA_TYPE* buffer;
	unsigned short channel_count;
	unsigned int widthTotalByte;
	int width;
	int height;
} IRImage, *PIRImage;

PIRImage ir_load_image(const char* fn);
