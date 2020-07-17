// Handles node operations
import config from "./node_config/configs.js"

const getNodesOfType = (nodes, type) => {
    return nodes.map((node) => {
       if (node.getBiome() === type) {
           return node;
       }
    });
}

export const defaultNodeCallback = (x, y, grid) => {
    let node = grid.getNodeFromGridPosition(x, y);
    let neighbours = grid.getNeighbours(x, y);

    const neighboursOfType = getNodesOfType(neighbours, node.getBiome())

    config[node.getBiome()]()

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