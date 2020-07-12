
export const loadTextures = (textures) => {
    textures.forEach((texture) => {
        console.log(texture.filePath, texture.name);
        TextureManager.loadFromFile(texture.filePath, texture.name);
    })
}

export const textureOptions = (textureFiles) => {
    return textureFiles.map((details) => {
        return details.name;
    })
}