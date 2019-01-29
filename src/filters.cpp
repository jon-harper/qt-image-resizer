#include "filters.h"

QImage Filters::scale(QImage *img, const QSize &sz, double blur)
{
    if(img->isNull())
        return (*img);
    const int dw = sz.width();
    const int dh = sz.height();

    const bool hasAlpha = img->hasAlphaChannel();

    if (img->depth() != 32)
        *img = img->convertToFormat(hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    else if(img->format() == QImage::Format_ARGB32_Premultiplied)
        *img = img->convertToFormat(QImage::Format_ARGB32);

    QImage buffer(sz, hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);

    // Allocate filter contribution info.
    double x_factor = static_cast<double>(buffer.width())/static_cast<double>(img->width());
    double y_factor = static_cast<double>(buffer.height())/static_cast<double>(img->height());

    double support = dMax(blur * dMax(1.0/x_factor, 1.0) * Lanczos3Support,
                          blur * dMax(1.0/y_factor, 1.0) * Lanczos3Support);
    if(support < Lanczos3Support)
        support = Lanczos3Support;

    ContributionInfo *contribution = new ContributionInfo[static_cast<unsigned>(2.0*std::max<double>(support, 0.5)+3)];

    // Scale
    if((dw * (img->height() + dh)) > (dh * (img->width() + dw))){
        QImage tmp(dw, img->height(), buffer.format());
        horizontalFilter(img, &tmp, x_factor, contribution, blur);
        verticalFilter(&tmp, &buffer, y_factor, contribution, blur);
    }
    else{
        QImage tmp(img->width(), dh, buffer.format());
        verticalFilter(img, &tmp, y_factor, contribution, blur);
        horizontalFilter(&tmp, &buffer, x_factor, contribution, blur);
    }

    delete[] contribution;
    return buffer;
}

void Filters::verticalFilter(QImage *srcImg, QImage *destImg,
                                 double y_factor, ContributionInfo *contrib, double blur)
{
    //Image data
    auto sw = static_cast<unsigned int>(srcImg->width());
    auto dw = static_cast<unsigned int>(destImg->width());
    auto dh = static_cast<unsigned int>(destImg->height());
    const double sh = double(srcImg->height());

    QRgb *srcData = reinterpret_cast<QRgb *>(srcImg->bits());
    QRgb *destData = reinterpret_cast<QRgb *>(destImg->bits());
    QRgb pixel;

    //calculate scale and support values
    double scale = blur * std::max<double>(1.0/y_factor, 1.0);
    double support = scale * Lanczos3Support;
    if (support <= 0.5){
        support = 0.5 + epsilon;
        scale = 1.0;
    }
    scale = 1.0 / scale;

    //process
    double r, g, b, a, density, center;
    unsigned int i, n, x, y, start, stop;

    for (y = 0; y < dh; ++y) {
        density = 0.0;
        center = (y + 0.5)/y_factor;
        start = static_cast<unsigned int>(std::max<double>(center-support+0.5, 0.0));
        stop = static_cast<unsigned int>(std::min<double>(center+support+0.5, sh));

        for (n = 0; n < (stop-start); ++n) {
            contrib[n].pixel = start + n;
            contrib[n].weight = Lanczos3(scale*((start+n)-center+0.5));
            density += contrib[n].weight;
        }

        if ((density != 0.0) && (density != 1.0)) {
            // Normalize
            density = 1.0/density;
            for (i = 0; i < n; ++i)
                contrib[i].weight *= density;
        }

        for (x = 0; x < dw; ++x){
            r = g = b = a = 0;
            for (i = 0; i < n; ++i) {
                pixel = *(srcData + contrib[i].pixel * sw + x);
                r += qRed(pixel) * contrib[i].weight;
                g += qGreen(pixel) * contrib[i].weight;
                b += qBlue(pixel) * contrib[i].weight;
                a += qAlpha(pixel) * contrib[i].weight;
            }
            r = r < 0 ? 0 : r > 255 ? 255 : r + 0.5;
            g = g < 0 ? 0 : g > 255 ? 255 : g + 0.5;
            b = b < 0 ? 0 : b > 255 ? 255 : b + 0.5;
            a = a < 0 ? 0 : a > 255 ? 255 : a + 0.5;
            *(destData + y * dw + x) = qRgba(static_cast<unsigned char>(r),
                                             static_cast<unsigned char>(g),
                                             static_cast<unsigned char>(b),
                                             static_cast<unsigned char>(a));
        }
    }
}

void Filters::horizontalFilter(QImage *srcImg, QImage *destImg,
                                   double x_factor, ContributionInfo *contrib, double blur)
{
    auto sw = static_cast<unsigned int>(srcImg->width());
    auto dw = static_cast<unsigned int>(destImg->width());
    auto dh = static_cast<unsigned int>(destImg->height());

    //QImage internally stores its data as a vector of QRgb, so this is safe.
    auto srcData = reinterpret_cast<QRgb *>(srcImg->bits());
    auto destData = reinterpret_cast<QRgb *>(destImg->bits());
    QRgb pixel;

    double scale = blur * dMax(1.0/x_factor, 1.0);
    double support = scale * Lanczos3Support;
    if (support <= 0.5) {
        support = 0.5 + epsilon;
        scale = 1.0;
    }
    scale = 1.0/scale;

    double r, g, b, a, center, density;
    unsigned int x, y, i, n, start, stop;

    for (x = 0; x < dw; ++x) {
        center = (x + 0.5)/x_factor;
        start = static_cast<unsigned int>(dMax(center - support + 0.5, 0.0));
        stop = static_cast<unsigned int>(dMin(center + support + 0.5, static_cast<double>(sw)));
        density = 0.0;

        for(n = 0; n < (stop-start); ++n){
            contrib[n].pixel = start+n;
            contrib[n].weight = Lanczos3(scale*((start + n) - center+0.5));
            density += contrib[n].weight;
        }

        if((density != 0.0) && (density != 1.0)){
            // Normalize
            density = 1.0/density;
            for(i = 0; i < n; ++i)
                contrib[i].weight *= density;
        }

        for(y = 0; y < dh; ++y){
            r = g = b = a = 0;
            for(i = 0; i < n; ++i){
                pixel = *(srcData+(y*sw)+contrib[i].pixel);
                r += qRed(pixel)*contrib[i].weight;
                g += qGreen(pixel)*contrib[i].weight;
                b += qBlue(pixel)*contrib[i].weight;
                a += qAlpha(pixel)*contrib[i].weight;
            }
            r = r < 0 ? 0 : r > 255 ? 255 : r + 0.5;
            g = g < 0 ? 0 : g > 255 ? 255 : g + 0.5;
            b = b < 0 ? 0 : b > 255 ? 255 : b + 0.5;
            a = a < 0 ? 0 : a > 255 ? 255 : a + 0.5;
            *(destData+(y*dw)+x) = qRgba(static_cast<unsigned char>(r),
                                         static_cast<unsigned char>(g),
                                         static_cast<unsigned char>(b),
                                         static_cast<unsigned char>(a));
        }
    }
}
