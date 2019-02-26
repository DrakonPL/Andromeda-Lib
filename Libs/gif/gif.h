//
// gif.h
// by Charlie Tangora
// Public domain.
// Email me : ctangora -at- gmail -dot- com
//
// This file offers a simple, very limited way to create animated GIFs directly in code.
//
// Those looking for particular cleverness are likely to be disappointed; it's pretty
// much a straight-ahead implementation of the GIF format with optional Floyd-Steinberg
// dithering. (It does at least use delta encoding - only the changed portions of each
// frame are saved.)
//
// So resulting files are often quite large. The hope is that it will be handy nonetheless
// as a quick and easily-integrated way for programs to spit out animations.
//
// There are two supported input formats, RGBA8 (the alpha is ignored), and
// 8-bit paletted (with a power-of-two palette size).
// (In the latter case you can save up to 768 bytes per frame by providing a global palette
// and reusing it for some frames.)
// Note that only 8-bit input frames can have transparent areas (producing a transparent
// GIF disables delta-coding).
// You can freely mix 32-bit and 8-bit input frames and even frames with differing sizes.
//
// USAGE:
// Create a GifWriter struct. Pass it to GifBegin() to initialize and write the header.
// Pass subsequent frames to GifWriteFrame() or GifWriteFrame8().
// Finally, call GifEnd() to close the file handle and free memory.
//
// A frame is of the type GifRGBA[width][height], aka uint8_t[width][height][4], such that
//    frame[x][y] = [red, green, blue, alpha]

#ifndef gif_h
#define gif_h

#include <stdio.h>   // for FILE*
#include <string.h>  // for memcpy and bzero
#include <stdint.h>  // for integer typedefs
#include <stddef.h>  // for offsetof

// Define these macros to hook into a custom memory allocator.
// TEMP_MALLOC and TEMP_FREE will only be called in stack fashion - frees in the reverse order of mallocs
// and any temp memory allocated by a function will be freed before it exits.
// MALLOC and FREE are used only by GifBegin and GifEnd respectively (to allocate a buffer the size of the image, which
// is used to find changed pixels for delta-encoding.)
// REALLOC is only used if you mix different frame sizes.

#ifndef GIF_TEMP_MALLOC
#include <stdlib.h>
#define GIF_TEMP_MALLOC malloc
#endif

#ifndef GIF_TEMP_FREE
#include <stdlib.h>
#define GIF_TEMP_FREE free
#endif

#ifndef GIF_MALLOC
#include <stdlib.h>
#define GIF_MALLOC malloc
#endif

#ifndef GIF_REALLOC
#include <stdlib.h>
#define GIF_REALLOC realloc
#endif

#ifndef GIF_FREE
#include <stdlib.h>
#define GIF_FREE free
#endif

const int kGifTransIndex = 0;

const int kGifAccumMargin = 64;

const uint8_t kGifNodeUnused = 4;

// Define this to collect and print statistics about the quality of the palette
//#define GIF_STATS(x)  x
#define GIF_STATS

struct GifStats {
    int leaves, searches, totaldiff, cols, nodes;
} GIF_STATS(stats);

// Layout of a pixel for GifWriteFrame. You can reorder this, but must be the same as GifRGBA32.
struct GifRGBA
{
    uint8_t r, g, b, a;

    uint8_t& comps(int comp)
    {
        return ((uint8_t*)this)[comp];
    }
};

struct GifRGBA32
{
    int32_t r, g, b, a;
};

struct GifPalette
{
    int bitDepth;  // log2 of the number of colors

    // alpha component should be set to 0
    GifRGBA colors[256];
};

struct GifKDNode {
    char splitComp;   // Color component index (dimension) to split on, or kGifNodeUnused
    uint8_t splitVal;
};

// k-d tree over RGB space, organized in heap fashion
// i.e. left child of node i is node i*2, right child is node i*2+1;
// nodes[0] is unused.
// nodes 256-511 are implicitly the leaves, containing a color
struct GifKDTree {
    GifKDNode nodes[256];
    GifPalette pal;
};


// max, min, and abs functions
int GifIMax(int l, int r) { return l>r?l:r; }
int GifIMin(int l, int r) { return l<r?l:r; }
uint8_t GifUI8Max(uint8_t l, uint8_t r) { return l>r?l:r; }
uint8_t GifUI8Min(uint8_t l, uint8_t r) { return l<r?l:r; }
int GifIAbs(int i) { return i<0?-i:i; }

bool GifRGBEqual( GifRGBA pixA, GifRGBA pixB )
{
    return pixA.r == pixB.r && pixA.g == pixB.g && pixA.b == pixB.b;
}

// Check if two palettes have the same colours (k-d tree stuff ignored)
bool GifPalettesEqual( const GifPalette* pPal1, const GifPalette* pPal2 )
{
    return pPal1->bitDepth == pPal2->bitDepth &&
           !memcmp(pPal1->colors, pPal2->colors, sizeof(GifRGBA) * (1 << pPal1->bitDepth));
}

// Update bestDiff and return true if color 'ind' is closer to r,g,b than bestDiff (and not transparent).
bool GifBetterColorMatch(const GifPalette* pPal, int ind, GifRGBA color, int& bestDiff)
{
    if(ind == kGifTransIndex) return false;

    int r_err = color.r - (int)pPal->colors[ind].r;
    int g_err = color.g - (int)pPal->colors[ind].g;
    int b_err = color.b - (int)pPal->colors[ind].b;
    int diff = GifIAbs(r_err)+GifIAbs(g_err)+GifIAbs(b_err);
    if(diff >= bestDiff)
        return false;
    bestDiff = diff;
    return true;
}

// walks the k-d tree to pick the palette entry for a desired color.
// Takes as in/out parameters the current best color and its error -
// only changes them if it finds a better color in its subtree.
// this is the major hotspot in the code at the moment.
void GifGetClosestPaletteColor(GifKDTree* tree, GifRGBA color, int& bestInd, int& bestDiff, int treeRoot = 1)
{
    GIF_STATS(stats.nodes++);

    // base case, reached the bottom of the tree
    if(treeRoot > (1 << tree->pal.bitDepth) - 1)
    {
        GIF_STATS(stats.leaves++);
        int ind = treeRoot - (1 << tree->pal.bitDepth);

        // check whether this color is better than the current winner
        if( GifBetterColorMatch(&tree->pal, ind, color, bestDiff) )
            bestInd = ind;
        return;
    }

    GifKDNode &node = tree->nodes[treeRoot];

    // ignore unused nodes
    if(node.splitComp == kGifNodeUnused) return;

    // Compare to the appropriate color component (r, g, or b) for this node of the k-d tree
    int comp = color.comps(node.splitComp);
    if(node.splitVal > comp)
    {
        // check the left subtree
        GifGetClosestPaletteColor(tree, color, bestInd, bestDiff, treeRoot*2);
        if( bestDiff > node.splitVal - comp )
        {
            // cannot prove there's not a better value in the right subtree, check that too
            GifGetClosestPaletteColor(tree, color, bestInd, bestDiff, treeRoot*2+1);
        }
    }
    else
    {
        GifGetClosestPaletteColor(tree, color, bestInd, bestDiff, treeRoot*2+1);
        // The left subtree has component values <= (node.splitVal - 1)
        if( bestDiff > comp - (node.splitVal - 1) )
        {
            GifGetClosestPaletteColor(tree, color, bestInd, bestDiff, treeRoot*2);
        }
    }
}

void GifSwapPixels(GifRGBA* image, int pixA, int pixB)
{
    GifRGBA temp = image[pixA];
    image[pixA] = image[pixB];
    image[pixB] = temp;
}

// just the partition operation from quicksort 3-way
// Center element used as pivot. Afterwards, the pixels in [left, right) have
// 'com' component equal to the pivot; those before/after are lesser/greater.
uint8_t GifPartition(GifRGBA* image, int com, int &left, int &right)
{
    GifSwapPixels(image, left, left + (right - left) / 2);
    uint8_t comPivot = image[left].comps(com);
    for(int i1=left+1; i1<right; ++i1)
    {
        uint8_t comArray = image[i1].comps(com);
        if( comArray < comPivot )
        {
            GifSwapPixels(image, i1, left);
            ++left;
        }
        else if( comArray > comPivot )
        {
            --right;
            GifSwapPixels(image, i1, right);
            --i1;
        }
    }
    return comPivot;
}

// Perform an incomplete sort, finding all elements above and below the desired median
int GifPartitionByMedian(GifRGBA* image, int com, uint8_t& pivotVal, int left, int right, int neededCenter)
{
    int initLeft = left, initRight = right;
    while(left < right-1)
    {
        int centerLeft = left, centerRight = right;
        pivotVal = GifPartition(image, com, centerLeft, centerRight);
        // Pixels with com equal to pivotVal are now in the interval [centerLeft, centerRight)

        if( neededCenter < centerLeft )
            right = centerLeft;
        else if( neededCenter >= centerRight )
            left = centerRight;
        else if( centerLeft != initLeft && neededCenter - centerLeft <= centerRight - neededCenter ||
                 centerRight == initRight )
            // Found the median, but have to decide whether to put it in left or right partition.
            // Never return initRight, would cause out-of-bounds read
            return centerLeft;
        else
        {
            ++pivotVal;
            return centerRight;
        }
    }
    // This happens when neededCenter == left == right - 1
    return neededCenter;
}

// Builds a palette by creating a balanced k-d tree of all pixels in the image
// (There are at most 255 leaves, so it's balanced but not complete.)
void GifSplitPalette(GifRGBA* image, int numPixels, int firstElt, int lastElt, int splitElt, int splitDist, int treeNode, bool buildForDither, GifKDTree* tree)
{
    if(lastElt <= firstElt || numPixels == 0)
        return;

    // base case, bottom of the tree
    if(lastElt == firstElt+1)
    {
        GIF_STATS(stats.cols++);

        // otherwise, take the average of all colors in this subcube
        uint64_t r=0, g=0, b=0;
        for(int ii=0; ii<numPixels; ++ii)
        {
            r += image[ii].r;
            g += image[ii].g;
            b += image[ii].b;
        }

        r += (uint64_t)numPixels / 2;  // round to nearest
        g += (uint64_t)numPixels / 2;
        b += (uint64_t)numPixels / 2;

        r /= (uint32_t)numPixels;
        g /= (uint32_t)numPixels;
        b /= (uint32_t)numPixels;

        GifRGBA& col = tree->pal.colors[firstElt];
        col.r = (uint8_t)r;
        col.g = (uint8_t)g;
        col.b = (uint8_t)b;

        return;
    }

    // Find the axis with the largest range
    int minR = 255, maxR = 0;
    int minG = 255, maxG = 0;
    int minB = 255, maxB = 0;
    for(int ii=0; ii<numPixels; ++ii)
    {
        int r = image[ii].r;
        int g = image[ii].g;
        int b = image[ii].b;

        if(r > maxR) maxR = r;
        if(r < minR) minR = r;

        if(g > maxG) maxG = g;
        if(g < minG) minG = g;

        if(b > maxB) maxB = b;
        if(b < minB) minB = b;
    }

    int rRange = maxR - minR;
    int gRange = maxG - minG;
    int bRange = maxB - minB;

    GifKDNode& node = tree->nodes[treeNode];

    // and split along that axis. (incidentally, this means this isn't a "proper" k-d tree but I don't know what else to call it)
    node.splitComp = offsetof(struct GifRGBA, g);
    if(bRange > gRange) node.splitComp = offsetof(struct GifRGBA, b);
    if(rRange > bRange && rRange > gRange) node.splitComp = offsetof(struct GifRGBA, r);

    int subPixelsA = numPixels * (splitElt - firstElt) / (lastElt - firstElt);
    subPixelsA = GifPartitionByMedian(image, node.splitComp, node.splitVal,  numPixels, subPixelsA);
    int subPixelsB = numPixels - subPixelsA;

    GifSplitPalette(image,            subPixelsA, firstElt, splitElt, splitElt-splitDist, splitDist/2, treeNode*2,   buildForDither, tree);
    GifSplitPalette(image+subPixelsA, subPixelsB, splitElt, lastElt,  splitElt+splitDist, splitDist/2, treeNode*2+1, buildForDither, tree);
}

// Finds all pixels that have changed from the previous image and
// moves them to the front of the buffer.
// This allows us to build a palette optimized for the colors of the
// changed pixels only.
int GifPickChangedPixels( const GifRGBA* lastFrame, GifRGBA* frame, int numPixels )
{
    int numChanged = 0;
    GifRGBA* writeIter = frame;

    for (int ii=0; ii<numPixels; ++ii)
    {
        if( !GifRGBEqual(*lastFrame, *frame) )
        {
            *writeIter++ = *frame;
            ++numChanged;
        }
        ++lastFrame;
        ++frame;
    }

    return numChanged;
}

// Creates a palette by placing all the image pixels in a k-d tree and then averaging the blocks at the bottom.
// This is known as the "modified median split" technique
void GifMakePalette( const GifRGBA* lastFrame, const GifRGBA* nextFrame, uint32_t width, uint32_t height, int bitDepth, bool buildForDither, GifKDTree* tree )
{
    tree->pal.bitDepth = bitDepth;

    // mark all internal nodes unused
    memset(tree->nodes, kGifNodeUnused, sizeof(tree->nodes));
    // Leaf nodes (palette entries) can also be unused, but we can't mark them unused because
    // they're implicit in the tree. Avoid nondeterminism due to random palette entries.
    memset(tree->pal.colors, 0, sizeof(tree->pal.colors));

    // SplitPalette is destructive (it sorts the pixels by color) so
    // we must create a copy of the image for it to destroy
    size_t imageSize = width * height * sizeof(GifRGBA);
    GifRGBA* destroyableImage = (GifRGBA*)GIF_TEMP_MALLOC(imageSize);
    memcpy(destroyableImage, nextFrame, imageSize);

    int numPixels = (int)(width * height);
    if(lastFrame)
        numPixels = GifPickChangedPixels(lastFrame, destroyableImage, numPixels);

    const int lastElt = 1 << bitDepth;
    const int splitElt = lastElt/2;
    const int splitDist = splitElt/2;

    GifSplitPalette(destroyableImage, numPixels, 1, lastElt, splitElt, splitDist, 1, buildForDither, tree);

    GIF_TEMP_FREE(destroyableImage);

    // add the bottom node for the transparency index
    tree->nodes[1 << (bitDepth-1)].splitVal = 0;
    tree->nodes[1 << (bitDepth-1)].splitComp = 0;
    tree->pal.colors[0] = {0, 0, 0, 0);
}

// Implements Floyd-Steinberg dithering, writes palette index to alpha
void GifDitherImage( const GifRGBA* lastFrame, const GifRGBA* nextFrame, GifRGBA* outFrame, uint32_t width, uint32_t height, GifKDTree* tree )
{
    int numPixels = (int)(width * height);

    // quantPixels holds color*256 for all pixels; alpha channel ignored.
    // The extra 8 bits of precision allow for sub-single-color error values
    // to be propagated
    GifRGBA32* quantPixels = (GifRGBA32*)GIF_TEMP_MALLOC(sizeof(GifRGBA32) * (size_t)numPixels);

    for( int ii=0; ii<numPixels*4; ++ii )
    {
        uint8_t pix = ((uint8_t*)nextFrame)[ii];
        ((int32_t*)quantPixels)[ii] = int32_t(pix) * 256;
    }

    for( uint32_t yy=0; yy<height; ++yy )
    {
        for( uint32_t xx=0; xx<width; ++xx )
        {
            GifRGBA32 nextPix = quantPixels[yy*width+xx];  // input
            GifRGBA& outPix = outFrame[yy*width+xx];  // output
            const GifRGBA* lastPix = lastFrame? &lastFrame[yy*width+xx] : NULL;

            // Cap to within reasonable bounds, to prevent excessive bleeding.
            // But it seems permissible to keep some additional error beyond
            // what can be corrected by a single pixel.
            nextPix.r = GifIMin( (255 + kGifAccumMargin) * 256, GifIMax( -kGifAccumMargin, nextPix.r ) );
            nextPix.g = GifIMin( (255 + kGifAccumMargin) * 256, GifIMax( -kGifAccumMargin, nextPix.g ) );
            nextPix.b = GifIMin( (255 + kGifAccumMargin) * 256, GifIMax( -kGifAccumMargin, nextPix.b ) );

            // Compute the colors we want (rounding to nearest)
            GifRGBA searchColor;
            searchColor.r = (uint8_t)GifIMin(255, GifIMax(0, (nextPix.r + 127) / 256));
            searchColor.g = (uint8_t)GifIMin(255, GifIMax(0, (nextPix.g + 127) / 256));
            searchColor.b = (uint8_t)GifIMin(255, GifIMax(0, (nextPix.b + 127) / 256));
            searchColor.a = 0;

            // if it happens that we want the color from last frame, then just write out
            // a transparent pixel
            if( lastFrame && GifRGBEqual(searchColor, *lastPix) )
            {
                outPix = searchColor;
                outPix.a = kGifTransIndex;
                continue;
            }

            int32_t bestDiff = 1000000;
            int32_t bestInd = kGifTransIndex;

            // Search the palete
            GifGetClosestPaletteColor(tree, searchColor, bestInd, bestDiff);
            GIF_STATS(stats.searches++);
            GIF_STATS(stats.totaldiff += bestDiff);

            // Write the result to the temp buffer
            outPix = tree->pal.colors[bestInd];
            outPix.a = bestInd;

            int32_t r_err = nextPix.r - (int32_t)outPix.r * 256;
            int32_t g_err = nextPix.g - (int32_t)outPix.g * 256;
            int32_t b_err = nextPix.b - (int32_t)outPix.b * 256;

            // Propagate the error to the four adjacent locations
            // that we haven't touched yet
            int quantloc_7 = (int)(yy*width+xx+1);
            int quantloc_3 = (int)(yy*width+width+xx-1);
            int quantloc_5 = (int)(yy*width+width+xx);
            int quantloc_1 = (int)(yy*width+width+xx+1);

            if(quantloc_7 < numPixels)
            {
                GifRGBA32& pix7 = quantPixels[quantloc_7];
                pix7.r += r_err * 6 / 16;
                pix7.g += g_err * 6 / 16;
                pix7.b += b_err * 6 / 16;
            }

            if(quantloc_3 < numPixels)
            {
                GifRGBA32& pix3 = quantPixels[quantloc_3];
                pix3.r += r_err * 3 / 16;
                pix3.g += g_err * 3 / 16;
                pix3.b += b_err * 3 / 16;
            }

            if(quantloc_5 < numPixels)
            {
                GifRGBA32& pix5 = quantPixels[quantloc_5];
                pix5.r += r_err * 4 / 16;
                pix5.g += g_err * 4 / 16;
                pix5.b += b_err * 4 / 16;
            }

            if(quantloc_1 < numPixels)
            {
                GifRGBA32& pix1 = quantPixels[quantloc_1];
                pix1.r += r_err / 16;
                pix1.g += g_err / 16;
                pix1.b += b_err / 16;
            }
        }
    }

    GIF_TEMP_FREE(quantPixels);
}

// Picks palette colors for the image using simple thresholding, no dithering. Writes palette index to alpha.
void GifThresholdImage( const GifRGBA* lastFrame, const GifRGBA* nextFrame, GifRGBA* outFrame, uint32_t width, uint32_t height, GifKDTree* tree )
{
    uint32_t numPixels = width*height;
    for( uint32_t ii=0; ii<numPixels; ++ii )
    {
        // if a previous color is available, and it matches the current color,
        // set the pixel to transparent
        if(lastFrame && GifRGBEqual(*lastFrame, *nextFrame))
        {
            *outFrame = *lastFrame;
            outFrame->a = kGifTransIndex;
        }
        else
        {
            // palettize the pixel
            int32_t bestDiff = 1000000;
            int32_t bestInd = 1;
            GifGetClosestPaletteColor(tree, *nextFrame, bestInd, bestDiff);
            GIF_STATS(stats.searches++);
            GIF_STATS(stats.totaldiff += bestDiff);

            // Write the resulting color to the output buffer
            *outFrame = tree->pal.colors[bestInd];
            outFrame->a = (uint8_t)bestInd;
        }

        if(lastFrame) ++lastFrame;
        ++outFrame;
        ++nextFrame;
    }
}

// Compare an already paletted frame to the previous one.
// nextFrame8 is 8-bit, lastFrame and outFrame are 32-bit.
void GifDeltaImage( const GifRGBA* lastFrame, const uint8_t* nextFrame8, GifRGBA* outFrame, uint32_t width, uint32_t height, bool deltaCoded, const GifPalette* pPal )
{
    uint32_t numPixels = width*height;
    int transReplacement = 0;
    if(deltaCoded)
    {
        // Not allowed to use kGifTransIndex, so remap it to nearest match
        int bestDiff = 1000000;
        GifRGBA col = pPal->colors[kGifTransIndex];
        for( int ind=0; ind<(1 << pPal->bitDepth); ++ind )
        {
            // check whether this color is better than the current winner
            if( GifBetterColorMatch(pPal, ind, col, bestDiff) )
                transReplacement = ind;
        }
    }

    for( uint32_t ii=0; ii<numPixels; ++ii )
    {
        int ind = nextFrame8[ii];
        if(ind == kGifTransIndex)
            ind = transReplacement;

        // if a previous color is available, and it matches the current color,
        // set the pixel to transparent
        if(lastFrame && GifRGBEqual(*lastFrame, pPal->colors[ind]))
        {
            *outFrame = *lastFrame;
            outFrame->a = kGifTransIndex;
        }
        else
        {
            *outFrame = pPal->colors[ind];
            outFrame->a = (uint8_t)ind;
        }

        if(lastFrame) ++lastFrame;
        ++outFrame;
    }
}

// Simple structure to write out the LZW-compressed portion of the image
// one bit at a time
struct GifBitStatus
{
    uint8_t bitIndex;  // how many bits in the partial byte written so far
    uint8_t byte;      // current partial byte
    
    uint32_t chunkIndex;
    uint8_t chunk[256];   // bytes are written in here until we have 256 of them, then written to the file
};

// insert a single bit
void GifWriteBit( GifBitStatus& stat, uint32_t bit )
{
    bit = bit & 1;
    bit = bit << stat.bitIndex;
    stat.byte |= bit;

    ++stat.bitIndex;
    if( stat.bitIndex > 7 )
    {
        // move the newly-finished byte to the chunk buffer
        stat.chunk[stat.chunkIndex++] = stat.byte;
        // and start a new byte
        stat.bitIndex = 0;
        stat.byte = 0;
    }
}

// write all bytes so far to the file
void GifWriteChunk( FILE* f, GifBitStatus& stat )
{
    fputc((int)stat.chunkIndex, f);
    fwrite(stat.chunk, 1, stat.chunkIndex, f);

    stat.bitIndex = 0;
    stat.byte = 0;
    stat.chunkIndex = 0;
}

void GifWriteCode( FILE* f, GifBitStatus& stat, uint32_t code, uint32_t length )
{
    for( uint32_t ii=0; ii<length; ++ii )
    {
        GifWriteBit(stat, code);
        code = code >> 1;

        if( stat.chunkIndex == 255 )
        {
            GifWriteChunk(f, stat);
        }
    }
}

// The LZW dictionary is a 256-ary tree constructed as the file is encoded,
// this is one node
struct GifLzwNode
{
    uint16_t m_next[256];
};

// write an image palette to the file
void GifWritePalette( const GifPalette* pPal, FILE* f )
{
    fputc(0, f);  // first color: transparency
    fputc(0, f);
    fputc(0, f);
    for(int ii=1; ii<(1 << pPal->bitDepth); ++ii)
    {
        const GifRGBA &col = pPal->colors[ii];
        fputc((int)col.r, f);
        fputc((int)col.g, f);
        fputc((int)col.b, f);
    }
}

// write the image header, LZW-compress and write out the image
// deltaCoded is true if transparency is used for delta coding, false if producing a transparent GIF
// localPalette is true to write out pPal as a local palette; otherwise it is the global palette.
void GifWriteLzwImage(FILE* f, GifRGBA* image, uint32_t left, uint32_t top,  uint32_t width, uint32_t height, uint32_t delay, const GifPalette* pPal, bool deltaCoded, bool localPalette)
{
    // graphics control extension
    fputc(0x21, f);
    fputc(0xf9, f);
    fputc(0x04, f);
    // disposal method
    if( deltaCoded )
        fputc(0x05, f); // leave this frame in place (next will draw on top)
    else
        fputc(0x09, f); // replace this frame with the background (so next can have transparent areas)
    fputc(delay & 0xff, f);
    fputc((delay >> 8) & 0xff, f);
    fputc(kGifTransIndex, f); // transparent color index
    fputc(0, f);

    fputc(0x2c, f); // image descriptor block

    fputc(left & 0xff, f);           // corner of image in canvas space
    fputc((left >> 8) & 0xff, f);
    fputc(top & 0xff, f);
    fputc((top >> 8) & 0xff, f);

    fputc(width & 0xff, f);          // width and height of image
    fputc((width >> 8) & 0xff, f);
    fputc(height & 0xff, f);
    fputc((height >> 8) & 0xff, f);

    if( localPalette )
    {
        fputc(0x80 + pPal->bitDepth-1, f); // local color table present, 2 ^ bitDepth entries
        GifWritePalette(pPal, f);
    }
    else
    {
        fputc(0, f); // no local color table
    }

    const int minCodeSize = pPal->bitDepth;
    const uint32_t clearCode = 1 << pPal->bitDepth;

    fputc(minCodeSize, f); // min code size 8 bits

    GifLzwNode* codetree = (GifLzwNode*)GIF_TEMP_MALLOC(sizeof(GifLzwNode)*4096);

    memset(codetree, 0, sizeof(GifLzwNode)*4096);
    int32_t curCode = -1;
    uint32_t codeSize = (uint32_t)minCodeSize + 1;
    uint32_t maxCode = clearCode+1;

    GifBitStatus stat;
    stat.byte = 0;
    stat.bitIndex = 0;
    stat.chunkIndex = 0;

    GifWriteCode(f, stat, clearCode, codeSize);  // start with a fresh LZW dictionary

    for(uint32_t yy=0; yy<height; ++yy)
    {
        for(uint32_t xx=0; xx<width; ++xx)
        {
            uint8_t nextValue = image[yy*width+xx].a;

            // "loser mode" - no compression, every single code is followed immediately by a clear
            //WriteCode( f, stat, nextValue, codeSize );
            //WriteCode( f, stat, 256, codeSize );

            if( curCode < 0 )
            {
                // the first value in the image
                curCode = nextValue;
            }
            else if( codetree[curCode].m_next[nextValue] )
            {
                // current run already in the dictionary
                curCode = codetree[curCode].m_next[nextValue];
            }
            else
            {
                // finish the current run, write a code
                GifWriteCode( f, stat, (uint32_t)curCode, codeSize );

                // insert the new run into the dictionary
                codetree[curCode].m_next[nextValue] = (uint16_t)++maxCode;

                if( maxCode >= (1ul << codeSize) )
                {
                    // dictionary entry count has broken a size barrier,
                    // we need more bits for codes
                    codeSize++;
                }
                if( maxCode == 4095 )
                {
                    // the dictionary is full, clear it out and begin anew
                    GifWriteCode(f, stat, clearCode, codeSize); // clear tree

                    memset(codetree, 0, sizeof(GifLzwNode)*4096);
                    codeSize = (uint32_t)minCodeSize + 1;
                    maxCode = clearCode+1;
                }

                curCode = nextValue;
            }
        }
    }

    // compression footer
    GifWriteCode( f, stat, (uint32_t)curCode, codeSize );
    GifWriteCode( f, stat, clearCode, codeSize );
    GifWriteCode( f, stat, clearCode+1, (uint32_t)minCodeSize+1 );

    // write out the last partial chunk
    while( stat.bitIndex ) GifWriteBit(stat, 0);
    if( stat.chunkIndex ) GifWriteChunk(f, stat);

    fputc(0, f); // image block terminator

    GIF_TEMP_FREE(codetree);
}

struct GifWriter
{
    FILE* f;
    GifRGBA* oldImage;
    bool firstFrame;
    bool deltaCoded;
    GifPalette* globalPal;
    int maxWidth;
    int maxHeight;
    int currentWidth;
    int currentHeight;
    bool sizeChanged;
};

// Handle a call to GifWriteFrame[8] with a different image size to the previous
void GifHandleSizeChange( GifWriter* writer, int width, int height )
{
    if(writer->currentWidth != width || writer->currentHeight != height)
    {
        writer->maxWidth = GifIMax(writer->maxWidth, width);
        writer->maxHeight = GifIMax(writer->maxHeight, height);
        writer->currentWidth = width;
        writer->currentHeight = height;
        writer->oldImage = (GifRGBA*)GIF_REALLOC(writer->oldImage, (size_t)(width*height) * sizeof(GifRGBA));
        writer->firstFrame = true;  // Ignore the contents of oldImage
        writer->sizeChanged = true;
    }
}

// Creates a gif file.
// The input GIFWriter is assumed to be uninitialized.
// The delay value is the time between frames in hundredths of a second - note that not all viewers pay much attention to this value.
// transparent is whether to produce a transparent GIF. It only works if using GifWriteFrame8()
//     to provide images containing transparency, and it disables delta coding.
// globalPal is a default palette to use for GifWriteFrame8(). It is not used by GifWriteFrame().
bool GifBegin( GifWriter* writer, FILE *file, uint32_t width, uint32_t height, uint32_t delay, bool transparent = false, const GifPalette* globalPal = NULL )
{
    if(!file) return false;
    writer->f = file;

    writer->firstFrame = true;
    writer->deltaCoded = !transparent;

    // allocate
    writer->oldImage = (GifRGBA*)GIF_MALLOC(width*height*sizeof(GifRGBA));

    fputs("GIF89a", writer->f);

    // screen descriptor
    fputc(width & 0xff, writer->f);
    fputc((width >> 8) & 0xff, writer->f);
    fputc(height & 0xff, writer->f);
    fputc((height >> 8) & 0xff, writer->f);
    writer->currentWidth = writer->maxWidth = (int)width;
    writer->currentHeight = writer->maxHeight = (int)height;
    writer->sizeChanged = false;

    if( globalPal )
        fputc(0xf0 + (globalPal->bitDepth - 1), writer->f);  // there is an unsorted global color table
    else
        fputc(0xf0, writer->f);  // there is an unsorted global color table of 2 entries
    fputc(0, writer->f);     // background color
    fputc(0, writer->f);     // pixels are square (we need to specify this because it's 1989)

    if( globalPal )
    {
        writer->globalPal = (GifPalette*)GIF_MALLOC(sizeof(GifPalette));
        memcpy(writer->globalPal, globalPal, sizeof(GifPalette));
        // write the global palette
        GifWritePalette(globalPal, writer->f);
    }
    else
    {
        writer->globalPal = NULL;
        // now the "global" palette (really just a dummy palette)
        // color 0: black
        fputc(0, writer->f);
        fputc(0, writer->f); 
        fputc(0, writer->f);
        // color 1: also black
        fputc(0, writer->f);
        fputc(0, writer->f);
        fputc(0, writer->f);
    }

    if( delay != 0 )
    {
        // animation header
        fputc(0x21, writer->f); // extension
        fputc(0xff, writer->f); // application specific
        fputc(11, writer->f); // length 11
        fputs("NETSCAPE2.0", writer->f); // yes, really
        fputc(3, writer->f); // 3 bytes of NETSCAPE2.0 data

        fputc(1, writer->f); // JUST BECAUSE
        fputc(0, writer->f); // loop infinitely (byte 0)
        fputc(0, writer->f); // loop infinitely (byte 1)

        fputc(0, writer->f); // block terminator
    }

    return true;
}

// Writes out a new frame to a GIF in progress.
// The GIFWriter should have been created by GIFBegin.
// AFAIK, it is legal to use different bit depths for different frames of an image -
// this may be handy to save bits in animations that don't change much.
bool GifWriteFrame( GifWriter* writer, const GifRGBA* image, uint32_t width, uint32_t height, uint32_t delay, int bitDepth = 8, bool dither = false )
{
    if(!writer->f) return false;
    if(bitDepth <= 0 || bitDepth > 8) return false;
    GIF_STATS(memset(&stats, 0, sizeof stats));

    GifHandleSizeChange(writer, (int)width, (int)height);
    const GifRGBA* oldImage = writer->firstFrame? NULL : writer->oldImage;
    // Only GifWriteFrame8 can produce transparent frames, but the frame before the current one
    // needs to be set to 'background' disposal to support that. So for simplicity, we disable
    // delta coding for all frames.
    if(!writer->deltaCoded)
        oldImage = NULL;
    writer->firstFrame = false;

    GifKDTree tree;
    GifMakePalette((dither? NULL : oldImage), image, width, height, bitDepth, dither, &tree);

    if(dither)
        GifDitherImage(oldImage, image, writer->oldImage, width, height, &tree);
    else
        GifThresholdImage(oldImage, image, writer->oldImage, width, height, &tree);

    GIF_STATS(
        printf("avg leaves = %.3f avg nodes = %.3f average diff = %.4f selected cols = %d\n",
               1. * stats.leaves / stats.searches, 1. * stats.nodes / stats.searches,
               1. * stats.totaldiff / stats.searches, stats.cols)
    );

    GifWriteLzwImage(writer->f, writer->oldImage, 0, 0, width, height, delay, &tree.pal, writer->deltaCoded, true);

    return true;
}

// (See also GifWriteFrame.)
// If palette is NULL, or if it is identical to the global palette, then the global palette is used.
// If the GIF is transparent then index kGifTransIndex is transparency, otherwise
// all color indices may be used (however, kGifTransIndex is internally used for delta-coding so
// when it occurs in the input it will be replaced with the nearest match, so it's better to avoid it).
bool GifWriteFrame8( GifWriter* writer, const uint8_t* image, uint32_t width, uint32_t height, uint32_t delay, const GifPalette* pal = NULL )
{
    if(!writer->f) return false;
    if(!writer->globalPal && !pal) return false;

    GifHandleSizeChange(writer, (int)width, (int)height);
    const GifRGBA* oldImage = writer->firstFrame? NULL : writer->oldImage;
    if(!writer->deltaCoded)
        oldImage = NULL;
    writer->firstFrame = false;

    // Only write the local palette if it differs from global, or if there is no global palette
    if(pal && writer->globalPal && GifPalettesEqual(writer->globalPal, pal))
        pal = NULL;
    bool localPalette = (pal != NULL);
    if(!pal)
        pal = writer->globalPal;

    GifDeltaImage(oldImage, image, writer->oldImage, width, height, writer->deltaCoded, pal);

    GifWriteLzwImage(writer->f, writer->oldImage, 0, 0, width, height, delay, pal, writer->deltaCoded, localPalette);

    return true;
}

// Writes the EOF code, closes the file handle, and frees temp memory used by a GIF.
// Many if not most viewers will still display a GIF properly if the EOF code is missing,
// but it's still a good idea to write it out.
bool GifEnd( GifWriter* writer )
{
    if(!writer->f) return false;

    fputc(0x3b, writer->f); // end of file
    if(writer->sizeChanged)
    {
        fseek(writer->f, 6, SEEK_SET);
        fputc(writer->maxWidth & 0xff, writer->f);
        fputc((writer->maxWidth >> 8) & 0xff, writer->f);
        fputc(writer->maxHeight & 0xff, writer->f);
        fputc((writer->maxHeight >> 8) & 0xff, writer->f);
    }
    fclose(writer->f);
    GIF_FREE(writer->oldImage);
    GIF_FREE(writer->globalPal);

    writer->f = NULL;
    writer->oldImage = NULL;
    writer->globalPal = NULL;

    return true;
}

#endif