import {drawGrid, initialize} from "./grid.js";
import {defaultNodeCallback, iterateNodes} from "./node.js";
import {loadTextures} from "./assetLoader.js";
import {textures} from "./node_config/tiles_textures/index.js"

loadTextures(textures);

let grid = new Grid(10, 10);
initialize(grid)

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
