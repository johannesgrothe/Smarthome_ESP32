#pragma once

#include "sh_fan.h"

// UNKNOWN 19496A87
static const uint16_t level_0[95] = {1252, 432, 1250, 432, 420, 1260, 422, 1262, 420, 1258, 422, 1260, 420,
                                     1258, 1254,
                                     430,
                                     420,
                                     1260, 420, 1260, 422, 1256, 424, 7992, 1252, 430, 1226, 454, 422, 1260,
                                     422, 1258,
                                     424,
                                     1256,
                                     422, 1260, 422, 1258, 1254, 428, 420, 1262, 420, 1260, 420, 1258, 422,
                                     7990, 1256,
                                     430,
                                     1226,
                                     456, 420, 1260, 422, 1258, 422, 1258, 422, 1256, 424, 1260, 1252, 430,
                                     420, 1262,
                                     418,
                                     1260,
                                     422, 1258, 422, 7990, 1254, 430, 1252, 430, 422, 1258, 420, 1258, 424,
                                     1258, 422,
                                     1260,
                                     420,
                                     1260, 1254, 430, 418, 1260, 420, 1260, 424, 1256, 368};

// UNKNOWN ACF483A9
static const uint16_t level_1[119] = {1236, 448, 1208, 450, 390, 1308, 376, 1284, 420, 1282, 400, 1260, 448,
                                      1232, 392,
                                      1310,
                                      398,
                                      1282, 426, 1256, 372, 1306, 1212, 7180, 1236, 466, 1238, 442, 398,
                                      1282, 396, 1286,
                                      396,
                                      1282, 396, 1286, 398, 1282, 372, 1310, 396, 1284, 398, 1284, 396,
                                      1260, 1260, 7176,
                                      1236,
                                      444, 1240, 438, 372, 1310, 370, 1310, 374, 1306, 346, 1314, 420, 1260,
                                      394, 1310,
                                      394,
                                      1284,
                                      394, 1286, 372, 1306, 1236, 7158, 1258, 442, 1262, 418, 394, 1288,
                                      398, 1282, 396,
                                      1262,
                                      392,
                                      1310, 396, 1286, 418, 1236, 420, 1282, 372, 1290, 418, 1260, 1260,
                                      7174, 1236, 446,
                                      1210,
                                      470, 370, 1312, 396, 1260, 394, 1308, 370, 1290, 418, 1262, 446, 1256,
                                      396, 1284,
                                      394,
                                      1286,
                                      370, 1284, 1236};

// UNKNOWN 78C22A70
static const uint16_t level_3[95] = {1264, 418, 1210, 470, 398, 1284, 396, 1266, 420, 1258, 1284, 418, 374,
                                     1308, 372,
                                     1288,
                                     418,
                                     1284, 398, 1282, 1264, 418, 1240, 7154, 1256, 422, 1262, 442, 368,
                                     1292, 392, 1290,
                                     418,
                                     1280,
                                     1238, 444, 372, 1308, 370, 1310, 372, 1306, 422, 1258, 1218, 464, 1210,
                                     7204, 1210,
                                     476,
                                     1232,
                                     442, 424, 1258, 398, 1262, 418, 1280, 1240, 442, 398, 1260, 394, 1288,
                                     416, 1260,
                                     422,
                                     1282,
                                     1238, 442, 1240, 7176, 1208, 470, 1238, 424, 420, 1280, 424, 1236, 392,
                                     1306, 1212,
                                     450,
                                     390,
                                     1312, 370, 1310, 372, 1308, 396, 1264, 1258, 444, 1238};

class SH_Fan_Westinghouse_IR : public SH_Fan {
protected:

public:

  explicit SH_Fan_Westinghouse_IR(const JsonObject&);

  void refresh() override;
};
