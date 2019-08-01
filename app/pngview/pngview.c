//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2013 Andrew Duncan
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

#define _GNU_SOURCE

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "backgroundLayer.h"
#include "imageLayer.h"
#include "key.h"
#include "loadpng.h"

#include "bcm_host.h"

//-------------------------------------------------------------------------

#define NDEBUG

//-------------------------------------------------------------------------

const char *program = NULL;

int main(int argc, char *argv[])
{
    uint16_t backgroundColor = 0x000F;
    int32_t layer = 1;
    uint32_t displayNumber = 0;
    int32_t xOffset = 0;
    int32_t yOffset = 0;
    uint32_t timeout = 0;
    program = basename(argv[0]);

    IMAGE_LAYER_T imageLayer;

    const char *imagePath = argv[1];

    // Load image from path
    if (loadPng(&(imageLayer.image), imagePath) == false)
    {
        fprintf(stderr, "unable to load %s\n", imagePath);
        exit(EXIT_FAILURE);
    }

    bcm_host_init();

    //---------------------------------------------------------------------

    DISPMANX_DISPLAY_HANDLE_T display = vc_dispmanx_display_open(displayNumber);
    assert(display != 0);

    //---------------------------------------------------------------------

    DISPMANX_MODEINFO_T info;
    int result = vc_dispmanx_display_get_info(display, &info);
    assert(result == 0);

    //---------------------------------------------------------------------

    BACKGROUND_LAYER_T backgroundLayer;

    if (backgroundColor > 0)
    {
        initBackgroundLayer(&backgroundLayer, backgroundColor, 0);
    }

    createResourceImageLayer(&imageLayer, layer);

    //---------------------------------------------------------------------

    DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(0);
    assert(update != 0);

    if (backgroundColor > 0)
    {
        addElementBackgroundLayer(&backgroundLayer, display, update);
    }

    xOffset = (info.width - imageLayer.image.width) / 2;
    yOffset = (info.height - imageLayer.image.height) / 2;

    addElementImageLayerOffset(&imageLayer,
                               xOffset,
                               yOffset,
                               display,
                               update);

    result = vc_dispmanx_update_submit_sync(update);
    assert(result == 0);

    //---------------------------------------------------------------------

    const int sleepMilliseconds = 10;
    usleep(sleepMilliseconds * 1000);

    if (backgroundColor > 0)
    {
        destroyBackgroundLayer(&backgroundLayer);
    }

    destroyImageLayer(&imageLayer);

    result = vc_dispmanx_display_close(display);
    assert(result == 0);

    //---------------------------------------------------------------------

    return 0;
}
