/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * $Id: parsedcolor.cpp,v 1.1 2000/05/03 08:49:37 ali Exp ali $
 *
 * $Log: parsedcolor.cpp,v $
 * Revision 1.1  2000/05/03 08:49:37  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "parser.h"
#include "parsedcolor.h"
#include "errdesc.h"

static char pcolor_chan_name[3][10] = { "red", "green", "blue" };

/*! A static array with some named colors. Used to provide a nicer
 * interface to modifiers like "color" and "colorize". You can write
 * things like "colorize(DarkBlue)".
 *
 * Array length isn't specified. The last array entry is {-1,-1,-1,""}.
 */
static struct NamedColorEntry
{
  int r,g,b;
  char name[25];
} named_color[] = {
  {255,250,250, "snow"},
  {248,248,255, "GhostWhite"},
  {245,245,245, "WhiteSmoke"},
  {220,220,220, "gainsboro"},
  {255,250,240, "FloralWhite"},
  {253,245,230, "OldLace"},
  {250,240,230, "linen"},
  {250,235,215, "AntiqueWhite"},
  {255,239,213, "PapayaWhip"},
  {255,235,205, "BlanchedAlmond"},
  {255,228,196, "bisque"},
  {255,218,185, "PeachPuff"},
  {255,222,173, "NavajoWhite"},
  {255,228,181, "moccasin"},
  {255,248,220, "cornsilk"},
  {255,255,240, "ivory"},
  {255,250,205, "LemonChiffon"},
  {255,245,238, "seashell"},
  {240,255,240, "honeydew"},
  {245,255,250, "MintCream"},
  {240,255,255, "azure"},
  {240,248,255, "AliceBlue"},
  {230,230,250, "lavender"},
  {255,240,245, "LavenderBlush"},
  {255,228,225, "MistyRose"},
  {255,255,255, "white"},
  {0,0,0, "black"},
  {47,79,79, "DarkSlateGray"},
  {47,79,79, "DarkSlateGrey"},
  {105,105,105, "DimGray"},
  {105,105,105, "DimGrey"},
  {112,128,144, "SlateGray"},
  {112,128,144, "SlateGrey"},
  {119,136,153, "LightSlateGray"},
  {119,136,153, "LightSlateGrey"},
  {190,190,190, "gray"},
  {190,190,190, "grey"},
  {211,211,211, "LightGrey"},
  {211,211,211, "LightGray"},
  {25,25,112, "MidnightBlue"},
  {0,0,128, "navy"},
  {0,0,128, "NavyBlue"},
  {100,149,237, "CornflowerBlue"},
  {72,61,139, "DarkSlateBlue"},
  {106,90,205, "SlateBlue"},
  {123,104,238, "MediumSlateBlue"},
  {132,112,255, "LightSlateBlue"},
  {0,0,205, "MediumBlue"},
  {65,105,225, "RoyalBlue"},
  {0,0,255, "blue"},
  {30,144,255, "DodgerBlue"},
  {0,191,255, "DeepSkyBlue"},
  {135,206,235, "SkyBlue"},
  {135,206,250, "LightSkyBlue"},
  {70,130,180, "SteelBlue"},
  {176,196,222, "LightSteelBlue"},
  {173,216,230, "LightBlue"},
  {176,224,230, "PowderBlue"},
  {175,238,238, "PaleTurquoise"},
  {0,206,209, "DarkTurquoise"},
  {72,209,204, "MediumTurquoise"},
  {64,224,208, "turquoise"},
  {0,255,255, "cyan"},
  {224,255,255, "LightCyan"},
  {95,158,160, "CadetBlue"},
  {102,205,170, "MediumAquamarine"},
  {127,255,212, "aquamarine"},
  {0,100,0, "DarkGreen"},
  {85,107,47, "DarkOliveGreen"},
  {143,188,143, "DarkSeaGreen"},
  {46,139,87, "SeaGreen"},
  {60,179,113, "MediumSeaGreen"},
  {32,178,170, "LightSeaGreen"},
  {152,251,152, "PaleGreen"},
  {0,255,127, "SpringGreen"},
  {124,252,0, "LawnGreen"},
  {0,255,0, "green"},
  {127,255,0, "chartreuse"},
  {0,250,154, "MediumSpringGreen"},
  {173,255,47, "GreenYellow"},
  {50,205,50, "LimeGreen"},
  {154,205,50, "YellowGreen"},
  {34,139,34, "ForestGreen"},
  {107,142,35, "OliveDrab"},
  {189,183,107, "DarkKhaki"},
  {240,230,140, "khaki"},
  {238,232,170, "PaleGoldenrod"},
  {250,250,210, "LightGoldenrodYellow"},
  {255,255,224, "LightYellow"},
  {255,255,0, "yellow"},
  {255,215,0, "gold"},
  {238,221,130, "LightGoldenrod"},
  {218,165,32, "goldenrod"},
  {184,134,11, "DarkGoldenrod"},
  {188,143,143, "RosyBrown"},
  {205,92,92, "IndianRed"},
  {139,69,19, "SaddleBrown"},
  {160,82,45, "sienna"},
  {205,133,63, "peru"},
  {222,184,135, "burlywood"},
  {245,245,220, "beige"},
  {245,222,179, "wheat"},
  {244,164,96, "SandyBrown"},
  {210,180,140, "tan"},
  {210,105,30, "chocolate"},
  {178,34,34, "firebrick"},
  {165,42,42, "brown"},
  {233,150,122, "DarkSalmon"},
  {250,128,114, "salmon"},
  {255,160,122, "LightSalmon"},
  {255,165,0, "orange"},
  {255,140,0, "DarkOrange"},
  {255,127,80, "coral"},
  {240,128,128, "LightCoral"},
  {255,99,71, "tomato"},
  {255,69,0, "OrangeRed"},
  {255,0,0, "red"},
  {255,105,180, "HotPink"},
  {255,20,147, "DeepPink"},
  {255,192,203, "pink"},
  {255,182,193, "LightPink"},
  {219,112,147, "PaleVioletRed"},
  {176,48,96, "maroon"},
  {199,21,133, "MediumVioletRed"},
  {208,32,144, "VioletRed"},
  {255,0,255, "magenta"},
  {238,130,238, "violet"},
  {221,160,221, "plum"},
  {218,112,214, "orchid"},
  {186,85,211, "MediumOrchid"},
  {153,50,204, "DarkOrchid"},
  {148,0,211, "DarkViolet"},
  {138,43,226, "BlueViolet"},
  {160,32,240, "purple"},
  {147,112,219, "MediumPurple"},
  {216,191,216, "thistle"},
  {255,250,250, "snow1"},
  {238,233,233, "snow2"},
  {205,201,201, "snow3"},
  {139,137,137, "snow4"},
  {255,245,238, "seashell1"},
  {238,229,222, "seashell2"},
  {205,197,191, "seashell3"},
  {139,134,130, "seashell4"},
  {255,239,219, "AntiqueWhite1"},
  {238,223,204, "AntiqueWhite2"},
  {205,192,176, "AntiqueWhite3"},
  {139,131,120, "AntiqueWhite4"},
  {255,228,196, "bisque1"},
  {238,213,183, "bisque2"},
  {205,183,158, "bisque3"},
  {139,125,107, "bisque4"},
  {255,218,185, "PeachPuff1"},
  {238,203,173, "PeachPuff2"},
  {205,175,149, "PeachPuff3"},
  {139,119,101, "PeachPuff4"},
  {255,222,173, "NavajoWhite1"},
  {238,207,161, "NavajoWhite2"},
  {205,179,139, "NavajoWhite3"},
  {139,121,94, "NavajoWhite4"},
  {255,250,205, "LemonChiffon1"},
  {238,233,191, "LemonChiffon2"},
  {205,201,165, "LemonChiffon3"},
  {139,137,112, "LemonChiffon4"},
  {255,248,220, "cornsilk1"},
  {238,232,205, "cornsilk2"},
  {205,200,177, "cornsilk3"},
  {139,136,120, "cornsilk4"},
  {255,255,240, "ivory1"},
  {238,238,224, "ivory2"},
  {205,205,193, "ivory3"},
  {139,139,131, "ivory4"},
  {240,255,240, "honeydew1"},
  {224,238,224, "honeydew2"},
  {193,205,193, "honeydew3"},
  {131,139,131, "honeydew4"},
  {255,240,245, "LavenderBlush1"},
  {238,224,229, "LavenderBlush2"},
  {205,193,197, "LavenderBlush3"},
  {139,131,134, "LavenderBlush4"},
  {255,228,225, "MistyRose1"},
  {238,213,210, "MistyRose2"},
  {205,183,181, "MistyRose3"},
  {139,125,123, "MistyRose4"},
  {240,255,255, "azure1"},
  {224,238,238, "azure2"},
  {193,205,205, "azure3"},
  {131,139,139, "azure4"},
  {131,111,255, "SlateBlue1"},
  {122,103,238, "SlateBlue2"},
  {105,89,205, "SlateBlue3"},
  {71,60,139, "SlateBlue4"},
  {72,118,255, "RoyalBlue1"},
  {67,110,238, "RoyalBlue2"},
  {58,95,205, "RoyalBlue3"},
  {39,64,139, "RoyalBlue4"},
  {0,0,255, "blue1"},
  {0,0,238, "blue2"},
  {0,0,205, "blue3"},
  {0,0,139, "blue4"},
  {30,144,255, "DodgerBlue1"},
  {28,134,238, "DodgerBlue2"},
  {24,116,205, "DodgerBlue3"},
  {16,78,139, "DodgerBlue4"},
  {99,184,255, "SteelBlue1"},
  {92,172,238, "SteelBlue2"},
  {79,148,205, "SteelBlue3"},
  {54,100,139, "SteelBlue4"},
  {0,191,255, "DeepSkyBlue1"},
  {0,178,238, "DeepSkyBlue2"},
  {0,154,205, "DeepSkyBlue3"},
  {0,104,139, "DeepSkyBlue4"},
  {135,206,255, "SkyBlue1"},
  {126,192,238, "SkyBlue2"},
  {108,166,205, "SkyBlue3"},
  {74,112,139, "SkyBlue4"},
  {176,226,255, "LightSkyBlue1"},
  {164,211,238, "LightSkyBlue2"},
  {141,182,205, "LightSkyBlue3"},
  {96,123,139, "LightSkyBlue4"},
  {198,226,255, "SlateGray1"},
  {185,211,238, "SlateGray2"},
  {159,182,205, "SlateGray3"},
  {108,123,139, "SlateGray4"},
  {202,225,255, "LightSteelBlue1"},
  {188,210,238, "LightSteelBlue2"},
  {162,181,205, "LightSteelBlue3"},
  {110,123,139, "LightSteelBlue4"},
  {191,239,255, "LightBlue1"},
  {178,223,238, "LightBlue2"},
  {154,192,205, "LightBlue3"},
  {104,131,139, "LightBlue4"},
  {224,255,255, "LightCyan1"},
  {209,238,238, "LightCyan2"},
  {180,205,205, "LightCyan3"},
  {122,139,139, "LightCyan4"},
  {187,255,255, "PaleTurquoise1"},
  {174,238,238, "PaleTurquoise2"},
  {150,205,205, "PaleTurquoise3"},
  {102,139,139, "PaleTurquoise4"},
  {152,245,255, "CadetBlue1"},
  {142,229,238, "CadetBlue2"},
  {122,197,205, "CadetBlue3"},
  {83,134,139, "CadetBlue4"},
  {0,245,255, "turquoise1"},
  {0,229,238, "turquoise2"},
  {0,197,205, "turquoise3"},
  {0,134,139, "turquoise4"},
  {0,255,255, "cyan1"},
  {0,238,238, "cyan2"},
  {0,205,205, "cyan3"},
  {0,139,139, "cyan4"},
  {151,255,255, "DarkSlateGray1"},
  {141,238,238, "DarkSlateGray2"},
  {121,205,205, "DarkSlateGray3"},
  {82,139,139, "DarkSlateGray4"},
  {127,255,212, "aquamarine1"},
  {118,238,198, "aquamarine2"},
  {102,205,170, "aquamarine3"},
  {69,139,116, "aquamarine4"},
  {193,255,193, "DarkSeaGreen1"},
  {180,238,180, "DarkSeaGreen2"},
  {155,205,155, "DarkSeaGreen3"},
  {105,139,105, "DarkSeaGreen4"},
  {84,255,159, "SeaGreen1"},
  {78,238,148, "SeaGreen2"},
  {67,205,128, "SeaGreen3"},
  {46,139,87, "SeaGreen4"},
  {154,255,154, "PaleGreen1"},
  {144,238,144, "PaleGreen2"},
  {124,205,124, "PaleGreen3"},
  {84,139,84, "PaleGreen4"},
  {0,255,127, "SpringGreen1"},
  {0,238,118, "SpringGreen2"},
  {0,205,102, "SpringGreen3"},
  {0,139,69, "SpringGreen4"},
  {0,255,0, "green1"},
  {0,238,0, "green2"},
  {0,205,0, "green3"},
  {0,139,0, "green4"},
  {127,255,0, "chartreuse1"},
  {118,238,0, "chartreuse2"},
  {102,205,0, "chartreuse3"},
  {69,139,0, "chartreuse4"},
  {192,255,62, "OliveDrab1"},
  {179,238,58, "OliveDrab2"},
  {154,205,50, "OliveDrab3"},
  {105,139,34, "OliveDrab4"},
  {202,255,112, "DarkOliveGreen1"},
  {188,238,104, "DarkOliveGreen2"},
  {162,205,90, "DarkOliveGreen3"},
  {110,139,61, "DarkOliveGreen4"},
  {255,246,143, "khaki1"},
  {238,230,133, "khaki2"},
  {205,198,115, "khaki3"},
  {139,134,78, "khaki4"},
  {255,236,139, "LightGoldenrod1"},
  {238,220,130, "LightGoldenrod2"},
  {205,190,112, "LightGoldenrod3"},
  {139,129,76, "LightGoldenrod4"},
  {255,255,224, "LightYellow1"},
  {238,238,209, "LightYellow2"},
  {205,205,180, "LightYellow3"},
  {139,139,122, "LightYellow4"},
  {255,255,0, "yellow1"},
  {238,238,0, "yellow2"},
  {205,205,0, "yellow3"},
  {139,139,0, "yellow4"},
  {255,215,0, "gold1"},
  {238,201,0, "gold2"},
  {205,173,0, "gold3"},
  {139,117,0, "gold4"},
  {255,193,37, "goldenrod1"},
  {238,180,34, "goldenrod2"},
  {205,155,29, "goldenrod3"},
  {139,105,20, "goldenrod4"},
  {255,185,15, "DarkGoldenrod1"},
  {238,173,14, "DarkGoldenrod2"},
  {205,149,12, "DarkGoldenrod3"},
  {139,101,8, "DarkGoldenrod4"},
  {255,193,193, "RosyBrown1"},
  {238,180,180, "RosyBrown2"},
  {205,155,155, "RosyBrown3"},
  {139,105,105, "RosyBrown4"},
  {255,106,106, "IndianRed1"},
  {238,99,99, "IndianRed2"},
  {205,85,85, "IndianRed3"},
  {139,58,58, "IndianRed4"},
  {255,130,71, "sienna1"},
  {238,121,66, "sienna2"},
  {205,104,57, "sienna3"},
  {139,71,38, "sienna4"},
  {255,211,155, "burlywood1"},
  {238,197,145, "burlywood2"},
  {205,170,125, "burlywood3"},
  {139,115,85, "burlywood4"},
  {255,231,186, "wheat1"},
  {238,216,174, "wheat2"},
  {205,186,150, "wheat3"},
  {139,126,102, "wheat4"},
  {255,165,79, "tan1"},
  {238,154,73, "tan2"},
  {205,133,63, "tan3"},
  {139,90,43, "tan4"},
  {255,127,36, "chocolate1"},
  {238,118,33, "chocolate2"},
  {205,102,29, "chocolate3"},
  {139,69,19, "chocolate4"},
  {255,48,48, "firebrick1"},
  {238,44,44, "firebrick2"},
  {205,38,38, "firebrick3"},
  {139,26,26, "firebrick4"},
  {255,64,64, "brown1"},
  {238,59,59, "brown2"},
  {205,51,51, "brown3"},
  {139,35,35, "brown4"},
  {255,140,105, "salmon1"},
  {238,130,98, "salmon2"},
  {205,112,84, "salmon3"},
  {139,76,57, "salmon4"},
  {255,160,122, "LightSalmon1"},
  {238,149,114, "LightSalmon2"},
  {205,129,98, "LightSalmon3"},
  {139,87,66, "LightSalmon4"},
  {255,165,0, "orange1"},
  {238,154,0, "orange2"},
  {205,133,0, "orange3"},
  {139,90,0, "orange4"},
  {255,127,0, "DarkOrange1"},
  {238,118,0, "DarkOrange2"},
  {205,102,0, "DarkOrange3"},
  {139,69,0, "DarkOrange4"},
  {255,114,86, "coral1"},
  {238,106,80, "coral2"},
  {205,91,69, "coral3"},
  {139,62,47, "coral4"},
  {255,99,71, "tomato1"},
  {238,92,66, "tomato2"},
  {205,79,57, "tomato3"},
  {139,54,38, "tomato4"},
  {255,69,0, "OrangeRed1"},
  {238,64,0, "OrangeRed2"},
  {205,55,0, "OrangeRed3"},
  {139,37,0, "OrangeRed4"},
  {255,0,0, "red1"},
  {238,0,0, "red2"},
  {205,0,0, "red3"},
  {139,0,0, "red4"},
  {255,20,147, "DeepPink1"},
  {238,18,137, "DeepPink2"},
  {205,16,118, "DeepPink3"},
  {139,10,80, "DeepPink4"},
  {255,110,180, "HotPink1"},
  {238,106,167, "HotPink2"},
  {205,96,144, "HotPink3"},
  {139,58,98, "HotPink4"},
  {255,181,197, "pink1"},
  {238,169,184, "pink2"},
  {205,145,158, "pink3"},
  {139,99,108, "pink4"},
  {255,174,185, "LightPink1"},
  {238,162,173, "LightPink2"},
  {205,140,149, "LightPink3"},
  {139,95,101, "LightPink4"},
  {255,130,171, "PaleVioletRed1"},
  {238,121,159, "PaleVioletRed2"},
  {205,104,137, "PaleVioletRed3"},
  {139,71,93, "PaleVioletRed4"},
  {255,52,179, "maroon1"},
  {238,48,167, "maroon2"},
  {205,41,144, "maroon3"},
  {139,28,98, "maroon4"},
  {255,62,150, "VioletRed1"},
  {238,58,140, "VioletRed2"},
  {205,50,120, "VioletRed3"},
  {139,34,82, "VioletRed4"},
  {255,0,255, "magenta1"},
  {238,0,238, "magenta2"},
  {205,0,205, "magenta3"},
  {139,0,139, "magenta4"},
  {255,131,250, "orchid1"},
  {238,122,233, "orchid2"},
  {205,105,201, "orchid3"},
  {139,71,137, "orchid4"},
  {255,187,255, "plum1"},
  {238,174,238, "plum2"},
  {205,150,205, "plum3"},
  {139,102,139, "plum4"},
  {224,102,255, "MediumOrchid1"},
  {209,95,238, "MediumOrchid2"},
  {180,82,205, "MediumOrchid3"},
  {122,55,139, "MediumOrchid4"},
  {191,62,255, "DarkOrchid1"},
  {178,58,238, "DarkOrchid2"},
  {154,50,205, "DarkOrchid3"},
  {104,34,139, "DarkOrchid4"},
  {155,48,255, "purple1"},
  {145,44,238, "purple2"},
  {125,38,205, "purple3"},
  {85,26,139, "purple4"},
  {171,130,255, "MediumPurple1"},
  {159,121,238, "MediumPurple2"},
  {137,104,205, "MediumPurple3"},
  {93,71,139, "MediumPurple4"},
  {255,225,255, "thistle1"},
  {238,210,238, "thistle2"},
  {205,181,205, "thistle3"},
  {139,123,139, "thistle4"},
  {0,0,0, "gray0"},
  {0,0,0, "grey0"},
  {26,26,26, "gray10"},
  {26,26,26, "grey10"},
  {51,51,51, "gray20"},
  {51,51,51, "grey20"},
  {77,77,77, "gray30"},
  {77,77,77, "grey30"},
  {102,102,102, "gray40"},
  {102,102,102, "grey40"},
  {127,127,127, "gray50"},
  {127,127,127, "grey50"},
  {153,153,153, "gray60"},
  {153,153,153, "grey60"},
  {179,179,179, "gray70"},
  {179,179,179, "grey70"},
  {204,204,204, "gray80"},
  {204,204,204, "grey80"},
  {229,229,229, "gray90"},
  {229,229,229, "grey90"},
  {255,255,255, "gray100"},
  {255,255,255, "grey100"},
  {169,169,169, "DarkGrey"},
  {169,169,169, "DarkGray"},
  {0,0,139, "DarkBlue"},
  {0,139,139, "DarkCyan"},
  {139,0,139, "DarkMagenta"},
  {139,0,0, "DarkRed"},
  {144,238,144, "LightGreen"},
  {-1,-1,-1,""}
};

void ParsedColor::print(FILE *f)
{
  fprintf(f,"rgb<%d,%d,%d>",val[0],val[1],val[2]);
}

ParsedColor::ParsedColor()
{
  val[0]=val[1]=val[2]=0;
}


bool ParsedColor::parseAsName(Parser &p)
{
  int cur=0;

  while(named_color[cur].r >= 0)
    {
      if(strcasecmp(p.tok_text, named_color[cur].name) == 0)
	{
	  val[0] = named_color[cur].r;
	  val[1] = named_color[cur].g;
	  val[2] = named_color[cur].b;
	  p.gentoken();
	  return true;
	};
      cur++;
    };
  return false;
}

bool ParsedColor::parse(Parser &p, int lowest, int highest, bool allowBrackets)
{
  bool haveopen;
  int chan;

  if(allowBrackets)
    haveopen=p.eatOpen(false);
  else
    haveopen=false;

  for(chan=0;chan<3;chan++)
    if(!p.eatSNumber(val[chan]))
      {
	if(chan==0)
	  if(parseAsName(p))
	    break;
	p.error(ERROR_EXPECTED_NUMBER,"Expected number for %s part of color definition, not $$",
		pcolor_chan_name[chan]);
	return false;
      }
    else
      {
	if(val[chan] < lowest)
	  {
	    p.error(ERROR_NUMBER_RANGE,"Value for '%s' is too low - must be at least %d",
		    pcolor_chan_name[chan], lowest);
	    return false;
	  };

	if(val[chan] > highest)
	  {
	    p.error(ERROR_NUMBER_RANGE,"Value for '%s' is too high - must be not more than %d",
		    pcolor_chan_name[chan], highest);
	    return false;
	  };
      };

  if(haveopen)
    if(!p.eatClose(true))
      return false;
  return true;
}


