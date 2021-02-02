// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Objectmacros.h"
#include "BoneIndices.h"
#include "CCDSolver.generated.h"

USTRUCT()
struct FCCDSolver
{
	GENERATED_USTRUCT_BODY()

public:
	FTransform Transform;
	FTransform LocalTransform;

	int32 TransformIndex;

	float CurrentAngleDelta;
	TArray<int32> ChildZeroLengthTransformIndices;
	FCCDSolver()
		: TransformIndex(INDEX_NONE)
		, CurrentAngleDelta(0.f)
	{
	}

	FCCDSolver(const FTransform& InTransform, const FTransform& InLocalTransform, const int32& InTransformIndex)
		: Transform(InTransform)
		, LocalTransform(InLocalTransform)
		, TransformIndex(InTransformIndex)
		, CurrentAngleDelta(0.f)
	{
	}
};

namespace AnimationCore
{
	ANIMATIONCORE_API bool SolveCCDIK(TArray<FCCDSolver>& InOutChain, const FVector& TargetPosition, float Precision, int32 MaxIteration);
};
