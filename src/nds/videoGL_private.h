#ifndef VIDEOGL_PRIVATE_H
#define VIDEOGL_PRIVATE_H
// the following structures were made private in blocksds's libnds fork
// grabbed from commit b4a1caf4f09a62bb9ae11f18521aa5a3f5d75665, source/arm9/video/videoGL.c

// Structures specific to allocating and deallocating texture and palette VRAM
// ---------------------------------------------------------------------------

typedef struct s_SingleBlock
{
    uint32_t indexOut;
    uint8_t *AddrSet;

    // 0-1: prev/next memory block
    // 2-3: prev/next empty/alloc block
    struct s_SingleBlock *node[4];

    uint32_t blockSize;
} s_SingleBlock;

typedef struct s_vramBlock
{
    uint8_t *startAddr, *endAddr;
    struct s_SingleBlock *firstBlock;
    struct s_SingleBlock *firstEmpty;
    struct s_SingleBlock *firstAlloc;

    struct s_SingleBlock *lastExamined;
    uint8_t *lastExaminedAddr;
    uint32_t lastExaminedSize;

    DynamicArray blockPtrs;
    DynamicArray deallocBlocks;

    uint32_t blockCount;
    uint32_t deallocCount;
} s_vramBlock;

typedef struct gl_texture_data
{
    void *vramAddr;       // Address to the texture loaded into VRAM
    uint32_t texIndex;    // The index in the Memory Block
    uint32_t texIndexExt; // The secondary index in the memory block (for GL_COMPRESSED)
    int palIndex;         // The palette index
    uint32_t texFormat;   // Specifications of how the texture is displayed
    uint32_t texSize;     // The size (in blocks) of the texture
} gl_texture_data;

typedef struct gl_palette_data
{
    void *vramAddr;         // Address to the palette loaded into VRAM
    uint32_t palIndex;      // The index in the memory block
    uint16_t addr;          // The offset address for texture palettes in VRAM
    uint16_t palSize;       // The length of the palette
    uint32_t connectCount;  // The number of textures currently using this palette
} gl_palette_data;

// This struct holds hidden globals for videoGL. It is initialized by glInit().
typedef struct gl_hidden_globals
{
    // VRAM blocks management
    // ----------------------

    s_vramBlock *vramBlocksTex; // One for textures
    s_vramBlock *vramBlocksPal; // One for palettes
    int vramLockTex; // Holds the current lock state of the VRAM banks
    int vramLockPal; // Holds the current lock state of the VRAM banks

    // Texture/palette manamenent
    // --------------------------

    // Arrays of texture and palettes. The index to access a texture is the same
    // as the name of that texture. The value of each array component is a
    // pointer to a texture or palette struct. When a texture/palette is
    // generated, the pointer is allocated. When it is freed, the pointer is set
    // deallocated and set to NULL, and the texture name (the array index) is
    // added to the deallocTex or deallocPal array to be reused when required.
    //
    // Note: Getting the activeTexture or activePalette from the arrays will
    // always succeed. glBindTexure() can only set activeTexture and
    // activePalette to an element that exists.
    DynamicArray texturePtrs;
    DynamicArray palettePtrs;

    // Array of names that have been deleted and are ready to be reused. They
    // are just a list of indices to the arrays texturePtrs and palettePtrs that
    // we can reuse.
    DynamicArray deallocTex;
    DynamicArray deallocPal;

    // Number of names available in the list of reusable names
    uint32_t deallocTexSize;
    uint32_t deallocPalSize;

    // Current number of allocated names. It's also the next name that will be
    // used (if there are no reusable names).
    int texCount;
    int palCount;

    // State not related to dynamic memory management
    // ----------------------------------------------

    int activeTexture; // The current active texture name
    int activePalette; // The current active palette name
    u32 clearColor; // Holds the current state of the clear color register
    GL_MATRIX_MODE_ENUM matrixMode; // Holds the current Matrix Mode

    uint8_t isActive; // Has glInit() been called before?
}
gl_hidden_globals;

// This is the actual data of the globals for videoGL.
extern gl_hidden_globals glGlob;

// those are also private in devkitpro's libnds
extern u8* vramBlock_getAddr( s_vramBlock *mb, u32 index );
extern u32 vramBlock_allocateBlock( s_vramBlock *mb, u32 size, u8 align );
extern u32 vramBlock_deallocateBlock( s_vramBlock *mb, u32 index );

#endif // VIDEOGL_PRIVATE_H
