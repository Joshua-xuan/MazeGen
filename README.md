# MazeGen
#   Iteration Log:
1. Does the program generate multiple maze designs? Does the program only generate valid mazes?
2. How does the program perform on a larger scale (100 * 100)? (Identify which part of the code causes this issue)
3. In which aspects does the program perform particularly poorly? (Identify which part of the code causes this situation)
4. How should we attempt next time?

### Iteration-1:
```
/**
 * @brief Generate a maze
 *
 * @param width width of the maze
 * @param height height of the maze
 * @return Maze* pointer to the generated maze structure
 */
Maze* generate_maze(int width, int height) {
    Maze *maze = (Maze*)malloc(sizeof(Maze));
    if (maze == NULL) {
        return NULL;
    }

    // Allocate memory for the map
    maze->map = (char**)malloc(height * sizeof(char*));
    if (maze->map == NULL) {
        free(maze);
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        maze->map[i] = (char*)malloc(width * sizeof(char));
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

    // Generate paths
    for (int i = 1; i < height - 1; i += 2) {
        for (int j = 1; j < width - 1; j += 2) {
            maze->map[i][j] = PATH;
        }
    }

    // Add horizontal paths
    for (int i = 1; i < height - 1; i += 2) {
        for (int j = 2; j < width - 1; j += 2) {
            if (rand() % 2 == 0) {
                maze->map[i][j] = PATH;
            }
        }
    }

    // Add vertical paths
    for (int i = 2; i < height - 1; i += 2) {
        for (int j = 1; j < width - 1; j += 2) {
            if (rand() % 2 == 0) {
                maze->map[i][j] = PATH;
            }
        }
    }

    // Set start and end points
    maze->width = width;
    maze->height = height;
    maze->start_x = 1;
    maze->start_y = 1;
    maze->end_x = width - 2;
    maze->end_y = height - 2;
    maze->map[maze->start_y][maze->start_x] = START;
    maze->map[maze->end_y][maze->end_x] = END;

    // Add diamonds and traps
    add_diamonds(maze);
    add_traps(maze);

    return maze;
}


```
![[截屏0006-05-07 01.28.44 1.png]]
1. The programme generates multiple maze designs, the programme does not generate only valid mazes, traps appear next to the entrances and do not satisfy the pathways.
2. The programme performs fine at larger sizes (100 * 100), it generates properly but takes some time, the main reason is that using random to generate random paths is slower to compute at larger sizes.
3. The programme did not consider removing the traps and lacked a method to remove them.
4. Next time, should design a way to remove the traps at the entrance or exit.


### Iteration-2:
```
/**
 * @brief Generate a maze
 *
 * @param width width of the maze
 * @param height height of the maze
 * @return Maze* pointer to the generated maze structure
 */
Maze* generate_maze(int width, int height) {
    Maze *maze = (Maze*)malloc(sizeof(Maze));
    if (maze == NULL) {
        return NULL;
    }

    // Allocate memory for the map
    maze->map = (char**)malloc(height * sizeof(char*));
    if (maze->map == NULL) {
        free(maze);
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        maze->map[i] = (char*)malloc(width * sizeof(char));
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

    // Generate paths
    for (int i = 1; i < height - 1; i += 2) {
        for (int j = 1; j < width - 1; j += 2) {
            maze->map[i][j] = PATH;
        }
    }

    // Add horizontal paths
    for (int i = 1; i < height - 1; i += 2) {
        for (int j = 2; j < width - 1; j += 2) {
            if (rand() % 2 == 0) {
                maze->map[i][j] = PATH;
            }
        }
    }

    // Add vertical paths
    for (int i = 2; i < height - 1; i += 2) {
        for (int j = 1; j < width - 1; j += 2) {
            if (rand() % 2 == 0) {
                maze->map[i][j] = PATH;
            }
        }
    }

    // Set start and end points
    maze->width = width;
    maze->height = height;
    maze->start_x = 1;
    maze->start_y = 1;
    maze->end_x = width - 2;
    maze->end_y = height - 2;
    maze->map[maze->start_y][maze->start_x] = START;
    maze->map[maze->end_y][maze->end_x] = END;

    // Add diamonds and traps
    add_diamonds(maze);
    add_traps(maze);

    // Remove traps near the start point
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (maze->map[maze->start_y + i][maze->start_x + j] == TRAP) {
                maze->map[maze->start_y + i][maze->start_x + j] = PATH;
            }
        }
    }

    // Remove traps near the end point
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (maze->map[maze->end_y + i][maze->end_x + j] == TRAP) {
                maze->map[maze->end_y + i][maze->end_x + j] = PATH;
            }
        }
    }

    // Add empty spaces at the beginning and end of each row
    for (int i = 0; i < height; i++) {
        maze->map[i][0] = PATH;
        maze->map[i][width - 1] = PATH;
    }

    // Add empty spaces at the beginning and end of each column
    for (int j = 0; j < width; j++) {
        maze->map[0][j] = PATH;
        maze->map[height - 1][j] = PATH;
    }

    // Print the maze
    printf("Generated maze:\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", maze->map[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return maze;
}


```
![[截屏0006-05-07 01.51.01.png]]
1. Generated multiple maze designs, didn't generate only valid mazes, the walls in the first and last rows were removed.
2. The program performs fine at larger sizes (100 * 100), it generates properly but takes some time, the main reason is that using random to generate random paths is slower to compute at larger sizes.
3. The programme does not allow spaces to be randomly generated, resulting in each generated maze having spaces in the first and last rows.
4. Next time a random space generation method should be created to ensure that the mazes have enough curves.

### Iteration-3:
```
/**  
 * @brief Generate a maze 
 * 
 * @param width width of the maze 
 * @param height height of the maze 
 * @return Maze* pointer to the generated maze structure */
 Maze* generate_maze(int width, int height) {  
    Maze *maze = (Maze*)malloc(sizeof(Maze));  
    if (maze == NULL) {  
        return NULL;  
    }  
  
    // Allocate memory for the map  
    maze->map = (char**)malloc(height * sizeof(char*));  
    if (maze->map == NULL) {  
        free(maze);  
        return NULL;  
    }  
    for (int i = 0; i < height; i++) {  
        maze->map[i] = (char*)malloc(width * sizeof(char));  
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
  
    // Generate paths  
    for (int i = 1; i < height - 1; i += 2) {  
        for (int j = 1; j < width - 1; j += 2) {  
            maze->map[i][j] = PATH;  
        }  
    }  
  
    // Add horizontal paths  
    for (int i = 1; i < height - 1; i += 2) {  
        for (int j = 2; j < width - 1; j += 2) {  
            if (rand() % 2 == 0) {  
                maze->map[i][j] = PATH;  
            }  
        }  
    }  
  
    // Add vertical paths  
    for (int i = 2; i < height - 1; i += 2) {  
        for (int j = 1; j < width - 1; j += 2) {  
            if (rand() % 2 == 0) {  
                maze->map[i][j] = PATH;  
            }  
        }  
    }  
  
    // Set start and end points  
    maze->width = width;  
    maze->height = height;  
    maze->start_x = 1;  
    maze->start_y = 1;  
    maze->end_x = width - 2;  
    maze->end_y = height - 2;  
    maze->map[maze->start_y][maze->start_x] = START;  
    maze->map[maze->end_y][maze->end_x] = END;  
  
    // Add diamonds and traps  
    add_diamonds(maze);  
    add_traps(maze);  
  
    // Remove traps near the start point  
    for (int i = -1; i <= 1; i++) {  
        for (int j = -1; j <= 1; j++) {  
            if (maze->map[maze->start_y + i][maze->start_x + j] == TRAP) {  
                maze->map[maze->start_y + i][maze->start_x + j] = PATH;  
            }  
        }  
    }  
  
    // Remove traps near the end point  
    for (int i = -1; i <= 1; i++) {  
        for (int j = -1; j <= 1; j++) {  
            if (maze->map[maze->end_y + i][maze->end_x + j] == TRAP) {  
                maze->map[maze->end_y + i][maze->end_x + j] = PATH;  
            }  
        }  
    }  
  
    // Randomly place empty spaces  
    for (int i = 1; i < height - 1; i++) {  
        for (int j = 1; j < width - 1; j++) {  
            if (maze->map[i][j] == WALL && rand() % 100 < 40) {  
                maze->map[i][j] = PATH;  
            }  
        }  
    }  
  
    return maze;  
}
```
![[截屏0006-05-07 02.13.42.png]]
1. The programme generates a variety of maze designs, but not every one of them is a valid maze.
2. The programme performed fine at larger sizes (100 * 100), generating properly but taking some time, mainly due to the use of random to randomly generate paths that are slower to compute at larger sizes.
3. The programme was not able to generate a valid maze every time.
4. Next time, should use depth-first search (DFS) to generate mazes, to ensure that each generated maze is connected and can reach the end from the start point. At the same time, diamonds and traps will be placed on the passable paths of the maze and will not be placed near the starting point.

### Iteration-4:
```
/**  
 * @brief Generate a maze * 
 * @param width width of the maze 
 * @param height height of the maze 
 * @return Maze* pointer to the generated maze structure 
 */
Maze* generate_maze(int width, int height) {  
    Maze *maze = (Maze*)malloc(sizeof(Maze));  
    if (maze == NULL) {  
        return NULL;  
    }  
  
    // Allocate memory for the map  
    maze->map = (char**)malloc(height * sizeof(char*));  
    if (maze->map == NULL) {  
        free(maze);  
        return NULL;  
    }  
    for (int i = 0; i < height; i++) {  
        maze->map[i] = (char*)malloc(width * sizeof(char));  
        if (maze->map[i] == NULL) {  
            for (int j = 0; j < i; j++) {  
                free(maze->map[j]);  
            }  
            free(maze->map);  
            free(maze);  
            return NULL;  
        }  
    }  
  
    // Allocate memory for visited array  
    int **visited = (int**)malloc(height * sizeof(int*));  
    if (visited == NULL) {  
        free(maze->map);  
        free(maze);  
        return NULL;  
    }  
    for (int i = 0; i < height; i++) {  
        visited[i] = (int*)calloc(width, sizeof(int));  
        if (visited[i] == NULL) {  
            for (int j = 0; j < i; j++) {  
                free(visited[j]);  
            }  
            free(visited);  
            for (int j = 0; j < height; j++) {  
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
  
    // Set start and end points  
    maze->width = width;  
    maze->height = height;  
    maze->start_x = 1;  
    maze->start_y = 1;  
    maze->end_x = width - 2;  
    maze->end_y = height - 2;  
  
    // Call DFS algorithm to generate maze  
    dfs(maze, maze->start_x, maze->start_y);  
  
    // Check if a path exists between start and end points  
    if (!is_path_exist(maze, maze->start_x, maze->start_y, maze->end_x, maze->end_y, visited)) {  
        reset_maze(maze);  
        free(visited);  
        return generate_maze(width, height);  
    }  
  
    // Free visited array  
    for (int i = 0; i < height; i++) {  
        free(visited[i]);  
    }  
    free(visited);  
  
    maze->map[maze->start_y][maze->start_x] = START;  
    maze->map[maze->end_y][maze->end_x] = END;  
  
    // Add diamonds and traps  
    add_diamonds(maze);  
    add_traps(maze);  
  
    return maze;  
}
```
![[截屏0006-05-07 15.46.50.png]]
1. The program generates multiple maze designs, and the is_path_exist method confirms access to ensure that only valid mazes are generated.
2. The programme suffers from segmentation fault memory overflow at larger sizes (100 * 100), and it is the DFS algorithm that suffers from stack overflow.
3. The programme DFS algorithm suffers from a stack overflow problem that prevents the generation of larger size mazes.
4. Next time should use the Iterative Depth-First Search (IDFS) algorithm, which is better suited to handle stack overflow problems than Recursive Depth-First Search (DFS) because it reduces memory consumption by limiting the depth of each search.

### Iteration-5:
```
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
```
![[截屏0006-05-07 23.17.32.png]]
1. The programme generates a variety of maze designs, most of which generate valid mazes.
2. The programme is able to generate valid mazes at larger sizes (100 * 100), using the IDFS method, which searches a fixed depth of nodes at a time, and then increases the depth of the search after all nodes at the current depth have been searched until the target node has been found, or until the entire graph has been searched.

# How your plan works
1. **Generate the maze:**
    - Generate the maze using the Iterative Depth-First Search (IDFS) algorithm to ensure that a path exists in the maze from the start point to the end point.
    - Randomly select a start point and an end point in the maze and make sure a path exists between them.
2. **Add diamonds and traps:**
    - Add diamonds and traps to the maze, making sure they are not located at the start point, end point, or path.
    - Make sure there are the right amount of diamonds and traps in the maze.
3. **Saving a maze:**
    - Saves the generated maze to the specified file.
4. **Generate maze with start point, end point, diamonds and traps:**
    - Accepts user input for maze width and height.
    - Generates the maze and saves the maze to the specified file.
### How you iterated your development
### Iteration 1: Basic maze generation
1. **Define the maze structure and basic symbols:**
    - Define the representation of the maze, including the structure of the maze as well as the symbols that may appear in the maze (walls, paths, start points, end points, etc.).
2. **Implement the basic maze generation algorithm:**
    - Generate the basic maze structure using an iterative depth-first search (IDFS) algorithm that ensures that a path exists in the maze that connects the start and end points.
3. **Generate simple mazes:**
    - Write code to generate a simple maze and ensure that the maze contains a start point, an end point, and a path that connects them.
### Iteration 2: Adding Diamonds and Traps
1. **Define the notation for diamonds and traps:**
    - Determine the representation of diamonds and traps and define their symbols.
2. **Implement the addition of diamonds and traps:**
    - Write code to randomly add a certain number of diamonds and traps to the maze, making sure they are not located at the start, end, or path.
3. **Generate maze with diamonds and traps:**
    - Modify the maze generation algorithm to add diamonds and traps to the maze, making sure they do not affect the solvability of the maze.
### Iteration 3: Saving the maze to a file
1. **Implement the maze saving function:**
    - Write code to save the generated maze to a file for subsequent use.
2. **Generate the maze and save it to a file: **
    - Call the maze generation function and the save function to generate the maze and save it to a file, checking that the contents of the file are correct.
### Iteration 4: Code Optimisation and Improvement
1. ** Optimise the maze generation algorithm:**
    - Optimise the maze generation algorithm to minimise the time required to generate the maze.
2. **Optimise the diamond and trap adding algorithm:**
    - Optimise the algorithm for adding diamonds and traps to improve the efficiency and performance of generating mazes.
3. **Optimising the code structure:**
    - Modularise the code to improve readability and maintainability, add comments and function descriptions.
4. **Testing and debugging:**
    - Conduct thorough testing and debugging of the code to ensure that every function works properly and fix any bugs that may exist.
# Limitations of the solution
1. **Maze generation algorithm: **
    - The Iterative Depth-First Search (IDFS) algorithm used in the code to generate the mazes is an effective method, but sometimes it may generate mazes with "dead ends", i.e., you cannot get to the end of the maze from the beginning.
    - When the size of the maze is large, the performance of the IDFS algorithm may be affected, and it may take a long time for the generated maze to ensure that a pathway exists.
2. **Distribution of diamonds and traps:**
    - In the current implementation, the number of diamonds and traps is determined based on the total size of the maze, but this approach may lead to an uneven distribution of diamonds and traps in the maze.
    - Also, the distribution of diamonds and traps is affected by the paths, and may appear to be concentrated around the paths.
3. **Code structure:**
    - The current code structure is relatively simple, with all functions implemented in the `main` function, which is not conducive to code maintenance and expansion.
    - The lack of comments and function descriptions makes the code difficult to read and understand.
# Any future improvements
1. **Improvements to the maze generation algorithm:**
    - Consider using other more efficient maze generation algorithms such as Prim's algorithm, Kruskal's algorithm, or recursive partitioning that generate mazes more efficiently and ensure that each maze has a solution.
2. **Improvement in the distribution of diamonds and traps:**
    - Consider designing more reasonable algorithms to distribute diamonds and traps. Consider determining the positions of diamonds and traps based on the structure of the maze and the distribution of paths to ensure that they are evenly distributed and do not affect the solvability of the maze.
3. **Improvement of the code structure:**
    - Modularise the code by encapsulating the functions of generating mazes, adding diamonds and traps, and saving mazes into functions respectively to improve the readability and maintainability of the code.
    - Add comments and function descriptions to make the code easier to understand and maintain.
4. **Performance optimisation:**
    - Optimise the maze generation algorithm to minimise the time needed to generate the maze.
    - Optimise the algorithm for adding diamonds and traps to improve the efficiency and performance of generating mazes.
