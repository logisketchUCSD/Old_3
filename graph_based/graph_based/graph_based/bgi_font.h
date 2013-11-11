/*
 * bgi_font.h - Parse Borland's font file (.chr)
 *
 * Author:     WeeSan Lee (weesan@cs.ucr.edu)
 * Created on: 7/17/05
 */

#ifndef BGI_FONT_H
#define BGI_FONT_H

#ifdef _WIN32
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
#else
#include <stdint.h>
#endif
#include <vector>

using namespace std;

#define BGI_FONT_CHARS_TOTAL  256

class BgiFontStroke {
public:
    enum OpCode {
		DONE = 0,
		SCAN = 1,
		MOVE = 2,
		DRAW = 3,
    };

private:
    enum OpCode eOpCode;
    int8_t iX;
    int8_t iY;

public:
    BgiFontStroke(OpCode eOpCode, int iX, int iY) :
		eOpCode(eOpCode),
		iX(iX),
		iY(iY) {
    }
    OpCode opCode(void) const {
		return (eOpCode);
    }
    int x(void) const {
		return (iX);
    }
    int y(void) const {
		return (iY);
    }
};

class BgiFontChar : public vector<BgiFontStroke> {
private:
    uint16_t iWidth;

public:
    BgiFontChar(void) : iWidth(0) {
    }
    int width(void) const {
		return (iWidth);
    }
    void width(int _iWidth) {
		iWidth = _iWidth;
    }
};

class BgiFont {
private:
    char *pzcFile;
    int iFileSize;
    int iHeight;
    BgiFontChar pcChars[BGI_FONT_CHARS_TOTAL];

public:
    BgiFont(const char *_pzcFile);
    ~BgiFont(void) {
	if (pzcFile) {
	    delete []pzcFile;
	}
    }
    const char *file(void) const {
		return (pzcFile);
    }
    int fileSize(void) const {
		return (iFileSize);
    }
    const BgiFontChar &operator[](int iIndex) const {
		return (pcChars[iIndex]);
    }
    int height(void) const {
		return (iHeight);
    }
};

#endif // BGI_FONT_H
