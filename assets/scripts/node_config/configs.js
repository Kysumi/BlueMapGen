import {processGrass} from "./grass.js";
import {processDirt} from "./dirt.js";
import {loadTextures} from "../assetLoader.js";

export const loadAllTextures = () => {
    loadTextures(textures);
}

export const randomTexture = (textures) => {
    const num = Setting.getRandomNumber(0, textures.length)
    return textures[num].name
}

export default {
    grass: processGrass,
    dirt: processDirt,
}