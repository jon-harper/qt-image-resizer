/*
 Portions of this software are derived from QImageBlitz's work based on
 ImageMagick. QImageBlitz is copyrighted under the following conditions:

 Copyright (C) 2004, 2005, 2007
 Daniel M. Duley <daniel.duley@verizon.net>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 Portions of this software are based on or use ImageMagick's
 algorithms. ImageMagick is copyrighted under the following conditions:

Copyright (C) 2003 ImageMagick Studio, a non-profit organization dedicated to
making software imaging solutions freely available.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files ("ImageMagick"), to deal
in ImageMagick without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense,  and/or sell
copies of ImageMagick, and to permit persons to whom the ImageMagick is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of ImageMagick.

The software is provided "as is", without warranty of any kind, express or
implied, including but not limited to the warranties of merchantability,
fitness for a particular purpose and noninfringement.  In no event shall
ImageMagick Studio be liable for any claim, damages or other liability,
whether in an action of contract, tort or otherwise, arising from, out of or
in connection with ImageMagick or the use or other dealings in ImageMagick.

Except as contained in this notice, the name of the ImageMagick Studio shall
not be used in advertising or otherwise to promote the sale, use or other
dealings in ImageMagick without prior written authorization from the
ImageMagick Studio.
*/

#ifndef FILTERS_H
#define FILTERS_H

#include <cmath>
#include <QImage>

#include "global.h"

inline double dMax(const double &a, const double &b) noexcept { return (a < b) ? b : a; }
inline double dMin(const double &a, const double &b) noexcept { return (a < b) ? a : b; }

/*! \namespace Filters
 *  \brief Contains values and inline functions for image processing.
 */
namespace Filters {

struct ContributionInfo
{
    double weight;
    unsigned int pixel;
};

static constexpr double MagickPI = 3.14159265358979323846264338327950288419716939937510;
static constexpr double epsilon = 1.0e-6;

static constexpr double Lanczos2Support = 2.0;
static constexpr double Lanczos3Support = 3.0;
static constexpr double Lanczos4Support = 4.0;

inline double Sinc(const double &x)
{
    const double alpha = MagickPI*x;
    return std::sin(alpha)/alpha;
}

static double SincFast(const double x)
{
    //Approximations of the sinc function sin(pi x)/(pi x) over the interval
    //[-4,4] constructed by Nicolas Robidoux and Chantal Racette with funding
    //from the Natural Sciences and Engineering Research Council of Canada.
    //Although the approximations are polynomials (for low order of
    //approximation) and quotients of polynomials (for higher order of
    //approximation) and consequently are similar in form to Taylor polynomials /
    //Pade approximants, the approximations are computed with a completely
    //different technique.
    //Summary: These approximations are "the best" in terms of bang (accuracy)
    //for the buck (flops). More specifically: Among the polynomial quotients
    //that can be computed using a fixed number of flops (with a given "+ - * /
    //budget"), the chosen polynomial quotient is the one closest to the
    //approximated function with respect to maximum absolute relative error over
    //the given interval.
    //The Remez algorithm, as implemented in the boost library's minimax package,
    //is the key to the construction: http://www.boost.org/doc/libs/1_36_0/libs/
    //math/doc/sf_and_dist/html/math_toolkit/backgrounders/remez.html

    //If outside of the interval of approximation, use the standard trig formula.
    if (x > 4.0) {
        return Sinc(x);
    }

    //The approximations only depend on x^2 (sinc is an even function).
    const double xx = x*x;
    //Maximum absolute relative error 6.3e-6 < 1/2^17.
    static constexpr double c0 = 0.173610016489197553621906385078711564924e-2L;
    static constexpr double c1 = -0.384186115075660162081071290162149315834e-3L;
    static constexpr double c2 = 0.393684603287860108352720146121813443561e-4L;
    static constexpr double c3 = -0.248947210682259168029030370205389323899e-5L;
    static constexpr double c4 = 0.107791837839662283066379987646635416692e-6L;
    static constexpr double c5 = -0.324874073895735800961260474028013982211e-8L;
    static constexpr double c6 = 0.628155216606695311524920882748052490116e-10L;
    static constexpr double c7 = -0.586110644039348333520104379959307242711e-12L;
    const double p = c0+xx*(c1+xx*(c2+xx*(c3+xx*(c4+xx*(c5+xx*(c6+xx*c7))))));
    return((xx-1.0)*(xx-4.0)*(xx-9.0)*(xx-16.0)*p);
}

inline double Lanczos3(const double &x)
{
    if (x < -3.0)
        return 0.0;
    if (x < 0.0)
        return SincFast(-x)*SincFast(-x/3.0);
    if (x < 3.0)
        return SincFast(x)*SincFast(x/3.0);
    return 0.0;
}

inline double Lanczos4(const double &x)
{
    if (x < -4.0)
        return 0.0;
    if (x < 0.0)
        return SincFast(-x)*SincFast(-x/4.0);
    if (x < 4.0)
        return SincFast(x)*SincFast(x/4.0);
    return 0.0;
}

QImage scale(QImage *img, const QSize &sz, double blur);
void horizontalFilter(QImage *srcImg, QImage *destImg,
                      double x_factor, ContributionInfo *contrib, double blur);
void verticalFilter(QImage *srcImg, QImage *destImg,
                    double y_factor, ContributionInfo *contrib, double blur);

} //namespace Filters

#endif // FILTERS_H
