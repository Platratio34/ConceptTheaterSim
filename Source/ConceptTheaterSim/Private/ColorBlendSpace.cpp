// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorBlendSpace.h"

void UColorBlendSpace::addColor(TArray<uint8> color)
{
    HSVColor hsv = rgbToHSV(color);
    colors.Add(hsv);
}

TArray<uint8> UColorBlendSpace::blend(TArray<float> weights)
{
    // XYColor outColor;
    // for (int i = 0; i < colors.Num(); i++) {
    //     if(weights[i] <= 0) // no need to blend if the color is not present
    //         continue;
    //     HSVColor color = colors[i];
    //     // XYColor xy = 
    // }
    TArray<uint8> outRGB;
    return outRGB;
}

HSVColor UColorBlendSpace::rgbToHSV(TArray<uint8> rgb)
{
    HSVColor hsv;

    float r = ((float)rgb[0]) / 255.0f;
    float g = ((float)rgb[1]) / 255.0f;
    float b = ((float)rgb[2]) / 255.0f;

    float cmax = max(r, max(g, b));
    float cmin = min(r, min(g, b));
    float diff = cmax - cmin;

    if(cmax == cmin) {
        hsv.hue = 0;
    } else if (cmax == r) {
        hsv.hue = fmod(60 * ((g - b) / diff) + 360, 360) / 360.0f;
    } else if (cmax == g) {
        hsv.hue = fmod(60 * ((r - b) / diff) + 360, 360) / 360.0f;
    } else if (cmax == b) {
        hsv.hue = fmod(60 * ((r - g) / diff) + 360, 360) / 360.0f;
    }
    
    if (cmax == 0) {
        hsv.saturation = 0;
    } else {
        hsv.saturation = (diff / cmax);
    }

    hsv.value = cmax;
    return hsv;
}

XYYColor UColorBlendSpace::rgbToXYY(TArray<uint8> rgb)
{
    float r = ((float)rgb[0]) / 255.0f;
    float g = ((float)rgb[1]) / 255.0f;
    float b = ((float)rgb[2]) / 255.0f;

    if(r > 0.04045) {
        r = pow((r + 0.055) / 1.055, 2.4);
    } else {
        r = r / 12.92;
    }

    if(g > 0.04045) {
        g = pow((g + 0.055) / 1.055, 2.4);
    } else {
        g = g / 12.92;
    }
    
    if(b > 0.04045) {
        b = pow((b + 0.055) / 1.055, 2.4);
    } else {
        b = b / 12.92;
    }

    float X = r * 0.4124 + g * 0.3576 + b * 0.1805;
    float Y = r * 0.2126 + g * 0.7152 + b * 0.0722;
    float Z = r * 0.0193 + g * 0.1192 + b * 0.9505;

    float x = X / (X + Y + Z);
    float y = Y / (X + Y + Z);
    float z = Z / (X + Y + Z);

    XYYColor xyY;
    xyY.x = x;
    xyY.y = y;
    xyY.Y = Y;
    return xyY;
}


TArray<uint8> UColorBlendSpace::xyYToRGB(XYYColor xyY) {
    float YOvery = xyY.Y / xyY.y;
    float X = xyY.x * YOvery;
    float Y = xyY.y;
    float Z = (1 - xyY.x - xyY.y) * YOvery;
    
    float r = (X * 3.2406) + (Y * -1.5372) + (Z * -0.4986);
    float g = (X * -0.9689) + (Y * 1.8758) + (Z * 0.0415);
    float b = (X * 0.0557) + (Y * -0.2040) + (Z * 1.0570);

    if(r < 0.0031308)
        r = 1.055 * pow(r, 1 / 2.4) - 0.055;
    else
        r = 12.92 * r;
    
    if(g < 0.0031308)
        g = 1.055 * pow(g, 1 / 2.4) - 0.055;
    else
        g = 12.92 * g;

    if(b < 0.0031308)
        b = 1.055 * pow(b, 1 / 2.4) - 0.055;
    else
        b = 12.92 * b;

    TArray<uint8> outRGB;
    outRGB.Add((uint8)(r * 255));
    outRGB.Add((uint8)(g * 255));
    outRGB.Add((uint8)(b * 255));
    return outRGB;
}



float UColorBlendSpace::max(float a, float b) {
    if(a > b) {
        return a;
    }
    return b;
}

float UColorBlendSpace::min(float a, float b) {
    if(a < b) {
        return a;
    }
    return b;
}

/*
Using sRGB => CIE XYZ
D65/2d:
    x = 0.31271
    y = 0.32902
    Y = 100;

[X]    [R]
|Y| = M|G|
[Z]    [B]
Where
    [SrXr SgXg SbXb]
M = |SrYr SgYg SbYb|
    [SrZr SgZg SbZb]
Where
[Sr]   [Xr Xg Xb]-1 [Xw]
|Sg| = |Yr Yg Yb|   |Yw|
[Sb]   [Zr Zg Zb]   [Zw]

X = x * Y/y
Y = y * Y/y or Y
Z = z * Y/y or (1 - x - y)*Y/y

x = X / (X + Y + Z)
y = Y / (X + Y + Z)
z = Z / (X + Y + Z) or 1 - x - y;

=====
sRGB: D65
    r = (0.64, 0.33)
    g = (0.30, 0.60)
    b = (0.15, 0.06)
D65/2d:
    x = 0.31271
    y = 0.32902
    Y = 2.4;

    X = 2.281028509
    Y = 2.4
    Z = 2.61336089

[Sr]   [0.64 0.30 0.15]-1 [0.95047]
|Sg| = |0.30 0.60 0.66|   |1.00000|
[Sb]   [1.00 1.00 1.00]   [1.08883]

matrix inverse:
        [11, 12, 13]
    A = |21, 22, 23|
        [31, 32, 33]
     -1    1  []
    A   = --- ||
          |A| []

    [0.664511 0.154324 0.162028]
M = |0.283881 0.668433 0.047685|
    [0.000088 0.072310 0.986039]

 -1  [ 1.656494 -0.354852 -0.255038]
M  = |-0.707196  1.655399  0.036153|
     [ 0.051714 -0.121365  1.011530]
*/