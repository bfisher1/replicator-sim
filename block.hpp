#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include "anim.hpp"
#include <time.h>
#include <iostream>

using namespace std;

enum BlockType {
  stone,
  water,
  coal,
  copper,
  air,
  sand,
  tree,
  zinc,
  iron,
  nickel,
  silicon,
  bedrock,
  unknown
};

class Block {
  public:
    BlockType type;
    string name;
    Anim *anim;
    Block(BlockType type, string name);
    Block();
    //todo crossable should be shared by all blocks of a class
    bool crossable;
    void draw(int screenX, int screenY, int scale);
    virtual void replacementOf(Block *block);
};

// block that is on top of another block
class OverlyingBlock : public Block {
  public:
    Block *below = NULL;
    void replacementOf(Block *block) override;
    OverlyingBlock(BlockType type, string name);
};

enum TreeAge {
  young,
  middle,
  adult,
  charred,
  old,
  stump
};

class TreeBlock : public OverlyingBlock {
  public:
    TreeAge age;
    time_t lastAgeUpdate;
    void ageTree();
    TreeBlock();
    void drawTree(int screenX, int screenY, int belowY, int scale);
};

class StoneBlock : public Block {
  public:
    StoneBlock();
};
class WaterBlock : public Block {
  public:
    WaterBlock();
};
class CoalBlock : public Block {
  public:
    CoalBlock();
};
class CopperBlock : public Block {
  public:
    CopperBlock();
};
class AirBlock : public Block {
  public:
    AirBlock();
};
class SandBlock : public Block {
  public:
    SandBlock();
};
class ZincBlock : public Block {
  public:
    ZincBlock();
};
class IronBlock : public Block {
  public:
    IronBlock();
};
class NickelBlock : public Block {
  public:
    NickelBlock();
};
class SiliconBlock : public Block {
  public:
    SiliconBlock();
};

class BedrockBlock : public Block {
  public:
    BedrockBlock();
};

class UnknownBlock : public Block {
  public:
    UnknownBlock();
};



Block *newblockFromType(BlockType type);


#endif