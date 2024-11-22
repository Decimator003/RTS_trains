// sudo apt install liballegro5-dev
// g++ -Wall -lm engine.cpp -o engine -lallegro -lallegro_primitives



#include "allegro5/allegro.h"
#include <allegro5/allegro_primitives.h>
#include <cmath>

#define PI M_PI

class engine {
public :
    void drawBoundary(int x1,int y1, int x2, int y2, int t) {
        al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(0, 0, 0), t);
    }
    void drawRectangle(int x1,int y1, int x2, int y2) {
        al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(100, 100, 100));
        drawBoundary(x1, y1, x2, y2, 2);
    }
    void drawCircle(int x, int y, int r, int t) {
        al_draw_circle(x, y, r, al_map_rgb(0, 0, 0), t);
    }
    void drawLine(int x1,int y1, int x2, int y2, int t) {
        al_draw_line(x1, y1, x2, y2, al_map_rgb(0, 0, 0), t);
    }
    void rotate(float &x1, float &y1, float x, float y, float x0, float y0, float angle) {
        x1 = (x - x0) * cos(angle) + (y - y0) * sin(angle) + x0;
        y1 = (x0 - x) * sin(angle) + (y - y0) * cos(angle) + y0;
    }
    void drawBody(float x1, float y1, float x2, float y2) {
        drawRectangle(x1, y1, x1 + 50, y2);
        drawRectangle(x1 + 50, y1 + 30, x2, y2);
        drawRectangle(x1 + 100, y1 + 5, x1 + 110, y1 + 30);
        al_draw_filled_circle(x1 + 25, y1 + 25, 10, al_map_rgb(255, 250, 250));
    }
    void drawWheel(int x0,int y0, int r, int t, int i) {
        float x1, x2, y1, y2;
        drawCircle(x0, y0, r, t);

        rotate(x1, y1, x0, y0 - r, x0, y0, (-1 * i * PI/32));
        rotate(x2, y2, x0, y0 + r, x0, y0, (-1 * i * PI/32));
        drawLine(x2, y2, x1, y1, t - 1);
        rotate(x1, y1, x0 + r, y0, x0, y0, (-1 * i * PI/32));
        rotate(x2, y2, x0 - r, y0, x0, y0, (-1 * i * PI/32));
        drawLine(x2, y2, x1, y1, t-1);
    }
    void drawRoad(float y1) {
        drawLine(0, y1, 700, y1, 5);
    }
    void translate(float &x1, float &y1, float l, float m) {
        x1 = x1 + l;
        y1 = y1 + m;
    }
    void shear(float &x0, float &y0, float x2, float y1, float a) {
        x0 = x0 - (y1 - y0) * a;
        y0 = y0;
    }
    void drawSmoke(float x1, float y1, float x2, float y2, float ct) {
        time_t t;
        srand((unsigned) time(&t));
        for(int i = 0; i < ct * 5; i++)
        {
            float x0 = ((float)rand()/(float)(RAND_MAX)) * (x2 - x1);
            float y0 = -1 * ((float)rand()/(float)(RAND_MAX)) * ct;
            translate(x0, y0, x1, y1);
            if(y0 < 50) continue;
            shear(x0, y0, x2, y1, 0.15 * (x2 - x0));
            drawCircle(x0, y0, 1, 0.75);
        }
    }

};

int main() {
    ALLEGRO_DISPLAY *display = NULL;
    
    if(!al_init()) return -1;
    al_init_primitives_addon();
    
    display = al_create_display(750, 500);
    al_clear_to_color(al_map_rgb(255, 255, 255));
    if(!display) return -1;

    al_clear_to_color(al_map_rgb(255, 255, 255));

    engine e;
    float x1 = 0,y1 = 220, x2 = 125, y2 = 300;
    for(float i = 0; i + 120 < 700; i += 2) {
        al_clear_to_color(al_map_rgb(255, 255, 255));

        e.translate(x1, y1, 2, 0);
        e.translate(x2, y2, 2, 0);
        e.drawBody(x1, y1, x2, y2);
        e.drawRoad(y2 + 20);
        e.drawWheel(x1 + 25, y2 + 10, 10, 3, i);
        e.drawWheel(x1 + 75, y2 + 10, 10, 3, i);
        e.drawWheel(x1 + 100, y2 + 10, 10, 3, i);
        e.drawSmoke(x1 + 100, y1 + 5, x1 + 110, y2 - 50, i);

        al_flip_display();
        al_rest(0.025);
    }
    al_destroy_display(display);
    return 0;
}
