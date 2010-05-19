// kate: indent-mode cstyle; indent-width 4; tab-width 4; space-indent false;
// vim:ai ts=4 et
#pragma once

#include <stdint.h>

#include <Geometry2d/Point.hpp>
#include <cblas.h>
#include "BLASWrap/blaswrap.h"
#include "difference_kalman.hpp"
#include "RobotModel.hpp"
#include <vector>

/* RBPF Includes */
#include <iostream>
#include <fstream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "Rbpf.hpp"
#include "RbpfState.hpp"
#include "RbpfModel.hpp"
#include "RbpfModelRolling.hpp"
#include "RbpfModelKicked.hpp"

typedef Geometry2d::Point Point;
typedef boost::numeric::ublas::vector<double> Vector;
typedef boost::numeric::ublas::matrix<double> Matrix;

namespace Modeling
{
	class BallModel
	{
		public:
			// Maximum time to coast a track (keep the track alive with no observations) in microseconds.
			static const uint64_t MaxCoastTime = 500000;

			typedef enum {
				MODELTESTS,
				RBPF,
				KALMAN,
				ABG
			} mode_t;

			typedef enum {
				VISION,
				BALL_SENSOR
			} observation_mode;

			BallModel(mode_t mode, RobotModel::RobotMap *robotMap);

			void observation(uint64_t time, const Geometry2d::Point &pos, observation_mode obs_mode);

			bool valid(uint64_t time);

			Geometry2d::Point predictPosAtTime(float dtime);
			void update(uint64_t cur_time);

			// Current observation - FIXME: remove this
			Geometry2d::Point observedPos;

			/** Kalman **/
			//State Transistion Matrix
			DMatrix A;
			//Input Transistion Matrix
			DMatrix B;
			//Initial Covariance Matrix
			DMatrix P;
			//Process Covariance Matrix
			DMatrix Q;
			//Measurement Covariance Matrix
			DMatrix R;
			//Measurement Model
			DMatrix H;
			//Measurement
			DVector Z;
			//Input
			DVector U;
			//Initial Condition
			DVector X0;

			//Difference Kalman Filter
			DifferenceKalmanFilter *posKalman;

			/** Alpha Beta Gamma **/
			// Current filtered state
			Geometry2d::Point pos;
			Geometry2d::Point abgPos;
			Geometry2d::Point vel;
			Geometry2d::Point accel;

			// Filter coefficients
			float alpha;
			float beta;
			float gamma;

			uint64_t lastObservedTime;
			int missedFrames; // AGC: remember that number of frames missed varies with framerate

			Geometry2d::Point prevObservedPos;

			// filter errors
			float kalmanTestPosError;
			float rbpfTestPosError;
			float kalmanTestVelError;
			float rbpfTestVelError;

		protected:
			typedef struct {
				uint64_t time;
				Geometry2d::Point pos;
				observation_mode obs_type;
			} observation_type;

			std::vector<observation_type> _observations;

			// mode of the filter
			mode_t _mode;

			// map of robots so filters can include this information
			RobotModel::RobotMap *_robotMap;

			// new particle filter implementation
			Rbpf* raoBlackwellizedParticleFilter;

			// Initialization functions for each model

			/**
			 * Initialize Rao-Blackwellized Particle Filter
			 *   Constructs initial state X, initial covariance P, adds several models
			 *   to the modelGraph, and sets some transition weights
			 */
			void initRBPF();

			/** Initialize the older implementation of the ball filter */
			void initKalman();

			/** Initializes the Alpha-Beta-Gamma filter */
			void initABG();

			// update functions

			/** Old-style Kalman update */
			void kalmanUpdate(float dtime);

			/** Newer RBPF update */
			void rbpfUpdate(float dtime);

			/** ABG Filter update */
			void abgUpdate(float dtime);

	};
}
