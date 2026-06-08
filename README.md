# 2D Graphics Editor in C

A simple ASCII-based 2D graphics editor that allows you to draw shapes using `*` and `_` characters on a 2D canvas.

## Features

- **Draw Shapes**: Line, Rectangle, Circle, Triangle
- **Fill Support**: All shapes can be drawn filled or as outlines
- **Object Management**: Add, delete, and list objects
- **Interactive Mode**: Command-line interface for interactive drawing
- **Demo Mode**: Pre-built demo showcasing features

## Compilation

```bash
gcc -o graphics_editor graphics_editor.c -lm
```

The `-lm` flag links the math library needed for circle drawing.

## Usage

### Demo Mode
```bash
./graphics_editor demo
```

This runs a demonstration showing:
- Drawing various shapes
- Displaying the canvas
- Listing objects
- Deleting objects
- Adding filled shapes

### Interactive Mode
```bash
./graphics_editor
```

Then use commands:

#### Drawing Commands

**Line**
```
line <x1> <y1> <x2> <y2> [char]
```
Example: `line 5 5 20 15 *`

**Rectangle**
```
rect <x> <y> <width> <height> [char] [filled]
```
Examples:
- `rect 10 5 20 10 *` - Rectangle outline
- `rect 10 5 20 10 * true` - Filled rectangle

**Circle**
```
circle <cx> <cy> <radius> [char] [filled]
```
Examples:
- `circle 40 12 6 *` - Circle outline
- `circle 40 12 6 * true` - Filled circle

**Triangle**
```
triangle <x1> <y1> <x2> <y2> <x3> <y3> [char] [filled]
```
Examples:
- `triangle 15 20 25 18 20 24 *` - Triangle outline
- `triangle 15 20 25 18 20 24 * true` - Filled triangle

#### Utility Commands

- `display` - Show the current canvas
- `list` - List all objects with their IDs
- `delete <id>` - Delete object by ID
- `clear` - Clear the entire canvas
- `help` - Show command help
- `exit` - Exit the program

## Canvas

- **Width**: 80 characters
- **Height**: 25 characters
- **Default Character**: `*`
- **Alternative Character**: `_` (or any other character)

## Drawing Algorithms

### Line Drawing
Uses Bresenham's line algorithm for accurate line drawing.

### Circle Drawing
- **Outline**: Uses Bresenham's circle algorithm with 8-octant symmetry
- **Filled**: Uses distance-based algorithm

### Rectangle
- **Outline**: Draws top, bottom, left, right edges
- **Filled**: Fills all pixels within bounds

### Triangle
- **Outline**: Draws three edges connecting vertices
- **Filled**: Uses barycentric coordinate test to fill interior

## Data Structure

### DrawObject
```c
typedef struct {
    int id;                  // Unique object identifier
    char type[20];          // "line", "rectangle", "circle", "triangle"
    int x1, y1, x2, y2;     // Coordinates for line/triangle
    int x, y, width, height; // Rectangle parameters
    int cx, cy, radius;     // Circle parameters
    int x3, y3;             // Third point for triangle
    char character;         // Character used for drawing
    int filled;             // 0 for outline, 1 for filled
} DrawObject;
```

### GraphicsEditor
```c
typedef struct {
    char canvas[25][80];        // 2D array for the drawing surface
    DrawObject objects[100];    // Array of drawn objects
    int object_count;          // Number of objects
    int next_id;               // Next available object ID
} GraphicsEditor;
```

## Examples

### Drawing a House
```
rect 20 10 30 12 *
triangle 20 10 50 10 35 3 *
rect 25 12 4 4 _
rect 40 12 4 4 _
rect 34 18 2 4 _
```

### Drawing a Grid
```
line 5 2 45 2 *
line 5 6 45 6 *
line 5 10 45 10 *
line 5 14 45 14 *
line 5 2 5 14 *
line 15 2 15 14 *
line 25 2 25 14 *
line 35 2 35 14 *
line 45 2 45 14 *
```

### Drawing Circles
```
circle 30 8 5 *
circle 50 8 5 * true
circle 70 8 5 _
```

## Limitations

- Maximum 100 objects on canvas
- Fixed canvas size (80x80 characters)
- Single-character drawing (no multi-color support)
- Cannot modify objects after creation (delete and redraw required)

## File Structure

- `graphics_editor.c` - Main implementation
- `graphics_editor.h` - Header file with function declarations
- `README.md` - This documentation

## Compilation Options

For optimization:
```bash
gcc -O2 -o graphics_editor graphics_editor.c -lm
```

With debug symbols:
```bash
gcc -g -o graphics_editor graphics_editor.c -lm
```

## Implementation Details

### Memory Management
- GraphicsEditor structure is dynamically allocated
- Canvas is statically allocated within the structure (600 characters + object metadata)

### Performance
- Linear time object deletion (requires canvas redraw)
- Optimal drawing algorithms for each shape type
- No unnecessary memory allocations during drawing

### Code Quality
- Clear separation between drawing logic and UI
- Reusable drawing functions for internal use
- Comprehensive input validation
