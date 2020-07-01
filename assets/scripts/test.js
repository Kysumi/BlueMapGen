// YOU MUST DEFINE THE FILE EXTENSION
// import demoDefault, {heh} from "module.js"

let grid = new Grid(20,20);

Window.describe()

let x = 0;
let y = 0;

while(Window.isOpen()) {

    let node = grid.getNodeFromGridPosition(x,y);
    node.draw();

    Window.processEvents();

    Window.clear();

    grid.process();
    // grid.draw();

    Window.display();

    x++;
    y++;

    if (x > 10 || y > 10) {
        x = 0;
        y = 0;
    }
}
