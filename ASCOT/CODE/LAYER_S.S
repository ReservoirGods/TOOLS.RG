**************************************************************************************
*	LAYER_S.S
*
*	layer rendering
*
*	[c] 2003 Reservoir Gods
**************************************************************************************



**************************************************************************************
;	EXPORTS / IMPORTS
**************************************************************************************

	export	Layer_Draw_2BP_Col1
	export	Layer_Draw_2BP_Col2
	export	Layer_Draw_2BP_Col3

	export	Layer_Draw_4BP_Col1
	export	Layer_Draw_4BP_Col2
	export	Layer_Draw_4BP_Col3
	export	Layer_Draw_4BP_Col4
	export	Layer_Draw_4BP_Col5
	export	Layer_Draw_4BP_Col6
	export	Layer_Draw_4BP_Col7
	export	Layer_Draw_4BP_Col8
	export	Layer_Draw_4BP_Col9
	export	Layer_Draw_4BP_Col10
	export	Layer_Draw_4BP_Col11
	export	Layer_Draw_4BP_Col12
	export	Layer_Draw_4BP_Col13
	export	Layer_Draw_4BP_Col14
	export	Layer_Draw_4BP_Col15

	export	Layer_DrawPic


**************************************************************************************
;	EQUATES
**************************************************************************************



**************************************************************************************
;	MACROS
**************************************************************************************

*------------------------------------------------------------------------------------*

	MACRO	mLayerDrawTop

	movem.l	d0-d7/a0-a6,-(a7)

	move.l	16*4(a7),a2
	addq.l	#4,a2
	move.w	17*4(a7),d7
	ext.l	d7

	subq.w	#1,d0
	bmi		.fin
	subq.w	#1,d1
	bmi		.fin

	move.l	#$30001,d5

	move.l	a1,d3
	and.w	#1,d3
	beq.s	.not1

	move.l	#$10003,d5

.not1:

.loopY:
	move.w	d0,d3

	move.l	a0,a3
	move.l	a1,a4

	move.l	d5,d6
.loopX:
	moveq	#0,d4
	move.b	(a3),d4
	add.l	d7,a3
	cmp.b	#' ',d4
	beq		.nextX

	add.w	d4,d4
	add.w	d4,d4
	move.l	(a2,d4.w),a5


	ENDM

*------------------------------------------------------------------------------------*

	MACRO	mLayerDrawBot

.nextX:
	add.w	d6,a4
	swap	d6
	dbra	d3,.loopX

	add.w	d2,a0
	lea		160*8(a1),a1

	dbra	d1,.loopY
.fin:
	movem.l	(a7)+,d0-d7/a0-a6
	rts

	ENDM

*------------------------------------------------------------------------------------*

	MACRO	mLayerDrawTop4

	movem.l	d0-d7/a0-a6,-(a7)

	move.l	16*4(a7),a2
	addq.l	#4,a2
	move.w	17*4(a7),d7
	ext.l	d7

	subq.w	#1,d0
	bmi		.fin
	subq.w	#1,d1
	bmi		.fin

	move.l	#$70001,d5

	move.l	a1,d3
	and.w	#1,d3
	beq.s	.not1

	move.l	#$10007,d5

.not1:

.loopY:
	move.w	d0,d3

	move.l	a0,a3
	move.l	a1,a4

	move.l	d5,d6
.loopX:
	moveq	#0,d4
	move.b	(a3),d4
	add.l	d7,a3
	cmp.b	#' ',d4
	beq		.nextX

	add.w	d4,d4
	add.w	d4,d4
	move.l	(a2,d4.w),a5


	ENDM

*------------------------------------------------------------------------------------*


**************************************************************************************
	TEXT
**************************************************************************************

*------------------------------------------------------------------------------------*
* FUNCTION: Layer_Draw_Col0
* ACTIOIN:  draws layer
* CREATION: 30.03.03 PNK
*------------------------------------------------------------------------------------*

; a0 -> ptext
; a1 -> pscreen
; a2 -> pFont
; d0 -> width
; d1 -> height
; d2 -> text stepy
; d3 -> text stepx
arse:
Layer_Draw_2BP_Col1:
	mLayerDrawTop

	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	moveq	#0,d4
	move.b	d4,(0*160)+2(a4)
	move.b	d4,(1*160)+2(a4)
	move.b	d4,(2*160)+2(a4)
	move.b	d4,(3*160)+2(a4)
	move.b	d4,(4*160)+2(a4)
	move.b	d4,(5*160)+2(a4)
	move.b	d4,(6*160)+2(a4)
	move.b	d4,(7*160)+2(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*

Layer_Draw_2BP_Col2:
	mLayerDrawTop

	moveq	#0,d4
	move.b	d4,(0*160)+0(a4)
	move.b	d4,(1*160)+0(a4)
	move.b	d4,(2*160)+0(a4)
	move.b	d4,(3*160)+0(a4)
	move.b	d4,(4*160)+0(a4)
	move.b	d4,(5*160)+0(a4)
	move.b	d4,(6*160)+0(a4)
	move.b	d4,(7*160)+0(a4)

	move.b	0*160(a5),0*160+2(a4)
	move.b	1*160(a5),1*160+2(a4)
	move.b	2*160(a5),2*160+2(a4)
	move.b	3*160(a5),3*160+2(a4)
	move.b	4*160(a5),4*160+2(a4)
	move.b	5*160(a5),5*160+2(a4)
	move.b	6*160(a5),6*160+2(a4)
	move.b	7*160(a5),7*160+2(a4)


	mLayerDrawBot

*------------------------------------------------------------------------------------*

Layer_Draw_2BP_Col3:
	mLayerDrawTop

	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)


	move.b	0*160(a5),0*160+2(a4)
	move.b	1*160(a5),1*160+2(a4)
	move.b	2*160(a5),2*160+2(a4)
	move.b	3*160(a5),3*160+2(a4)
	move.b	4*160(a5),4*160+2(a4)
	move.b	5*160(a5),5*160+2(a4)
	move.b	6*160(a5),6*160+2(a4)
	move.b	7*160(a5),7*160+2(a4)


	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col1:
	mLayerDrawTop4

	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	moveq	#0,d4
	move.b	d4,(0*160)+2(a4)
	move.b	d4,(1*160)+2(a4)
	move.b	d4,(2*160)+2(a4)
	move.b	d4,(3*160)+2(a4)
	move.b	d4,(4*160)+2(a4)
	move.b	d4,(5*160)+2(a4)
	move.b	d4,(6*160)+2(a4)
	move.b	d4,(7*160)+2(a4)

	move.b	d4,(0*160)+4(a4)
	move.b	d4,(1*160)+4(a4)
	move.b	d4,(2*160)+4(a4)
	move.b	d4,(3*160)+4(a4)
	move.b	d4,(4*160)+4(a4)
	move.b	d4,(5*160)+4(a4)
	move.b	d4,(6*160)+4(a4)
	move.b	d4,(7*160)+4(a4)

	move.b	d4,(0*160)+6(a4)
	move.b	d4,(1*160)+6(a4)
	move.b	d4,(2*160)+6(a4)
	move.b	d4,(3*160)+6(a4)
	move.b	d4,(4*160)+6(a4)
	move.b	d4,(5*160)+6(a4)
	move.b	d4,(6*160)+6(a4)
	move.b	d4,(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col2:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	d4,(0*160)+0(a4)
	move.b	d4,(1*160)+0(a4)
	move.b	d4,(2*160)+0(a4)
	move.b	d4,(3*160)+0(a4)
	move.b	d4,(4*160)+0(a4)
	move.b	d4,(5*160)+0(a4)
	move.b	d4,(6*160)+0(a4)
	move.b	d4,(7*160)+0(a4)

	move.b	0*160(a5),(0*160)+2(a4)
	move.b	1*160(a5),(1*160)+2(a4)
	move.b	2*160(a5),(2*160)+2(a4)
	move.b	3*160(a5),(3*160)+2(a4)
	move.b	4*160(a5),(4*160)+2(a4)
	move.b	5*160(a5),(5*160)+2(a4)
	move.b	6*160(a5),(6*160)+2(a4)
	move.b	7*160(a5),(7*160)+2(a4)

	move.b	d4,(0*160)+4(a4)
	move.b	d4,(1*160)+4(a4)
	move.b	d4,(2*160)+4(a4)
	move.b	d4,(3*160)+4(a4)
	move.b	d4,(4*160)+4(a4)
	move.b	d4,(5*160)+4(a4)
	move.b	d4,(6*160)+4(a4)
	move.b	d4,(7*160)+4(a4)

	move.b	d4,(0*160)+6(a4)
	move.b	d4,(1*160)+6(a4)
	move.b	d4,(2*160)+6(a4)
	move.b	d4,(3*160)+6(a4)
	move.b	d4,(4*160)+6(a4)
	move.b	d4,(5*160)+6(a4)
	move.b	d4,(6*160)+6(a4)
	move.b	d4,(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col3:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	move.b	0*160(a5),(0*160)+2(a4)
	move.b	1*160(a5),(1*160)+2(a4)
	move.b	2*160(a5),(2*160)+2(a4)
	move.b	3*160(a5),(3*160)+2(a4)
	move.b	4*160(a5),(4*160)+2(a4)
	move.b	5*160(a5),(5*160)+2(a4)
	move.b	6*160(a5),(6*160)+2(a4)
	move.b	7*160(a5),(7*160)+2(a4)

	move.b	d4,(0*160)+4(a4)
	move.b	d4,(1*160)+4(a4)
	move.b	d4,(2*160)+4(a4)
	move.b	d4,(3*160)+4(a4)
	move.b	d4,(4*160)+4(a4)
	move.b	d4,(5*160)+4(a4)
	move.b	d4,(6*160)+4(a4)
	move.b	d4,(7*160)+4(a4)

	move.b	d4,(0*160)+6(a4)
	move.b	d4,(1*160)+6(a4)
	move.b	d4,(2*160)+6(a4)
	move.b	d4,(3*160)+6(a4)
	move.b	d4,(4*160)+6(a4)
	move.b	d4,(5*160)+6(a4)
	move.b	d4,(6*160)+6(a4)
	move.b	d4,(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col4:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	d4,(0*160)+0(a4)
	move.b	d4,(1*160)+0(a4)
	move.b	d4,(2*160)+0(a4)
	move.b	d4,(3*160)+0(a4)
	move.b	d4,(4*160)+0(a4)
	move.b	d4,(5*160)+0(a4)
	move.b	d4,(6*160)+0(a4)
	move.b	d4,(7*160)+0(a4)

	move.b	d4,(0*160)+2(a4)
	move.b	d4,(1*160)+2(a4)
	move.b	d4,(2*160)+2(a4)
	move.b	d4,(3*160)+2(a4)
	move.b	d4,(4*160)+2(a4)
	move.b	d4,(5*160)+2(a4)
	move.b	d4,(6*160)+2(a4)
	move.b	d4,(7*160)+2(a4)

	move.b	0*160(a5),(0*160)+4(a4)
	move.b	1*160(a5),(1*160)+4(a4)
	move.b	2*160(a5),(2*160)+4(a4)
	move.b	3*160(a5),(3*160)+4(a4)
	move.b	4*160(a5),(4*160)+4(a4)
	move.b	5*160(a5),(5*160)+4(a4)
	move.b	6*160(a5),(6*160)+4(a4)
	move.b	7*160(a5),(7*160)+4(a4)

	move.b	d4,(0*160)+6(a4)
	move.b	d4,(1*160)+6(a4)
	move.b	d4,(2*160)+6(a4)
	move.b	d4,(3*160)+6(a4)
	move.b	d4,(4*160)+6(a4)
	move.b	d4,(5*160)+6(a4)
	move.b	d4,(6*160)+6(a4)
	move.b	d4,(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col5:
	mLayerDrawTop4

	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	moveq	#0,d4
	move.b	d4,(0*160)+2(a4)
	move.b	d4,(1*160)+2(a4)
	move.b	d4,(2*160)+2(a4)
	move.b	d4,(3*160)+2(a4)
	move.b	d4,(4*160)+2(a4)
	move.b	d4,(5*160)+2(a4)
	move.b	d4,(6*160)+2(a4)
	move.b	d4,(7*160)+2(a4)

	move.b	0*160(a5),(0*160)+4(a4)
	move.b	1*160(a5),(1*160)+4(a4)
	move.b	2*160(a5),(2*160)+4(a4)
	move.b	3*160(a5),(3*160)+4(a4)
	move.b	4*160(a5),(4*160)+4(a4)
	move.b	5*160(a5),(5*160)+4(a4)
	move.b	6*160(a5),(6*160)+4(a4)
	move.b	7*160(a5),(7*160)+4(a4)

	move.b	d4,(0*160)+6(a4)
	move.b	d4,(1*160)+6(a4)
	move.b	d4,(2*160)+6(a4)
	move.b	d4,(3*160)+6(a4)
	move.b	d4,(4*160)+6(a4)
	move.b	d4,(5*160)+6(a4)
	move.b	d4,(6*160)+6(a4)
	move.b	d4,(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col6:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	d4,(0*160)+0(a4)
	move.b	d4,(1*160)+0(a4)
	move.b	d4,(2*160)+0(a4)
	move.b	d4,(3*160)+0(a4)
	move.b	d4,(4*160)+0(a4)
	move.b	d4,(5*160)+0(a4)
	move.b	d4,(6*160)+0(a4)
	move.b	d4,(7*160)+0(a4)

	move.b	0*160(a5),(0*160)+2(a4)
	move.b	1*160(a5),(1*160)+2(a4)
	move.b	2*160(a5),(2*160)+2(a4)
	move.b	3*160(a5),(3*160)+2(a4)
	move.b	4*160(a5),(4*160)+2(a4)
	move.b	5*160(a5),(5*160)+2(a4)
	move.b	6*160(a5),(6*160)+2(a4)
	move.b	7*160(a5),(7*160)+2(a4)

	move.b	0*160(a5),(0*160)+4(a4)
	move.b	1*160(a5),(1*160)+4(a4)
	move.b	2*160(a5),(2*160)+4(a4)
	move.b	3*160(a5),(3*160)+4(a4)
	move.b	4*160(a5),(4*160)+4(a4)
	move.b	5*160(a5),(5*160)+4(a4)
	move.b	6*160(a5),(6*160)+4(a4)
	move.b	7*160(a5),(7*160)+4(a4)

	move.b	d4,(0*160)+6(a4)
	move.b	d4,(1*160)+6(a4)
	move.b	d4,(2*160)+6(a4)
	move.b	d4,(3*160)+6(a4)
	move.b	d4,(4*160)+6(a4)
	move.b	d4,(5*160)+6(a4)
	move.b	d4,(6*160)+6(a4)
	move.b	d4,(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col7:
	mLayerDrawTop4

	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	move.b	0*160(a5),(0*160)+2(a4)
	move.b	1*160(a5),(1*160)+2(a4)
	move.b	2*160(a5),(2*160)+2(a4)
	move.b	3*160(a5),(3*160)+2(a4)
	move.b	4*160(a5),(4*160)+2(a4)
	move.b	5*160(a5),(5*160)+2(a4)
	move.b	6*160(a5),(6*160)+2(a4)
	move.b	7*160(a5),(7*160)+2(a4)

	move.b	0*160(a5),(0*160)+4(a4)
	move.b	1*160(a5),(1*160)+4(a4)
	move.b	2*160(a5),(2*160)+4(a4)
	move.b	3*160(a5),(3*160)+4(a4)
	move.b	4*160(a5),(4*160)+4(a4)
	move.b	5*160(a5),(5*160)+4(a4)
	move.b	6*160(a5),(6*160)+4(a4)
	move.b	7*160(a5),(7*160)+4(a4)

	moveq	#0,d4
	move.b	d4,(0*160)+6(a4)
	move.b	d4,(1*160)+6(a4)
	move.b	d4,(2*160)+6(a4)
	move.b	d4,(3*160)+6(a4)
	move.b	d4,(4*160)+6(a4)
	move.b	d4,(5*160)+6(a4)
	move.b	d4,(6*160)+6(a4)
	move.b	d4,(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col8:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	d4,(0*160)+0(a4)
	move.b	d4,(1*160)+0(a4)
	move.b	d4,(2*160)+0(a4)
	move.b	d4,(3*160)+0(a4)
	move.b	d4,(4*160)+0(a4)
	move.b	d4,(5*160)+0(a4)
	move.b	d4,(6*160)+0(a4)
	move.b	d4,(7*160)+0(a4)

	move.b	d4,(0*160)+2(a4)
	move.b	d4,(1*160)+2(a4)
	move.b	d4,(2*160)+2(a4)
	move.b	d4,(3*160)+2(a4)
	move.b	d4,(4*160)+2(a4)
	move.b	d4,(5*160)+2(a4)
	move.b	d4,(6*160)+2(a4)
	move.b	d4,(7*160)+2(a4)

	move.b	d4,(0*160)+4(a4)
	move.b	d4,(1*160)+4(a4)
	move.b	d4,(2*160)+4(a4)
	move.b	d4,(3*160)+4(a4)
	move.b	d4,(4*160)+4(a4)
	move.b	d4,(5*160)+4(a4)
	move.b	d4,(6*160)+4(a4)
	move.b	d4,(7*160)+4(a4)

	move.b	0*160(a5),(0*160)+6(a4)
	move.b	1*160(a5),(1*160)+6(a4)
	move.b	2*160(a5),(2*160)+6(a4)
	move.b	3*160(a5),(3*160)+6(a4)
	move.b	4*160(a5),(4*160)+6(a4)
	move.b	5*160(a5),(5*160)+6(a4)
	move.b	6*160(a5),(6*160)+6(a4)
	move.b	7*160(a5),(7*160)+6(a4)

	mLayerDrawBot


*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col9:
	mLayerDrawTop4

	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	moveq	#0,d4
	move.b	d4,(0*160)+2(a4)
	move.b	d4,(1*160)+2(a4)
	move.b	d4,(2*160)+2(a4)
	move.b	d4,(3*160)+2(a4)
	move.b	d4,(4*160)+2(a4)
	move.b	d4,(5*160)+2(a4)
	move.b	d4,(6*160)+2(a4)
	move.b	d4,(7*160)+2(a4)

	move.b	d4,(0*160)+4(a4)
	move.b	d4,(1*160)+4(a4)
	move.b	d4,(2*160)+4(a4)
	move.b	d4,(3*160)+4(a4)
	move.b	d4,(4*160)+4(a4)
	move.b	d4,(5*160)+4(a4)
	move.b	d4,(6*160)+4(a4)
	move.b	d4,(7*160)+4(a4)

	move.b	0*160(a5),(0*160)+6(a4)
	move.b	1*160(a5),(1*160)+6(a4)
	move.b	2*160(a5),(2*160)+6(a4)
	move.b	3*160(a5),(3*160)+6(a4)
	move.b	4*160(a5),(4*160)+6(a4)
	move.b	5*160(a5),(5*160)+6(a4)
	move.b	6*160(a5),(6*160)+6(a4)
	move.b	7*160(a5),(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col10:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	d4,(0*160)+0(a4)
	move.b	d4,(1*160)+0(a4)
	move.b	d4,(2*160)+0(a4)
	move.b	d4,(3*160)+0(a4)
	move.b	d4,(4*160)+0(a4)
	move.b	d4,(5*160)+0(a4)
	move.b	d4,(6*160)+0(a4)
	move.b	d4,(7*160)+0(a4)

	move.b	0*160(a5),(0*160)+2(a4)
	move.b	1*160(a5),(1*160)+2(a4)
	move.b	2*160(a5),(2*160)+2(a4)
	move.b	3*160(a5),(3*160)+2(a4)
	move.b	4*160(a5),(4*160)+2(a4)
	move.b	5*160(a5),(5*160)+2(a4)
	move.b	6*160(a5),(6*160)+2(a4)
	move.b	7*160(a5),(7*160)+2(a4)

	move.b	d4,(0*160)+4(a4)
	move.b	d4,(1*160)+4(a4)
	move.b	d4,(2*160)+4(a4)
	move.b	d4,(3*160)+4(a4)
	move.b	d4,(4*160)+4(a4)
	move.b	d4,(5*160)+4(a4)
	move.b	d4,(6*160)+4(a4)
	move.b	d4,(7*160)+4(a4)

	move.b	0*160(a5),(0*160)+6(a4)
	move.b	1*160(a5),(1*160)+6(a4)
	move.b	2*160(a5),(2*160)+6(a4)
	move.b	3*160(a5),(3*160)+6(a4)
	move.b	4*160(a5),(4*160)+6(a4)
	move.b	5*160(a5),(5*160)+6(a4)
	move.b	6*160(a5),(6*160)+6(a4)
	move.b	7*160(a5),(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col11:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	move.b	0*160(a5),(0*160)+2(a4)
	move.b	1*160(a5),(1*160)+2(a4)
	move.b	2*160(a5),(2*160)+2(a4)
	move.b	3*160(a5),(3*160)+2(a4)
	move.b	4*160(a5),(4*160)+2(a4)
	move.b	5*160(a5),(5*160)+2(a4)
	move.b	6*160(a5),(6*160)+2(a4)
	move.b	7*160(a5),(7*160)+2(a4)

	move.b	d4,(0*160)+4(a4)
	move.b	d4,(1*160)+4(a4)
	move.b	d4,(2*160)+4(a4)
	move.b	d4,(3*160)+4(a4)
	move.b	d4,(4*160)+4(a4)
	move.b	d4,(5*160)+4(a4)
	move.b	d4,(6*160)+4(a4)
	move.b	d4,(7*160)+4(a4)

	move.b	0*160(a5),(0*160)+6(a4)
	move.b	1*160(a5),(1*160)+6(a4)
	move.b	2*160(a5),(2*160)+6(a4)
	move.b	3*160(a5),(3*160)+6(a4)
	move.b	4*160(a5),(4*160)+6(a4)
	move.b	5*160(a5),(5*160)+6(a4)
	move.b	6*160(a5),(6*160)+6(a4)
	move.b	7*160(a5),(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col12:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	d4,(0*160)+0(a4)
	move.b	d4,(1*160)+0(a4)
	move.b	d4,(2*160)+0(a4)
	move.b	d4,(3*160)+0(a4)
	move.b	d4,(4*160)+0(a4)
	move.b	d4,(5*160)+0(a4)
	move.b	d4,(6*160)+0(a4)
	move.b	d4,(7*160)+0(a4)

	move.b	d4,(0*160)+2(a4)
	move.b	d4,(1*160)+2(a4)
	move.b	d4,(2*160)+2(a4)
	move.b	d4,(3*160)+2(a4)
	move.b	d4,(4*160)+2(a4)
	move.b	d4,(5*160)+2(a4)
	move.b	d4,(6*160)+2(a4)
	move.b	d4,(7*160)+2(a4)

	move.b	0*160(a5),(0*160)+4(a4)
	move.b	1*160(a5),(1*160)+4(a4)
	move.b	2*160(a5),(2*160)+4(a4)
	move.b	3*160(a5),(3*160)+4(a4)
	move.b	4*160(a5),(4*160)+4(a4)
	move.b	5*160(a5),(5*160)+4(a4)
	move.b	6*160(a5),(6*160)+4(a4)
	move.b	7*160(a5),(7*160)+4(a4)

	move.b	0*160(a5),(0*160)+6(a4)
	move.b	1*160(a5),(1*160)+6(a4)
	move.b	2*160(a5),(2*160)+6(a4)
	move.b	3*160(a5),(3*160)+6(a4)
	move.b	4*160(a5),(4*160)+6(a4)
	move.b	5*160(a5),(5*160)+6(a4)
	move.b	6*160(a5),(6*160)+6(a4)
	move.b	7*160(a5),(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col13:
	mLayerDrawTop4

	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	moveq	#0,d4
	move.b	d4,(0*160)+2(a4)
	move.b	d4,(1*160)+2(a4)
	move.b	d4,(2*160)+2(a4)
	move.b	d4,(3*160)+2(a4)
	move.b	d4,(4*160)+2(a4)
	move.b	d4,(5*160)+2(a4)
	move.b	d4,(6*160)+2(a4)
	move.b	d4,(7*160)+2(a4)

	move.b	0*160(a5),(0*160)+4(a4)
	move.b	1*160(a5),(1*160)+4(a4)
	move.b	2*160(a5),(2*160)+4(a4)
	move.b	3*160(a5),(3*160)+4(a4)
	move.b	4*160(a5),(4*160)+4(a4)
	move.b	5*160(a5),(5*160)+4(a4)
	move.b	6*160(a5),(6*160)+4(a4)
	move.b	7*160(a5),(7*160)+4(a4)

	move.b	0*160(a5),(0*160)+6(a4)
	move.b	1*160(a5),(1*160)+6(a4)
	move.b	2*160(a5),(2*160)+6(a4)
	move.b	3*160(a5),(3*160)+6(a4)
	move.b	4*160(a5),(4*160)+6(a4)
	move.b	5*160(a5),(5*160)+6(a4)
	move.b	6*160(a5),(6*160)+6(a4)
	move.b	7*160(a5),(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col14:
	mLayerDrawTop4

	moveq	#0,d4
	move.b	d4,(0*160)+0(a4)
	move.b	d4,(1*160)+0(a4)
	move.b	d4,(2*160)+0(a4)
	move.b	d4,(3*160)+0(a4)
	move.b	d4,(4*160)+0(a4)
	move.b	d4,(5*160)+0(a4)
	move.b	d4,(6*160)+0(a4)
	move.b	d4,(7*160)+0(a4)

	move.b	0*160(a5),(0*160)+2(a4)
	move.b	1*160(a5),(1*160)+2(a4)
	move.b	2*160(a5),(2*160)+2(a4)
	move.b	3*160(a5),(3*160)+2(a4)
	move.b	4*160(a5),(4*160)+2(a4)
	move.b	5*160(a5),(5*160)+2(a4)
	move.b	6*160(a5),(6*160)+2(a4)
	move.b	7*160(a5),(7*160)+2(a4)

	move.b	0*160(a5),(0*160)+4(a4)
	move.b	1*160(a5),(1*160)+4(a4)
	move.b	2*160(a5),(2*160)+4(a4)
	move.b	3*160(a5),(3*160)+4(a4)
	move.b	4*160(a5),(4*160)+4(a4)
	move.b	5*160(a5),(5*160)+4(a4)
	move.b	6*160(a5),(6*160)+4(a4)
	move.b	7*160(a5),(7*160)+4(a4)

	move.b	0*160(a5),(0*160)+6(a4)
	move.b	1*160(a5),(1*160)+6(a4)
	move.b	2*160(a5),(2*160)+6(a4)
	move.b	3*160(a5),(3*160)+6(a4)
	move.b	4*160(a5),(4*160)+6(a4)
	move.b	5*160(a5),(5*160)+6(a4)
	move.b	6*160(a5),(6*160)+6(a4)
	move.b	7*160(a5),(7*160)+6(a4)

	mLayerDrawBot

*------------------------------------------------------------------------------------*


Layer_Draw_4BP_Col15:
	mLayerDrawTop4

	move.b	0*160(a5),0*160(a4)
	move.b	1*160(a5),1*160(a4)
	move.b	2*160(a5),2*160(a4)
	move.b	3*160(a5),3*160(a4)
	move.b	4*160(a5),4*160(a4)
	move.b	5*160(a5),5*160(a4)
	move.b	6*160(a5),6*160(a4)
	move.b	7*160(a5),7*160(a4)

	move.b	0*160(a5),(0*160)+2(a4)
	move.b	1*160(a5),(1*160)+2(a4)
	move.b	2*160(a5),(2*160)+2(a4)
	move.b	3*160(a5),(3*160)+2(a4)
	move.b	4*160(a5),(4*160)+2(a4)
	move.b	5*160(a5),(5*160)+2(a4)
	move.b	6*160(a5),(6*160)+2(a4)
	move.b	7*160(a5),(7*160)+2(a4)

	move.b	0*160(a5),(0*160)+4(a4)
	move.b	1*160(a5),(1*160)+4(a4)
	move.b	2*160(a5),(2*160)+4(a4)
	move.b	3*160(a5),(3*160)+4(a4)
	move.b	4*160(a5),(4*160)+4(a4)
	move.b	5*160(a5),(5*160)+4(a4)
	move.b	6*160(a5),(6*160)+4(a4)
	move.b	7*160(a5),(7*160)+4(a4)

	move.b	0*160(a5),(0*160)+6(a4)
	move.b	1*160(a5),(1*160)+6(a4)
	move.b	2*160(a5),(2*160)+6(a4)
	move.b	3*160(a5),(3*160)+6(a4)
	move.b	4*160(a5),(4*160)+6(a4)
	move.b	5*160(a5),(5*160)+6(a4)
	move.b	6*160(a5),(6*160)+6(a4)
	move.b	7*160(a5),(7*160)+6(a4)

	mLayerDrawBot


Layer_DrawPic:
	movem.l	d0-d7/a0-a6,-(a7)

	move.l	a0,d0
	beq		.cull
	move.l	a1,d0
	beq		.cull

	moveq	#40,d7										; 41 loops

.CopyLoop:

	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location

	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location

	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location

	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location

	dbra	d7,.CopyLoop

	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location

	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location

	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location
	movem.l	(a1)+,d0-d6/a2-a6							; read 48 bytes
	movem.l	d0-d6/a2-a6,(a0)							; write 48 bytes
	lea		48(a0),a0									; next chunk location

	movem.l	(a1),d0-d6/a2								; read 48 bytes
	movem.l	d0-d6/a2,(a0)								; write 48 bytes

.cull:
	movem.l	(a7)+,d0-d7/a0-a6
	rts