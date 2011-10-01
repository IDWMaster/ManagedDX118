#ifndef MatrixData
#define MatrixData
#include "pch.h"
#include "BasicMath.h"

class GPUMatrixData {
	public:
		float4x4 model;
		float4x4 view;
		float4x4 projection;
	};
#endif