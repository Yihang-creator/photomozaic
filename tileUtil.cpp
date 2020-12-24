
// File:        tileUtil.cpp
// Author:      Cinda
// Date:        2020-10-31
// Description: file handling functions that prepare data for the tiling algorithm


#include "tileUtil.h"

/**
 * Function tile:
 * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
 *                      be replaced by thumbnail images whose average color is close
 *                      to the pixel.
 * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
 *                      nearest neighbor search. 
 * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
 *                      filename of an image whose average color is that key.
 *
 * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
 * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
 * is used as a key in photos. 
 */

PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos){

/* your code here! */
    int width = target.width();
    int height = target.height();
    PNG photoMozaic = PNG(TILESIZE*width,TILESIZE*height);
    for (int i = 0; i<width; i++) {
        for (int j = 0; j <height; j++) {
            RGBAPixel nearestNeighbor = ss.findNearestNeighbor(*target.getPixel(i,j));
            PNG replaceImage;
            replaceImage.readFromFile(photos.find(nearestNeighbor)->second);
            replaceImage.resize(TILESIZE,TILESIZE);
            for (int k = 0; k< TILESIZE; k++) {
                for (int m = 0; m< TILESIZE; m++) {
                    *photoMozaic.getPixel(TILESIZE*i + k, TILESIZE*j + m) = *replaceImage.getPixel(k,m);
                }
            }
        }
    }
    return photoMozaic;

}

/* buildMap: function for building the map of <key, value> pairs, where the key is an
 * RGBAPixel representing the average color over an image, and the value is 
 * a string representing the path/filename.png of the TILESIZExTILESIZE image
 * whose average color is the key.
 * 
 * We've provided a bit of the C++ code that allows you to iterate over the files
 * in a directory. It is up to you to figure out what it means, and how to use it.
 * 
 * @param path is the subdirectory in which the tiles can be found. In our examples
 * this is imlib.
 *
*/
map<RGBAPixel, string> tiler::buildMap(string path) {

    map < RGBAPixel, string> thumbs;
    for (const auto & entry : fs::directory_iterator(path)) {
        PNG curr; curr.readFromFile(entry.path());
    /* your code here */
        int height = curr.height();
        int width  = curr.width();
        int size = height*width;
        int sumRed = 0;
        int sumGreen = 0;
        int sumBlue = 0;
        int sumAlpha = 0;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                RGBAPixel* pixel = curr.getPixel(i,j);
                sumRed+= pixel->r;
                sumGreen+= pixel->g;
                sumBlue+= pixel->b;
                sumAlpha+= pixel->a;
            }
        }
        RGBAPixel average(sumRed/size,sumGreen/size,sumBlue/size,sumAlpha/size);
        thumbs.insert(std::pair<RGBAPixel,string> (average,entry.path()));
    }
    return thumbs;
}


