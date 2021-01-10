/*!
 * \file CFEMStandardLineAdjacentQuadGrid.cpp
 * \brief Functions for the class CFEMStandardLineAdjacentQuadGrid.
 * \author E. van der Weide
 * \version 7.0.8 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2020, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/fem/CFEMStandardLineAdjacentQuadGrid.hpp"

/*----------------------------------------------------------------------------------*/
/*            Public member functions of CFEMStandardLineAdjacentQuadGrid.          */
/*----------------------------------------------------------------------------------*/

CFEMStandardLineAdjacentQuadGrid::CFEMStandardLineAdjacentQuadGrid(const unsigned short val_nPoly,
                                                                   const unsigned short val_orderExact,
                                                                   const unsigned short val_faceID_Elem,
                                                                   const unsigned short val_orientation,
                                                                   const bool           val_useLGL,
                                                                   CGemmBase           *val_gemm)
  : CFEMStandardQuadBase(),
    CFEMStandardLineBase(val_nPoly, val_orderExact) {

  /*--- Store the faceID of the element and the orientation. ---*/
  faceID_Elem = val_faceID_Elem;
  orientation = val_orientation;

  /*--- Convert the pointer for the gemm functionality. ---*/
  gemmDOFs2Int = dynamic_cast<CGemmFaceQuad *> (val_gemm);
  if( !gemmDOFs2Int )
    SU2_MPI::Error(string("Dynamic cast failure. This should not happen"), CURRENT_FUNCTION);

  /*--- Determine the 1D parametric locations of the grid DOFs. 1D is enough,
        because a tensor product is used to obtain the 2D coordinates. ---*/
  if( val_useLGL) Location1DGridDOFsLGL(nPoly, rLineDOFs);
  else            Location1DGridDOFsEquidistant(nPoly, rLineDOFs);

  /*--- Convert the 1D coordinates of the standard line to the parametric 
        coordinates in normal and tangential direction of the adjacent
        quadrilateral. ---*/
  vector<passivedouble> rNormal, rTangential;
  ConvertCoor1DFaceTo2DQuad(rLineInt, faceID_Elem, orientation, rNormal, rTangential);

  /*--- Create the 1D Lagrangian basis functions for the parametric
        coordinates in normal direction. ---*/
  ColMajorMatrix<passivedouble> lagN, derLagN;

  LagBasisIntPointsLine(rLineDOFs, rNormal, lagN);
  DerLagBasisIntPointsLine(rLineDOFs, rNormal, derLagN);

  /*--- Create the 1D Lagrangian basis functions for the parametric
        coordinates in tangential direction. ---*/
  ColMajorMatrix<passivedouble> lagT, derLagT;

  LagBasisIntPointsLine(rLineDOFs, rTangential, lagT);
  DerLagBasisIntPointsLine(rLineDOFs, rTangential, derLagT);

  /*--- Allocate the memory for the first index of tensorSol, etc. As this
        function is only called for 2D simulations, there are two
        contributions to the tensor products. ---*/
  tensorSol.resize(2);
  tensorDSolDr.resize(2);
  tensorDSolDs.resize(2);

  /*--- Set the components of the tensors. For the derivatives this
        depends on the faceID in the element. ---*/
  tensorSol[0] = lagN;
  tensorSol[1] = lagT;

  if((faceID_Elem == 0) || (faceID_Elem == 2)) {
    tensorDSolDr[0] = lagN;    tensorDSolDr[1] = derLagT;
    tensorDSolDs[0] = derLagN; tensorDSolDs[1] = lagT;
  }
  else {
    tensorDSolDr[0] = derLagN; tensorDSolDr[1] = lagT;
    tensorDSolDs[0] = lagN;    tensorDSolDs[1] = derLagT;
  }
}

void CFEMStandardLineAdjacentQuadGrid::CoorIntPoints(const bool                notUsed,
                                                     ColMajorMatrix<su2double> &matCoorDOF,
                                                     ColMajorMatrix<su2double> &matCoorInt) {

  /*--- Call the general functionality of gemmDOFs2Int with the appropriate
        arguments to compute the coordinates in the integration points
        of the face. ---*/
  gemmDOFs2Int->DOFs2Int(tensorSol, faceID_Elem, 2, matCoorDOF, matCoorInt);
}
