struct ouaheader
{
    unsigned long magic_num;
    unsigned int fps;
    unsigned long pic_num;
    unsigned int width;
    unsigned int height;
    int spotx;
    int spoty;

    unsigned int clr_num;
    unsigned int clr_byte;

    void calc_color_byte()
    {
        clr_byte = 1;
        while((1 << (clr_byte - 1)) < clr_num) clr_byte++;

        while(clr_byte % 8 != 0) clr_byte++;
        clr_byte /= 8;
    }
};

const int magic_str = 0x41474F58;

struct pixinfo
{
    unsigned int idx;
    unsigned short repeat;
};
