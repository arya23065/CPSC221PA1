
#include <cmath>
#include "block.h"

using namespace std;
using namespace cs221util;


  // set values in data attribute, based on a dimension x dimension square region
  //   with upper-left corner at (left, upper) in the input PNG image
  // The orientation of the pixels in the data vector must match the orientation
  //   of the pixels in the PNG.
  // PRE: upper and left (and upper + dimension, left + dimension) are valid
  //        vector indices
  void Block::Build(PNG& im, int upper, int left, int dimension) {
    int temp_upper = upper;
    int temp_left = left;

      // fills currBlock with left to right pixels of top row then next row etc.
      for (unsigned i = 0; i < (unsigned)dimension; i++) {
        vector<HSLAPixel> currRow;
        for (unsigned j = 0; j < (unsigned)dimension; j++) {
          HSLAPixel* currPixel = im.getPixel(temp_left + j, temp_upper + i);
          currRow.push_back(*currPixel);
        }
        data.push_back(currRow);
      }
  }

  // write the pixel colour data fom data attribute into im,
  //   with upper-left corner at (left, upper)
  // PRE: upper and left (and upper + dimension, left + dimension) are valid
  //        vector indices
  void Block::Render(PNG& im, int upper, int left) const {
    int temp_upper = upper;
    int temp_left = left;
    const int dimension = Dimension();

    for (int i = 0; i < dimension; i++) {
      for (int j = 0; j < dimension; j++) {
        HSLAPixel* blankPixel = im.getPixel((unsigned) temp_left + j, (unsigned) temp_upper + i);
        HSLAPixel currPixel = data.at(i).at(j);
        *blankPixel = currPixel;
      }
    }
  }


  // "Reverse" the Hue and Luminance channels for each pixel in the data attribute
  //   to simulate a photo-negative effect.
  // Refer to the HSLAPixel documentation to determine an appropriate transformation
  //   for "reversing" hue and luminance.
  void Block::Negative() {
    const int dimension = Dimension();

    for (int i = 0; i < dimension; i++)  {
      for (int j = 0; j < dimension; j++) {
        HSLAPixel currPixel = data.at(i).at(j);
        double h = currPixel.h;
        currPixel.h = fmod(h + 180, 360);
        double l = currPixel.l;
        currPixel.l = 1.0 - l;
        data.at(i).at(j) = currPixel;
      }
    }
  }

  // Return the horizontal (or vertical) size of the data block's image region
 int Block::Dimension() const {
   vector<HSLAPixel> currBlock = data.front();
   return currBlock.size();
 }
