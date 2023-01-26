# qt-image-resizer

This is a simple application I wrote around 2013. It is
updated to use Qt5 and C++17 where applicable and shows good use of 
`QObject::moveToThread` and queued signals and slots. It uses
ImageMagick's Lanczos3 implementation and contains a number of widgets
that may be useful to other implementers.

The code is not completely doxygenated, but most of the code is self-documenting or
contains inline comments.

The doxygen stylesheet is outdated and has not been tested in years. You
may wish to use your own or the default stylesheet instead.

## License

With the exclusion of `filters.h` and `filters.cpp`, this software is licensed
under the GPLv3. The filters implementation and definitions are separately licensed
under QImageBlitz and ImageMagick's licenses.

## Why the Weird Licensing for Two Files?

I originally updated QImageBlitz for C++11 in 2012/2013 and optimized it for speed.
I wrote this application around the resulting filters.h. QImageBlitz has its
own license that sits on top of ImageMagick's license. I later rewrote the application code but kept the core scaling and filtering functions the same.

Summary: Everything is GPLv3 but filters.h and filters.cpp. Those two files are unre-useable
except as separately licensed code.
