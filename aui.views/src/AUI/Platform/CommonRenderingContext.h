#pragma once

#include "IRenderingContext.h"
#include "AWindow.h"

class CommonRenderingContext: public IRenderingContext {
protected:
#if AUI_PLATFORM_WIN

    AString mWindowClass;

    /**
     * GetDC() HDC
     */
    HDC mWindowDC;

    /**
     * BeginPaint() HDC
     */
    HDC mPainterDC;

    PAINTSTRUCT mPaintstruct;
#endif
#if AUI_PLATFORM_LINUX
    void initX11Window(const Init& init, XSetWindowAttributes& swa, XVisualInfo* vi);
#endif
public:
#if AUI_PLATFORM_LINUX
    static Display* ourDisplay;
    static Screen* ourScreen;

    static struct Atoms {
        Atom wmProtocols;
        Atom wmDeleteWindow;
        Atom wmHints;
        Atom wmState;
        Atom netWmState;
        Atom netWmStateMaximizedVert;
        Atom netWmStateMaximizedHorz;
        Atom clipboard;
        Atom utf8String;
        Atom textPlain;
        Atom textPlainUtf8;
        Atom auiClipboard;
        Atom incr;
        Atom targets;
        Atom netWmSyncRequest;
        Atom netWmSyncRequestCounter;
    } ourAtoms;
#endif

    static void ensureXLibInitialized();

    void init(const Init& init) override;

    void beginPaint(AWindow& window) override;

    void endPaint(AWindow& window) override;

    ~CommonRenderingContext() override = default;

    void destroyNativeWindow(AWindow& window) override;
};