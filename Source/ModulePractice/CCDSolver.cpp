// Fill out your copyright notice in the Description page of Project Settings.


#include "CCDSolver.h"

namespace AnimationCore
{
	bool SolveCCDIK(TArray<FCCDSolver>& InOutChain, const FVector& TargetPosition, float Precision, int32 MaxIteration)
	{
		struct Local
		{
			static bool UpdateChainLink(TArray<FCCDSolver>& Chain, int32 LinkIndex, const FVector& TargetPos)
			{
				int32 const TipBoneLinkIndex = Chain.Num() - 1;
				FCCDSolver& CurrentLink = Chain[LinkIndex];

				// update new tip position
				FVector TipPosition = Chain[TipBoneLinkIndex].Transform.GetLocation();
				
				FTransform& CurrentLinkTransform = CurrentLink.Transform;
				FVector ToEnd = TipPosition - CurrentLinkTransform.GetLocation();
				FVector ToTarget = TargetPos - CurrentLinkTransform.GetLocation();

				ToEnd.Normalize();
				ToTarget.Normalize();

				float Angle = FMath::Acos(FVector::DotProduct(ToEnd, ToTarget));
				CurrentLink.CurrentAngleDelta += Angle;

				FVector RotationAxis = FVector::CrossProduct(ToEnd, ToTarget);
				if (RotationAxis.SizeSquared() > 0.f)
				{
					RotationAxis.Normalize();
					FQuat DeltaRotation(RotationAxis, Angle);
					FQuat NewRotation = DeltaRotation * CurrentLinkTransform.GetRotation();
					NewRotation.Normalize();
					CurrentLinkTransform.SetRotation(NewRotation);

					// if this node has parent, then make sure to refresh local transform
					if (LinkIndex > 0)
					{
						const FCCDSolver& Parent = Chain[LinkIndex - 1];
						CurrentLink.LocalTransform = CurrentLinkTransform.GetRelativeTransform(Parent.Transform);
						CurrentLink.LocalTransform.NormalizeRotation();
					}

					// update all children nodes
					FTransform CurrentParentTransform = CurrentLinkTransform;
					for (int32 ChildLinkIndex = LinkIndex + 1; ChildLinkIndex <= TipBoneLinkIndex; ++ChildLinkIndex)
					{
						FCCDSolver& ChildIterLink = Chain[ChildLinkIndex];
						const FTransform LocalTransform = ChildIterLink.LocalTransform;
						ChildIterLink.Transform = LocalTransform * CurrentParentTransform;
						ChildIterLink.Transform.NormalizeRotation();
						CurrentParentTransform = ChildIterLink.Transform;
					}

					return true;
				}

				return false;
			}
		};

		bool bBoneLocationUpdated = false;
		const int32 NumChainLinks = InOutChain.Num();

		// iteration
		const int32 TipBoneLinkIndex = NumChainLinks - 1;
		bool bLocalUpdated = false;

		const FVector TargetPos = TargetPosition;
		FVector TipPos = InOutChain[TipBoneLinkIndex].Transform.GetLocation();
		float Distance = FVector::Dist(TargetPos, TipPos);
		int32 IterationCount = 0;
		while ((Distance > Precision) && (IterationCount++ < MaxIteration))
		{
			for (int32 LinkIndex = TipBoneLinkIndex - 1; LinkIndex > 0; --LinkIndex)
			{
				bLocalUpdated |= Local::UpdateChainLink(InOutChain, LinkIndex, TargetPos);
			}

			Distance = FVector::Dist(InOutChain[TipBoneLinkIndex].Transform.GetLocation(), TargetPosition);

			bBoneLocationUpdated |= bLocalUpdated;

			if (!bLocalUpdated)
				break;
		}

		return bBoneLocationUpdated;
	}
}