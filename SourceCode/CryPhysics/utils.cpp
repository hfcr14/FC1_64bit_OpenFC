//////////////////////////////////////////////////////////////////////
//
//	Utils
//	
//	File: utils.cpp
//	Description : Various utility functions
//
//	History:
//	-:Created by Anton Knyazev
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "utils.h"
#include "primitives.h"

#if defined(WIN64) 
__declspec(noinline) int float2int(float x)
{
	//return x>0 ? (int)(x+0.5f) : (int)(x-0.5f);
	floatint u;
	u.fval = x + fmag;
	return u.ival - imag;
}
#endif

#if defined(LINUX64)
int float2int(float x)
{
	//return x>0 ? (int)(x+0.5f) : (int)(x-0.5f);
	floatint u;
	u.fval = x + fmag;
	return u.ival - imag;
}
#endif
// Mass properties calculations 

void compute_projection_integrals(vectorr* ab, real pi[10])
{
	real a0[4], b0[4], a1[3], b1[3], C[4][3], da, db;
	int i, edge;
	for (i = 0; i < 10; i++) pi[i] = 0;

	for (edge = 0; edge < 3; edge++, ab++) {
		for (i = 1, a0[0] = ab[0].x; i < 4; i++) a0[i] = a0[i - 1] * ab[0].x;
		for (i = 1, b0[0] = ab[0].y; i < 4; i++) b0[i] = b0[i - 1] * ab[0].y;
		for (i = 1, a1[0] = ab[1].x; i < 3; i++) a1[i] = a1[i - 1] * ab[1].x;
		for (i = 1, b1[0] = ab[1].y; i < 3; i++) b1[i] = b1[i - 1] * ab[1].y;
		for (i = 1, C[0][0] = a1[1] + a1[0] * a0[0] + a0[1]; i < 3; i++) C[0][i] = a1[0] * C[0][i - 1] + a0[i + 1];
		for (i = 1, C[1][0] = b1[1] + b1[0] * b0[0] + b0[1]; i < 3; i++) C[1][i] = b1[0] * C[1][i - 1] + b0[i + 1];
		da = a1[0] - a0[0]; db = b1[0] - b0[0];
		C[2][0] = 3 * a1[1] + 2 * a1[0] * a0[0] + a0[1];
		C[2][1] = a0[0] * C[2][0] + 4 * a1[2];
		C[2][2] = 4 * b1[2] + 3 * b1[1] * b0[0] + 2 * b1[0] * b0[1] + b0[2];
		C[3][0] = 3 * a0[1] + 2 * a0[0] * a1[0] + a1[1];
		C[3][1] = a1[0] * C[3][0] + 4 * a0[2];
		C[3][2] = 4 * b0[2] + 3 * b0[1] * b1[0] + 2 * b0[0] * b1[1] + b1[2];
		pi[0] += db * (a0[0] + a1[0]);
		for (i = 0; i < 3; i++) {
			pi[i + 1] += db * C[0][i];
			pi[i + 4] += da * C[1][i];
		}
		pi[7] += db * (b1[0] * C[2][0] + b0[0] * C[3][0]);
		pi[8] += db * (b1[0] * C[2][1] + b0[0] * C[3][1]);
		pi[9] += da * (a1[0] * C[2][2] + a0[0] * C[3][2]);
	}

	pi[0] *= 0.5;
	pi[1] *= 1.0 / 6; pi[2] *= 1.0 / 12; pi[3] *= 1.0 / 20;
	pi[4] *= -1.0 / 6; pi[5] *= -1.0 / 12; pi[6] *= -1.0 / 20;
	pi[7] *= 1.0 / 24; pi[8] *= 1.0 / 60; pi[9] *= -1.0 / 60;
}

void compute_face_integrals(vectorr* p, vectorr n, real fi[12])
{
	real pi[10], k[4], t, w;
	int i;

	compute_projection_integrals(p, pi);

	w = -(n * p[0]);
	for (k[0] = 1.0 / n.z, i = 1; i < 4; i++) k[i] = k[i - 1] * k[0];

	for (i = 0; i < 3; i++) fi[i * 3 + 0] = k[0] * pi[i + 1]; // a, a2, a3
	for (i = 0; i < 3; i++) fi[i * 3 + 1] = k[0] * pi[i + 4]; // b, b2, b3

	// a2b, b2g, g2a
	fi[9] = k[0] * pi[8];
	fi[10] = -k[1] * (n.x * pi[9] + n.y * pi[6] + w * pi[5]);
	fi[11] = k[2] * (n.x * n.x * pi[3] + n.y * n.y * pi[9] + w * w * pi[1] + 2 * (n.x * n.y * pi[8] + n.x * w * pi[2] + n.y * w * pi[7]));

	for (i = 0, t = n.x; i < 3; i++, t *= n.x) pi[i + 1] *= t;
	for (i = 0, t = n.y; i < 3; i++, t *= n.y) pi[i + 4] *= t;
	for (i = 0; i < 3; i++) pi[i + 7] *= n.x * n.y;
	pi[8] *= n.x; pi[9] *= n.y;

	// g, g2, g3
	fi[2] = -k[1] * (pi[1] + pi[4] + w * pi[0]);
	fi[5] = k[2] * (pi[2] + 2 * pi[7] + pi[5] + w * (2 * (pi[1] + pi[4]) + w * pi[0]));
	fi[8] = -k[3] * (pi[3] + 3 * (pi[8] + pi[9]) + pi[6] + w * (3 * (pi[2] + pi[5] + 2 * pi[7] + w * (pi[1] + pi[4])) + w * w * pi[0]));
}

real ComputeMassProperties(strided_pointer<const vectorf> points, const index_t* faces, int nfaces, vectorr& center, matrix3x3& I)
{
	real M = 0, fi[12], nmax, diag[3] = { 0,0,0 };
	vectorr n, p[4];
	int i, g;
	center.zero();
	I.SetZero();

	for (nfaces--; nfaces >= 0; nfaces--, faces += 3) {
		n = points[faces[1]] - points[faces[0]] ^ points[faces[2]] - points[faces[0]];
		if (n.len2() < 1E-8) continue;
		n.normalize();

		for (i = 0, nmax = -1; i < 3; i++) if (n[i] * n[i] > nmax)
		{
			nmax = n[i] * n[i]; g = i;
		}
		for (i = 0; i < 3; i++) p[i] = points[faces[i]].permutated(g);
		p[3] = p[0]; n = n.permutated(g);

		compute_face_integrals(p, n, fi);
		g = g ^ g >> 1 ^ 1;
		for (i = 0; i < 4; i++)
			((vectorr*)fi)[i] = ((vectorr*)fi)[i].permutated(g);
		n = n.permutated(g);

		M += n.x * fi[0];
		for (i = 0; i < 3; i++) {
			center[i] += n[i] * fi[i + 3];
			diag[i] += n[i] * fi[i + 6];
		}
		I(0, 1) += n.x * fi[9];
		I(1, 2) += n.y * fi[10];
		I(0, 2) += n.z * fi[11];
	}

	if (M > 0)
		center /= M * 2;
	I(0, 0) = (diag[1] + diag[2]) * (1.0 / 3) - M * (center.y * center.y + center.z * center.z);
	I(1, 1) = (diag[0] + diag[2]) * (1.0 / 3) - M * (center.x * center.x + center.z * center.z);
	I(2, 2) = (diag[0] + diag[1]) * (1.0 / 3) - M * (center.x * center.x + center.y * center.y);
	I(1, 0) = I(0, 1) = -I(0, 1) * 0.5 + M * center.x * center.y;
	I(2, 0) = I(0, 2) = -I(0, 2) * 0.5 + M * center.x * center.z;
	I(2, 1) = I(1, 2) = -I(1, 2) * 0.5 + M * center.y * center.z;
	return M;
}

void ComputeMeshEigenBasis(strided_pointer<const vectorf> pVertices, const index_t* pIndices, int nTris, vectorr* eigen_axes, vectorr& center)
{
	int i, j, k;
	vectorr m, mean(zero), v[3];
	real s, t, sum = 0, mtxbuf[9];
	matrix C(3, 3, mtx_symmetric, mtxbuf); C.zero();

	// find mean point
	for (i = 0; i < nTris * 3; i += 3) {
		v[0] = pVertices[pIndices[i + 0]];
		v[1] = pVertices[pIndices[i + 1]];
		v[2] = pVertices[pIndices[i + 2]];
		s = (v[1] - v[0] ^ v[2] - v[0]).len() * (real)0.5;
		mean += (v[0] + v[1] + v[2]) * real(1.0 / 3) * s;
		sum += s;
	}
	if (sum == 0) {
		center = nTris > 0 ? pVertices[pIndices[0]] : vectorf(zero);
		((matrix3x3RM&)eigen_axes[0]).SetIdentity();
		return;
	}
	mean /= sum;

	// calculate covariance matrix
	for (i = 0; i < nTris * 3; i += 3) {
		v[0] = pVertices[pIndices[i + 0]] - mean;
		v[1] = pVertices[pIndices[i + 1]] - mean;
		v[2] = pVertices[pIndices[i + 2]] - mean;
		s = (v[1] - v[0] ^ v[2] - v[0]).len() * (real)0.5;
		m = v[0] + v[1] + v[2];
		for (j = 0; j < 3; j++) for (k = j; k < 3; k++) {
			t = s * real(1.0 / 12) * (m[j] * m[k] + v[0][j] * v[0][k] + v[1][j] * v[1][k] + v[2][j] * v[2][k]);
			C[j][k] += t; if (k != j) C[k][j] += t;
		}
	}

	// find eigenvectors of covariance matrix (normalized)
	real eval[3];
	matrix eigenBasis(3, 3, 0, eigen_axes[0]);
	C.jacobi_transformation(eigenBasis, eval, 0);
	center = mean;
}

int ChoosePrimitiveForMesh(strided_pointer<const vectorf> pVertices, const index_t* pIndices, int nTris, const vectorr* eigen_axes, const vectorr& center,
	int flags, float tolerance, primitive*& pprim)
{
	static cylinder acyl;
	static box abox;
	static sphere asphere;
	int i, j, ibest;
	real error_max[3], error_avg[4], locerror, locarea;

	if (flags & mesh_approx_cylinder) {
		float r[3], h[3], area[2], zloc, rinv, hinv;
		matrix3x3f Basis = (matrix3x3RM&)eigen_axes[0];
		vectorf axis, ptloc, n, ptmin, ptmax, c;
		int iz, bBest, itype;
		error_avg[3] = (real)1E10; ibest = 3;

		ptmin = ptmax = Basis * pVertices[pIndices[0]];
		for (i = 1; i < nTris * 3; i++) {
			ptloc = Basis * pVertices[pIndices[i]];
			ptmin.x = min(ptmin.x, ptloc.x); ptmax.x = max(ptmax.x, ptloc.x);
			ptmin.y = min(ptmin.y, ptloc.y); ptmax.y = max(ptmax.y, ptloc.y);
			ptmin.z = min(ptmin.z, ptloc.z); ptmax.z = max(ptmax.z, ptloc.z);
		}
		c = ((ptmin + ptmax) * 0.5f) * Basis;

		for (iz = 0; iz < 3; iz++) {
			axis = eigen_axes[iz];
			for (i = 0, r[iz] = h[iz] = 0; i < nTris * 3; i++) {
				ptloc = pVertices[pIndices[i]] - c; zloc = ptloc * axis;
				r[iz] = max(r[iz], (ptloc - axis * zloc).len2()); h[iz] = max(h[iz], zloc);
			}
			r[iz] = sqrt_tpl(r[iz]);
			if (fabs_tpl(r[iz]) < (real)1E-5 || fabs_tpl(h[iz]) < (real)1E-5)
				continue;
			rinv = (real)1 / r[iz];
			hinv = (real)1 / h[iz];
			error_max[iz] = error_avg[iz] = 0;
			area[0] = area[1] = 0;

			for (i = 0; i < nTris; i++) {
				n = pVertices[pIndices[i * 3 + 1]] - pVertices[pIndices[i * 3]] ^ pVertices[pIndices[i * 3 + 2]] - pVertices[pIndices[i * 3]];
				locarea = n.len(); n /= locarea; locarea *= (real)0.5; zloc = fabs_tpl(n * axis);
				itype = isneg((real)0.5 - zloc); // 0-cylinder side, 1-cap
				locerror = 0;	// locerror will contain maximum distance from from triangle points to the cyl surface, normalized by cyl size
				if (itype) for (j = 0; j < 3; j++)
					locerror = max(locerror, fabs_tpl(fabs_tpl((pVertices[pIndices[i * 3 + j]] - c) * axis) * hinv - (real)1));
				else for (j = 0; j < 3; j++) {
					ptloc = pVertices[pIndices[i * 3 + j]] - c;
					locerror = max(locerror, fabs_tpl((ptloc - axis * (ptloc * axis)).len() * rinv - (real)1));
				}
				error_max[iz] = max(error_max[iz], locerror);
				error_avg[iz] += locerror * locarea;
				area[itype] += locarea;
			}
			error_avg[iz] /= (area[0] + area[1]);
			// additionally check if object area is close to that of the cylinder
			locerror = fabs_tpl((area[0] - r[iz] * h[iz] * pi * 4) * (rinv * hinv * ((real)0.5 / pi)));
			locerror = max(locerror, fabs_tpl((area[1] - r[iz] * r[iz] * pi * 2) * (rinv * rinv * ((real)0.5 / pi))));
			error_max[iz] = max(error_max[iz], locerror);
			error_avg[iz] = error_avg[iz] * (real)0.7 + locerror * (real)0.3;
			bBest = isneg(error_avg[iz] - error_avg[ibest]);
			ibest = ibest & ~- bBest | iz & -bBest;
		}

		if (ibest < 3 && error_max[ibest] < tolerance * 1.5f && error_avg[ibest] < tolerance) {
			acyl.axis = eigen_axes[ibest];
			acyl.center = c;
			acyl.r = r[ibest];
			acyl.hh = h[ibest];
			pprim = &acyl;
			return cylinder::type;
		}
	}

	if (flags & mesh_approx_box) {
		int itry, iside;
		matrix3x3f Basis = (matrix3x3RM&)eigen_axes[0];
		vectorf size[2], rsize, pt, ptmin, ptmax, c[2];
		real area;
		error_max[0] = error_avg[0] = error_max[1] = error_avg[1] = 1E10;

		for (itry = 0; itry < 2; itry++) {
			ptmin = ptmax = Basis * pVertices[pIndices[0]];
			for (i = 1; i < nTris * 3; i++) {
				pt = Basis * pVertices[pIndices[i]];
				ptmin.x = min(ptmin.x, pt.x); ptmax.x = max(ptmax.x, pt.x);
				ptmin.y = min(ptmin.y, pt.y); ptmax.y = max(ptmax.y, pt.y);
				ptmin.z = min(ptmin.z, pt.z); ptmax.z = max(ptmax.z, pt.z);
			}
			c[itry] = ((ptmin + ptmax) * 0.5f) * Basis;
			size[itry] = (ptmax - ptmin) * 0.5f;
			if (size[itry].x * size[itry].y * size[itry].z == 0)
				continue;
			error_max[itry] = error_avg[itry] = 0;
			rsize.x = 1.0f / size[itry].x; rsize.y = 1.0f / size[itry].y; rsize.z = 1.0f / size[itry].z;
			for (i = 0, area = 0; i < nTris; i++) {
				pt = Basis * ((pVertices[pIndices[i * 3]] + pVertices[pIndices[i * 3 + 1]] + pVertices[pIndices[i * 3 + 2]]) * (1.0f / 3) - c[itry]);
				pt.x = fabsf(pt.x * rsize.x); pt.y = fabsf(pt.y * rsize.y); pt.z = fabsf(pt.z * rsize.z);
				locarea = (pVertices[pIndices[i * 3 + 1]] - pVertices[pIndices[i * 3]] ^ pVertices[pIndices[i * 3 + 2]] - pVertices[pIndices[i * 3]]).len();
				iside = idxmax3(&pt.x);	locerror = 0;
				for (j = 0; j < 3; j++)
					locerror = max(locerror, fabs_tpl((fabs_tpl((pVertices[pIndices[i * 3 + j]] - c[itry]) * Basis.GetRow(iside))) * rsize[iside] - (real)1));
				error_max[itry] = max(error_max[itry], locerror);
				error_avg[itry] += locerror * locarea;
				area += locarea;
			}
			error_avg[itry] /= area;
			// additionally check if object area is close to that of the box
			locerror = fabs_tpl((size[itry].x * size[itry].y + size[itry].x * size[itry].z + size[itry].y * size[itry].z) * 16 - area);
			error_max[itry] = max(error_max[itry], locerror);
			error_avg[itry] = error_avg[itry] * (real)0.7 + locerror * (real)0.3;
			Basis.SetIdentity(); // try axis aligned box after eigen-vectors aligned 
		}

		ibest = isneg(error_avg[1] - error_avg[0]); //-tolerance*0.01f // favor axis-aligned box slightly
		if (error_max[ibest] < tolerance * 1.5f && error_avg[ibest] < tolerance) {
			abox.size = size[ibest];
			abox.center = c[ibest];
			if (ibest) {
				abox.bOriented = 0;
				abox.Basis.SetIdentity();
			}
			else {
				abox.bOriented = 1;
				abox.Basis = (matrix3x3RM&)eigen_axes[0];
			}
			pprim = &abox;
			return box::type;
		}
	}

	if (flags & mesh_approx_sphere) {
		vectorr p0, p1, p2, n;
		real r, rinv, area;
		for (i = 0, r = 0; i < nTris * 3; i++) r += (pVertices[pIndices[i]] - center).len();
		r /= nTris * 3;	rinv = (real)1.0 / r;
		error_max[0] = error_avg[0] = area = 0;
		for (i = 0; i < nTris; i++) {
			p0 = pVertices[pIndices[i * 3]]; p1 = pVertices[pIndices[i * 3 + 1]]; p2 = pVertices[pIndices[i * 3 + 2]];
			locerror = fabs_tpl((p0 - center).len() - r) * rinv;
			locerror = max(locerror, fabs_tpl((p1 - center).len() - r) * rinv);
			locerror = max(locerror, fabs_tpl((p2 - center).len() - r) * rinv);
			n = p1 - p0 ^ p2 - p0; locarea = n.len();
			if (locarea > 1E-5)
				locerror = max(locerror, fabs_tpl(((p0 - center) * n) / locarea - r) * rinv);
			error_max[0] = max(error_max[0], locerror);
			error_avg[0] += locerror * locarea;
			area += locarea;
		}
		error_avg[0] /= area;
		if (error_max[0] < tolerance * 1.5f && error_avg[0] < tolerance) {
			asphere.r = r;
			asphere.center = center;
			pprim = &asphere;
			return sphere::type;
		}
	}

	return triangle::type;
}

void ExtrudeBox(const box* pbox, const vectorf& dir, float step, box* pextbox)
{
	float proj, maxproj;
	int i;
	maxproj = (pbox->Basis.GetRow(0) - dir * (dir * pbox->Basis.GetRow(0))).len2() * pbox->size[0];
	proj = (pbox->Basis.GetRow(1) - dir * (dir * pbox->Basis.GetRow(1))).len2() * pbox->size[1];
	i = isneg(maxproj - proj); maxproj = max(proj, maxproj);
	proj = (pbox->Basis.GetRow(2) - dir * (dir * pbox->Basis.GetRow(2))).len2() * pbox->size[2];
	i |= isneg(maxproj - proj) << 1; i &= 2 | (i >> 1 ^ 1);

	pextbox->Basis.SetRow(2, dir);
	pextbox->Basis.SetRow(0, (pbox->Basis.GetRow(i) - dir * (dir * pbox->Basis.GetRow(i))).normalized());
	pextbox->Basis.SetRow(1, pextbox->Basis.GetRow(2) ^ pextbox->Basis.GetRow(0));
	pextbox->bOriented = 1;
	matrix3x3f mtx = pextbox->Basis * pbox->Basis.T();
	(pextbox->size = mtx.Fabs() * pbox->size).z += fabs_tpl(step) * 0.5f;
	pextbox->center = pbox->center + dir * (step * 0.5f);
}

real RotatePointToPlane(const vectorr& pt, const vectorr& axis, const vectorr& center, const vectorr& n, const vectorr& origin)
{
	vectorr ptc, ptz, ptx, pty;
	real kcos, ksin, k, a, b, c, d;
	ptc = pt - center;
	ptz = axis * (axis * ptc); ptx = ptc - ptz; pty = axis ^ ptx;
	kcos = ptx * n; ksin = pty * n; k = (ptz + center - origin) * n;
	a = sqr(ksin) + sqr(kcos); b = ksin * k; c = sqr(k) - sqr(kcos); d = b * b - a * c;
	if (d >= 0)
		return asin_tpl((sqrt_tpl(d) * sgnnz(b) - b) / a);
	else return 0;
}


void get_xqs_from_matrices(float* pMtx3x3, float* pMtx3x3T, float* pMtx4x4, float* pMtx4x4T, vectorf& pos, quaternionf& q, float& scale)
{
	if (pMtx4x4) {
		matrix3x3in4x4f& mtx((matrix3x3in4x4f&)*pMtx4x4);
		scale = mtx.GetRow(0).len();
		q = quaternionf(mtx / scale);
		pos = mtx.GetColumn(3);
	}
	else if (pMtx4x4T) {
		matrix3x3in4x4Tf& mtx((matrix3x3in4x4Tf&)*pMtx4x4T);
		scale = mtx.GetRow(0).len();
		q = quaternionf(mtx / scale);
		pos = mtx.GetColumn(3);
	}
	else if (pMtx3x3) {
		matrix3x3f& mtx((matrix3x3f&)*pMtx3x3);
		scale = mtx.GetRow(0).len();
		q = quaternionf(mtx / scale);
	}
	else if (pMtx3x3T) {
		matrix3x3Tf& mtx((matrix3x3Tf&)*pMtx3x3);
		scale = mtx.GetRow(0).len();
		q = quaternionf(mtx / scale);
	}
}


float frand(float range)
{
	return rand() * (1.0 / RAND_MAX) * range;
}


inline int get_neighb(int iTri, int iEdge, char* pCellDiv, int szx)
{
	static int offsx[8] = { 0,-1, 0, 1,-1, 0, 1, 0 };
	static int offsy[8] = { 1, 0,-1, 0, 0,-1, 0, 1 };
	static int buddytri[16] = { 1,1,0,0,1,0,0,1, 0,1,1,0,1,1,0,0 };

	if (iEdge == 2)
		return iTri ^ 1;
	int i = pCellDiv[iTri >> 1] << 2 | (iTri & 1) << 1 | iEdge & 1;
	int iBuddyCell = (iTri >> 1) + offsx[i] + offsy[i] * szx;
	return iBuddyCell * 2 + buddytri[pCellDiv[iBuddyCell] << 3 | i];
}

inline void get_edge_ends(int iTri, int iEdge, int& iend0, int& iend1, char* pCellDiv, int szx)
{
	int i = pCellDiv[iTri >> 1] + (iTri & 1 ^ 1) * 2 + iEdge & 3;
	iend0 = (iTri >> 1) + (i >> 1) * szx + (i & 1 ^ i >> 1);
	i = pCellDiv[iTri >> 1] + (iTri & 1 ^ 1) * 2 + inc_mod3[iEdge] & 3;
	iend1 = (iTri >> 1) + (i >> 1) * szx + (i & 1 ^ i >> 1);
}

inline void output_point(vector2df*& ptout, int& nOut, int& nOutAlloc, const vector2df& pt)
{
	if (nOut == nOutAlloc) {
		vector2df* tmp = ptout;
		memcpy(ptout = new vector2df[nOutAlloc += 32], tmp, nOut * sizeof(vector2df));
		delete[] tmp;
	}
	ptout[nOut++] = pt;
}


int BreakPolygon(vector2df* ptSrc, int nPt, int nCellx, int nCelly, int maxPatchTris, vector2df*& ptout, int*& nPtOut, float jointhresh, int seed)
{
	int i, j, k, i0, i1, ix, iy, nPatches, nSeedTris, nPatchTris, iTri, iTriNew, nCells, ihead, itail, ipt0, ipt1, nOut, iMotherPatch, szQueue, nPatchPt,
		nOutAlloc, bOut, iEdge, iPatch, iCurPatch, ilist[32], nInters, iPass, ix0, iy0, ix1, iy1, bHasInclusions, nPatchesOut, iExit, iEdge0, ipt, iEnter;
	vector2di sz;
	vector2df step, rstep, pt0, pt1, d0, d1, dc, ptmin, ptmax;
	float snap, rsnap;
	quotientf ytest, ymin, tlist[32], t0, t1;
	vector2df* ptin, * pt;
	int* pTris, * pGrid, * pHash, * queue, * pSeedTris, * pPatchSeeds, * pPatchMothers;
	char* pCellDiv;

	sz.set(nCellx + 3, nCelly + 3);
	for (i = 1, ptmin = ptmax = ptSrc[0]; i < nPt; i++) {
		ptmin.x = min(ptmin.x, ptSrc[i].x); ptmin.y = min(ptmin.y, ptSrc[i].y);
		ptmax.x = max(ptmax.x, ptSrc[i].x); ptmax.y = max(ptmax.y, ptSrc[i].y);
	}
	step.set((ptmax.x - ptmin.x) / nCellx, (ptmax.y - ptmin.y) / nCelly);
	rstep.set(1.0f / step.x, 1.0f / step.y);
	setexp(snap = 1.0f, getexp((step.x + step.y) * (1.0f / 256)));
	rsnap = 1.0f / snap;
	ptmin -= step * 1.5f;
	nCells = sz.x * sz.y;

	ptin = new vector2df[nPt + 1];
	pt = new vector2df[nCells];
	pTris = new int[nCells * 2];
	pCellDiv = new char[nCells];
	pGrid = new int[nCells + 1];
	for (szQueue = 8; szQueue < nCells >> 2; szQueue <<= 1);
	queue = new int[szQueue];
	pSeedTris = new int[nCells * 2];
	pPatchSeeds = new int[nCells * 2];
	pPatchMothers = new int[nCells * 2];
	memset(pGrid, 0, sizeof(int) * (nCells + 1));

	if (seed != -1)
		srand((unsigned int)seed);

	for (i = 0; i < nPt; i++) // snap source points
		ptin[i].set(float2int((ptSrc[i].x - ptmin.x) * rsnap) * snap, float2int((ptSrc[i].y - ptmin.y) * rsnap) * snap);
	ptin[i] = ptin[0];

	for (ix = 0; ix < sz.x; ix++) for (iy = 0; iy < sz.y; iy++)	// generate jittered grid points
		pt[ix + iy * sz.x].set(
			(float2int((ix * step.x + frand(step.x * 0.45f) - snap * 0.5f) * rsnap) + 0.5f) * snap,
			(float2int((iy * step.y + frand(step.y * 0.45f) - snap * 0.5f) * rsnap) + 0.5f) * snap);

	// register source sements in a hash
	for (iPass = 0; iPass < 2; iPass++) {
		for (i = 0; i < nPt; i++) {
			ix0 = float2int(min(ptin[i].x, ptin[i + 1].x) * rstep.x);
			ix1 = float2int(max(ptin[i].x, ptin[i + 1].x) * rstep.x);
			iy0 = float2int(min(ptin[i].y, ptin[i + 1].y) * rstep.y);
			iy1 = float2int(max(ptin[i].y, ptin[i + 1].y) * rstep.y);
			if (iPass == 0) for (ix = ix0; ix <= ix1; ix++) for (iy = iy0; iy <= iy1; iy++)
				pGrid[ix + iy * sz.x]++;	// calculate number of source points in each cell
			else for (ix = ix0; ix <= ix1; ix++) for (iy = iy0; iy <= iy1; iy++)
				pHash[--pGrid[ix + iy * sz.x]] = i; // add reference to this segment into a corresponding cell
		}
		if (iPass == 0) {
			for (i = 1; i <= sz.x * sz.y; i++) pGrid[i] += pGrid[i - 1];
			pHash = new int[pGrid[sz.x * sz.y]];
		}
	}

	for (i = nCells - 1; i >= 0; i--) {
		pCellDiv[i] = rand() & 1;	// randomly choose the way each cell is split
		pTris[i * 2] = pTris[i * 2 + 1] = -1;
	}
	// mark borders
	for (ix = 0; ix < sz.x; ix++) pTris[ix * 2] = pTris[ix * 2 + 1] = pTris[(ix + sz.x * (sz.y - 1)) * 2] = pTris[(ix + sz.x * (sz.y - 1)) * 2 + 1] = 3 << 29;
	for (iy = 0; iy < sz.y; iy++) pTris[iy * sz.x * 2] = pTris[iy * sz.x * 2 + 1] = pTris[(iy * sz.x + sz.x - 1) * 2] = pTris[(iy * sz.x + sz.x - 1) * 2 + 1] = 3 << 29;

	// unite grid triangles into patches by randomly growing them 
	pTris[pPatchSeeds[0] = queue[0] = ((sz.y >> 1) + (sz.x >> 1) * sz.x) * 2] = 0; itail = 0; ihead = 1;
	nSeedTris = 0; nPatches = 0;
	do {
		nPatchTris = float2int(frand(maxPatchTris) - 0.5f);	// request random number of triangles for this patch
		for (i = 0; i < 3; i++) if (pTris[iTri = get_neighb(queue[itail], i, pCellDiv, sz.x)] == -1)
			pTris[pSeedTris[nSeedTris++] = iTri] = -2; // immediately queue neighbours as potential subsequent patch seeds
		if (nPatchTris > 0) do { // grow iCurPatch around a seed triangle
			iTri = queue[itail]; itail = itail + 1 & szQueue - 1;
			if (pTris[iTri] < 0) {
				if (frand(1) > jointhresh) {
					pTris[iTri] = nPatches; nPatchTris--;
					pPatchSeeds[nPatches] = iTri;
				}
				else if (pTris[iTri] == -1)
					pTris[pSeedTris[nSeedTris++] = iTri] = -2;
			}
			if (pTris[iTri] == nPatches)
				for (i = 0; i < 3; i++) if (pTris[get_neighb(iTri, i, pCellDiv, sz.x)] < 0) {
					queue[ihead] = get_neighb(iTri, i, pCellDiv, sz.x); ihead = ihead + 1 & szQueue - 1;
				}
		} while (nPatchTris > 0 && ihead != itail);

		pPatchMothers[nPatches++] = -1;
		for (i = j = 0; i < nSeedTris; i++) if (pTris[pSeedTris[i]] < 0)
			pSeedTris[j++] = pSeedTris[i];
		for (; ihead != itail; itail = itail + 1 & szQueue - 1) if (pTris[queue[itail]] == -1)
			pTris[pSeedTris[j++] = queue[itail]] = -2;
		if ((nSeedTris = j) == 0)	// all triangles are assigned to patches
			break;
		pTris[pPatchSeeds[nPatches] = queue[0] = pSeedTris[0]] = nPatches; itail = 0; ihead = 1;
	} while (true);

	// find patches that border upon only one patch (=are contained inside it) and merge them with this enclosing patch
	do {
		for (iCurPatch = bHasInclusions = 0; iCurPatch < nPatches; iCurPatch++) if (pPatchMothers[iCurPatch] >= -1) {
			pTris[queue[0] = pPatchSeeds[iCurPatch]] |= 1 << 30; itail = 0; ihead = 1; iMotherPatch = -1;

			do {
				iTri = queue[itail]; itail = itail + 1 & szQueue - 1;
				for (iEdge = 0; iEdge < 3; iEdge++) {
					iPatch = pTris[iTriNew = get_neighb(iTri, iEdge, pCellDiv, sz.x)] & ~(1 << 30);
					if (pPatchMothers[iCurPatch] >= 0) // assign tri to outer patch, if it exists
						pTris[iTri] = pPatchMothers[iCurPatch] | 1 << 30;

					if (iPatch != iCurPatch && iPatch < nCells * 2 && pPatchMothers[iCurPatch] < 0) {
						pPatchSeeds[iCurPatch] = iTri;
						if (iMotherPatch < 0)
							iMotherPatch = iPatch;
						else if (iPatch != iMotherPatch)
							break; // we have at least 2 distinct neighbours
					}
					else if (pTris[iTriNew] == iCurPatch) { // check if this patch tri hasn't been processed yet
						queue[ihead] = iTriNew; ihead = ihead + 1 & szQueue - 1;
						pTris[iTriNew] |= 1 << 30;
					}
				}
			} while (itail != ihead && iEdge == 3);

			if (pPatchMothers[iCurPatch] >= 0) {
				pPatchMothers[pPatchMothers[iCurPatch]] = -1; // 'needs recheck' state
				pPatchMothers[iCurPatch] = -3; // 'always ignore this patch from now on' state
				bHasInclusions = 1;
			}
			else {
				if (iEdge == 3) {
					pPatchMothers[iCurPatch] = iMotherPatch;
					bHasInclusions = 1;
				}
				else
					pPatchMothers[iCurPatch] = -2; // 'checked' state
			}
		}

		for (i = nCells * 2 - 1; i >= 0; i--) pTris[i] &= ~(1 << 30);
		if (bHasInclusions) do { // merge nested inclusions
			for (i = j = 0; i < nPatches; i++)	if (pPatchMothers[i] >= 0 && pPatchMothers[pPatchMothers[i]] >= 0) {
				pPatchMothers[i] = pPatchMothers[pPatchMothers[i]]; j++;
			}
		} while (j);
	} while (bHasInclusions);

	nPtOut = new int[nPatches];
	ptout = new vector2df[nOutAlloc = max(16, ((sz.x - 2) * (sz.y - 2) / nPatches) >> 2)];
	nPatchesOut = nOut = 0;

	// generate pieces geometries by intersecting patches with the source polygon 
	for (iCurPatch = 0; iCurPatch < nPatches; iCurPatch++) {
		if (pPatchMothers[iCurPatch] != -2)
			continue;
		iTri = pPatchSeeds[iCurPatch];
		for (iEdge = 0; iEdge < 3 && pTris[get_neighb(iTri, iEdge, pCellDiv, sz.x)] == iCurPatch; iEdge++);
		nPatchPt = nOut; iEdge0 = iEdge; iExit = iEnter = -1;

		// find initial bOut state
		get_edge_ends(iTri, iEdge, ipt0, ipt1, pCellDiv, sz.x);
		ymin.set(1, 0); bOut = 1;
		for (ipt = ipt0; ipt < nCells; ipt += sz.x) for (i = pGrid[ipt]; i < pGrid[ipt + 1]; i++)
			if (inrange(pt[ipt0].x, ptin[pHash[i]].x, ptin[pHash[i] + 1].x)) {
				pt0 = ptin[pHash[i]]; pt1 = ptin[pHash[i] + 1];
				(ytest = quotientf((pt[ipt0].x - pt0.x) * (pt1.y - pt0.y), pt1.x - pt0.x) + pt0.y).fixsign();
				if (ytest > pt[ipt0].y && ytest < ymin) {
					ymin = ytest; bOut = isneg(pt0.x - pt1.x);
				}
			}

		do {
			get_edge_ends(iTri, iEdge, ipt0, ipt1, pCellDiv, sz.x);
			// check all source line segments in ipt0-ipt1 rect for intersection with this edge
			d0 = pt[ipt1] - pt[ipt0];	nInters = 0;
			for (ix = -1, iy = 0; ix <= 1; ix++) for (iy = -sz.x; iy <= sz.x; iy += sz.x) // ix,iy - rectange dimensions
				if (ipt1 == ipt0 + ix + iy) goto foundixiy;
		foundixiy:
			for (i0 = iszero(ix) ^ 1; i0 >= 0; i0--) for (i1 = iszero(iy) ^ 1; i1 >= 0; i1--)
				for (i = pGrid[ipt0 + (ix & -i0) + (iy & -i1)]; i < pGrid[ipt0 + (ix & -i0) + (iy & -i1) + 1]; i++) {
					d1 = ptin[pHash[i] + 1] - ptin[pHash[i]]; dc = ptin[pHash[i]] - pt[ipt0];
					t0.y = t1.y = d0 ^ d1; t0.x = dc ^ d1; t1.x = dc ^ d0;
					if (t0.isin01() & t1.isin01()) { // we have a valid intersection
						for (j = 0; j < nInters && tlist[j] < t0; j++); // keep a sorted array of intersection t's
						if (j < nInters&& tlist[j] == t0)
							continue; // skip duplicates
						for (k = nInters; k > j; k--) { tlist[k] = tlist[k - 1]; ilist[k] = ilist[k - 1]; }
						tlist[j] = t0; ilist[j] = pHash[i]; nInters++;
					}
				}
			for (i = 0; i < nInters; i++) {
				if (bOut) { // output points from iExit to j
					if (iExit >= 0 && iExit != ilist[i]) do {
						iExit = iExit + 1 & ~- isneg(nPt - iExit - 2);
						output_point(ptout, nOut, nOutAlloc, ptin[iExit] + ptmin);
					} while (iExit != ilist[i]);
					if (iEnter < 0)
						iEnter = ilist[i];
				}
				else
					iExit = ilist[i];
				// add intersection point to output
				output_point(ptout, nOut, nOutAlloc, pt[ipt0] + (pt[ipt1] - pt[ipt0]) * tlist[i].val() + ptmin);
				bOut ^= 1;
			}
			if (!bOut)
				output_point(ptout, nOut, nOutAlloc, pt[ipt1] + ptmin);

			do {
				iEdge = inc_mod3[iEdge];
				iTriNew = get_neighb(iTri, iEdge, pCellDiv, sz.x);
				if (pTris[iTriNew] != iCurPatch)
					break;
				for (iEdge = 0; iEdge < 2 && get_neighb(iTriNew, iEdge, pCellDiv, sz.x) != iTri; iEdge++);
				iTri = iTriNew;
			} while (true);
		} while (iTri != pPatchSeeds[iCurPatch] || iEdge != iEdge0);

		if (bOut && iEnter >= 0 && iExit != iEnter) do {
			iExit = iExit + 1 & ~- isneg(nPt - iExit - 2);
			output_point(ptout, nOut, nOutAlloc, ptin[iExit] + ptmin);
		} while (iExit != iEnter);

		if (nPatchPt = nOut - nPatchPt)
			nPtOut[nPatchesOut++] = nPatchPt;
	}

	delete[] pGrid; delete[] pHash;
	delete[] ptin; delete[] pt;
	delete[] pTris; delete[] pCellDiv;
	delete[] queue; delete[] pSeedTris; delete[] pPatchSeeds; delete[] pPatchMothers;

	return nPatchesOut;
}


int GetProjCubePlane(const vectorf& pt)
{
	int iPlane = isneg(fabsf(pt.x) - fabsf(pt.y));
	iPlane |= isneg(fabsf(pt[iPlane]) - fabsf(pt.z)) << 1; iPlane &= 2 | (iPlane >> 1 ^ 1);
	return iPlane << 1 | isnonneg(pt[iPlane]);
}


void RasterizePolygonIntoCubemap(const vectorf* pt, int npt, int iPass, int* pGrid[6], int nRes, float rmin, float rmax, float zscale)
{
	int iPlane, iPlaneEnd, ipt, ipt1, i, j, iBrd[2], iBrdNext[2], idx, lxdir, iSign, ixlim[2], iylim, imask, idcell, iz,
		irmin, iyend, iOrder, nCells, ixleft, iEnter, nPlanes;
	vectori ic;
	vector2di icell, ibound;
	vectorf nplane, n, rn;
	vector2df ptint[2], dp[2];
	float kres, krres, koffs, dz, dp0[2], denom;
	quotientf z;

	struct cube_plane {
		int iEnter, iExit;
		float minz;
		int npt;
		vector2df pt[32];
	};
	static cube_plane planes[6];

	nplane = pt[1] - pt[0] ^ pt[2] - pt[0];
	if (isnonneg(nplane * pt[0]) ^ iPass)
		return;
	kres = 0.5f * nRes; krres = 2.0f / nRes;
	koffs = 1.0f - krres * 0.5f;
	irmin = float2int(rmin * zscale);
	for (i = 0; i < 6; i++) {
		planes[i].npt = 0; planes[i].iExit = -1; planes[i].minz = rmax;
	}
	z.x = pt[0] * nplane;

	for (i = 0; i < 3; i++) for (ipt = 0; ipt < npt; ipt++) {
		planes[i * 2 + 0].minz = min(planes[i * 2 + 0].minz, max(0.0f, -pt[ipt][i]));
		planes[i * 2 + 1].minz = min(planes[i * 2 + 1].minz, max(0.0f, pt[ipt][i]));
	}

	for (ipt = 0; ipt < npt; ipt++) {
		ipt1 = ipt + 1 & ipt - npt + 1 >> 31;
		iPlane = GetProjCubePlane(pt[ipt]);
		iPlaneEnd = GetProjCubePlane(pt[ipt1]);
		n = pt[ipt] ^ pt[ipt1]; rn.zero();
		ic.z = iPlane >> 1;	denom = 1.0f / (fabsf(pt[ipt][ic.z]) + isneg(fabsf(pt[ipt][ic.z]) - 1E-5f) * 1E4f);	// store the starting point
		planes[iPlane].pt[planes[iPlane].npt++ & 31].set(pt[ipt][inc_mod3[ic.z]] * denom, pt[ipt][dec_mod3[ic.z]] * denom);

		for (nPlanes = 0; iPlane != iPlaneEnd && nPlanes < 6; nPlanes++) {
			ic.z = iPlane >> 1; iSign = ((iPlane & 1) << 1) - 1;
			ic.x = inc_mod3[ic.z]; ic.y = dec_mod3[ic.z];
			ibound.x = sgnnz(n[ic.y]) * iSign; ibound.y = -sgnnz(n[ic.x]) * iSign;
			ptint[0].x = ibound.x; // intersect line with face boundary conditions and take the intersection that is inside face
			ptint[0].y = -n[ic.z] * iSign - n[ic.x] * ptint[0].x;	// only check boundaries that lie farther along ccw movement of line around origin-edge plane normal
			ptint[1].y = ibound.y;
			ptint[1].x = -n[ic.z] * iSign - n[ic.y] * ptint[1].y;
			idx = inrange(ptint[1].x, -n[ic.x], n[ic.x]);
			if (rn[ic[idx ^ 1]] == 0) rn[ic[idx ^ 1]] = 1.0f / (n[ic[idx ^ 1]] + isneg(fabsf(n[ic[idx ^ 1]]) - 1E-5f) * 1E4f);
			ptint[idx][idx ^ 1] *= rn[ic[idx ^ 1]];
			// store ptint[idx] in iPlane's point list
			planes[iPlane].pt[planes[iPlane].npt++ & 31] = ptint[idx];
			planes[iPlane].iExit = idx + 1 - ibound[idx];
			iPlane = ic[idx] << 1 | ibound[idx] + 1 >> 1;
			iEnter = (idx ^ 1) + 1 - iSign;
			if (planes[iPlane].iExit >= 0) { // add corner points between the last exit point and this enter point
				iOrder = (((iPlane & 1) << 1) - 1) * ((iPass << 1) - 1); j = iOrder >> 31;
				for (i = planes[iPlane].iExit; i != iEnter; i = i + iOrder & 3)
					planes[iPlane].pt[planes[iPlane].npt++ & 31].set(1 - ((i + j ^ i + j << 1) & 2), 1 - (i + j & 2));
				planes[iPlane].iExit = -1;
			}
			else
				planes[iPlane].iEnter = iEnter;
			// store ptint[idx] in the new iPlane's point list (transform it to the new iPlane beforehand)
			ptint[idx][idx] = ptint[idx][idx ^ 1];
			ptint[idx][idx ^ 1] = iSign;
			planes[iPlane].pt[planes[iPlane].npt++ & 31] = ptint[idx];
			if (planes[iPlane].npt > 32)
				planes[iPlane].npt = 0; // should not happen
		}
	}

	for (iPlane = nCells = i = 0, ic.z = 6; iPlane < 6; iPlane++) {
		j = iszero(planes[iPlane].npt) ^ 1;
		nCells += j; j <<= iPlane >> 1;	// j = plane axis bit
		ic.z -= (iPlane >> 1) + 1 & -(j & (i & j ^ j)) >> 31; // subtract plane axis index+1 from the sum if this axis hasn't been encountered yet
		i |= j;	// accumulate used axes mask
	}
	if (nCells == 4 && ic.z >= 1 && ic.z <= 3) {
		// we have 4 sides that form a 'ring', meaning one (and only one) axis is unaffected edges
		ic.z--; iPlane = isneg(nplane[ic.z]) ^ iPass | ic.z << 1;
		iOrder = (((iPlane & 1) << 1) - 1) * ((iPass << 1) - 1); j = iOrder >> 31;
		for (i = 0; planes[iPlane].npt < 4; i = i + iOrder & 3)
			planes[iPlane].pt[planes[iPlane].npt++ & 31].set(1 - ((i + j ^ i + j << 1) & 2), 1 - (i + j & 2));
	}

	// rasterize resulting polygons in each plane
	for (iPlane = nCells = 0; iPlane < 6; iPlane++) {
		iOrder = (((iPlane & 1) << 1) - 1) * ((iPass << 1) - 1); j = iOrder >> 31;
		if (planes[iPlane].iExit >= 0) // close pending exits
			for (i = planes[iPlane].iExit; i != planes[iPlane].iEnter; i = i + iOrder & 3)
				planes[iPlane].pt[planes[iPlane].npt++ & 31].set(1 - ((i + j ^ i + j << 1) & 2), 1 - (i + j & 2));

		if (planes[iPlane].npt && planes[iPlane].npt < 32) {
			ic.z = iPlane >> 1; ic.x = inc_mod3[ic.z]; ic.y = dec_mod3[ic.z]; iSign = (iPlane & 1) * 2 - 1;
			dz = nplane[ic.x] * krres;

			for (i = 1, iBrd[0] = iBrd[1] = 0; i < planes[iPlane].npt; i++) {	// find the uppermost and the lowest points
				imask = -isneg(planes[iPlane].pt[iBrd[0]].y - planes[iPlane].pt[i].y);
				iBrd[0] = iBrd[0] & ~imask | i & imask;
				imask = -isneg(planes[iPlane].pt[i].y - planes[iPlane].pt[iBrd[1]].y);
				iBrd[1] = iBrd[1] & ~imask | i & imask;
			}
			iyend = min(nRes - 1, max(0, float2int((planes[iPlane].pt[iBrd[1]].y + koffs) * kres + 0.5f)));
			ixleft = min(nRes - 1, max(0, float2int((planes[iPlane].pt[iBrd[0]].x + koffs) * kres)));
			icell.y = min(nRes - 1, max(0, float2int((planes[iPlane].pt[iBrd[0]].y + koffs) * kres)));
			iBrd[1] = iBrd[0];

			do {
				iBrdNext[0] = iBrd[0] + iOrder;
				iBrdNext[0] += planes[iPlane].npt & iBrdNext[0] >> 31; // wrap -1 to npt-1 and npt to 0
				iBrdNext[0] &= iBrdNext[0] - planes[iPlane].npt >> 31;
				iBrdNext[1] = iBrd[1] - iOrder;
				iBrdNext[1] += planes[iPlane].npt & iBrdNext[1] >> 31; // wrap -1 to npt-1 and npt to 0
				iBrdNext[1] &= iBrdNext[1] - planes[iPlane].npt >> 31;
				idx = isneg(planes[iPlane].pt[iBrdNext[0]].y - planes[iPlane].pt[iBrdNext[1]].y);
				dp[0] = planes[iPlane].pt[iBrdNext[0]] - planes[iPlane].pt[iBrd[0]];
				dp0[0] = (dp[0] ^ planes[iPlane].pt[iBrd[0]] + vector2df(koffs, koffs)) * kres;
				dp[1] = planes[iPlane].pt[iBrdNext[1]] - planes[iPlane].pt[iBrd[1]];
				dp0[1] = (dp[1] ^ planes[iPlane].pt[iBrd[1]] + vector2df(koffs, koffs)) * kres;
				lxdir = sgnnz(dp[0].x);
				ixlim[0] = min(nRes - 1, max(0, float2int((min(planes[iPlane].pt[iBrd[0]].x, planes[iPlane].pt[iBrdNext[0]].x) + koffs) * kres)));
				ixlim[1] = min(nRes - 1, max(0, float2int((max(planes[iPlane].pt[iBrd[1]].x, planes[iPlane].pt[iBrdNext[1]].x) + koffs) * kres)));
				icell.y = max(icell.y, iyend);
				iylim = min(nRes - 1, max(iyend, float2int((planes[iPlane].pt[iBrdNext[idx]].y + koffs) * kres)));
				do {
					// search left or right (dep. on sgn(dp.x)) from the left border to find the leftmost filled cell
					// left: iterate while point is inside and x!=xmin-1, increment x after loop; right: while point is outside and x!=xmax
					for (icell.x = ixleft; isneg((dp[0] ^ icell) * lxdir - dp0[0] * lxdir) & (iszero(ixlim[lxdir + 1 >> 1] + (lxdir >> 31) - icell.x) ^ 1); icell.x += lxdir);
					icell.x -= lxdir >> 31; ixleft = icell.x;
					// search right from the leftmost cell to the end of the right border, filling data
					z.y = nplane[ic.x] * (icell.x * krres - koffs) + nplane[ic.y] * (icell.y * krres - koffs) + nplane[ic.z] * iSign;
					idcell = icell.x + icell.y * nRes;
					// 1st (front face) pass output: 
					//  iz<rmin - set the highest bit
					//  else - update z value in the lower 30 bits
					// 2nd (back face) pass output:
					//  iz<rmin - clear the highest bit
					//  else - update z value in the lower 30 bits
					// after both passes: 
					//  if the highest bit is set, change cell z to irmin
					if (iPass == 0) for (; isneg((dp[1] ^ icell) - dp0[1]) & isneg(icell.x - ixlim[1] - 1); icell.x++, z.y += dz, idcell++) {
						iz = float2int(max(planes[iPlane].minz, min(fabsf(z.val()), rmax)) * zscale); nCells++;
						imask = iz - irmin >> 31; iz = (iz | imask) & (1u << 31) - 1;
						pGrid[iPlane][idcell] = min(pGrid[iPlane][idcell] & (1u << 31) - 1, iz) | (pGrid[iPlane][idcell] | imask) & (1 << 31);
					}
					else for (; isneg((dp[1] ^ icell) - dp0[1]) & isneg(icell.x - ixlim[1] - 1); icell.x++, z.y += dz, idcell++) {
						iz = float2int(max(planes[iPlane].minz, min(fabsf(z.val()), rmax)) * zscale); nCells++;
						//pGrid[iPlane][idcell] &= irmin-iz>>31 | (1u<<31)-1;
						imask = iz - irmin >> 31; iz = (iz | imask) & (1u << 31) - 1;
						pGrid[iPlane][idcell] = min(pGrid[iPlane][idcell] & (1u << 31) - 1, iz) | (pGrid[iPlane][idcell] & ~imask) & (1 << 31);
					}
				} while (--icell.y >= iylim);
				iBrd[idx] = iBrdNext[idx];
				ixleft = ixleft & -idx | ixlim[0] & ~- idx; // update ixleft if the left branch advances
			} while (iylim > iyend);
		}
	}

	if (nCells == 0) { // do not allow objects to take no rasterized space
		iPlane = GetProjCubePlane(pt[0]);
		ic.z = iPlane >> 1; ic.x = inc_mod3[ic.z]; ic.y = dec_mod3[ic.z];
		denom = 1.0f / fabsf(pt[0][ic.z]);
		icell.x = min(nRes - 1, max(0, float2int((pt[0][ic.x] * denom + koffs) * kres)));
		icell.y = min(nRes - 1, max(0, float2int((pt[0][ic.y] * denom + koffs) * kres)));
		idcell = icell.x + icell.y * nRes;
		iz = float2int(min(fabsf(pt[0][ic.z]), rmax) * zscale);
		if (iPass == 0) {
			imask = iz - irmin >> 31; iz = (iz | imask) & (1u << 31) - 1;
			pGrid[iPlane][idcell] = min(pGrid[iPlane][idcell] & (1u << 31) - 1, iz) | (pGrid[iPlane][idcell] | imask) & (1 << 31);
		}
		else
			pGrid[iPlane][idcell] &= irmin - iz >> 31 | (1u << 31) - 1;
	}
}


inline int get_cubemap_cell_buddy(int idCell, int iBuddy, int nRes)
{
	int idx, istep, bWrap, idBuddy, idWrappedBuddy;
	vectori icell, iaxis;
	idx = iBuddy & 1; // step axis: 0 - local x, 1 - local y
	istep = (iBuddy & 2) - 1; // step direction

	icell[idx] = (idCell >> 8 * idx & 0xFF) + istep; // unpacked cell (x,y,z)
	icell[idx ^ 1] = idCell >> 8 * (idx ^ 1) & 0xFF;
	icell.z = nRes - 1 & -(idCell >> 16 & 1);

	iaxis.z = idCell >> 17;
	iaxis.x = inc_mod3[iaxis.z];
	iaxis.y = dec_mod3[iaxis.z];

	idBuddy = icell.x | icell.y << 8 | idCell & 0x70000;
	idWrappedBuddy = icell[idx ^ 1] << 8 * idx | icell.z << 8 * (idx ^ 1) | iaxis[idx] << 17 | istep + 1 << 15;

	bWrap = icell[idx] >> 31 | nRes - 1 - icell[idx] >> 31;
	return idWrappedBuddy & bWrap | idBuddy & ~bWrap;
}

void GrowAndCompareCubemaps(int* pGridOcc[6], int* pGrid[6], int nRes, int nGrow, int& nCells, int& nOccludedCells)
{
	struct cell_info {
		int idcell;
		int z;
	};
	int i, iPlane, icell, icell1, idcell, idcell1, ix, iy, bUsed, bVisible, ipass, ihead = 0, itail = 0, itailend, imaxz = (1u << 31) - 1;
	cell_info queue[4096];
	nCells = nOccludedCells = 0;

	for (iPlane = 0; iPlane < 6; iPlane++) {
		for (iy = 0; iy < nRes; iy++) for (ix = 0; ix < nRes; ix++) {
			icell = iy * nRes + ix;
			bUsed = isneg(pGrid[iPlane][icell] - imaxz);
			bVisible = isneg(pGrid[iPlane][icell] - pGridOcc[iPlane][icell]);
			nCells += bUsed; nOccludedCells += bUsed & (bVisible ^ 1);

			if (bUsed & -nGrow >> 31) for (i = 0, idcell = ix | iy << 8 | iPlane << 16; i < 4; i++) { // enqueue neighbouring unused cells
				idcell1 = get_cubemap_cell_buddy(idcell, i, nRes);
				icell1 = (idcell1 >> 8 & 0xFF) * nRes + (idcell1 & 0xFF);
				if (pGrid[idcell1 >> 16][icell1] >= imaxz) {
					queue[ihead].idcell = idcell1; queue[ihead].z = pGrid[iPlane][icell];
					ihead = ihead + 1 & sizeof(queue) / sizeof(queue[0]) - 1;
				}
			}
		}
	}

	for (ipass = 0; ipass < nGrow; ipass++)
		for (itailend = ihead; itail != itailend; itail = itail + 1 & sizeof(queue) / sizeof(queue[0]) - 1) {
			idcell = queue[itail].idcell;
			icell = (idcell >> 8 & 0xFF) * nRes + (idcell & 0xFF);
			iPlane = idcell >> 16;
			if (pGrid[iPlane][icell] < imaxz)
				continue;
			pGrid[iPlane][icell] = queue[itail].z;

			bVisible = isneg(pGrid[iPlane][icell] - pGridOcc[iPlane][icell]);
			nCells++; nOccludedCells += (bVisible ^ 1);

			for (i = 0; i < 4; i++) { // enqueue neighbouring unused cells
				idcell1 = get_cubemap_cell_buddy(idcell, i, nRes);
				icell1 = (idcell1 >> 8 & 0xFF) * nRes + (idcell1 & 0xFF);
				if (pGrid[idcell1 >> 16][icell1] >= imaxz) {
					queue[ihead].idcell = idcell1; queue[ihead].z = pGrid[iPlane][icell];
					ihead = ihead + 1 & sizeof(queue) / sizeof(queue[0]) - 1;
				}
			}
		}
}


void debug_calc_tri_resistance(const vectorf* pt, const vectorf& n, const vectorf& v, const vectorf& w, vectorf& P, vectorf& L)
{
	float square = (pt[1] - pt[0] ^ pt[2] - pt[0]).len2() * 0.25f;
	if (square < sqr(0.01f)) {
		vectorf r = (pt[0] + pt[1] + pt[2]) * (1.0f / 3);
		vectorf vloc = v + (w ^ r), dP = n * ((n * vloc) * sqrt_tpl(square));
		P += dP; L += r ^ dP;
		return;
	}
	vectorf subpt[3];
	subpt[0] = (pt[0] + pt[2]) * 0.5f; subpt[1] = pt[0]; subpt[2] = (pt[0] + pt[1]) * 0.5f;
	debug_calc_tri_resistance(subpt, n, v, w, P, L);
	subpt[0] = pt[1]; subpt[1] = (pt[1] + pt[2]) * 0.5f;
	debug_calc_tri_resistance(subpt, n, v, w, P, L);
	subpt[0] = (pt[0] + pt[2]) * 0.5f; subpt[2] = pt[2];
	debug_calc_tri_resistance(subpt, n, v, w, P, L);
	subpt[1] = (pt[0] + pt[1]) * 0.5f; subpt[2] = (pt[1] + pt[2]) * 0.5f;
	debug_calc_tri_resistance(subpt, n, v, w, P, L);
}


int crop_polygon_with_plane(const vectorf* ptsrc, int nsrc, vectorf* ptdst, const vectorf& n, float d)
{
	int i0, i1, ndst, iCount;
	for (i0 = 0; i0 < nsrc && ptsrc[i0] * n >= d; i0++);
	if (i0 == nsrc)
		return 0;
	for (iCount = ndst = 0; iCount < nsrc; iCount++, i0 = i1) {
		i1 = i0 + 1 & i0 - nsrc + 1 >> 31;
		ptdst[ndst] = ptsrc[i0];
		ndst += isneg(ptsrc[i0] * n - d);
		if ((ptsrc[i0] * n - d) * (ptsrc[i1] * n - d) < 0)
			ptdst[ndst++] = ptsrc[i0] + (ptsrc[i1] - ptsrc[i0]) * ((d - ptsrc[i0] * n) / ((ptsrc[i1] - ptsrc[i0]) * n));
	}
	return ndst;
}


void CalcMediumResistance(const vectorf* ptsrc, int npt, const vectorf& n, const plane& waterPlane,
	const vectorf& vworld, const vectorf& wworld, const vectorf& com, vectorf& P, vectorf& L)
{
	int i;
	vectorf pt0[16], pt[16], v, w, rotax, dP(zero), dL(zero);
	float x0, y0, dx, dy, Fxy, Fxx, Fxxy, Fxyy, Fxxx, square = 0, sina;
	npt = crop_polygon_with_plane(ptsrc, npt, pt0, waterPlane.n, waterPlane.origin * waterPlane.n);
	for (i = 0; i < npt; i++) pt0[i] -= com;
	npt = crop_polygon_with_plane(pt0, npt, pt, wworld ^ n, vworld * n);

	/*vectorf dP1(zero),dL1(zero); pt[npt] = pt[0];
	for(i=0,pt0[0].zero();i<npt;i++) pt0[0]+=pt[i]; pt0[0]/=npt;
	for(i=0;i<npt;i++) {
		pt0[1]=pt[i]; pt0[2]=pt[i+1];
		debug_calc_tri_resistance(pt0,n, vworld,wworld, dP1,dL1);
	}*/

	rotax = n ^ vectorf(0, 0, 1);
	sina = rotax.len();
	if (sina > 0.001f)
		rotax /= sina;
	else
		rotax.Set(1, 0, 0);
	v = vworld.rotated(rotax, n.z, sina);
	w = wworld.rotated(rotax, n.z, sina);
	for (i = 0; i < npt; i++)
		pt[i] = pt[i].rotated(rotax, n.z, sina);
	pt[npt] = pt[0];

	for (i = 0; i < npt; i++) {
		square += pt[i].x * pt[i + 1].y - pt[i + 1].x * pt[i].y;
		x0 = pt[i].x; y0 = pt[i].y; dx = pt[i + 1].x - pt[i].x; dy = pt[i + 1].y - pt[i].y;
		Fxy = x0 * y0 + (dx * y0 + dy * x0) * 0.5f + dx * dy * (1.0f / 3);
		Fxx = x0 * x0 + dx * x0 + dx * dx * (1.0f / 3);
		dP.z += dy * (w.x * Fxy - w.y * 0.5f * Fxx);
		dL.x += v.z * dy * Fxy;
		dL.y -= v.z * dy * 0.5f * Fxx;
		Fxxy = dx * dx * dy * 0.25f + (dx * dx * y0 + dx * dy * x0 * 2) * (1.0f / 3) + (x0 * y0 * dx * 2 + x0 * x0 * dy) * 0.5f + x0 * x0 * y0;
		Fxyy = dy * dy * dx * 0.25f + (dy * dy * x0 + dy * dx * y0 * 2) * (1.0f / 3) + (y0 * x0 * dy * 2 + y0 * y0 * dx) * 0.5f + y0 * y0 * x0;
		Fxxx = dx * dx * dx * 0.25f + dx * dx * x0 + dx * x0 * x0 * 1.5f + x0 * x0 * x0;
		dL.x += dy * (w.x * Fxyy - w.y * 0.5f * Fxxy);
		dL.y -= dy * (w.x * 0.5 * Fxxy - w.y * (1.0f / 3) * Fxxx);
	}
	dP.z += v.z * square * 0.5f;
	P -= dP.rotated(rotax, n.z, -sina);
	L -= dL.rotated(rotax, n.z, -sina);
}


static const int g_nRanges = 5;
static int g_rngb2a[] = { 0,'A',26,'a',52,'0',62,'+',63,'/' };
static int g_rnga2b[] = { '+',62,'/',63,'0',52,'A',0,'a',26 };
inline int mapsymb(int symb, int* pmap, int n) {
	int i, j;
	for (i = j = 0; j < n; j++) i += isneg(symb - pmap[j * 2]); i = n - 1 - i;
	return symb - pmap[i * 2] + pmap[i * 2 + 1];
}
int g_bitcount[256];
struct ComputeBitCount {
	ComputeBitCount() {
		for (int i = 0; i < 256; i++) for (int j = g_bitcount[i] = 0; j < 8; j++)
			g_bitcount[i] += (i >> j) & 1;
	}
};
static ComputeBitCount now;

int bin2ascii(const unsigned char* pin, int sz, unsigned char* pout)
{
	int a0, a1, a2, i, j, nout, chr[3];
	for (i = nout = 0; i < sz; i += 3, nout += 4) {
		a0 = pin[i]; j = isneg(i + 1 - sz); a1 = pin[i + j] & -j; j = isneg(i + 2 - sz); a2 = pin[i + j * 2] & -j;
		chr[0] = a0 >> 2; chr[1] = a0 << 4 & 0x30 | (a1 >> 4) & 0x0F;
		chr[2] = a1 << 2 & 0x3C | a2 >> 6 & 0x03; chr[3] = a2 & 0x03F;
		for (j = 0; j < 4; j++)
			*pout++ = mapsymb(chr[j], g_rngb2a, 5);
	}
	return nout;
}
int ascii2bin(const unsigned char* pin, int sz, unsigned char* pout)
{
	int a0, a1, a2, a3, i, nout;
	for (i = nout = 0; i < sz; i += 4, nout += 3) {
		a0 = mapsymb(pin[i + 0], g_rnga2b, 5); a1 = mapsymb(pin[i + 1], g_rnga2b, 5);
		a2 = mapsymb(pin[i + 2], g_rnga2b, 5); a3 = mapsymb(pin[i + 3], g_rnga2b, 5);
		*pout++ = a0 << 2 | a1 >> 4; *pout++ = a1 << 4 & 0xF0 | a2 >> 2 & 0x0F; *pout++ = a2 << 6 & 0xC0 | a3;
	}
	return nout;
}


int CoverPolygonWithCircles(strided_pointer<vector2df> pt, int npt, bool bConsecutive, const vector2df& center,
	vector2df*& centers, float*& radii, float minCircleRadius)
{
	intptr_t imask;
	int i, nCircles = 0, nSkipped;
	vector2df pts[3], bisector;
	float r, area, len2;
	ptitem2d* pdata, * plist, * pvtx, * pvtx_max, * pvtx_left, * pvtx_right;

	static vector2df g_centers[32];
	static float g_radii[32];
	centers = g_centers;
	radii = g_radii;
	if (npt < 2)
		return 0;
	pdata = plist = new ptitem2d[npt];
	for (i = 0, r = 0; i < npt; i++) {
		pdata[i].pt = pt[i] - center;
		pdata[i].next = pdata + (i + 1 & i + 1 - npt >> 31);
		pdata[i].prev = pdata + i - 1 + (npt & i - 1 >> 31);
		r = max(r, pdata[i].pt.len2());
	}
	if (r < sqr(minCircleRadius)) {
		g_centers[0] = center;
		g_radii[0] = sqrt_tpl(r);
		return 1;
	}
	if (!bConsecutive) {
		edgeitem* pcontour = new edgeitem[npt], * pedge;
		if (qhull2d(pdata, npt, pcontour)) {
			plist = pvtx = (pedge = pcontour)->pvtx; npt = 0;
			do {
				pvtx->next = pedge->next->pvtx;
				pvtx->prev = pedge->prev->pvtx;
				pvtx = pvtx->next; npt++;
			} while ((pedge = pedge->next) != pcontour);
		}
		delete[] pcontour;
	}
	for (i = 0, area = 0, pvtx = plist; i < npt; i++, pvtx = pvtx->next)
		area += pvtx->pt ^ pvtx->next->pt;
	if (fabs_tpl(area * 0.5f - r * pi) < area * 0.4f) {
		// one circle fits the figure quite ok
		g_centers[0] = center;
		g_radii[0] = sqrt_tpl(r);
		return 1;
	}

	do {
		pvtx = pvtx_max = plist; do { // find the farthest from the center vertex
			imask = -isneg(pvtx_max->pt.len2() - pvtx->pt.len2());
			pvtx_max = (ptitem2d*)((intptr_t)pvtx_max & ~imask | (intptr_t)pvtx & imask);
		} while ((pvtx = pvtx->next) != plist);
		len2 = pvtx_max->pt.len2();

		// find the farthest from the center vertex in +30 degrees vicinity of the global maximum
		for (pvtx = (pvtx_left = pvtx_max)->next;
			pvtx != pvtx_max && sqr(pvtx->pt ^ pvtx_max->pt) < 0.25f * pvtx->pt.len2() * len2 && pvtx->pt * pvtx_max->pt > 0; pvtx = pvtx->next)
		{
			imask = -((intptr_t)isneg(pvtx_left->pt.len2() - pvtx->pt.len2()) | iszero((intptr_t)pvtx_left - (intptr_t)pvtx_max));
			pvtx_left = (ptitem2d*)((intptr_t)pvtx_left & ~imask | (intptr_t)pvtx & imask);
		}
		// find the farthest from the center vertex in -30 degrees vicinity of the global maximum
		for (pvtx = (pvtx_right = pvtx_max)->prev;
			pvtx != pvtx_max && sqr(pvtx->pt ^ pvtx_max->pt) < 0.25f * pvtx->pt.len2() * len2 && pvtx->pt * pvtx_max->pt > 0; pvtx = pvtx->prev)
		{
			imask = -((intptr_t)isneg(pvtx_right->pt.len2() - pvtx->pt.len2()) | iszero((intptr_t)pvtx_right - (intptr_t)pvtx_max));
			pvtx_right = (ptitem2d*)((intptr_t)pvtx_right & ~imask | (intptr_t)pvtx & imask);
		}

		// find a circle w/ center on left-right bisector that covers all 3 max vertices
		bisector = (pvtx_left->pt + pvtx_right->pt).normalized();
		pts[0] = pvtx_left->pt; pts[1] = pvtx_right->pt; pts[2] = pvtx_max->pt;
		for (i = 0, r = 0; i < 3; i++) {
			float x = bisector * pts[i];
			r = max(r, (sqr(x) + sqr(bisector ^ pts[i])) / (2 * x));
		}
		g_centers[nCircles] = center + bisector * r;
		g_radii[nCircles++] = r;

		// remove all vertices that lie inside (or close enough to) this new circle
		for (i = nSkipped = 0, pvtx = plist; i < npt; i++, pvtx = pvtx->next)
			if ((pvtx->pt + center - g_centers[nCircles - 1]).len2() < r * 1.1f) {
				pvtx->next->prev = pvtx->prev; pvtx->prev->next = pvtx->next;
				nSkipped++;
				if (pvtx == plist) {
					if (pvtx->prev != pvtx)
						plist = pvtx->prev;
					else
						goto allcircles;
				}
			}
		npt -= nSkipped;
	} while (nSkipped && nCircles < sizeof(g_centers) / sizeof(g_centers[0]));

allcircles:
	delete[] pdata;
	return nCircles;
}


void WritePacked(CStream& stm, int num)
{
	int i; for (i = 0; i < 16 && (unsigned int)num >= 1u << i * 2; i++);
	stm.WriteNumberInBits(i, 5);
	if (i > 0)
		stm.WriteNumberInBits(num, i * 2);
}
void WritePacked(CStream& stm, uint64 num)
{
	WritePacked(stm, (int)(num & 0xFFFFFFFF));
	WritePacked(stm, (int)(num >> 32 & 0xFFFFFFFF));
}

void ReadPacked(CStream& stm, int& num)
{
	int i; num = 0;
	stm.ReadNumberInBits(i, 5);
	if (i > 16)
		CryError("ReadPacked i==%i", i);
	if (i > 0)
		stm.ReadNumberInBits(num, i * 2);
}
void ReadPacked(CStream& stm, uint64& num)
{
	int ilo, ihi;
	ReadPacked(stm, ilo);
	ReadPacked(stm, ihi);
	num = (uint64)(unsigned int)ihi << 32 | (uint64)(unsigned int)ilo;
}

void WriteCompressedPos(CStream& stm, const vectorf& pos, bool bCompress)
{
	if (!inrange(pos.x, 0.0f, 2048.0f) || !inrange(pos.y, 0.0f, 2048.0f) || !inrange(pos.z, 0.0f, 512.0f) || !bCompress) {
		stm.Write(false);
		stm.Write(pos);
	}
	else {
		stm.Write(true);
		stm.WriteNumberInBits(float2int(pos.x * 512.0f), 20);
		stm.WriteNumberInBits(float2int(pos.y * 512.0f), 20);
		stm.WriteNumberInBits(float2int(pos.z * 512.0f), 18);
	}
	//stm.WritePkd(CStreamData_WorldPos(const_cast<vectorf&>(pos)));
}
void ReadCompressedPos(CStream& stm, vectorf& pos, bool& bWasCompressed)
{
	stm.Read(bWasCompressed);
	if (bWasCompressed) {
		unsigned int ix, iy, iz;
		stm.ReadNumberInBits(ix, 20); pos.x = ix * (1.0f / 512);
		stm.ReadNumberInBits(iy, 20); pos.y = iy * (1.0f / 512);
		stm.ReadNumberInBits(iz, 18); pos.z = iz * (1.0f / 512);
	}
	else
		stm.Read(pos);
	//stm.ReadPkd(CStreamData_WorldPos(pos));
}
vectorf CompressPos(const vectorf& pos)
{
	if (!inrange(pos.x, 0.0f, 2048.0f) || !inrange(pos.y, 0.0f, 2048.0f) || !inrange(pos.z, 0.0f, 512.0f))
		return pos;
	return vectorf(float2int(pos.x * 512.0f) * (1.0f / 512), float2int(pos.y * 512.0f) * (1.0f / 512), float2int(pos.z * 512.0f) * (1.0f / 512));
	//return CStreamData_WorldPos(const_cast<vectorf&>(pos)).GetCompressed();
}

bool getCompressedQuat(const quaternionf& q, Vec3_tpl<short>& res)
{
	vectorf angles = Ang3::GetAnglesXYZ(matrix3x3f(q));
	bool bGimbalLocked;
	if (bGimbalLocked = fabs_tpl(angles.y) > pi * 0.5f - 0.03f)
		angles = Ang3::GetAnglesXYZ(matrix3x3f(q * GetRotationAA((float)pi / 6, vectorf(0, 1, 0))));
	res.x = max(-32768, min(32767, float2int(angles.x * (32767 / pi))));
	res.y = max(-32768, min(32767, float2int(angles.y * (32767 / (pi * 0.5f)))));
	res.z = max(-32768, min(32767, float2int(angles.z * (32767 / pi))));
	return bGimbalLocked;
}
void WriteCompressedQuat(CStream& stm, const quaternionf& q)
{
	Vec3_tpl<short> sangles;
	stm.Write(getCompressedQuat(q, sangles));
	stm.Write(sangles.x); stm.Write(sangles.y); stm.Write(sangles.z);
}
void ReadCompressedQuat(CStream& stm, quaternionf& q)
{
	bool bGimbalLocked; stm.Read(bGimbalLocked);
	Vec3_tpl<short> sangles; stm.Read(sangles.x); stm.Read(sangles.y); stm.Read(sangles.z);
	q = Quat::GetRotationXYZ(vectorf(sangles.x * (pi / 32767), sangles.y * (pi * 0.5f / 32767), sangles.z * (pi / 32767)));
	if (bGimbalLocked)
		q *= GetRotationAA((float)-pi / 6, vectorf(0, 1, 0));
}
quaternionf CompressQuat(const quaternionf& q)
{
	Vec3_tpl<short> sangles;
	bool bGimbalLocked = getCompressedQuat(q, sangles);
	quaternionf qres = Quat::GetRotationXYZ(vectorf(sangles.x * (pi / 32767), sangles.y * (pi * 0.5f / 32767), sangles.z * (pi / 32767)));
	if (bGimbalLocked)
		qres *= GetRotationAA((float)-pi / 6, vectorf(0, 1, 0));
	return qres;
}
