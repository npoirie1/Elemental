/*
   Copyright (c) 2009-2014, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "El-C.h"

#define CHECK_ERROR(error) \
  if( error != EL_SUCCESS ) \
  { \
      const char* errString = ElErrorString(error); \
      printf("Aborting: %s was returned\n",errString); \
      ElFinalize(); \
      return 0; \
  }

int 
main( int argc, char* argv[] )
{
    ElError error = ElInitialize( &argc, &argv );
    /* Manual error check here? */

    ElInt m, n, mSub, nSub;
    bool print, display;
    error = ElInput_I("--m","matrix height",10,&m);
    CHECK_ERROR(error)
    error = ElInput_I("--n","matrix width",10,&n);
    CHECK_ERROR(error)
    error = ElInput_I("--mSub","submatrix height",5,&mSub);
    CHECK_ERROR(error)
    error = ElInput_I("--nSub","submatrix width",5,&nSub);
    CHECK_ERROR(error)
    error = ElInput_b("--print","print matrix?",false,&print);
    CHECK_ERROR(error)
    error = ElInput_b("--display","display matrix?",true,&display);
    error = ElProcessInput();
    CHECK_ERROR(error)
    error = ElPrintInputReport();
    CHECK_ERROR(error)

    if( mSub > m || nSub > n )
    {
        int worldRank;
        MPI_Comm_rank( MPI_COMM_WORLD, &worldRank );
        if( worldRank == 0 )
            printf("Invalid submatrix dimensions\n");
        ElFinalize();
        return 0;
    }

    ElGrid grid;
    error = ElGridCreate( MPI_COMM_WORLD, EL_COLUMN_MAJOR, &grid );
    CHECK_ERROR(error);

    ElDistMatrix_z A;
    error = ElDistMatrixCreateSpecific_z( EL_MR, EL_MC, grid, &A );
    CHECK_ERROR(error)
    error = ElDistMatrixResize_z( A, m, n );
    CHECK_ERROR(error)
    
    ElInt i, j;
    for( j=0; j<n; ++j )
    {
        for( i=0; i<m; ++i )
        {
            error = ElDistMatrixSet_z( A, i, j, i+j );
            CHECK_ERROR(error)
        }
    }

    if( print )
    {
        error = ElPrintDistMatrix_z( A, "A" );
        CHECK_ERROR(error)
    }
    if( display )
    {
        error = ElDisplayDistMatrix_z( A, "A" );
        CHECK_ERROR(error)
    }

    /* Extract an mSub x nSub submatrix */
    ElInt* rowInds = malloc(mSub*sizeof(ElInt));
    ElInt* colInds = malloc(nSub*sizeof(ElInt));
    for( i=0; i<mSub; ++i )
        rowInds[i] = rand() % m;
    for( j=0; j<nSub; ++j )
        colInds[j] = rand() % n;
    ElDistMatrix_z ASub;
    error = ElDistMatrixGetSubmatrix_z
            ( A, mSub, rowInds, nSub, colInds, &ASub );
    CHECK_ERROR(error)
    if( print )
    {
        error = ElPrintDistMatrix_z( ASub, "ASub" );
        CHECK_ERROR(error)
    }
    if( display )
    {
        error = ElDisplayDistMatrix_z( ASub, "ASub" );
        CHECK_ERROR(error)
    }

    error = ElFinalize();
    /* Manual error check here? */
    return 0;
}
