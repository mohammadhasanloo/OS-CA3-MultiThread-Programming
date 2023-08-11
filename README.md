# Image Filters Application with MultiThread Programming

This application processes images by applying various filters to them. It reads an input image file, performs the specified filtering operations, and then saves the modified image as an output file. The program provides both serial and parallel implementations of the image filtering operations.

## Table of Contents

- [Description](#description)
- [Serial Implementation](#serial-implementation)
- [Parallel Implementation](#parallel-implementation)

## Description

The Image Filters Application reads a BMP image, applies a series of filters, and saves the modified image. The supported filters include horizontal and vertical mirror effects, median filtering, color reversal, and adding a plus sign to the center of the image.

## Serial Implementation

The `serial.cpp` file contains a serial implementation of the image filtering operations. The image filters are applied sequentially, and the modified image is saved as "output.bmp".

## Parallel Implementation

The `parallel.cpp` file contains a parallel implementation of the image filtering operations. The image filters are applied concurrently using POSIX threads. The modified image is saved as "output.bmp".
