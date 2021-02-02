// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "CCDSolver.h"
#include "AnimNode_CCDPractice.generated.h"

USTRUCT()
struct ANIMGRAPHRUNTIME_API FAnimNode_CCDPractice : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Effector, meta = (PinShownByDefault))
		FVector EffectorLocation;

	UPROPERTY(EditAnywhere, Category = Effector)
		TEnumAsByte<enum EBoneControlSpace> EffectorLocationSpace;

	UPROPERTY(EditAnywhere, Category = Effector)
		FBoneSocketTarget EffectorTarget;

	UPROPERTY(EditAnywhere, Category = Solver)
		FBoneReference TipBone;

	UPROPERTY(EditAnywhere, Category = Solver)
		FBoneReference RootBone;

	UPROPERTY(EditAnywhere, Category = Solver)
		float Precision;

	UPROPERTY(EditAnywhere, Category = Solver)
		int32 MaxIteration;

public:
	FAnimNode_CCDPractice();

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

private:
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;

	static FTransform GetTargetTransform(const FTransform& InComponentTransform, FCSPose<FCompactPose>& MeshBases, FBoneSocketTarget& InTarget, EBoneControlSpace Space, const FVector& InOffset);
};