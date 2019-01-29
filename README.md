# qt-image-resizer

This is a (relatively) simple application I wrote in around 2013 and have largely
updated to use Qt5 and C++17 where applicable. It shows good use of 
`QObject::moveToThread` and queued signals and slots, produces fast, quality results
using ImageMagick's Lanczos3 implementation, and contains a number of widgets
that may be useful to other implementers.

It's not completely doxygenated, but most of the code is self-documenting or
contains inline comments.

I haven't tested the doxygen stylesheet in ages and it probably looks awful. You
might want to use your own or the default instead.

## License

With the exclusion of `filters.h` and `filters.cpp`, this software is licensed
under the GPLv3. The filters implementation and definitions are separately licensed
under QImageBlitz and ImageMagick's licenses.

## Why the Weird Licensing for Two Files?

I originally updated QImageBlitz for C++11 in 2012/2013 and optimized it for speed,
then wrote this application around the resulting filters.h. QImageBlitz has its
own license that sits on top of ImageMagick's license. I later rewrote the application code (three times), but kept the core scaling and filtering functions the same.

In 2018/2019, I brought into filters.h ImageMagick's more recent, faster SincFast and updated the code with inline constexpr constants.

tl;dr: Everything is GPLv3 but filters.h and filters.cpp. Those two files are unre-useable
except as separately licensed code.