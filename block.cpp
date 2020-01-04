#include "block.hpp"

Block::Block(BlockType t, string n) {
  type = t;
  name = n;
}

Block::Block() {
  //
}

void Block::draw(int screenX, int screenY, int scale) {
  drawStillFrame(anim, screenX, screenY, 0, false);
}




TreeBlock::TreeBlock()
      : Block(BlockType::tree, "tree") {
  age = TreeAge::young;
}

StoneBlock::StoneBlock()
      : Block(BlockType::stone, "stone") {
}

WaterBlock::WaterBlock()
      : Block(BlockType::water, "water") {
}

CoalBlock::CoalBlock()
      : Block(BlockType::coal, "coal") {
}

CopperBlock::CopperBlock()
      : Block(BlockType::copper, "copper") {
}

AirBlock::AirBlock()
      : Block(BlockType::air, "air") {
}

SandBlock::SandBlock()
      : Block(BlockType::sand, "sand") {
}

ZincBlock::ZincBlock()
      : Block(BlockType::zinc, "zinc") {
}

IronBlock::IronBlock()
      : Block(BlockType::iron, "iron") {
}

NickelBlock::NickelBlock()
      : Block(BlockType::nickel, "nickel") {
}

SiliconBlock::SiliconBlock()
      : Block(BlockType::silicon, "silicon") {
}

UnknownBlock::UnknownBlock()
      : Block(BlockType::unknown, "unknown") {
}

Block *newblockFromType(BlockType type) {
  switch(type) {
    case BlockType::stone:
      return new StoneBlock();
    case BlockType::water:
      return new WaterBlock();
    case BlockType::coal:
      return new CoalBlock();
    case BlockType::copper:
      return new CopperBlock();
    case BlockType::air:
      return new AirBlock();
    case BlockType::sand:
      return new SandBlock();
    case BlockType::tree:
      return new TreeBlock();
    case BlockType::zinc:
      return new ZincBlock();
    case BlockType::iron:
      return new IronBlock();
    case BlockType::nickel:
      return new NickelBlock();
    case BlockType::silicon:
      return new SiliconBlock();
    case BlockType::unknown:
      return new UnknownBlock();
    default:
      cout << "Truly unknown block being created" << endl;
      exit(0);
      break;
  }
}