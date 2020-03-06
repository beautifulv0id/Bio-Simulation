#ifndef INCLUDED__BIOAPPLICATION__TGA__HPP
#define INCLUDED__BIOAPPLICATION__TGA__HPP
#include <string>
#include <vector>
#include <fstream>
#include <QPixmap>


struct TGA_HEADER;

/**
 *****************************************************************************
 *
 *  This class is used to load TGA pictures from a file
 *  and store it in a shared pointer.
 *
 *****************************************************************************/

class TGA
{
public:
    static std::shared_ptr<TGA> readTGA(const std::string& img_path);
    QPixmap toQPixmap();

    const std::vector<char> pixel_;
    const int width_;
    const int height_;
    const bool alpha_;

private:
    TGA(std::vector<char> pixel, int width, int height, bool alpha);
    static const TGA_HEADER leseHeader(std::ifstream &file);
};


/**
 *****************************************************************************
 *
 *  This class is used to store the TGA header (in a compact struct)
 *
 *****************************************************************************/

// Source https://www.c-plusplus.net/forum/157181-full
#pragma pack( push, 1 )
struct TGA_HEADER
{
    char  identsize_;
    char  colourmaptype_;
    char  imagetype_;

    short colourmapstart_;
    short colourmaplength_;
    char  colourmapbits_;

    short xstart_;
    short ystart_;
    short width_;
    short height_;
    char  bits_;
    char  descriptor_;
};
#pragma pack( pop )

#endif // INCLUDED__BIOAPPLICATION__TGA__HPP
