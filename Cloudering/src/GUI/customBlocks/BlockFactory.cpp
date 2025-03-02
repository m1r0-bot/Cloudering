#include "BlockFactory.h"
#include "Block.h"
#include "Camera_Block.h"
#include "Shader_Block.h"


std::unique_ptr<Block> BlockFactory::create(EnumBlockType blockType) {
    switch (blockType) {
    case CAMERA: return std::make_unique<Camera_Block>();
    case SHADER: return std::make_unique<Shader_Block>();
    default: return nullptr;
    }
}