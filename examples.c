#include <stdio.h>
#include <stdlib.h>
#include "graphics_editor.h"

// Example 1: Basic Shapes
void example_1_basic_shapes(void) {
    printf("Example 1: Basic Shapes\n");
    printf("----------------------------------------\n\n");
    
    GraphicsEditor* editor = init_editor();
    
    draw_line(editor, 5, 5, 20, 5, '*');
    draw_rectangle(editor, 25, 3, 12, 8, '*', 0);
    draw_circle(editor, 50, 8, 4, '*', 0);
    draw_triangle(editor, 10, 15, 20, 15, 15, 18, '*', 0);
    
    display_canvas(editor);
    printf("\n");
    
    free_editor(editor);
}

// Example 2: Filled Shapes
void example_2_filled_shapes(void) {
    printf("Example 2: Filled Shapes\n");
    printf("----------------------------------------\n\n");
    
    GraphicsEditor* editor = init_editor();
    
    draw_rectangle(editor, 5, 2, 15, 10, '*', 1);
    draw_circle(editor, 35, 8, 5, '*', 1);
    draw_triangle(editor, 50, 15, 65, 15, 57, 20, '*', 1);
    
    display_canvas(editor);
    printf("\n");
    
    free_editor(editor);
}

// Example 3: Different Characters
void example_3_different_characters(void) {
    printf("Example 3: Different Characters\n");
    printf("----------------------------------------\n\n");
    
    GraphicsEditor* editor = init_editor();
    
    draw_rectangle(editor, 5, 2, 15, 8, '*', 0);
    draw_rectangle(editor, 25, 2, 15, 8, '_', 0);
    draw_circle(editor, 50, 6, 4, '*', 0);
    draw_circle(editor, 60, 6, 4, '_', 0);
    
    display_canvas(editor);
    printf("\n");
    
    free_editor(editor);
}

// Example 4: Complex Scene (House)
void example_4_complex_scene(void) {
    printf("Example 4: Complex Scene - House\n");
    printf("----------------------------------------\n\n");
    
    GraphicsEditor* editor = init_editor();
    
    // Main wall
    draw_rectangle(editor, 20, 10, 30, 12, '*', 0);
    
    // Roof
    draw_triangle(editor, 20, 10, 50, 10, 35, 3, '*', 0);
    
    // Windows
    draw_rectangle(editor, 25, 12, 4, 4, '_', 0);
    draw_rectangle(editor, 40, 12, 4, 4, '_', 0);
    
    // Door
    draw_rectangle(editor, 34, 18, 2, 4, '_', 0);
    
    // Tree trunk
    draw_line(editor, 60, 15, 60, 22, '*');
    
    // Tree foliage
    draw_circle(editor, 60, 12, 3, '*', 0);
    
    display_canvas(editor);
    list_objects(editor);
    printf("\n");
    
    free_editor(editor);
}

// Example 5: Add and Delete Operations
void example_5_add_delete(void) {
    printf("Example 5: Add and Delete Operations\n");
    printf("----------------------------------------\n\n");
    
    GraphicsEditor* editor = init_editor();
    
    printf("Creating 3 objects...\n");
    int id1 = draw_rectangle(editor, 5, 2, 12, 6, '*', 0);
    int id2 = draw_circle(editor, 30, 6, 4, '*', 0);
    int id3 = draw_triangle(editor, 50, 12, 55, 12, 52, 8, '*', 0);
    
    printf("Canvas with 3 objects:\n");
    display_canvas(editor);
    list_objects(editor);
    
    printf("\nDeleting object %d (circle)...\n", id2);
    delete_object(editor, id2);
    
    printf("Canvas after deletion:\n");
    display_canvas(editor);
    list_objects(editor);
    
    printf("\n");
    
    free_editor(editor);
}

// Example 6: Grid Pattern
void example_6_grid(void) {
    printf("Example 6: Grid Pattern\n");
    printf("----------------------------------------\n\n");
    
    GraphicsEditor* editor = init_editor();
    
    // Draw vertical lines
    for (int x = 5; x < 45; x += 8) {
        draw_line(editor, x, 2, x, 14, '*');
    }
    
    // Draw horizontal lines
    for (int y = 2; y < 15; y += 4) {
        draw_line(editor, 5, y, 45, y, '*');
    }
    
    display_canvas(editor);
    printf("\n");
    
    free_editor(editor);
}

// Example 7: Multiple Circles
void example_7_circles(void) {
    printf("Example 7: Multiple Circles\n");
    printf("----------------------------------------\n\n");
    
    GraphicsEditor* editor = init_editor();
    
    // Outline circles
    draw_circle(editor, 15, 8, 5, '*', 0);
    draw_circle(editor, 40, 8, 5, '*', 0);
    draw_circle(editor, 65, 8, 5, '*', 0);
    
    // Filled circles
    draw_circle(editor, 15, 18, 4, '_', 1);
    draw_circle(editor, 40, 18, 4, '_', 1);
    draw_circle(editor, 65, 18, 4, '_', 1);
    
    display_canvas(editor);
    printf("\n");
    
    free_editor(editor);
}

// Example 8: Nested Shapes
void example_8_nested_shapes(void) {
    printf("Example 8: Nested Shapes\n");
    printf("----------------------------------------\n\n");
    
    GraphicsEditor* editor = init_editor();
    
    // Outer rectangle
    draw_rectangle(editor, 5, 2, 30, 16, '*', 0);
    
    // Inner rectangle
    draw_rectangle(editor, 10, 5, 20, 10, '*', 0);
    
    // Circle in the middle
    draw_circle(editor, 20, 10, 3, '*', 1);
    
    display_canvas(editor);
    printf("\n");
    
    free_editor(editor);
}

int main(void) {
    printf("\n");
    printf("==================================================\n");
    printf("2D Graphics Editor - C Examples\n");
    printf("==================================================\n\n");
    
    example_1_basic_shapes();
    example_2_filled_shapes();
    example_3_different_characters();
    example_4_complex_scene();
    example_5_add_delete();
    example_6_grid();
    example_7_circles();
    example_8_nested_shapes();
    
    printf("==================================================\n");
    printf("All examples completed!\n");
    printf("==================================================\n");
    
    return 0;
}
