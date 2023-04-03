//---------------------------------------------------------------------------
/* 3D.h: declarations for 3D rotation/translation class 3D.cpp */

#ifndef _3DH
#define _3DH

//---------------------------------------------------------------------------
class _3D
{
private:

	// internal vectors and matrices
	double WorldUp[3];
	double ViewMoveMatrix[12];
	double ViewRotationMatrix[12];
	double WorldTransform[12];
	double ObjectMoveMatrix[12];
	double ObjectRotationMatrix[12];
	double ObjectTransform[12];
	double CombinedTransform[12];

	// helper functions
	void MatrixMultiply(double *A, double *B, double *C);
	void MoveFill(double *A, double Cx, double Cy, double Cz);
	void RotateFill(double *A, double Cx, double Cy, double Cz);

public:

	double pi;

	_3D(void); // constructor

	// 3D functions
	void _3DAxisAngle(double x, double y, double z, double theta);
	void _3DAxisAngleObject(double x, double y, double z, double theta);
	void _3DGetView(double *x1, double *y1, double *z1, double *x2, double *y2, double *z2);
	int  _3DLookAt(double x1, double y1, double z1, double x2, double y2, double z2);
	void _3DPOV(double Cx, double Cy, double Cz, double xAngle, double yAngle, double zAngle);
	void _3DRoll(double theta);
	void _3DRollObject(double theta);
	void _3DRotateRight(double theta);
	void _3DRotateRightObject(double theta);
	void _3DRotateUp(double theta);
	void _3DRotateUpObject(double theta);
	void _3DMove(double x, double y, double z);
	void _3DMoveObject(double x, double y, double z);
	void _3DMoveForward(double n);
	void _3DMoveForwardObject(double n);
	void _3DMoveRight(double n);
	void _3DMoveRightObject(double n);
	void _3DMoveUp(double n);
	void _3DMoveUpObject(double n);
	void _3DSetObject(double Ox, double Oy, double Oz, int xAngle, int yAngle, int zAngle);
	void _3DUpVector(double x, double y, double z);
};
#endif
