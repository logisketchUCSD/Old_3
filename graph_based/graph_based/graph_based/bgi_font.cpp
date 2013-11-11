/*
 * bgi_font.cc - Parse Borland's font file (.chr)
 *
 * Author:     WeeSan Lee (weesan@cs.ucr.edu)
 * Created on: 7/17/05
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include "bgi_font.h"

#define BGI_FONT_SIGNATURE    '+'

#ifdef __GNUC__
#define PACKED  __attribute ((packed))
#else
#define PACKED
#pragma pack(1)
#endif

// Modified from bgikit.zip
static int decode_stroke(uint16_t *iptr, int *x, int *y ) {
    struct DECODE {
		signed   short int xoff  : 7;
		unsigned short int flag1 : 1;
		signed   short int yoff  : 7;
		unsigned short int flag2 : 1;
    } cword;
    
    cword = *(struct DECODE *)iptr;
    
    *x = cword.xoff;
    *y = cword.yoff;
    
    return ((cword.flag1 << 1) + cword.flag2);
}

class BgiFontFileHeader {
private:
    uint16_t uiHeaderSize PACKED;  // Version 2.0 header format
    uint8_t  pucFontName[4];       // Font internal name
    uint16_t uiFontSize PACKED;    // File size in bytes
    uint8_t  ucFontMajor;          // Driver version
    uint8_t  ucFontMinor;
    uint8_t  ucBgiMajor;           // BGI version
    uint8_t  ucBgiMinor;

public:
    int headerSize(void) const {
		return (uiHeaderSize);
    }
    const char *fontName(void) const {
		return ((char *)pucFontName);
    }
    int fontSize(void) const {
		return (uiFontSize);
    }
    int fontMajor(void) const {
		return (ucFontMajor);
    }
    int fontMinor(void) const {
		return (ucFontMinor);
    }
    int bgiMajor(void) const {
		return (ucBgiMajor);
    }
    int bgiMinor(void) const {
		return (ucBgiMinor);
    }
};

class BgiFontHeader {
private:
    uint8_t  ucSignature;          // Signature
    uint16_t uiNoofChar PACKED;    // No of chars in the font file
    uint8_t  ucUnused1;            // Unused
    uint8_t  ucFirstChar;          // First char in the font file
    uint16_t uiCharDefs PACKED;    // Offset to the char definitions?
    uint8_t  ucScanable;           // Scanable?
    int8_t   zcOrig2Capitol;       // Height from orig. to capitol
    int8_t   zcOrig2Baseline;      // Height from orig. to baseline
    int8_t   zcOrig2Decender;      // Height from orig. to decender
    uint8_t  pucFontName[5];       // Unused

public:
    char signature(void) const {
		return (ucSignature);
    }
    int noofChar(void) const {
		return (uiNoofChar);
    }
    char firstChar(void) const {
		return (ucFirstChar);
    }
    int charDefs(void) const {
		return (uiCharDefs);
    }
    bool scanable(void) const {
		return (ucScanable != 0);
    }
    int orig2Capitol(void) const {
		return (zcOrig2Capitol);
    }
    int orig2Baseline(void) const {
		return (zcOrig2Baseline);
    }
    int orig2Decender(void) const {
		return (zcOrig2Decender);
    }
};

BgiFont::BgiFont(const char *_pzcFile) :
    pzcFile(NULL),
    iFileSize(0),
    iHeight(0) {
    // Initialization
    memset(pcChars, 0, sizeof(pcChars));

    // Copy the font filename
    pzcFile = new char [strlen(_pzcFile) + 1];
    assert(pzcFile != NULL);
    strcpy(pzcFile, _pzcFile);

    int iRet;
    struct stat sStat;

    // Get the file size
    iRet = stat(file(), &sStat);
    assert(iRet == 0);
    iFileSize = sStat.st_size;
    assert(iFileSize != 0);

    // Allocate memory to read the whole file
    char *pzcBuffer = new char [fileSize()];
    assert(pzcBuffer != NULL);

    // Open the file
	FILE *fp = fopen(file(), "rb");
    assert(fp != NULL);

    // Read the whole file into the memory
	iRet = fread(pzcBuffer, 1, fileSize(), fp);
    assert(iRet == fileSize());

    // Close the file
    fclose(fp);
    
    // Look for start of file header
    char *p = pzcBuffer;
    int i = fileSize();
    while (*p && *p != 0x1a && i-- > 0) {
		p++;
    }
    assert(i > 0);
    p++;
    
    // File header
    BgiFontFileHeader *pcFileHeader = (BgiFontFileHeader *)p;
    /*
    printf("Header size = %d\n"
	   "Font name   = %.4s\n"
	   "Font size   = %d\n"
	   "Ver = %d.%d\n"
	   "\n",
	   pcFileHeader->headerSize(),
	   pcFileHeader->fontName(),
	   pcFileHeader->fontSize(),
	   pcFileHeader->fontMajor(),
	   pcFileHeader->fontMinor());
    */

    // Font Header
    BgiFontHeader *pcHeader = (BgiFontHeader *)(pzcBuffer +
						pcFileHeader->headerSize());

    assert(pcHeader->signature() == BGI_FONT_SIGNATURE);
    /*
    printf("Signature     = %c\n"
	   "No of chars   = %d\n"
	   "First char    = %d\n"
	   "Char defs     = %d\n"
	   "Scanable      = %d\n"
	   "Orig2Capitol  = %d\n"
	   "Orig2Baseline = %d\n"
	   "Orig2Decender = %d\n"
	   "\n",
	   pcHeader->signature(),
	   pcHeader->noofChar(),
	   pcHeader->firstChar(),
	   pcHeader->charDefs(),
	   pcHeader->scanable(),
	   pcHeader->orig2Capitol(),
	   pcHeader->orig2Baseline(),
	   pcHeader->orig2Decender());
    */

    // Get the height of the font
    // NOTE: orig2Decender() return negative value
    iHeight = pcHeader->orig2Capitol() - pcHeader->orig2Decender();

    // Get the last character
    int iLastChar = pcHeader->firstChar() + pcHeader->noofChar();

    // Get Offset to individual character definitions
    uint16_t *puiCharsOffset = (uint16_t *)((char *)pcHeader +
					    sizeof(BgiFontHeader));
    /*
    printf("Offset to individual char defs:\n");
    for (int i = pcHeader->firstChar(); i < iLastChar; i++) {
	printf("%02x = 0x%04x\t",
	       i, puiCharsOffset[i - pcHeader->firstChar()]);
	if (i % 4 == 0) {
	    printf("\n");
	}
    }
    printf("\n");
    */

    // Get the character widths
    uint8_t *pucCharsWidth = (uint8_t *)((char *)puiCharsOffset +
					 2 * pcHeader->noofChar());
    for (i = pcHeader->firstChar(); i < iLastChar; i++) {
		pcChars[i].width(pucCharsWidth[i - pcHeader->firstChar()]);
    }
    /*
    printf("Chars width:\n");
    for (int i = pcHeader->firstChar(); i < iLastChar; i++) {
	pcChars[i].width(pucCharsWidth[i - pcHeader->firstChar()]);
	printf("%02x = 0x%04x\t",
	       i, pucCharsWidth[i - pcHeader->firstChar()]);
	if (i % 4 == 0) {
	    printf("\n");
	}
    }
    printf("\n");
    */

    // Get the character definitions
    uint16_t *puiCharDefs = (uint16_t *)(pucCharsWidth + pcHeader->noofChar());
    for (i = pcHeader->firstChar(); i < iLastChar; i++) {
		if (!pcChars[i].width()) {
			continue;
		}
		int iRet;
		uint16_t *puiDef = 
			(uint16_t *)((char *)puiCharDefs +
				puiCharsOffset[i - pcHeader->firstChar()]);
		do {
			int x = 0, y = 0;
			iRet = decode_stroke(puiDef, &x, &y);
			pcChars[i].push_back(BgiFontStroke((BgiFontStroke::OpCode)iRet,
						       x, y));
			puiDef++;
		} while (iRet != 0);
    }
    /*
    for (i = pcHeader->firstChar(); i < iLastChar; i++) {
	if (!pcChars[i].size()) {
	    continue;
	}
	printf("Char %02x, offset %02x:\n",
	       i, puiCharsOffset[i - pcHeader->firstChar()]);
	for (int j = 0; j < pcChars[i].size(); j++) {
	    switch (pcChars[i][j].opCode()) {
	    case 0:
		printf("\tDone\n");
		break;
	    case 1:
		// No supported
		break;
	    case 2:
		printf("\tMove %2d %2d\n",
		       pcChars[i][j].x(), pcChars[i][j].y());
		break;
	    case 3:
		printf("\tDraw %2d %2d\n",
		       pcChars[i][j].x(), pcChars[i][j].y());
		break;
	    default:
		break;
	    }
	}
    }
    */

    // Free the allocated memory
    delete []pzcBuffer;
}

#ifdef DEBUG_BGI_FONT

#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/fl_draw.h>

class FontWindow;
static int draw_char(FontWindow *w, int x, int y,
		     char zcChar, float fScale);
static void draw_string(FontWindow *w, int x, int y,
			const char *pzcString,	float fScale);

class FontWindow : public Fl_Window, public BgiFont {
private:
    float fScale;

protected:
    void draw(void) {
	draw_string(this, 10, 10,
		    "QUICK BROWN FOX JUMPS OVER THE LAZY DOG "
		    "quick brown fox jumps over the lazy dog "
		    "`1234567890-=~!@#$%^&*()_+[]\\;',./{}|:\"<>?",
		    scale());
    }

public:
    FontWindow(const char *pzcFont, float fScale = 1.0) :
		Fl_Window(1000, 400),
		BgiFont(pzcFont),
		fScale(fScale) {
		box(FL_FLAT_BOX);
		color(FL_WHITE);
    }
    float scale(void) const {
		return (fScale);
    }
};

static int draw_char(FontWindow *w, int x, int y, char zcChar,
		     float fScale = 1.0) {
    const BgiFontChar cChar = (*w)[zcChar];
    int iHeight = w->height();

    fl_begin_line();
    for (int i = 0; i < cChar.size(); i++) {
		switch (cChar[i].opCode()) {
		case BgiFontStroke::MOVE:
			fl_end_line();
			fl_begin_line();
			fl_vertex(x + cChar[i].x() * fScale,
				  y + iHeight - cChar[i].y() * fScale);
			break;
		case BgiFontStroke::DRAW:
		    fl_vertex(x + cChar[i].x() * fScale,
			      y + iHeight - cChar[i].y() * fScale);
			break;
		case BgiFontStroke::DONE:
		    break;
		default:
		    break;
		}
    }
    fl_end_line();

    return ((int)(cChar.width() * fScale));
}

static void draw_string(FontWindow *w,
			int x, int y, const char *pzcString,
			float fScale = 1.0) {
    int iOrigX = x;
    for (const char *p = pzcString; p && *p; p++) {
		x += draw_char(w, x, y, *p, fScale);
		if (x + 20 > w->w()) {
		    x = iOrigX;
			y += (int)(w->height() * fScale);
		}
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "Usage: %s font_file\n", argv[0]);
		return (1);
    }

    FontWindow w(argv[1], 1.0);

    w.show(argc, argv);
    return (Fl::run());
}

#endif // DEBUG_BGI_FONT
