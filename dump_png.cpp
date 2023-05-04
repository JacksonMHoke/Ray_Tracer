#include <png.h>
#include <cassert>

typedef unsigned int Pixel;

void Dump_png(Pixel* data,int width,int height,const char* filename)
{
    FILE* file=fopen(filename,"wb");
    assert(file);

    png_structp png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
    assert(png_ptr);
    png_infop info_ptr=png_create_info_struct(png_ptr);
    assert(info_ptr);
    bool result=setjmp(png_jmpbuf(png_ptr));
    assert(!result);
    png_init_io(png_ptr,file);
    int color_type=PNG_COLOR_TYPE_RGBA;
    png_set_IHDR(png_ptr,info_ptr,width,height,8,color_type,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);

    Pixel** row_pointers=new Pixel*[height];
    for(int j=0;j<height;j++) row_pointers[j]=data+width*(height-j-1);
    png_set_rows(png_ptr,info_ptr,(png_byte**)row_pointers);
    png_write_png(png_ptr,info_ptr,PNG_TRANSFORM_BGR|PNG_TRANSFORM_SWAP_ALPHA,0);
    delete[] row_pointers;
    png_destroy_write_struct(&png_ptr,&info_ptr);
    fclose(file);
}

void Read_png(Pixel*& data,int& width,int& height,const char* filename)
{
    FILE *file = fopen(filename, "rb");
    assert(file);

    unsigned char header[8];
    int num_read=fread(&header, 1, sizeof header, file);
    assert(num_read==sizeof header);
    int ret_sig=png_sig_cmp((png_bytep)header, 0, sizeof header);
    assert(!ret_sig);
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    assert(png_ptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    assert(info_ptr);
    png_infop end_info = png_create_info_struct(png_ptr);
    assert(end_info);
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, sizeof header);
    png_read_info(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    if(bit_depth == 16)
        png_set_strip_16(png_ptr);

    if(bit_depth < 8)
        png_set_packing(png_ptr);
    
    if(color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_swap_alpha(png_ptr);

    if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_bgr(png_ptr);

    if(color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

    height = png_get_image_height(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);

    data = new Pixel[width * height];

    png_read_update_info(png_ptr, info_ptr);

    for(int i = 0; i < height; i++)
        png_read_row(png_ptr, (png_bytep)(data + (height-i-1) * width), 0);

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(file);
}
