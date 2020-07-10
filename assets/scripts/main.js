import {drawGrid} from "./grid.js";
import {defaultNodeCallback, iterateNodes} from "./node.js";

let grid = new Grid(100, 100);

const proccessNodes = () => {
    iterateNodes(grid, defaultNodeCallback)
}

while (Window.isOpen()) {

    Window.processEvents();
    Window.clear();

    proccessNodes();

    drawGrid(grid)

    Window.display();
}
