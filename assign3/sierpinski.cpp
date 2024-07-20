/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * This function recursive draws a Sierpinski triangle on the provided GWindow.
 * A Sierpinski triangle of order 0 is a single filled black triangle. For higher
 * orders, the function draws three smaller Sierpinski triangles inside the current
 * triangle.
 *
 * @param window The graphical window where the triangles will be drawn
 * @param one The first corner point of the triangle
 * @param two The second corner point of the triangle
 * @param three The third corner point of the triangle
 * @param order The order of the Sierpinski triangle to draw
 * @return The total count of black triangles drawn
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    int count = 0;
    if (order == 0) {
        // draw one black triangle of order 0
        fillBlackTriangle(window, one, two, three);
        count++;
    } else {
        // draw three triangers inside that are one order smaller than the current one
        count += drawSierpinskiTriangle(window, one, GPoint((one.x + two.x) / 2, (one.y + two.y) / 2),
                               GPoint((one.x + three.x) / 2, (one.y + three.y) / 2), order - 1);
        count += drawSierpinskiTriangle(window, two, GPoint((two.x + one.x) / 2, (two.y + one.y) / 2),
                               GPoint((two.x + three.x) / 2, (two.y + three.y) / 2), order - 1);
        count += drawSierpinskiTriangle(window, three, GPoint((three.x + two.x) / 2, (three.y + two.y) / 2),
                               GPoint((one.x + three.x) / 2, (one.y + three.y) / 2), order - 1);
    }

    return count;
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

