#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <GL/gl.h>  // For OpenGL functions and constants
#include <GL/glu.h> // For OpenGL utility library functions
#include <tchar.h>
#include <windows.h>


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");



int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void swap(int &x1, int &y1,int &x2, int &y2)
{
    x1 ^=x2;
    x2 ^=x1;
    x1 ^=x2;

    y1 ^=y2;
    y2 ^=y1;
    y1 ^=y2;
}

int round (double x)
{
    return (int)(x+0.5);
}

void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float incX = dx / (float)steps;
    float incY = dy / (float)steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; ++i)
    {
        SetPixel(hdc, round(x), round(y), color);
        x += incX;
        y += incY;
    }
}

void FloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf) {
    COLORREF C = GetPixel(hdc, x, y);
    if (C == Cb || C == Cf) {
        return;
    }
    SetPixel(hdc, x, y, Cf);  // Set the pixel to the fill color
    FloodFill(hdc, x + 1, y, Cb, Cf);  // Fill right
    FloodFill(hdc, x - 1, y, Cb, Cf);  // Fill left
    FloodFill(hdc, x, y + 1, Cb, Cf);  // Fill down
    FloodFill(hdc, x, y - 1, Cb, Cf);  // Fill up
}

// Store the points of the polygon
POINT pt[5];
int ptCount = 0;

void DrawPolygon(HDC hdc, COLORREF color) {
    // Draw lines between points to form the polygon
    for (int i = 0; i < ptCount - 1; i++) {
        drawLine(hdc, pt[i].x, pt[i].y, pt[i+1].x, pt[i+1].y, color);
    }
    // Close the polygon
    if (ptCount == 5) {
        drawLine(hdc, pt[4].x, pt[4].y, pt[0].x, pt[0].y, color);
    }
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static bool polygonDrawn = false;  // Flag indicating whether the polygon has been drawn
    switch (message)                  /* handle the messages */
    {
        case WM_LBUTTONDOWN:
            if (ptCount < 5) {
                pt[ptCount].x = LOWORD(lParam);
                pt[ptCount].y = HIWORD(lParam);
                ptCount++;
                hdc = GetDC(hwnd); // Get the device context
                DrawPolygon(hdc, RGB(255, 0, 0));  // Draw the polygon in red
                ReleaseDC(hwnd, hdc);
            } else {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                hdc = GetDC(hwnd); // Get the device context
                FloodFill(hdc, x, y, RGB(255, 0, 0), RGB(255, 255, 0));  // Fill with yellow color
                ReleaseDC(hwnd, hdc);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

