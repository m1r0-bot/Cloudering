#pragma once
#include <memory>
#include "Block.h"

class BlockFactory {
public:
	static std::unique_ptr<Block> create(EnumBlockType scenarioID);
};