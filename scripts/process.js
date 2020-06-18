	// Replicate grid state to copy
	// auto gridCopy = grid;

	// for (auto xAxis = 0; xAxis < size.x; xAxis++) {
	// 	for (auto yAxis = 0; yAxis < size.y; yAxis++) {

	// 		auto node = gridCopy[xAxis][yAxis];
	// 		auto count = getAliveNeighbours(sf::Vector2i(xAxis, yAxis)).size();

	// 		if (count < 2) {
	// 			gridCopy[xAxis][yAxis].kill();
	// 		}
	// 		else if (count > 3) {
	// 			gridCopy[xAxis][yAxis].kill();
	// 		}
	// 		else if (count == 3) {
	// 			gridCopy[xAxis][yAxis].born();
	// 		}
	// 	}
	// }

	// // Flip array back again
    // grid = gridCopy;


var grid = new Grid(20,20);

function mainloop() {

    Window.clear();

    grid.process();
    grid.draw();

    Window.display();
}

// setInterval(mainloop, 0.5);


while(Window.isOpen())
{
    console.log(Window.isOpen())

    Window.processEventQueue();

    mainloop();
}