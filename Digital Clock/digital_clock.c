#include <stdio.h>
#include <time.h>
#include "raylib.h"

#define WIDTH 1220
#define HEIGHT 600

#define OFFSET 50
#define START_X 100
#define DIGIT_DISTANCE 170
#define ON_COLOR RED
#define OFF_COLOR DARKGRAY
#define COLON_RADIUS 25

#define SEGMENT_WIDTH 60
#define SEGMENT_THICKNESS 33

int digits[10][7] = {
    {1, 1, 1, 0, 1, 1, 1}, // Digit 0
    {0, 0, 1, 0, 0, 1, 0}, // Digit 1
    {1, 0, 1, 1, 1, 0, 1}, // Digit 2
    {1, 0, 1, 1, 0, 1, 1}, // Digit 3 
    {0, 1, 1, 1, 0, 1, 0}, // Digit 4
    {1, 1, 0, 1, 0, 1, 1}, // Digit 5
    {1, 1, 0, 1, 1, 1, 1}, // Digit 6
    {1, 0, 1, 0, 0, 1, 0}, // Digit 7
    {1, 1, 1, 1, 1, 1, 1}, // Digit 8
    {1, 1, 1, 1, 0, 1, 1}  // Digit 9
};

void DrawSegment(Vector2 center, bool horizontal, Color color)
{
    int count = 6;
        
    Vector2 a, b, c, d, e, f;
    if (horizontal)
    {
        
        a = (Vector2) {center.x - SEGMENT_WIDTH/2 - SEGMENT_THICKNESS/2, center.y};
        b = (Vector2) {center.x - SEGMENT_WIDTH/2, center.y + SEGMENT_THICKNESS/2};
        c = (Vector2) {center.x - SEGMENT_WIDTH/2, center.y - SEGMENT_THICKNESS/2};
        d = (Vector2) {center.x + SEGMENT_WIDTH/2, center.y + SEGMENT_THICKNESS/2};
        e = (Vector2) {center.x + SEGMENT_WIDTH/2, center.y - SEGMENT_THICKNESS/2};
        f = (Vector2) {center.x + SEGMENT_WIDTH/2 + SEGMENT_THICKNESS/2, center.y};
    }
    else
    {
        a = (Vector2) {center.x, center.y - SEGMENT_WIDTH/2 - SEGMENT_THICKNESS/2};
        b = (Vector2) {center.x - SEGMENT_THICKNESS/2, center.y - SEGMENT_WIDTH/2};
        c = (Vector2) {center.x + SEGMENT_THICKNESS/2, center.y - SEGMENT_WIDTH/2};
        d = (Vector2) {center.x - SEGMENT_THICKNESS/2, center.y + SEGMENT_WIDTH/2};
        e = (Vector2) {center.x + SEGMENT_THICKNESS/2, center.y + SEGMENT_WIDTH/2};
        f = (Vector2) {center.x, center.y + SEGMENT_THICKNESS/2 + SEGMENT_WIDTH/2};
    }
    Vector2 points[] = {a,b,c,d,e,f};
    DrawTriangleStrip(points, count, color); 
}

void DrawDigit(Vector2 center, int digit)
{
    int *digit_segment = digits[digit];

    //Draw first strip
    Vector2 first = {center.x, center.y - SEGMENT_WIDTH - OFFSET};
    DrawSegment(first, true, digit_segment[0] ? ON_COLOR : OFF_COLOR);
    Vector2 second = {center.x - SEGMENT_WIDTH/2 - OFFSET/2, center.y - SEGMENT_WIDTH/2 - OFFSET/2};
    DrawSegment(second, false, digit_segment[1] ? ON_COLOR : OFF_COLOR);
    Vector2 third = {center.x + SEGMENT_WIDTH/2 + OFFSET/2, center.y - SEGMENT_WIDTH/2 - OFFSET/2};
    DrawSegment(third, false, digit_segment[2] ? ON_COLOR : OFF_COLOR);
    Vector2 fourth = {center.x, center.y};
    DrawSegment(fourth, true, digit_segment[3] ? ON_COLOR : OFF_COLOR);
    Vector2 fifth = {center.x - SEGMENT_WIDTH/2 - OFFSET/2, center.y + SEGMENT_WIDTH/2 + OFFSET/2};
    DrawSegment(fifth, false, digit_segment[4] ? ON_COLOR : OFF_COLOR);
    Vector2 sixth = {center.x + SEGMENT_WIDTH/2 + OFFSET/2, center.y + SEGMENT_WIDTH/2 + OFFSET/2};
    DrawSegment(sixth, false, digit_segment[5] ? ON_COLOR : OFF_COLOR);
    Vector2 seventh = {center.x, center.y + SEGMENT_WIDTH + OFFSET};
    DrawSegment(seventh, true, digit_segment[6] ? ON_COLOR : OFF_COLOR);
}

void DrawColon(Vector2 center, int secondes)
{
    DrawCircleV((Vector2){center.x, center.y - 50}, COLON_RADIUS, secondes % 2 ? ON_COLOR : OFF_COLOR);
    DrawCircleV((Vector2){center.x, center.y + 50}, COLON_RADIUS, secondes % 2 ? ON_COLOR : OFF_COLOR);
}

void DrawTime(int hours, int minutes, int secondes)
{
    // HOURS
    DrawDigit((Vector2){START_X, HEIGHT/2}, hours /10);
    DrawDigit((Vector2){START_X + DIGIT_DISTANCE, HEIGHT/2}, hours % 10);
    
    DrawColon((Vector2){START_X + 1.75*DIGIT_DISTANCE, HEIGHT/2}, secondes);

    // MINUTES
    DrawDigit((Vector2){START_X + 2.5*DIGIT_DISTANCE, HEIGHT/2}, minutes / 10);
    DrawDigit((Vector2){START_X + 3.5*DIGIT_DISTANCE, HEIGHT/2}, minutes % 10);

    DrawColon((Vector2){START_X + 4.25*DIGIT_DISTANCE, HEIGHT/2}, secondes);
    
    //SECONDES
    DrawDigit((Vector2){START_X + 5*DIGIT_DISTANCE, HEIGHT/2}, secondes / 10);
    DrawDigit((Vector2){START_X + 6*DIGIT_DISTANCE, HEIGHT/2}, secondes % 10);
}

int main(int argc, char *argv[])
{
    // time_t current_time = time(NULL);
    // char *time_string = ctime(&current_time);

    // printf("The current time is %s", time_string);    
    InitWindow(WIDTH, HEIGHT, "Digital Clock");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        time_t current_time = time(NULL);
        struct tm *current_localtime =localtime(&current_time);
        
        DrawTime(current_localtime->tm_hour, current_localtime->tm_min, current_localtime->tm_sec);
        EndDrawing();
    }

    return 0;
}

        
// DrawRectangle(50, 50, 300, 20, RAYWHITE);
// 
// int count = 4;
// Vector2 a, b, c, d;
// a = (Vector2) {50,50};
// b = (Vector2) {100, 100};
// c = (Vector2) {80, 20};
// d = (Vector2) {200, 30};
// Vector2 points[] = {
//     a,b,c,d
// };
// DrawTriangleStrip(points, count, SKYBLUE);
// Create real 7-segment display 
// int count = 6;
// Vector2 center = {WIDTH/2, HEIGHT/2};
// float segment_width = 200;
// float segment_thickness= 50;
// Vector2 a, b, c, d, e, f;
// a = (Vector2) {center.x - segment_width/2 - segment_thickness/2, center.y};
// b = (Vector2) {center.x - segment_width/2, center.y - segment_thickness/2};
// c = (Vector2) {center.x - segment_width/2, center.y + segment_thickness/2};
// d = (Vector2) {center.x + segment_width/2, center.y - segment_thickness/2};
// e = (Vector2) {center.x + segment_width/2, center.y + segment_thickness/2};
// f = (Vector2) {center.x + segment_width/2 + segment_thickness/2, center.y};
// Vector2 points[] = {a,c,b,e,d,f};
// DrawTriangleStrip(points, count, SKYBLUE);
// Vector2 center = (Vector2){300,200};
// DrawSegment(center, true);
// Vector2 center2 = (Vector2){400, 400};
// DrawSegment(center2, false);