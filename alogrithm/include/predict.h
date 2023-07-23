#ifndef PREDICT_H
#define PREDICT_H
#include <librealsense2/rs.hpp>

extern rs2_pose predict_pose(rs2_pose & pose, float dt_s);

#endif //PREDICT_H