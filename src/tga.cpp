#include "tga.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <QPixmap>
#include "myexceptions.hpp"

TGA::TGA(std::vector<char> pixel, int width, int height, bool alpha) :
    pixel_(std::move(pixel)), width_(width), height_(height), alpha_(alpha)
{

}

std::shared_ptr<TGA> TGA::readTGA(const std::string& img_path)
{

    std::ifstream file( img_path, std::ios::binary | std::ios::in );

    if( !file.is_open() ){
        std::cout << "Error: opening file failed." << std::endl;
        throw TGALoadException();
    }

    const TGA_HEADER header_s(leseHeader(file));

    /* test for correct format */
    if(header_s.identsize_ != 0 || header_s.colourmaptype_ != 0 ||
            header_s.colourmapstart_ != 0 || header_s.colourmaplength_ != 0 ||
                header_s.xstart_ != 0 || header_s.ystart_ !=0 ||
                (header_s.bits_ != 24 && header_s.bits_ != 32) || header_s.imagetype_ != 2){

        std::cout << "Error: wrong format." << std::endl;
        throw TGALoadException();
    }

    bool alpha = header_s.bits_ == 32 ? true : false;
    int width = header_s.width_;
    int height = header_s.height_;
    std::vector<char> pixel;

    /* write TGA pixel data to pixel_ vector.
     * in case of no alpha channel, the pixels need to be reorderd
     * from BGR to RGB format.
     */
    if(alpha){
        for(int i = 0; i < (width * height); i++){
            for(int j = 0; j < 4; j++){
                char p;
                file.read(reinterpret_cast<char*> (&p), sizeof(char));
                pixel.push_back(p);
            }
        }
    }

    else{
        for(int i = 0; i < (width * height); i++){
            unsigned char b, g, r;
            file.read(reinterpret_cast<char*> (&b), sizeof(unsigned char));
            file.read(reinterpret_cast<char*> (&g), sizeof(unsigned char));
            file.read(reinterpret_cast<char*> (&r), sizeof(unsigned char));
            pixel.push_back(r);
            pixel.push_back(g);
            pixel.push_back(b);
        }
    }

    std::shared_ptr<TGA> tga(new TGA(std::move(pixel), width, height, alpha));

    return tga;
}


const TGA_HEADER TGA::leseHeader(std::ifstream &file)
{

    TGA_HEADER header;
    file.read(reinterpret_cast<char*>(& header), sizeof(header));
    if( file.bad() ){
        std::cout << "Error reading header failed." << std::endl;
        throw TGALoadException();
    }
    return header;
}

QPixmap TGA::toQPixmap()
{
    std::shared_ptr<unsigned char> c((unsigned char*) malloc(pixel_.size()));
    std::memcpy(c.get(), pixel_.data(), pixel_.size());
    QImage qImg;
    if(alpha_) qImg = QImage(c.get(), width_, height_, QImage::Format_ARGB32);
    else qImg = QImage(c.get(), width_, height_, QImage::Format_RGB888);
    qImg = qImg.mirrored(false, true);
    qImg = qImg.scaledToHeight(32, Qt::FastTransformation);
    qImg = qImg.scaledToWidth(32, Qt::FastTransformation);
    if(qImg.isNull()) return QPixmap();
    return QPixmap::fromImage(qImg);
}
