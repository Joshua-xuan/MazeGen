#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define symbols in the maze
#define WALL '#'
#define PATH ' '
#define START 'S'
#define END 'E'
#define DIAMOND '*'
#define TRAP 'T'

// Maze structure
typedef struct {
    char **map;  // Maze map
    int width;   // Maze width
    int height;  // Maze height
    int start_x; // Start point x coordinate
    int start_y; // Start point y coordinate
    int end_x;   // End point x coordinate
    int end_y;   // End point y coordinate
} Maze;

// Point structure
typedef struct {
    int x;
    int y;
} Point;

// Function prototypes
Maze *generate_maze(int width, int height);

void idfs(Maze *maze);

void reset_maze(Maze *maze);

void free_maze(Maze *maze);

int save_maze(Maze *maze, const char *filename);

void generate_maze_with_features(int width, int height, const char *filename);

int is_path_exist(Maze *maze, int x1, int y1, int x2, int y2, int **visited);

void add_diamonds(Maze *maze);

void add_traps(Maze *maze, Point *paths, int num_paths);

int find_paths(Maze *maze, Point *paths);

void find_paths_recursive(Maze *maze, int x, int y, int end_x, int end_y, int **visited, Point *paths, int *num_paths);

int is_valid_trap_position(int x, int y, Point *paths, int num_paths);

// Generate the maze using Iterative Depth-First Search (IDFS) algorithm
void idfs(Maze *maze) {
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    int stack_x[maze->width * maze->height];
    int stack_y[maze->width * maze->height];
    int stack_top = 0;

    // Randomly select the start position
    maze->start_x = rand() % (maze->width);
    maze->start_y = rand() % (maze->height);

    // Randomly select the end position
    maze->end_x = rand() % (maze->width);
    maze->end_y = rand() % (maze->height);

    stack_x[stack_top] = maze->start_x;
    stack_y[stack_top] = maze->start_y;
    stack_top++;

    while (stack_top > 0) {
        int x = stack_x[stack_top - 1];
        int y = stack_y[stack_top - 1];
        stack_top--;

        int index[] = {0, 1, 2, 3};
        for (int i = 0; i < 4; i++) {
            int r = i + rand() % (4 - i);
            int tmp = index[i];
            index[i] = index[r];
            index[r] = tmp;
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[index[i]];
            int ny = y + dy[index[i]];
            int nnx = nx + dx[index[i]];
            int nny = ny + dy[index[i]];

            if (nnx >= 0 && nnx < maze->width && nny >= 0 && nny < maze->height && maze->map[nny][nnx] == WALL) {
                maze->map[ny][nx] = PATH;
                maze->map[nny][nnx] = PATH;
                stack_x[stack_top] = nnx;
                stack_y[stack_top] = nny;
                stack_top++;
            }
        }
    }
}

// Check if a path exists between two points in the maze
int is_path_exist(Maze *maze, int x1, int y1, int x2, int y2, int **visited) {
    if (x1 == x2 && y1 == y2) {
        return 1;
    }

    visited[y1][x1] = 1; // Mark the current cell as visited

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        int nx = x1 + dx[i];
        int ny = y1 + dy[i];
        if (nx >= 0 && nx < maze->width && ny >= 0 && ny < maze->height && maze->map[ny][nx] == PATH &&
            !visited[ny][nx]) {
            if (is_path_exist(maze, nx, ny, x2, y2, visited)) {
                return 1;
            }
        }
    }

    // Reset visited flag if no path found from this cell
    visited[y1][x1] = 0;

    return 0;
}

// Add diamonds to the maze
void add_diamonds(Maze *maze) {
    int num_diamonds = maze->width * maze->height / 10;
    for (int i = 0; i < num_diamonds; i++) {
        int x, y;
        do {
            x = rand() % (maze->width);
            y = rand() % (maze->height);
        } while (maze->map[y][x] != PATH);
        maze->map[y][x] = DIAMOND;
    }
}

// Add traps to the maze
void add_traps(Maze *maze, Point *paths, int num_paths) {
    int num_traps = maze->width * maze->height / 20;

    for (int i = 0; i < num_traps; i++) {
        int x, y;
        do {
            x = rand() % (maze->width);
            y = rand() % (maze->height);
        } while (maze->map[y][x] != PATH && (x != maze->start_x || y != maze->start_y) &&
                 (x != maze->end_x || y != maze->end_y) && !is_valid_trap_position(x, y, paths, num_paths));

        maze->map[y][x] = TRAP;
    }
}

// Check if a given position is a valid trap position
int is_valid_trap_position(int x, int y, Point *paths, int num_paths) {
    for (int i = 0; i < num_paths; i++) {
        if (paths[i].x == x && paths[i].y == y) {
            return 1;
        }
    }
    return 0;
}

// Find all paths in the maze
int find_paths(Maze *maze, Point *paths) {
    int **visited = (int **) malloc(maze->height * sizeof(int *));
    if (visited == NULL) {
        return -1;
    }

    for (int i = 0; i < maze->height; i++) {
        visited[i] = (int *) calloc(maze->width, sizeof(int));
        if (visited[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(visited[j]);
            }
            free(visited);
            return -1;
        }
    }

    int num_paths = 0;
    find_paths_recursive(maze, maze->start_x, maze->start_y, maze->end_x, maze->end_y, visited, paths, &num_paths);

    for (int i = 0; i < maze->height; i++) {
        free(visited[i]);
    }
    free(visited);

    return num_paths;
}

// Recursive function to find all paths in the maze
void find_paths_recursive(Maze *maze, int x, int y, int end_x, int end_y, int **visited, Point *paths, int *num_paths) {
    visited[y][x] = 1;
    paths[*num_paths].x = x;
    paths[*num_paths].y = y;
    (*num_paths)++;

    if (x == end_x && y == end_y) {
        return;
    }

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < maze->width && ny >= 0 && ny < maze->height && maze->map[ny][nx] == PATH &&
            !visited[ny][nx]) {
            find_paths_recursive(maze, nx, ny, end_x, end_y, visited, paths, num_paths);
        }
    }

    visited[y][x] = 0;
    (*num_paths)--;
}

// Reset the maze
void reset_maze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->map[i][j] = WALL;
        }
    }
}

// Free memory allocated for the maze
void free_maze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->map[i]);
    }
    free(maze->map);
    free(maze);
}

/**
 * @brief Generate a maze *
 * @param width width of the maze
 * @param height height of the maze
 * @return Maze* pointer to the generated maze structure
 * */
Maze *generate_maze(int width, int height) {
    Maze *maze = (Maze *) malloc(sizeof(Maze));
    if (maze == NULL) {
        return NULL;
    }

    // Allocate memory for the map
    maze->map = (char **) malloc(height * sizeof(char *));
    if (maze->map == NULL) {
        free(maze);
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        maze->map[i] = (char *) malloc(width * sizeof(char));
        if (maze->map[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(maze->map[j]);
            }
            free(maze->map);
            free(maze);
            return NULL;
        }
    }

    // Initialize the maze with walls
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze->map[i][j] = WALL;
        }
    }

    // Set width points and height points
    maze->width = width;
    maze->height = height;

    // Generate the maze
    int num_paths = 0;
    Point *paths = (Point *) malloc(width * height * sizeof(Point));
    do {
        reset_maze(maze);
        idfs(maze);
        num_paths = find_paths(maze, paths);
    } while (num_paths == 0);

    maze->map[maze->start_y][maze->start_x] = START;
    maze->map[maze->end_y][maze->end_x] = END;

    // Add diamonds
    add_diamonds(maze);

    // Add traps
    add_traps(maze, paths, num_paths);

    free(paths);

    return maze;
}

// Save the maze to a file
int save_maze(Maze *maze, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return 1;
    }

    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            fputc(maze->map[i][j], fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);
    return 0;
}

// Generate a maze with diamonds and traps
void generate_maze_with_features(int width, int height, const char *filename) {
    printf("Generating maze...\n");

    // Generate the maze
    Maze *maze = generate_maze(width, height);
    if (maze == NULL) {
        printf("Error: Failed to generate maze\n");
        return;
    }

    // Save the maze to a file
    if (save_maze(maze, filename)) {
        printf("Error: Failed to save maze to file\n");
    } else {
        printf("Maze saved to %s\n", filename);
    }

    // Free memory
    free_maze(maze);
}

// Main function
int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 4) {
        printf("Usage: %s <filename> <width> <height>\n", argv[0]);
        return 1;
    }

    // Parse command line arguments
    char *filename = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    // Check for valid width and height
    if (width < 5 || width > 100 || height < 5 || height > 100) {
        printf("Error: Width and height must be between 5 and 100\n");
        return 1;
    }

    // Initialize random seed
    srand(time(NULL));

    // Generate the maze with diamonds and traps
    generate_maze_with_features(width, height, filename);

    return 0;
}
