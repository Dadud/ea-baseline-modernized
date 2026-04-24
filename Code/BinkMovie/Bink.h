// Bink.h - RAD Game Tools Bink video codec stub
// This is a minimal stub for build purposes only.
// The real Bink SDK is available from https://radvideo.com/bink/
#ifndef _BINK_H_
#define _BINK_H_

// Basic Bink handle type
typedef struct BINKSTRUCT* HBINK;

// Bink video structure (minimal stub)
struct BINKSTRUCT {
    int Width;
    int Height;
    unsigned int Flags;
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
static inline void BinkCopyToBuffer(HBINK bink, void* dest, int destwidth, int destheight, int destpitch, unsigned int flags) {
    (void)bink;
    (void)dest;
    (void)destwidth;
    (void)destheight;
    (void)destpitch;
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
static inline void BinkWait(HBINK bink) {
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

#endif // _BINK_H_
