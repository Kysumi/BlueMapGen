import {processGrass} from "./grass.js";
import {processDirt} from "./dirt.js";
import {processRiver} from "./river.js";

export const randomTexture = (textures) => {
    const num = Setting.getRandomNumber(0, textures.length)
    return textures[num].name
}

export default {
    grass: processGrass,
    dirt: processDirt,
    river: processRiver,
}