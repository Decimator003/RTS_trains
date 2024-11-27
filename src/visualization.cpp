#include "../include/visualization.h"
#include <iostream>

Visualization::Visualization() : display(nullptr) {
    if(!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return;
    }
    if(!al_init_primitives_addon()) {
        std::cerr << "Failed to initialize primitives addon!" << std::endl;
        return;
    }
    initializeDisplay();

	 // Initialize font
    font = al_create_builtin_font();
    if(!font) {
        std::cerr << "Failed to create font!" << std::endl;
        return;
    }
}

Visualization::~Visualization() {
	 if(font) {
        al_destroy_font(font);
    }

    if(display) {
        al_destroy_display(display);
    }
}

void Visualization::initializeDisplay() {
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if(!display) {
        std::cerr << "Failed to create display!" << std::endl;
        return;
    }
}

void Visualization::drawTrack(int trackNumber, bool occupied, bool lightStatus) {
    int y = TRACK_Y_OFFSET + (trackNumber * TRACK_SPACING);
   
    // Draw main track line
    al_draw_line(0, y, SCREEN_WIDTH, y, al_map_rgb(0, 0, 0), 3);
   
    // Draw traffic light
    ALLEGRO_COLOR lightColor = lightStatus ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 0, 0);
    al_draw_filled_circle(SCREEN_WIDTH - 20, y - 10, 5, lightColor);
}

void Visualization::drawTrain(int trackNumber, float position) {
    int y = TRACK_Y_OFFSET + (trackNumber * TRACK_SPACING);
   
    // Draw train body
    al_draw_filled_rectangle(position, y - 15, position + 50, y - 5,
                           al_map_rgb(100, 100, 100));
   
    // Draw wheels
    al_draw_filled_circle(position + 10, y, 5, al_map_rgb(0, 0, 0));
    al_draw_filled_circle(position + 40, y, 5, al_map_rgb(0, 0, 0));
}

void Visualization::drawSwitch(int switchId, bool position) {
    int x = 50;
    int y1 = TRACK_Y_OFFSET + (switchId * TRACK_SPACING);
    int y2 = y1 + TRACK_SPACING;
   
    ALLEGRO_COLOR switchColor = al_map_rgb(255, 165, 0);
    if(position) {
        al_draw_line(x, y1, x + 30, y2, switchColor, 3);
    } else {
        al_draw_line(x, y2, x + 30, y1, switchColor, 3);
    }
}

void Visualization::drawStation() {
    // Draw station boundary
    al_draw_rectangle(10, 50, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 50,
                     al_map_rgb(0, 0, 0), 2);
}

void Visualization::clearScreen() {
    al_clear_to_color(al_map_rgb(255, 255, 255));
}

void Visualization::updateDisplay() {
    al_flip_display();
}


void Visualization::drawTime(const std::string& time) {
    if(font) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 10, 10, 0, time.c_str());
    }
}
