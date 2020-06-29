// YOU MUST DEFINE THE FILE EXTENSION
// import demoDefault, {heh} from "module.js"

let grid = new Grid(20,20);

while(Window.isOpen()) {
    Window.processEvents()

    Window.clear();

    grid.process();
    grid.draw();

    Window.display();
}
