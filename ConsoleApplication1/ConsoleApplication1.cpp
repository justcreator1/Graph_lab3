

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#define PI 3.14159265358979323846 

using namespace std;

struct RGB {
    unsigned char r, g, b;
};
struct Point {
    int x, y;
};

double Sign(double x) 
{
    if (x > 0) {
        return 1.0;
    }
    if (x == 0) {
        return 0;
    }
    return -1.0;
}

class Canvas1 {
public:
    int width, height;
    std::vector<RGB> pixels;
    Canvas1(const int& w, const int& h)
    {
        width = w;
        height = h;
        pixels.assign(width * height, { 255,255,255 });
    }
    void Replace_Pixel(int x, int y, RGB& color)
    {
        pixels[y * width + x] = color;
    }
    
    bool Save_Canvas(const string& filepath)
    {
        ofstream stream(filepath, ios::binary);
        if (!stream) {
            cout << "не удалось создать файл" << endl;
            return false;
        }
        stream << "P6\n" << width << " " << height << "\n255\n";
        stream.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(RGB));
        return true;
    }

    void CDA(Point start, Point end,RGB color) 
    {
        double L;
        if (abs(start.x - end.x) >= abs(start.y - end.y)) {
            L = abs(start.x - end.x);
        }
        else {
            L = abs(start.y - end.y);
        }
        double dx = (double)(end.x - start.x) / L;
        double dy = (double)(end.y - start.y) / L;
        double x = start.x + 0.5 * Sign(dx);
        double y = start.y + 0.5 * Sign(dy);
        for (int i = 1; i < L + 1; i++) {
            Replace_Pixel(floor(x),floor(y), color);
            x += dx;
            y += dy;
        }
    }
    void Brezenhem(Point start, Point end, RGB color) 
    {
        double dx = (double)(end.x - start.x);
        double dy = (double)(end.y - start.y);
        double sx = Sign(dx);
        double sy = Sign(dy);
        dx = abs(dx);
        dy = abs(dy);
        int flag = 0;
        if (dx < dy) {
            double t = dx;
            dx = dy;
            dy = t;
            flag = 1;
        }
        double f = dy / dx - 0.5;
        double x = start.x;
        double y = start.y;
        for(int i =0;i<dx;i++)
        {
            Replace_Pixel(floor(x), floor(y), color);
            if (f >= 0) {
                if (flag == 1) {
                    x = x + sx;
                }
                else {
                    y = y + sy;
                }
                f -= 1;
            }
            if (flag == 1) {
                y = y + sy;
            }
            else {
                x = x + sx;
            }
            f = f + dy / dx;
        };

        
    }
    void BrezenhemC(Point start, Point end, RGB color)
    {
        int dx = (end.x - start.x);
        int dy = (end.y - start.y);
        int sx = Sign(dx);
        int sy = Sign(dy);
        dx = abs(dx);
        dy = abs(dy);
        int flag = 0;
        if (dx < dy) {
            int t = dx;
            dx = dy;
            dy = t;
            flag = 1;
        }
        int f = 2*dy - dx;
        int x = start.x;
        int y = start.y;
        for (int i = 0; i < dx; i++)
        {
            Replace_Pixel(floor(x), floor(y), color);
            if (f >= 0) {
                if (flag == 1) {
                    x = x + sx;
                }
                else {
                    y = y + sy;
                }
                f -= 2 * dx;
            }
            if (flag == 1) {
                y = y + sy;
            }
            else {
                x = x + sx;
            }
            f = f + 2*dy;
        };
    }
};

vector<Point> PV(double radius, double startAngle, Point center) {
    vector<Point> vertices(5);

    for (int i = 0; i < 5; i++) {
        double angle = startAngle + i * 2 * PI / 5;
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        vertices[i] = { int(x), int(y)};
    }

    return vertices;
}
int main()
{
    double r;
    cin >> r;
    double c;
    cin >> c;
    vector<Point> vertices = PV(r, c, { int(r) + 1,int(r) + 1 });
    Canvas1 cda(2 * int(r) + 2, 2 * int(r) + 2);
    Canvas1 br(2 * int(r) + 2, 2 * int(r) + 2);
    Canvas1 brc(2 * int(r) + 2, 2 * int(r) + 2);
    for (int i = 1; i < 6; i++)
    {
        cda.CDA(vertices[(i - 1)*2%5], vertices[i*2%5], { 255,0,0 });
        br.Brezenhem(vertices[(i - 1) * 2 % 5], vertices[i * 2 % 5], { 255,0,0 });
        brc.BrezenhemC(vertices[(i - 1) * 2 % 5], vertices[i * 2 % 5], { 255,0,0 });
    }
    cda.Save_Canvas("cda.ppm");
    br.Save_Canvas("br.ppm");
    brc.Save_Canvas("brc.ppm");

}
