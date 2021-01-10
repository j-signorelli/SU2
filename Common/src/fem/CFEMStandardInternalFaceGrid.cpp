/*!
 * \file CFEMStandardInternalFaceGrid.cpp
 * \brief Functions for the class CFEMStandardInternalFaceGrid.
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

#include "../../include/fem/CFEMStandardInternalFaceGrid.hpp"

/*----------------------------------------------------------------------------------*/
/*            Public member functions of CFEMStandardInternalFaceGrid.              */
/*----------------------------------------------------------------------------------*/

CFEMStandardInternalFaceGrid::CFEMStandardInternalFaceGrid(CFEMStandardElementBase *val_elem0,
		                                           CFEMStandardElementBase *val_elem1) {
  elem0 = val_elem0;
  elem1 = val_elem1;
}

void CFEMStandardInternalFaceGrid::CoorIntPoints(ColMajorMatrix<su2double> &coorGridDOFsVol,
                                                 ColMajorMatrix<su2double> &coorIntPointsFace) {

  /*--- Per definition the polynomial degree of the element on side 0 is higher or equal
        to the polynomial degree of the element on side 1. Hence the element on side 0
        is used to compute the coordinates of the integration points. The first argument
        is a dummy to be consistent with other classes. ---*/
  elem0->CoorIntPoints(true, coorGridDOFsVol, coorIntPointsFace);
}
