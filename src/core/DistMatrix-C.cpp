/*
   Copyright (c) 2009-2014, Jack Poulson
   All rights reserved.
   Copyright (c) 2014, Jed Brown
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "El-lite.hpp"
#include EL_COPY_INC 

#include "El-C.h"
using namespace El;

#define RC(TYPE,INPUT) reinterpret_cast<TYPE>(INPUT)

#define RCG(grid) RC(Grid*,grid)
#define RCG_const(grid) RC(const Grid*,grid)

#define RCADM_s(A) RC(AbstractDistMatrix<float          >*,A)
#define RCADM_d(A) RC(AbstractDistMatrix<double         >*,A)
#define RCADM_c(A) RC(AbstractDistMatrix<Complex<float >>*,A)
#define RCADM_z(A) RC(AbstractDistMatrix<Complex<double>>*,A)

#define RCADM_s_const(A) RC(const AbstractDistMatrix<float          >*,A)
#define RCADM_d_const(A) RC(const AbstractDistMatrix<double         >*,A)
#define RCADM_c_const(A) RC(const AbstractDistMatrix<Complex<float >>*,A)
#define RCADM_z_const(A) RC(const AbstractDistMatrix<Complex<double>>*,A)

#define RCB_c(buffer) RC(Complex<float>*,buffer)
#define RCB_z(buffer) RC(Complex<double>*,buffer)

#define RCB_c_const(buffer) RC(const Complex<float >*,buffer)
#define RCB_z_const(buffer) RC(const Complex<double>*,buffer)

#define CATCH \
  catch( std::bad_alloc& e ) \
  { ReportException(e); return EL_ALLOC_ERROR; } \
  catch( std::logic_error& e ) \
  { ReportException(e); return EL_LOGIC_ERROR; } \
  catch( std::runtime_error& e ) \
  { ReportException(e); return EL_RUNTIME_ERROR; } \
  catch( std::exception& e ) \
  { ReportException(e); return EL_ERROR; }

extern "C" {

// DistMatrix<T,MC,MR>::DistMatrix( const Grid& g )
// ------------------------------------------------
ElError ElDistMatrixCreate_s( ElConstGrid gridHandle, ElDistMatrix_s* AHandle )
{
    try 
    {
        const Grid* grid = RCG_const(gridHandle);
        *AHandle = (ElDistMatrix_s)RC(struct ElDistMatrix_sDummy*,
                                      new DistMatrix<float>(*grid));
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCreate_d( ElConstGrid gridHandle, ElDistMatrix_d* AHandle )
{
    try 
    {
        const Grid* grid = RCG_const(gridHandle);
        *AHandle = (ElDistMatrix_d)RC(struct ElDistMatrix_dDummy*,
                                      new DistMatrix<double>(*grid));
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCreate_c( ElConstGrid gridHandle, ElDistMatrix_c* AHandle )
{
    try 
    {
        const Grid* grid = RCG_const(gridHandle);
        *AHandle = (ElDistMatrix_c)RC(struct ElDistMatrix_cDummy*,
                                      new DistMatrix<Complex<float>>(*grid));
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCreate_z( ElConstGrid gridHandle, ElDistMatrix_z* AHandle )
{
    try 
    {
        const Grid* grid = RCG_const(gridHandle);
        *AHandle = (ElDistMatrix_z)RC(struct ElDistMatrix_zDummy*,
                                      new DistMatrix<Complex<double>>(*grid));
    }
    CATCH
    return EL_SUCCESS;
}

// DistMatrix<T,U,V>::DistMatrix( const Grid& g )
// ----------------------------------------------
ElError ElDistMatrixCreateSpecific_s
( ElDist U_C, ElDist V_C, ElConstGrid gridHandle, ElDistMatrix_s* AHandle )
{
    try 
    {
        Dist U = static_cast<Dist>(U_C);
        Dist V = static_cast<Dist>(V_C);
        const Grid* grid = RCG_const(gridHandle);

        AbstractDistMatrix<float>* ADM;
        if( U == CIRC && V == CIRC )
            ADM = new DistMatrix<float,CIRC,CIRC>(*grid); 
        else if( U == MC && V == MR )
            ADM = new DistMatrix<float,MC,MR>(*grid);
        else if( U == MC && V == STAR )
            ADM = new DistMatrix<float,MC,STAR>(*grid);
        else if( U == MD && V == STAR )
            ADM = new DistMatrix<float,MD,STAR>(*grid);
        else if( U == MR && V == MC )
            ADM = new DistMatrix<float,MR,MC>(*grid);
        else if( U == MR && V == STAR )
            ADM = new DistMatrix<float,MR,STAR>(*grid);
        else if( U == STAR && V == MC )
            ADM = new DistMatrix<float,STAR,MC>(*grid);
        else if( U == STAR && V == MD )
            ADM = new DistMatrix<float,STAR,MD>(*grid);
        else if( U == STAR && V == MR )
            ADM = new DistMatrix<float,STAR,MR>(*grid);
        else if( U == STAR && V == STAR )
            ADM = new DistMatrix<float,STAR,STAR>(*grid);
        else if( U == STAR && V == VC )
            ADM = new DistMatrix<float,STAR,VC>(*grid);
        else if( U == STAR && V == VR )
            ADM = new DistMatrix<float,STAR,VR>(*grid);
        else if( U == VC && V == STAR )
            ADM = new DistMatrix<float,VC,STAR>(*grid);
        else if( U == VR && V == STAR )
            ADM = new DistMatrix<float,VR,STAR>(*grid);
        else
            RuntimeError("Invalid distribution pair");

        *AHandle = (ElDistMatrix_s)RC(struct ElDistMatrix_sDummy*,ADM);
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCreateSpecific_d
( ElDist U_C, ElDist V_C, ElConstGrid gridHandle, ElDistMatrix_d* AHandle )
{
    try 
    {
        Dist U = static_cast<Dist>(U_C);
        Dist V = static_cast<Dist>(V_C);
        const Grid* grid = RCG_const(gridHandle);

        AbstractDistMatrix<double>* ADM;
        if( U == CIRC && V == CIRC )
            ADM = new DistMatrix<double,CIRC,CIRC>(*grid);
        else if( U == MC && V == MR )
            ADM = new DistMatrix<double,MC,MR>(*grid);
        else if( U == MC && V == STAR )
            ADM = new DistMatrix<double,MC,STAR>(*grid);
        else if( U == MD && V == STAR )
            ADM = new DistMatrix<double,MD,STAR>(*grid);
        else if( U == MR && V == MC )
            ADM = new DistMatrix<double,MR,MC>(*grid);
        else if( U == MR && V == STAR )
            ADM = new DistMatrix<double,MR,STAR>(*grid);
        else if( U == STAR && V == MC )
            ADM = new DistMatrix<double,STAR,MC>(*grid);
        else if( U == STAR && V == MD )
            ADM = new DistMatrix<double,STAR,MD>(*grid);
        else if( U == STAR && V == MR )
            ADM = new DistMatrix<double,STAR,MR>(*grid);
        else if( U == STAR && V == STAR )
            ADM = new DistMatrix<double,STAR,STAR>(*grid);
        else if( U == STAR && V == VC )
            ADM = new DistMatrix<double,STAR,VC>(*grid);
        else if( U == STAR && V == VR )
            ADM = new DistMatrix<double,STAR,VR>(*grid);
        else if( U == VC && V == STAR )
            ADM = new DistMatrix<double,VC,STAR>(*grid);
        else if( U == VR && V == STAR )
            ADM = new DistMatrix<double,VR,STAR>(*grid);
        else
            RuntimeError("Invalid distribution pair");

        *AHandle = (ElDistMatrix_d)RC(struct ElDistMatrix_dDummy*,ADM);
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCreateSpecific_c
( ElDist U_C, ElDist V_C, ElConstGrid gridHandle, ElDistMatrix_c* AHandle )
{
    try 
    {
        Dist U = static_cast<Dist>(U_C);
        Dist V = static_cast<Dist>(V_C);
        const Grid* grid = RCG_const(gridHandle);

        AbstractDistMatrix<Complex<float>>* ADM;
        if( U == CIRC && V == CIRC )
            ADM = new DistMatrix<Complex<float>,CIRC,CIRC>(*grid);
        else if( U == MC && V == MR )
            ADM = new DistMatrix<Complex<float>,MC,MR>(*grid);
        else if( U == MC && V == STAR )
            ADM = new DistMatrix<Complex<float>,MC,STAR>(*grid);
        else if( U == MD && V == STAR )
            ADM = new DistMatrix<Complex<float>,MD,STAR>(*grid);
        else if( U == MR && V == MC )
            ADM = new DistMatrix<Complex<float>,MR,MC>(*grid);
        else if( U == MR && V == STAR )
            ADM = new DistMatrix<Complex<float>,MR,STAR>(*grid);
        else if( U == STAR && V == MC )
            ADM = new DistMatrix<Complex<float>,STAR,MC>(*grid);
        else if( U == STAR && V == MD )
            ADM = new DistMatrix<Complex<float>,STAR,MD>(*grid);
        else if( U == STAR && V == MR )
            ADM = new DistMatrix<Complex<float>,STAR,MR>(*grid);
        else if( U == STAR && V == STAR )
            ADM = new DistMatrix<Complex<float>,STAR,STAR>(*grid);
        else if( U == STAR && V == VC )
            ADM = new DistMatrix<Complex<float>,STAR,VC>(*grid);
        else if( U == STAR && V == VR )
            ADM = new DistMatrix<Complex<float>,STAR,VR>(*grid);
        else if( U == VC && V == STAR )
            ADM = new DistMatrix<Complex<float>,VC,STAR>(*grid);
        else if( U == VR && V == STAR )
            ADM = new DistMatrix<Complex<float>,VR,STAR>(*grid);
        else
            RuntimeError("Invalid distribution pair");

        *AHandle = (ElDistMatrix_c)RC(struct ElDistMatrix_cDummy*,ADM);
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCreateSpecific_z
( ElDist U_C, ElDist V_C, ElConstGrid gridHandle, ElDistMatrix_z* AHandle )
{
    try 
    {
        Dist U = static_cast<Dist>(U_C);
        Dist V = static_cast<Dist>(V_C);
        const Grid* grid = RCG_const(gridHandle);

        AbstractDistMatrix<Complex<double>>* ADM;
        if( U == CIRC && V == CIRC )
            ADM = new DistMatrix<Complex<double>,CIRC,CIRC>(*grid);
        else if( U == MC && V == MR ) 
            ADM = new DistMatrix<Complex<double>,MC,MR>(*grid);
        else if( U == MC && V == STAR )
            ADM = new DistMatrix<Complex<double>,MC,STAR>(*grid);
        else if( U == MD && V == STAR )
            ADM = new DistMatrix<Complex<double>,MD,STAR>(*grid);
        else if( U == MR && V == MC )
            ADM = new DistMatrix<Complex<double>,MR,MC>(*grid);
        else if( U == MR && V == STAR )
            ADM = new DistMatrix<Complex<double>,MR,STAR>(*grid);
        else if( U == STAR && V == MC )
            ADM = new DistMatrix<Complex<double>,STAR,MC>(*grid);
        else if( U == STAR && V == MD )
            ADM = new DistMatrix<Complex<double>,STAR,MD>(*grid);
        else if( U == STAR && V == MR )
            ADM = new DistMatrix<Complex<double>,STAR,MR>(*grid);
        else if( U == STAR && V == STAR )
            ADM = new DistMatrix<Complex<double>,STAR,STAR>(*grid);
        else if( U == STAR && V == VC )
            ADM = new DistMatrix<Complex<double>,STAR,VC>(*grid);
        else if( U == STAR && V == VR )
            ADM = new DistMatrix<Complex<double>,STAR,VR>(*grid);
        else if( U == VC && V == STAR )
            ADM = new DistMatrix<Complex<double>,VC,STAR>(*grid);
        else if( U == VR && V == STAR )
            ADM = new DistMatrix<Complex<double>,VR,STAR>(*grid);
        else
            RuntimeError("Invalid distribution pair");

        *AHandle = (ElDistMatrix_z)RC(struct ElDistMatrix_zDummy*,ADM);
    }
    CATCH
    return EL_SUCCESS;
}

// DistMatrix<T,U,V>::~DistMatrix()
// --------------------------------
ElError ElDistMatrixDestroy_s( ElConstDistMatrix_s AHandle )
{ 
    try { delete RCADM_s_const(AHandle); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixDestroy_d( ElConstDistMatrix_d AHandle )
{ 
    try { delete RCADM_d_const(AHandle); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixDestroy_c( ElConstDistMatrix_c AHandle )
{ 
    try { delete RCADM_c_const(AHandle); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixDestroy_z( ElConstDistMatrix_z AHandle )
{ 
    try { delete RCADM_z_const(AHandle); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::Empty()
// -------------------------------
ElError ElDistMatrixEmpty_s( ElDistMatrix_s AHandle )
{
    try { RCADM_s(AHandle)->Empty(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixEmpty_d( ElDistMatrix_d AHandle )
{
    try { RCADM_d(AHandle)->Empty(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixEmpty_c( ElDistMatrix_c AHandle )
{
    try { RCADM_c(AHandle)->Empty(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixEmpty_z( ElDistMatrix_z AHandle )
{
    try { RCADM_z(AHandle)->Empty(); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::EmptyData()
// -----------------------------------
ElError ElDistMatrixEmptyData_s( ElDistMatrix_s AHandle )
{
    try { RCADM_s(AHandle)->EmptyData(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixEmptyData_d( ElDistMatrix_d AHandle )
{
    try { RCADM_d(AHandle)->EmptyData(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixEmptyData_c( ElDistMatrix_c AHandle )
{
    try { RCADM_c(AHandle)->EmptyData(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixEmptyData_z( ElDistMatrix_z AHandle )
{
    try { RCADM_z(AHandle)->EmptyData(); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::SetGrid( const Grid& g )
// ------------------------------------------------
ElError ElDistMatrixSetGrid_s( ElDistMatrix_s AHandle, ElConstGrid gridHandle )
{
    try { RCADM_s(AHandle)->SetGrid(*RCG_const(gridHandle)); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSetGrid_d( ElDistMatrix_d AHandle, ElConstGrid gridHandle )
{
    try { RCADM_d(AHandle)->SetGrid(*RCG_const(gridHandle)); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSetGrid_c( ElDistMatrix_c AHandle, ElConstGrid gridHandle )
{
    try { RCADM_c(AHandle)->SetGrid(*RCG_const(gridHandle)); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSetGrid_z( ElDistMatrix_z AHandle, ElConstGrid gridHandle )
{
    try { RCADM_z(AHandle)->SetGrid(*RCG_const(gridHandle)); }
    CATCH
    return EL_SUCCESS;
}

// B = A
// -----
ElError ElDistMatrixCopy_s
( ElConstDistMatrix_s AHandle, ElDistMatrix_s BHandle )
{ 
    try { Copy( *RCADM_s_const(AHandle), *RCADM_s(BHandle) ); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCopy_d
( ElConstDistMatrix_d AHandle, ElDistMatrix_d BHandle )
{
    try { Copy( *RCADM_d_const(AHandle), *RCADM_d(BHandle) ); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCopy_c
( ElConstDistMatrix_c AHandle, ElDistMatrix_c BHandle )
{
    try { Copy( *RCADM_c_const(AHandle), *RCADM_c(BHandle) ); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixCopy_z
( ElConstDistMatrix_z AHandle, ElDistMatrix_z BHandle )
{
    try { Copy( *RCADM_z_const(AHandle), *RCADM_z(BHandle) ); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::Resize( Int height, Int width )
// -------------------------------------------------------
ElError ElDistMatrixResize_s
( ElDistMatrix_s AHandle, ElInt height, ElInt width )
{
    try { RCADM_s(AHandle)->Resize(height,width); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixResize_d
( ElDistMatrix_d AHandle, ElInt height, ElInt width )
{
    try { RCADM_d(AHandle)->Resize(height,width); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixResize_c
( ElDistMatrix_c AHandle, ElInt height, ElInt width )
{
    try { RCADM_c(AHandle)->Resize(height,width); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixResize_z
( ElDistMatrix_z AHandle, ElInt height, ElInt width )
{
    try { RCADM_z(AHandle)->Resize(height,width); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::Resize( Int height, Int width, Int ldim )
// -----------------------------------------------------------------
ElError ElDistMatrixResizeWithLDim_s
( ElDistMatrix_s AHandle, ElInt height, ElInt width, ElInt ldim )
{
    try { RCADM_s(AHandle)->Resize(height,width,ldim); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixResizeWithLDim_d
( ElDistMatrix_d AHandle, ElInt height, ElInt width, ElInt ldim )
{
    try { RCADM_d(AHandle)->Resize(height,width,ldim); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixResizeWithLDim_c
( ElDistMatrix_c AHandle, ElInt height, ElInt width, ElInt ldim )
{
    try { RCADM_c(AHandle)->Resize(height,width,ldim); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixResizeWithLDim_z
( ElDistMatrix_z AHandle, ElInt height, ElInt width, ElInt ldim )
{
    try { RCADM_z(AHandle)->Resize(height,width,ldim); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::MakeConsistent()
// ----------------------------------------
ElError ElDistMatrixMakeConsistent_s
( ElDistMatrix_s AHandle, bool includeViewers )
{
    try { RCADM_s(AHandle)->MakeConsistent(includeViewers); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMakeConsistent_d
( ElDistMatrix_d AHandle, bool includeViewers )
{
    try { RCADM_d(AHandle)->MakeConsistent(includeViewers); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMakeConsistent_c
( ElDistMatrix_c AHandle, bool includeViewers )
{
    try { RCADM_c(AHandle)->MakeConsistent(includeViewers); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMakeConsistent_z
( ElDistMatrix_z AHandle, bool includeViewers )
{
    try { RCADM_z(AHandle)->MakeConsistent(includeViewers); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::MakeSizeConsistent()
// --------------------------------------------
ElError ElDistMatrixMakeSizeConsistent_s
( ElDistMatrix_s AHandle, bool includeViewers )
{
    try { RCADM_s(AHandle)->MakeSizeConsistent(includeViewers); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMakeSizeConsistent_d
( ElDistMatrix_d AHandle, bool includeViewers )
{
    try { RCADM_d(AHandle)->MakeSizeConsistent(includeViewers); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMakeSizeConsistent_c
( ElDistMatrix_c AHandle, bool includeViewers )
{
    try { RCADM_c(AHandle)->MakeSizeConsistent(includeViewers); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMakeSizeConsistent_z
( ElDistMatrix_z AHandle, bool includeViewers )
{
    try { RCADM_z(AHandle)->MakeSizeConsistent(includeViewers); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::Align( Int colAlign, Int rowAlign, bool constrain )
// ---------------------------------------------------------------------------
ElError ElDistMatrixAlign_s
( ElDistMatrix_s AHandle, ElInt colAlign, ElInt rowAlign, bool constrain )
{
    try { RCADM_s(AHandle)->Align(colAlign,rowAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlign_d
( ElDistMatrix_d AHandle, ElInt colAlign, ElInt rowAlign, bool constrain )
{
    try { RCADM_d(AHandle)->Align(colAlign,rowAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlign_c
( ElDistMatrix_c AHandle, ElInt colAlign, ElInt rowAlign, bool constrain )
{
    try { RCADM_c(AHandle)->Align(colAlign,rowAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlign_z
( ElDistMatrix_z AHandle, ElInt colAlign, ElInt rowAlign, bool constrain )
{
    try { RCADM_z(AHandle)->Align(colAlign,rowAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::AlignCols( Int colAlign, bool constrain )
// -----------------------------------------------------------------
ElError ElDistMatrixAlignCols_s
( ElDistMatrix_s AHandle, ElInt colAlign, bool constrain )
{
    try { RCADM_s(AHandle)->AlignCols(colAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlignCols_d
( ElDistMatrix_d AHandle, ElInt colAlign, bool constrain )
{
    try { RCADM_d(AHandle)->AlignCols(colAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlignCols_c
( ElDistMatrix_c AHandle, ElInt colAlign, bool constrain )
{
    try { RCADM_c(AHandle)->AlignCols(colAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlignCols_z
( ElDistMatrix_z AHandle, ElInt colAlign, bool constrain )
{
    try { RCADM_z(AHandle)->AlignCols(colAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::AlignRows( Int rowAlign, bool constrain )
// -----------------------------------------------------------------
ElError ElDistMatrixAlignRows_s
( ElDistMatrix_s AHandle, ElInt rowAlign, bool constrain )
{
    try { RCADM_s(AHandle)->AlignRows(rowAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlignRows_d
( ElDistMatrix_d AHandle, ElInt rowAlign, bool constrain )
{
    try { RCADM_d(AHandle)->AlignRows(rowAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlignRows_c
( ElDistMatrix_c AHandle, ElInt rowAlign, bool constrain )
{
    try { RCADM_c(AHandle)->AlignRows(rowAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAlignRows_z
( ElDistMatrix_z AHandle, ElInt rowAlign, bool constrain )
{
    try { RCADM_z(AHandle)->AlignRows(rowAlign,constrain); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::FreeAlignments()
// ----------------------------------------
ElError ElDistMatrixFreeAlignments_s( ElDistMatrix_s AHandle )
{ 
    try { RCADM_s(AHandle)->FreeAlignments(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixFreeAlignments_d( ElDistMatrix_d AHandle )
{ 
    try { RCADM_d(AHandle)->FreeAlignments(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixFreeAlignments_c( ElDistMatrix_c AHandle )
{ 
    try { RCADM_c(AHandle)->FreeAlignments(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixFreeAlignments_z( ElDistMatrix_z AHandle )
{ 
    try { RCADM_z(AHandle)->FreeAlignments(); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::SetRoot( Int root )
// -------------------------------------------
ElError ElDistMatrixSetRoot_s( ElDistMatrix_s AHandle, ElInt root )
{
    try { RCADM_s(AHandle)->SetRoot(root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSetRoot_d( ElDistMatrix_d AHandle, ElInt root )
{
    try { RCADM_d(AHandle)->SetRoot(root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSetRoot_c( ElDistMatrix_c AHandle, ElInt root )
{
    try { RCADM_c(AHandle)->SetRoot(root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSetRoot_z( ElDistMatrix_z AHandle, ElInt root )
{
    try { RCADM_z(AHandle)->SetRoot(root); }
    CATCH
    return EL_SUCCESS;
}

// TODO: Align[Cols,Rows]With. Need a C version of DistData

// TODO: Align[Cols,Rows]AndResize

// void DistMatrix<T,U,V>::Attach
// ( Int height, Int width, const Grid& grid, Int colAlign, Int rowAlign, 
//   T* buffer, Int ldim, Int root )
// ----------------------------------------------------------------------
ElError ElDistMatrixAttach_s
( ElDistMatrix_s AHandle, ElInt height, ElInt width, ElConstGrid gridHandle,
  ElInt colAlign, ElInt rowAlign, float* buffer, ElInt ldim, ElInt root )
{
    try { RCADM_s(AHandle)->Attach
          (height,width,*RCG_const(gridHandle),colAlign,rowAlign,buffer,
           ldim,root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAttach_d
( ElDistMatrix_d AHandle, ElInt height, ElInt width, ElConstGrid gridHandle,
  ElInt colAlign, ElInt rowAlign, double* buffer, ElInt ldim, ElInt root )
{
    try { RCADM_d(AHandle)->Attach
          (height,width,*RCG_const(gridHandle),colAlign,rowAlign,buffer,
           ldim,root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAttach_c
( ElDistMatrix_c AHandle, ElInt height, ElInt width, ElConstGrid gridHandle,
  ElInt colAlign, ElInt rowAlign, complex_float* buffer, ElInt ldim, 
  ElInt root )
{
    try { RCADM_c(AHandle)->Attach
          (height,width,*RCG_const(gridHandle),colAlign,rowAlign,RCB_c(buffer),
           ldim,root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAttach_z
( ElDistMatrix_z AHandle, ElInt height, ElInt width, ElConstGrid gridHandle,
  ElInt colAlign, ElInt rowAlign, complex_double* buffer, ElInt ldim, 
  ElInt root )
{
    try { RCADM_z(AHandle)->Attach
          (height,width,*RCG_const(gridHandle),colAlign,rowAlign,RCB_z(buffer),
           ldim,root); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::LockedAttach
// ( Int height, Int width, const Grid& grid, Int colAlign, Int rowAlign, 
//   const T* buffer, Int ldim, Int root )
// ----------------------------------------------------------------------
ElError ElDistMatrixLockedAttach_s
( ElDistMatrix_s AHandle, ElInt height, ElInt width, ElConstGrid gridHandle,
  ElInt colAlign, ElInt rowAlign, const float* buffer, 
  ElInt ldim, ElInt root )
{
    try { RCADM_s(AHandle)->LockedAttach
          (height,width,*RCG_const(gridHandle),colAlign,rowAlign,buffer,
           ldim,root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedAttach_d
( ElDistMatrix_d AHandle, ElInt height, ElInt width, ElConstGrid gridHandle,
  ElInt colAlign, ElInt rowAlign, const double* buffer, 
  ElInt ldim, ElInt root )
{
    try { RCADM_d(AHandle)->LockedAttach
          (height,width,*RCG_const(gridHandle),colAlign,rowAlign,buffer,
           ldim,root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedAttach_c
( ElDistMatrix_c AHandle, ElInt height, ElInt width, ElConstGrid gridHandle,
  ElInt colAlign, ElInt rowAlign, const complex_float* buffer, 
  ElInt ldim, ElInt root )
{
    try { RCADM_c(AHandle)->LockedAttach
          (height,width,*RCG_const(gridHandle),colAlign,rowAlign,
           RCB_c_const(buffer),ldim,root); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedAttach_z
( ElDistMatrix_z AHandle, ElInt height, ElInt width, ElConstGrid gridHandle,
  ElInt colAlign, ElInt rowAlign, const complex_double* buffer, 
  ElInt ldim, ElInt root )
{
    try { RCADM_z(AHandle)->LockedAttach
          (height,width,*RCG_const(gridHandle),colAlign,rowAlign,
           RCB_z_const(buffer),ldim,root); }
    CATCH
    return EL_SUCCESS;
}

// Int DistMatrix<T,U,V>::Height() const
// -------------------------------------
ElError ElDistMatrixHeight_s( ElConstDistMatrix_s AHandle, ElInt* height )
{ 
    try { *height = RCADM_s_const(AHandle)->Height(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixHeight_d( ElConstDistMatrix_d AHandle, ElInt* height )
{ 
    try { *height = RCADM_d_const(AHandle)->Height(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixHeight_c( ElConstDistMatrix_c AHandle, ElInt* height )
{ 
    try { *height = RCADM_c_const(AHandle)->Height(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixHeight_z( ElConstDistMatrix_z AHandle, ElInt* height )
{ 
    try { *height = RCADM_z_const(AHandle)->Height(); }
    CATCH
    return EL_SUCCESS;
}

// Int DistMatrix<T,U,V>::Width() const
// ------------------------------------
ElError ElDistMatrixWidth_s( ElConstDistMatrix_s AHandle, ElInt* width )
{ 
    try { *width = RCADM_s_const(AHandle)->Width(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixWidth_d( ElConstDistMatrix_d AHandle, ElInt* width )
{ 
    try { *width = RCADM_d_const(AHandle)->Width(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixWidth_c( ElConstDistMatrix_c AHandle, ElInt* width )
{ 
    try { *width = RCADM_c_const(AHandle)->Width(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixWidth_z( ElConstDistMatrix_z AHandle, ElInt* width )
{ 
    try { *width = RCADM_z_const(AHandle)->Width(); }
    CATCH
    return EL_SUCCESS;
}

// Int DistMatrix<T,U,V>::DiagonalLength( Int offset ) const
// ---------------------------------------------------------
ElError ElDistMatrixDiagonalLength_s
( ElConstDistMatrix_s AHandle, ElInt offset, ElInt* length )
{ 
    try { *length = RCADM_s_const(AHandle)->DiagonalLength(offset); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixDiagonalLength_d
( ElConstDistMatrix_d AHandle, ElInt offset, ElInt* length )
{ 
    try { *length = RCADM_d_const(AHandle)->DiagonalLength(offset); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixDiagonalLength_c
( ElConstDistMatrix_c AHandle, ElInt offset, ElInt* length )
{ 
    try { *length = RCADM_c_const(AHandle)->DiagonalLength(offset); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixDiagonalLength_z
( ElConstDistMatrix_z AHandle, ElInt offset, ElInt* length )
{ 
    try { *length = RCADM_z_const(AHandle)->DiagonalLength(offset); }
    CATCH
    return EL_SUCCESS;
}

// bool DistMatrix<T,U,V>::Viewing() const
// ---------------------------------------
ElError ElDistMatrixViewing_s( ElConstDistMatrix_s AHandle, bool* viewing )
{ 
    try { *viewing = RCADM_s_const(AHandle)->Viewing(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixViewing_d( ElConstDistMatrix_d AHandle, bool* viewing )
{ 
    try { *viewing = RCADM_d_const(AHandle)->Viewing(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixViewing_c( ElConstDistMatrix_c AHandle, bool* viewing )
{ 
    try { *viewing = RCADM_c_const(AHandle)->Viewing(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixViewing_z( ElConstDistMatrix_z AHandle, bool* viewing )
{ 
    try { *viewing = RCADM_z_const(AHandle)->Viewing(); }
    CATCH
    return EL_SUCCESS;
}

// bool DistMatrix<T,U,V>::Locked() const
// --------------------------------------
ElError ElDistMatrixLocked_s( ElConstDistMatrix_s AHandle, bool* locked )
{ 
    try { *locked = RCADM_s_const(AHandle)->Locked(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocked_d( ElConstDistMatrix_d AHandle, bool* locked )
{ 
    try { *locked = RCADM_d_const(AHandle)->Locked(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocked_c( ElConstDistMatrix_c AHandle, bool* locked )
{ 
    try { *locked = RCADM_c_const(AHandle)->Locked(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocked_z( ElConstDistMatrix_z AHandle, bool* locked )
{ 
    try { *locked = RCADM_z_const(AHandle)->Locked(); }
    CATCH
    return EL_SUCCESS;
}

// Int DistMatrix<T,U,V>::LocalHeight() const
// ------------------------------------------
ElError ElDistMatrixLocalHeight_s
( ElConstDistMatrix_s AHandle, ElInt* localHeight )
{ 
    try { *localHeight = RCADM_s_const(AHandle)->LocalHeight(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocalHeight_d
( ElConstDistMatrix_d AHandle, ElInt* localHeight )
{ 
    try { *localHeight = RCADM_d_const(AHandle)->LocalHeight(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocalHeight_c
( ElConstDistMatrix_c AHandle, ElInt* localHeight )
{ 
    try { *localHeight = RCADM_c_const(AHandle)->LocalHeight(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocalHeight_z
( ElConstDistMatrix_z AHandle, ElInt* localHeight )
{ 
    try { *localHeight = RCADM_z_const(AHandle)->LocalHeight(); }
    CATCH
    return EL_SUCCESS;
}

// Int DistMatrix<T,U,V>::LocalWidth() const
// -----------------------------------------
ElError ElDistMatrixLocalWidth_s
( ElConstDistMatrix_s AHandle, ElInt* localWidth )
{ 
    try { *localWidth = RCADM_s_const(AHandle)->LocalWidth(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocalWidth_d
( ElConstDistMatrix_d AHandle, ElInt* localWidth )
{ 
    try { *localWidth = RCADM_d_const(AHandle)->LocalWidth(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocalWidth_c
( ElConstDistMatrix_c AHandle, ElInt* localWidth )
{ 
    try { *localWidth = RCADM_c_const(AHandle)->LocalWidth(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLocalWidth_z
( ElConstDistMatrix_z AHandle, ElInt* localWidth )
{ 
    try { *localWidth = RCADM_z_const(AHandle)->LocalWidth(); }
    CATCH
    return EL_SUCCESS;
}

// Int DistMatrix<T,U,V>::LDim() const
// -----------------------------------------
ElError ElDistMatrixLDim_s( ElConstDistMatrix_s AHandle, ElInt* ldim )
{ 
    try { *ldim = RCADM_s_const(AHandle)->LDim(); }
    CATCH
    return EL_SUCCESS; 
}

ElError ElDistMatrixLDim_d( ElConstDistMatrix_d AHandle, ElInt* ldim )
{ 
    try { *ldim = RCADM_d_const(AHandle)->LDim(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLDim_c( ElConstDistMatrix_c AHandle, ElInt* ldim )
{ 
    try { *ldim = RCADM_c_const(AHandle)->LDim(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLDim_z( ElConstDistMatrix_z AHandle, ElInt* ldim )
{ 
    try { *ldim = RCADM_z_const(AHandle)->LDim(); }
    CATCH
    return EL_SUCCESS;
}

// Matrix<T>& DistMatrix<T,U,V>::Matrix()
// --------------------------------------
ElError ElDistMatrixMatrix_s( ElDistMatrix_s AHandle, ElMatrix_s* ALocHandle )
{
    try { *ALocHandle = (ElMatrix_s)reinterpret_cast<struct ElMatrix_sDummy*>
                        (&RCADM_s(AHandle)->Matrix()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMatrix_d( ElDistMatrix_d AHandle, ElMatrix_d* ALocHandle )
{
    try { *ALocHandle = (ElMatrix_d)reinterpret_cast<struct ElMatrix_dDummy*>
                        (&RCADM_d(AHandle)->Matrix()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMatrix_c( ElDistMatrix_c AHandle, ElMatrix_c* ALocHandle )
{
    try { *ALocHandle = (ElMatrix_c)reinterpret_cast<struct ElMatrix_cDummy*>
                        (&RCADM_c(AHandle)->Matrix()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMatrix_z( ElDistMatrix_z AHandle, ElMatrix_z* ALocHandle )
{
    try { *ALocHandle = (ElMatrix_z)reinterpret_cast<struct ElMatrix_zDummy*>
                        (&RCADM_z(AHandle)->Matrix()); }
    CATCH
    return EL_SUCCESS;
}

// const Matrix<T>& DistMatrix<T,U,V>::LockedMatrix() const
// --------------------------------------------------------
ElError ElDistMatrixLockedMatrix_s
( ElConstDistMatrix_s AHandle, ElConstMatrix_s* ALocHandle )
{
    try { *ALocHandle = (ElConstMatrix_s)
                        reinterpret_cast<const struct ElMatrix_sDummy*>
                        (&RCADM_s_const(AHandle)->LockedMatrix()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedMatrix_d
( ElConstDistMatrix_d AHandle, ElConstMatrix_d* ALocHandle )
{
    try { *ALocHandle = (ElConstMatrix_d)
                        reinterpret_cast<const struct ElMatrix_dDummy*>
                        (&RCADM_d_const(AHandle)->LockedMatrix()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedMatrix_c
( ElConstDistMatrix_c AHandle, ElConstMatrix_c* ALocHandle )
{
    try { *ALocHandle = (ElConstMatrix_c)
                        reinterpret_cast<const struct ElMatrix_cDummy*>
                        (&RCADM_c_const(AHandle)->LockedMatrix()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedMatrix_z
( ElConstDistMatrix_z AHandle, ElConstMatrix_z* ALocHandle )
{
    try { *ALocHandle = (ElConstMatrix_z)
                        reinterpret_cast<const struct ElMatrix_zDummy*>
                        (&RCADM_z_const(AHandle)->LockedMatrix()); }
    CATCH
    return EL_SUCCESS;
}

// size_t DistMatrix<T,U,V>::AllocatedMemory() const
// -------------------------------------------------
ElError ElDistMatrixAllocatedMemory_s
( ElConstDistMatrix_s AHandle, size_t* mem )
{ 
    try { *mem = RCADM_s_const(AHandle)->AllocatedMemory(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAllocatedMemory_d
( ElConstDistMatrix_d AHandle, size_t* mem )
{ 
    try { *mem = RCADM_d_const(AHandle)->AllocatedMemory(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAllocatedMemory_c
( ElConstDistMatrix_c AHandle, size_t* mem )
{ 
    try { *mem = RCADM_c_const(AHandle)->AllocatedMemory(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixAllocatedMemory_z
( ElConstDistMatrix_z AHandle, size_t* mem )
{ 
    try { *mem = RCADM_z_const(AHandle)->AllocatedMemory(); }
    CATCH
    return EL_SUCCESS;
}

// T* DistMatrix<T,U,V>::Buffer()
// ------------------------------
ElError ElDistMatrixBuffer_s( ElDistMatrix_s AHandle, float** buffer )
{ 
    try { *buffer = RCADM_s(AHandle)->Buffer(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixBuffer_d( ElDistMatrix_d AHandle, double** buffer )
{ 
    try { *buffer = RCADM_d(AHandle)->Buffer(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixBuffer_c( ElDistMatrix_c AHandle, complex_float** buffer )
{ 
    try { *buffer = (complex_float*)RCADM_c(AHandle)->Buffer(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixBuffer_z( ElDistMatrix_z AHandle, complex_double** buffer )
{ 
    try { *buffer = (complex_double*)RCADM_z(AHandle)->Buffer(); }
    CATCH
    return EL_SUCCESS;
}

// const T* DistMatrix<T,U,V>::LockedBuffer() const
// ------------------------------------------------
ElError ElDistMatrixLockedBuffer_s
( ElConstDistMatrix_s AHandle, const float** buffer )
{ 
    try { *buffer = RCADM_s_const(AHandle)->LockedBuffer(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedBuffer_d
( ElConstDistMatrix_d AHandle, const double** buffer )
{ 
    try { *buffer = RCADM_d_const(AHandle)->LockedBuffer(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedBuffer_c
( ElConstDistMatrix_c AHandle, const complex_float** buffer )
{ 
    try 
    { *buffer = (const complex_float*)RCADM_c_const(AHandle)->LockedBuffer(); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixLockedBuffer_z
( ElConstDistMatrix_z AHandle, const complex_double** buffer )
{ 
    try 
    { *buffer = (const complex_double*)RCADM_z_const(AHandle)->LockedBuffer(); }
    CATCH
    return EL_SUCCESS;
}

// const Grid& DistMatrix<T,U,V>::Grid() const
// -------------------------------------------
ElError ElDistMatrixGrid_s
( ElConstDistMatrix_s AHandle, ElConstGrid* gridHandle )
{ 
    try 
    { *gridHandle = (ElConstGrid)reinterpret_cast<const struct ElGridDummy*>
                    (&RCADM_s_const(AHandle)->Grid()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGrid_d
( ElConstDistMatrix_d AHandle, ElConstGrid* gridHandle )
{ 
    try
    { *gridHandle = (ElConstGrid)reinterpret_cast<const struct ElGridDummy*>
                    (&RCADM_d_const(AHandle)->Grid()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGrid_c
( ElConstDistMatrix_c AHandle, ElConstGrid* gridHandle )
{ 
    try
    { *gridHandle = (ElConstGrid)reinterpret_cast<const struct ElGridDummy*>
                    (&RCADM_c_const(AHandle)->Grid()); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGrid_z
( ElConstDistMatrix_z AHandle, ElConstGrid* gridHandle )
{ 
    try
    { *gridHandle = (ElConstGrid)reinterpret_cast<const struct ElGridDummy*>
                    (&RCADM_z_const(AHandle)->Grid()); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::Get( Int i, Int j ) const
// -------------------------------------------------
ElError ElDistMatrixGet_s
( ElConstDistMatrix_s AHandle, ElInt i, ElInt j, float* val )
{
    try { *val = RCADM_s_const(AHandle)->Get(i,j); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGet_d
( ElConstDistMatrix_d AHandle, ElInt i, ElInt j, double* val )
{
    try { *val = RCADM_d_const(AHandle)->Get(i,j); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGet_c
( ElConstDistMatrix_c AHandle, ElInt i, ElInt j, complex_float* val )
{
    try 
    { 
        Complex<float> alpha = RCADM_c_const(AHandle)->Get(i,j);
        val->real = alpha.real();
        val->imag = alpha.imag();
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGet_z
( ElConstDistMatrix_z AHandle, ElInt i, ElInt j, complex_double* val )
{
    try 
    { 
        Complex<double> alpha = RCADM_z_const(AHandle)->Get(i,j); 
        val->real = alpha.real();
        val->imag = alpha.imag();
    }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::GetRealPart( Int i, Int j ) const
// ---------------------------------------------------------
ElError ElDistMatrixGetRealPart_c
( ElConstDistMatrix_c AHandle, ElInt i, ElInt j, float* val )
{
    try { *val = RCADM_c_const(AHandle)->GetRealPart(i,j); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGetRealPart_z
( ElConstDistMatrix_z AHandle, ElInt i, ElInt j, double* val )
{
    try { *val = RCADM_c_const(AHandle)->GetRealPart(i,j); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::GetImagPart( Int i, Int j ) const
// ---------------------------------------------------------
ElError ElDistMatrixGetImagPart_c
( ElConstDistMatrix_c AHandle, ElInt i, ElInt j, float* val )
{
    try { *val = RCADM_c_const(AHandle)->GetImagPart(i,j); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGetImagPart_z
( ElConstDistMatrix_z AHandle, ElInt i, ElInt j, double* val )
{
    try { *val = RCADM_c_const(AHandle)->GetImagPart(i,j); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::Set( Int i, Int j, T alpha )
// ----------------------------------------------------
ElError ElDistMatrixSet_s
( ElDistMatrix_s AHandle, ElInt i, ElInt j, float alpha )
{
    try { RCADM_s(AHandle)->Set(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSet_d
( ElDistMatrix_d AHandle, ElInt i, ElInt j, double alpha )
{
    try { RCADM_d(AHandle)->Set(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSet_c
( ElDistMatrix_c AHandle, ElInt i, ElInt j, complex_float alpha )
{
    try { RCADM_c(AHandle)->Set(i,j,Complex<float>(alpha.real,alpha.imag)); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSet_z
( ElDistMatrix_z AHandle, ElInt i, ElInt j, complex_double alpha )
{
    try { RCADM_z(AHandle)->Set(i,j,Complex<double>(alpha.real,alpha.imag)); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::SetRealPart( Int i, Int j, Base<T> alpha )
// ------------------------------------------------------------------
ElError ElDistMatrixSetRealPart_c
( ElDistMatrix_c AHandle, ElInt i, ElInt j, float alpha )
{
    try { RCADM_c(AHandle)->SetRealPart(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSetRealPart_z
( ElDistMatrix_z AHandle, ElInt i, ElInt j, double alpha )
{
    try { RCADM_z(AHandle)->SetRealPart(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::SetImagPart( Int i, Int j, Base<T> alpha )
// ------------------------------------------------------------------
ElError ElDistMatrixSetImagPart_c
( ElDistMatrix_c AHandle, ElInt i, ElInt j, float alpha )
{
    try { RCADM_c(AHandle)->SetImagPart(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixSetImagPart_z
( ElDistMatrix_z AHandle, ElInt i, ElInt j, double alpha )
{
    try { RCADM_z(AHandle)->SetImagPart(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::Update( Int i, Int j, T alpha )
// -------------------------------------------------------
ElError ElDistMatrixUpdate_s
( ElDistMatrix_s AHandle, ElInt i, ElInt j, float alpha )
{
    try { RCADM_s(AHandle)->Update(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixUpdate_d
( ElDistMatrix_d AHandle, ElInt i, ElInt j, double alpha )
{
    try { RCADM_d(AHandle)->Update(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixUpdate_c
( ElDistMatrix_c AHandle, ElInt i, ElInt j, complex_float alpha )
{
    try { RCADM_c(AHandle)->Update
          (i,j,Complex<float>(alpha.real,alpha.imag)); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixUpdate_z
( ElDistMatrix_z AHandle, ElInt i, ElInt j, complex_double alpha )
{
    try { RCADM_z(AHandle)->Update
          (i,j,Complex<double>(alpha.real,alpha.imag)); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::UpdateRealPart( Int i, Int j, Base<T> alpha )
// ---------------------------------------------------------------------
ElError ElDistMatrixUpdateRealPart_c
( ElDistMatrix_c AHandle, ElInt i, ElInt j, float alpha )
{
    try { RCADM_c(AHandle)->UpdateRealPart(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixUpdateRealPart_z
( ElDistMatrix_z AHandle, ElInt i, ElInt j, double alpha )
{
    try { RCADM_z(AHandle)->UpdateRealPart(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::UpdateImagPart( Int i, Int j, Base<T> alpha )
// ---------------------------------------------------------------------
ElError ElDistMatrixUpdateImagPart_c
( ElDistMatrix_c AHandle, ElInt i, ElInt j, float alpha )
{
    try { RCADM_c(AHandle)->UpdateImagPart(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixUpdateImagPart_z
( ElDistMatrix_z AHandle, ElInt i, ElInt j, double alpha )
{
    try { RCADM_z(AHandle)->UpdateImagPart(i,j,alpha); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::MakeReal( Int i, Int j )
// ------------------------------------------------
ElError ElDistMatrixMakeReal_c( ElDistMatrix_c AHandle, ElInt i, ElInt j )
{
    try { RCADM_c(AHandle)->MakeReal(i,j); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixMakeReal_z( ElDistMatrix_z AHandle, ElInt i, ElInt j )
{
    try { RCADM_z(AHandle)->MakeReal(i,j); }
    CATCH
    return EL_SUCCESS;
}

// void DistMatrix<T,U,V>::Conjugate( Int i, Int j )
// -------------------------------------------------
ElError ElDistMatrixConjugate_c( ElDistMatrix_c AHandle, ElInt i, ElInt j )
{
    try { RCADM_c(AHandle)->Conjugate(i,j); }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixConjugate_z( ElDistMatrix_z AHandle, ElInt i, ElInt j )
{
    try { RCADM_z(AHandle)->Conjugate(i,j); }
    CATCH
    return EL_SUCCESS;
}

// DistMatrix<T,UDiag,VDiag> DistMatrix<T,U,V>::GetDiagonal( Int offset ) const
// ----------------------------------------------------------------------------
ElError ElDistMatrixGetDiagonal_s
( ElConstDistMatrix_s AHandle, ElInt offset, ElDistMatrix_s* dHandle )
{
    try 
    {
        auto AAbs = RCADM_s_const(AHandle);
        const Grid& grid = AAbs->Grid();

        const DistData distData = AAbs->DistData();
        const Dist U = distData.colDist;
        const Dist V = distData.rowDist;

        if( U == CIRC && V == CIRC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,CIRC,CIRC>*>(AAbs);
            auto* d = new DistMatrix<float,CIRC,CIRC>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == MC && V == MR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,MC,MR>*>(AAbs);
            auto* d = new DistMatrix<float,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == MC && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,MC,STAR>*>(AAbs);
            auto* d = new DistMatrix<float,MC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == MD && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,MD,STAR>*>(AAbs);
            auto* d = new DistMatrix<float,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == STAR && V == MC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,STAR,MC>*>(AAbs);
            auto* d = new DistMatrix<float,MC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == STAR && V == MD )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,STAR,MD>*>(AAbs);
            auto* d = new DistMatrix<float,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == STAR && V == MR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,STAR,MR>*>(AAbs);
            auto* d = new DistMatrix<float,MR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == STAR && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,STAR,STAR>*>(AAbs);
            auto* d = new DistMatrix<float,STAR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == STAR && V == VC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,STAR,VC>*>(AAbs);
            auto* d = new DistMatrix<float,VC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == STAR && V == VR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,STAR,VR>*>(AAbs);
            auto* d = new DistMatrix<float,VR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == VC && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,VC,STAR>*>(AAbs);
            auto* d = new DistMatrix<float,VC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else if( U == VR && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<float,VR,STAR>*>(AAbs);
            auto* d = new DistMatrix<float,VR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_s)
                       reinterpret_cast<struct ElDistMatrix_sDummy*>(d);
        }
        else
            RuntimeError("Invalid distribution pair");
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGetDiagonal_d
( ElConstDistMatrix_d AHandle, ElInt offset, ElDistMatrix_d* dHandle )
{
    try 
    {
        auto AAbs = RCADM_d_const(AHandle);
        const Grid& grid = AAbs->Grid();

        const DistData distData = AAbs->DistData();
        const Dist U = distData.colDist;
        const Dist V = distData.rowDist;

        if( U == CIRC && V == CIRC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,CIRC,CIRC>*>(AAbs);
            auto* d = new DistMatrix<double,CIRC,CIRC>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == MC && V == MR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,MC,MR>*>(AAbs);
            auto* d = new DistMatrix<double,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == MC && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,MC,STAR>*>(AAbs);
            auto* d = new DistMatrix<double,MC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == MD && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,MD,STAR>*>(AAbs);
            auto* d = new DistMatrix<double,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == STAR && V == MC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,STAR,MC>*>(AAbs);
            auto* d = new DistMatrix<double,MC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == STAR && V == MD )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,STAR,MD>*>(AAbs);
            auto* d = new DistMatrix<double,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == STAR && V == MR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,STAR,MR>*>(AAbs);
            auto* d = new DistMatrix<double,MR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == STAR && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,STAR,STAR>*>(AAbs);
            auto* d = new DistMatrix<double,STAR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == STAR && V == VC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,STAR,VC>*>(AAbs);
            auto* d = new DistMatrix<double,VC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == STAR && V == VR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,STAR,VR>*>(AAbs);
            auto* d = new DistMatrix<double,VR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == VC && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,VC,STAR>*>(AAbs);
            auto* d = new DistMatrix<double,VC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else if( U == VR && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<double,VR,STAR>*>(AAbs);
            auto* d = new DistMatrix<double,VR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_d)
                       reinterpret_cast<struct ElDistMatrix_dDummy*>(d);
        }
        else
            RuntimeError("Invalid distribution pair");
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGetDiagonal_c
( ElConstDistMatrix_c AHandle, ElInt offset, ElDistMatrix_c* dHandle )
{
    try 
    {
        auto AAbs = RCADM_c_const(AHandle);
        const Grid& grid = AAbs->Grid();

        const DistData distData = AAbs->DistData();
        const Dist U = distData.colDist;
        const Dist V = distData.rowDist;

        if( U == CIRC && V == CIRC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,CIRC,CIRC>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,CIRC,CIRC>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == MC && V == MR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,MC,MR>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == MC && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,MC,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,MC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == MD && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,MD,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == STAR && V == MC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,STAR,MC>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,MC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == STAR && V == MD )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,STAR,MD>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == STAR && V == MR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,STAR,MR>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,MR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == STAR && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,STAR,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,STAR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == STAR && V == VC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,STAR,VC>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,VC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == STAR && V == VR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,STAR,VR>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,VR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == VC && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,VC,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,VC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else if( U == VR && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<float>,VR,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<float>,VR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_c)
                       reinterpret_cast<struct ElDistMatrix_cDummy*>(d);
        }
        else
            RuntimeError("Invalid distribution pair");
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGetDiagonal_z
( ElConstDistMatrix_z AHandle, ElInt offset, ElDistMatrix_z* dHandle )
{
    try 
    {
        auto AAbs = RCADM_z_const(AHandle);
        const Grid& grid = AAbs->Grid();

        const DistData distData = AAbs->DistData();
        const Dist U = distData.colDist;
        const Dist V = distData.rowDist;

        if( U == CIRC && V == CIRC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,CIRC,CIRC>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,CIRC,CIRC>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == MC && V == MR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,MC,MR>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == MC && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,MC,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,MC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == MD && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,MD,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == STAR && V == MC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,STAR,MC>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,MC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == STAR && V == MD )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,STAR,MD>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,MD,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == STAR && V == MR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,STAR,MR>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,MR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == STAR && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,STAR,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,STAR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == STAR && V == VC )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,STAR,VC>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,VC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == STAR && V == VR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,STAR,VR>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,VR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == VC && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,VC,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,VC,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else if( U == VR && V == STAR )
        {
            auto A = 
              dynamic_cast<const DistMatrix<Complex<double>,VR,STAR>*>(AAbs);
            auto* d = new DistMatrix<Complex<double>,VR,STAR>(grid);
            A->GetDiagonal( *d, offset );
            *dHandle = (ElDistMatrix_z)
                       reinterpret_cast<struct ElDistMatrix_zDummy*>(d);
        }
        else
            RuntimeError("Invalid distribution pair");
    }
    CATCH
    return EL_SUCCESS;
}

// TODO: More diagonal manipulation
// ================================

// DistMatrix<T,STAR,STAR> DistMatrix<T,U,V>::GetSubmatrix
// ( const std::vector<Int>& rowInds, const std::vector<Int>& colInds ) const
// --------------------------------------------------------------------------
ElError ElDistMatrixGetSubmatrix_s
( ElConstDistMatrix_s AHandle,
  ElInt numRowInds, const ElInt* rowInds,
  ElInt numColInds, const ElInt* colInds, ElDistMatrix_s* ASubHandle )
{
    try
    {
        auto A = RCADM_s_const(AHandle);
        auto ASub = new DistMatrix<float,STAR,STAR>(A->Grid());

        std::vector<Int> rowIndVec(rowInds,rowInds+numRowInds),
                         colIndVec(colInds,colInds+numColInds);
        A->GetSubmatrix( rowIndVec, colIndVec, *ASub );
        *ASubHandle = (ElDistMatrix_s)ASub;
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGetSubmatrix_d
( ElConstDistMatrix_d AHandle,
  ElInt numRowInds, const ElInt* rowInds,
  ElInt numColInds, const ElInt* colInds, ElDistMatrix_d* ASubHandle )
{
    try
    {
        auto A = RCADM_d_const(AHandle);
        auto ASub = new DistMatrix<double,STAR,STAR>(A->Grid());

        std::vector<Int> rowIndVec(rowInds,rowInds+numRowInds),
                         colIndVec(colInds,colInds+numColInds);
        A->GetSubmatrix( rowIndVec, colIndVec, *ASub );
        *ASubHandle = (ElDistMatrix_d)ASub;
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGetSubmatrix_c
( ElConstDistMatrix_c AHandle,
  ElInt numRowInds, const ElInt* rowInds,
  ElInt numColInds, const ElInt* colInds, ElDistMatrix_c* ASubHandle )
{
    try
    {
        auto A = RCADM_c_const(AHandle);
        auto ASub = new DistMatrix<Complex<float>,STAR,STAR>(A->Grid());

        std::vector<Int> rowIndVec(rowInds,rowInds+numRowInds),
                         colIndVec(colInds,colInds+numColInds);
        A->GetSubmatrix( rowIndVec, colIndVec, *ASub );
        *ASubHandle = (ElDistMatrix_c)ASub;
    }
    CATCH
    return EL_SUCCESS;
}

ElError ElDistMatrixGetSubmatrix_z
( ElConstDistMatrix_z AHandle,
  ElInt numRowInds, const ElInt* rowInds,
  ElInt numColInds, const ElInt* colInds, ElDistMatrix_z* ASubHandle )
{
    try
    {
        auto A = RCADM_z_const(AHandle);
        auto ASub = new DistMatrix<Complex<double>,STAR,STAR>(A->Grid());

        std::vector<Int> rowIndVec(rowInds,rowInds+numRowInds),
                         colIndVec(colInds,colInds+numColInds);
        A->GetSubmatrix( rowIndVec, colIndVec, *ASub );
        *ASubHandle = (ElDistMatrix_z)ASub;
    }
    CATCH
    return EL_SUCCESS;
}

/// TODO: Many more member functions

} // extern "C"
