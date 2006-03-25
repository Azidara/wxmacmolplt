/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

#include "Globals.h"
#include "Progress.h"
#include "MyTypes.h"
#include "Frame.h"
#include "MoleculeData.h"
#include "BasisSet.h"
#include "GlobalExceptions.h"
#include "SurfaceTypes.h"
//#include "ChipmanSurface.h"
#include "Math3D.h"
#include "VirtualSphere.h"
#include "InputData.h"
#include "Prefs.h"
#include <string.h>
#include <stdio.h>
#include <sstream>
#include "CML.h"
#include "XML.hpp"
#include "MMPPrefs.h"

void OrthogonalizeMatrix(Matrix4D a);

//Applies a rotation of the specified number of degrees to the specified axis.
//Axis=(0 for x, 1 for y, 2 for z)
void ApplyRotation(Matrix4D RotMat, long Axis, float AngleDegrees) {
	CPoint3D	InitialTrans;
//First back rotate the translation to get the inital translation
	InitialTrans.x = RotMat[3][0];
	InitialTrans.y = RotMat[3][1];
	InitialTrans.z = RotMat[3][2];
//Now zero out the translation part of the matrix
	RotMat[3][0] = RotMat[3][1] = RotMat[3][2] = 0.0;
//Now calculate the rotation
	float AngleRads = AngleDegrees * kPi/180.0;
	float cosAngle = cos(AngleRads);
	float sinAngle = sin(AngleRads);
	Matrix4D	TempRot, TempCopyMat;
	for (long ii=0; ii<4; ii++) {
		TempRot[ii][ii] = 1.0;
		for (long jj=0; jj<ii; jj++) {
			TempRot[ii][jj] = 0.0;
			TempRot[jj][ii] = 0.0;
		}
	}
	switch (Axis) {
		case 0:	//rotate about the x-axis
			TempRot[1][1] = cosAngle;
			TempRot[2][2] = cosAngle;
			TempRot[1][2] = -sinAngle;
			TempRot[2][1] = sinAngle;
		break;
		case 1:	//rotate about the y-axis
			TempRot[0][0] = cosAngle;
			TempRot[2][2] = cosAngle;
			TempRot[0][2] = sinAngle;
			TempRot[2][0] = -sinAngle;
		break;
		case 2:	//rotate about the z-axis
			TempRot[0][0] = cosAngle;
			TempRot[1][1] = cosAngle;
			TempRot[0][1] = -sinAngle;
			TempRot[1][0] = sinAngle;
		break;
	}
	MultiplyMatrix (TempRot, RotMat, TempCopyMat);
	CopyMatrix (TempCopyMat, RotMat);
//Now rotate the translation to the new orientation
	RotMat[3][0] = InitialTrans.x;
	RotMat[3][1] = InitialTrans.y;
	RotMat[3][2] = InitialTrans.z;
}	/*Apply Rotation*/
void CalculateCenterOfMass(mpAtom * AtomList, long NumAtoms, float * AtomMasses, CPoint3D * Center) {
	float TotalMass=0.0;
	Center->x = Center->y = Center->z = 0.0;
	for (long i=0; i<NumAtoms; i++) {
		float mass = AtomMasses[AtomList[i].GetType() - 1];
		mass *= mass;	//Masses are stored as the square root
		TotalMass += mass;
		Center->x += mass * AtomList[i].Position.x;
		Center->y += mass * AtomList[i].Position.y;
		Center->z += mass * AtomList[i].Position.z;
	}
	Center->x /= TotalMass;
	Center->y /= TotalMass;
	Center->z /= TotalMass;
}
void MinimizeDifferences(mpAtom * FixedAtoms, mpAtom * targetAtoms, long NumAtoms,
		WinPrefs * Prefs, long NumOptAtoms) {
	Matrix4D	FitMatrix, TempRotMat;
	float		SquaresValue, NewSquareValue, RotAngle;
	long		iOptAtoms, i;
	CPoint3D *	RotCoords = new CPoint3D[NumAtoms];
	if (!RotCoords) return;

	InitRotationMatrix(FitMatrix);		//zero out the fit matrix to start with...
		//Move the center of mass of the target to the same point as the fixed set
	
		CPoint3D	Center1, Center2;
	CalculateCenterOfMass(FixedAtoms, NumOptAtoms, Prefs->GetAtomMassLoc(), &Center1);
	CalculateCenterOfMass(targetAtoms, NumOptAtoms, Prefs->GetAtomMassLoc(), &Center2);
	Center1.x -= Center2.x;
	Center1.y -= Center2.y;
	Center1.z -= Center2.z;	//Apply the center of mass translation
	for (i=0; i<NumAtoms; i++) {
		targetAtoms[i].Position.x += Center1.x;
		targetAtoms[i].Position.y += Center1.y;
		targetAtoms[i].Position.z += Center1.z;
	}
	Boolean	Done;
	for (long ipass=0; ipass<4; ipass++) {
		if (ipass<3) {
			RotAngle = 10.0;
			if (ipass == 0) iOptAtoms = MIN(2, NumAtoms);
			else if (ipass == 1) iOptAtoms = MIN(3, NumAtoms);
			else iOptAtoms = NumOptAtoms;
			for (i=0; i<iOptAtoms; i++)
				Rotate3DPt(FitMatrix, targetAtoms[i].Position, &(RotCoords[i]));
			SquaresValue = CalculateSquaresValue(iOptAtoms, FixedAtoms, RotCoords);
		}
		Done = false;
		while (!Done) {
			Done = true;
			Boolean RotDone = false;
			for (long jpass=0; jpass<2; jpass++) {
				while (!RotDone) {
					RotDone = true;
					CopyMatrix (FitMatrix, TempRotMat);
					for (long ii=0; ii<3; ii++) {
						ApplyRotation(TempRotMat, ii, RotAngle);
						for (i=0; i<iOptAtoms; i++)
							Rotate3DPt(TempRotMat, targetAtoms[i].Position, &(RotCoords[i]));
						NewSquareValue = CalculateSquaresValue(iOptAtoms, FixedAtoms, RotCoords);
						if (NewSquareValue<SquaresValue) {
							RotDone = Done = false;
							SquaresValue = NewSquareValue;
							CopyMatrix (TempRotMat, FitMatrix);
						} else {
							ApplyRotation(TempRotMat, ii, -2.0*RotAngle);
							for (i=0; i<iOptAtoms; i++)
								Rotate3DPt(TempRotMat, targetAtoms[i].Position, &(RotCoords[i]));
							NewSquareValue = CalculateSquaresValue(iOptAtoms, FixedAtoms, RotCoords);
							if (NewSquareValue<SquaresValue) {
								RotDone = Done = false;
								SquaresValue = NewSquareValue;
								CopyMatrix (TempRotMat, FitMatrix);
							} else {
								CopyMatrix (FitMatrix, TempRotMat);
							}
						}
					}
	//"clean up" the rotation matrix make the rotation part orthogonal and magnitude 1
					OrthogonalizeRotationMatrix (FitMatrix);
				}
				RotAngle *= 0.1;
			}
		}
	}
//Done with this set of frames. Copy the RotCoords over on to the FileCoords array to make them permenant.

	for (i=0; i<NumAtoms; i++)
		Rotate3DPt(FitMatrix, targetAtoms[i].Position, &(RotCoords[i]));
	for (i=0; i<NumAtoms; i++)
		targetAtoms[i].Position = RotCoords[i];
	delete [] RotCoords;
}	/*MinimizeDifferences*/


VibRec::VibRec(const long & NumVibs, const long & NumAtoms) {
	init();
	Setup(NumVibs, NumAtoms);
}
VibRec::VibRec(void) {
	init();
}
VibRec::~VibRec(void) {
	if (Frequencies) delete [] Frequencies;
	if (NormMode) delete [] NormMode;
	if (Intensities) delete [] Intensities;
	if (ReducedMass) delete [] ReducedMass;
	if (RamanIntensity) delete [] RamanIntensity;
	if (Depolarization) delete [] Depolarization;
}
void VibRec::init(void) {
	NormMode = NULL;
	Frequencies = NULL;
	Intensities = NULL;
	ReducedMass = NULL;
	RamanIntensity = NULL;
	Depolarization = NULL;
	NumModes = 0;
	CurrentMode = 0;
	FreqLength = 0;
}
void VibRec::Setup(const long & NumVibs, const long & NumAtoms) {
	if (NumVibs>0) {
		NormMode = new CPoint3D[NumAtoms*NumVibs];
		Intensities = new float[NumVibs];
		Frequencies = new char[NumVibs*20];
		FreqLength = NumVibs*20;
		if (!NormMode || !Frequencies || !Intensities)
			throw MemoryError();
		if (NumVibs == 1)	//Read in the single mode string
			strcpy(Frequencies, "\11single mode");
	//		GetIndString((unsigned char *)Frequencies, kIStringID, 1);
		//init the intensities all to 1
		for (long i=0; i<NumVibs; i++) Intensities[i]=1.0;
		for (long i=0; i<(NumAtoms*NumVibs); i++) {
			NormMode[i].x = 0.0;
			NormMode[i].y = 0.0;
			NormMode[i].z = 0.0;
		}
		NumModes = NumVibs;
	}
}
bool VibRec::Resize(long NumAtoms, long Length) {	//reduce the allocation to the current # of modes
	CPoint3D * temp = new CPoint3D[NumModes*NumAtoms];
	if (temp) {
		memcpy(temp, NormMode, NumModes*NumAtoms*sizeof(CPoint3D));
		delete [] NormMode;
		NormMode = temp;
		char * tempfreq = new char[Length];
		if (tempfreq) {
			memcpy(tempfreq, Frequencies, Length*sizeof(char));
			delete [] Frequencies;
			Frequencies = tempfreq;
			FreqLength = Length;
		}
		float * tempi = new float[NumModes];
		if (tempi) {
			memcpy(tempi, Intensities, NumModes*sizeof(float));
			delete [] Intensities;
			Intensities = tempi;
		}
	} else return false;
	return true;
}
long VibRec::GetSize(BufferFile * Buffer, long NumAtoms) {
	Boolean	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = Write(Buffer, NumAtoms);
	Buffer->SetOutput(cState);
	return length;
}
VibRec * VibRec::Read(BufferFile * Buffer, long NumAtoms) {
	long	code, length, total;
	Boolean	Success=true;
	VibRec * lRec = NULL;
	
	total = Buffer->Read((Ptr) &code, sizeof(long));
	if (code == 1) {
		lRec = new VibRec(0,0);
		if (!lRec) throw MemoryError();
		total += Buffer->Read((Ptr) &lRec->NumModes, sizeof(long));
		total += Buffer->Read((Ptr) &lRec->CurrentMode, sizeof(long));
		while (Buffer->BytesLeftInBlock()) {
			total += Buffer->Read((Ptr) &code, sizeof(long));
			total += Buffer->Read((Ptr) &length, sizeof(long));
			switch (code) {
				case 2:	//Normal modes
					code = lRec->NumModes*NumAtoms*sizeof(CPoint3D);
					if (length == code) {
						lRec->NormMode = new CPoint3D[lRec->NumModes*NumAtoms];
						if (!lRec->NormMode) throw MemoryError();
						total += Buffer->Read((Ptr) lRec->NormMode, length);
					} else {
						Success = false;
						break;
					}
				break;
				case 3:	//frequencies
					lRec->Frequencies = new char[length];
					if (!lRec->Frequencies) throw MemoryError();
					lRec->FreqLength = length;
					total += Buffer->Read((Ptr) lRec->Frequencies, length);
				break;
				case 4:	//IR intensities
					lRec->Intensities = new float[lRec->NumModes];
					if (!lRec->Intensities) throw MemoryError();
					total += Buffer->Read((Ptr) lRec->Intensities, length);
				break;
				case 5:	//Reduced Mass
					if (length == (lRec->NumModes * sizeof(float))) {
						lRec->ReducedMass = new float[lRec->NumModes];
						total += Buffer->Read((Ptr) lRec->ReducedMass, length);
					}
				break;
				case 6:	//Raman Intensities
					if (length == (lRec->NumModes * sizeof(float))) {
						lRec->RamanIntensity = new float[lRec->NumModes];
						total += Buffer->Read((Ptr) lRec->RamanIntensity, length);
					}
					break;
				case 7:	//Depolarization
					if (length == (lRec->NumModes * sizeof(float))) {
						lRec->Depolarization = new float[lRec->NumModes];
						total += Buffer->Read((Ptr) lRec->Depolarization, length);
					}
					break;
				default:
					Buffer->BufferSkip(length);
			}
		}
	}
	if (lRec && !Success) {	//A data error occured so delete the vibs
		delete lRec;
		lRec = NULL;
	}
	return lRec;
}
long VibRec::Write(BufferFile * Buffer, long NumAtoms) {
	long total, length, code;
	
	code = 1;
	total = Buffer->Write((Ptr) &code, sizeof(long));
	total += Buffer->Write((Ptr) &NumModes, sizeof(long));
	total += Buffer->Write((Ptr) &CurrentMode, sizeof(long));
	if (NormMode) {
		code = 2;
		total += Buffer->Write((Ptr) &code, sizeof(long));
		length = NumModes*NumAtoms*sizeof(CPoint3D);
		total += Buffer->Write((Ptr) &length, sizeof(long));
		total += Buffer->Write((Ptr) NormMode, length);
		if (Frequencies) {
			code = 3;
			total += Buffer->Write((Ptr) &code, sizeof(long));
			total += Buffer->Write((Ptr) &FreqLength, sizeof(long));
			total += Buffer->Write((Ptr) Frequencies, FreqLength);
		}
		if (Intensities) {
			code = 4;
			total += Buffer->Write((Ptr) &code, sizeof(long));
			length = NumModes * sizeof(float);
			total += Buffer->Write((Ptr) &length, sizeof(long));
			total += Buffer->Write((Ptr) Intensities, length);
		}
		if (ReducedMass) {
			code = 5;
			total += Buffer->Write((Ptr) &code, sizeof(long));
			length = NumModes * sizeof(float);
			total += Buffer->Write((Ptr) &length, sizeof(long));
			total += Buffer->Write((Ptr) ReducedMass, length);
		}
		if (RamanIntensity) {
			code = 6;
			total += Buffer->Write((Ptr) &code, sizeof(long));
			length = NumModes * sizeof(float);
			total += Buffer->Write((Ptr) &length, sizeof(long));
			total += Buffer->Write((Ptr) RamanIntensity, length);
		}
		if (Depolarization) {
			code = 7;
			total += Buffer->Write((Ptr) &code, sizeof(long));
			length = NumModes * sizeof(float);
			total += Buffer->Write((Ptr) &length, sizeof(long));
			total += Buffer->Write((Ptr) Depolarization, length);
		}
	}
	return total;
}
VibRec * VibRec::ReadOldVibRec45(BufferFile * Buffer) {
	VibRec * tVib = new VibRec(0,0);
	if (!tVib) throw MemoryError();
		long junk;
	Buffer->Read((Ptr) &junk, sizeof(long));
	Buffer->Read((Ptr) &junk, sizeof(long));
	Buffer->Read((Ptr) &(tVib->NumModes), sizeof(long));
	Buffer->Read((Ptr) &(tVib->CurrentMode), sizeof(long));
	if (Buffer->BytesLeftInBlock() > 0)
		Buffer->Read((Ptr) &(tVib->FreqLength), sizeof(long));
	else tVib->FreqLength = 0;
	return tVib;
}
void VibRec::ReadCode46(BufferFile * Buffer, long length) {
	if (Frequencies) delete [] Frequencies;
	Frequencies = new char[length];
	if (!Frequencies) throw MemoryError();
	Buffer->Read((Ptr) Frequencies, length);
	FreqLength = length;
}
void VibRec::ReadCode47(BufferFile * Buffer, long NumAtoms, long length) {
	if (NormMode) delete [] NormMode;
	long tModes = MAX(NumModes, length/(NumAtoms*sizeof(CPoint3D)));
	NormMode = new CPoint3D[tModes*NumAtoms*sizeof(CPoint3D)];
	if (!NormMode) throw MemoryError();
	Buffer->Read((Ptr) NormMode, length);
	if (NumModes <= 0)
		NumModes = tModes;
}
float VibRec::GetIntensity(long Mode) const {
	float result = 1.0;	//default to 1.0
	if (Intensities) result = Intensities[Mode];
	return result;
}
float VibRec::GetRamanIntensity(long Mode) const {
	float result = 0.0;
	if (RamanIntensity) result = RamanIntensity[Mode];
	return result;
}
float VibRec::GetFrequency(long Mode) const {
	float result = 0.0;
	if (Frequencies && (Mode >= 0) && (Mode < NumModes)) {
		long FreqPos = 0;
		for (long i=0; i<Mode; i++)
			FreqPos += 1 + Frequencies[FreqPos];
		char token[kMaxLineLength];
		strncpy(token, (char *) &(Frequencies[FreqPos+1]), Frequencies[FreqPos]);
		token[Frequencies[FreqPos]] = 0;
		sscanf(token, "%f", &result);
		if (token[Frequencies[FreqPos]-1] == 'i' || token[Frequencies[FreqPos]-1] == 'I')
			result *= -1.0;
	}
	return result;
}

bool Orb2DSurface::Needs2DPlane(void) const {
	bool result = false;
	if (Visible && (PlotOrb>=0)) result = true;
	return result;
}

void Orb2DSurface::Update(MoleculeData * MainData) {
	if (Visible && (PlotOrb>=0)) {
		Progress * lProgress = new Progress;
		if (lProgress) {
			lProgress->ChangeText("Calculating MO Grid...");
			if (GetRotate2DMap()) SetPlaneToScreenPlane(MainData);
			CalculateMOGrid(MainData, lProgress);
			delete lProgress;
		}
	}
}
void Orb2DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kOrb2DType));
	
	Surf2DBase::Write2DXML(sElem, false);
	OrbSurfBase::WriteXML(sElem);
}
Orb2DSurface::Orb2DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_2DSurface:
					Surf2DBase::Read2DXML(child);
					break;
				case MMP_OrbSurfBase:
					OrbSurfBase::ReadXML(child);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void Orb3DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kOrb3DType));
	
	Surf3DBase::Write3DXML(sElem, false);
	OrbSurfBase::WriteXML(sElem);
}
Orb3DSurface::Orb3DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_3DSurface:
					Surf3DBase::Read3DXML(child);
					break;
				case MMP_OrbSurfBase:
					OrbSurfBase::ReadXML(child);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void General2DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kGeneral2DSurface));
	
	Surf2DBase::Write2DXML(sElem, true);
}
General2DSurface::General2DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_2DSurface:
					Surf2DBase::Read2DXML(child);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void General3DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kGeneral3DSurface));
	
	Surf3DBase::Write3DXML(sElem, true);
}
General3DSurface::General3DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_3DSurface:
					Surf3DBase::Read3DXML(child);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void TEDensity2DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kTotalDensity2D));
	
	Surf2DBase::Write2DXML(sElem, false);
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}
TEDensity2DSurface::TEDensity2DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_2DSurface:
					Surf2DBase::Read2DXML(child);
					break;
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
			}
		}
		child = child->getNextChild();
	}
}
void TEDensity3DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kTotalDensity3D));
	
	Surf3DBase::Write3DXML(sElem, false);
	{
		std::ostringstream b;
		b << MaxMEPValue;
		sElem->addChildElement(CML_convert(MMP_SurfMaxMEPValue), b.str().c_str());
	}
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}
TEDensity3DSurface::TEDensity3DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_3DSurface:
					Surf3DBase::Read3DXML(child);
					break;
				case MMP_SurfMaxMEPValue:
				{
					double tf;
					if (child->getDoubleValue(tf))
						MaxMEPValue = tf;
				}
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
					break;
			}
		}
		child = child->getNextChild();
	}
}
void MEP2DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kMEP2D));
	
	Surf2DBase::Write2DXML(sElem, true);
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}
MEP2DSurface::MEP2DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_2DSurface:
					Surf2DBase::Read2DXML(child);
					break;
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
			}
		}
		child = child->getNextChild();
	}
}
void MEP3DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kMEP3D));
	
	Surf3DBase::Write3DXML(sElem, true);
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}
MEP3DSurface::MEP3DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_3DSurface:
					Surf3DBase::Read3DXML(child);
					break;
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
					break;
			}
		}
		child = child->getNextChild();
	}
}
void Surf2DBase::Write2DXML(XMLElement * parent, bool writeGrid) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_2DSurface));
	
	Surface::WriteXML(sElem);
	
	char line[kMaxLineLength];
	XMLElement * orig = sElem->addChildElement(CML_convert(MMP_SurfOrigin));
	snprintf(line, kMaxLineLength, "%f", Origin.x);
	orig->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Origin.y);
	orig->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Origin.z);
	orig->addAttribute(CML_convert(Z3Attr), line);

	XMLElement * xi = sElem->addChildElement(CML_convert(MMP_SurfXInc));
	snprintf(line, kMaxLineLength, "%f", XInc.x);
	xi->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", XInc.y);
	xi->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", XInc.z);
	xi->addAttribute(CML_convert(Z3Attr), line);

	XMLElement * yi = sElem->addChildElement(CML_convert(MMP_SurfYInc));
	snprintf(line, kMaxLineLength, "%f", YInc.x);
	yi->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", YInc.y);
	yi->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", YInc.z);
	yi->addAttribute(CML_convert(Z3Attr), line);
		
	snprintf(line, kMaxLineLength, "%f", GridMax);
	sElem->addChildElement(CML_convert(MMP_SurfGridMax), line);
	snprintf(line, kMaxLineLength, "%f", GridMin);
	sElem->addChildElement(CML_convert(MMP_SurfGridMin), line);
	snprintf(line, kMaxLineLength, "%d", NumGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumGridPoints), line);
	snprintf(line, kMaxLineLength, "%d", NumContours);
	sElem->addChildElement(CML_convert(MMP_SurfNumContours), line);
	snprintf(line, kMaxLineLength, "%f", MaxContourValue);
	sElem->addChildElement(CML_convert(MMP_SurfMaxContourValue), line);

	XMLElement * color = sElem->addChildElement(CML_convert(MMP_SurfPosColor));
	snprintf(line, kMaxLineLength, "%f", (PosColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);

	color = sElem->addChildElement(CML_convert(MMP_SurfNegColor));
	snprintf(line, kMaxLineLength, "%f", (NegColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);
	
	sElem->addChildElement(CML_convert(MMP_SurfShowZeroContour), (GetShowZeroContour()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfScreenPlane), (GetRotate2DMap()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfDashedContours), (GetDashLine()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfPosNegContours), (ContourBothPosNeg()?trueXML:falseXML));
	
	if (writeGrid && Grid) {
		std::ostringstream buf;
		long len = NumGridPoints*NumGridPoints;
#ifdef UseHandles
		HLock(Grid);
		for (int i=0; i<len; i++)
			buf << ((float *)(*Grid))[i] << " ";
		HUnlock(Grid);
#else
		for (int i=0; i<len; i++)
			buf << Grid[i] << " ";
#endif
		sElem->addChildElement(CML_convert(MMP_SurfGrid), buf.str().c_str());
	}
}
void Surf2DBase::Read2DXML(XMLElement * parent) {
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			float tf;
			double td;
			long tl;
			bool tb;
			switch (type) {
				case MMP_BaseSurface:
					Surface::ReadXML(child);
					break;
				case MMP_SurfOrigin:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						Origin.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						Origin.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						Origin.z = tf;
					break;
				case MMP_SurfXInc:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						XInc.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						XInc.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						XInc.z = tf;
					break;
				case MMP_SurfYInc:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						YInc.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						YInc.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						YInc.z = tf;
					break;
				case MMP_SurfGridMax:
					if (child->getDoubleValue(td))
						GridMax = td;
					break;
				case MMP_SurfGridMin:
					if (child->getDoubleValue(td))
						GridMin = td;
					break;
				case MMP_SurfNumGridPoints:
					if (child->getLongValue(tl))
						NumGridPoints = tl;
					break;
				case MMP_SurfNumContours:
					if (child->getLongValue(tl))
						NumContours = tl;
					break;
				case MMP_SurfMaxContourValue:
					if (child->getDoubleValue(td))
						MaxContourValue = td;
					break;
				case MMP_SurfPosColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						PosColor.red = (short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						PosColor.green = (short)(tf*65535.0);
						if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
							PosColor.blue = (short)(tf*65535.0);
							break;
				case MMP_SurfNegColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						NegColor.red = (short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						NegColor.green = (short)(tf*65535.0);
						if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
							NegColor.blue = (short)(tf*65535.0);
							break;
				case MMP_SurfShowZeroContour:
					if (child->getBoolValue(tb))
						SetShowZeroContour(tb);
					break;
				case MMP_SurfScreenPlane:
					if (child->getBoolValue(tb))
						SetRotate2DMap(tb);
					break;
				case MMP_SurfDashedContours:
					if (child->getBoolValue(tb))
						SetDashLine(tb);
					break;
				case MMP_SurfContourPosNeg:
					if (child->getBoolValue(tb))
						SetContourBothPosNeg(tb);
					break;
				case MMP_SurfGrid:
					if (NumGridPoints > 0) {
						long count = NumGridPoints*NumGridPoints;
						AllocateGrid(count);
#ifdef UseHandles
						HLock(Grid);
						long test = child->getFloatArray(count, (float *) (*Grid));
						HUnlock(Grid);
#else
						long test = child->getFloatArray(count, Grid);
#endif
						if (test != count) FreeGrid();
					}
					break;
			}
		}
		child = child->getNextChild();
	}
}
void Surf3DBase::Write3DXML(XMLElement * parent, bool writeGrid) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_3DSurface));
	
	Surface::WriteXML(sElem);
	
	char line[kMaxLineLength];
	XMLElement * orig = sElem->addChildElement(CML_convert(MMP_SurfOrigin));
	snprintf(line, kMaxLineLength, "%f", Origin.x);
	orig->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Origin.y);
	orig->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Origin.z);
	orig->addAttribute(CML_convert(Z3Attr), line);
	
	snprintf(line, kMaxLineLength, "%f", GridMax);
	sElem->addChildElement(CML_convert(MMP_SurfGridMax), line);
	snprintf(line, kMaxLineLength, "%f", GridMin);
	sElem->addChildElement(CML_convert(MMP_SurfGridMin), line);
	snprintf(line, kMaxLineLength, "%f", XGridInc);
	sElem->addChildElement(CML_convert(MMP_SurfXInc), line);
	snprintf(line, kMaxLineLength, "%f", YGridInc);
	sElem->addChildElement(CML_convert(MMP_SurfYInc), line);
	snprintf(line, kMaxLineLength, "%f", ZGridInc);
	sElem->addChildElement(CML_convert(MMP_SurfZInc), line);

	snprintf(line, kMaxLineLength, "%d", NumXGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumXGridPoints), line);
	snprintf(line, kMaxLineLength, "%d", NumYGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumYGridPoints), line);
	snprintf(line, kMaxLineLength, "%d", NumZGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumZGridPoints), line);
	snprintf(line, kMaxLineLength, "%d", NumGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumGridPoints), line);

	XMLElement * color = sElem->addChildElement(CML_convert(MMP_SurfPosColor));
	snprintf(line, kMaxLineLength, "%f", (PosColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);
	
	color = sElem->addChildElement(CML_convert(MMP_SurfNegColor));
	snprintf(line, kMaxLineLength, "%f", (NegColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);

	color = sElem->addChildElement(CML_convert(MMP_SurfTransColor));
	snprintf(line, kMaxLineLength, "%f", (TranspColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (TranspColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (TranspColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);

	snprintf(line, kMaxLineLength, "%f", ContourValue);
	sElem->addChildElement(CML_convert(MMP_SurfContourValue), line);
	snprintf(line, kMaxLineLength, "%f", GridSize);
	sElem->addChildElement(CML_convert(MMP_SurfGridSize), line);

	sElem->addChildElement(CML_convert(MMP_SurfSolidSurface), (SolidSurface()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfColorizeSurface), (ColorByValue()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfUseSurfaceNormals), (UseSurfaceNormals()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfUseRGBColor), (UseRGBColoration()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfFixedGrid), (GetFixGrid()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfContourPosNeg), (ContourBothPosNeg()?trueXML:falseXML));

	if (writeGrid && Grid) {
		std::ostringstream buf;
		long len = NumGridPoints*NumGridPoints*NumGridPoints;
#ifdef UseHandles
		HLock(Grid);
		for (int i=0; i<len; i++)
			buf << ((float *)(*Grid))[i] << " ";
		HUnlock(Grid);
#else
		for (int i=0; i<len; i++)
			buf << Grid[i] << " ";
#endif
		sElem->addChildElement(CML_convert(MMP_SurfGrid), buf.str().c_str());
	}
	if (ContourHndl) {
		snprintf(line, kMaxLineLength, "%d", NumVertices);
		sElem->addChildElement(CML_convert(MMP_SurfNumVertices), line);
		snprintf(line, kMaxLineLength, "%d", NumPosContourTriangles);
		sElem->addChildElement(CML_convert(MMP_SurfNumPosTris), line);
		snprintf(line, kMaxLineLength, "%d", NumNegContourTriangles);
		sElem->addChildElement(CML_convert(MMP_SurfNumNegTris), line);
		std::ostringstream buf, vbuf;
		long length = 3*(NumPosContourTriangles + NumNegContourTriangles);
#ifdef UseHandles
		HLock(ContourHndl);
		for (int i=0; i<NumVertices; i++)
			buf << ((CPoint3D *)(*ContourHndl))[i].x << " " << ((CPoint3D *)(*ContourHndl))[i].y << " "
				<< ((CPoint3D *)(*ContourHndl))[i].z << " ";
		HUnlock(ContourHndl);
		HLock(VertexHndl);
		for (int j=0; j<length; j++)
			vbuf << ((long *)(*VertexHndl))[j] << " ";
		HUnlock(VertexHndl);
#else
		for (int i=0; i<NumVertices; i++)
			buf << ContourHndl[i].x << " " << ContourHndl[i].y << " "
				<< ContourHndl[i].z << " ";
		for (int j=0; j<length; j++)
			vbuf << VertexList[j] << " ";
#endif
		sElem->addChildElement(CML_convert(MMP_SurfVertices), buf.str().c_str());
		sElem->addChildElement(CML_convert(MMP_SurfVerticesList), vbuf.str().c_str());
		if (SurfaceNormals) {
			std::ostringstream sbuf;
#ifdef UseHandles
			 HLock(SurfaceNormals);
			 for (int i=0; i<NumVertices; i++)
				 sbuf << ((CPoint3D *)(*SurfaceNormals))[i].x << " " << ((CPoint3D *)(*SurfaceNormals))[i].y << " "
					 << ((CPoint3D *)(*SurfaceNormals))[i].z << " ";
			 HUnlock(SurfaceNormals);
#else
			 for (int i=0; i<NumVertices; i++)
				 sbuf << SurfaceNormals[i].x << " " << SurfaceNormals[i].y << " "
					 << SurfaceNormals[i].z << " ";
#endif
			 sElem->addChildElement(CML_convert(MMP_SurfNormals), sbuf.str().c_str());
		}
		if (List) {
			std::ostringstream lbuf;
#ifdef UseHandles
			HLock(List);
			for (int i=0; i<NumVertices; i++)
				lbuf << ((float *)(*List))[i] << " ";
			HUnlock(List);
#else
			for (int i=0; i<NumVertices; i++)
				lbuf << List[i] << " ";
#endif
			sElem->addChildElement(CML_convert(MMP_SurfSurfaceValues), lbuf.str().c_str());
		}
	}
}
void Surf3DBase::Read3DXML(XMLElement * parent) {
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			float tf;
			double td;
			long tl;
			bool tb;
			switch (type) {
				case MMP_BaseSurface:
					Surface::ReadXML(child);
					break;
				case MMP_SurfOrigin:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						Origin.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						Origin.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						Origin.z = tf;
					break;
				case MMP_SurfGridMax:
					if (child->getDoubleValue(td))
						GridMax = td;
					break;
				case MMP_SurfGridMin:
					if (child->getDoubleValue(td))
						GridMin = td;
					break;
				case MMP_SurfXInc:
					if (child->getDoubleValue(td))
						XGridInc = td;
					break;
				case MMP_SurfYInc:
					if (child->getDoubleValue(td))
						YGridInc = td;
					break;
				case MMP_SurfZInc:
					if (child->getDoubleValue(td))
						ZGridInc = td;
					break;
				case MMP_SurfNumXGridPoints:
					if (child->getLongValue(tl))
						NumXGridPoints = tl;
					break;
				case MMP_SurfNumYGridPoints:
					if (child->getLongValue(tl))
						NumYGridPoints = tl;
					break;
				case MMP_SurfNumZGridPoints:
					if (child->getLongValue(tl))
						NumZGridPoints = tl;
					break;
				case MMP_SurfNumGridPoints:
					if (child->getLongValue(tl))
						NumGridPoints = tl;
					break;
				case MMP_SurfPosColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						PosColor.red = (short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						PosColor.green = (short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
						PosColor.blue = (short)(tf*65535.0);
					break;
				case MMP_SurfNegColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						NegColor.red = (short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						NegColor.green = (short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
						NegColor.blue = (short)(tf*65535.0);
					break;
				case MMP_SurfTransColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						TranspColor.red = (short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						TranspColor.green = (short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
						TranspColor.blue = (short)(tf*65535.0);
					break;
				case MMP_SurfContourValue:
					if (child->getDoubleValue(td))
						ContourValue = td;
					break;
				case MMP_SurfGridSize:
					if (child->getDoubleValue(td))
						GridSize = td;
					break;
				case MMP_SurfSolidSurface:
					if (child->getBoolValue(tb))
						SolidSurface(tb);
					break;
				case MMP_SurfColorizeSurface:
					if (child->getBoolValue(tb))
						SetColorByValue(tb);
					break;
				case MMP_SurfUseSurfaceNormals:
					if (child->getBoolValue(tb))
						UseSurfaceNormals(tb);
					break;
				case MMP_SurfUseRGBColor:
					if (child->getBoolValue(tb))
						UseRGBColoration(tb);
					break;
				case MMP_SurfFixedGrid:
					if (child->getBoolValue(tb))
						SetFixGrid(tb);
					break;
				case MMP_SurfContourPosNeg:
					if (child->getBoolValue(tb))
						SetContourBothPosNeg(tb);
					break;
				case MMP_SurfGrid:
					if (NumGridPoints > 0) {
						long count = NumGridPoints*NumGridPoints*NumGridPoints;
						AllocateGrid(count);
#ifdef UseHandles
						HLock(Grid);
						long test = child->getFloatArray(count, (float *) (*Grid));
						HUnlock(Grid);
#else
						long test = child->getFloatArray(count, Grid);
#endif
						if (test != count) FreeGrid();
					}
					break;
				case MMP_SurfNumVertices:
					if (child->getLongValue(tl))
						NumVertices = tl;
					break;
				case MMP_SurfNumPosTris:
					if (child->getLongValue(tl))
						NumPosContourTriangles = tl;
					break;
				case MMP_SurfNumNegTris:
					if (child->getLongValue(tl))
						NumNegContourTriangles = tl;
					break;
				case MMP_SurfVertices:
					if (NumVertices > 0) {
						AllocateContour(NumVertices, NumPosContourTriangles+NumNegContourTriangles);
#ifdef UseHandles
						HLock(ContourHndl);
						long test = child->getFloatArray(3*NumVertices, (float *) (*ContourHndl));
						HUnlock(ContourHndl);
#else
						long test = child->getFloatArray(3*NumVertices, (float *) ContourHndl);
#endif
						if (test != (3*NumVertices)) FreeContour();
					}
					break;
				case MMP_SurfVerticesList:
					if (VertexAllocation > 0) {
						long length = 3*(NumPosContourTriangles + NumNegContourTriangles);
#ifdef UseHandles
						HLock(VertexHndl);
						long test = child->getLongArray(length, (long *) (*VertexHndl));
						HUnlock(VertexHndl);
#else
						long test = child->getLongArray(length, VertexList);
#endif
						if (test != length) FreeContour();
					}
					break;
				case MMP_SurfNormals:
					if (NumVertices > 0) {
						AllocateNormals(NumVertices);
#ifdef UseHandles
						HLock(SurfaceNormals);
						long test = child->getFloatArray(NumVertices*3, (float *) (*SurfaceNormals));
						HUnlock(SurfaceNormals);
#else
						long test = child->getFloatArray(NumVertices*3, (float *) SurfaceNormals);
#endif
						if (test != 3*NumVertices) FreeNormals();
					}
					break;
				case MMP_SurfSurfaceValues:
					if (NumVertices > 0) {
						AllocateList(NumVertices);
#ifdef UseHandles
						HLock(List);
						long test = child->getFloatArray(NumVertices, (float *) (*List));
						HUnlock(List);
#else
						long test = child->getFloatArray(NumVertices, List);
#endif
						if (test != NumVertices) FreeList();
					}
					break;
			}
		}
		child = child->getNextChild();
	}
}
void OrbSurfBase::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_OrbSurfBase));

	if (PlotOrb >= 0) {
		std::ostringstream b;
		b << PlotOrb;
		sElem->addChildElement(CML_convert(MMP_OrbSurfPlotOrb), b.str().c_str());
	}
	if (TargetSet >= 0) {
		std::ostringstream b;
		b << TargetSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
	if (UseAOs()) sElem->addChildElement(CML_convert(MMP_OrbSurfUseAOs), trueXML);
	if (UseBetaSet()) sElem->addChildElement(CML_convert(MMP_OrbSurfUseBetaOrbs), trueXML);
	if (GetPhaseChange()) sElem->addChildElement(CML_convert(MMP_OrbSurfPhaseChage), trueXML);
	if (UseSphericalHarmonics()) sElem->addChildElement(CML_convert(MMP_OrbSurfSpherical), trueXML);
	if (GetOrbOccDisplay()) sElem->addChildElement(CML_convert(MMP_OrbSurfOccNums), trueXML);
}
void OrbSurfBase::ReadXML(XMLElement * parent) {
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			long tl;
			bool tb;
			switch (type) {
				case MMP_OrbSurfPlotOrb:
					if (child->getLongValue(tl))
						PlotOrb = tl;
					break;
				case MMP_OrbSurfTargetSet:
					if (child->getLongValue(tl))
						TargetSet = tl;
					break;
				case MMP_OrbSurfUseAOs:
					if (child->getBoolValue(tb))
						UseAOs(tb);
					break;
				case MMP_OrbSurfUseBetaOrbs:
					if (child->getBoolValue(tb))
						UseBetaSet(tb);
					break;
				case MMP_OrbSurfPhaseChage:
					if (child->getBoolValue(tb))
						SetPhaseChange(tb);
					break;
				case MMP_OrbSurfSpherical:
					if (child->getBoolValue(tb))
						UseSphericalHarmonics(tb);
					break;
				case MMP_OrbSurfOccNums:
					if (child->getBoolValue(tb))
						SetOrbOccDisplay(tb);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void Surface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_BaseSurface));
	//id, label, visible
	sElem->addAttribute(CML_convert(MMP_VisibleAttr), (Visible?trueXML:falseXML));
	std::ostringstream b;
	b << ID;
	sElem->addChildElement(CML_convert(MMP_SurfaceId), b.str().c_str());
	if (Label) sElem->addChildElement(CML_convert(MMP_SurfaceLabel), Label);
}
bool Surface::ReadXML(XMLElement * parent) {
	bool vis;
	if (parent->getAttributeValue(CML_convert(MMP_VisibleAttr), vis))
		Visible = vis;
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_SurfaceId:
				{
					long temp;
					if (child->getLongValue(temp))
						ID = temp;
				}
					break;
				case MMP_SurfaceLabel:
				{
					const char * c = child->getValue();
					if (c) SetLabel(c);
				}
					break;
			}
		}
		child = child->getNextChild();
	}
	return true;
}
Surface * Surface::ReadSurface(XMLElement * parent) {
	Surface * result = NULL;
	const char * surftype = parent->getAttributeValue(CML_convert(dataTypeAttr));
	try {
		if (surftype) {
			SurfaceType t;
			if (TextToSurfaceType(surftype, t)) {
				switch (t) {
					case kOrb3DType:
						result = new Orb3DSurface(parent);
						break;
					case kOrb2DType:
						result = new Orb2DSurface(parent);
						break;
					case kTotalDensity3D:
						result = new TEDensity3DSurface(parent);
						break;
					case kTotalDensity2D:
						result = new TEDensity2DSurface(parent);
						break;
					case kMEP2D:
						result = new MEP2DSurface(parent);
						break;
					case kMEP3D:
						result = new MEP3DSurface(parent);
						break;
					case kSolventType3D:
						break;
					case kGeneral3DSurface:
						result = new General3DSurface(parent);
						break;
					case kGeneral2DSurface:
						result = new General2DSurface(parent);
						break;
				}
			}
		}
	}
	catch (...) {
		if (result) {
			delete result;
			result = NULL;
		}
	}
	return result;
}
long Orb2DSurface::Write(BufferFile * Buffer) {
	long code, length, total=0;

	code = kOrb2DType;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 1;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &ID, sizeof(long));
	total += Buffer->Write((Ptr) &Visible, sizeof(Boolean));
	total += Buffer->Write((Ptr) &PlotOrb, sizeof(long));
	total += Buffer->Write((Ptr) &Options, sizeof(long));
	total += Buffer->Write((Ptr) &NumGridPoints, sizeof(long));
	total += Buffer->Write((Ptr) &PosColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NegColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NumContours, sizeof(long));
	total += Buffer->Write((Ptr) &MaxContourValue, sizeof(float));
	total += Buffer->Write((Ptr) &SurfOptions, sizeof(long));
	total += Buffer->Write((Ptr) &Origin, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &XInc, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &YInc, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &GridMax, sizeof(float));
	total += Buffer->Write((Ptr) &GridMin, sizeof(float));
	return total;
}

MEP2DSurface::MEP2DSurface(WinPrefs * Prefs) : Surf2DBase(Prefs) {
	OrbSet = -1;
}
MEP2DSurface::MEP2DSurface(BufferFile * Buffer, long length){
	long	size;
	OrbSet = -1;
	long total = Buffer->Read((Ptr) &size, sizeof(long));
		CPoint3D		Origin;
		CPoint3D		XInc;
		CPoint3D		YInc;
		float			GridMax, GridMin;
	if (size != 1) throw DataError();
	total += Buffer->Read((Ptr) &ID, sizeof(long));
	total += Buffer->Read((Ptr) &Visible, sizeof(Boolean));
	total += Buffer->Read((Ptr) &NumGridPoints, sizeof(long));
	total += Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
	total += Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
	total += Buffer->Read((Ptr) &NumContours, sizeof(long));
	total += Buffer->Read((Ptr) &MaxContourValue, sizeof(float));
	total += Buffer->Read((Ptr) &SurfOptions, sizeof(long));
	total += Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
	total += Buffer->Read((Ptr) &XInc, sizeof(CPoint3D));
	total += Buffer->Read((Ptr) &YInc, sizeof(CPoint3D));
	total += Buffer->Read((Ptr) &GridMax, sizeof(float));
	total += Buffer->Read((Ptr) &GridMin, sizeof(float));
	if (total < length) {
		total += Buffer->Read((Ptr) &size, sizeof(long));
		if (size == 100) {
			total += Buffer->Read((Ptr) &size, sizeof(long));
			total += Buffer->Read((Ptr) &OrbSet, sizeof(long));
		}
	}
	NextSurface = NULL;
	Label = NULL;
	Grid = NULL;
	GridAllocation = 0;
	SetContourBothPosNeg(true);
}
MEP2DSurface::MEP2DSurface(MEP2DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	OrbSet = target->getTargetOrbitalSet();
	NextSurface = NULL;
	GridAllocation = 0;
	Grid = NULL;
}

void MEP2DSurface::UpdateData(MEP2DSurface * target) {
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	OrbSet = target->getTargetOrbitalSet();
}
char * MEP2DSurface::GetLabel(void) const {
		char * text=NULL;
	if (Label) text = Surface::GetLabel();
	else {
		text = new char[100];
		int nchar;
		sprintf(text, "2D Molecular Electrostatic Potential%n", &nchar);
		if (Visible) sprintf(&(text[nchar]), " Visible");
	}
	return text;
}

MEP3DSurface::MEP3DSurface(WinPrefs * Prefs) : Surf3DBase(Prefs) {
	OrbSet = -1;
	SetContourBothPosNeg(true);
}
MEP3DSurface::MEP3DSurface(BufferFile * Buffer, long length) : Surf3DBase() {
	OrbSet = -1;
		long size;
	long total = Buffer->Read((Ptr) &size, sizeof(long));
	if (size == 10) {
		total += Buffer->Read((Ptr) &size, sizeof(long));
		total += Surf3DBase::Read3D(Buffer, size);
		if (total < length) {
			total += Buffer->Read((Ptr) &size, sizeof(long));
			if (size == 100) {
				total += Buffer->Read((Ptr) &size, sizeof(long));
				total += Buffer->Read((Ptr) &OrbSet, sizeof(long));
			}
		}
	} else {
		if (size != 1) throw DataError();
		Buffer->Read((Ptr) &ID, sizeof(long));
		Buffer->Read((Ptr) &Visible, sizeof(Boolean));
		Buffer->Read((Ptr) &NumGridPoints, sizeof(long));
		Buffer->Read((Ptr) &Mode, sizeof(long));
		Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &TranspColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &ContourValue, sizeof(float));
		Buffer->Read((Ptr) &GridSize, sizeof(float));
		Buffer->Read((Ptr) &NumPosContourTriangles, sizeof(long));
		Buffer->Read((Ptr) &NumNegContourTriangles, sizeof(long));
		Buffer->Read((Ptr) &GridMax, sizeof(float));
		Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
		Buffer->Read((Ptr) &XGridInc, sizeof(float));
		Buffer->Read((Ptr) &YGridInc, sizeof(float));
		Buffer->Read((Ptr) &ZGridInc, sizeof(float));
		Buffer->Read((Ptr) &NumXGridPoints, sizeof(long));
		Buffer->Read((Ptr) &NumYGridPoints, sizeof(long));
		Buffer->Read((Ptr) &NumZGridPoints, sizeof(long));

		if (length > (sizeof(long)+sizeof(TEDensity3DSurface))) {
			Buffer->Read((Ptr) &size, sizeof(long));
			if (size == 2) {
				Buffer->Read((Ptr) &size, sizeof(long));
				if (AllocateContour(size/sizeof(CPoint3D), (NumPosContourTriangles+
					NumNegContourTriangles))) {
	#ifdef UseHandles
					HLock(ContourHndl);
					Buffer->Read((Ptr) *ContourHndl, size);
					HUnlock(ContourHndl);
					for (long i=0; i<(NumPosContourTriangles+NumNegContourTriangles); i++) {
						((long *) *VertexHndl)[3*i] = 3*i;
						((long *) *VertexHndl)[3*i+1] = 3*i+1;
						((long *) *VertexHndl)[3*i+2] = 3*i+2;
					}
	#else
					Buffer->Read((Ptr) ContourHndl, size);
					for (long i=0; i<(NumPosContourTriangles+NumNegContourTriangles); i++) {
						VertexList[3*i] = 3*i;
						VertexList[3*i+1] = 3*i+1;
						VertexList[3*i+2] = 3*i+2;
					}
	#endif
					NumVertices = 3*(NumPosContourTriangles+
						NumNegContourTriangles);
				}
			}
		}
		SetContourBothPosNeg(true);
	}
}


long MEP2DSurface::Write(BufferFile * Buffer) {
	long code, length, total=0;

	code = kMEP2D;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 1;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &ID, sizeof(long));
	total += Buffer->Write((Ptr) &Visible, sizeof(Boolean));
	total += Buffer->Write((Ptr) &NumGridPoints, sizeof(long));
	total += Buffer->Write((Ptr) &PosColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NegColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NumContours, sizeof(long));
	total += Buffer->Write((Ptr) &MaxContourValue, sizeof(float));
	total += Buffer->Write((Ptr) &SurfOptions, sizeof(long));
	total += Buffer->Write((Ptr) &Origin, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &XInc, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &YInc, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &GridMax, sizeof(float));
	total += Buffer->Write((Ptr) &GridMin, sizeof(float));
	code = 100;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &OrbSet, sizeof(long));
	return total;
}

char * MEP3DSurface::GetLabel(void) const {
		char * text=NULL;
	if (Label) text = Surface::GetLabel();
	else {
		text = new char[100];
		int nchar;
		sprintf(text, "3D Molecular Electrostatic Potential%n", &nchar);
		if (Visible) sprintf(&(text[nchar]), " Visible");
	}
	return text;
}
void MEP3DSurface::UpdateData(MEP3DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	SolidSurface(target->SolidSurface());
	SetContourBothPosNeg(target->ContourBothPosNeg());
	SetColorByValue(target->ColorByValue());
	SetFixGrid(target->GetFixGrid());
	UseSurfaceNormals(target->UseSurfaceNormals());
	NumGridPoints = target->GetNumGridPoints();
	ContourValue = target->GetContourValue();
	GridSize = target->GetGridSize();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	target->GetTranspColor(&TranspColor);
	OrbSet = target->getTargetOrbitalSet();
}

long MEP3DSurface::Write(BufferFile * Buffer) {
	long code, length, total=0;

	code = kMEP3D;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 10;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	length = Surf3DBase::GetSizeofSurface(Buffer, true);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Surf3DBase::Write3D(Buffer, true);
	code = 100;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &OrbSet, sizeof(long));
	
	return total;
}
Orb2DSurface::Orb2DSurface(WinPrefs * Prefs) : Surf2DBase(Prefs) {
}
Orb2DSurface::Orb2DSurface(BufferFile * Buffer, long /*length*/) {
	long	size;

		CPoint3D		Origin;
		CPoint3D		XInc;
		CPoint3D		YInc;
		float			GridMax, GridMin;
	Buffer->Read((Ptr) &size, sizeof(long));
	if (size != 1) throw DataError();
	Buffer->Read((Ptr) &ID, sizeof(long));
	Buffer->Read((Ptr) &Visible, sizeof(Boolean));
	Buffer->Read((Ptr) &PlotOrb, sizeof(long));
	Buffer->Read((Ptr) &Options, sizeof(long));
	Buffer->Read((Ptr) &NumGridPoints, sizeof(long));
	Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
	Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
	Buffer->Read((Ptr) &NumContours, sizeof(long));
	Buffer->Read((Ptr) &MaxContourValue, sizeof(float));
	Buffer->Read((Ptr) &SurfOptions, sizeof(long));
	Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
	Buffer->Read((Ptr) &XInc, sizeof(CPoint3D));
	Buffer->Read((Ptr) &YInc, sizeof(CPoint3D));
	Buffer->Read((Ptr) &GridMax, sizeof(float));
	Buffer->Read((Ptr) &GridMin, sizeof(float));
	NextSurface = NULL;
	Label = NULL;
	Grid = NULL;
	GridAllocation = 0;
	SetContourBothPosNeg(true);
}
Orb2DSurface::Orb2DSurface(Orb2DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	PlotOrb = target->GetTargetOrb();
	Options = target->GetOptions();
	NumGridPoints = target->GetNumGridPoints();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	NextSurface = NULL;
	Grid = NULL;
	GridAllocation = 0;
}


void Orb2DSurface::UpdateData(Orb2DSurface * target) {
	Visible = target->GetVisibility();
	PlotOrb = target->GetTargetOrb();
	Options = target->GetOptions();
	NumGridPoints = target->GetNumGridPoints();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
}
char * Orb2DSurface::GetLabel(void) const {
		char * text=NULL;
	if (Label) text = Surface::GetLabel();
	else {
		text = new char[100];
		int nchar, nchar2=0;
		sprintf(text, "2D Orbital%n", &nchar);
		if (Options&1) sprintf(&(text[nchar]), " AO #%n",&nchar2);
		else if (Options&2) sprintf(&(text[nchar]), " MO #%n",&nchar2);
		else if (Options&4) sprintf(&(text[nchar]), " LMO #%n", &nchar2);
		nchar += nchar2;
		if (PlotOrb >= 0) {
			sprintf(&(text[nchar]), " %d%n", PlotOrb+1, &nchar2);
			nchar += nchar2;
		}
		if (Visible) sprintf(&(text[nchar]), " Visible");
	}
	return text;
}
Orb3DSurface::Orb3DSurface(WinPrefs * Prefs) : Surf3DBase(Prefs) {
	SetContourBothPosNeg(true);
}
Orb3DSurface::Orb3DSurface(BufferFile * Buffer, long length) {
		long size, pos;
	Buffer->Read((Ptr) &size, sizeof(long));
	if (size == 10) {
		Buffer->Read((Ptr) &size, sizeof(long));
		pos = Buffer->GetFilePos();
		Surf3DBase::Read3D(Buffer, size);
		Buffer->SetFilePos(pos + size);
		Buffer->Read((Ptr) &size, sizeof(long));
		OrbSurfBase::Read(Buffer, size);
	} else {
		if (size != 1) throw DataError();
		Buffer->Read((Ptr) &ID, sizeof(long));
		Buffer->Read((Ptr) &Visible, sizeof(Boolean));
		Buffer->Read((Ptr) &PlotOrb, sizeof(long));
		Buffer->Read((Ptr) &Options, sizeof(long));
		Buffer->Read((Ptr) &NumGridPoints, sizeof(long));
		Buffer->Read((Ptr) &Mode, sizeof(long));
		Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &TranspColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &ContourValue, sizeof(float));
		Buffer->Read((Ptr) &GridSize, sizeof(float));
		Buffer->Read((Ptr) &NumPosContourTriangles, sizeof(long));
		Buffer->Read((Ptr) &NumNegContourTriangles, sizeof(long));
		Buffer->Read((Ptr) &GridMax, sizeof(float));
		Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
		Buffer->Read((Ptr) &XGridInc, sizeof(float));
		Buffer->Read((Ptr) &YGridInc, sizeof(float));
		Buffer->Read((Ptr) &ZGridInc, sizeof(float));
		Buffer->Read((Ptr) &NumXGridPoints, sizeof(long));
		Buffer->Read((Ptr) &NumYGridPoints, sizeof(long));
		Buffer->Read((Ptr) &NumZGridPoints, sizeof(long));
		if (length > (sizeof(long)+sizeof(Orb3DSurface))) {
			Buffer->Read((Ptr) &size, sizeof(long));
			if (size == 2) {
				Buffer->Read((Ptr) &size, sizeof(long));
				if (AllocateContour(size/sizeof(CPoint3D), (NumPosContourTriangles+
					NumNegContourTriangles))) {
	#ifdef UseHandles
					HLock(ContourHndl);
					Buffer->Read((Ptr) *ContourHndl, size);
					HUnlock(ContourHndl);
					for (long i=0; i<(NumPosContourTriangles+NumNegContourTriangles); i++) {
						((long *) *VertexHndl)[3*i] = 3*i;
						((long *) *VertexHndl)[3*i+1] = 3*i+1;
						((long *) *VertexHndl)[3*i+2] = 3*i+2;
					}
	#else
					Buffer->Read((Ptr) ContourHndl, size);
					for (long i=0; i<(NumPosContourTriangles+NumNegContourTriangles); i++) {
						VertexList[3*i] = 3*i;
						VertexList[3*i+1] = 3*i+1;
						VertexList[3*i+2] = 3*i+2;
					}
	#endif
					NumVertices = 3*(NumPosContourTriangles+NumNegContourTriangles);
				}
			}
		}
		SetContourBothPosNeg(true);
	}
}
long Orb3DSurface::Write(BufferFile * Buffer) {
	long code, length, total=0;

	code = kOrb3DType;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 10;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	length = Surf3DBase::GetSizeofSurface(Buffer, false);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Surf3DBase::Write3D(Buffer, false);
	length = OrbSurfBase::GetSizeofSurface(Buffer);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += OrbSurfBase::Write(Buffer);
	return total;
}
Orb3DSurface::Orb3DSurface(Orb3DSurface * target) : Surf3DBase() {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	PlotOrb = target->GetTargetOrb();
	Options = target->GetOptions();
	SolidSurface(target->SolidSurface());
	SetContourBothPosNeg(target->ContourBothPosNeg());
	SetColorByValue(target->ColorByValue());
	SetFixGrid(target->GetFixGrid());
	UseSurfaceNormals(target->UseSurfaceNormals());
	NumGridPoints = target->GetNumGridPoints();
	ContourValue = target->GetContourValue();
	GridSize = target->GetGridSize();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	target->GetTranspColor(&TranspColor);
}
void Orb3DSurface::UpdateData(Orb3DSurface * target) {
	Visible = target->GetVisibility();
	PlotOrb = target->GetTargetOrb();
	Options = target->GetOptions();
	SolidSurface(target->SolidSurface());
	SetContourBothPosNeg(target->ContourBothPosNeg());
	SetColorByValue(target->ColorByValue());
	SetFixGrid(target->GetFixGrid());
	UseSurfaceNormals(target->UseSurfaceNormals());
	NumGridPoints = target->GetNumGridPoints();
	ContourValue = target->GetContourValue();
	GridSize = target->GetGridSize();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	target->GetTranspColor(&TranspColor);
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
}
char * Orb3DSurface::GetLabel(void) const {
		char * text=NULL;
	if (Label) text = Surface::GetLabel();
	else {
		text = new char[100];
		int nchar, nchar2=0;
		sprintf(text, "3D Orbital%n", &nchar);
		if (Options&1) sprintf(&(text[nchar]), " AO #%n",&nchar2);
		else if (Options&2) sprintf(&(text[nchar]), " MO #%n",&nchar2);
		else if (Options&4) sprintf(&(text[nchar]), " LMO #%n", &nchar2);
		nchar += nchar2;
		if (PlotOrb >= 0) {
			sprintf(&(text[nchar]), " %d%n", PlotOrb+1, &nchar2);
			nchar += nchar2;
		}
		if (Visible) sprintf(&(text[nchar]), " Visible");
	}
	return text;
}
General3DSurface::General3DSurface(WinPrefs * Prefs) : Surf3DBase(Prefs) {
}
General3DSurface::General3DSurface(BufferFile * Buffer, long length) : Surf3DBase() {
		long size;
	Buffer->Read((Ptr) &size, sizeof(long));
	if (size == 10) {
		Buffer->Read((Ptr) &size, sizeof(long));
		Surf3DBase::Read3D(Buffer, size);
	} else {
		if (size != 1) throw DataError();
		Buffer->Read((Ptr) &ID, sizeof(long));
		Buffer->Read((Ptr) &Visible, sizeof(Boolean));
		Buffer->Read((Ptr) &Mode, sizeof(long));
		Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &TranspColor, sizeof(RGBColor));
		Buffer->Read((Ptr) &ContourValue, sizeof(float));
		Buffer->Read((Ptr) &NumPosContourTriangles, sizeof(long));
		Buffer->Read((Ptr) &NumNegContourTriangles, sizeof(long));
		Buffer->Read((Ptr) &GridMin, sizeof(float));
		Buffer->Read((Ptr) &GridMax, sizeof(float));
		Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
		Buffer->Read((Ptr) &XGridInc, sizeof(float));
		Buffer->Read((Ptr) &YGridInc, sizeof(float));
		Buffer->Read((Ptr) &ZGridInc, sizeof(float));
		Buffer->Read((Ptr) &NumXGridPoints, sizeof(long));
		Buffer->Read((Ptr) &NumYGridPoints, sizeof(long));
		Buffer->Read((Ptr) &NumZGridPoints, sizeof(long));

		if (length > (sizeof(long)+sizeof(General3DSurface))) {
			Buffer->Read((Ptr) &size, sizeof(long));
			if (size == 2) {	//individual contour
				Buffer->Read((Ptr) &size, sizeof(long));
				AllocateContour(size/sizeof(CPoint3D), NumPosContourTriangles+
					NumNegContourTriangles);
				if (ContourHndl) {
	#ifdef UseHandles
					HLock(ContourHndl);
					Buffer->Read((Ptr) *ContourHndl, size);
					HUnlock(ContourHndl);
					for (long i=0; i<(NumPosContourTriangles+NumNegContourTriangles); i++) {
						((long *) *VertexHndl)[3*i] = 3*i;
						((long *) *VertexHndl)[3*i+1] = 3*i+1;
						((long *) *VertexHndl)[3*i+2] = 3*i+2;
					}
	#else
					Buffer->Read((Ptr) ContourHndl, size);
					for (long i=0; i<(NumPosContourTriangles+NumNegContourTriangles); i++) {
						VertexList[3*i] = 3*i;
						VertexList[3*i+1] = 3*i+1;
						VertexList[3*i+2] = 3*i+2;
					}
	#endif
					NumVertices = 3*(NumPosContourTriangles+NumNegContourTriangles);
				}
			} else if (size == 3) {	//Full 3D grid of data
				Buffer->Read((Ptr) &size, sizeof(long));
				AllocateGrid(size/sizeof(float));
				if (Grid) {
	#ifdef UseHandles
					HLock(Grid);
					Buffer->Read((Ptr) *Grid, size);
					HUnlock(Grid);
	#else
					Buffer->Read((Ptr) Grid, size);
	#endif
				}
			}
		}
	}
}

long General3DSurface::Write(BufferFile * Buffer) {
	long code, length, total=0;

	code = kGeneral3DSurface;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 10;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	length = Surf3DBase::GetSizeofSurface(Buffer, true);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Surf3DBase::Write3D(Buffer, true);
	return total;
}
char * General3DSurface::GetLabel(void) const {
		char * text=NULL;
	if (Label) text = Surface::GetLabel();
	else {
		text = new char[100];
		int nchar;
		sprintf(text, "General 3D surface%n", &nchar);
		if (Visible) sprintf(&(text[nchar]), " Visible");
	}
	return text;
}
General2DSurface::General2DSurface(BufferFile * Buffer, long length) {
		long size;
	Buffer->Read((Ptr) &size, sizeof(long));
	if (size != 1) throw DataError();
	Buffer->Read((Ptr) &ID, sizeof(long));
	Buffer->Read((Ptr) &Visible, sizeof(Boolean));
	Buffer->Read((Ptr) &NumGridPoints, sizeof(long));
	Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
	Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
	Buffer->Read((Ptr) &NumContours, sizeof(long));
	Buffer->Read((Ptr) &MaxContourValue, sizeof(float));
	Buffer->Read((Ptr) &SurfOptions, sizeof(long));
	Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
	Buffer->Read((Ptr) &XInc, sizeof(CPoint3D));
	Buffer->Read((Ptr) &YInc, sizeof(CPoint3D));
	Buffer->Read((Ptr) &GridMin, sizeof(float));
	Buffer->Read((Ptr) &GridMax, sizeof(float));

	SetContourBothPosNeg(true);
	NextSurface = NULL;
	Label = NULL;
	Grid = NULL;
	GridAllocation = 0;
	if (length > (sizeof(long)+sizeof(General2DSurface))) {
		Buffer->Read((Ptr) &size, sizeof(long));
		if (size == 2) {	//Full 2D grid of data
			Buffer->Read((Ptr) &size, sizeof(long));
			AllocateGrid(size/sizeof(float));
			if (Grid) {
#ifdef UseHandles
				HLock(Grid);
				Buffer->Read((Ptr) *Grid, size);
				HUnlock(Grid);
#else
				Buffer->Read((Ptr) Grid, size);
#endif
			}
		}
	}
}
General2DSurface::General2DSurface(WinPrefs * Prefs) : Surf2DBase(Prefs) {
}
char * General2DSurface::GetLabel(void) const {
		char * text=NULL;
	if (Label) text = Surface::GetLabel();
	else {
		text = new char[100];
		int nchar;
		sprintf(text, "General 2D surface%n", &nchar);
		if (Visible) sprintf(&(text[nchar]), " Visible");
	}
	return text;
}
long General2DSurface::Write(BufferFile * Buffer) {
	long code, length, total=0;

	code = kGeneral2DSurface;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 1;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &ID, sizeof(long));
	total += Buffer->Write((Ptr) &Visible, sizeof(Boolean));
	total += Buffer->Write((Ptr) &NumGridPoints, sizeof(long));
	total += Buffer->Write((Ptr) &PosColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NegColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NumContours, sizeof(long));
	total += Buffer->Write((Ptr) &MaxContourValue, sizeof(float));
	total += Buffer->Write((Ptr) &SurfOptions, sizeof(long));
	total += Buffer->Write((Ptr) &Origin, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &XInc, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &YInc, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &GridMax, sizeof(float));
	total += Buffer->Write((Ptr) &GridMin, sizeof(float));
	if (Grid) {
		code = 2;
		total += Buffer->Write((Ptr) &code, sizeof(long));
		length = GridAllocation*sizeof(float);
		total += Buffer->Write((Ptr) &length, sizeof(long));
#ifdef UseHandles
		HLock(Grid);
		total += Buffer->Write((Ptr) *Grid, length);
		HUnlock(Grid);
#else
		total += Buffer->Write((Ptr) Grid, length);
#endif
	}
	return total;
}
TEDensity3DSurface::TEDensity3DSurface(WinPrefs * Prefs) : Surf3DBase(Prefs) {
	MaxMEPValue = 0.1;
	OrbSet = -1;
	SetContourBothPosNeg(false);
}

char * TEDensity3DSurface::GetLabel(void) const {
		char * text=NULL;
	if (Label) text = Surface::GetLabel();
	else {
		text = new char[100];
		int nchar;
		sprintf(text, "3D Total Electron Density%n", &nchar);
		if (Visible) sprintf(&(text[nchar]), " Visible");
	}
	return text;
}
void TEDensity3DSurface::UpdateData(TEDensity3DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	SolidSurface(target->SolidSurface());
	SetContourBothPosNeg(target->ContourBothPosNeg());
	SetColorByValue(target->ColorByValue());
	SetFixGrid(target->GetFixGrid());
	UseSurfaceNormals(target->UseSurfaceNormals());
	NumGridPoints = target->GetNumGridPoints();
	ContourValue = target->GetContourValue();
	GridSize = target->GetGridSize();
	target->GetPosColor(&PosColor);
	target->GetTranspColor(&TranspColor);
	OrbSet = target->getTargetOrbSet();
}
long TEDensity3DSurface::Write(BufferFile * Buffer) {
	long code, length, total=0;

	code = kTotalDensity3D;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 10;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	length = Surf3DBase::GetSizeofSurface(Buffer, false);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Surf3DBase::Write3D(Buffer, false);
	code = 3;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = sizeof(float);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &MaxMEPValue, length);
	code = 4;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &OrbSet, length);
	return total;
}

TEDensity3DSurface::TEDensity3DSurface(BufferFile * Buffer, long length) {
		long code, size, total;
	OrbSet = -1;
	total = Buffer->Read((Ptr) &size, sizeof(long));
	if (size == 10) {
		total += Buffer->Read((Ptr) &size, sizeof(long));
		code = Buffer->GetFilePos();
		Surf3DBase::Read3D(Buffer, size);
		Buffer->SetFilePos(code + size);
		total += size;
		total += Buffer->Read((Ptr) &size, sizeof(long));
		if (size == 3) {
			total += Buffer->Read((Ptr) &size, sizeof(long));
			total += Buffer->Read((Ptr) &MaxMEPValue, sizeof(float));
		}
		if (total < length) {
			total += Buffer->Read((Ptr) &size, sizeof(long));
			if (size == 4) {
				total += Buffer->Read((Ptr) &size, sizeof(long));
				total += Buffer->Read((Ptr) &OrbSet, sizeof(long));
			}
		}
	} else {
		if (size != 1) throw DataError();
		total += Buffer->Read((Ptr) &ID, sizeof(long));
		total += Buffer->Read((Ptr) &Visible, sizeof(Boolean));
		total += Buffer->Read((Ptr) &NumGridPoints, sizeof(long));
		total += Buffer->Read((Ptr) &Mode, sizeof(long));
		total += Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
		total += Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
		total += Buffer->Read((Ptr) &TranspColor, sizeof(RGBColor));
		total += Buffer->Read((Ptr) &ContourValue, sizeof(float));
		total += Buffer->Read((Ptr) &GridSize, sizeof(float));
		total += Buffer->Read((Ptr) &NumPosContourTriangles, sizeof(long));
		NumNegContourTriangles = 0;
		total += Buffer->Read((Ptr) &GridMax, sizeof(float));
		total += Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
		total += Buffer->Read((Ptr) &XGridInc, sizeof(float));
		total += Buffer->Read((Ptr) &YGridInc, sizeof(float));
		total += Buffer->Read((Ptr) &ZGridInc, sizeof(float));
		total += Buffer->Read((Ptr) &NumXGridPoints, sizeof(long));
		total += Buffer->Read((Ptr) &NumYGridPoints, sizeof(long));
		total += Buffer->Read((Ptr) &NumZGridPoints, sizeof(long));
			float junk;
		total += Buffer->Read((Ptr) &junk, sizeof(float));

		while (total < length) {
			total += Buffer->Read((Ptr) &code, sizeof(long));
			total += Buffer->Read((Ptr) &size, sizeof(long));
			switch (code) {
				case 2:
					if (AllocateContour(size/sizeof(CPoint3D), NumPosContourTriangles)) {
#ifdef UseHandles
						HLock(ContourHndl);
						total += Buffer->Read((Ptr) *ContourHndl, size);
						HUnlock(ContourHndl);
						for (long i=0; i<NumPosContourTriangles; i++) {
							((long *) *VertexHndl)[3*i] = 3*i;
							((long *) *VertexHndl)[3*i+1] = 3*i+1;
							((long *) *VertexHndl)[3*i+2] = 3*i+2;
						}
#else
						total += Buffer->Read((Ptr) ContourHndl, size);
						for (long i=0; i<NumPosContourTriangles; i++) {
							VertexList[3*i] = 3*i;
							VertexList[3*i+1] = 3*i+1;
							VertexList[3*i+2] = 3*i+2;
						}
#endif
					}
				break;
				case 3:
					total += Buffer->Read((Ptr) &MaxMEPValue, sizeof(float));
				break;
				case 4:
					AllocateList(size/sizeof(float));
					if (List) {
#ifdef UseHandles
						HLock(List);
						total += Buffer->Read((Ptr) *List, size);
						HUnlock(List);
#else
						total += Buffer->Read((Ptr) List, size);
#endif
					}
				break;
			}
		}
		SetContourBothPosNeg(false);
	}
}
long TEDensity2DSurface::Write(BufferFile * Buffer) {
	long code, length, total=0;

	code = kTotalDensity2D;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 1;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &ID, sizeof(long));
	total += Buffer->Write((Ptr) &Visible, sizeof(Boolean));
	total += Buffer->Write((Ptr) &NumGridPoints, sizeof(long));
	total += Buffer->Write((Ptr) &PosColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NegColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NumContours, sizeof(long));
	total += Buffer->Write((Ptr) &MaxContourValue, sizeof(float));
	total += Buffer->Write((Ptr) &SurfOptions, sizeof(long));
	total += Buffer->Write((Ptr) &Origin, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &XInc, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &YInc, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &GridMax, sizeof(float));
	total += Buffer->Write((Ptr) &GridMin, sizeof(float));
	code = 100;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &OrbSet, sizeof(long));
	
	return total;
}

TEDensity2DSurface::TEDensity2DSurface(BufferFile * Buffer, long length){
	long	size;
	OrbSet = -1;
	long total = Buffer->Read((Ptr) &size, sizeof(long));
		CPoint3D		Origin;
		CPoint3D		XInc;
		CPoint3D		YInc;
		float			GridMax, GridMin;
	if (size != 1) throw DataError();
	total += Buffer->Read((Ptr) &ID, sizeof(long));
	total += Buffer->Read((Ptr) &Visible, sizeof(Boolean));
	total += Buffer->Read((Ptr) &NumGridPoints, sizeof(long));
	total += Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
	total += Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
	total += Buffer->Read((Ptr) &NumContours, sizeof(long));
	total += Buffer->Read((Ptr) &MaxContourValue, sizeof(float));
	total += Buffer->Read((Ptr) &SurfOptions, sizeof(long));
	total += Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
	total += Buffer->Read((Ptr) &XInc, sizeof(CPoint3D));
	total += Buffer->Read((Ptr) &YInc, sizeof(CPoint3D));
	total += Buffer->Read((Ptr) &GridMax, sizeof(float));
	total += Buffer->Read((Ptr) &GridMin, sizeof(float));
	if (total < length) {
		total += Buffer->Read((Ptr) &size, sizeof(long));
		if (size == 100) {
			total += Buffer->Read((Ptr) &size, sizeof(long));
			total += Buffer->Read((Ptr) &OrbSet, sizeof(long));
		}
	}
	NextSurface = NULL;
	Label = NULL;
	Grid = NULL;
	GridAllocation = 0;
	SetContourBothPosNeg(false);
}
TEDensity2DSurface::TEDensity2DSurface(TEDensity2DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	OrbSet = target->getTargetOrbSet();
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	NextSurface = NULL;
	Grid = NULL;
	GridAllocation = 0;
}
TEDensity2DSurface::TEDensity2DSurface(WinPrefs * Prefs) : Surf2DBase(Prefs) {
	SetContourBothPosNeg(false);
	OrbSet = -1;
}

char * TEDensity2DSurface::GetLabel(void) const {
		char * text=NULL;
	if (Label) text = Surface::GetLabel();
	else {
		text = new char[100];
		int nchar;
		sprintf(text, "2D Total Electron Density%n", &nchar);
		if (Visible) sprintf(&(text[nchar]), " Visible");
	}
	return text;
}
void TEDensity2DSurface::Update(MoleculeData * MainData) {
	if (Visible) {
		Progress * lProgress = new Progress;
		if (lProgress) {
			lProgress->ChangeText("Calculating TED Grid...");
			if (GetRotate2DMap()) SetPlaneToScreenPlane(MainData);
			CalculateMOGrid(MainData, lProgress);
			delete lProgress;
		}
	}
}
void TEDensity2DSurface::UpdateData(TEDensity2DSurface * target) {
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	target->setTargetOrbSet(OrbSet);
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
}


void Frame::ReadSurfaceList(BufferFile * Buffer, long length) {
	long total = 0;
	while (total<length) {
			long SurfaceSize;
			Surface * NewSurface;
		total += Buffer->Read((Ptr) &SurfaceSize, sizeof(long));
		long FilePos = Buffer->GetFilePos();
		long SurfaceType;
		total += Buffer->Read((Ptr) &SurfaceType, sizeof(long));
		SurfaceSize -= sizeof(long);
		NewSurface = NULL;
		switch (SurfaceType) {
			case kOrb3DType:
				NewSurface = new Orb3DSurface(Buffer, SurfaceSize);
			break;
			case kOrb2DType:
				NewSurface = new Orb2DSurface(Buffer, SurfaceSize);
			break;
			case kTotalDensity3D:
				NewSurface = new TEDensity3DSurface(Buffer, SurfaceSize);
			break;
			case kTotalDensity2D:
				NewSurface = new TEDensity2DSurface(Buffer, SurfaceSize);
			break;
			case kMEP2D:
				NewSurface = new MEP2DSurface(Buffer, SurfaceSize);
			break;
			case kMEP3D:
				NewSurface = new MEP3DSurface(Buffer, SurfaceSize);
			break;
#ifdef ChipmanCode
			case kSolventType3D:
				NewSurface = new ChipSurface(Buffer, SurfaceSize);
			break;
#endif
			case kGeneral3DSurface:
				NewSurface = new General3DSurface(Buffer, SurfaceSize);
			break;
			case kGeneral2DSurface:
				NewSurface = new General2DSurface(Buffer, SurfaceSize);
			break;
		}
		if (NewSurface) {
			if (SurfaceList) {
				Surface * temp = SurfaceList;
				while (temp->GetNextSurface()) temp = temp->GetNextSurface();
				temp->SetNextSurface(NewSurface);
			} else SurfaceList = NewSurface;
		}
		SurfaceType = Buffer->GetFilePos();
		Buffer->BufferSkip((SurfaceSize+sizeof(long))-(SurfaceType-FilePos));
		total += SurfaceSize;
	}
}
//Calculate the sum of the squares of the change in position of each atom between
//CoordSetA and CoordSetB
float CalculateSquaresValue(long NumOptAtoms, mpAtom CoordSetA[], CPoint3D CoordSet[]) {
	float result=0.0, x, y, z;
	for (long iatom=0; iatom<NumOptAtoms; iatom++) {
		x = (CoordSetA[iatom].Position.x - CoordSet[iatom].x);
		y = (CoordSetA[iatom].Position.y - CoordSet[iatom].y);
		z = (CoordSetA[iatom].Position.z - CoordSet[iatom].z);
		result += x*x + y*y + z*z;
	}
	return result;
}	/*CalculateSquaresValue*/
Surface * Frame::GetSurface(long SurfaceNum) {
	Surface * result = SurfaceList;
	for (long i=0; (i<SurfaceNum)&&result; i++) result = result->GetNextSurface();
	return result;
}
Surface * Frame::GetSurfaceWithID(long SurfaceID) {
	Surface * result = SurfaceList;
	while (result) {
		if (result->GetSurfaceID() == SurfaceID) break;
		result = result->GetNextSurface();
	}
	return result;
}
void Frame::DeleteSurface(long SurfaceNum) {
	Surface * previous=NULL, * target=SurfaceList;
	for (long i=0; (i<SurfaceNum)&&target; i++) {previous=target; target=target->GetNextSurface();}
	if (target) {
		if (previous) previous->SetNextSurface(target->GetNextSurface());
		else SurfaceList = target->GetNextSurface();
		delete target;
	}
}
void Frame::DeleteSurfaceWithID(long SurfaceID) {
	Surface * previous=NULL, * target=SurfaceList;
	while (target) {
		if (target->GetSurfaceID() == SurfaceID) break;
		previous = target;
		target=target->GetNextSurface();
	}
	if (target) {
		if (previous) previous->SetNextSurface(target->GetNextSurface());
		else SurfaceList = target->GetNextSurface();
		delete target;
	}
}
void Frame::AppendSurface(Surface * NewSurface) {
	if (SurfaceList) {
		Surface * temp = SurfaceList;
		while (temp->NextSurface) temp = temp->NextSurface;
		temp->SetNextSurface(NewSurface);
	} else SurfaceList = NewSurface;
}
long Frame::GetNumSurfaces(void) {
	long result = 0;
	Surface * temp = SurfaceList;
	while (temp) {result++; temp = temp->GetNextSurface();}
	return result;
}
bool Frame::SurfaceExportPossible(void) {
	bool result = false;
	Surface * temp = SurfaceList;
	while (temp) {result = (result || temp->ExportPossible()); temp = temp->GetNextSurface();}
	return result;
}
MemoryError::MemoryError(const char *Message) {
	long length = strlen(Message);
	ErrorMessage = new char [length];
	strcpy(ErrorMessage, Message);
};
