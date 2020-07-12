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
    let aliveNodesCount = getAliveNodesCount(neighbours);

    // if (aliveNodesCount < 2 || aliveNodesCount > 3) {
    //     node.kill();
    // } else {
    //     node.born();
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