//---------------------------------------------------------------------------
// Toon ink shader
//	c0		= (0,0.5,1.0,2.0)
//	c1-4	= World*View*Projection matrix
//	c5		= Vertex normal multiplier
//	c11		= Ink color
//---------------------------------------------------------------------------

vs.1.1
dcl_position v0
dcl_normal v1
mov	r0,	v0
mul	r1,	v1,	c5
add	r2,	r0,	r1
m4x4 oPos, r2, c1
mov	oD0, c11

