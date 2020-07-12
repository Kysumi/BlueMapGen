import {drawGrid, initialize} from "./grid.js";
import {defaultNodeCallback, iterateNodes} from "./node.js";
import {loadAllTextures} from "./node_config/configs.js";

let grid = new Grid(10, 10);
initialize(grid)

loadAllTextures();

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
