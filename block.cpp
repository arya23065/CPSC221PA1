
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
    for (int x = 0; x <= 12; x++) {
      vector<HSLAPixel> currBlock;
      // fills currBlock with left to right pixels of top row then next row etc.
      for (unsigned i = 0; i < (unsigned)dimension; i++) {
        for (unsigned j = 0; j < (unsigned)dimension; j++) {
          HSLAPixel* currPixel = im.getPixel(temp_upper + i, temp_left + j);
          currBlock.push_back(*currPixel);
        }
      }
      if (x <= 4) {
        temp_left += x * dimension + 1;
      } else if (x <= 8) {
        temp_upper += dimension + 1;
        temp_left += (x - 5) * dimension + 1;
      } else {
        temp_upper += 2 * dimension + 1;
        temp_left += (x - 9) * dimension + 1;
      }
      data.push_back(currBlock);
    }
  }

  // write the pixel colour data fom data attribute into im,
  //   with upper-left corner at (left, upper)
  // PRE: upper and left (and upper + dimension, left + dimension) are valid
  //        vector indices
  void Block::Render(PNG& im, int upper, int left) const {
    //reverse of build?
    int temp_upper = upper;
    int temp_left = left;
    for (int x = 0; x <= 12; x++) {
      vector<HSLAPixel> currBlock = data.front();
      HSLAPixel currPixel = data.front(); // doesnt work - type error
      const int dimension = Dimension();

      for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
          HSLAPixel* blankPixel = im.getPixel(temp_upper + i, temp_left + j);
          HSLAPixel currPixel = data.at(i * (int) dimension + j); // doesnt work - type error
          blankPixel = *currPixel;
        }
      }
    }


  }

  // "Reverse" the Hue and Luminance channels for each pixel in the data attribute
  //   to simulate a photo-negative effect.
  // Refer to the HSLAPixel documentation to determine an appropriate transformation
  //   for "reversing" hue and luminance.
  void Block::Negative() {

  }

  // Return the horizontal (or vertical) size of the data block's image region
 int Block::Dimension() const {
   vector<HSLAPixel> currBlock = data.front();
   return (int)sqrt(currBlock.size());
 }
