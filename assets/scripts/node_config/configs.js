import {processGrass, textureFiles as grassFiles} from "./grass.js";
import {processDirt, textureFiles as dirtFiles} from "./dirt.js";
import {loadTextures} from "../assetLoader.js";

export const loadAllTextures = () => {
    loadTextures(grassFiles);
    loadTextures(dirtFiles);
}

export default {
    "grass": processGrass,
    "dirt": processDirt
}