/*
**	Command & Conquer Renegade(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : ww3d                                                         *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/ww3d2/dx8polygonrenderer.h                   $*
 *                                                                                             *
 *              Original Author:: Greg Hjelstrom                                               *
 *                                                                                             *
 *                      $Author:: Greg_h                                                      $*
 *                                                                                             *
 *                     $Modtime:: 2/12/02 2:29p                                               $*
 *                                                                                             *
 *                    $Revision:: 23                                                          $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#if defined(_MSC_VER)
#pragma once
#endif

#ifndef DX8_POLYGON_RENDERER_H
#define DX8_POLYGON_RENDERER_H


#include "always.h"
#include "dx8list.h"
#include "sortingrenderer.h"

class MeshClass;
class DX8PolygonRendererClass;
class DX8TextureCategoryClass;


/**
** DX8PolygonRendererClass
** This is a record of a batch/range of polygons to be rendered.  These hang off of the DX8TextureCategoryClass's
** and are rendered after the system installs a vertex buffer and textures in the DX8 wrapper.
*/
class DX8PolygonRendererClass : public MultiListObjectClass 
{
	MeshClass *						mesh;
	DX8TextureCategoryClass *	texture_category;			
	unsigned							index_offset;				// absolute index of index 0 for our parent mesh
	unsigned							vertex_offset;				// absolute index of vertex 0 for our parent mesh
	unsigned							index_count;				// number of indices
	unsigned							min_vertex_index;			// relative index of the first vertex our polys reference
	unsigned							vertex_index_range;		// range to the last vertex our polys reference
	bool								strip;						// is this a strip?

public:
	DX8PolygonRendererClass(
		unsigned index_count,
		MeshClass* mesh_,
		DX8TextureCategoryClass* tex_cat,
		unsigned vertex_offset,
		unsigned index_offset,
		bool strip);
	DX8PolygonRendererClass(const DX8PolygonRendererClass& src,MeshClass * mesh_);
	~DX8PolygonRendererClass();

	void								Render(/*const Matrix3D & tm,*/int base_vertex_offset);
	void								Render_Sorted(/*const Matrix3D & tm,*/int base_vertex_offset,const SphereClass & bounding_sphere);
	void								Set_Vertex_Index_Range(unsigned min_vertex_index_,unsigned vertex_index_range_);
	
	unsigned							Get_Vertex_Offset(void)	{ return vertex_offset; }
	unsigned							Get_Index_Offset(void)	{ return index_offset; }

	MeshClass*						Get_Mesh_Class() { return mesh; }
//	MeshModelClass*				Get_Mesh_Model_Class() { return mesh->Peek_Model(); }
	
	DX8TextureCategoryClass*	Get_Texture_Category() { return texture_category; }
	void								Set_Texture_Category(DX8TextureCategoryClass* tc) { texture_category=tc; }

	void Log();
};

// ----------------------------------------------------------------------------


#endif