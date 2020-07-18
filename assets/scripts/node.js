// Handles node operations
import config from "./node_config/configs.js"

export const getNodesOfType = (nodes, type) => {
    return nodes.map((node) => {
       if (node.getBiome() === type) {
           return node;
       }
    });
}

export const defaultNodeCallback = (x, y, grid) => {
    let node = grid.getNodeFromGridPosition(x, y);
    config[node.getBiome()](node, grid)
}

export const iterateNodes = (grid, callback) => {

    const gridSize = grid.getSize();
    const xSize = gridSize.x();
    const ySize = gridSize.y();

    for (let x = 0; x < xSize; x++) {
        for (let y = 0; y < ySize; y++) {
            callback(x, y, grid)
        }
    }
}

/// Game of life conditions

// if(aliveNodesCount < 2)
// {
//     node.kill()
// }
// else if(aliveNodesCount == 3)
// {
//     node.born()
// }
// else if(aliveNodesCount > 3)
// {
//     node.kill()
// }