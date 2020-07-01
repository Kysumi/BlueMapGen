// YOU MUST DEFINE THE FILE EXTENSION
// import demoDefault, {heh} from "module.js"

let grid = new Grid(20,20);



const proccess = () => {

    let x = 0;
    let y = 0;

    let node = grid.getNodeFromGridPosition(x,y);
}

while(Window.isOpen()) {

    // Window.processEvents();
    console.log(grid.getSize().x())
    Window.clear();

    // node.draw();

    // grid.process();
    grid.draw();

    Window.display();

}
