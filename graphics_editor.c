#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CANVAS_WIDTH 80

#define CANVAS_HEIGHT 25
#define MAX_OBJECTS 100
#define BG_CHAR '_'

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
    
    // Initialize canvas with background characters
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            editor->canvas[y][x] = BG_CHAR;
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
    return BG_CHAR;
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
void draw_rectangle_internal(GraphicsEditor* editor, int x, int y, int width, int height, char ch, int filled) {
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
}

// Draw a rectangle
int draw_rectangle(GraphicsEditor* editor, int x, int y, int width, int height, char ch, int filled) {
    if (editor->object_count >= MAX_OBJECTS) {
        printf("Error: Maximum objects reached!\n");
        return -1;
    }
    
    draw_rectangle_internal(editor, x, y, width, height, ch, filled);
    
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
void draw_triangle_internal(GraphicsEditor* editor, int x1, int y1, int x2, int y2, int x3, int y3, char ch, int filled) {
    if (filled) {
        draw_filled_triangle(editor, x1, y1, x2, y2, x3, y3, ch);
    } else {
        // Draw outline
        draw_line_internal(editor, x1, y1, x2, y2, ch);
        draw_line_internal(editor, x2, y2, x3, y3, ch);
        draw_line_internal(editor, x3, y3, x1, y1, ch);
    }
}

// Draw a triangle
int draw_triangle(GraphicsEditor* editor, int x1, int y1, int x2, int y2, int x3, int y3, char ch, int filled) {
    if (editor->object_count >= MAX_OBJECTS) {
        printf("Error: Maximum objects reached!\n");
        return -1;
    }
    
    draw_triangle_internal(editor, x1, y1, x2, y2, x3, y3, ch, filled);
    
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
void draw_object_internal(GraphicsEditor* editor, DrawObject* obj) {
    if (strcmp(obj->type, "line") == 0) {
        draw_line_internal(editor, obj->x1, obj->y1, obj->x2, obj->y2, obj->character);
    } else if (strcmp(obj->type, "circle") == 0) {
        draw_circle_internal(editor, obj->cx, obj->cy, obj->radius, obj->character, obj->filled);
    } else if (strcmp(obj->type, "rectangle") == 0) {
        draw_rectangle_internal(editor, obj->x, obj->y, obj->width, obj->height, obj->character, obj->filled);
    } else if (strcmp(obj->type, "triangle") == 0) {
        draw_triangle_internal(editor, obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3, obj->character, obj->filled);
    }
}

void redraw_all(GraphicsEditor* editor) {
    // Clear canvas
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            editor->canvas[y][x] = BG_CHAR;
        }
    }
    // Redraw all remaining objects
    for (int i = 0; i < editor->object_count; i++) {
        draw_object_internal(editor, &editor->objects[i]);
    }
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
    redraw_all(editor);
    
    return 1;
}

// Modify an object by ID (advanced properties)
int modify_object(GraphicsEditor* editor, int obj_id, const char* property, int param_count, char* params[]) {
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
    
    DrawObject* obj = &editor->objects[found];
    
    if (strcmp(property, "char") == 0) {
        if (param_count < 1) {
            printf("Error: Missing character value!\n");
            return 0;
        }
        obj->character = params[0][0];
        printf("Object %d character updated to '%c'\n", obj_id, obj->character);
    } else if (strcmp(property, "fill") == 0) {
        if (param_count < 1) {
            printf("Error: Missing fill value (true/false)!\n");
            return 0;
        }
        obj->filled = (strcmp(params[0], "true") == 0 || strcmp(params[0], "1") == 0);
        printf("Object %d filled status updated to %s\n", obj_id, obj->filled ? "true" : "false");
    } else if (strcmp(property, "move") == 0) {
        if (param_count < 2) {
            printf("Error: Missing dx and dy values!\n");
            return 0;
        }
        int dx = atoi(params[0]);
        int dy = atoi(params[1]);
        if (strcmp(obj->type, "line") == 0) {
            obj->x1 += dx; obj->y1 += dy;
            obj->x2 += dx; obj->y2 += dy;
        } else if (strcmp(obj->type, "circle") == 0) {
            obj->cx += dx; obj->cy += dy;
        } else if (strcmp(obj->type, "rectangle") == 0) {
            obj->x += dx; obj->y += dy;
        } else if (strcmp(obj->type, "triangle") == 0) {
            obj->x1 += dx; obj->y1 += dy;
            obj->x2 += dx; obj->y2 += dy;
            obj->x3 += dx; obj->y3 += dy;
        }
        printf("Object %d moved by delta (%d, %d)\n", obj_id, dx, dy);
    } else if (strcmp(property, "resize") == 0) {
        if (strcmp(obj->type, "line") == 0) {
            if (param_count < 4) {
                printf("Error: line resize requires new x1 y1 x2 y2!\n");
                return 0;
            }
            obj->x1 = atoi(params[0]); obj->y1 = atoi(params[1]);
            obj->x2 = atoi(params[2]); obj->y2 = atoi(params[3]);
            printf("Line %d resized to (%d,%d) to (%d,%d)\n", obj_id, obj->x1, obj->y1, obj->x2, obj->y2);
        } else if (strcmp(obj->type, "circle") == 0) {
            if (param_count < 1) {
                printf("Error: circle resize requires new radius!\n");
                return 0;
            }
            obj->radius = atoi(params[0]);
            printf("Circle %d resized to radius %d\n", obj_id, obj->radius);
        } else if (strcmp(obj->type, "rectangle") == 0) {
            if (param_count < 2) {
                printf("Error: rectangle resize requires new width and height!\n");
                return 0;
            }
            obj->width = atoi(params[0]);
            obj->height = atoi(params[1]);
            printf("Rectangle %d resized to size %dx%d\n", obj_id, obj->width, obj->height);
        } else if (strcmp(obj->type, "triangle") == 0) {
            if (param_count < 6) {
                printf("Error: triangle resize requires new x1 y1 x2 y2 x3 y3!\n");
                return 0;
            }
            obj->x1 = atoi(params[0]); obj->y1 = atoi(params[1]);
            obj->x2 = atoi(params[2]); obj->y2 = atoi(params[3]);
            obj->x3 = atoi(params[4]); obj->y3 = atoi(params[5]);
            printf("Triangle %d resized to (%d,%d), (%d,%d), (%d,%d)\n", obj_id, obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
        }
    } else {
        printf("Error: Unknown property '%s'\n", property);
        return 0;
    }
    
    // Redraw canvas
    redraw_all(editor);
    
    return 1;
}

// Clear the canvas
void clear_canvas(GraphicsEditor* editor) {
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            editor->canvas[y][x] = BG_CHAR;
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

// Helper to read stdin safely
void get_line(char* buf, int size) {
    if (fgets(buf, size, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = 0;
    } else {
        buf[0] = 0;
    }
}

// Forward declaration of demo
void demo(GraphicsEditor* editor);

// Menu-driven loop
void menu_loop(GraphicsEditor* editor) {
    char input_buf[256];
    int running = 1;
    
    // Add default initial shapes so the user has something to see and modify immediately
    draw_rectangle(editor, 10, 5, 20, 10, '*', 0);
    draw_circle(editor, 50, 12, 6, '*', 0);
    draw_triangle(editor, 15, 20, 25, 18, 20, 24, '*', 0);
    draw_line(editor, 5, 2, 35, 15, '*');
    
    // Display the initial canvas right away
    printf("\nInitial Canvas:\n");
    display_canvas(editor);
    
    while (running) {
        printf("\n=== 2D Graphics Editor Menu ===\n");
        printf("1. Display Canvas\n");
        printf("2. List All Shapes\n");
        printf("3. Add a Shape\n");
        printf("4. Modify a Shape\n");
        printf("5. Delete a Shape\n");
        printf("6. Clear Canvas\n");
        printf("7. Run Auto Demo\n");
        printf("8. Exit\n");
        printf("Enter choice (1-8): ");
        
        get_line(input_buf, sizeof(input_buf));
        int choice = atoi(input_buf);
        
        switch (choice) {
            case 1:
                display_canvas(editor);
                break;
            case 2:
                list_objects(editor);
                break;
            case 3: {
                printf("\nSelect Shape Type:\n");
                printf("  1. Line\n");
                printf("  2. Rectangle\n");
                printf("  3. Circle\n");
                printf("  4. Triangle\n");
                printf("Enter shape choice (1-4): ");
                get_line(input_buf, sizeof(input_buf));
                int shape_choice = atoi(input_buf);
                
                if (shape_choice == 1) {
                    int x1, y1, x2, y2;
                    char ch;
                    printf("Enter coordinates (x1 y1 x2 y2), e.g. 5 2 35 15: ");
                    get_line(input_buf, sizeof(input_buf));
                    if (sscanf(input_buf, "%d %d %d %d", &x1, &y1, &x2, &y2) == 4) {
                        printf("Enter drawing character (default '*'): ");
                        get_line(input_buf, sizeof(input_buf));
                        ch = (strlen(input_buf) > 0) ? input_buf[0] : '*';
                        int id = draw_line(editor, x1, y1, x2, y2, ch);
                        printf("Line created with ID %d!\n", id);
                        display_canvas(editor);
                    } else {
                        printf("Invalid format! Expected 4 integers: x1 y1 x2 y2 (e.g. 5 2 35 15)\n");
                    }
                } else if (shape_choice == 2) {
                    int x, y, w, h, filled;
                    char ch;
                    printf("Enter position and size (x y width height), e.g. 10 5 20 10: ");
                    get_line(input_buf, sizeof(input_buf));
                    if (sscanf(input_buf, "%d %d %d %d", &x, &y, &w, &h) == 4) {
                        printf("Enter drawing character (default '*'): ");
                        get_line(input_buf, sizeof(input_buf));
                        ch = (strlen(input_buf) > 0) ? input_buf[0] : '*';
                        printf("Is it filled? (1 for yes, 0 for no): ");
                        get_line(input_buf, sizeof(input_buf));
                        filled = atoi(input_buf);
                        int id = draw_rectangle(editor, x, y, w, h, ch, filled);
                        printf("Rectangle created with ID %d!\n", id);
                        display_canvas(editor);
                    } else {
                        printf("Invalid format! Expected 4 integers: x y width height (e.g. 10 5 20 10)\n");
                    }
                } else if (shape_choice == 3) {
                    int cx, cy, r, filled;
                    char ch;
                    printf("Enter center and radius (cx cy radius), e.g. 40 12 5: ");
                    get_line(input_buf, sizeof(input_buf));
                    if (sscanf(input_buf, "%d %d %d", &cx, &cy, &r) == 3) {
                        printf("Enter drawing character (default '*'): ");
                        get_line(input_buf, sizeof(input_buf));
                        ch = (strlen(input_buf) > 0) ? input_buf[0] : '*';
                        printf("Is it filled? (1 for yes, 0 for no): ");
                        get_line(input_buf, sizeof(input_buf));
                        filled = atoi(input_buf);
                        int id = draw_circle(editor, cx, cy, r, ch, filled);
                        printf("Circle created with ID %d!\n", id);
                        display_canvas(editor);
                    } else {
                        printf("Invalid format! Expected 3 integers: cx cy radius (e.g. 40 12 5)\n");
                    }
                } else if (shape_choice == 4) {
                    int x1, y1, x2, y2, x3, y3, filled;
                    char ch;
                    printf("Enter 3 vertex coordinates (x1 y1 x2 y2 x3 y3), e.g. 15 20 25 18 20 24: ");
                    get_line(input_buf, sizeof(input_buf));
                    if (sscanf(input_buf, "%d %d %d %d %d %d", &x1, &y1, &x2, &y2, &x3, &y3) == 6) {
                        printf("Enter drawing character (default '*'): ");
                        get_line(input_buf, sizeof(input_buf));
                        ch = (strlen(input_buf) > 0) ? input_buf[0] : '*';
                        printf("Is it filled? (1 for yes, 0 for no): ");
                        get_line(input_buf, sizeof(input_buf));
                        filled = atoi(input_buf);
                        int id = draw_triangle(editor, x1, y1, x2, y2, x3, y3, ch, filled);
                        printf("Triangle created with ID %d!\n", id);
                        display_canvas(editor);
                    } else {
                        printf("Invalid format! Expected 6 integers: x1 y1 x2 y2 x3 y3 (e.g. 15 20 25 18 20 24)\n");
                    }
                } else {
                    printf("Invalid shape selection!\n");
                }
                break;
            }
            case 4: {
                list_objects(editor);
                if (editor->object_count == 0) break;
                
                printf("Enter ID of the shape to modify: ");
                get_line(input_buf, sizeof(input_buf));
                int id = atoi(input_buf);
                
                // Find shape
                int found = -1;
                for (int i = 0; i < editor->object_count; i++) {
                    if (editor->objects[i].id == id) {
                        found = i;
                        break;
                    }
                }
                if (found == -1) {
                    printf("Shape ID %d not found!\n", id);
                    break;
                }
                
                printf("\nSelect Property to Modify:\n");
                printf("  1. Character\n");
                printf("  2. Fill Status\n");
                printf("  3. Move (shift position)\n");
                printf("  4. Resize / Update size\n");
                printf("Enter modification choice (1-4): ");
                get_line(input_buf, sizeof(input_buf));
                int prop_choice = atoi(input_buf);
                
                if (prop_choice == 1) {
                    printf("Enter new drawing character: ");
                    get_line(input_buf, sizeof(input_buf));
                    char* params[] = { input_buf };
                    modify_object(editor, id, "char", 1, params);
                    display_canvas(editor);
                } else if (prop_choice == 2) {
                    printf("Set filled (1 for yes, 0 for no): ");
                    get_line(input_buf, sizeof(input_buf));
                    char* val = (atoi(input_buf) == 1) ? "true" : "false";
                    char* params[] = { val };
                    modify_object(editor, id, "fill", 1, params);
                    display_canvas(editor);
                } else if (prop_choice == 3) {
                    int dx, dy;
                    printf("Enter movement delta (dx dy): ");
                    get_line(input_buf, sizeof(input_buf));
                    if (sscanf(input_buf, "%d %d", &dx, &dy) == 2) {
                        char dx_str[16], dy_str[16];
                        sprintf(dx_str, "%d", dx);
                        sprintf(dy_str, "%d", dy);
                        char* params[] = { dx_str, dy_str };
                        modify_object(editor, id, "move", 2, params);
                        display_canvas(editor);
                    } else {
                        printf("Invalid offsets!\n");
                    }
                } else if (prop_choice == 4) {
                    DrawObject* obj = &editor->objects[found];
                    if (strcmp(obj->type, "line") == 0) {
                        int x1, y1, x2, y2;
                        printf("Enter new coordinates (x1 y1 x2 y2): ");
                        get_line(input_buf, sizeof(input_buf));
                        if (sscanf(input_buf, "%d %d %d %d", &x1, &y1, &x2, &y2) == 4) {
                            char x1s[16], y1s[16], x2s[16], y2s[16];
                            sprintf(x1s, "%d", x1); sprintf(y1s, "%d", y1);
                            sprintf(x2s, "%d", x2); sprintf(y2s, "%d", y2);
                            char* params[] = { x1s, y1s, x2s, y2s };
                            modify_object(editor, id, "resize", 4, params);
                            display_canvas(editor);
                        } else {
                            printf("Invalid coordinates!\n");
                        }
                    } else if (strcmp(obj->type, "circle") == 0) {
                        int r;
                        printf("Enter new radius: ");
                        get_line(input_buf, sizeof(input_buf));
                        r = atoi(input_buf);
                        char rs[16];
                        sprintf(rs, "%d", r);
                        char* params[] = { rs };
                        modify_object(editor, id, "resize", 1, params);
                        display_canvas(editor);
                    } else if (strcmp(obj->type, "rectangle") == 0) {
                        int w, h;
                        printf("Enter new width and height: ");
                        get_line(input_buf, sizeof(input_buf));
                        if (sscanf(input_buf, "%d %d", &w, &h) == 2) {
                            char ws[16], hs[16];
                            sprintf(ws, "%d", w); sprintf(hs, "%d", h);
                            char* params[] = { ws, hs };
                            modify_object(editor, id, "resize", 2, params);
                            display_canvas(editor);
                        } else {
                            printf("Invalid dimensions!\n");
                        }
                    } else if (strcmp(obj->type, "triangle") == 0) {
                        int x1, y1, x2, y2, x3, y3;
                        printf("Enter new vertex coordinates (x1 y1 x2 y2 x3 y3): ");
                        get_line(input_buf, sizeof(input_buf));
                        if (sscanf(input_buf, "%d %d %d %d %d %d", &x1, &y1, &x2, &y2, &x3, &y3) == 6) {
                            char x1s[16], y1s[16], x2s[16], y2s[16], x3s[16], y3s[16];
                            sprintf(x1s, "%d", x1); sprintf(y1s, "%d", y1);
                            sprintf(x2s, "%d", x2); sprintf(y2s, "%d", y2);
                            sprintf(x3s, "%d", x3); sprintf(y3s, "%d", y3);
                            char* params[] = { x1s, y1s, x2s, y2s, x3s, y3s };
                            modify_object(editor, id, "resize", 6, params);
                            display_canvas(editor);
                        } else {
                            printf("Invalid vertices!\n");
                        }
                    }
                } else {
                    printf("Invalid property choice!\n");
                }
                break;
            }
            case 5: {
                list_objects(editor);
                if (editor->object_count == 0) break;
                printf("Enter ID of the shape to delete: ");
                get_line(input_buf, sizeof(input_buf));
                int id = atoi(input_buf);
                delete_object(editor, id);
                display_canvas(editor);
                break;
            }
            case 6:
                clear_canvas(editor);
                printf("Canvas cleared!\n");
                display_canvas(editor);
                break;
            case 7:
                clear_canvas(editor);
                demo(editor);
                break;
            case 8:
                printf("Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Unknown option! Please enter a choice between 1 and 8.\n");
        }
    }
}

// Demo function showing adding, deleting, and modifying objects
void demo(GraphicsEditor* editor) {
    printf("2D Graphics Editor - Output Diagram (* and _ format)\n");
    printf("==================================================\n\n");
    
    printf("1. Adding shapes (Circle, Rectangle, Line, Triangle)...\n");
    
    // Draw a rectangle
    int rect_id = draw_rectangle(editor, 10, 5, 20, 10, '*', 0);
    printf("   * Rectangle drawn (ID: %d)\n", rect_id);
    
    // Draw a circle
    int circle_id = draw_circle(editor, 50, 12, 6, '*', 0);
    printf("   * Circle drawn (ID: %d)\n", circle_id);
    
    // Draw a triangle
    int tri_id = draw_triangle(editor, 15, 20, 25, 18, 20, 24, '*', 0);
    printf("   * Triangle drawn (ID: %d)\n", tri_id);
    
    // Draw a line
    int line_id = draw_line(editor, 5, 2, 35, 15, '*');
    printf("   * Line drawn (ID: %d)\n", line_id);
    
    // Display the initial canvas
    printf("\nCanvas (Initial):\n");
    display_canvas(editor);
    list_objects(editor);
    
    printf("\n2. Modifying shapes in the picture...\n");
    
    // Modify circle: Change to filled
    printf("   * Modifying Circle (ID: %d): Changing filled status to true\n", circle_id);
    char* fill_params[] = { "true" };
    modify_object(editor, circle_id, "fill", 1, fill_params);
    
    // Modify rectangle: Move it right by 5 and down by 2
    printf("   * Modifying Rectangle (ID: %d): Moving by delta (5, 2)\n", rect_id);
    char* move_params[] = { "5", "2" };
    modify_object(editor, rect_id, "move", 2, move_params);
    
    // Display modified canvas
    printf("\nCanvas (After Modifications):\n");
    display_canvas(editor);
    
    printf("\n3. Deleting shapes from the picture...\n");
    
    // Delete the triangle
    printf("   * Deleting Triangle (ID: %d)\n", tri_id);
    delete_object(editor, tri_id);
    
    // Display canvas after deletion
    printf("\nCanvas (After Deletion):\n");
    display_canvas(editor);
    list_objects(editor);
}

int main(void) {
    GraphicsEditor* editor = init_editor();
    
    printf("2D Graphics Editor\n");
    printf("==================\n");
    
    menu_loop(editor);
    
    free_editor(editor);
    return 0;
}
