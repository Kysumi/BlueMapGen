import {drawGrid} from "./grid.js";
import {defaultNodeCallback, iterateNodes} from "./node.js";
import {loadAllTextures} from "./node_config/configs.js";

let grid = new Grid(24, 24);

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
