/* Cleaned-up version of sample_vi_fd.c
 * Purpose: Stream raw camera image over network (e.g., to VLC)
 * Removed: AI model loading and face detection
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "cvi_comm_video.h"
#include "vi.h"
#include "venc.h"
#include "cvi_sys.h"

static bool running = true;

void handle_sig(int sig) {
    running = false;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sig);
    signal(SIGTERM, handle_sig);

    CVI_SYS_Init();

    // VI (Video Input) setup
    VI_DEV viDev = 0;
    VI_PIPE viPipe = 0;
    VI_CHN viChn = 0;
    SIZE_S stSize = {320, 240}; // resolution

    CVI_VI_SetDevAttr(viDev, &(VI_DEV_ATTR_S){
        .enIntfMode = VI_MODE_DIGITAL_CAMERA,
        .enWorkMode = VI_WORK_MODE_1Multiplex,
        .enScanMode = VI_SCAN_PROGRESSIVE,
        .enDataSeq = VI_INPUT_DATA_YVYU,
        .enInputDataType = DATA_TYPE_YUV,
        .stSize = stSize
    });

    CVI_VI_EnableDev(viDev);
    CVI_VI_SetChnAttr(viPipe, viChn, &(VI_CHN_ATTR_S){
        .stSize = stSize,
        .enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420,
        .enDynamicRange = DYNAMIC_RANGE_SDR8,
        .enVideoFormat = VIDEO_FORMAT_LINEAR,
        .enCompressMode = COMPRESS_MODE_NONE
    });

    CVI_VI_EnableChn(viPipe, viChn);

    // VENC (Video Encode) setup
    VENC_CHN vencChn = 0;
    CVI_VENC_Create(vencChn, &(VENC_CHN_ATTR_S){
        .stVencAttr.enType = PT_H264,
        .stVencAttr.u32Profile = 1,
        .stVencAttr.u32PicWidth = 320,
        .stVencAttr.u32PicHeight = 240,
        .stVencAttr.u32BufSize = 2 * 320 * 240,
        .stVencAttr.u32FrameRate = 15,
        .stVencAttr.enGopMode = VENC_GOPMODE_NORMALP
    });

    // Bind VI to VENC
    MPP_CHN_S stSrcChn = {.enModId = CVI_ID_VI, .s32DevId = viPipe, .s32ChnId = viChn};
    MPP_CHN_S stDestChn = {.enModId = CVI_ID_VENC, .s32DevId = 0, .s32ChnId = vencChn};
    CVI_SYS_Bind(&stSrcChn, &stDestChn);

    CVI_VENC_StartRecvPic(vencChn);

    // Open output pipe (send to stdout or a file or TCP stream)
    FILE *out = fopen("/mnt/data/video.h264", "wb");
    if (!out) {
        perror("fopen");
        return -1;
    }

    while (running) {
        VENC_STREAM_S stStream;
        if (CVI_VENC_GetStream(vencChn, &stStream, 1000) == CVI_SUCCESS) {
            fwrite(stStream.pstPack[0].pu8Addr, 1, stStream.pstPack[0].u32Len, out);
            fflush(out);
            CVI_VENC_ReleaseStream(vencChn, &stStream);
        }
    }

    fclose(out);

    // Cleanup
    CVI_VENC_StopRecvPic(vencChn);
    CVI_SYS_UnBind(&stSrcChn, &stDestChn);
    CVI_VENC_DestroyChn(vencChn);
    CVI_VI_DisableChn(viPipe, viChn);
    CVI_VI_DisableDev(viDev);
    CVI_SYS_Exit();

    return 0;
}
