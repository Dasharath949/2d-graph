#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CANVAS_WIDTH 80
#define CANVAS_HEIGHT 25
#define MAX_OBJECTS 100

typedef struct {
    int id;
    char type[20];          // "line", "rectangle", "circle", "triangle"
    int x1, y1, x2, y2;     // for line and triangle
    int x, y, width, height; // for rectangle
    int cx, cy, radius;     // for circle
    int x3, y3;             // for triangle (third point)
    char character;
    int filled;
} DrawObject;

typedef struct {
    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    DrawObject objects[MAX_OBJECTS];
    int object_count;
    int next_id;
} GraphicsEditor;

// Initialize the graphics editor
GraphicsEditor* init_editor(void) {
    GraphicsEditor* editor = (GraphicsEditor*)malloc(sizeof(GraphicsEditor));
    
    // Initialize canvas with spaces
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

// Get a pixel from the canvas
char get_pixel(GraphicsEditor* editor, int x, int y) {
    if (x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT) {
        return editor->canvas[y][x];
    }
    return ' ';
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
    strcpy(obj.type, "line");
    obj.x1 = x1;
    obj.y1 = y1;
    obj.x2 = x2;
    obj.y2 = y2;
    obj.character = ch;
    
    editor->objects[editor->object_count++] = obj;
    return obj.id;
}

// Bresenham's circle algorithm
void draw_circle_internal(GraphicsEditor* editor, int cx, int cy, int radius, char ch, int filled) {
    if (filled) {
        // Draw filled circle
        for (int y = cy - radius; y <= cy + radius; y++) {
            for (int x = cx - radius; x <= cx + radius; x++) {
                int dist_sq = (x - cx) * (x - cx) + (y - cy) * (y - cy);
                if (dist_sq <= radius * radius) {
                    set_pixel(editor, x, y, ch);
                }
            }
        }
    } else {
        // Draw circle outline using Bresenham's algorithm
        int x = radius, y = 0;
        int d = 3 - 2 * radius;
        
        while (x >= y) {
            // Draw 8 octants
            set_pixel(editor, cx + x, cy + y, ch);
            set_pixel(editor, cx - x, cy + y, ch);
            set_pixel(editor, cx + x, cy - y, ch);
            set_pixel(editor, cx - x, cy - y, ch);
            set_pixel(editor, cx + y, cy + x, ch);
            set_pixel(editor, cx - y, cy + x, ch);
            set_pixel(editor, cx + y, cy - x, ch);
            set_pixel(editor, cx - y, cy - x, ch);
            
            if (d < 0) {
                d = d + 4 * y + 6;
            } else {
                d = d + 4 * (y - x) + 10;
                x--;
            }
            y++;
        }
    }
}

// Draw a circle
int draw_circle(GraphicsEditor* editor, int cx, int cy, int radius, char ch, int filled) {
    if (editor->object_count >= MAX_OBJECTS) {
        printf("Error: Maximum objects reached!\n");
        return -1;
    }
    
    draw_circle_internal(editor, cx, cy, radius, ch, filled);
    
    DrawObject obj;
    obj.id = editor->next_id++;
    strcpy(obj.type, "circle");
    obj.cx = cx;
    obj.cy = cy;
    obj.radius = radius;
    obj.character = ch;
    obj.filled = filled;
    
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
        // Top and bottom edges
        for (int xx = x; xx < x + width; xx++) {
            set_pixel(editor, xx, y, ch);
            set_pixel(editor, xx, y + height - 1, ch);
        }
        // Left and right edges
        for (int yy = y; yy < y + height; yy++) {
            set_pixel(editor, x, yy, ch);
            set_pixel(editor, x + width - 1, yy, ch);
        }
    }
    
    DrawObject obj;
    obj.id = editor->next_id++;
    strcpy(obj.type, "rectangle");
    obj.x = x;
    obj.y = y;
    obj.width = width;
    obj.height = height;
    obj.character = ch;
    obj.filled = filled;
    
    editor->objects[editor->object_count++] = obj;
    return obj.id;
}

// Check if a point is inside a triangle using barycentric coordinates
int point_in_triangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3) {
    int d1 = (px - x2) * (y1 - y2) - (x1 - x2) * (py - y2);
    int d2 = (px - x3) * (y2 - y3) - (x2 - x3) * (py - y3);
    int d3 = (px - x1) * (y3 - y1) - (x3 - x1) * (py - y1);
    
    int has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    int has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    
    return !(has_neg && has_pos);
}

// Draw a filled triangle
void draw_filled_triangle(GraphicsEditor* editor, int x1, int y1, int x2, int y2, int x3, int y3, char ch) {
    int min_x = x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3);
    int max_x = x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3);
    int min_y = y1 < y2 ? (y1 < y3 ? y1 : y3) : (y2 < y3 ? y2 : y3);
    int max_y = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);
    
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            if (point_in_triangle(x, y, x1, y1, x2, y2, x3, y3)) {
                set_pixel(editor, x, y, ch);
            }
        }
    }
}

// Draw a triangle
int draw_triangle(GraphicsEditor* editor, int x1, int y1, int x2, int y2, int x3, int y3, char ch, int filled) {
    if (editor->object_count >= MAX_OBJECTS) {
        printf("Error: Maximum objects reached!\n");
        return -1;
    }
    
    if (filled) {
        draw_filled_triangle(editor, x1, y1, x2, y2, x3, y3, ch);
    } else {
        // Draw outline
        draw_line_internal(editor, x1, y1, x2, y2, ch);
        draw_line_internal(editor, x2, y2, x3, y3, ch);
        draw_line_internal(editor, x3, y3, x1, y1, ch);
    }
    
    DrawObject obj;
    obj.id = editor->next_id++;
    strcpy(obj.type, "triangle");
    obj.x1 = x1;
    obj.y1 = y1;
    obj.x2 = x2;
    obj.y2 = y2;
    obj.x3 = x3;
    obj.y3 = y3;
    obj.character = ch;
    obj.filled = filled;
    
    editor->objects[editor->object_count++] = obj;
    return obj.id;
}

// Delete an object by ID
int delete_object(GraphicsEditor* editor, int obj_id) {
    int found = -1;
    
    // Find the object
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
    
    // Remove the object
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
        } else if (strcmp(obj->type, "circle") == 0) {
            draw_circle_internal(editor, obj->cx, obj->cy, obj->radius, obj->character, obj->filled);
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
        } else if (strcmp(obj->type, "triangle") == 0) {
            if (obj->filled) {
                draw_filled_triangle(editor, obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3, obj->character);
            } else {
                draw_line_internal(editor, obj->x1, obj->y1, obj->x2, obj->y2, obj->character);
                draw_line_internal(editor, obj->x2, obj->y2, obj->x3, obj->y3, obj->character);
                draw_line_internal(editor, obj->x3, obj->y3, obj->x1, obj->y1, obj->character);
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
        } else if (strcmp(obj->type, "circle") == 0) {
            printf(" center (%d,%d) radius %d", obj->cx, obj->cy, obj->radius);
            if (obj->filled) printf(" [FILLED]");
        } else if (strcmp(obj->type, "triangle") == 0) {
            printf(" (%d,%d) (%d,%d) (%d,%d)", obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
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
    printf("2D Graphics Editor - Demo\n");
    printf("==================================================\n\n");
    
    printf("Drawing shapes...\n");
    
    // Draw a rectangle
    draw_rectangle(editor, 10, 5, 20, 10, '*', 0);
    printf("* Rectangle drawn\n");
    
    // Draw a filled circle
    draw_circle(editor, 50, 12, 6, '*', 1);
    printf("* Filled circle drawn\n");
    
    // Draw a triangle
    draw_triangle(editor, 15, 20, 25, 18, 20, 24, '*', 0);
    printf("* Triangle drawn\n");
    
    // Draw some lines
    draw_line(editor, 5, 2, 35, 15, '*');
    draw_line(editor, 60, 5, 75, 20, '*');
    printf("* Lines drawn\n");
    
    // Display the canvas
    printf("\nCanvas:\n");
    display_canvas(editor);
    
    // List objects
    list_objects(editor);
    
    // Delete an object
    printf("\nDeleting object ID 2...\n");
    delete_object(editor, 2);
    display_canvas(editor);
    
    // Add more shapes
    printf("Adding a filled rectangle...\n");
    draw_rectangle(editor, 65, 1, 12, 5, '_', 1);
    display_canvas(editor);
}

int main(int argc, char* argv[]) {
    GraphicsEditor* editor = init_editor();
    
    // Check for demo mode
    if (argc > 1 && strcmp(argv[1], "demo") == 0) {
        demo(editor);
    } else {
        // Interactive mode
        printf("2D Graphics Editor - Interactive Mode\n");
        printf("=====================================\n");
        printf("Commands: line, rect, circle, triangle, display, list, delete, clear, help, exit\n\n");
        
        int running = 1;
        while (running) {
            printf("> ");
            
            char command[256];
            if (fgets(command, sizeof(command), stdin) == NULL) {
                break;
            }
            
            // Remove newline
            command[strcspn(command, "\n")] = 0;
            
            char* parts[10];
            int part_count = 0;
            
            // Parse command
            char* token = strtok(command, " ");
            while (token != NULL && part_count < 10) {
                parts[part_count++] = token;
                token = strtok(NULL, " ");
            }
            
            if (part_count == 0) continue;
            
            if (strcmp(parts[0], "exit") == 0) {
                printf("Goodbye!\n");
                running = 0;
            } else if (strcmp(parts[0], "help") == 0) {
                printf("Commands: line, rect, circle, triangle, display, list, delete, clear, help, exit\n");
            } else if (strcmp(parts[0], "display") == 0) {
                display_canvas(editor);
            } else if (strcmp(parts[0], "list") == 0) {
                list_objects(editor);
            } else if (strcmp(parts[0], "clear") == 0) {
                clear_canvas(editor);
                printf("Canvas cleared!\n");
            } else if (strcmp(parts[0], "line") == 0) {
                if (part_count < 5) {
                    printf("Usage: line <x1> <y1> <x2> <y2> [char]\n");
                } else {
                    int x1 = atoi(parts[1]);
                    int y1 = atoi(parts[2]);
                    int x2 = atoi(parts[3]);
                    int y2 = atoi(parts[4]);
                    char ch = (part_count > 5) ? parts[5][0] : '*';
                    int id = draw_line(editor, x1, y1, x2, y2, ch);
                    printf("Line created (ID: %d)\n", id);
                }
            } else if (strcmp(parts[0], "rect") == 0) {
                if (part_count < 5) {
                    printf("Usage: rect <x> <y> <width> <height> [char] [filled]\n");
                } else {
                    int x = atoi(parts[1]);
                    int y = atoi(parts[2]);
                    int w = atoi(parts[3]);
                    int h = atoi(parts[4]);
                    char ch = (part_count > 5) ? parts[5][0] : '*';
                    int filled = (part_count > 6) ? (strcmp(parts[6], "true") == 0) : 0;
                    int id = draw_rectangle(editor, x, y, w, h, ch, filled);
                    printf("Rectangle created (ID: %d)\n", id);
                }
            } else if (strcmp(parts[0], "circle") == 0) {
                if (part_count < 4) {
                    printf("Usage: circle <cx> <cy> <radius> [char] [filled]\n");
                } else {
                    int cx = atoi(parts[1]);
                    int cy = atoi(parts[2]);
                    int r = atoi(parts[3]);
                    char ch = (part_count > 4) ? parts[4][0] : '*';
                    int filled = (part_count > 5) ? (strcmp(parts[5], "true") == 0) : 0;
                    int id = draw_circle(editor, cx, cy, r, ch, filled);
                    printf("Circle created (ID: %d)\n", id);
                }
            } else if (strcmp(parts[0], "triangle") == 0) {
                if (part_count < 7) {
                    printf("Usage: triangle <x1> <y1> <x2> <y2> <x3> <y3> [char] [filled]\n");
                } else {
                    int x1 = atoi(parts[1]);
                    int y1 = atoi(parts[2]);
                    int x2 = atoi(parts[3]);
                    int y2 = atoi(parts[4]);
                    int x3 = atoi(parts[5]);
                    int y3 = atoi(parts[6]);
                    char ch = (part_count > 7) ? parts[7][0] : '*';
                    int filled = (part_count > 8) ? (strcmp(parts[8], "true") == 0) : 0;
                    int id = draw_triangle(editor, x1, y1, x2, y2, x3, y3, ch, filled);
                    printf("Triangle created (ID: %d)\n", id);
                }
            } else if (strcmp(parts[0], "delete") == 0) {
                if (part_count < 2) {
                    printf("Usage: delete <id>\n");
                } else {
                    int id = atoi(parts[1]);
                    if (delete_object(editor, id)) {
                        printf("Object %d deleted!\n", id);
                    }
                }
            } else {
                printf("Unknown command: %s. Type 'help' for help.\n", parts[0]);
            }
        }
    }
    
    free_editor(editor);
    return 0;
}
