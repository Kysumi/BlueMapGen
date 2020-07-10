// YOU MUST DEFINE THE FILE EXTENSION
// import demoDefault, {heh} from "module.js"

let grid = new Grid(10,10);
const gridSize = grid.getSize();

const getAliveNodes = (nodes) => {
    let aliveNodes = [];


    // nodes.forEach((node) => {
    //     if (node.alive()) {
    //         aliveNodes.push(node);
    //     }
    // })

    return aliveNodes;
}

const proccessNodes = () => {
    console.log('Processing next iteration');

    const xSize = gridSize.x();
    const ySize = gridSize.y();

    for (let x = 0; x < xSize; x++) {
        for (let y = 0; y < ySize; y++) {
            let node = grid.getNodeFromGridPosition(x,y);
            let neighbours = grid.getNeighbours(x,y);

            // let alive = getAliveNodes(neighbours);

            let alive = [];
            for (let index = 0; index < neighbours.length; ++index) {
                let node = neighbours[index];
                console.log(`Node status  ${node.x()} : ${node.y()} -> alive: ${node.alive()}`);

                if (node.alive()) {
                    alive.push(node);
                }
            }

            if (alive.length < 2 || alive.length > 3) {
                node.kill();
                console.log(`KILL! ${x} : ${y}`)
            } else {
                console.log(`BORN! ${x} : ${y}`)
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
