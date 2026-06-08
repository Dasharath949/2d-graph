#ifndef SIMPLE_GRAPHICS_H
#define SIMPLE_GRAPHICS_H

#define CANVAS_WIDTH 80
#define CANVAS_HEIGHT 25
#define MAX_OBJECTS 100

typedef struct {
    int id;
    char type[20];
    int x1, y1, x2, y2;
    int x, y, width, height;
    char character;
    int filled;
} DrawObject;

typedef struct {
    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    DrawObject objects[MAX_OBJECTS];
    int object_count;
    int next_id;
} GraphicsEditor;

// Function declarations
GraphicsEditor* init_editor(void);
void set_pixel(GraphicsEditor* editor, int x, int y, char ch);
void draw_line_internal(GraphicsEditor* editor, int x1, int y1, int x2, int y2, char ch);
int draw_line(GraphicsEditor* editor, int x1, int y1, int x2, int y2, char ch);
int draw_rectangle(GraphicsEditor* editor, int x, int y, int width, int height, char ch, int filled);
int delete_object(GraphicsEditor* editor, int obj_id);
void clear_canvas(GraphicsEditor* editor);
void display_canvas(GraphicsEditor* editor);
void list_objects(GraphicsEditor* editor);
void free_editor(GraphicsEditor* editor);

#endif
