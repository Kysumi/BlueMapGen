// YOU MUST DEFINE THE FILE EXTENSION
// import demoDefault, {heh} from "module.js"

let grid = new Grid(100,100);
const gridSize = grid.getSize();

const proccess = () => {
    const xSize = gridSize.x();
    const ySize = gridSize.y();

    for (let x = 0; x < xSize; x++) {
        for (let y = 0; y < ySize; y++) {
            let node = grid.getNodeFromGridPosition(x,y);
            node.draw();
        }
    }
}

while(Window.isOpen()) {

    Window.processEvents();
    Window.clear();

    grid.process();
    grid.draw();

    Window.display();

}
