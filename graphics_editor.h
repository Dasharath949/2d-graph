#ifndef GRAPHICS_EDITOR_H
#define GRAPHICS_EDITOR_H

typedef struct {
    int id;
    char type[20];
    int x1, y1, x2, y2;
    int x, y, width, height;
    int cx, cy, radius;
    int x3, y3;
    char character;
    int filled;
} DrawObject;

typedef struct {
    char canvas[25][80];
    DrawObject objects[100];
    int object_count;
    int next_id;
} GraphicsEditor;

// Initialize the graphics editor
GraphicsEditor* init_editor(void);

// Drawing functions
int draw_line(GraphicsEditor* editor, int x1, int y1, int x2, int y2, char ch);
int draw_rectangle(GraphicsEditor* editor, int x, int y, int width, int height, char ch, int filled);
int draw_circle(GraphicsEditor* editor, int cx, int cy, int radius, char ch, int filled);
int draw_triangle(GraphicsEditor* editor, int x1, int y1, int x2, int y2, int x3, int y3, char ch, int filled);

// Object management
int delete_object(GraphicsEditor* editor, int obj_id);
void clear_canvas(GraphicsEditor* editor);
void list_objects(GraphicsEditor* editor);

// Display function
void display_canvas(GraphicsEditor* editor);

// Cleanup
void free_editor(GraphicsEditor* editor);

#endif
