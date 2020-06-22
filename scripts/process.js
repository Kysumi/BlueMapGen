// import {temp} from './export.js'

var grid = new Grid(62,62);

function mainloop() {

    Window.clear();

    grid.process();
    grid.draw();

    Window.display();
}

while (Window.isOpen())
{
    console.log(Window.isOpen())

    Window.processEventQueue();

    mainloop();

    // temp();
}