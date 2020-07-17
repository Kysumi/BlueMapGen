// Handles node operations

const getAliveNodesCount = (nodes) => {
    let aliveNodes = [];

    nodes.forEach((node) => {
        if (node.alive()) {
            aliveNodes.push(node);
        }
    })

    return aliveNodes.length;
}

export const defaultNodeCallback = (x, y, grid) => {
    let node = grid.getNodeFromGridPosition(x, y);
    let neighbours = grid.getNeighbours(x, y);

    const biome = node.getBiome();
    console.log(biome)

    const process = biome

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