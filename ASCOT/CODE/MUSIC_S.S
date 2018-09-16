**************************************************************************************
*	MUSIC_S.S
*
*	musicmon replay
*
*	[c] 2000 Reservoir Gods/Ninja III of Newline/Indendent
**************************************************************************************

	export	MusicMon_Start
	export	MusicMon_Stop
	export	MusicMon_Vbl

	import	gMusicMonPos
	import	gMusicMonPat

**************************************************************************************
	TEXT
**************************************************************************************

MusicMon_Start:
	move.w	#1,-(a7)
	pea		(a0)
	jsr		MusicMon_Play
	addq.l	#6,a7
	rts

MusicMon_Stop:
	move.w	#0,-(a7)
	pea		0.l
	jsr		MusicMon_Play
	addq.l	#6,a7
	rts

*gMusicMonPat:
*	dc.w	0
*gMusicMonPos:
*	dc.w	0


MusicMon00:
  dc.b      $ff,$ff 
MusicMon01:
  dc.b      $ff 
MusicMon02:
  dc.b      $00 
MusicMon03:
  dc.b      $00 
MusicMon04:
  dc.b      $00 

MusicMon_Play:
	movem.l	a0-a6/d0-d7,-(a7) 

	lea       gpMusicMonSong(pc),a1
	tst.w     $44(a7) 
	bne       MusicMon06 
	cmpi.b    #-1,$17(a1) 
	beq       MusicMon09 
	bsr       MusicMon62 
	bra       MusicMon09 
MusicMon06:
	movea.l   $40(a7),a2
	move.l    a2,(a1) 
	move.b    $45(a7),4(a1) 
	lea       MusicMon69(pc),a3
	clr.l     (a3)
	lea       MusicMon6a(pc),a3
	clr.l     (a3)
	lea       MusicMon6b(pc),a3
	clr.l     (a3)
	lea       MusicMon67(pc),a3
	clr.l     8(a3) 
	lea       MusicMon00(pc),a3
	move.b    #1,5(a3)
	move.b    $c(a2),4(a3)
	clr.w     $12(a1) 
	move.l    a2,d1 
	add.l     (a2),d1 
	move.l    d1,6(a1)
	move.l    a2,d1 
	add.l     4(a2),d1
	move.l    d1,$a(a1) 
	move.l    a2,d1 
	add.l     8(a2),d1
	move.l    d1,$e(a1) 
	moveq     #0,d3 
	bsr       MusicMon5e 
	cmpi.b    #-1,$17(a1) 
	bne       MusicMon09 


MusicMon09:
MusicMon0a:
  movem.l   (a7)+,a0-a6/d0-d7 
  rts 

MusicMon_Vbl:
  movem.l   a1-a6/d1-d5,-(a7) 
  move.b    MusicMon04(pc),d1
  tst.b     d1
  beq       MusicMon18 
  lea       gpMusicMonSong(pc),a1
  lea       MusicMon69(pc),a2
  btst      #1,$ff8260.l
  beq       MusicMon0c 
  subq.b    #1,$24(a1)
  bcc       MusicMon0d 
  move.b    #2,$24(a1)
  bra       MusicMon18 
MusicMon0c:
  btst      #1,$ff820a.l
  bne       MusicMon0d 
  subq.b    #1,$24(a1)
  bcc       MusicMon0d 
  move.b    #5,$24(a1)
  bra       MusicMon18 
MusicMon0d:
  subq.b    #1,$13(a1)						; speed
  bcc       MusicMon16 

	addq.w	#1,gMusicMonPos

  move.b    MusicMon03(pc),$13(a1)			; reset speed counter
  moveq     #0,d1 
MusicMon0e:
  subq.b    #1,$14(a1,d1.w)					; channel delay
  bpl       MusicMon15 
  move.w    d1,d2 
  add.w     d2,d2 
  add.w     d2,d2 
MusicMon0f:
  movea.l   $18(a1,d2.w),a3					; current channel ptr
MusicMon10:
  moveq     #0,d3 
  move.b    (a3)+,d3
  cmpi.b    #$7f,d3 
  bne       MusicMon11 

	clr.w	gMusicMonPos
	addq.w	#1,gMusicMonPat

  bsr       MusicMon5d 
  bra.s     MusicMon0f 
MusicMon11:
  btst      #7,d3 
  beq       MusicMon12 
  andi.b    #$7f,d3 
  move.b    d3,$14(a1,d1.w) 
  bra       MusicMon14 
MusicMon12:
  btst      #6,d3 
  beq       MusicMon13 
  andi.b    #$3f,d3 
  add.w     d3,d3 
  movea.l   $a(a1),a4 
  move.w    0(a4,d3.w),d3 
  add.l     (a1),d3 
  move.l    d3,(a2) 
  bra.s     MusicMon10 
MusicMon13:
  clr.b     $14(a1,d1.w)
  bsr       MusicMon19 
  btst      #7,(a3) 
  bne.s     MusicMon10 
MusicMon14:
  move.l    a3,$18(a1,d2.w) 
MusicMon15:
  lea       $2a(a2),a2
  addq.w    #1,d1 
  cmpi.w    #3,d1 
  bne.s     MusicMon0e 

MusicMon16:
  lea       MusicMon69(pc),a2
  lea       MusicMon67(pc),a1
  moveq     #0,d3 
  moveq     #0,d1 
MusicMon17:
  bsr       MusicMon2e 
  lea       $2a(a2),a2
  addq.w    #1,d1 
  cmpi.w    #3,d1 
  bne.s     MusicMon17 
  bsr       MusicMon_OutputYM 
MusicMon18:
  movem.l   (a7)+,a1-a6/d1-d5 
  rts 


MusicMon19:
  lea       MusicMon67(pc),a5
  movea.l   (a2),a4 
  clr.l     4(a2) 
  clr.l     $12(a2) 
  move.w    d1,d4 
  addq.w    #3,d4 
  bset      d1,7(a5)
  bset      d4,7(a5)
  move.b    #-1,$16(a2) 
  btst      #0,(a4) 
  beq       MusicMon1a 
  bclr      d1,7(a5)
MusicMon1a:
  btst      #1,(a4) 
  beq       MusicMon21 
  bclr      d4,7(a5)
  move.b    $14(a4),$17(a2) 
  cmpi.b    #-1,1(a4) 
  beq       MusicMon21 
  btst      #2,(a4) 
  beq       MusicMon1b 
  move.b    1(a4),$16(a2) 
  bra       MusicMon1e 
MusicMon1b:
  move.b    d3,d4 
  subi.b    #$3e,d4 
  neg.b     d4
  lsr.b     #1,d4 
  add.b     1(a4),d4
  subi.b    #$f,d4
  bpl       MusicMon1c 
  clr.b     d4
  bra       MusicMon1d 
MusicMon1c:
  cmpi.b    #$1f,d4 
  ble       MusicMon1d 
  moveq     #$1f,d4 
MusicMon1d:
  move.b    d4,$16(a2)
MusicMon1e:
  move.b    $1a(a4),$26(a2) 
  clr.b     $25(a2) 
  move.b    $18(a4),$27(a2) 
  move.b    $16(a4),$29(a2) 
  move.b    #1,$28(a2)
  move.b    7(a4),d4
  lsr.b     #4,d4 
  cmpi.b    #3,d4 
  bne       MusicMon1f 
  move.b    $16(a4),d5
  add.b     d5,d5 
  move.b    d5,$28(a2)
  bra       MusicMon21 
MusicMon1f:
  tst.b     $17(a4) 
  bne       MusicMon20 
  move.b    #3,$28(a2)
MusicMon20:
  cmpi.b    #1,d4 
  bne       MusicMon21 
  neg.b     $28(a2) 
MusicMon21:
  move.b    $c(a4),d4 
  lsr.b     #4,d4 
  move.b    d4,7(a2)
  move.b    $a(a4),$19(a2)
  move.b    7(a4),d4
  andi.b    #3,d4 
  cmpi.b    #3,d4 
  bne       MusicMon22 
  move.b    9(a4),d5
  bra       MusicMon23 
MusicMon22:
  moveq     #1,d5 
  tst.b     8(a4) 
  bne       MusicMon23 
  moveq     #2,d5 
MusicMon23:
  move.b    d5,$1a(a2)
  neg.b     d5
  move.b    d5,$18(a2)
  cmpi.b    #1,d4 
  bne       MusicMon24 
  neg.b     $1a(a2) 
MusicMon24:
  move.b    $d(a4),$1b(a2)
  moveq     #0,d5 
  move.b    $b(a4),d5 
  move.w    d5,$1c(a2)
  move.b    7(a4),d4
  lsr.b     #2,d4 
  andi.b    #3,d4 
  cmpi.b    #3,d4 
  bne       MusicMon25 
  add.w     d5,d5 
  bra       MusicMon26 
MusicMon25:
  move.b    2(a4),d5
  addq.w    #1,d5 
  cmpi.b    #1,d4 
  bne       MusicMon26 
  neg.w     d5
MusicMon26:
  move.w    d5,$1e(a2)
  move.b    $11(a4),$24(a2) 
  moveq     #0,d5 
  move.b    $10(a4),d5
  addq.w    #1,d5 
  clr.w     $20(a2) 
  btst      #6,(a4) 
  bne       MusicMon27 
  neg.w     d5
  move.w    $e(a4),$20(a2)
MusicMon27:
  move.w    d5,$22(a2)
  lea       MusicMon6c(pc),a5
  btst      #5,(a4) 
  bne       MusicMon28 
  addi.b    #$c,d3
MusicMon28:
  add.w     d3,d3 
  move.w    0(a5,d3.w),8(a2)
  move.b    $13(a4),$14(a2) 
  movea.l   a4,a6 
  lea       3(a6),a6
  moveq     #0,d4 
  moveq     #0,d5 
MusicMon29:
  move.b    (a6)+,d5
  cmpi.b    #-1,d5
  bne       MusicMon2a 
  move.w    #$ff,$a(a2,d4.w)
  tst.w     d4
  beq       MusicMon2d 
  bra       MusicMon2c 
MusicMon2a:
  add.w     d5,d5 
  add.w     d3,d5 
  cmpi.w    #$a8,d5 
  ble       MusicMon2b 
  move.w    d3,d5 
MusicMon2b:
  move.w    0(a5,d5.w),$a(a2,d4.w)
  addq.w    #2,d4 
  cmpi.b    #6,d4 
  bne.s     MusicMon29 
MusicMon2c:
  btst      #7,(a4) 
  beq       MusicMon2d 
  move.b    #2,$12(a2)
MusicMon2d:
  rts 

MusicMon2e:
  tst.l     (a2)
  beq       MusicMon4e 
  movea.l   (a2),a3 
  move.w    d1,d2 
  add.w     d2,d2 
  subq.b    #1,$19(a2)
  bhi       MusicMon2f 
  bsr       MusicMon58 
MusicMon2f:
  subq.b    #1,4(a2)
  bcc       MusicMon35 
  move.b    $15(a3),4(a2) 
MusicMon30:
  move.b    6(a2),d3
  move.b    $1b(a3,d3.w),d3 
  cmpi.b    #-1,d3
  bne       MusicMon34 
  subq.b    #1,7(a2)
  bmi       MusicMon33 
  move.b    $c(a3),d3 
  andi.b    #$f,d3
  add.b     d3,$15(a2)
  clr.b     6(a2) 
  btst      #1,(a3) 
  beq       MusicMon31 
  addq.w    #3,d1 
  bclr      d1,7(a1)
  subq.w    #3,d1 
  move.b    $14(a3),$17(a2) 
MusicMon31:
  clr.w     $20(a2) 
  btst      #6,(a3) 
  bne       MusicMon32 
  move.w    $e(a3),$20(a2)
MusicMon32:
  clr.b     $25(a2) 
  move.b    $13(a3),$14(a2) 
  clr.b     $13(a2) 
  bra.s     MusicMon30 
MusicMon33:
  clr.b     7(a2) 
  move.b    #-1,$16(a2) 
  bra       MusicMon35 
MusicMon34:
  move.b    d3,5(a2)
  addq.b    #1,6(a2)
MusicMon35:
  move.b    5(a2),d3
  tst.b     $18(a2) 
  bmi       MusicMon36 
  sub.b     $18(a2),d3
MusicMon36:
  sub.b     MusicMon02(pc),d3
  sub.b     $15(a2),d3
  bpl       MusicMon37 
  moveq     #0,d3 
MusicMon37:
  move.b    d3,8(a1,d1.w) 
  subq.b    #1,$1b(a2)
  bcc       MusicMon38 
  bsr       MusicMon52 
MusicMon38:
  subq.b    #1,$13(a2)
  bcc       MusicMon3d 
  move.b    6(a3),$13(a2) 
  move.b    $12(a2),d3
  move.w    8(a2,d3.w),$10(a2)
  tst.b     $13(a3) 
  beq       MusicMon3a 
  tst.b     d3
  beq       MusicMon39 
  subq.b    #1,$14(a2)
MusicMon39:
  tst.b     $14(a2) 
  beq       MusicMon3b 
MusicMon3a:
  addq.b    #2,d3 
  andi.b    #7,d3 
  cmpi.w    #$ff,8(a2,d3.w) 
  bne       MusicMon3c 
MusicMon3b:
  moveq     #0,d3 
MusicMon3c:
  move.b    d3,$12(a2)
MusicMon3d:
  move.w    $10(a2),d4
  tst.b     $24(a2) 
  bgt       MusicMon3f 
  btst      #3,(a3) 
  bne       MusicMon3e 
  sub.w     $20(a2),d4
  bra       MusicMon3f 
MusicMon3e:
  add.w     $20(a2),d4
MusicMon3f:
  add.w     $1c(a2),d4
  move.b    $b(a3),d3 
  sub.w     d3,d4 
  bpl       MusicMon40 
  moveq     #0,d4 
  bra       MusicMon41 
MusicMon40:
  cmpi.w    #$fff,d4
  ble       MusicMon41 
  move.w    #$fff,d4
MusicMon41:
  move.w    d4,$e(a1) 
  move.b    d4,0(a1,d2.w) 
  move.b    $e(a1),1(a1,d2.w) 
  subq.b    #1,$24(a2)
  bcc       MusicMon42 
  bsr       MusicMon4f 
MusicMon42:
  tst.b     $14(a3) 
  beq       MusicMon43 
  subq.b    #1,$17(a2)
  bcc       MusicMon43 
  clr.b     $17(a2) 
  addq.b    #3,d1 
  bset      d1,7(a1)
  subq.b    #3,d1 
  bra       MusicMon4e 
MusicMon43:
  cmpi.b    #-1,$16(a2) 
  beq       MusicMon4e 
  subq.b    #1,$27(a2)
  bhi       MusicMon48 
  move.b    $18(a3),$27(a2) 
  move.b    $29(a2),d3
  move.b    $16(a3),d4
  add.b     d4,d4 
  add.b     $28(a2),d3
  bpl       MusicMon45 
  move.b    7(a3),d5
  lsr.b     #4,d5 
  cmpi.b    #2,d5 
  bne       MusicMon44 
  moveq     #0,d3 
  neg.b     $28(a2) 
  bra       MusicMon45 
MusicMon44:
  move.b    d4,d3 
MusicMon45:
  cmp.b     d4,d3 
  ble       MusicMon47 
  cmpi.b    #2,d5 
  bne       MusicMon46 
  move.b    d4,d3 
  neg.b     $28(a2) 
  bra       MusicMon47 
MusicMon46:
  moveq     #0,d3 
MusicMon47:
  move.b    d3,$29(a2)
MusicMon48:
  move.b    $16(a2),d3
  btst      #4,(a3) 
  beq       MusicMon49 
  add.b     $25(a2),d3
  bra       MusicMon4a 
MusicMon49:
  sub.b     $25(a2),d3
MusicMon4a:
  add.b     $29(a2),d3
  sub.b     $16(a3),d3
  bpl       MusicMon4b 
  moveq     #0,d3 
MusicMon4b:
  cmpi.b    #$1f,d3 
  ble       MusicMon4c 
  move.b    #$1f,d3 
MusicMon4c:
  move.b    d3,6(a1)
  subq.b    #1,$26(a2)
  bcc       MusicMon4e 
  clr.b     $26(a2) 
  move.b    $25(a2),d4
  add.b     $19(a3),d4
  addq.b    #1,d4 
  cmp.b     $12(a3),d4
  ble       MusicMon4d 
  move.b    $12(a3),d4
MusicMon4d:
  move.b    d4,$25(a2)
MusicMon4e:
  rts 
MusicMon4f:
  clr.b     $24(a2) 
  move.w    $20(a2),d4
  add.w     $22(a2),d4
  bpl       MusicMon50 
  moveq     #0,d4 
  bra       MusicMon51 
MusicMon50:
  move.w    $e(a3),d5 
  cmp.w     d5,d4 
  ble       MusicMon51 
  move.w    d5,d4 
MusicMon51:
  move.w    d4,$20(a2)
  rts 
MusicMon52:
  clr.b     $1b(a2) 
  move.b    7(a3),d4
  lsr.b     #2,d4 
  andi.b    #3,d4 
  cmpi.b    #3,d4 
  bne       MusicMon53 
  move.b    2(a3),$1b(a2) 
MusicMon53:
  move.w    $1c(a2),d5
  add.w     $1e(a2),d5
  move.b    $b(a3),d3 
  add.w     d3,d3 
  cmp.w     d3,d5 
  ble       MusicMon55 
  cmpi.b    #2,d4 
  bne       MusicMon54 
  neg.w     $1e(a2) 
  move.w    d3,d5 
  bra       MusicMon57 
MusicMon54:
  moveq     #0,d5 
MusicMon55:
  tst.w     d5
  bpl       MusicMon57 
  cmpi.b    #2,d4 
  bne       MusicMon56 
  neg.w     $1e(a2) 
  moveq     #0,d5 
  bra       MusicMon57 
MusicMon56:
  move.w    d3,d5 
MusicMon57:
  move.w    d5,$1c(a2)
  moveq     #0,d3 
  rts 
MusicMon58:
  move.b    8(a3),$19(a2) 
  move.b    $18(a2),d3
  add.b     $1a(a2),d3
  bpl       MusicMon5a 
  move.b    7(a3),d4
  andi.b    #3,d4 
  cmpi.b    #2,d4 
  bne       MusicMon59 
  neg.b     $1a(a2) 
  neg.b     d3
  bra       MusicMon5a 
MusicMon59:
  move.b    9(a3),d3
MusicMon5a:
  cmp.b     9(a3),d3
  ble       MusicMon5c 
  cmpi.b    #2,d4 
  bne       MusicMon5b 
  neg.b     $1a(a2) 
  add.b     $1a(a2),d3
  add.b     $1a(a2),d3
  bra       MusicMon5c 
MusicMon5b:
  moveq     #0,d3 
MusicMon5c:
  move.b    d3,$18(a2)
  moveq     #0,d3 
  rts 

MusicMon5d:
  moveq     #0,d3 
  move.b    5(a1),d3
  subq.b    #1,$12(a1)
  bne       MusicMon61 
  addq.b    #1,d3 
MusicMon5e:
  movea.l   $e(a1),a4 
  cmpi.b    #-1,0(a4,d3.w)
  bne       MusicMon60 
  cmpi.b    #1,4(a1)
  beq       MusicMon5f 
  lea       4(a7),a7
  bsr       MusicMon62 
  bra       MusicMon18 
MusicMon5f:
  moveq     #0,d3 
MusicMon60:
  move.b    0(a4,d3.w),$12(a1)
  move.b    d3,5(a1)
MusicMon61:
  add.w     d3,d3 
  add.w     d3,d3 
  clr.w     $14(a1) 
  clr.b     $16(a1) 
  movea.l   (a1),a4 
  movea.l   6(a1),a5
  adda.w    0(a5,d3.w),a4 
  moveq     #0,d4 
  move.l    a4,$18(a1)
  move.b    2(a5,d3.w),d4 
  adda.w    d4,a4 
  move.l    a4,$1c(a1)
  move.b    3(a5,d3.w),d4 
  adda.w    d4,a4 
  move.l    a4,$20(a1)
  rts 

MusicMon62:
  moveq     #0,d3 
  move.b    $17(a1),d3
  move.b    #-1,$17(a1) 

  lea       MusicMon00(pc),a1
  clr.b     5(a1) 
  lea       MusicMon67(pc),a1
  clr.l     8(a1) 
MusicMon_OutputYM:
  move.b    #7,$ff8800.l
  move.b    $ff8800.l,d4
  move.b    7(a1),d3
  move.b    d3,d5 
  move.b    MusicMon01(pc),d2
  andi.b    #$3f,d2 
  and.b     d2,d3 
  not.b     d2
  and.b     d2,d4 
  or.b      d4,d3 
  move.b    d3,7(a1)
  move.w    MusicMon00(pc),d2
  lea       $e(a1),a1 
  move.w    #$d,d1
MusicMon64:
  btst      d1,d2 
  bne       MusicMon65 
  subq.w    #1,a1 
  bra       MusicMon66 
MusicMon65:
  move.b    d1,$ff8800.l
  move.b    -(a1),$ff8802.l 
MusicMon66:
  dbf       d1,MusicMon64
  move.b    d5,7(a1)
  rts 

MusicMon_Data:
MusicMon67:
  dcb.w     8,0 
gpMusicMonSong:
  dcb.w     11,0
  dc.b      $00,$ff,$00,$00,$00,$00,$00,$00 
  dcb.w     4,0 
MusicMon69:
  dcb.w     21,0
MusicMon6a:
  dcb.w     21,0
MusicMon6b:
  dcb.w     21,0
MusicMon6c:
  DC.B      $0E,$EC,$0E,$1A,$0D,'N',$0C,$8A 
  DC.B      $0B,$DB,$0B,'1',$0A,$8C,$09,$F8 
  DC.B      $09,'j',$08,$E0,$08,'b',$07,$E8 
  DC.B      $07,'w',$07,$0C,$06,$A7,$06,'G' 
  DC.B      $05,$ED,$05,$98,$05,'G',$04,$FC 
  DC.B      $04,$B4,$04,'p',$04,'1',$03,$F4 
  DC.B      $03,$BC,$03,$86,$03,'S',$03,'$' 
  DC.B      $02,$F6,$02,$CC,$02,$A4,$02,'~' 
  DC.B      $02,'Z',$02,'8',$02,$18,$01,$FA 
  DC.B      $01,$DE,$01,$C3,$01,$AA,$01,$92 
  DC.B      $01,'{',$01,'f',$01,'R',$01,'?' 
  DC.B      $01,'-',$01,$1C,$01,$0C,$00,$FD 
  DC.B      $00,$EF,$00,$E1,$00,$D5,$00,$C9 
  DC.B      $00,$BE,$00,$B3,$00,$A9,$00,$9F 
  DC.B      $00,$96,$00,$8E,$00,$86,$00,$7F 
  DC.B      $00,'w',$00,'q',$00,'j',$00,'d' 
  DC.B      $00,'_',$00,'Y',$00,'T',$00,'P' 
  DC.B      $00,'K',$00,'G',$00,'C',$00,'?' 
  DC.B      $00,';',$00,'9',$00,'5',$00,'2' 
  DC.B      $00,'0',$00,'-',$00,'*',$00,'(' 
  DC.B      $00,'&',$00,'#',$00,'!',$00,' ' 
  DC.B      $00,$1D 


