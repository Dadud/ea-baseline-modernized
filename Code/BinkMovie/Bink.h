// Bink.h - RAD Game Tools Bink video codec stub
// This is a minimal stub for build purposes only.
// The real Bink SDK is available from https://radvideo.com/bink/
#ifndef _BINK_H_
#define _BINK_H_

// Bink surface formats
#define BINKSURFACE565 0x00000004L
#define BINKSURFACE555 0x00000005L
#define BINKSURFACE8888 0x00000008L

// Bink copy flags
#define BINKCOPYNOSCALING 0x00000001L

// Basic Bink handle type
typedef struct BINKSTRUCT* HBINK;

// Bink video structure (minimal stub)
struct BINKSTRUCT {
    int Width;
    int Height;
    unsigned int Flags;
    unsigned int FrameRate;       // frames per second * FrameRateDiv
    unsigned int FrameRateDiv;    // divisor for FrameRate
    unsigned int FrameNum;        // current frame number
    unsigned int Frames;          // total number of frames
};

// Open a Bink video file
// Returns NULL on failure, valid HBINK on success
static inline HBINK BinkOpen(const char* filename, unsigned int flags) {
    (void)filename;
    (void)flags;
    return NULL; // Stub: always fails (no video)
}

// Close a Bink video
static inline void BinkClose(HBINK bink) {
    (void)bink;
}

// Copy a rectangle from the Bink video to a buffer
static inline void BinkCopyToBuffer(HBINK bink, void* dest, int destwidth, int destheight, int destpitch, int destx, unsigned int flags) {
    (void)bink;
    (void)dest;
    (void)destwidth;
    (void)destheight;
    (void)destpitch;
    (void)destx;
    (void)flags;
}

// Copy a rectangle to another surface (stub)
static inline void BinkCopyToBufferRect(HBINK bink, void* dest, int destwidth, int destheight, int destpitch, int destx, int desty, int srcw, int srch, int srcx, int srcy, unsigned int flags) {
    (void)bink;
    (void)dest;
    (void)destwidth;
    (void)destheight;
    (void)destpitch;
    (void)destx;
    (void)desty;
    (void)srcw;
    (void)srch;
    (void)srcx;
    (void)srcy;
    (void)flags;
}

// Request Bink to use DirectSound for audio
static inline void BinkSoundUseDirectSound(int enable) {
    (void)enable;
}

// Get Bink frame rate (frames per 1000 seconds)
static inline unsigned int BinkGetFrameRate(HBINK bink) {
    (void)bink;
    return 30000;
}

// Get current frame number
static inline unsigned int BinkGetFrame(HBINK bink) {
    (void)bink;
    return 0;
}

// Advance to next frame
static inline void BinkNextFrame(HBINK bink) {
    (void)bink;
}

// Wait for a specific frame
static inline int BinkWait(HBINK bink) {
    (void)bink;
    return 0;
}

// Do frame decoding
static inline void BinkDoFrame(HBINK bink) {
    (void)bink;
}

// Set volume (0-100)
static inline void BinkSetVolume(HBINK bink, int volume) {
    (void)bink;
    (void)volume;
}

// Set pan (0=left, 100=right, 50=center)
static inline void BinkSetPan(HBINK bink, int pan) {
    (void)bink;
    (void)pan;
}

// Get the number of frames
static inline unsigned int BinkGetFrames(HBINK bink) {
    (void)bink;
    return 0;
}

// Check if Bink needs to redraw
static inline int BinkNeedsToDraw(HBINK bink) {
    (void)bink;
    return 0;
}

// Set the frame rate divisor
static inline void BinkSetFrameRateDiv(HBINK bink, unsigned int div) {
    (void)bink;
    (void)div;
}

#endif // _BINK_H_
