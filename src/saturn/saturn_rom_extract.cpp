#include "saturn_rom_extract.h"
#include "saturn_assets.h"

#include <filesystem>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#include "saturn/libs/portable-file-dialogs.h"

extern "C" {
#include "pc/platform.h"
#include "pc/pngutils.h"
}

struct Asset {
    std::string path;
    int mio0;
    int pos;
    int len;
    std::vector<int> metadata;
};

struct SaturnAsset {
    std::string path;
    unsigned char* data;
    std::vector<int> metadata;
};

std::vector<Asset> assets = {
   { "gfx/levels/wf/5.ia8.png", 1083968, 74168, 256, { 16, 16 } },
   { "gfx/textures/fire/lll_textures.09000.rgba16.png", 1083968, 92856, 2048, { 32, 32 } },
   { "gfx/textures/segment2/font_graphics.05900.ia4.png", 1083968, 22784, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05940.ia4.png", 1083968, 22848, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05980.ia4.png", 1083968, 22912, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.059C0.ia4.png", 1083968, 22976, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05A00.ia4.png", 1083968, 23040, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05A40.ia4.png", 1083968, 23104, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05A80.ia4.png", 1083968, 23168, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05AC0.ia4.png", 1083968, 23232, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05B00.ia4.png", 1083968, 23296, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05B40.ia4.png", 1083968, 23360, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05B80.ia4.png", 1083968, 23424, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05BC0.ia4.png", 1083968, 23488, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05C00.ia4.png", 1083968, 23552, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05C40.ia4.png", 1083968, 23616, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05C80.ia4.png", 1083968, 23680, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05CC0.ia4.png", 1083968, 23744, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05D00.ia4.png", 1083968, 23808, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05D40.ia4.png", 1083968, 23872, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05D80.ia4.png", 1083968, 23936, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05DC0.ia4.png", 1083968, 24000, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05E00.ia4.png", 1083968, 24064, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05E40.ia4.png", 1083968, 24128, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05E80.ia4.png", 1083968, 24192, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05EC0.ia4.png", 1083968, 24256, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05F00.ia4.png", 1083968, 24320, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05F40.ia4.png", 1083968, 24384, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05F80.ia4.png", 1083968, 24448, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.05FC0.ia4.png", 1083968, 24512, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06000.ia4.png", 1083968, 24576, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06040.ia4.png", 1083968, 24640, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06080.ia4.png", 1083968, 24704, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.060C0.ia4.png", 1083968, 24768, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06100.ia4.png", 1083968, 24832, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06140.ia4.png", 1083968, 24896, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06180.ia4.png", 1083968, 24960, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.061C0.ia4.png", 1083968, 25024, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06200.ia4.png", 1083968, 25088, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06240.ia4.png", 1083968, 25152, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06280.ia4.png", 1083968, 25216, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.062C0.ia4.png", 1083968, 25280, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06300.ia4.png", 1083968, 25344, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06340.ia4.png", 1083968, 25408, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06380.ia4.png", 1083968, 25472, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.063C0.ia4.png", 1083968, 25536, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06400.ia4.png", 1083968, 25600, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06440.ia4.png", 1083968, 25664, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06480.ia4.png", 1083968, 25728, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.064C0.ia4.png", 1083968, 25792, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06500.ia4.png", 1083968, 25856, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06540.ia4.png", 1083968, 25920, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06580.ia4.png", 1083968, 25984, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.065C0.ia4.png", 1083968, 26048, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06600.ia4.png", 1083968, 26112, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06640.ia4.png", 1083968, 26176, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06680.ia4.png", 1083968, 26240, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.066C0.ia4.png", 1083968, 26304, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06700.ia4.png", 1083968, 26368, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06740.ia4.png", 1083968, 26432, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06780.ia4.png", 1083968, 26496, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.067C0.ia4.png", 1083968, 26560, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06800.ia4.png", 1083968, 26624, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06840.ia4.png", 1083968, 26688, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06880.ia4.png", 1083968, 26752, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.068C0.ia4.png", 1083968, 26816, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06900.ia4.png", 1083968, 26880, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06940.ia4.png", 1083968, 26944, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06980.ia4.png", 1083968, 27008, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.069C0.ia4.png", 1083968, 27072, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06A00.ia4.png", 1083968, 27136, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06A40.ia4.png", 1083968, 27200, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06A80.ia4.png", 1083968, 27264, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06AC0.ia4.png", 1083968, 27328, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06B00.ia4.png", 1083968, 27392, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06B40.ia4.png", 1083968, 27456, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06B80.ia4.png", 1083968, 27520, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06BC0.ia4.png", 1083968, 27584, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06C00.ia4.png", 1083968, 27648, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06C40.ia4.png", 1083968, 27712, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06C80.ia4.png", 1083968, 27776, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06CC0.ia4.png", 1083968, 27840, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06D00.ia4.png", 1083968, 27904, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06D40.ia4.png", 1083968, 27968, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06D80.ia4.png", 1083968, 28032, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06DC0.ia4.png", 1083968, 28096, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06E00.ia4.png", 1083968, 28160, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06E40.ia4.png", 1083968, 28224, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06E80.ia4.png", 1083968, 28288, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06EC0.ia4.png", 1083968, 28352, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06F00.ia4.png", 1083968, 28416, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06F40.ia4.png", 1083968, 28480, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06F80.ia4.png", 1083968, 28544, 64, { 16, 8 } },
   { "gfx/textures/segment2/font_graphics.06FC0.ia4.png", 1083968, 28608, 64, { 16, 8 } },
   { "gfx/textures/segment2/segment2.00000.rgba16.png", 1083968, 0, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.00200.rgba16.png", 1083968, 512, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.00400.rgba16.png", 1083968, 1024, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.00600.rgba16.png", 1083968, 1536, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.00800.rgba16.png", 1083968, 2048, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.00A00.rgba16.png", 1083968, 2560, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.00C00.rgba16.png", 1083968, 3072, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.00E00.rgba16.png", 1083968, 3584, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.01000.rgba16.png", 1083968, 4096, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.01200.rgba16.png", 1083968, 4608, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.01400.rgba16.png", 1083968, 5120, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.01600.rgba16.png", 1083968, 5632, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.01800.rgba16.png", 1083968, 6144, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.01A00.rgba16.png", 1083968, 6656, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.01C00.rgba16.png", 1083968, 7168, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.01E00.rgba16.png", 1083968, 7680, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.02000.rgba16.png", 1083968, 8192, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.02200.rgba16.png", 1083968, 8704, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.02400.rgba16.png", 1083968, 9216, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.02800.rgba16.png", 1083968, 9728, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.02A00.rgba16.png", 1083968, 10240, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.02C00.rgba16.png", 1083968, 10752, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.02E00.rgba16.png", 1083968, 11264, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.03000.rgba16.png", 1083968, 11776, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.03200.rgba16.png", 1083968, 12288, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.03600.rgba16.png", 1083968, 12800, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.03800.rgba16.png", 1083968, 13312, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.03A00.rgba16.png", 1083968, 13824, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.03C00.rgba16.png", 1083968, 14336, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.04000.rgba16.png", 1083968, 14848, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.04400.rgba16.png", 1083968, 15360, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.04800.rgba16.png", 1083968, 15872, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.04A00.rgba16.png", 1083968, 16384, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.05600.rgba16.png", 1083968, 16896, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.05800.rgba16.png", 1083968, 17408, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.05A00.rgba16.png", 1083968, 17920, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.05C00.rgba16.png", 1083968, 18432, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.06200.rgba16.png", 1083968, 18944, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06280.rgba16.png", 1083968, 19072, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06300.rgba16.png", 1083968, 19200, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06380.rgba16.png", 1083968, 19328, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06400.rgba16.png", 1083968, 19456, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06480.rgba16.png", 1083968, 19584, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06500.rgba16.png", 1083968, 19712, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06580.rgba16.png", 1083968, 19840, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06600.rgba16.png", 1083968, 19968, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06680.rgba16.png", 1083968, 20096, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06700.rgba16.png", 1083968, 20224, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06780.rgba16.png", 1083968, 20352, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06800.rgba16.png", 1083968, 20480, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06880.rgba16.png", 1083968, 20608, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06900.rgba16.png", 1083968, 20736, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06980.rgba16.png", 1083968, 20864, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06A00.rgba16.png", 1083968, 20992, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06A80.rgba16.png", 1083968, 21120, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06B00.rgba16.png", 1083968, 21248, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06B80.rgba16.png", 1083968, 21376, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06C00.rgba16.png", 1083968, 21504, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06C80.rgba16.png", 1083968, 21632, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06D00.rgba16.png", 1083968, 21760, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06D80.rgba16.png", 1083968, 21888, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06E00.rgba16.png", 1083968, 22016, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06E80.rgba16.png", 1083968, 22144, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06F00.rgba16.png", 1083968, 22272, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.06F80.rgba16.png", 1083968, 22400, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.07000.rgba16.png", 1083968, 22528, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.07080.rgba16.png", 1083968, 22656, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.07340.ia1.png", 1083968, 24050, 16, { 8, 16 } },
   { "gfx/textures/segment2/segment2.07B50.rgba16.png", 1083968, 28672, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.07D50.rgba16.png", 1083968, 29184, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.07F50.rgba16.png", 1083968, 29696, 512, { 16, 16 } },
   { "gfx/textures/segment2/segment2.08150.rgba16.png", 1083968, 30208, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.081D0.rgba16.png", 1083968, 30336, 128, { 8, 8 } },
   { "gfx/textures/segment2/segment2.0F458.ia8.png", 1083968, 74424, 2048, { 32, 64 } },
   { "gfx/textures/segment2/segment2.0FC58.ia8.png", 1083968, 76472, 2048, { 32, 64 } },
   { "gfx/textures/segment2/segment2.10458.ia8.png", 1083968, 78520, 4096, { 64, 64 } },
   { "gfx/textures/segment2/segment2.11458.ia8.png", 1083968, 82616, 2048, { 32, 64 } },
   { "gfx/textures/segment2/segment2.11C58.rgba16.png", 1083968, 84664, 2048, { 32, 32 } },
   { "gfx/textures/segment2/segment2.12458.rgba16.png", 1083968, 86712, 2048, { 32, 32 } },
   { "gfx/textures/segment2/segment2.12C58.rgba16.png", 1083968, 88760, 2048, { 32, 32 } },
   { "gfx/textures/segment2/segment2.13458.ia16.png", 1083968, 90808, 2048, { 32, 32 } },
   { "gfx/textures/segment2/segment2.13C58.rgba16.png", 1083968, 92856, 2048, { 32, 32 } },
   { "gfx/textures/segment2/segment2.14838.ia8.png", 1083968, 95896, 64, { 8, 8 } },
   { "gfx/textures/segment2/shadow_quarter_circle.ia8.png", 1083968, 73912, 256, { 16, 16 } },
   { "gfx/textures/segment2/shadow_quarter_square.ia8.png", 1083968, 74168, 256, { 16, 16 } },
   { "gfx/actors/bubble/bubble.rgba16.png", 1132368, 118112, 2048, { 32, 32 } },
   { "gfx/actors/bubble/mr_i_bubble.rgba16.png", 1132368, 120160, 2048, { 32, 32 } },
   { "gfx/actors/burn_smoke/burn_smoke.ia16.png", 1132368, 137216, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_center.rgba16.png", 1132368, 12432, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_closed.rgba16.png", 1132368, 16528, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_closed_unused_0.rgba16.png", 1132368, 16528, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_closed_unused_1.rgba16.png", 1132368, 16528, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_dead.rgba16.png", 1132368, 30864, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_down_unused.rgba16.png", 1132368, 28816, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_half_closed.rgba16.png", 1132368, 14480, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_left_unused.rgba16.png", 1132368, 22672, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_right_unused.rgba16.png", 1132368, 24720, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_eyes_up_unused.rgba16.png", 1132368, 26768, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_logo.rgba16.png", 1132368, 6288, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_metal.rgba16.png", 1132368, 144, 4096, { 64, 32 } },
   { "gfx/actors/mario/mario_metal_wing_tip_unused.rgba16.png", 1132368, 45200, 4096, { 32, 64 } },
   { "gfx/actors/mario/mario_metal_wing_unused.rgba16.png", 1132368, 41104, 4096, { 32, 64 } },
   { "gfx/actors/mario/mario_mustache.rgba16.png", 1132368, 10384, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_overalls_button.rgba16.png", 1132368, 4240, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_sideburn.rgba16.png", 1132368, 8336, 2048, { 32, 32 } },
   { "gfx/actors/mario/mario_wing.rgba16.png", 1132368, 32912, 4096, { 32, 64 } },
   { "gfx/actors/mario/mario_wing_tip.rgba16.png", 1132368, 37008, 4096, { 32, 64 } },
   { "gfx/actors/mario_cap/mario_cap_metal.rgba16.png", 1132368, 144, 4096, { 64, 32 } },
   { "gfx/actors/mario_cap/mario_cap_metal_wing_tip_unused.rgba16.png", 1132368, 45200, 4096, { 32, 64 } },
   { "gfx/actors/mario_cap/mario_cap_metal_wing_unused.rgba16.png", 1132368, 41104, 4096, { 32, 64 } },
   { "gfx/actors/mario_cap/mario_cap_wing.rgba16.png", 1132368, 32912, 4096, { 32, 64 } },
   { "gfx/actors/mario_cap/mario_cap_wing_tip.rgba16.png", 1132368, 37008, 4096, { 32, 64 } },
   { "gfx/actors/sparkle/sparkle_0.rgba16.png", 1132368, 160912, 2048, { 32, 32 } },
   { "gfx/actors/sparkle/sparkle_1.rgba16.png", 1132368, 162960, 2048, { 32, 32 } },
   { "gfx/actors/sparkle/sparkle_2.rgba16.png", 1132368, 165008, 2048, { 32, 32 } },
   { "gfx/actors/sparkle/sparkle_3.rgba16.png", 1132368, 167056, 2048, { 32, 32 } },
   { "gfx/actors/sparkle/sparkle_4.rgba16.png", 1132368, 169104, 2048, { 32, 32 } },
   { "gfx/actors/sparkle/sparkle_5.rgba16.png", 1132368, 171152, 2048, { 32, 32 } },
   { "gfx/actors/sparkle_animation/sparkle_animation_0.ia16.png", 1132368, 207496, 2048, { 32, 32 } },
   { "gfx/actors/sparkle_animation/sparkle_animation_1.ia16.png", 1132368, 209544, 2048, { 32, 32 } },
   { "gfx/actors/sparkle_animation/sparkle_animation_2.ia16.png", 1132368, 211592, 2048, { 32, 32 } },
   { "gfx/actors/sparkle_animation/sparkle_animation_3.ia16.png", 1132368, 213640, 2048, { 32, 32 } },
   { "gfx/actors/sparkle_animation/sparkle_animation_4.ia16.png", 1132368, 215688, 2048, { 32, 32 } },
   { "gfx/actors/stomp_smoke/stomp_smoke_0.ia16.png", 1132368, 139592, 2048, { 32, 32 } },
   { "gfx/actors/stomp_smoke/stomp_smoke_1.ia16.png", 1132368, 141640, 2048, { 32, 32 } },
   { "gfx/actors/stomp_smoke/stomp_smoke_2.ia16.png", 1132368, 143688, 2048, { 32, 32 } },
   { "gfx/actors/stomp_smoke/stomp_smoke_3.ia16.png", 1132368, 145736, 2048, { 32, 32 } },
   { "gfx/actors/stomp_smoke/stomp_smoke_4.ia16.png", 1132368, 147784, 2048, { 32, 32 } },
   { "gfx/actors/stomp_smoke/stomp_smoke_5.ia16.png", 1132368, 149832, 2048, { 32, 32 } },
   { "gfx/actors/walk_smoke/walk_smoke_0.ia16.png", 1132368, 122528, 2048, { 32, 32 } },
   { "gfx/actors/walk_smoke/walk_smoke_1.ia16.png", 1132368, 124576, 2048, { 32, 32 } },
   { "gfx/actors/walk_smoke/walk_smoke_2.ia16.png", 1132368, 126624, 2048, { 32, 32 } },
   { "gfx/actors/walk_smoke/walk_smoke_3.ia16.png", 1132368, 128672, 2048, { 32, 32 } },
   { "gfx/actors/walk_smoke/walk_smoke_4.ia16.png", 1132368, 130720, 2048, { 32, 32 } },
   { "gfx/actors/walk_smoke/walk_smoke_5.ia16.png", 1132368, 132768, 2048, { 32, 32 } },
   { "gfx/actors/walk_smoke/walk_smoke_6.ia16.png", 1132368, 134816, 2048, { 32, 32 } },
   { "gfx/actors/water_splash/water_splash_0.rgba16.png", 1132368, 173512, 4096, { 32, 64 } },
   { "gfx/actors/water_splash/water_splash_1.rgba16.png", 1132368, 177608, 4096, { 32, 64 } },
   { "gfx/actors/water_splash/water_splash_2.rgba16.png", 1132368, 181704, 4096, { 32, 64 } },
   { "gfx/actors/water_splash/water_splash_3.rgba16.png", 1132368, 185800, 4096, { 32, 64 } },
   { "gfx/actors/water_splash/water_splash_4.rgba16.png", 1132368, 189896, 4096, { 32, 64 } },
   { "gfx/actors/water_splash/water_splash_5.rgba16.png", 1132368, 193992, 4096, { 32, 64 } },
   { "gfx/actors/water_splash/water_splash_6.rgba16.png", 1132368, 198088, 4096, { 32, 64 } },
   { "gfx/actors/water_splash/water_splash_7.rgba16.png", 1132368, 202184, 4096, { 32, 64 } },
   { "gfx/actors/water_wave/water_wave_0.ia16.png", 1132368, 152408, 2048, { 32, 32 } },
   { "gfx/actors/water_wave/water_wave_1.ia16.png", 1132368, 154456, 2048, { 32, 32 } },
   { "gfx/actors/water_wave/water_wave_2.ia16.png", 1132368, 156504, 2048, { 32, 32 } },
   { "gfx/actors/water_wave/water_wave_3.ia16.png", 1132368, 158552, 2048, { 32, 32 } },
   { "gfx/actors/white_particle_small/small_snow_particle.rgba16.png", 1132368, 206720, 512, { 16, 16 } },
   { "gfx/actors/bullet_bill/bullet_bill_eye.rgba16.png", 1222624, 47784, 4096, { 64, 32 } },
   { "gfx/actors/bullet_bill/bullet_bill_mouth.rgba16.png", 1222624, 51880, 4096, { 64, 32 } },
   { "gfx/actors/heave_ho/heave-ho_arm_ornament.rgba16.png", 1222624, 68040, 2048, { 32, 32 } },
   { "gfx/actors/heave_ho/heave-ho_face.rgba16.png", 1222624, 59848, 2048, { 32, 32 } },
   { "gfx/actors/heave_ho/heave-ho_logo.rgba16.png", 1222624, 63944, 4096, { 64, 32 } },
   { "gfx/actors/heave_ho/heave-ho_platform.rgba16.png", 1222624, 61896, 2048, { 32, 32 } },
   { "gfx/actors/heave_ho/heave-ho_roller.rgba16.png", 1222624, 70088, 512, { 16, 16 } },
   { "gfx/actors/heave_ho/heave-ho_turnkey.rgba16.png", 1222624, 70600, 2048, { 32, 32 } },
   { "gfx/actors/hoot/hoot_eyes.rgba16.png", 1222624, 2592, 2048, { 32, 32 } },
   { "gfx/actors/hoot/hoot_wing.rgba16.png", 1222624, 7760, 2048, { 32, 32 } },
   { "gfx/actors/hoot/hoot_wing_tip.rgba16.png", 1222624, 9808, 2048, { 32, 32 } },
   { "gfx/actors/king_bobomb/king_bob-omb_hand.rgba16.png", 1222624, 64, 2048, { 32, 32 } },
   { "gfx/actors/thwomp/thwomp_face.rgba16.png", 1222624, 39168, 4096, { 32, 64 } },
   { "gfx/actors/thwomp/thwomp_surface.rgba16.png", 1222624, 43264, 2048, { 32, 32 } },
   { "gfx/actors/yellow_sphere/yellow_sphere.rgba16.png", 1222624, 64, 2048, { 32, 32 } },
   { "gfx/actors/yellow_sphere_small/small_yellow_sphere.rgba16.png", 1222624, 64, 2048, { 32, 32 } },
   { "gfx/actors/yoshi_egg/yoshi_egg_0_unused.rgba16.png", 1222624, 22456, 2048, { 32, 32 } },
   { "gfx/actors/yoshi_egg/yoshi_egg_1_unused.rgba16.png", 1222624, 24504, 2048, { 32, 32 } },
   { "gfx/actors/yoshi_egg/yoshi_egg_2_unused.rgba16.png", 1222624, 26552, 2048, { 32, 32 } },
   { "gfx/actors/yoshi_egg/yoshi_egg_3_unused.rgba16.png", 1222624, 28600, 2048, { 32, 32 } },
   { "gfx/actors/yoshi_egg/yoshi_egg_4_unused.rgba16.png", 1222624, 30648, 2048, { 32, 32 } },
   { "gfx/actors/yoshi_egg/yoshi_egg_5_unused.rgba16.png", 1222624, 32696, 2048, { 32, 32 } },
   { "gfx/actors/yoshi_egg/yoshi_egg_6_unused.rgba16.png", 1222624, 34744, 2048, { 32, 32 } },
   { "gfx/actors/yoshi_egg/yoshi_egg_7_unused.rgba16.png", 1222624, 36792, 2048, { 32, 32 } },
   { "gfx/actors/bully/bully_eye.rgba16.png", 1256544, 9320, 2048, { 32, 32 } },
   { "gfx/actors/bully/bully_horn.rgba16.png", 1256544, 224, 512, { 16, 16 } },
   { "gfx/actors/bully/bully_left_side.rgba16.png", 1256544, 1128, 4096, { 32, 64 } },
   { "gfx/actors/bully/bully_right_side.rgba16.png", 1256544, 5224, 4096, { 32, 64 } },
   { "gfx/actors/chillychief/chill_bully_eye.rgba16.png", 1256544, 9320, 2048, { 32, 32 } },
   { "gfx/actors/bobomb/bob-omb_buddy_left_side.rgba16.png", 1264928, 120, 4096, { 32, 64 } },
   { "gfx/actors/bobomb/bob-omb_buddy_right_side.rgba16.png", 1264928, 4216, 4096, { 32, 64 } },
   { "gfx/actors/bobomb/bob-omb_eyes.rgba16.png", 1264928, 18552, 2048, { 32, 32 } },
   { "gfx/actors/bobomb/bob-omb_eyes_blink.rgba16.png", 1264928, 20600, 2048, { 32, 32 } },
   { "gfx/actors/king_bobomb/bob-omb_buddy_body_unused.rgba16.png", 1264928, 25720, 8192, { 64, 64 } },
   { "gfx/actors/king_bobomb/bob-omb_buddy_left_side_unused.rgba16.png", 1264928, 120, 4096, { 32, 64 } },
   { "gfx/actors/king_bobomb/bob-omb_buddy_right_side_unused.rgba16.png", 1264928, 4216, 4096, { 32, 64 } },
   { "gfx/actors/king_bobomb/king_bob-omb_arm.rgba16.png", 1264928, 8312, 2048, { 32, 32 } },
   { "gfx/actors/king_bobomb/king_bob-omb_body_unused.rgba16.png", 1264928, 10360, 8192, { 64, 64 } },
   { "gfx/actors/king_bobomb/king_bob-omb_crown_rim.rgba16.png", 1264928, 24696, 1024, { 32, 16 } },
   { "gfx/actors/king_bobomb/king_bob-omb_eyes.rgba16.png", 1264928, 18552, 4096, { 32, 64 } },
   { "gfx/actors/king_bobomb/king_bob-omb_left_side.rgba16.png", 1264928, 33912, 4096, { 32, 64 } },
   { "gfx/actors/king_bobomb/king_bob-omb_right_side.rgba16.png", 1264928, 38008, 4096, { 32, 64 } },
   { "gfx/actors/water_bubble/water_bubble.rgba16.png", 1264928, 65152, 2048, { 32, 32 } },
   { "gfx/actors/clam_shell/clam_shell.rgba16.png", 1292560, 48, 2048, { 32, 32 } },
   { "gfx/actors/clam_shell/clam_shell_mouth.rgba16.png", 1292560, 2096, 2048, { 32, 32 } },
   { "gfx/actors/manta/manta_eye.rgba16.png", 1292560, 12192, 2048, { 32, 32 } },
   { "gfx/actors/manta/manta_fin_corner.rgba16.png", 1292560, 6048, 2048, { 32, 32 } },
   { "gfx/actors/manta/manta_fin_edge.rgba16.png", 1292560, 14240, 4096, { 64, 32 } },
   { "gfx/actors/manta/manta_gills.rgba16.png", 1292560, 8096, 4096, { 32, 64 } },
   { "gfx/actors/sushi/sushi_eye.rgba16.png", 1292560, 38608, 1024, { 32, 16 } },
   { "gfx/actors/sushi/sushi_snout.rgba16.png", 1292560, 36560, 2048, { 32, 32 } },
   { "gfx/actors/sushi/sushi_tooth.rgba16.png", 1292560, 39632, 128, { 8, 8 } },
   { "gfx/actors/tornado/tornado.ia16.png", 1292560, 75848, 4096, { 32, 64 } },
   { "gfx/actors/unagi/unagi_body.rgba16.png", 1292560, 44832, 2048, { 32, 32 } },
   { "gfx/actors/unagi/unagi_eye.rgba16.png", 1292560, 46880, 512, { 16, 16 } },
   { "gfx/actors/unagi/unagi_head_base.rgba16.png", 1292560, 47392, 2048, { 32, 32 } },
   { "gfx/actors/unagi/unagi_mouth.rgba16.png", 1292560, 49952, 128, { 8, 8 } },
   { "gfx/actors/unagi/unagi_tail.rgba16.png", 1292560, 50080, 2048, { 32, 32 } },
   { "gfx/actors/unagi/unagi_tooth.rgba16.png", 1292560, 49440, 512, { 16, 16 } },
   { "gfx/actors/whirlpool/whirlpool.ia16.png", 1292560, 75848, 4096, { 32, 64 } },
   { "gfx/actors/eyerok/eyerok_bricks.rgba16.png", 1334928, 36160, 2048, { 32, 32 } },
   { "gfx/actors/eyerok/eyerok_eye_closed.rgba16.png", 1334928, 44352, 2048, { 32, 32 } },
   { "gfx/actors/eyerok/eyerok_eye_mostly_closed.rgba16.png", 1334928, 42304, 2048, { 32, 32 } },
   { "gfx/actors/eyerok/eyerok_eye_mostly_open.rgba16.png", 1334928, 40256, 2048, { 32, 32 } },
   { "gfx/actors/eyerok/eyerok_eye_open.rgba16.png", 1334928, 38208, 2048, { 32, 32 } },
   { "gfx/actors/klepto/klepto_beak.rgba16.png", 1334928, 4104, 4096, { 32, 64 } },
   { "gfx/actors/klepto/klepto_chest_tuft.rgba16.png", 1334928, 8, 2048, { 32, 32 } },
   { "gfx/actors/klepto/klepto_eye.rgba16.png", 1334928, 2056, 2048, { 32, 32 } },
   { "gfx/actors/klepto/klepto_wing.rgba16.png", 1334928, 8200, 4096, { 64, 32 } },
   { "gfx/actors/klepto/klepto_wing_flap.rgba16.png", 1334928, 12296, 2048, { 32, 32 } },
   { "gfx/actors/pokey/pokey_body.rgba16.png", 1334928, 75896, 2048, { 32, 32 } },
   { "gfx/actors/pokey/pokey_face.rgba16.png", 1334928, 71504, 2048, { 32, 32 } },
   { "gfx/actors/pokey/pokey_face_blink.rgba16.png", 1334928, 73552, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.00800.rgba16.png", 1334928, 36160, 2048, { 32, 32 } },
   { "gfx/actors/fwoosh/fwoosh_face.ia16.png", 1384912, 88072, 2048, { 32, 32 } },
   { "gfx/actors/monty_mole/monty_mole_cheek.rgba16.png", 1384912, 2416, 2048, { 32, 32 } },
   { "gfx/actors/monty_mole/monty_mole_claw.rgba16.png", 1384912, 10608, 2048, { 32, 32 } },
   { "gfx/actors/monty_mole/monty_mole_eye.rgba16.png", 1384912, 4464, 2048, { 32, 32 } },
   { "gfx/actors/monty_mole/monty_mole_nose.rgba16.png", 1384912, 6512, 2048, { 32, 32 } },
   { "gfx/actors/monty_mole/monty_mole_tooth.rgba16.png", 1384912, 8560, 2048, { 32, 32 } },
   { "gfx/actors/monty_mole_hole/monty_mole_hole.ia16.png", 1384912, 64, 2048, { 32, 32 } },
   { "gfx/actors/smoke/smoke.ia16.png", 1384912, 29376, 2048, { 32, 32 } },
   { "gfx/actors/ukiki/ukiki_butt.rgba16.png", 1384912, 39872, 2048, { 32, 32 } },
   { "gfx/actors/ukiki/ukiki_face.rgba16.png", 1384912, 31680, 4096, { 64, 32 } },
   { "gfx/actors/ukiki/ukiki_face_blink.rgba16.png", 1384912, 35776, 4096, { 64, 32 } },
   { "gfx/actors/ukiki/ukiki_fur.rgba16.png", 1384912, 41920, 2048, { 32, 32 } },
   { "gfx/actors/penguin/penguin_beak.rgba16.png", 1443440, 21984, 2048, { 32, 32 } },
   { "gfx/actors/penguin/penguin_eye_angry.rgba16.png", 1443440, 17888, 2048, { 32, 32 } },
   { "gfx/actors/penguin/penguin_eye_angry_unused.rgba16.png", 1443440, 19936, 2048, { 32, 32 } },
   { "gfx/actors/penguin/penguin_eye_closed.rgba16.png", 1443440, 15840, 2048, { 32, 32 } },
   { "gfx/actors/penguin/penguin_eye_half_closed.rgba16.png", 1443440, 13792, 2048, { 32, 32 } },
   { "gfx/actors/penguin/penguin_eye_open.rgba16.png", 1443440, 11744, 2048, { 32, 32 } },
   { "gfx/actors/snowman/mr_blizzard_eye.rgba16.png", 1443440, 46192, 2048, { 32, 32 } },
   { "gfx/actors/snowman/mr_blizzard_left_side.rgba16.png", 1443440, 38000, 4096, { 32, 64 } },
   { "gfx/actors/snowman/mr_blizzard_mitten.rgba16.png", 1443440, 35952, 2048, { 32, 32 } },
   { "gfx/actors/snowman/mr_blizzard_mouth.rgba16.png", 1443440, 48240, 2048, { 32, 32 } },
   { "gfx/actors/snowman/mr_blizzard_right_side.rgba16.png", 1443440, 42096, 4096, { 32, 64 } },
   { "gfx/actors/spindrift/spindrift_face.rgba16.png", 1443440, 1744, 2048, { 32, 32 } },
   { "gfx/actors/spindrift/spindrift_head.rgba16.png", 1443440, 7888, 2048, { 32, 32 } },
   { "gfx/actors/spindrift/spindrift_leaf.rgba16.png", 1443440, 5840, 2048, { 32, 32 } },
   { "gfx/actors/spindrift/spindrift_petal.rgba16.png", 1443440, 3792, 2048, { 32, 32 } },
   { "gfx/levels/ccm/5.rgba16.png", 1443440, 48240, 2048, { 32, 32 } },
   { "gfx/levels/ccm/6.rgba16.png", 1443440, 46192, 2048, { 32, 32 } },
   { "gfx/actors/capswitch/cap_switch_base.rgba16.png", 1464912, 11336, 128, { 16, 4 } },
   { "gfx/actors/capswitch/cap_switch_head.ia16.png", 1464912, 7240, 4096, { 32, 64 } },
   { "gfx/actors/springboard/springboard_base_unused.rgba16.png", 1464912, 2072, 2048, { 32, 32 } },
   { "gfx/actors/springboard/springboard_top_unused.rgba16.png", 1464912, 24, 2048, { 32, 32 } },
   { "gfx/actors/boo/boo_eyes.rgba16.png", 1469536, 39744, 4096, { 64, 32 } },
   { "gfx/actors/boo/boo_mouth.rgba16.png", 1469536, 43840, 2048, { 32, 32 } },
   { "gfx/actors/boo_castle/bbh_boo_eyes.rgba16.png", 1469536, 39744, 4096, { 64, 32 } },
   { "gfx/actors/boo_castle/bbh_boo_mouth.rgba16.png", 1469536, 43840, 2048, { 32, 32 } },
   { "gfx/actors/book/book_cover.rgba16.png", 1469536, 4192, 2048, { 32, 32 } },
   { "gfx/actors/bookend/bookend_cover.rgba16.png", 1469536, 4192, 2048, { 32, 32 } },
   { "gfx/actors/bookend/bookend_mouth.rgba16.png", 1469536, 2144, 1024, { 16, 32 } },
   { "gfx/actors/bookend/bookend_pages.rgba16.png", 1469536, 3168, 1024, { 16, 32 } },
   { "gfx/actors/bookend/bookend_spine.rgba16.png", 1469536, 96, 1024, { 16, 32 } },
   { "gfx/actors/bookend/bookend_tooth.rgba16.png", 1469536, 1120, 1024, { 16, 32 } },
   { "gfx/actors/chain_chomp/chain_chomp_tooth.rgba16.png", 1469536, 27376, 2048, { 32, 32 } },
   { "gfx/actors/chair/chair_bottom.rgba16.png", 1469536, 16480, 1024, { 32, 16 } },
   { "gfx/actors/chair/chair_front.rgba16.png", 1469536, 12384, 2048, { 32, 32 } },
   { "gfx/actors/chair/chair_leg.rgba16.png", 1469536, 14432, 2048, { 32, 32 } },
   { "gfx/actors/chair/chair_surface_unused.rgba16.png", 1469536, 17504, 2048, { 32, 32 } },
   { "gfx/actors/haunted_cage/bbh_cage_bars.rgba16.png", 1469536, 55944, 2048, { 32, 32 } },
   { "gfx/actors/haunted_cage/bbh_cage_double_ornament.rgba16.png", 1469536, 51848, 2048, { 32, 32 } },
   { "gfx/actors/haunted_cage/bbh_cage_floor.rgba16.png", 1469536, 49800, 2048, { 32, 32 } },
   { "gfx/actors/haunted_cage/bbh_cage_garbage.rgba16.png", 1469536, 57992, 2048, { 32, 32 } },
   { "gfx/actors/haunted_cage/bbh_cage_ornament.rgba16.png", 1469536, 53896, 1024, { 32, 16 } },
   { "gfx/actors/haunted_cage/bbh_cage_wooden_base.rgba16.png", 1469536, 54920, 1024, { 32, 16 } },
   { "gfx/actors/mad_piano/mad_piano_body.rgba16.png", 1469536, 29424, 1024, { 16, 32 } },
   { "gfx/actors/mad_piano/mad_piano_keys.rgba16.png", 1469536, 32496, 1024, { 32, 16 } },
   { "gfx/actors/mad_piano/mad_piano_keys_corner.rgba16.png", 1469536, 30448, 1024, { 32, 16 } },
   { "gfx/actors/mad_piano/mad_piano_keys_edge.rgba16.png", 1469536, 33520, 1024, { 32, 16 } },
   { "gfx/actors/mad_piano/mad_piano_mouth.rgba16.png", 1469536, 31472, 1024, { 16, 32 } },
   { "gfx/actors/mad_piano/mad_piano_tooth.rgba16.png", 1469536, 27376, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.0A000.rgba16.png", 1469536, 49800, 2048, { 32, 32 } },
   { "gfx/actors/peach/peach_chest_jewel.rgba16.png", 1497200, 11304, 512, { 16, 16 } },
   { "gfx/actors/peach/peach_crown_jewel.rgba16.png", 1497200, 10792, 512, { 16, 16 } },
   { "gfx/actors/peach/peach_dress.rgba16.png", 1497200, 16424, 2048, { 32, 32 } },
   { "gfx/actors/peach/peach_eye_closed.rgba16.png", 1497200, 8744, 2048, { 32, 32 } },
   { "gfx/actors/peach/peach_eye_mostly_closed.rgba16.png", 1497200, 6696, 2048, { 32, 32 } },
   { "gfx/actors/peach/peach_eye_mostly_open.rgba16.png", 1497200, 4648, 2048, { 32, 32 } },
   { "gfx/actors/peach/peach_eye_open.rgba16.png", 1497200, 2600, 2048, { 32, 32 } },
   { "gfx/actors/peach/peach_lips.rgba16.png", 1497200, 13864, 2048, { 32, 32 } },
   { "gfx/actors/peach/peach_lips_scrunched.rgba16.png", 1497200, 11816, 2048, { 32, 32 } },
   { "gfx/actors/peach/peach_nostril.rgba16.png", 1497200, 15912, 512, { 16, 16 } },
   { "gfx/actors/yoshi/yoshi_eye.rgba16.png", 1497200, 116104, 512, { 16, 16 } },
   { "gfx/actors/yoshi/yoshi_eye_blink.rgba16.png", 1497200, 116616, 512, { 16, 16 } },
   { "gfx/actors/yoshi/yoshi_nostril.rgba16.png", 1497200, 117128, 512, { 16, 16 } },
   { "gfx/actors/bub/bub_eye_border.rgba16.png", 1575856, 5128, 2048, { 32, 32 } },
   { "gfx/actors/bub/bub_eyes.rgba16.png", 1575856, 1032, 4096, { 64, 32 } },
   { "gfx/actors/bub/bub_fins.rgba16.png", 1575856, 7176, 2048, { 32, 32 } },
   { "gfx/actors/bub/bub_scales.rgba16.png", 1575856, 9224, 4096, { 64, 32 } },
   { "gfx/actors/bubba/bubba_eye_border.rgba16.png", 1575856, 5128, 2048, { 32, 32 } },
   { "gfx/actors/bubba/bubba_eyes_unused.rgba16.png", 1575856, 1032, 4096, { 64, 32 } },
   { "gfx/actors/bubba/bubba_fins.rgba16.png", 1575856, 7176, 2048, { 32, 32 } },
   { "gfx/actors/bubba/bubba_scales.rgba16.png", 1575856, 9224, 8192, { 64, 64 } },
   { "gfx/actors/bubba/bubba_sunglasses.rgba16.png", 1575856, 8, 1024, { 16, 32 } },
   { "gfx/actors/lakitu_cameraman/lakitu_cameraman_cloud_face_unused.rgba16.png", 1575856, 60640, 2048, { 32, 32 } },
   { "gfx/actors/lakitu_cameraman/lakitu_cameraman_eyes_closed.rgba16.png", 1575856, 66784, 4096, { 64, 32 } },
   { "gfx/actors/lakitu_cameraman/lakitu_cameraman_eyes_open.rgba16.png", 1575856, 62688, 4096, { 64, 32 } },
   { "gfx/actors/lakitu_cameraman/lakitu_cameraman_frown.rgba16.png", 1575856, 72928, 2048, { 32, 32 } },
   { "gfx/actors/lakitu_cameraman/lakitu_cameraman_shell.rgba16.png", 1575856, 70880, 2048, { 32, 32 } },
   { "gfx/actors/lakitu_enemy/lakitu_enemy_cloud_face_unused.rgba16.png", 1575856, 60640, 2048, { 32, 32 } },
   { "gfx/actors/lakitu_enemy/lakitu_enemy_eyes_closed.rgba16.png", 1575856, 66784, 4096, { 64, 32 } },
   { "gfx/actors/lakitu_enemy/lakitu_enemy_eyes_open.rgba16.png", 1575856, 62688, 4096, { 64, 32 } },
   { "gfx/actors/lakitu_enemy/lakitu_enemy_frown.rgba16.png", 1575856, 72928, 2048, { 32, 32 } },
   { "gfx/actors/lakitu_enemy/lakitu_enemy_shell.rgba16.png", 1575856, 70880, 2048, { 32, 32 } },
   { "gfx/actors/star/star_eye.rgba16.png", 1575856, 31280, 2048, { 32, 32 } },
   { "gfx/actors/wiggler/wiggler_eye.rgba16.png", 1575856, 31280, 2048, { 32, 32 } },
   { "gfx/actors/wiggler/wiggler_flower.rgba16.png", 1575856, 33328, 2048, { 32, 32 } },
   { "gfx/actors/wiggler/wiggler_frown.rgba16.png", 1575856, 35376, 2048, { 32, 32 } },
   { "gfx/actors/wiggler/wiggler_nose_left_side.rgba16.png", 1575856, 37424, 4096, { 32, 64 } },
   { "gfx/actors/wiggler/wiggler_nose_right_side.rgba16.png", 1575856, 41520, 4096, { 32, 64 } },
   { "gfx/actors/wiggler/wiggler_segment_left_side.rgba16.png", 1575856, 23088, 4096, { 32, 64 } },
   { "gfx/actors/wiggler/wiggler_segment_right_side.rgba16.png", 1575856, 27184, 4096, { 32, 64 } },
   { "gfx/actors/bomb/bomb_left_side.rgba16.png", 1606720, 359440, 4096, { 32, 64 } },
   { "gfx/actors/bomb/bomb_right_side.rgba16.png", 1606720, 363536, 4096, { 32, 64 } },
   { "gfx/actors/bomb/bomb_spike.rgba16.png", 1606720, 367632, 2048, { 32, 32 } },
   { "gfx/actors/bowser/bowser_armband.rgba16.png", 1606720, 146488, 2048, { 32, 32 } },
   { "gfx/actors/bowser/bowser_armband_spike.rgba16.png", 1606720, 142392, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_blue_eye_unused.rgba16.png", 1606720, 156728, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_body.rgba16.png", 1606720, 140344, 2048, { 32, 32 } },
   { "gfx/actors/bowser/bowser_chest.rgba16.png", 1606720, 152632, 2048, { 32, 32 } },
   { "gfx/actors/bowser/bowser_claw_edge.rgba16.png", 1606720, 171064, 4096, { 32, 64 } },
   { "gfx/actors/bowser/bowser_claw_horn_angle.rgba16.png", 1606720, 179256, 4096, { 32, 64 } },
   { "gfx/actors/bowser/bowser_claw_horn_tooth.rgba16.png", 1606720, 175160, 4096, { 32, 64 } },
   { "gfx/actors/bowser/bowser_eye_center_0.rgba16.png", 1606720, 195640, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_center_1.rgba16.png", 1606720, 195640, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_closed_0.rgba16.png", 1606720, 191544, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_closed_1.rgba16.png", 1606720, 191544, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_far_left_0.rgba16.png", 1606720, 203832, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_far_left_1.rgba16.png", 1606720, 203832, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_half_closed_0.rgba16.png", 1606720, 187448, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_half_closed_1.rgba16.png", 1606720, 187448, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_left_0.rgba16.png", 1606720, 183352, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_left_1.rgba16.png", 1606720, 183352, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_right_0.rgba16.png", 1606720, 199736, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eye_right_1.rgba16.png", 1606720, 199736, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_eyebrow.rgba16.png", 1606720, 130104, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_hair.rgba16.png", 1606720, 166968, 4096, { 32, 64 } },
   { "gfx/actors/bowser/bowser_mouth_unused.rgba16.png", 1606720, 160824, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_muzzle.rgba16.png", 1606720, 134200, 2048, { 32, 32 } },
   { "gfx/actors/bowser/bowser_nostrils.rgba16.png", 1606720, 136248, 4096, { 64, 32 } },
   { "gfx/actors/bowser/bowser_shell.rgba16.png", 1606720, 128056, 2048, { 32, 32 } },
   { "gfx/actors/bowser/bowser_shell_edge.rgba16.png", 1606720, 154680, 2048, { 32, 32 } },
   { "gfx/actors/bowser/bowser_tongue.rgba16.png", 1606720, 148536, 4096, { 32, 64 } },
   { "gfx/actors/bowser/bowser_upper_face.rgba16.png", 1606720, 164920, 2048, { 32, 32 } },
   { "gfx/actors/bowser_flame/bowser_flame_0.rgba16.png", 1606720, 0, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_1.rgba16.png", 1606720, 8192, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_10.rgba16.png", 1606720, 81920, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_11.rgba16.png", 1606720, 90112, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_12.rgba16.png", 1606720, 98304, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_13.rgba16.png", 1606720, 106496, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_2.rgba16.png", 1606720, 16384, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_3.rgba16.png", 1606720, 24576, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_4.rgba16.png", 1606720, 32768, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_5.rgba16.png", 1606720, 40960, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_6.rgba16.png", 1606720, 49152, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_7.rgba16.png", 1606720, 57344, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_8.rgba16.png", 1606720, 65536, 8192, { 64, 64 } },
   { "gfx/actors/bowser_flame/bowser_flame_9.rgba16.png", 1606720, 73728, 8192, { 64, 64 } },
   { "gfx/actors/impact_ring/impact_ring_left_side.ia16.png", 1606720, 117328, 4096, { 32, 64 } },
   { "gfx/actors/impact_ring/impact_ring_right_side.ia16.png", 1606720, 121424, 4096, { 32, 64 } },
   { "gfx/actors/impact_smoke/impact_smoke_0.ia16.png", 1606720, 371576, 8192, { 64, 64 } },
   { "gfx/actors/impact_smoke/impact_smoke_1.ia16.png", 1606720, 379768, 8192, { 64, 64 } },
   { "gfx/actors/impact_smoke/impact_smoke_2.ia16.png", 1606720, 387960, 8192, { 64, 64 } },
   { "gfx/actors/impact_smoke/impact_smoke_3.ia16.png", 1606720, 396152, 8192, { 64, 64 } },
   { "gfx/actors/piranha_plant/piranha_plant_tongue.rgba16.png", 1606720, 148536, 4096, { 32, 64 } },
   { "gfx/actors/water_mine/water_mine_left_side_unused.rgba16.png", 1606720, 359440, 4096, { 32, 64 } },
   { "gfx/actors/water_mine/water_mine_right_side_unused.rgba16.png", 1606720, 363536, 4096, { 32, 64 } },
   { "gfx/actors/water_mine/water_mine_spike_unused.rgba16.png", 1606720, 367632, 2048, { 32, 32 } },
   { "gfx/textures/segment2/font_graphics.06410.ia1.png", 1606720, 83574, 16, { 16, 8 } },
   { "gfx/actors/cyan_fish/cyan_fish.rgba16.png", 1809600, 54376, 2048, { 32, 32 } },
   { "gfx/actors/seaweed/seaweed_base.rgba16.png", 1809600, 38416, 2048, { 32, 32 } },
   { "gfx/actors/seaweed/seaweed_lower_center.rgba16.png", 1809600, 36368, 2048, { 32, 32 } },
   { "gfx/actors/seaweed/seaweed_tip.rgba16.png", 1809600, 32272, 2048, { 32, 32 } },
   { "gfx/actors/seaweed/seaweed_upper_center.rgba16.png", 1809600, 34320, 2048, { 32, 32 } },
   { "gfx/actors/skeeter/skeeter_eye.rgba16.png", 1809600, 144, 2048, { 32, 32 } },
   { "gfx/actors/skeeter/skeeter_iris.rgba16.png", 1809600, 2192, 256, { 16, 8 } },
   { "gfx/actors/treasure_chest/treasure_chest_front.rgba16.png", 1809600, 87976, 4096, { 64, 32 } },
   { "gfx/actors/treasure_chest/treasure_chest_lock.rgba16.png", 1809600, 81832, 2048, { 32, 32 } },
   { "gfx/actors/treasure_chest/treasure_chest_lock_top.rgba16.png", 1809600, 85928, 2048, { 32, 32 } },
   { "gfx/actors/treasure_chest/treasure_chest_side.rgba16.png", 1809600, 83880, 2048, { 32, 32 } },
   { "gfx/actors/water_ring/water_ring.rgba16.png", 1809600, 74624, 4096, { 64, 32 } },
   { "gfx/actors/chain_ball/chain_ball.rgba16.png", 1851952, 133864, 2048, { 32, 32 } },
   { "gfx/actors/chain_chomp/chain_chomp_bright_shine.rgba16.png", 1851952, 136144, 2048, { 32, 32 } },
   { "gfx/actors/chain_chomp/chain_chomp_dull_shine.rgba16.png", 1851952, 138192, 2048, { 32, 32 } },
   { "gfx/actors/chain_chomp/chain_chomp_eye.rgba16.png", 1851952, 144336, 2048, { 32, 32 } },
   { "gfx/actors/chain_chomp/chain_chomp_tongue.rgba16.png", 1851952, 140240, 2048, { 32, 32 } },
   { "gfx/actors/dorrie/dorrie_tongue.rgba16.png", 1851952, 140240, 2048, { 32, 32 } },
   { "gfx/actors/koopa/koopa_eye_border.rgba16.png", 1851952, 22088, 2048, { 32, 32 } },
   { "gfx/actors/koopa/koopa_eyes_closed.rgba16.png", 1851952, 20040, 2048, { 32, 32 } },
   { "gfx/actors/koopa/koopa_eyes_open.rgba16.png", 1851952, 17992, 2048, { 32, 32 } },
   { "gfx/actors/koopa/koopa_nostrils.rgba16.png", 1851952, 24136, 4096, { 64, 32 } },
   { "gfx/actors/koopa/koopa_shell_back.rgba16.png", 1851952, 11848, 2048, { 32, 32 } },
   { "gfx/actors/koopa/koopa_shell_front.rgba16.png", 1851952, 9800, 2048, { 32, 32 } },
   { "gfx/actors/koopa/koopa_shell_front_top.rgba16.png", 1851952, 15944, 2048, { 32, 32 } },
   { "gfx/actors/koopa/koopa_shoe.rgba16.png", 1851952, 13896, 2048, { 32, 32 } },
   { "gfx/actors/koopa_flag/koopa_flag_banner.rgba16.png", 1851952, 72, 2048, { 32, 32 } },
   { "gfx/actors/koopa_shell/koopa_shell_back.rgba16.png", 1851952, 11848, 2048, { 32, 32 } },
   { "gfx/actors/koopa_shell/koopa_shell_front.rgba16.png", 1851952, 9800, 2048, { 32, 32 } },
   { "gfx/actors/piranha_plant/piranha_plant_bottom_lip.rgba16.png", 1851952, 78840, 2048, { 32, 32 } },
   { "gfx/actors/piranha_plant/piranha_plant_leaf.rgba16.png", 1851952, 82936, 4096, { 32, 64 } },
   { "gfx/actors/piranha_plant/piranha_plant_skin.rgba16.png", 1851952, 74744, 2048, { 32, 32 } },
   { "gfx/actors/piranha_plant/piranha_plant_stem.rgba16.png", 1851952, 76792, 2048, { 32, 32 } },
   { "gfx/actors/piranha_plant/piranha_plant_tooth.rgba16.png", 1851952, 80888, 2048, { 32, 32 } },
   { "gfx/actors/poundable_pole/poundable_pole_side.rgba16.png", 1851952, 6224, 2048, { 32, 32 } },
   { "gfx/actors/poundable_pole/poundable_pole_top.rgba16.png", 1851952, 4176, 2048, { 32, 32 } },
   { "gfx/actors/whomp/whomp_back.rgba16.png", 1851952, 115552, 4096, { 32, 64 } },
   { "gfx/actors/whomp/whomp_face.rgba16.png", 1851952, 119648, 4096, { 32, 64 } },
   { "gfx/actors/whomp/whomp_hand.rgba16.png", 1851952, 123744, 2048, { 32, 32 } },
   { "gfx/actors/whomp/whomp_surface.rgba16.png", 1851952, 125792, 2048, { 32, 32 } },
   { "gfx/textures/segment2/font_graphics.06420.ia1.png", 1851952, 133979, 16, { 16, 8 } },
   { "gfx/actors/lakitu_cameraman/lakitu_camera_lens.rgba16.png", 1934096, 14336, 512, { 16, 16 } },
   { "gfx/actors/mips/mips_eyes.rgba16.png", 1934096, 64624, 2048, { 32, 32 } },
   { "gfx/actors/toad/toad_face.rgba16.png", 1934096, 22816, 2048, { 32, 32 } },
   { "gfx/actors/toad/toad_head.rgba16.png", 1934096, 24864, 2048, { 32, 32 } },
   { "gfx/actors/chillychief/chill_bully_left_side.rgba16.png", 1987056, 96, 4096, { 32, 64 } },
   { "gfx/actors/chillychief/chill_bully_right_side.rgba16.png", 1987056, 4192, 4096, { 32, 64 } },
   { "gfx/actors/moneybag/moneybag_eyes.rgba16.png", 1987056, 18864, 2048, { 32, 32 } },
   { "gfx/actors/moneybag/moneybag_mouth.rgba16.png", 1987056, 14768, 4096, { 64, 32 } },
   { "gfx/actors/dorrie/dorrie_eye.rgba16.png", 1998560, 39840, 512, { 16, 16 } },
   { "gfx/actors/dorrie/dorrie_skin.rgba16.png", 1998560, 40352, 4096, { 32, 64 } },
   { "gfx/actors/mr_i_eyeball/mr_i_eyeball_left_side.rgba16.png", 1998560, 128, 4096, { 32, 64 } },
   { "gfx/actors/mr_i_eyeball/mr_i_eyeball_right_side.rgba16.png", 1998560, 4224, 4096, { 32, 64 } },
   { "gfx/actors/mr_i_iris/mr_i_iris_closed.rgba16.png", 1998560, 14704, 2048, { 32, 32 } },
   { "gfx/actors/mr_i_iris/mr_i_iris_mostly_closed.rgba16.png", 1998560, 12656, 2048, { 32, 32 } },
   { "gfx/actors/mr_i_iris/mr_i_iris_mostly_open.rgba16.png", 1998560, 10608, 2048, { 32, 32 } },
   { "gfx/actors/mr_i_iris/mr_i_iris_open.rgba16.png", 1998560, 8560, 2048, { 32, 32 } },
   { "gfx/actors/scuttlebug/scuttlebug_eye.rgba16.png", 1998560, 65800, 2048, { 32, 32 } },
   { "gfx/actors/scuttlebug/scuttlebug_iris.rgba16.png", 1998560, 76040, 2048, { 32, 32 } },
   { "gfx/actors/scuttlebug/scuttlebug_left_side.rgba16.png", 1998560, 67848, 4096, { 32, 64 } },
   { "gfx/actors/scuttlebug/scuttlebug_leg.rgba16.png", 1998560, 78088, 2048, { 32, 32 } },
   { "gfx/actors/scuttlebug/scuttlebug_right_side.rgba16.png", 1998560, 71944, 4096, { 32, 64 } },
   { "gfx/actors/snufit/snufit_body.rgba16.png", 1998560, 28896, 2048, { 32, 32 } },
   { "gfx/actors/snufit/snufit_eye.rgba16.png", 1998560, 30944, 2048, { 32, 32 } },
   { "gfx/actors/snufit/snufit_mask_strap.rgba16.png", 1998560, 32992, 1024, { 16, 32 } },
   { "gfx/actors/snufit/snufit_mouth.rgba16.png", 1998560, 34016, 2048, { 32, 32 } },
   { "gfx/actors/swoop/swoop_body.rgba16.png", 1998560, 17008, 2048, { 32, 32 } },
   { "gfx/actors/swoop/swoop_eye.rgba16.png", 1998560, 19056, 2048, { 32, 32 } },
   { "gfx/actors/swoop/swoop_nose.rgba16.png", 1998560, 21104, 2048, { 32, 32 } },
   { "gfx/actors/swoop/swoop_wing.rgba16.png", 1998560, 23152, 2048, { 32, 32 } },
   { "gfx/actors/amp/amp_body.rgba16.png", 2040320, 6936, 2048, { 32, 32 } },
   { "gfx/actors/amp/amp_electricity.rgba16.png", 2040320, 3864, 1024, { 16, 32 } },
   { "gfx/actors/amp/amp_eyes.rgba16.png", 2040320, 4888, 2048, { 32, 32 } },
   { "gfx/actors/amp/amp_mouth.rgba16.png", 2040320, 8984, 2048, { 32, 32 } },
   { "gfx/actors/blue_coin_switch/blue_coin_switch_side.rgba16.png", 2040320, 24, 1024, { 32, 16 } },
   { "gfx/actors/blue_coin_switch/blue_coin_switch_top.rgba16.png", 2040320, 1048, 2048, { 32, 32 } },
   { "gfx/actors/bobomb/bob-omb_left_side.rgba16.png", 2040320, 121440, 4096, { 32, 64 } },
   { "gfx/actors/bobomb/bob-omb_right_side.rgba16.png", 2040320, 125536, 4096, { 32, 64 } },
   { "gfx/actors/breakable_box/cork_box_surface.rgba16.png", 2040320, 74384, 2048, { 32, 32 } },
   { "gfx/actors/breakable_box/crazy_box_surface.rgba16.png", 2040320, 72336, 2048, { 32, 32 } },
   { "gfx/actors/cannon_barrel/cannon_barrel.rgba16.png", 2040320, 22696, 2048, { 32, 32 } },
   { "gfx/actors/cannon_base/cannon_base.rgba16.png", 2040320, 18872, 2048, { 32, 32 } },
   { "gfx/actors/cannon_lid/cannon_lid.rgba16.png", 2040320, 16472, 2048, { 32, 32 } },
   { "gfx/actors/checkerboard_platform/checkerboard_platform.rgba16.png", 2040320, 52288, 2048, { 32, 32 } },
   { "gfx/actors/checkerboard_platform/checkerboard_platform_side.rgba16.png", 2040320, 51264, 1024, { 32, 16 } },
   { "gfx/actors/chuckya/chuckya_body_arm_left_side.rgba16.png", 2040320, 32632, 4096, { 32, 64 } },
   { "gfx/actors/chuckya/chuckya_body_arm_right_side.rgba16.png", 2040320, 36728, 4096, { 32, 64 } },
   { "gfx/actors/chuckya/chuckya_eyes.rgba16.png", 2040320, 26488, 4096, { 32, 64 } },
   { "gfx/actors/chuckya/chuckya_hand_antenna.rgba16.png", 2040320, 30584, 2048, { 32, 32 } },
   { "gfx/actors/exclamation_box/exclamation_box_front.rgba16.png", 2040320, 95784, 2048, { 32, 32 } },
   { "gfx/actors/exclamation_box/exclamation_box_side.rgba16.png", 2040320, 97832, 4096, { 64, 32 } },
   { "gfx/actors/exclamation_box/metal_cap_box_front.rgba16.png", 2040320, 83496, 2048, { 32, 32 } },
   { "gfx/actors/exclamation_box/metal_cap_box_side.rgba16.png", 2040320, 85544, 4096, { 64, 32 } },
   { "gfx/actors/exclamation_box/vanish_cap_box_front.rgba16.png", 2040320, 77352, 2048, { 32, 32 } },
   { "gfx/actors/exclamation_box/vanish_cap_box_side.rgba16.png", 2040320, 79400, 4096, { 32, 64 } },
   { "gfx/actors/exclamation_box/wing_cap_box_front.rgba16.png", 2040320, 89640, 2048, { 32, 32 } },
   { "gfx/actors/exclamation_box/wing_cap_box_side.rgba16.png", 2040320, 91688, 4096, { 32, 64 } },
   { "gfx/actors/exclamation_box_outline/exclamation_box_outline.rgba16.png", 2040320, 151912, 2048, { 32, 32 } },
   { "gfx/actors/exclamation_box_outline/exclamation_point.rgba16.png", 2040320, 154240, 1024, { 16, 32 } },
   { "gfx/actors/flyguy/flyguy_cloth_wrinkle.rgba16.png", 2040320, 57480, 4096, { 64, 32 } },
   { "gfx/actors/flyguy/flyguy_face.rgba16.png", 2040320, 61576, 2048, { 32, 32 } },
   { "gfx/actors/flyguy/flyguy_propeller.ia16.png", 2040320, 63624, 2048, { 32, 32 } },
   { "gfx/actors/goomba/goomba_body.rgba16.png", 2040320, 103728, 2048, { 32, 32 } },
   { "gfx/actors/goomba/goomba_face.rgba16.png", 2040320, 105776, 2048, { 32, 32 } },
   { "gfx/actors/goomba/goomba_face_blink.rgba16.png", 2040320, 107824, 2048, { 32, 32 } },
   { "gfx/actors/heart/spinning_heart.rgba16.png", 2040320, 55264, 2048, { 32, 32 } },
   { "gfx/actors/metal_box/metal_box_side.rgba16.png", 2040320, 145816, 4096, { 32, 64 } },
   { "gfx/actors/purple_switch/purple_switch_base.rgba16.png", 2040320, 49320, 128, { 16, 4 } },
   { "gfx/actors/purple_switch/purple_switch_exclamation_point.rgba16.png", 2040320, 49448, 1024, { 16, 32 } },
   { "gfx/actors/blue_fish/blue_fish.rgba16.png", 2102288, 112096, 2048, { 32, 32 } },
   { "gfx/actors/butterfly/butterfly_wing.rgba16.png", 2102288, 17320, 4096, { 32, 64 } },
   { "gfx/actors/coin/coin_front.ia16.png", 2102288, 22400, 2048, { 32, 32 } },
   { "gfx/actors/coin/coin_side.ia16.png", 2102288, 26496, 2048, { 32, 32 } },
   { "gfx/actors/coin/coin_tilt_left.ia16.png", 2102288, 28544, 2048, { 32, 32 } },
   { "gfx/actors/coin/coin_tilt_right.ia16.png", 2102288, 24448, 2048, { 32, 32 } },
   { "gfx/actors/dirt/dirt_particle.rgba16.png", 2102288, 179704, 512, { 16, 16 } },
   { "gfx/actors/door/bbh_door.rgba16.png", 2102288, 66832, 4096, { 32, 64 } },
   { "gfx/actors/door/bbh_door_overlay.rgba16.png", 2102288, 70928, 2048, { 32, 32 } },
   { "gfx/actors/door/door_lock.rgba16.png", 2102288, 79120, 1024, { 16, 32 } },
   { "gfx/actors/door/hmc_mural_door.rgba16.png", 2102288, 60688, 4096, { 32, 64 } },
   { "gfx/actors/door/hmc_mural_door_overlay.rgba16.png", 2102288, 64784, 2048, { 32, 32 } },
   { "gfx/actors/door/metal_door.rgba16.png", 2102288, 54544, 4096, { 32, 64 } },
   { "gfx/actors/door/metal_door_overlay.rgba16.png", 2102288, 58640, 2048, { 32, 32 } },
   { "gfx/actors/door/one_star_door_sign.rgba16.png", 2102288, 75024, 2048, { 32, 32 } },
   { "gfx/actors/door/polished_wooden_door.rgba16.png", 2102288, 40208, 4096, { 32, 64 } },
   { "gfx/actors/door/polished_wooden_door_overlay.rgba16.png", 2102288, 44304, 4096, { 32, 64 } },
   { "gfx/actors/door/rough_wooden_door.rgba16.png", 2102288, 48400, 4096, { 32, 64 } },
   { "gfx/actors/door/rough_wooden_door_overlay.rgba16.png", 2102288, 52496, 2048, { 32, 32 } },
   { "gfx/actors/door/three_star_door_sign.rgba16.png", 2102288, 77072, 2048, { 32, 32 } },
   { "gfx/actors/door/zero_star_door_sign.rgba16.png", 2102288, 72976, 2048, { 32, 32 } },
   { "gfx/actors/explosion/explosion_0.rgba16.png", 2102288, 2568, 2048, { 32, 32 } },
   { "gfx/actors/explosion/explosion_1.rgba16.png", 2102288, 4616, 2048, { 32, 32 } },
   { "gfx/actors/explosion/explosion_2.rgba16.png", 2102288, 6664, 2048, { 32, 32 } },
   { "gfx/actors/explosion/explosion_3.rgba16.png", 2102288, 8712, 2048, { 32, 32 } },
   { "gfx/actors/explosion/explosion_4.rgba16.png", 2102288, 10760, 2048, { 32, 32 } },
   { "gfx/actors/explosion/explosion_5.rgba16.png", 2102288, 12808, 2048, { 32, 32 } },
   { "gfx/actors/explosion/explosion_6.rgba16.png", 2102288, 14856, 2048, { 32, 32 } },
   { "gfx/actors/flame/flame_0.ia16.png", 2102288, 95008, 2048, { 32, 32 } },
   { "gfx/actors/flame/flame_1.ia16.png", 2102288, 97056, 2048, { 32, 32 } },
   { "gfx/actors/flame/flame_2.ia16.png", 2102288, 99104, 2048, { 32, 32 } },
   { "gfx/actors/flame/flame_3.ia16.png", 2102288, 101152, 2048, { 32, 32 } },
   { "gfx/actors/flame/flame_4.ia16.png", 2102288, 103200, 2048, { 32, 32 } },
   { "gfx/actors/flame/flame_5.ia16.png", 2102288, 105248, 2048, { 32, 32 } },
   { "gfx/actors/flame/flame_6.ia16.png", 2102288, 107296, 2048, { 32, 32 } },
   { "gfx/actors/flame/flame_7.ia16.png", 2102288, 109344, 2048, { 32, 32 } },
   { "gfx/actors/leaves/leaf.rgba16.png", 2102288, 117728, 512, { 16, 16 } },
   { "gfx/actors/mist/mist.ia16.png", 2102288, 128, 2048, { 32, 32 } },
   { "gfx/actors/mushroom_1up/1-up_mushroom.rgba16.png", 2102288, 169512, 4096, { 32, 64 } },
   { "gfx/actors/pebble/pebble.rgba16.png", 2102288, 115456, 2048, { 32, 32 } },
   { "gfx/actors/power_meter/power_meter_five_segments.rgba16.png", 2102288, 158688, 2048, { 32, 32 } },
   { "gfx/actors/power_meter/power_meter_four_segments.rgba16.png", 2102288, 160736, 2048, { 32, 32 } },
   { "gfx/actors/power_meter/power_meter_full.rgba16.png", 2102288, 152544, 2048, { 32, 32 } },
   { "gfx/actors/power_meter/power_meter_left_side.rgba16.png", 2102288, 144352, 4096, { 32, 64 } },
   { "gfx/actors/power_meter/power_meter_one_segment.rgba16.png", 2102288, 166880, 2048, { 32, 32 } },
   { "gfx/actors/power_meter/power_meter_right_side.rgba16.png", 2102288, 148448, 4096, { 32, 64 } },
   { "gfx/actors/power_meter/power_meter_seven_segments.rgba16.png", 2102288, 154592, 2048, { 32, 32 } },
   { "gfx/actors/power_meter/power_meter_six_segments.rgba16.png", 2102288, 156640, 2048, { 32, 32 } },
   { "gfx/actors/power_meter/power_meter_three_segments.rgba16.png", 2102288, 162784, 2048, { 32, 32 } },
   { "gfx/actors/power_meter/power_meter_two_segments.rgba16.png", 2102288, 164832, 2048, { 32, 32 } },
   { "gfx/actors/sand/sand_particle.rgba16.png", 2102288, 178896, 512, { 16, 16 } },
   { "gfx/actors/star/star_surface.rgba16.png", 2102288, 173808, 2048, { 32, 32 } },
   { "gfx/actors/tree/m_tree.rgba16.png", 2102288, 205336, 4096, { 32, 64 } },
   { "gfx/actors/tree/pine_tree.rgba16.png", 2102288, 196448, 4096, { 32, 64 } },
   { "gfx/actors/tree/snowy_pine_tree.rgba16.png", 2102288, 200776, 4096, { 32, 64 } },
   { "gfx/actors/tree/tree_left_side.rgba16.png", 2102288, 187944, 4096, { 32, 64 } },
   { "gfx/actors/tree/tree_right_side.rgba16.png", 2102288, 192040, 4096, { 32, 64 } },
   { "gfx/actors/warp_pipe/warp_pipe_side.rgba16.png", 2102288, 32320, 4096, { 32, 64 } },
   { "gfx/actors/warp_pipe/warp_pipe_top.rgba16.png", 2102288, 37224, 2048, { 32, 32 } },
   { "gfx/actors/white_particle/snow_particle.rgba16.png", 2102288, 181920, 512, { 16, 16 } },
   { "gfx/actors/wooden_signpost/wooden_signpost_back.rgba16.png", 2102288, 182728, 2048, { 32, 32 } },
   { "gfx/actors/wooden_signpost/wooden_signpost_front.rgba16.png", 2102288, 184776, 2048, { 32, 32 } },
   { "gfx/levels/intro/0.rgba16.png", 2532256, 32416, 2048, { 32, 32 } },
   { "gfx/levels/intro/1.rgba16.png", 2532256, 34464, 2048, { 32, 32 } },
   { "gfx/levels/intro/2_copyright.rgba16.png", 2532256, 46240, 4096, { 128, 16 } },
   { "gfx/levels/intro/3_tm.rgba16.png", 2532256, 50336, 512, { 16, 16 } },
   { "gfx/textures/title_screen_bg/title_screen_bg.033C0.rgba16.png", 2558144, 13248, 3200, { 80, 20 } },
   { "gfx/textures/title_screen_bg/title_screen_bg.04040.rgba16.png", 2558144, 16448, 3200, { 80, 20 } },
   { "gfx/textures/title_screen_bg/title_screen_bg.04CC0.rgba16.png", 2558144, 19648, 3200, { 80, 20 } },
   { "gfx/textures/title_screen_bg/title_screen_bg.05940.rgba16.png", 2558144, 22848, 3200, { 80, 20 } },
   { "gfx/levels/menu/main_menu_seg7.00018.rgba16.png", 2778544, 24, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.00818.rgba16.png", 2778544, 2072, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.01018.rgba16.png", 2778544, 4120, 4096, { 64, 32 } },
   { "gfx/levels/menu/main_menu_seg7.02018.rgba16.png", 2778544, 8216, 4096, { 64, 32 } },
   { "gfx/levels/menu/main_menu_seg7.03468.rgba16.png", 2778544, 13416, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.03C68.rgba16.png", 2778544, 15464, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.04468.rgba16.png", 2778544, 17512, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.04C68.rgba16.png", 2778544, 19560, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.05468.rgba16.png", 2778544, 21608, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.06328.rgba16.png", 2778544, 25384, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.06B28.rgba16.png", 2778544, 27432, 2048, { 32, 32 } },
   { "gfx/levels/menu/main_menu_seg7.073D0.rgba16.png", 2778544, 29648, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.075D0.rgba16.png", 2778544, 30160, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.077D0.rgba16.png", 2778544, 30672, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.079D0.rgba16.png", 2778544, 31184, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.07BD0.rgba16.png", 2778544, 31696, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.07DD0.rgba16.png", 2778544, 32208, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.07FD0.rgba16.png", 2778544, 32720, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.081D0.rgba16.png", 2778544, 33232, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.083D0.rgba16.png", 2778544, 33744, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.085D0.rgba16.png", 2778544, 34256, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.087D0.rgba16.png", 2778544, 34768, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.089D0.rgba16.png", 2778544, 35280, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.08BD0.rgba16.png", 2778544, 35792, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.08DD0.rgba16.png", 2778544, 36304, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.08FD0.rgba16.png", 2778544, 36816, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.091D0.rgba16.png", 2778544, 37328, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.093D0.rgba16.png", 2778544, 37840, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.095D0.rgba16.png", 2778544, 38352, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.097D0.rgba16.png", 2778544, 38864, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.099D0.rgba16.png", 2778544, 39376, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.09BD0.rgba16.png", 2778544, 39888, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.09DD0.rgba16.png", 2778544, 40400, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.09FD0.rgba16.png", 2778544, 40912, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.0A1D0.rgba16.png", 2778544, 41424, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.0A3D0.rgba16.png", 2778544, 41936, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.0A5D0.rgba16.png", 2778544, 42448, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.0A7D0.rgba16.png", 2778544, 42960, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.0A9D0.rgba16.png", 2778544, 43472, 512, { 16, 16 } },
   { "gfx/levels/menu/main_menu_seg7.0D1A8.rgba16.png", 2778544, 48352, 4096, { 64, 32 } },
   { "gfx/levels/menu/main_menu_seg7.0E1A8.rgba16.png", 2778544, 52448, 4096, { 64, 32 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AC40.ia8.png", 2778544, 44096, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AC80.ia8.png", 2778544, 44160, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0ACC0.ia8.png", 2778544, 44224, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AD00.ia8.png", 2778544, 44288, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AD40.ia8.png", 2778544, 44352, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AD80.ia8.png", 2778544, 44416, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0ADC0.ia8.png", 2778544, 44480, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AE00.ia8.png", 2778544, 44544, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AE40.ia8.png", 2778544, 44608, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AE80.ia8.png", 2778544, 44672, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AEC0.ia8.png", 2778544, 44736, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AF00.ia8.png", 2778544, 44800, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AF40.ia8.png", 2778544, 44864, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AF80.ia8.png", 2778544, 44928, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0AFC0.ia8.png", 2778544, 44992, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B000.ia8.png", 2778544, 45056, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B040.ia8.png", 2778544, 45120, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B080.ia8.png", 2778544, 45184, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B0C0.ia8.png", 2778544, 45248, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B100.ia8.png", 2778544, 45312, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B140.ia8.png", 2778544, 45376, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B180.ia8.png", 2778544, 45440, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B1C0.ia8.png", 2778544, 45504, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B200.ia8.png", 2778544, 45568, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B240.ia8.png", 2778544, 45632, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B280.ia8.png", 2778544, 45696, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B2C0.ia8.png", 2778544, 45760, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B300.ia8.png", 2778544, 45824, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B340.ia8.png", 2778544, 45888, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B380.ia8.png", 2778544, 45952, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B3C0.ia8.png", 2778544, 46016, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B400.ia8.png", 2778544, 46080, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B440.ia8.png", 2778544, 46144, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B480.ia8.png", 2778544, 46208, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B4C0.ia8.png", 2778544, 46272, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B500.ia8.png", 2778544, 46336, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B540.ia8.png", 2778544, 46400, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B580.ia8.png", 2778544, 46464, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B5C0.ia8.png", 2778544, 46528, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B600.ia8.png", 2778544, 46592, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B640.ia8.png", 2778544, 46656, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B680.ia8.png", 2778544, 46720, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B6C0.ia8.png", 2778544, 46784, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B700.ia8.png", 2778544, 46848, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B740.ia8.png", 2778544, 46912, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B780.ia8.png", 2778544, 46976, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B7C0.ia8.png", 2778544, 46648, 64, { 8, 8 } },
   { "gfx/levels/menu/main_menu_seg7_us.0B800.ia8.png", 2778544, 47104, 64, { 8, 8 } },
   { "gfx/textures/segment2/segment2.05000.rgba16.png", 2778544, 41424, 512, { 16, 16 } },
   { "gfx/textures/skyboxes/water.png", 2803376, 0, 131392, { -1 } },
   { "gfx/textures/skyboxes/ccm.png", 2854672, 0, 131392, { -1 } },
   { "gfx/textures/skyboxes/clouds.png", 2913232, 0, 84288, { -1 } },
   { "gfx/textures/skyboxes/bitfs.png", 2949184, 0, 102720, { -1 } },
   { "gfx/textures/skyboxes/wdw.png", 2974960, 0, 131392, { -1 } },
   { "gfx/textures/skyboxes/cloud_floor.png", 3045504, 0, 131392, { -1 } },
   { "gfx/textures/skyboxes/ssl.png", 3085536, 0, 131392, { -1 } },
   { "gfx/textures/skyboxes/bbh.png", 3125680, 0, 84288, { -1 } },
   { "gfx/textures/skyboxes/bidw.png", 3153104, 0, 131392, { -1 } },
   { "gfx/textures/skyboxes/bits.png", 3198656, 0, 131392, { -1 } },
   { "gfx/levels/lll/7.rgba16.png", 3269072, 6144, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.00000.rgba16.png", 3269072, 0, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.00800.rgba16.png", 3269072, 2048, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.01000.rgba16.png", 3269072, 4096, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.01800.rgba16.png", 3269072, 6144, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.02000.rgba16.png", 3269072, 8192, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.02800.rgba16.png", 3269072, 10240, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.03000.rgba16.png", 3269072, 12288, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.03800.rgba16.png", 3269072, 14336, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.04000.rgba16.png", 3269072, 16384, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.04800.rgba16.png", 3269072, 18432, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.05000.rgba16.png", 3269072, 20480, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.05800.rgba16.png", 3269072, 22528, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.06000.rgba16.png", 3269072, 24576, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.06800.rgba16.png", 3269072, 26624, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.07000.rgba16.png", 3269072, 28672, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.07800.rgba16.png", 3269072, 30720, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.08000.rgba16.png", 3269072, 32768, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.08800.rgba16.png", 3269072, 34816, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.09800.rgba16.png", 3269072, 38912, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.0A000.rgba16.png", 3269072, 40960, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.0A800.rgba16.png", 3269072, 43008, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.0B000.rgba16.png", 3269072, 45056, 2048, { 32, 32 } },
   { "gfx/textures/fire/lll_textures.0B800.rgba16.png", 3269072, 47104, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/2.ia16.png", 3305024, 47104, 4096, { 32, 64 } },
   { "gfx/textures/cave/hmc_textures.0C000.ia16.png", 3305024, 43008, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.00000.rgba16.png", 3305024, 0, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.00800.rgba16.png", 3305024, 2048, 4096, { 32, 64 } },
   { "gfx/textures/spooky/bbh_textures.01800.rgba16.png", 3305024, 6144, 4096, { 32, 64 } },
   { "gfx/textures/spooky/bbh_textures.02800.rgba16.png", 3305024, 10240, 4096, { 32, 64 } },
   { "gfx/textures/spooky/bbh_textures.03800.rgba16.png", 3305024, 14336, 4096, { 32, 64 } },
   { "gfx/textures/spooky/bbh_textures.04800.rgba16.png", 3305024, 18432, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.05000.rgba16.png", 3305024, 20480, 4096, { 32, 64 } },
   { "gfx/textures/spooky/bbh_textures.06000.rgba16.png", 3305024, 24576, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.06800.rgba16.png", 3305024, 26624, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.07000.rgba16.png", 3305024, 28672, 4096, { 64, 32 } },
   { "gfx/textures/spooky/bbh_textures.08000.rgba16.png", 3305024, 32768, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.08800.rgba16.png", 3305024, 34816, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.09000.rgba16.png", 3305024, 36864, 4096, { 64, 32 } },
   { "gfx/textures/spooky/bbh_textures.0A800.ia16.png", 3305024, 43008, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.0B000.ia16.png", 3305024, 45056, 2048, { 32, 32 } },
   { "gfx/textures/spooky/bbh_textures.0B800.ia16.png", 3305024, 47104, 4096, { 32, 64 } },
   { "gfx/levels/ssl/7.rgba16.png", 3330160, 32768, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.00000.rgba16.png", 3330160, 0, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.01000.rgba16.png", 3330160, 4096, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.01800.rgba16.png", 3330160, 6144, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.02000.rgba16.png", 3330160, 8192, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.02800.rgba16.png", 3330160, 10240, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.03000.rgba16.png", 3330160, 12288, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.03800.rgba16.png", 3330160, 14336, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.04000.rgba16.png", 3330160, 16384, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.04800.rgba16.png", 3330160, 18432, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.05000.rgba16.png", 3330160, 20480, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.05800.rgba16.png", 3330160, 22528, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.06000.rgba16.png", 3330160, 24576, 4096, { 32, 64 } },
   { "gfx/textures/generic/bob_textures.07000.rgba16.png", 3330160, 28672, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.07800.rgba16.png", 3330160, 30720, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.08000.rgba16.png", 3330160, 32768, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.08800.rgba16.png", 3330160, 34816, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.09000.rgba16.png", 3330160, 36864, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.09800.rgba16.png", 3330160, 38912, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.0A000.rgba16.png", 3330160, 40960, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.0A800.rgba16.png", 3330160, 43008, 2048, { 32, 32 } },
   { "gfx/textures/generic/bob_textures.0B000.ia16.png", 3330160, 45056, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.0B000.ia16.png", 3330160, 45056, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.0BC00.ia16.png", 3330160, 45056, 2048, { 32, 32 } },
   { "gfx/textures/water/jrb_textures.00000.rgba16.png", 3361584, 0, 2048, { 32, 32 } },
   { "gfx/textures/water/jrb_textures.00800.rgba16.png", 3361584, 2048, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.01800.rgba16.png", 3361584, 6144, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.02800.rgba16.png", 3361584, 10240, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.03800.rgba16.png", 3361584, 14336, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.04800.rgba16.png", 3361584, 18432, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.05800.rgba16.png", 3361584, 22528, 2048, { 32, 32 } },
   { "gfx/textures/water/jrb_textures.06000.rgba16.png", 3361584, 24576, 2048, { 32, 32 } },
   { "gfx/textures/water/jrb_textures.06800.rgba16.png", 3361584, 26624, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.07800.rgba16.png", 3361584, 30720, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.08800.rgba16.png", 3361584, 34816, 2048, { 32, 32 } },
   { "gfx/textures/water/jrb_textures.09000.rgba16.png", 3361584, 36864, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.0A000.rgba16.png", 3361584, 40960, 2048, { 32, 32 } },
   { "gfx/textures/water/jrb_textures.0A800.rgba16.png", 3361584, 43008, 4096, { 64, 32 } },
   { "gfx/textures/water/jrb_textures.0B800.rgba16.png", 3361584, 47104, 4096, { 64, 32 } },
   { "gfx/textures/grass/wf_textures.06000.rgba16.png", 3397392, 20480, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.0A000.rgba16.png", 3397392, 24576, 2048, { 32, 32 } },
   { "gfx/textures/sky/metal_hole.rgba16.png", 3397392, 26624, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.00000.rgba16.png", 3397392, 0, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.00800.rgba16.png", 3397392, 2048, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.01000.rgba16.png", 3397392, 4096, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.01800.rgba16.png", 3397392, 6144, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.02000.rgba16.png", 3397392, 8192, 4096, { 32, 64 } },
   { "gfx/textures/sky/rr_textures.03000.rgba16.png", 3397392, 12288, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.03800.rgba16.png", 3397392, 14336, 4096, { 64, 32 } },
   { "gfx/textures/sky/rr_textures.04800.rgba16.png", 3397392, 18432, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.05000.rgba16.png", 3397392, 20480, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.05800.rgba16.png", 3397392, 22528, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.06000.rgba16.png", 3397392, 24576, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.07000.rgba16.png", 3397392, 28672, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.07800.rgba16.png", 3397392, 30720, 2048, { 32, 32 } },
   { "gfx/textures/sky/rr_textures.08000.rgba16.png", 3397392, 32768, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.00000.rgba16.png", 3412288, 0, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.00800.rgba16.png", 3412288, 2048, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.01000.rgba16.png", 3412288, 4096, 4096, { 32, 64 } },
   { "gfx/textures/snow/ccm_textures.02000.rgba16.png", 3412288, 8192, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.02800.rgba16.png", 3412288, 10240, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.03000.rgba16.png", 3412288, 12288, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.03800.rgba16.png", 3412288, 14336, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.04000.rgba16.png", 3412288, 16384, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.04800.rgba16.png", 3412288, 18432, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.05000.rgba16.png", 3412288, 20480, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.05800.rgba16.png", 3412288, 22528, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.06000.rgba16.png", 3412288, 24576, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.06800.rgba16.png", 3412288, 26624, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.07000.rgba16.png", 3412288, 28672, 4096, { 64, 32 } },
   { "gfx/textures/snow/ccm_textures.08000.rgba16.png", 3412288, 32768, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.08800.rgba16.png", 3412288, 34816, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.09000.ia16.png", 3412288, 36864, 2048, { 32, 32 } },
   { "gfx/textures/snow/ccm_textures.09800.ia16.png", 3412288, 38912, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/castle_light.ia16.png", 3439184, 47104, 2048, { 32, 32 } },
   { "gfx/levels/ddd/1.rgba16.png", 3439184, 4096, 2048, { 32, 32 } },
   { "gfx/levels/jrb/0.rgba16.png", 3439184, 4096, 2048, { 32, 32 } },
   { "gfx/textures/cave/hmc_textures.00000.rgba16.png", 3439184, 0, 4096, { 32, 64 } },
   { "gfx/textures/cave/hmc_textures.01000.rgba16.png", 3439184, 4096, 2048, { 32, 32 } },
   { "gfx/textures/cave/hmc_textures.01800.rgba16.png", 3439184, 6144, 4096, { 32, 64 } },
   { "gfx/textures/cave/hmc_textures.02800.rgba16.png", 3439184, 10240, 2048, { 32, 32 } },
   { "gfx/textures/cave/hmc_textures.03000.rgba16.png", 3439184, 12288, 2048, { 32, 32 } },
   { "gfx/textures/cave/hmc_textures.03800.rgba16.png", 3439184, 14336, 4096, { 32, 64 } },
   { "gfx/textures/cave/hmc_textures.04800.rgba16.png", 3439184, 18432, 4096, { 64, 32 } },
   { "gfx/textures/cave/hmc_textures.05800.rgba16.png", 3439184, 22528, 4096, { 32, 64 } },
   { "gfx/textures/cave/hmc_textures.06800.rgba16.png", 3439184, 26624, 2048, { 32, 32 } },
   { "gfx/textures/cave/hmc_textures.07000.rgba16.png", 3439184, 28672, 2048, { 32, 32 } },
   { "gfx/textures/cave/hmc_textures.07800.rgba16.png", 3439184, 30720, 4096, { 32, 64 } },
   { "gfx/textures/cave/hmc_textures.08800.rgba16.png", 3439184, 34816, 4096, { 32, 64 } },
   { "gfx/textures/cave/hmc_textures.09800.rgba16.png", 3439184, 38912, 2048, { 32, 32 } },
   { "gfx/textures/cave/hmc_textures.0A000.rgba16.png", 3439184, 40960, 2048, { 32, 32 } },
   { "gfx/textures/cave/hmc_textures.0A800.rgba16.png", 3439184, 43008, 4096, { 32, 64 } },
   { "gfx/textures/cave/hmc_textures.0B800.ia16.png", 3439184, 47104, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.04800.rgba16.png", 3467104, 2048, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.00000.rgba16.png", 3467104, 0, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.00800.rgba16.png", 3467104, 2048, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.01000.rgba16.png", 3467104, 4096, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.01800.rgba16.png", 3467104, 6144, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.02000.rgba16.png", 3467104, 8192, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.02800.rgba16.png", 3467104, 10240, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.03000.rgba16.png", 3467104, 12288, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.03800.rgba16.png", 3467104, 14336, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.04000.rgba16.png", 3467104, 16384, 4096, { 32, 64 } },
   { "gfx/textures/machine/ttc_textures.05000.rgba16.png", 3467104, 20480, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.05800.rgba16.png", 3467104, 22528, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.06000.rgba16.png", 3467104, 24576, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.06800.rgba16.png", 3467104, 26624, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.07000.rgba16.png", 3467104, 28672, 2048, { 32, 32 } },
   { "gfx/textures/machine/ttc_textures.07800.rgba16.png", 3467104, 30720, 2048, { 16, 64 } },
   { "gfx/textures/machine/ttc_textures.08000.rgba16.png", 3467104, 32768, 1024, { 64, 8 } },
   { "gfx/textures/machine/ttc_textures.08400.rgba16.png", 3467104, 33792, 2048, { 32, 32 } },
   { "gfx/levels/ttm/8.rgba16.png", 3479904, 43008, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.00000.rgba16.png", 3479904, 0, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.00800.rgba16.png", 3479904, 2048, 4096, { 64, 32 } },
   { "gfx/textures/mountain/ttm_textures.01800.rgba16.png", 3479904, 6144, 4096, { 32, 64 } },
   { "gfx/textures/mountain/ttm_textures.02800.rgba16.png", 3479904, 10240, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.03000.rgba16.png", 3479904, 12288, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.03800.rgba16.png", 3479904, 14336, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.04000.rgba16.png", 3479904, 16384, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.04800.rgba16.png", 3479904, 18432, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.05000.rgba16.png", 3479904, 20480, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.05800.rgba16.png", 3479904, 22528, 4096, { 32, 64 } },
   { "gfx/textures/mountain/ttm_textures.06800.rgba16.png", 3479904, 26624, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.07000.rgba16.png", 3479904, 28672, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.07800.rgba16.png", 3479904, 30720, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.08000.rgba16.png", 3479904, 32768, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.08800.rgba16.png", 3479904, 34816, 4096, { 64, 32 } },
   { "gfx/textures/mountain/ttm_textures.09800.rgba16.png", 3479904, 38912, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.0A800.rgba16.png", 3479904, 43008, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.0B000.rgba16.png", 3479904, 45056, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.0B800.rgba16.png", 3479904, 47104, 2048, { 32, 32 } },
   { "gfx/textures/mountain/ttm_textures.0C000.rgba16.png", 3479904, 49152, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.00000.rgba16.png", 3502928, 0, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.00800.rgba16.png", 3502928, 2048, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.01000.rgba16.png", 3502928, 4096, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.01800.rgba16.png", 3502928, 6144, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.02000.rgba16.png", 3502928, 8192, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.02800.rgba16.png", 3502928, 10240, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.03000.rgba16.png", 3502928, 12288, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.03800.rgba16.png", 3502928, 14336, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.04000.rgba16.png", 3502928, 16384, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.04800.rgba16.png", 3502928, 18432, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.05000.rgba16.png", 3502928, 20480, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.05800.rgba16.png", 3502928, 22528, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.06800.rgba16.png", 3502928, 26624, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.07000.rgba16.png", 3502928, 28672, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.07800.rgba16.png", 3502928, 30720, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.08000.rgba16.png", 3502928, 32768, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.08800.rgba16.png", 3502928, 34816, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.09000.rgba16.png", 3502928, 36864, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.09800.rgba16.png", 3502928, 38912, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.0A000.rgba16.png", 3502928, 40960, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.0A800.rgba16.png", 3502928, 43008, 2048, { 32, 32 } },
   { "gfx/textures/grass/wf_textures.0B800.ia16.png", 3502928, 47104, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.00000.rgba16.png", 3534096, 0, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.00800.rgba16.png", 3534096, 2048, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.01000.rgba16.png", 3534096, 4096, 4096, { 64, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.02000.rgba16.png", 3534096, 8192, 4096, { 32, 64 } },
   { "gfx/textures/outside/castle_grounds_textures.03000.rgba16.png", 3534096, 12288, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.03800.rgba16.png", 3534096, 14336, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.04000.rgba16.png", 3534096, 16384, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.04800.rgba16.png", 3534096, 18432, 4096, { 32, 64 } },
   { "gfx/textures/outside/castle_grounds_textures.05800.rgba16.png", 3534096, 22528, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.06000.rgba16.png", 3534096, 24576, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.06800.rgba16.png", 3534096, 26624, 4096, { 64, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.07800.rgba16.png", 3534096, 30720, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.08000.rgba16.png", 3534096, 32768, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.08800.rgba16.png", 3534096, 34816, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.09000.rgba16.png", 3534096, 36864, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.09800.rgba16.png", 3534096, 38912, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.0A000.rgba16.png", 3534096, 40960, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.0A800.rgba16.png", 3534096, 43008, 2048, { 32, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.0B000.rgba16.png", 3534096, 45056, 1024, { 16, 32 } },
   { "gfx/textures/outside/castle_grounds_textures.0B400.rgba16.png", 3534096, 46080, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.00000.rgba16.png", 3561856, 0, 4096, { 64, 32 } },
   { "gfx/textures/inside/inside_castle_textures.01000.rgba16.png", 3561856, 4096, 4096, { 64, 32 } },
   { "gfx/textures/inside/inside_castle_textures.02000.rgba16.png", 3561856, 8192, 4096, { 32, 64 } },
   { "gfx/textures/inside/inside_castle_textures.03000.rgba16.png", 3561856, 12288, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.03800.rgba16.png", 3561856, 14336, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.04000.rgba16.png", 3561856, 16384, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.05000.rgba16.png", 3561856, 20480, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.05800.rgba16.png", 3561856, 22528, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.06000.rgba16.png", 3561856, 24576, 4096, { 32, 64 } },
   { "gfx/textures/inside/inside_castle_textures.07000.rgba16.png", 3561856, 28672, 4096, { 32, 64 } },
   { "gfx/textures/inside/inside_castle_textures.08000.rgba16.png", 3561856, 32768, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.08800.rgba16.png", 3561856, 34816, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.09000.rgba16.png", 3561856, 36864, 4096, { 32, 64 } },
   { "gfx/textures/inside/inside_castle_textures.0A000.rgba16.png", 3561856, 40960, 4096, { 32, 64 } },
   { "gfx/textures/inside/inside_castle_textures.0B000.rgba16.png", 3561856, 45056, 2048, { 32, 32 } },
   { "gfx/textures/inside/inside_castle_textures.0B800.rgba16.png", 3561856, 47104, 4096, { 64, 32 } },
   { "gfx/textures/effect/bubble.06048.rgba16.png", 3601712, 24648, 2048, { 32, 32 } },
   { "gfx/textures/effect/flower.00008.rgba16.png", 3601712, 8, 2048, { 32, 32 } },
   { "gfx/textures/effect/flower.00808.rgba16.png", 3601712, 2056, 2048, { 32, 32 } },
   { "gfx/textures/effect/flower.01008.rgba16.png", 3601712, 4104, 2048, { 32, 32 } },
   { "gfx/textures/effect/flower.01808.rgba16.png", 3601712, 6152, 2048, { 32, 32 } },
   { "gfx/textures/effect/lava_bubble.02020.rgba16.png", 3601712, 8224, 2048, { 32, 32 } },
   { "gfx/textures/effect/lava_bubble.02820.rgba16.png", 3601712, 10272, 2048, { 32, 32 } },
   { "gfx/textures/effect/lava_bubble.03020.rgba16.png", 3601712, 12320, 2048, { 32, 32 } },
   { "gfx/textures/effect/lava_bubble.03820.rgba16.png", 3601712, 14368, 2048, { 32, 32 } },
   { "gfx/textures/effect/lava_bubble.04020.rgba16.png", 3601712, 16416, 2048, { 32, 32 } },
   { "gfx/textures/effect/lava_bubble.04820.rgba16.png", 3601712, 18464, 2048, { 32, 32 } },
   { "gfx/textures/effect/lava_bubble.05020.rgba16.png", 3601712, 20512, 2048, { 32, 32 } },
   { "gfx/textures/effect/lava_bubble.05820.rgba16.png", 3601712, 22560, 2048, { 32, 32 } },
   { "gfx/textures/effect/tiny_bubble.0684C.rgba16.png", 3601712, 26700, 512, { 16, 16 } },
   { "gfx/textures/effect/tiny_bubble.06AD8.rgba16.png", 3601712, 27352, 512, { 16, 16 } },
   { "gfx/levels/bbh/0.rgba16.png", 3611712, 0, 4096, { 32, 64 } },
   { "gfx/levels/bbh/1.rgba16.png", 3611712, 4096, 2048, { 32, 32 } },
   { "gfx/levels/bbh/2.rgba16.png", 3611712, 6144, 2048, { 32, 32 } },
   { "gfx/levels/bbh/3.rgba16.png", 3611712, 8192, 4096, { 32, 64 } },
   { "gfx/levels/bbh/4.rgba16.png", 3611712, 12288, 1024, { 16, 32 } },
   { "gfx/levels/bbh/5.rgba16.png", 3611712, 13312, 4096, { 32, 64 } },
   { "gfx/levels/bbh/6.rgba16.png", 3611712, 17408, 1024, { 16, 32 } },
   { "gfx/levels/ccm/0.rgba16.png", 3684688, 0, 2048, { 32, 32 } },
   { "gfx/levels/ccm/1.rgba16.png", 3684688, 2048, 256, { 32, 4 } },
   { "gfx/levels/ccm/10.rgba16.png", 3684688, 17152, 2048, { 32, 32 } },
   { "gfx/levels/ccm/11.rgba16.png", 3684688, 19200, 2048, { 32, 32 } },
   { "gfx/levels/ccm/12.rgba16.png", 3684688, 72024, 2048, { 32, 32 } },
   { "gfx/levels/ccm/2.rgba16.png", 3684688, 2304, 2048, { 32, 32 } },
   { "gfx/levels/ccm/3.rgba16.png", 3684688, 4352, 2048, { 32, 32 } },
   { "gfx/levels/ccm/4.rgba16.png", 3684688, 6400, 2048, { 32, 32 } },
   { "gfx/levels/ccm/7.rgba16.png", 3684688, 12544, 2048, { 32, 32 } },
   { "gfx/levels/ccm/8.ia16.png", 3684688, 14592, 512, { 16, 16 } },
   { "gfx/levels/ccm/9.ia16.png", 3684688, 15104, 2048, { 32, 32 } },
   { "gfx/levels/lll/27.ia16.png", 3684688, 14592, 512, { 16, 16 } },
   { "gfx/levels/castle_inside/1.rgba16.png", 3760960, 2048, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/10.rgba16.png", 3760960, 28672, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/11.rgba16.png", 3760960, 30720, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/12.rgba16.png", 3760960, 32768, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/13.rgba16.png", 3760960, 34816, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/14.rgba16.png", 3760960, 36864, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/15.rgba16.png", 3760960, 38912, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/16.ia16.png", 3760960, 40960, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/17.rgba16.png", 3760960, 43008, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/18.rgba16.png", 3760960, 47104, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/19.rgba16.png", 3760960, 51200, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/20.rgba16.png", 3760960, 55296, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/21.rgba16.png", 3760960, 59392, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/22.rgba16.png", 3760960, 63488, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/23_us.rgba16.png", 3760960, 67584, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/24_us.rgba16.png", 3760960, 71680, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/25.rgba16.png", 3760960, 75776, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/26.rgba16.png", 3760960, 79872, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/27.rgba16.png", 3760960, 83968, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/28.rgba16.png", 3760960, 88064, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/29.rgba16.png", 3760960, 92160, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/3.rgba16.png", 3760960, 8192, 4096, { 32, 64 } },
   { "gfx/levels/castle_inside/30.rgba16.png", 3760960, 94208, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/31.rgba16.png", 3760960, 96256, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/32.rgba16.png", 3760960, 100352, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/33.rgba16.png", 3760960, 104448, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/34.rgba16.png", 3760960, 108544, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/35.rgba16.png", 3760960, 112640, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/36.rgba16.png", 3760960, 116736, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/37.rgba16.png", 3760960, 120832, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/38.rgba16.png", 3760960, 124928, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/39.rgba16.png", 3760960, 129024, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/4.rgba16.png", 3760960, 12288, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/40.rgba16.png", 3760960, 133120, 4096, { 64, 32 } },
   { "gfx/levels/castle_inside/5.rgba16.png", 3760960, 14336, 4096, { 32, 64 } },
   { "gfx/levels/castle_inside/6.rgba16.png", 3760960, 18432, 4096, { 32, 64 } },
   { "gfx/levels/castle_inside/7.rgba16.png", 3760960, 22528, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/8.rgba16.png", 3760960, 24576, 2048, { 32, 32 } },
   { "gfx/levels/castle_inside/9.rgba16.png", 3760960, 26624, 2048, { 32, 32 } },
   { "gfx/levels/hmc/7.rgba16.png", 3760960, 92160, 2048, { 32, 32 } },
   { "gfx/levels/hmc/0.rgba16.png", 4001216, 0, 4096, { 32, 64 } },
   { "gfx/levels/hmc/1.rgba16.png", 4001216, 4096, 4096, { 32, 64 } },
   { "gfx/levels/hmc/2.rgba16.png", 4001216, 8192, 4096, { 32, 64 } },
   { "gfx/levels/hmc/3.rgba16.png", 4001216, 12288, 2048, { 32, 32 } },
   { "gfx/levels/hmc/4.rgba16.png", 4001216, 14336, 2048, { 32, 32 } },
   { "gfx/levels/hmc/5.rgba16.png", 4001216, 16384, 2048, { 32, 32 } },
   { "gfx/levels/hmc/6.rgba16.png", 4001216, 18432, 4096, { 32, 64 } },
   { "gfx/levels/ssl/0.rgba16.png", 4093616, 0, 2048, { 32, 32 } },
   { "gfx/levels/ssl/1.ia16.png", 4093616, 2048, 2048, { 32, 32 } },
   { "gfx/levels/ssl/10.rgba16.png", 4093616, 55208, 4096, { 32, 64 } },
   { "gfx/levels/ssl/11.rgba16.png", 4093616, 59304, 4096, { 32, 64 } },
   { "gfx/levels/ssl/2.rgba16.png", 4093616, 4096, 2048, { 32, 32 } },
   { "gfx/levels/ssl/3.rgba16.png", 4093616, 6144, 2048, { 32, 32 } },
   { "gfx/levels/ssl/4.rgba16.png", 4093616, 8192, 2048, { 32, 32 } },
   { "gfx/levels/ssl/5.rgba16.png", 4093616, 10240, 4096, { 32, 64 } },
   { "gfx/levels/ssl/6.rgba16.png", 4093616, 14336, 2048, { 32, 32 } },
   { "gfx/levels/ssl/8.rgba16.png", 4093616, 49064, 2048, { 32, 32 } },
   { "gfx/levels/ssl/9.rgba16.png", 4093616, 51112, 4096, { 32, 64 } },
   { "gfx/levels/bob/0.rgba16.png", 4178608, 0, 2048, { 32, 32 } },
   { "gfx/levels/bob/1.rgba16.png", 4178608, 2048, 2048, { 32, 32 } },
   { "gfx/levels/bob/2.rgba16.png", 4178608, 4096, 2048, { 32, 32 } },
   { "gfx/levels/bob/3.rgba16.png", 4178608, 6144, 2048, { 32, 32 } },
   { "gfx/levels/bob/4.rgba16.png", 4178608, 8192, 2048, { 32, 32 } },
   { "gfx/levels/sl/0.rgba16.png", 4218800, 0, 2048, { 32, 32 } },
   { "gfx/levels/sl/1.rgba16.png", 4218800, 2048, 2048, { 32, 32 } },
   { "gfx/levels/sl/2.rgba16.png", 4218800, 4096, 2048, { 32, 32 } },
   { "gfx/levels/sl/3.rgba16.png", 4218800, 6144, 2048, { 32, 32 } },
   { "gfx/levels/sl/4.rgba16.png", 4218800, 8192, 2048, { 32, 32 } },
   { "gfx/levels/wdw/0.rgba16.png", 4255088, 0, 2048, { 32, 32 } },
   { "gfx/levels/wdw/1.rgba16.png", 4255088, 2048, 2048, { 32, 32 } },
   { "gfx/levels/wdw/2.rgba16.png", 4255088, 4096, 2048, { 32, 32 } },
   { "gfx/levels/wdw/3.rgba16.png", 4255088, 6144, 2048, { 32, 32 } },
   { "gfx/levels/wdw/4.rgba16.png", 4255088, 8192, 2048, { 32, 32 } },
   { "gfx/levels/jrb/1.rgba16.png", 4302688, 2048, 4096, { 64, 32 } },
   { "gfx/levels/jrb/2.rgba16.png", 4302688, 6144, 2048, { 32, 32 } },
   { "gfx/levels/jrb/3.rgba16.png", 4302688, 8192, 2048, { 32, 32 } },
   { "gfx/levels/thi/0.rgba16.png", 4343504, 0, 2048, { 32, 32 } },
   { "gfx/levels/thi/1.rgba16.png", 4343504, 2048, 2048, { 32, 32 } },
   { "gfx/levels/ttc/0.rgba16.png", 4378400, 0, 2048, { 32, 32 } },
   { "gfx/levels/ttc/1.rgba16.png", 4378400, 2048, 2048, { 16, 64 } },
   { "gfx/levels/ttc/2.rgba16.png", 4378400, 90000, 2048, { 32, 32 } },
   { "gfx/levels/rr/1.rgba16.png", 4421744, 2048, 4096, { 64, 32 } },
   { "gfx/levels/rr/2.rgba16.png", 4421744, 6144, 2048, { 32, 32 } },
   { "gfx/levels/rr/quarter_flying_carpet.rgba16.png", 4421744, 0, 2048, { 32, 32 } },
   { "gfx/levels/castle_grounds/0.rgba16.png", 4500416, 0, 4096, { 32, 64 } },
   { "gfx/levels/castle_grounds/1.rgba16.png", 4500416, 4096, 4096, { 64, 32 } },
   { "gfx/levels/castle_grounds/2.rgba16.png", 4500416, 8192, 4096, { 64, 32 } },
   { "gfx/levels/castle_grounds/3.rgba16.png", 4500416, 51688, 4096, { 32, 64 } },
   { "gfx/levels/castle_grounds/4.rgba16.png", 4500416, 55784, 4096, { 32, 64 } },
   { "gfx/levels/castle_grounds/5.ia8.png", 4500416, 60136, 2048, { 64, 32 } },
   { "gfx/levels/bitdw/0.rgba16.png", 4541952, 0, 2048, { 32, 32 } },
   { "gfx/levels/bitdw/1.rgba16.png", 4541952, 2048, 2048, { 32, 32 } },
   { "gfx/levels/bitdw/2.rgba16.png", 4541952, 4096, 2048, { 32, 32 } },
   { "gfx/levels/bitdw/3.rgba16.png", 4541952, 6144, 2048, { 32, 32 } },
   { "gfx/levels/vcutm/0.rgba16.png", 4572672, 0, 2048, { 32, 32 } },
   { "gfx/levels/vcutm/1.rgba16.png", 4572672, 2048, 4096, { 64, 32 } },
   { "gfx/levels/vcutm/2.rgba16.png", 4572672, 6144, 4096, { 32, 64 } },
   { "gfx/levels/vcutm/3.rgba16.png", 4572672, 10240, 2048, { 32, 32 } },
   { "gfx/levels/bitfs/0.rgba16.png", 4592848, 0, 4096, { 32, 64 } },
   { "gfx/levels/bitfs/1.rgba16.png", 4592848, 4096, 2048, { 32, 32 } },
   { "gfx/levels/bitfs/2.rgba16.png", 4592848, 6144, 2048, { 32, 32 } },
   { "gfx/levels/bits/0.rgba16.png", 4637600, 0, 4096, { 32, 64 } },
   { "gfx/levels/bits/1.rgba16.png", 4637600, 4096, 4096, { 64, 32 } },
   { "gfx/levels/bits/2.rgba16.png", 4637600, 8192, 2048, { 32, 32 } },
   { "gfx/levels/lll/0.rgba16.png", 4687008, 0, 2048, { 32, 32 } },
   { "gfx/levels/lll/1.rgba16.png", 4687008, 2048, 2048, { 32, 32 } },
   { "gfx/levels/lll/10.rgba16.png", 4687008, 20480, 2048, { 32, 32 } },
   { "gfx/levels/lll/11.rgba16.png", 4687008, 22528, 2048, { 32, 32 } },
   { "gfx/levels/lll/12.rgba16.png", 4687008, 24576, 2048, { 32, 32 } },
   { "gfx/levels/lll/13.rgba16.png", 4687008, 26624, 2048, { 32, 32 } },
   { "gfx/levels/lll/14.rgba16.png", 4687008, 28672, 2048, { 32, 32 } },
   { "gfx/levels/lll/15.rgba16.png", 4687008, 30720, 2048, { 32, 32 } },
   { "gfx/levels/lll/16.rgba16.png", 4687008, 32768, 2048, { 32, 32 } },
   { "gfx/levels/lll/17.rgba16.png", 4687008, 34816, 2048, { 32, 32 } },
   { "gfx/levels/lll/18.rgba16.png", 4687008, 36864, 2048, { 32, 32 } },
   { "gfx/levels/lll/19.rgba16.png", 4687008, 38912, 2048, { 32, 32 } },
   { "gfx/levels/lll/2.rgba16.png", 4687008, 4096, 2048, { 32, 32 } },
   { "gfx/levels/lll/20.rgba16.png", 4687008, 40960, 2048, { 32, 32 } },
   { "gfx/levels/lll/21.rgba16.png", 4687008, 43008, 2048, { 32, 32 } },
   { "gfx/levels/lll/22.rgba16.png", 4687008, 45056, 2048, { 32, 32 } },
   { "gfx/levels/lll/23.rgba16.png", 4687008, 47104, 2048, { 32, 32 } },
   { "gfx/levels/lll/24.rgba16.png", 4687008, 49152, 2048, { 32, 32 } },
   { "gfx/levels/lll/25.rgba16.png", 4687008, 51200, 2048, { 32, 32 } },
   { "gfx/levels/lll/26.rgba16.png", 4687008, 53248, 512, { 16, 16 } },
   { "gfx/levels/lll/28.rgba16.png", 4687008, 54272, 2048, { 32, 32 } },
   { "gfx/levels/lll/29.rgba16.png", 4687008, 56320, 2048, { 32, 32 } },
   { "gfx/levels/lll/3.rgba16.png", 4687008, 6144, 2048, { 32, 32 } },
   { "gfx/levels/lll/30.rgba16.png", 4687008, 58368, 2048, { 32, 32 } },
   { "gfx/levels/lll/31.rgba16.png", 4687008, 60416, 2048, { 32, 32 } },
   { "gfx/levels/lll/32.rgba16.png", 4687008, 62464, 2048, { 64, 16 } },
   { "gfx/levels/lll/4.rgba16.png", 4687008, 8192, 2048, { 32, 32 } },
   { "gfx/levels/lll/5.rgba16.png", 4687008, 10240, 2048, { 32, 32 } },
   { "gfx/levels/lll/6.rgba16.png", 4687008, 12288, 2048, { 32, 32 } },
   { "gfx/levels/lll/8.rgba16.png", 4687008, 16384, 2048, { 32, 32 } },
   { "gfx/levels/lll/9.rgba16.png", 4687008, 18432, 2048, { 32, 32 } },
   { "gfx/levels/ddd/0.rgba16.png", 4774192, 0, 4096, { 32, 64 } },
   { "gfx/levels/ddd/2.rgba16.png", 4774192, 6144, 2048, { 32, 32 } },
   { "gfx/levels/ddd/3.rgba16.png", 4774192, 8192, 4096, { 64, 32 } },
   { "gfx/levels/ddd/4.rgba16.png", 4774192, 12288, 2048, { 32, 32 } },
   { "gfx/levels/wf/0.rgba16.png", 4808848, 0, 2048, { 32, 32 } },
   { "gfx/levels/wf/1.rgba16.png", 4808848, 2048, 2048, { 32, 32 } },
   { "gfx/levels/wf/2.rgba16.png", 4808848, 4096, 2048, { 32, 32 } },
   { "gfx/levels/wf/3.rgba16.png", 4808848, 6144, 2048, { 32, 32 } },
   { "gfx/levels/wf/4.rgba16.png", 4808848, 8192, 2048, { 32, 32 } },
   { "gfx/levels/pss/0.rgba16.png", 4913456, 0, 2048, { 32, 32 } },
   { "gfx/levels/pss/1.ia16.png", 4913456, 2048, 2048, { 32, 32 } },
   { "gfx/levels/pss/2.rgba16.png", 4913456, 4096, 2048, { 32, 32 } },
   { "gfx/levels/cotmc/0.rgba16.png", 4948176, 0, 4096, { 64, 32 } },
   { "gfx/levels/cotmc/1.rgba16.png", 4948176, 4096, 2048, { 32, 32 } },
   { "gfx/levels/cotmc/2.rgba16.png", 4948176, 6144, 2048, { 32, 32 } },
   { "gfx/levels/cotmc/3.rgba16.png", 4948176, 8192, 2048, { 32, 32 } },
   { "gfx/levels/cotmc/4.rgba16.png", 4948176, 10240, 2048, { 64, 16 } },
   { "gfx/levels/totwc/0.rgba16.png", 4975664, 0, 4096, { 32, 64 } },
   { "gfx/levels/totwc/1.rgba16.png", 4975664, 4096, 2048, { 32, 32 } },
   { "gfx/levels/totwc/2.rgba16.png", 4975664, 6144, 2048, { 32, 32 } },
   { "gfx/levels/totwc/3.ia16.png", 4975664, 8192, 2048, { 32, 32 } },
   { "gfx/levels/bowser_1/0.rgba16.png", 4991264, 0, 4096, { 32, 64 } },
   { "gfx/levels/bowser_1/1.rgba16.png", 4991264, 4096, 2048, { 32, 32 } },
   { "gfx/levels/bowser_1/2.rgba16.png", 4991264, 6144, 2048, { 32, 32 } },
   { "gfx/levels/wmotr/0.rgba16.png", 4997920, 0, 2048, { 32, 32 } },
   { "gfx/levels/wmotr/1.rgba16.png", 4997920, 2048, 1024, { 32, 16 } },
   { "gfx/levels/wmotr/2.rgba16.png", 4997920, 3072, 2048, { 32, 32 } },
   { "gfx/levels/wmotr/3.rgba16.png", 4997920, 5120, 512, { 32, 8 } },
   { "gfx/levels/wmotr/4.rgba16.png", 4997920, 5632, 512, { 8, 32 } },
   { "gfx/levels/bowser_2/0.rgba16.png", 5037008, 0, 2048, { 32, 32 } },
   { "gfx/levels/bowser_3/0.rgba16.png", 5041152, 0, 2048, { 32, 32 } },
   { "gfx/levels/bowser_3/1.rgba16.png", 5041152, 2048, 2048, { 32, 32 } },
   { "gfx/levels/bowser_3/2.rgba16.png", 5041152, 4096, 4096, { 32, 64 } },
   { "gfx/levels/ttm/0.ia16.png", 5052688, 0, 2048, { 32, 32 } },
   { "gfx/levels/ttm/1.rgba16.png", 5052688, 2048, 2048, { 32, 32 } },
   { "gfx/levels/ttm/2.rgba16.png", 5052688, 4096, 2048, { 32, 32 } },
   { "gfx/levels/ttm/3.rgba16.png", 5052688, 6144, 2048, { 32, 32 } },
   { "gfx/levels/ttm/4.rgba16.png", 5052688, 8192, 2048, { 32, 32 } },
   { "gfx/levels/ttm/5.rgba16.png", 5052688, 10240, 2048, { 32, 32 } },
   { "gfx/levels/ttm/6.rgba16.png", 5052688, 12288, 4096, { 64, 32 } },
   { "gfx/levels/ttm/7.rgba16.png", 5052688, 16384, 4096, { 64, 32 } },
   { "gfx/textures/ipl3_raw/ipl3_font_00.ia1.png", -1, 2928, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_01.ia1.png", -1, 2951, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_02.ia1.png", -1, 2974, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_03.ia1.png", -1, 2997, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_04.ia1.png", -1, 3020, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_05.ia1.png", -1, 3043, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_06.ia1.png", -1, 3066, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_07.ia1.png", -1, 3089, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_08.ia1.png", -1, 3112, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_09.ia1.png", -1, 3135, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_10.ia1.png", -1, 3158, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_11.ia1.png", -1, 3181, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_12.ia1.png", -1, 3204, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_13.ia1.png", -1, 3227, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_14.ia1.png", -1, 3250, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_15.ia1.png", -1, 3273, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_16.ia1.png", -1, 3296, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_17.ia1.png", -1, 3319, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_18.ia1.png", -1, 3342, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_19.ia1.png", -1, 3365, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_20.ia1.png", -1, 3388, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_21.ia1.png", -1, 3411, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_22.ia1.png", -1, 3434, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_23.ia1.png", -1, 3457, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_24.ia1.png", -1, 3480, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_25.ia1.png", -1, 3503, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_26.ia1.png", -1, 3526, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_27.ia1.png", -1, 3549, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_28.ia1.png", -1, 3572, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_29.ia1.png", -1, 3595, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_30.ia1.png", -1, 3618, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_31.ia1.png", -1, 3641, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_32.ia1.png", -1, 3664, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_33.ia1.png", -1, 3687, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_34.ia1.png", -1, 3710, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_35.ia1.png", -1, 3733, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_36.ia1.png", -1, 3756, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_37.ia1.png", -1, 3779, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_38.ia1.png", -1, 3802, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_39.ia1.png", -1, 3825, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_40.ia1.png", -1, 3848, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_41.ia1.png", -1, 3871, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_42.ia1.png", -1, 3894, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_43.ia1.png", -1, 3917, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_44.ia1.png", -1, 3940, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_45.ia1.png", -1, 3963, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_46.ia1.png", -1, 3986, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_47.ia1.png", -1, 4009, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_48.ia1.png", -1, 4032, 23, { 13, 14 } },
   { "gfx/textures/ipl3_raw/ipl3_font_49.ia1.png", -1, 4055, 23, { 13, 14 } },
   { "gfx/textures/intro_raw/hand_closed.rgba16.png", -1, 2463112, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/hand_open.rgba16.png", -1, 2461056, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/mario_face_shine.ia8.png", -1, 2511696, 1024, { 32, 32 } },
   { "gfx/textures/intro_raw/red_star_0.rgba16.png", -1, 2465160, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/red_star_1.rgba16.png", -1, 2467208, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/red_star_2.rgba16.png", -1, 2469256, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/red_star_3.rgba16.png", -1, 2471304, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/red_star_4.rgba16.png", -1, 2473352, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/red_star_5.rgba16.png", -1, 2475400, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/red_star_6.rgba16.png", -1, 2477448, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/red_star_7.rgba16.png", -1, 2479496, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/sparkle_0.rgba16.png", -1, 2498696, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/sparkle_1.rgba16.png", -1, 2500744, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/sparkle_2.rgba16.png", -1, 2502792, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/sparkle_3.rgba16.png", -1, 2504840, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/sparkle_4.rgba16.png", -1, 2506888, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/sparkle_5.rgba16.png", -1, 2508936, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/white_star_0.rgba16.png", -1, 2481544, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/white_star_1.rgba16.png", -1, 2483592, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/white_star_2.rgba16.png", -1, 2485640, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/white_star_3.rgba16.png", -1, 2487688, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/white_star_4.rgba16.png", -1, 2489736, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/white_star_5.rgba16.png", -1, 2491784, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/white_star_6.rgba16.png", -1, 2493832, 2048, { 32, 32 } },
   { "gfx/textures/intro_raw/white_star_7.rgba16.png", -1, 2495880, 2048, { 32, 32 } },
   { "sound/sound_data.ctl", -1, 5748512, 97856, { } },
   { "sound/sound_data.tbl", -1, 5846368, 2216704, { } },
   { "sound/sequences.bin", -1, 8063072, 114112, { } },
   { "sound/bank_sets", -1, 8177184, 160, { } },
};

std::vector<SaturnAsset> saturn_assets = {
    { "gfx/textures/saturn/mario_logo.rgba16.png", mario_logo_rgba16, { 32, 32 } },
    { "gfx/textures/saturn/white.rgba16.png", white_rgba16, { 32, 32 } },
    { "gfx/textures/title_screen_bg/title_screen_bg.001C0.rgba16.png", saturn_titlescreen_1_rgba16, { 80, 20 } },
    { "gfx/textures/title_screen_bg/title_screen_bg.00E40.rgba16.png", saturn_titlescreen_2_rgba16, { 80, 20 } },
    { "gfx/textures/title_screen_bg/title_screen_bg.01AC0.rgba16.png", saturn_titlescreen_3_rgba16, { 80, 20 } },
    { "gfx/textures/title_screen_bg/title_screen_bg.02740.rgba16.png", saturn_titlescreen_4_rgba16, { 80, 20 } },
    { "gfx/actors/mario/no_m.rgba16.png", mario_no_m_rgba16, { 32, 32 } },
    { "gfx/levels/intro/4_saturn.rgba16.png", saturn_planet_rgba16, { 32, 32 } },
    { "gfx/levels/intro/5_ring.rgba16.png", saturn_ring_rgba16, { 32, 32 } },
};

std::vector<int> bitfs_ptrtable = {
    0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 8, 9, 10,11,12,13,14,15,
    8, 9, 16,17,18,19,20,21,22,23,16,17,24,25,26,27,28,29,
    30,31,24,25,32,33,34,35,36,37,38,39,32,33,40,41,42,43,
    44,45,46,47,40,41,48,48,48,48,48,48,48,48,48,48,48,48,
    48,48,48,48,48,48,48,48
};

enum FormatEnum {
    FMT_RGBA,
    FMT_IA,
    FMT_I,
};

struct FormatTableEntry {
    FormatEnum format;
    int depth;
};

std::map<std::string, FormatTableEntry> format_table = {
    { "rgba16", (FormatTableEntry){ FMT_RGBA, 16 } },
    { "rgba32", (FormatTableEntry){ FMT_RGBA, 32 } },
    { "ia1",    (FormatTableEntry){ FMT_IA,   1  } },
    { "ia4",    (FormatTableEntry){ FMT_IA,   4  } },
    { "ia8",    (FormatTableEntry){ FMT_IA,   8  } },
    { "ia16",   (FormatTableEntry){ FMT_IA,   16 } },
    { "i4",     (FormatTableEntry){ FMT_I,    4  } },
    { "i8",     (FormatTableEntry){ FMT_I,    8  } }
};

#define EXTRACT_PATH "res"

struct mio0_header {
    unsigned int dest_size;
    unsigned int comp_offset;
    unsigned int uncomp_offset;
};

mio0_header read_mio0_header(unsigned char* data) {
    int ptr = 4;
    mio0_header header;
#define READ_U32 (data[ptr++] << 24) | (data[ptr++] << 16) | (data[ptr++] << 8) | data[ptr++]
    header.dest_size = READ_U32;
    header.comp_offset = READ_U32;
    header.uncomp_offset = READ_U32;
#undef READ_U32
    return header;
}

unsigned char* read_mio0(unsigned char* buf) {
    int bytes_written = 0;
    int bit_idx = 0;
    int comp_idx = 0;
    int uncomp_idx = 0;
    mio0_header head = read_mio0_header(buf);
    unsigned char* out = (unsigned char*)malloc(head.dest_size);
    while (bytes_written < head.dest_size) {
        if ((&buf[16])[bit_idx / 8] & (1 << (7 - (bit_idx % 8)))) {
            out[bytes_written] = buf[head.uncomp_offset + uncomp_idx];
            bytes_written++;
            uncomp_idx++;
        }
        else {
            int idx;
            int length;
            int i;
            unsigned char* vals = &buf[head.comp_offset + comp_idx];
            comp_idx += 2;
            length = ((vals[0] & 0xF0) >> 4) + 3;
            idx = ((vals[0] & 0x0F) << 8) + vals[1] + 1;
            for (i = 0; i < length; i++) {
                out[bytes_written] = out[bytes_written - idx];
                bytes_written++;
            }
        }
        bit_idx++;
    }
    return out;
}

struct Range {
      signed char off;
    unsigned char len;
};

#define BIT(arr, bit) (((arr)[(bit) / 8] >> (7 - (bit) % 8)) & 1)

void decode_image(unsigned char* img, unsigned char* raw, int pixels, std::vector<Range> ranges) {
    int bitptr = 0;
    int ptr = 0;
    for (int i = 0; i < pixels; i++) {
        int len = 0;
        for (const auto& range : ranges) {
            if (range.off == -1) {
                img[ptr++] = range.len;
                continue;
            }
            int end = range.off + range.len;
            if (len < end) len = end;
            int value = 0;
            for (int j = 0; j < range.len; j++) {
                value = (value << 1) | BIT(raw, bitptr + range.off + j);
            }
            img[ptr++] = value * 0xFF / ((1 << range.len) - 1);
        }
        bitptr += len;
    }
}

#undef BIT

#define RANGE(_off, _len) (Range){ .off = _off, .len = _len  }
#define CONST(value)      (Range){ .off = -1  , .len = value }

unsigned char* raw2rgba(unsigned char* raw, int width, int height, int depth) {
    unsigned char* img = (unsigned char*)malloc(width * height * 4);
    switch (depth) {
        case 16:
            decode_image(img, raw, width * height, {
                RANGE(0 , 5),
                RANGE(5 , 5),
                RANGE(10, 5),
                RANGE(15, 1)
            });
            break;
        case 32:
            decode_image(img, raw, width * height, {
                RANGE(0 , 8),
                RANGE(8 , 8),
                RANGE(16, 8),
                RANGE(24, 8)
            });
            break;
    }
    return img;
}

unsigned char* raw2ia(unsigned char* raw, int width, int height, int depth) {
    unsigned char* img = (unsigned char*)malloc(width * height * 2);
    switch (depth) {
        case 1:
            decode_image(img, raw, width * height, {
                RANGE(0, 1),
                RANGE(0, 1)
            });
            break;
        case 4:
            decode_image(img, raw, width * height, {
                RANGE(0, 3),
                RANGE(3, 1)
            });
            break;
        case 8:
            decode_image(img, raw, width * height, {
                RANGE(0, 4),
                RANGE(4, 4)
            });
            break;
        case 16:
            decode_image(img, raw, width * height, {
                RANGE(0, 8),
                RANGE(8, 8)
            });
            break;
    }
    return img;
}

unsigned char* raw2i(unsigned char* raw, int width, int height, int depth) {
    unsigned char* img = (unsigned char*)malloc(width * height * 2);
    switch (depth) {
        case 4:
            decode_image(img, raw, width * height, {
                RANGE(0, 4),
                CONST(255),
            });
            break;
        case 8:
            decode_image(img, raw, width * height, {
                RANGE(0, 8),
                CONST(255),
            });
            break;
    }
    return img;
}

unsigned char* raw2skybox(unsigned char* raw, int len, int use_bitfs) {
    int table_index = len - 8 * 10 * 4;
    unsigned int table[80];
    for (int i = 0; i < 80; i++) {
        table[i] = (raw[table_index + i * 4 + 0] << 24) |
                   (raw[table_index + i * 4 + 1] << 16) |
                   (raw[table_index + i * 4 + 2] <<  8) |
                    raw[table_index + i * 4 + 3];
    }
    unsigned char* skybox = (unsigned char*)malloc(80 * 32 * 32 * 4);
    for (int i = 0; i < 80; i++) {
        // for some reason the bitfs ptr table is completely fucked
        table[i] = use_bitfs ? bitfs_ptrtable[i] * 0x800 : table[i] - table[0];
    }
    for (int i = 0; i < 80; i++) {
        decode_image(skybox + i * 32 * 32 * 4, raw + table[i], 32 * 32, {
            RANGE(0 , 5),
            RANGE(5 , 5),
            RANGE(10, 5),
            RANGE(15, 1)
        });
    }
    return skybox;
}

#undef RANGE
#undef CONST

void write_png(std::string path, void* data, int width, int height, int depth) {
    std::filesystem::path dst = std::filesystem::path(EXTRACT_PATH) / path;
    std::filesystem::create_directories(dst.parent_path());
    std::cout << "exporting " << dst << std::endl;
    pngutils_write_png(std::filesystem::absolute(dst).c_str(), width, height, depth, data, 0);
}

void skybox2png(std::string filename, unsigned char* img) {
    for (int i = 0; i < 80; i++) {
        write_png(filename + "." + std::to_string(i) + ".rgba16.png", img + i * 32 * 32 * 4, 32, 32, 4);
    }
    free(img);
}

void rgba2png(std::string filename, unsigned char* img, int width, int height) {
    write_png(filename, img, width, height, 4);
    free(img);
}

void ia2png(std::string filename, unsigned char* img, int width, int height) {
    write_png(filename, img, width, height, 2);
    free(img);
}

#define ROM_OK           0
#define ROM_NEED_EXTRACT 1
#define ROM_MISSING      2
#define ROM_INVALID      3

int saturn_rom_status(std::filesystem::path extract_dest, std::vector<std::string>* todo) {
    bool needs_extract = false;
    bool needs_rom = false;
    for (const auto& entry : assets) {
        if (entry.metadata.size() > 0 && entry.metadata[0] == -1) {
            bool missing = false;
            std::filesystem::path path = extract_dest / entry.path;
            std::string filename = path.string();
            std::string ext = path.extension().string();
            std::string name_without_ext = filename.substr(0, filename.length() - ext.length());
            for (int i = 0; i < 80; i++) {
                if (!std::filesystem::exists(name_without_ext + "." + std::to_string(i) + ".rgba16.png")) missing = true;
            }
            if (missing) {
                needs_extract = true;
                todo->push_back(entry.path);
            }
            continue;
        }
        if (!std::filesystem::exists(extract_dest / entry.path)) {
            needs_extract = true;
            todo->push_back(entry.path);
        }
    }
    needs_rom = needs_extract;
    for (const auto& entry : saturn_assets) {
        if (!std::filesystem::exists(extract_dest / entry.path)) {
            needs_extract = true;
            todo->push_back(entry.path);
        }
    }
    if (!needs_extract) return ROM_OK;
    if (!std::filesystem::exists("sm64.z64") && needs_rom) return ROM_MISSING;
    return ROM_NEED_EXTRACT;
}

int saturn_extract_rom() {
    std::filesystem::path extract_dest = EXTRACT_PATH;
    std::vector<std::string> todo = {};
    int status = saturn_rom_status(extract_dest, &todo);
    if (status == ROM_OK) return 0;
    if (status == ROM_MISSING) {
        pfd::message("ROM Extract Error", "Cannot find 'sm64.z64'.\n\nPut an unmodified, US version of SM64 into Saturn's executable directory and name it 'sm64.z64'.", pfd::choice::ok);
        return 1;
    }
    if (status == ROM_INVALID) {
        pfd::message("ROM Extract Error", "Couldn't verify 'sm64.z64'.\n\nThe file may be corrupted, extended, or from the wrong region. Use an unmodified US version of SM64.", pfd::choice::ok);
        return 1;
    }
    std::ifstream stream = std::ifstream("sm64.z64", std::ios::binary);
    unsigned char* data = (unsigned char*)malloc(1024 * 1024 * 8);
    stream.read((char*)data, 1024 * 1024 * 8);
    stream.close();
    std::map<int, unsigned char*> mio0 = {};
    for (const auto& asset : assets) {
        if (std::find(todo.begin(), todo.end(), asset.path) == todo.end()) continue;
        if (mio0.find(asset.mio0) != mio0.end()) continue;
        if (asset.mio0 == -1) mio0.insert({ -1, data });
        else mio0.insert({ asset.mio0, read_mio0(data + asset.mio0) });
    }
    for (const auto& asset : assets) {
        if (std::find(todo.begin(), todo.end(), asset.path) == todo.end()) continue;
        std::istringstream iss = std::istringstream(asset.path);
        std::vector<std::string> tokens = {};
        std::string token;
        while (std::getline(iss, token, '.')) {
            tokens.push_back(token);
        }
        unsigned char* buf = mio0[asset.mio0];
        if (tokens[tokens.size() - 1] == "png") {
            if (asset.metadata[0] == -1) {
                unsigned char* img = raw2skybox(buf + asset.pos, asset.len, asset.path == "gfx/textures/skyboxes/bitfs.png");
                skybox2png(tokens[0], img);
                continue;
            }
            FormatTableEntry format = format_table[tokens[tokens.size() - 2]];
            if (format.format == FMT_RGBA) {
                unsigned char* img = raw2rgba(buf + asset.pos, asset.metadata[0], asset.metadata[1], format.depth);
                rgba2png(asset.path, img, asset.metadata[0], asset.metadata[1]);
            }
            else if (format.format == FMT_IA) {
                unsigned char* img = raw2ia(buf + asset.pos, asset.metadata[0], asset.metadata[1], format.depth);
                ia2png(asset.path, img, asset.metadata[0], asset.metadata[1]);
            }
            else if (format.format == FMT_I) {
                unsigned char* img = raw2i(buf + asset.pos, asset.metadata[0], asset.metadata[1], format.depth);
                ia2png(asset.path, img, asset.metadata[0], asset.metadata[1]);
            }
        }
        else {
            std::filesystem::path dst = std::filesystem::path(EXTRACT_PATH) / asset.path;
            std::filesystem::create_directories(dst.parent_path());
            std::cout << "exporting " << dst << std::endl;
            std::ofstream out = std::ofstream(dst);
            out.write((char*)(buf + asset.pos), asset.len);
            out.close();
        }
    }
    for (const auto& asset : saturn_assets) {
        if (std::find(todo.begin(), todo.end(), asset.path) == todo.end()) continue;
        unsigned char* img = raw2rgba(asset.data, asset.metadata[0], asset.metadata[1], 16);
        rgba2png(asset.path, img, asset.metadata[0], asset.metadata[1]);
    }
    for (const auto& entry : mio0) {
        free(entry.second);
    }
    return 0;
}