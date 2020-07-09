// YOU MUST DEFINE THE FILE EXTENSION
// import demoDefault, {heh} from "module.js"

let grid = new Grid(30,30);
const gridSize = grid.getSize();

const getAliveNodes = (nodes) => {
    let aliveNodes = [];

    nodes.forEach((node) => {
        if (node.alive()) {
            aliveNodes.push(node);
        }
    })

    return aliveNodes;
}

const proccessNodes = () => {
    const xSize = gridSize.x();
    const ySize = gridSize.y();

    for (let x = 0; x < xSize; x++) {
        for (let y = 0; y < ySize; y++) {
            let node = grid.getNodeFromGridPosition(x,y);
            let neighbours = grid.getNeighbours(x,y);

            let alive = getAliveNodes(neighbours);

            if (alive.length < 2 || alive.length > 3) {
                node.kill();
                console.log('KILL!')
            } else {
                console.log('BORN')
                node.born();
            }
        }
    }
}

while(Window.isOpen()) {

    Window.processEvents();
    Window.clear();

    proccessNodes();

    // We need to do this
    grid.flipBuffer();
    grid.draw();


    Window.display();

}
