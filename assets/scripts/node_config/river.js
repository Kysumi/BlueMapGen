import {getNodesOfType} from "../node.js";
import {randomTexture} from "./configs.js";
import dirtTextures from "tiles_textures/dirt.js";

export const processRiver = (node, grid) => {
    let neighbours = grid.getNeighbours(node.x(), node.y())

    const neighboursOfType = getNodesOfType(neighbours, node.getBiome())
    const typeCount = neighboursOfType.length

    if(typeCount > 3)
    {
        node.setTexture(randomTexture(dirtTextures))
        node.setBiome("dirt")
    }

    console.log('Heh river go brrrr')
}