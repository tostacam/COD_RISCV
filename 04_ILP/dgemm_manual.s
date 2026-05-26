	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 5
	.globl	_dgemm                          ; -- Begin function dgemm
	.p2align	2
_dgemm:                                 ; @dgemm
	.cfi_startproc
; %bb.0:
	ldrsw	x8, [x0]
	cbz	w8, LBB0_7
; %bb.1:
	mov	x9, #0                          ; =0x0
	ldr	x13, [x0, #8]
	ldr	x11, [x0, #24]
	ldr	x10, [x0, #40]
	add	x11, x11, #32
	lsl	x12, x8, #3
	add	x13, x13, #32
	movi.2d	v0, #0000000000000000
LBB0_2:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_3 Depth 2
                                        ;       Child Loop BB0_4 Depth 3
	mov	x14, #0                         ; =0x0
	mul	x15, x9, x8
	add	x15, x10, x15, lsl #3
	mov	x16, x11
LBB0_3:                                 ;   Parent Loop BB0_2 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB0_4 Depth 3
	mov	x17, #0                         ; =0x0
	movi.2d	v1, #0000000000000000
	mov	x0, x13
	mov	x1, x16
	movi.2d	v3, #0000000000000000
	movi.2d	v2, #0000000000000000
	movi.2d	v4, #0000000000000000
LBB0_4:                                 ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_3 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldp	q5, q6, [x0, #-32]
	ldp	q7, q16, [x0], #64
	ldp	q17, q18, [x1, #-32]
	ldp	q19, q20, [x1], #64
	fmla.2d	v1, v17, v5
	fmla.2d	v3, v18, v6
	fmla.2d	v2, v19, v7
	fmla.2d	v4, v20, v16
	add	x17, x17, #8
	cmp	x17, x8
	b.lo	LBB0_4
; %bb.5:                                ;   in Loop: Header=BB0_3 Depth=2
	fadd.2d	v1, v3, v1
	fadd.2d	v1, v1, v0
	fadd.2d	v2, v4, v2
	fadd.2d	v1, v2, v1
	faddp.2d	d1, v1
	str	d1, [x15, x14, lsl #3]
	add	x14, x14, #1
	add	x16, x16, x12
	cmp	x14, x8
	b.ne	LBB0_3
; %bb.6:                                ;   in Loop: Header=BB0_2 Depth=1
	add	x9, x9, #1
	add	x13, x13, x12
	cmp	x9, x8
	b.ne	LBB0_2
LBB0_7:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	stp	x26, x25, [sp, #-80]!           ; 16-byte Folded Spill
	stp	x24, x23, [sp, #16]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #48]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	add	x29, sp, #64
	sub	sp, sp, #592
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
Lloh0:
	adrp	x8, ___stack_chk_guard@GOTPAGE
Lloh1:
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
Lloh2:
	ldr	x8, [x8]
	stur	x8, [x29, #-72]
	mov	w19, #1024                      ; =0x400
	stp	x19, x19, [sp]
Lloh3:
	adrp	x2, l_.str@PAGE
Lloh4:
	add	x2, x2, l_.str@PAGEOFF
	add	x0, sp, #328
	mov	w1, #256                        ; =0x100
	bl	_snprintf
	stp	x19, x19, [sp]
Lloh5:
	adrp	x2, l_.str.1@PAGE
Lloh6:
	add	x2, x2, l_.str.1@PAGEOFF
	add	x0, sp, #72
	mov	w1, #256                        ; =0x100
	bl	_snprintf
Lloh7:
	adrp	x19, l_.str.2@PAGE
Lloh8:
	add	x19, x19, l_.str.2@PAGEOFF
	add	x0, sp, #328
	mov	x1, x19
	bl	_fopen
	mov	x20, x0
	add	x0, sp, #72
	mov	x1, x19
	bl	_fopen
	mov	x21, x0
	mov	x0, x20
	bl	_read_matrix
	mov	x19, x0
	mov	x20, x1
	lsr	x25, x0, #32
	mov	x0, x21
	bl	_read_matrix
	mov	x21, x0
	mov	x22, x1
	bl	_transpose_matrix
	mov	x23, x0
	mov	x24, x1
	mul	w8, w19, w25
	sxtw	x0, w8
	mov	w1, #8                          ; =0x8
	bl	_calloc
	mov	x25, x0
	stp	x19, x20, [sp, #24]
	stp	x23, x24, [sp, #40]
	stp	x19, x0, [sp, #56]
Lloh9:
	adrp	x0, _dgemm@PAGE
Lloh10:
	add	x0, x0, _dgemm@PAGEOFF
	add	x1, sp, #24
	bl	_benchmark
	str	d0, [sp]
Lloh11:
	adrp	x0, l_.str.3@PAGE
Lloh12:
	add	x0, x0, l_.str.3@PAGEOFF
	bl	_printf
	mov	x0, x19
	mov	x1, x20
	bl	_release_matrix
	mov	x0, x21
	mov	x1, x22
	bl	_release_matrix
	mov	x0, x19
	mov	x1, x25
	bl	_release_matrix
	ldur	x8, [x29, #-72]
Lloh13:
	adrp	x9, ___stack_chk_guard@GOTPAGE
Lloh14:
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
Lloh15:
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	LBB1_2
; %bb.1:
	mov	w0, #0                          ; =0x0
	add	sp, sp, #592
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp], #80             ; 16-byte Folded Reload
	ret
LBB1_2:
	bl	___stack_chk_fail
	.loh AdrpLdrGotLdr	Lloh13, Lloh14, Lloh15
	.loh AdrpAdd	Lloh11, Lloh12
	.loh AdrpAdd	Lloh9, Lloh10
	.loh AdrpAdd	Lloh7, Lloh8
	.loh AdrpAdd	Lloh5, Lloh6
	.loh AdrpAdd	Lloh3, Lloh4
	.loh AdrpLdrGotLdr	Lloh0, Lloh1, Lloh2
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"./datasets/matrix_%dx%d_A.bin"

l_.str.1:                               ; @.str.1
	.asciz	"./datasets/matrix_%dx%d_B.bin"

l_.str.2:                               ; @.str.2
	.asciz	"rb"

l_.str.3:                               ; @.str.3
	.asciz	"Execution time: %f seconds \n"

.subsections_via_symbols
