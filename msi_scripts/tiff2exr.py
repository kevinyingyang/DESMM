#!/usr/bin/env python

import sys
import numpy

# print "starting..."

import Image

# print sys.argv[1]
# print sys.argv[2]
# print sys.argv[3]
# print sys.argv[4]
# print sys.argv[5]
# print sys.argv[6]
# print sys.argv[7]
# print sys.argv[8]

value_max = numpy.float32(65535.0)

# red channel
tiff_fn = sys.argv[7]
red_img_data = Image.open(tiff_fn);
red_img_data = numpy.asarray(list(red_img_data.getdata()), dtype=numpy.uint16)
red_img_data = numpy.array(red_img_data,dtype=numpy.float32)
red_img_data = red_img_data/value_max
# print red_img_data[4435155]
red_img_data = numpy.array(red_img_data,dtype=numpy.float16)
# print red_img_data[4435155]

# green channel
tiff_fn = sys.argv[4]
green_img_data = Image.open(tiff_fn)
green_img_data = numpy.asarray(list(green_img_data.getdata()), dtype=numpy.uint16)
green_img_data = green_img_data/value_max
# print green_img_data[4435155]
green_img_data = numpy.array(green_img_data,dtype=numpy.float16)
# print green_img_data[4435155]

# blue channel
tiff_fn = sys.argv[2]
blue_img_data = Image.open(tiff_fn)
blue_img_data = numpy.asarray(list(blue_img_data.getdata()), dtype=numpy.uint16)
blue_img_data = blue_img_data/value_max
# print blue_img_data[4435155]
blue_img_data = numpy.array(blue_img_data,dtype=numpy.float16)
# print blue_img_data[4435155]

# F1 channel
tiff_fn = sys.argv[1]
F1_img_data = Image.open(tiff_fn)
F1_img_data = numpy.asarray(list(F1_img_data.getdata()), dtype=numpy.uint16)
F1_img_data = F1_img_data/value_max
# print F1_img_data[4435155]
F1_img_data = numpy.array(F1_img_data,dtype=numpy.float16)
# print F1_img_data[4435155]

# F2 channel
tiff_fn = sys.argv[2]
F2_img_data = Image.open(tiff_fn)
F2_img_data = numpy.asarray(list(F2_img_data.getdata()), dtype=numpy.uint16)
F2_img_data = F2_img_data/value_max
# print F2_img_data[4435155]
F2_img_data = numpy.array(F2_img_data,dtype=numpy.float16)
# print F2_img_data[4435155]

# F3 channel
tiff_fn = sys.argv[3]
F3_img_data = Image.open(tiff_fn)
F3_img_data = numpy.asarray(list(F3_img_data.getdata()), dtype=numpy.uint16)
F3_img_data = F3_img_data/value_max
# print F3_img_data[4435155]
F3_img_data = numpy.array(F3_img_data,dtype=numpy.float16)
# print F3_img_data[4435155]

# F4 channel
tiff_fn = sys.argv[4]
F4_img_data = Image.open(tiff_fn)
F4_img_data = numpy.asarray(list(F4_img_data.getdata()), dtype=numpy.uint16)
F4_img_data = F4_img_data/value_max
# print F4_img_data[4435155]
F4_img_data = numpy.array(F4_img_data,dtype=numpy.float16)
# print F4_img_data[4435155]

# F5 channel
tiff_fn = sys.argv[5]
F5_img_data = Image.open(tiff_fn)
F5_img_data = numpy.asarray(list(F5_img_data.getdata()), dtype=numpy.uint16)
F5_img_data = F5_img_data/value_max
# print F5_img_data[4435155]
F5_img_data = numpy.array(F5_img_data,dtype=numpy.float16)
# print F5_img_data[4435155]

# F6 channel
tiff_fn = sys.argv[6]
F6_img_data = Image.open(tiff_fn)
F6_img_data = numpy.asarray(list(F6_img_data.getdata()), dtype=numpy.uint16)
F6_img_data = F6_img_data/value_max
# print F6_img_data[4435155]
F6_img_data = numpy.array(F6_img_data,dtype=numpy.float16)
# print F6_img_data[4435155]

# F7 channel
tiff_fn = sys.argv[7]
F7_img_data = Image.open(tiff_fn)
F7_img_data = numpy.asarray(list(F7_img_data.getdata()), dtype=numpy.uint16)
F7_img_data = F7_img_data/value_max
# print F7_img_data[4435155]
F7_img_data = numpy.array(F7_img_data,dtype=numpy.float16)
# print F7_img_data[4435155]

# F8 channel
tiff_fn = sys.argv[8]
F8_img_data = Image.open(tiff_fn)
F8_img_data = numpy.asarray(list(F8_img_data.getdata()), dtype=numpy.uint16)
F8_img_data = F8_img_data/value_max
# print F8_img_data[4435155]
F8_img_data = numpy.array(F8_img_data,dtype=numpy.float16)
# print F8_img_data[4435155]

# w,h
im = Image.open(tiff_fn)
width = im.size[0]
height = im.size[1]

import OpenEXR
import Imath
import numpy

exr_fn = sys.argv[9]
red_img_data = red_img_data.tostring()
green_img_data = green_img_data.tostring()
blue_img_data = blue_img_data.tostring()
F1_img_data = F1_img_data.tostring()
F2_img_data = F2_img_data.tostring()
F3_img_data = F3_img_data.tostring()
F4_img_data = F4_img_data.tostring()
F5_img_data = F5_img_data.tostring()
F6_img_data = F6_img_data.tostring()
F7_img_data = F7_img_data.tostring()
F8_img_data = F8_img_data.tostring()

HEADER = OpenEXR.Header(width,height)
HEADER['channels'] = { 'R'      : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'B'      : Imath.Channel(Imath.PixelType(OpenEXR.HALF)), 
                       'G'      : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'w370nm' : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'w417nm' : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'w475nm' : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'w525nm' : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'w585nm' : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'w628nm' : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'w675nm' : Imath.Channel(Imath.PixelType(OpenEXR.HALF)),
                       'w755nm' : Imath.Channel(Imath.PixelType(OpenEXR.HALF)) }
# print HEADER
out_exr = OpenEXR.OutputFile(exr_fn, HEADER)

out_exr.writePixels({'B': blue_img_data, 'G': green_img_data, 'R': red_img_data, 'w370nm': F1_img_data, 'w417nm': F2_img_data, 'w475nm': F3_img_data, 'w525nm': F4_img_data, 'w585nm': F5_img_data, 'w628nm': F6_img_data, 'w675nm': F7_img_data, 'w755nm': F8_img_data})
print "write the EXR file ",exr_fn," done..."

