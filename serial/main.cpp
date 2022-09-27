#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using namespace std;

#pragma pack(1)
#pragma once

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;


typedef struct tagBITMAPFILEHEADER
{
  WORD bfType;
  DWORD bfSize;
  WORD bfReserved1;
  WORD bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

int rows;
int cols;

typedef struct RGB {
    vector<vector<int>> r;
    vector<vector<int>> g;
    vector<vector<int>> b;
} RGB;

RGB image, image_copy;

bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
{
  std::ifstream file(fileName);

  if (file)
  {
    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new char[length];
    file.read(&buffer[0], length);

    PBITMAPFILEHEADER file_header;
    PBITMAPINFOHEADER info_header;

    file_header = (PBITMAPFILEHEADER)(&buffer[0]);
    info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
    rows = info_header->biHeight;
    cols = info_header->biWidth;
    bufferSize = file_header->bfSize;
    return 1;
  }
  else
  {
    cout << "File" << fileName << " doesn't exist!" << endl;
    return 0;
  }
}

void getPixlesFromBMP24(int end, int rows, int cols, char *fileReadBuffer)
{
  int count = 1;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    count += extra;
    image.r.emplace_back(cols, 0);
    image.g.emplace_back(cols, 0);
    image.b.emplace_back(cols, 0);    
    for (int j = cols - 1; j >= 0; j--)
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
          // fileReadBuffer[end - count] is the red value
          image.r[i][j] = fileReadBuffer[end - count];
          count++;
          break;
        case 1:
          // fileReadBuffer[end - count] is the green value
          image.g[i][j] = fileReadBuffer[end - count];
          count++;
          break;
        case 2:
          // fileReadBuffer[end - count] is the blue value
          image.b[i][j] = fileReadBuffer[end - count];
          count++;
          break;
        // go to the next position in the buffer
        }
      }
  }

  /*
  for(int i=0;i< rows;i++){
    for(int j=0;j<cols;j++){
      cout << image.r[i][j] << endl;
      cout << image.g[i][j] << endl;
      cout << image.b[i][j] << endl;
    }
  }
  */

}

void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize)
{
  std::ofstream write(nameOfFileToCreate);
  if (!write)
  {
    cout << "Failed to write " << nameOfFileToCreate << endl;
    return;
  }
  int count = 1;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    count += extra;
    for (int j = cols - 1; j >= 0; j--)
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
          // write red value in fileBuffer[bufferSize - count]
          fileBuffer[bufferSize - count] = image.r[i][j];
          count++;
          break;
        case 1:
          // write green value in fileBuffer[bufferSize - count]
          fileBuffer[bufferSize - count] = image.g[i][j];
          count++;
          break;
        case 2:
          // write blue value in fileBuffer[bufferSize - count]
          fileBuffer[bufferSize - count] = image.b[i][j];
          count++;
          break;
        // go to the next position in the buffer
        }
      }
  }
  write.write(fileBuffer, bufferSize);
}

void apply_horizontal_mirror_filter(){
  for(int i=0;i<rows;i++)
  {
    reverse(image.r[i].begin(), image.r[i].end());
    reverse(image.g[i].begin(), image.g[i].end());
    reverse(image.b[i].begin(), image.b[i].end());
  }
}

void apply_vectical_mirror_filter(){
  reverse(image.r.begin(), image.r.end());
  reverse(image.g.begin(), image.g.end());
  reverse(image.b.begin(), image.b.end());
}

int calculate_median(char rgb, int row, int col) {
    vector<int> pixels;
    int median;

    if (rgb == 'r'){
      for(int i =row-1; i<=row+1; i++) {
        for(int j= col-1; j<=col+1; j++) {
          pixels.push_back(image_copy.r[i][j]);
        }
      }
    }
    else if (rgb == 'g'){
      for(int i =row-1; i<=row+1; i++) {
        for(int j= col-1; j<=col+1; j++) {
          pixels.push_back(image_copy.g[i][j]);
        }
      }
    }
    else if (rgb == 'b'){
      for(int i =row-1; i<=row+1; i++) {
        for(int j= col-1; j<=col+1; j++) {
          pixels.push_back(image_copy.b[i][j]);
        }
      }
    }        
    sort(pixels.begin(), pixels.end());
    median = pixels[(pixels.size()+1)/2];
    //cout << median << endl;

    return median;
}


void apply_median_filter() {
    image_copy = image;
    for(int i=1; i<rows-1; i++) {
        for(int j=1; j<cols-1; j++) {
            //cout << image.r[i][j] << endl;
            image.r[i][j] = calculate_median('r', i, j);
            image.g[i][j] = calculate_median('g', i, j);
            image.b[i][j] = calculate_median('b', i, j);
        }
    }
}

int decrease_from_255(int& i)
{
  return 255 - i;
}

void apply_reverse_color_filter(){
  for(int i=0;i<rows;i++)
  {
    transform(image.r[i].begin(), image.r[i].end(), image.r[i].begin(), &decrease_from_255);
    transform(image.g[i].begin(), image.g[i].end(), image.g[i].begin(), &decrease_from_255);
    transform(image.b[i].begin(), image.b[i].end(), image.b[i].begin(), &decrease_from_255);
  }
}

void add_plus_sign_to_image() {
  int center_of_rows;
  int center_of_cols;

  if(rows%2 == 0)
    center_of_rows = rows/2;
  else
    center_of_rows = (rows+1)/2;

  if(cols%2 == 0)
    center_of_cols = cols/2;
  else
    center_of_cols = (cols+1)/2;

  for(int i=0;i<rows;i++)
  {
    image.r[i][center_of_cols] = 255;
    image.g[i][center_of_cols] = 255;
    image.b[i][center_of_cols] = 255;

    image.r[i][center_of_cols - 1] = 255;
    image.g[i][center_of_cols - 1] = 255;
    image.b[i][center_of_cols - 1] = 255;

    image.r[i][center_of_cols + 1] = 255;
    image.g[i][center_of_cols + 1] = 255;
    image.b[i][center_of_cols + 1] = 255;
  }  

    for(int i=0;i<cols;i++)
  {
    image.r[center_of_rows][i] = 255;
    image.g[center_of_rows][i] = 255;
    image.b[center_of_rows][i] = 255;

    image.r[center_of_rows - 1][i] = 255;
    image.g[center_of_rows - 1][i] = 255;
    image.b[center_of_rows - 1][i] = 255;

    image.r[center_of_rows + 1][i] = 255;
    image.g[center_of_rows + 1][i] = 255;
    image.b[center_of_rows + 1][i] = 255;
  } 
}

int main(int argc, char *argv[])
{
  char *fileBuffer;
  int bufferSize;
  char *fileName = argv[1];
  auto started = chrono::high_resolution_clock::now();

  if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize))
  {
    cout << "File read error" << endl;
    return 1;
  }

  // read input file

  //cout << bufferSize << endl;
  //cout << fileBuffer << endl;
  getPixlesFromBMP24(bufferSize, rows, cols, fileBuffer);

  // apply filters
  apply_horizontal_mirror_filter();
  apply_vectical_mirror_filter();
  apply_median_filter();
  apply_reverse_color_filter();
  add_plus_sign_to_image();

  // write output file
  writeOutBmp24(fileBuffer, "output.bmp", bufferSize);

  auto done = chrono::high_resolution_clock::now();
  cout << chrono::duration_cast<chrono::milliseconds>(done - started).count() << endl;

  return 0;
}