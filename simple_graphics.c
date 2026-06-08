#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_graphics.h"

// Initialize the graphics editor
GraphicsEditor* init_editor(void) {
    GraphicsEditor* editor = (GraphicsEditor*)malloc(sizeof(GraphicsEditor));
    
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            editor->canvas[y][x] = ' ';
        }
    }
    
    editor->object_count = 0;
    editor->next_id = 0;
    
    return editor;
}

// Set a pixel on the canvas
void set_pixel(GraphicsEditor* editor, int x, int y, char ch) {
    if (x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT) {
        editor->canvas[y][x] = ch;
    }
}

// Bresenham's line algorithm
void draw_line_internal(GraphicsEditor* editor, int x1, int y1, int x2, int y2, char ch) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    
    int x = x1, y = y1;
    
    while (1) {
        set_pixel(editor, x, y, ch);
        
        if (x == x2 && y == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

// Draw a line
int draw_line(GraphicsEditor* editor, int x1, int y1, int x2, int y2, char ch) {
    if (editor->object_count >= MAX_OBJECTS) {
        printf("Error: Maximum objects reached!\n");
        return -1;
    }
    
    draw_line_internal(editor, x1, y1, x2, y2, ch);
    
    DrawObject obj;
    obj.id = editor->next_id++;
    strncpy(obj.type, "line", sizeof(obj.type) - 1);
    obj.type[sizeof(obj.type) - 1] = '\0';
    obj.x1 = x1;
    obj.y1 = y1;
    obj.x2 = x2;
    obj.y2 = y2;
    obj.character = ch;
    
    editor->objects[editor->object_count++] = obj;
    return obj.id;
}

// Draw a rectangle
int draw_rectangle(GraphicsEditor* editor, int x, int y, int width, int height, char ch, int filled) {
    if (editor->object_count >= MAX_OBJECTS) {
        printf("Error: Maximum objects reached!\n");
        return -1;
    }
    
    if (filled) {
        // Draw filled rectangle
        for (int yy = y; yy < y + height; yy++) {
            for (int xx = x; xx < x + width; xx++) {
                set_pixel(editor, xx, yy, ch);
            }
        }
    } else {
        // Draw outline
        for (int xx = x; xx < x + width; xx++) {
            set_pixel(editor, xx, y, ch);
            set_pixel(editor, xx, y + height - 1, ch);
        }
        for (int yy = y; yy < y + height; yy++) {
            set_pixel(editor, x, yy, ch);
            set_pixel(editor, x + width - 1, yy, ch);
        }
    }
    
    DrawObject obj;
    obj.id = editor->next_id++;
    strncpy(obj.type, "rectangle", sizeof(obj.type) - 1);
    obj.type[sizeof(obj.type) - 1] = '\0';
    obj.x = x;
    obj.y = y;
    obj.width = width;
    obj.height = height;
    obj.character = ch;
    obj.filled = filled;
    
    editor->objects[editor->object_count++] = obj;
    return obj.id;
}

// Delete an object by ID
int delete_object(GraphicsEditor* editor, int obj_id) {
    int found = -1;
    
    for (int i = 0; i < editor->object_count; i++) {
        if (editor->objects[i].id == obj_id) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("Object ID %d not found!\n", obj_id);
        return 0;
    }
    
    for (int i = found; i < editor->object_count - 1; i++) {
        editor->objects[i] = editor->objects[i + 1];
    }
    editor->object_count--;
    
    // Redraw canvas
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            editor->canvas[y][x] = ' ';
        }
    }
    
    // Redraw all remaining objects
    for (int i = 0; i < editor->object_count; i++) {
        DrawObject* obj = &editor->objects[i];
        
        if (strcmp(obj->type, "line") == 0) {
            draw_line_internal(editor, obj->x1, obj->y1, obj->x2, obj->y2, obj->character);
        } else if (strcmp(obj->type, "rectangle") == 0) {
            if (obj->filled) {
                for (int yy = obj->y; yy < obj->y + obj->height; yy++) {
                    for (int xx = obj->x; xx < obj->x + obj->width; xx++) {
                        set_pixel(editor, xx, yy, obj->character);
                    }
                }
            } else {
                for (int xx = obj->x; xx < obj->x + obj->width; xx++) {
                    set_pixel(editor, xx, obj->y, obj->character);
                    set_pixel(editor, xx, obj->y + obj->height - 1, obj->character);
                }
                for (int yy = obj->y; yy < obj->y + obj->height; yy++) {
                    set_pixel(editor, obj->x, yy, obj->character);
                    set_pixel(editor, obj->x + obj->width - 1, yy, obj->character);
                }
            }
        }
    }
    
    return 1;
}

// Clear the canvas
void clear_canvas(GraphicsEditor* editor) {
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            editor->canvas[y][x] = ' ';
        }
    }
    editor->object_count = 0;
    editor->next_id = 0;
}

// Display the canvas
void display_canvas(GraphicsEditor* editor) {
    printf("\n");
    printf("+");
    for (int x = 0; x < CANVAS_WIDTH; x++) printf("-");
    printf("+\n");
    
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            printf("%c", editor->canvas[y][x]);
        }
        printf("|\n");
    }
    
    printf("+");
    for (int x = 0; x < CANVAS_WIDTH; x++) printf("-");
    printf("+\n");
}

// List all objects
void list_objects(GraphicsEditor* editor) {
    printf("\nObjects on canvas:\n");
    
    if (editor->object_count == 0) {
        printf("  (empty)\n");
        return;
    }
    
    for (int i = 0; i < editor->object_count; i++) {
        DrawObject* obj = &editor->objects[i];
        printf("  ID %d: %s", obj->id, obj->type);
        
        if (strcmp(obj->type, "line") == 0) {
            printf(" (%d,%d) to (%d,%d)", obj->x1, obj->y1, obj->x2, obj->y2);
        } else if (strcmp(obj->type, "rectangle") == 0) {
            printf(" at (%d,%d) %dx%d", obj->x, obj->y, obj->width, obj->height);
            if (obj->filled) printf(" [FILLED]");
        }
        printf("\n");
    }
}

// Free the editor
void free_editor(GraphicsEditor* editor) {
    free(editor);
}

// Demo function
void demo(GraphicsEditor* editor) {
    printf("2D Graphics Editor - Line & Rectangle Demo\n");
    printf("==================================================\n\n");
    
    printf("Drawing shapes...\n");
    
    // Draw lines
    draw_line(editor, 5, 2, 35, 15, '*');
    printf("* Line 1 drawn\n");
    
    draw_line(editor, 60, 5, 75, 20, '*');
    printf("* Line 2 drawn\n");
    
    // Draw rectangles
    draw_rectangle(editor, 10, 5, 20, 10, '*', 0);
    printf("* Rectangle 1 (outline) drawn\n");
    
    draw_rectangle(editor, 65, 1, 12, 5, '_', 1);
    printf("* Rectangle 2 (filled) drawn\n");
    
    // Display the canvas
    printf("\nCanvas:\n");
    display_canvas(editor);
    
    // List objects
    list_objects(editor);
    
    // Delete an object
    printf("\nDeleting object ID 2...\n");
    delete_object(editor, 2);
    display_canvas(editor);
    
    // Add another line
    printf("Adding another line...\n");
    draw_line(editor, 40, 1, 40, 20, '*');
    display_canvas(editor);
}

int main(void) {
    GraphicsEditor* editor = init_editor();
    demo(editor);
    free_editor(editor);
    return 0;
}
