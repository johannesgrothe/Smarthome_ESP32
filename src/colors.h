#ifndef __RGB_HSL_Converter__
#define __RGB_HSL_Converter__

static double threeway_max(double a, double b, double c)
{
    return max(a, max(b, c));
}

static double threeway_min(double a, double b, double c)
{
    return min(a, min(b, c));
}

static double hue2rgb(double p, double q, double t)
{
    if(t < 0) t += 1;
    if(t > 1) t -= 1;
    if(t < 1/6.0) return p + (q - p) * 6 * t;
    if(t < 1/2.0) return q;
    if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
    return p;
}

/**
 * Converts an RGB color value to HSL.
 * Assumes r, g, and b are contained in the set [0, 255] and
 * returns h, s, and l in the set [0, 1].
 *
 * @param   Number  r       The red color value
 * @param   Number  g       The green color value
 * @param   Number  b       The blue color value
 * @return  Array           The HSL representation
 */
static void rgbToHsl(uint8_t r, uint8_t g, uint8_t b, float hsl[])
{ 
    double rd = (double) r/255;
    double gd = (double) g/255;
    double bd = (double) b/255;
    double max = threeway_max(rd, gd, bd);
    double min = threeway_min(rd, gd, bd);
    double h = 0, s = 0, l = (max + min) / 2;

    if (max == min) {
        h = s = 0; // achromatic
    } else {
        double d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        if (max == rd) {
            h = (gd - bd) / d + (gd < bd ? 6 : 0);
        } else if (max == gd) {
            h = (bd - rd) / d + 2;
        } else if (max == bd) {
            h = (rd - gd) / d + 4;
        }
        h /= 6;
    }
    hsl[0] = h;
    hsl[1] = s;
    hsl[2] = l;
}

/**
 * Converts an HSL color value to RGB.
 * Assumes h, s, and l are contained in the set [0, 1] and
 * returns r, g, and b in the set [0, 255].
 *
 * @param   Number  h       The hue
 * @param   Number  s       The saturation
 * @param   Number  l       The lightness
 * @return  Array           The RGB representation
 */
static void hslToRgb(double h, double s, double l, uint8_t rgb[])
{
    double r, g, b;

    if (s == 0) {
        r = g = b = l; // achromatic
    } else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3.0);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3.0);
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}


// /**
//  * Converts an RGB color value to HSV.
//  * Assumes r, g, and b are contained in the set [0, 255] and
//  * returns h, s, and v in the set [0, 1].
//  *
//  * @param   Number  r       The red color value
//  * @param   Number  g       The green color value
//  * @param   Number  b       The blue color value
//  * @return  Array           The HSV representation
//  */
// static void rgbToHsv(uint8_t r, uint8_t g, uint8_t b, float hsv[])
// {
//     double rd = (double) r / 255;
//     double gd = (double) g / 255;
//     double bd = (double) b / 255;
//     double max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
//     double h, s, v = max;

//     double d = max - min;
//     s = max == 0 ? 0 : d / max;

//     if (max == min) { 
//         h = 0; // achromatic
//     } else {
//         if (max == rd) {
//             h = (gd - bd) / d + (gd < bd ? 6 : 0);
//         } else if (max == gd) {
//             h = (bd - rd) / d + 2;
//         } else if (max == bd) {
//             h = (rd - gd) / d + 4;
//         }
//         h /= 6;
//     }

//     hsv[0] = h * 360;
//     hsv[1] = s * 100;
//     hsv[2] = v * 100;
// }


// /**
//  * Converts an HSV color value to RGB.
//  * Assumes h, s, and v are contained in the set [0, 1] and
//  * returns r, g, and b in the set [0, 255].
//  *
//  * @param   Number  h       The hue
//  * @param   Number  s       The saturation
//  * @param   Number  v       The value
//  * @return  Array           The RGB representation
//  */
// static void hsvToRgb(double h, double s, double v, uint8_t rgb[])
// {
//     double r, g, b;

//     int i = int(h * 6);
//     double f = h * 6 - i;
//     double p = v * (1 - s);
//     double q = v * (1 - f * s);
//     double t = v * (1 - (1 - f) * s);

//     switch(i % 6){
//         case 0: r = v, g = t, b = p; break;
//         case 1: r = q, g = v, b = p; break;
//         case 2: r = p, g = v, b = t; break;
//         case 3: r = p, g = q, b = v; break;
//         case 4: r = t, g = p, b = v; break;
//         case 5: r = v, g = p, b = q; break;
//     }

//     rgb[0] = r * 255;
//     rgb[1] = g * 255;
//     rgb[2] = b * 255;
// }

#endif