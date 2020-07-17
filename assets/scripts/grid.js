import {iterateNodes} from "./node.js";
import {randomTexture} from "./node_config/configs.js";
import dirtTextures from "node_config/tiles_textures/dirt.js";
import grassTextures from "node_config/tiles_textures/grass.js";
import waterTextures from "node_config/tiles_textures/river.js";

export const drawGrid = (grid) => {
    grid.flipBuffer();
    grid.draw();
}

const randomize = (x, y, grid) => {
    const num = Setting.getRandomNumber(0,10)
    const node = grid.getNodeFromGridPosition(x, y);

    if (num > 5) {
        node.setTexture(randomTexture(dirtTextures))
    } else if (num < 1) {
        node.setTexture(randomTexture(waterTextures))
    } else {
        node.setTexture(randomTexture(grassTextures))
    }
}

export const initialize = (grid) => {
    iterateNodes(grid, randomize)
}