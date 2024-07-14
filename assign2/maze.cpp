#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

const int directions[4][2] = {
    {-1, 0},	// up
    {1, 0},		// down
    {0, -1},	// left
    {0, 1}		// right
};

/*
 * Get all valid movement coordinates in the maze at the current coordinates
 * @param maze Grid of bool representing the maze, where true indicates there is no walls
 * 				and false indicates there is a wall
 * @param cur Current coordinate
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;

    // Go through all four directions
    for (const auto& dir : directions) {
        int newY = cur.row + dir[0];
        int newX = cur.col + dir[1];
        if (isValidPos(maze, newX, newY))
            neighbors.add(GridLocation(newY, newX));
    }
    return neighbors;
}

/*
 * Validate a maze path solution, which must be end-at-exit, begin-at-entry, not-through-wall, 
 * no-teleport, no-revisit
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntry = {0, 0};
    GridLocation cur, pre;
    Set<GridLocation> pathSet;
    int stackSize = path.size();

    if (path.isEmpty()) {
        error("Path is empty");
    } else if ((cur = path.pop()) != mazeExit) {
        error("Path does not end at maze exit");
    }
    pathSet.add(cur);

    while (!path.isEmpty()) {
        // keep the previous location for validation
        pre = cur;
        cur = path.pop();
        pathSet.add(cur);
        if (!maze.get(cur.row, cur.col)) {
            error("Path go through walls");
        }
        if (cur.row != pre.row && cur.col != pre.col) {
            error("Path has teleports");
        }
    }

    // Now 'cur' must be the entry location
    if (cur != mazeEntry) {
        error("Path does not begin at maze entry");
    }

    // There cannot be duplicate location in the path
    if (pathSet.size() < stackSize) {
        error("Path has revisit");
    }
}

/*
 * Generate a feasible solution to the maze.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    // MazeGraphics::drawGrid(maze);
    GridLocation mazeEntry = {0, 0};
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation cur = mazeEntry;
    Stack<GridLocation> path;
    Set<GridLocation> visited, nextMoves;
    path.push(mazeEntry);
    visited.add(mazeEntry);

    /*
     * generate next feasible movement and try one by one
     */
    while (!path.isEmpty()) {
        cur = path.peek();
        if (cur == mazeExit) {
            // we have reached the maze exit
            return path;
        }
        nextMoves = generateValidMoves(maze, cur);

        // remove duplicates between 'nextMoves' and 'visited' from 'nextMoves'
        nextMoves -= visited;

        // step forward
        if (!nextMoves.isEmpty()) {
            // simply select the first viable move
            cur = nextMoves.first();
            path.push(cur);
            visited.add(cur);
        } else {
            // if there is no viable move, withdraw one step
            path.pop();
        }
    }

    // there is no solution to this maze, return an empty set
    error("An unsolved maze is passed in");
    return Stack<GridLocation>();
}


/*
 * Check if a location is accessible int this maze
 */
bool isValidPos(const Grid<bool>& maze, int x, int y) {
    int m_height = maze.numRows();
    int m_length = maze.numCols();
    if (y >= 0 && y < m_height && x >= 0 && x < m_length && maze.get(y, x)) {
        return true;
    }
    return false;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    lines = readLines(in);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
