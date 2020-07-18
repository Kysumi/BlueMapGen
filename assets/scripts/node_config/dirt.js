import {getNodesOfType} from "../node.js";
import {randomTexture} from "configs.js";
import grassTextures from "tiles_textures/grass.js";
import waterTextures from "tiles_textures/river.js";

export const processDirt = (node, grid) => {
    let neighbours = grid.getNeighbours(node.x(), node.y())

    const neighboursOfType = getNodesOfType(neighbours, node.getBiome())
    const typeCount = neighboursOfType.length

    if(typeCount < 2)
    {
        node.setTexture(randomTexture(waterTextures))
        node.setBiome("river")
    }
    else if(typeCount > 3)
    {
        node.setTexture(randomTexture(grassTextures))
        node.setBiome("grass")
    }

    console.log('dirt was called yo')
}