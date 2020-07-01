// YOU MUST DEFINE THE FILE EXTENSION
// import demoDefault, {heh} from "module.js"

let grid = new Grid(24,24);
const gridSize = grid.getSize();

const proccess = () => {
    const xSize = gridSize.x();
    const ySize = gridSize.y();

    for (let x = 0; x < xSize; x++) {
        for (let y = 0; y < ySize; y++) {
            let node = grid.getNodeFromGridPosition(x,y);

        }
    }
}

while(Window.isOpen()) {

    // Window.processEvents();
    console.log(grid.getSize().x())
    Window.clear();

    proccess()

    // node.draw();

    // grid.process();
    grid.draw();

    Window.display();

}
