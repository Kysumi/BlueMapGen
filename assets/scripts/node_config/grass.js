import {getNodesOfType} from "../node.js";
import {randomTexture} from "./configs.js";
import dirtTextures from "tiles_textures/dirt.js";
import waterTextures from "tiles_textures/river.js";

export const processGrass = (node, grid) => {
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
        node.setTexture(randomTexture(dirtTextures))
        node.setBiome("dirt")
    }
    console.log('Grass!!')
}