@.div_by_zero_error = constant [23 x i8] c"Error division by zero\00"
@.str_specifier = constant [4 x i8] c"%s\0A\00"
@.int_specifier = constant [4 x i8] c"%d\0A\00"
@.var22 = constant [9 x i8] c"got here\00"
@.var49 = constant [13 x i8] c"check powers\00"
@.var65 = constant [10 x i8] c"with true\00"
@.var96 = constant [11 x i8] c"with false\00"

declare i32 @printf(i8*, ...)
declare void @exit(i32)

define void @print(i8*) {
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)
	ret void
}

define void @printi(i32) {
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)
	ret void
}

define i32 @powern(i32 %arg0, i32 %arg1) {
	%stack_array_ptr = alloca [50 x i32]
	%var0 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %arg0, i32* %var0
	%var1 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %arg1, i32* %var1
	%var2 = add i32 0, 1
	%var3 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 2
	store i32 %var2, i32* %var3
	br label %label_20
label_20:
	%var4 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var5 = load i32, i32* %var4
	%var6 = add i32 0, 0
	%var7 = icmp sgt i32 %var5, %var6
	br i1 %var7, label %label_26, label %label_41
label_26:
	%var8 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 2
	%var9 = load i32, i32* %var8
	%var10 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var11 = load i32, i32* %var10
	%var12 = mul i32 %var9, %var11
	%var13 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 2
	store i32 %var12, i32* %var13
	%var14 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var15 = load i32, i32* %var14
	%var16 = add i32 0, 1
	%var17 = sub i32 %var15, %var16
	%var18 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %var17, i32* %var18
	br label %label_20
label_41:
	%var19 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 2
	%var20 = load i32, i32* %var19
	ret i32 %var20
	ret i32 0
}

define i32 @dum(i32 %arg0) {
	%stack_array_ptr = alloca [50 x i32]
	%var21 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %arg0, i32* %var21
	call void @print(i8* getelementptr ([9 x i8], [9 x i8]* @.var22, i32 0, i32 0))
	%var23 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var24 = load i32, i32* %var23
	%var25 = trunc i32 %var24 to i1
	br i1 %var25, label %label_56, label %label_58
label_56:
	br label %label_60
label_58:
	br label %label_60
label_60:
	%var26 = phi i32 [1, %label_56], [0, %label_58]
	ret i32 %var26
	ret i32 0
}

define i32 @ifat(i32 %arg0, i32 %arg1, i32 %arg2, i32 %arg3) {
	%stack_array_ptr = alloca [50 x i32]
	%var27 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %arg0, i32* %var27
	%var28 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %arg1, i32* %var28
	%var29 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 2
	store i32 %arg2, i32* %var29
	%var30 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 3
	store i32 %arg3, i32* %var30
	%var31 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var32 = load i32, i32* %var31
	%var33 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 3
	%var34 = load i32, i32* %var33
	%var35 = icmp slt i32 %var32, %var34
	br i1 %var35, label %label_82, label %label_90
	br label %label_82
label_82:
	%var36 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 2
	%var37 = load i32, i32* %var36
	%var38 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var39 = load i32, i32* %var38
	%var40 = icmp slt i32 %var37, %var39
	br i1 %var40, label %label_104, label %label_90
	br label %label_90
label_90:
	%var41 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var42 = load i32, i32* %var41
	%var43 = trunc i32 %var42 to i1
	br i1 %var43, label %label_95, label %label_97
label_95:
	br label %label_99
label_97:
	br label %label_99
label_99:
	%var44 = phi i32 [1, %label_95], [0, %label_97]
	%var45 = call i32 @dum(i32 %var44)
	%var46 = trunc i32 %var45 to i1
	br i1 %var46, label %label_104, label %label_108
label_104:
	%var47 = add i32 0, 5
	ret i32 %var47
	br label %label_112
label_108:
	%var48 = add i32 0, 1
	ret i32 %var48
	br label %label_112
label_112:
	ret i32 0
}

define void @main() {
	%stack_array_ptr = alloca [50 x i32]
	call void @print(i8* getelementptr ([13 x i8], [13 x i8]* @.var49, i32 0, i32 0))
	%var50 = add i32 0, 2
	%var51 = add i32 0, 6
	%var52 = call i32 @powern(i32 %var50, i32 %var51)
	call void @printi(i32 %var52)
	%var53 = add i32 0, 3
	%var54 = add i32 0, 4
	%var55 = call i32 @powern(i32 %var53, i32 %var54)
	call void @printi(i32 %var55)
	%var56 = add i32 0, 7
	%var57 = add i32 0, 3
	%var58 = call i32 @powern(i32 %var56, i32 %var57)
	call void @printi(i32 %var58)
	%var59 = add i32 0, 0
	%var60 = add i32 0, 2
	%var61 = call i32 @powern(i32 %var59, i32 %var60)
	call void @printi(i32 %var61)
	%var62 = add i32 0, 2
	%var63 = add i32 0, 0
	%var64 = call i32 @powern(i32 %var62, i32 %var63)
	call void @printi(i32 %var64)
	call void @print(i8* getelementptr ([10 x i8], [10 x i8]* @.var65, i32 0, i32 0))
	%var66 = add i32 0, 1
	br label %label_141
label_141:
	br label %label_145
label_143:
	br label %label_145
label_145:
	%var67 = phi i32 [1, %label_141], [0, %label_143]
	%var68 = add i32 0, 5
	%var69 = add i32 0, 7
	%var70 = call i32 @ifat(i32 %var66, i32 %var67, i32 %var68, i32 %var69)
	call void @printi(i32 %var70)
	%var71 = add i32 0, 5
	br label %label_153
label_153:
	br label %label_157
label_155:
	br label %label_157
label_157:
	%var72 = phi i32 [1, %label_153], [0, %label_155]
	%var73 = add i32 0, 1
	%var74 = add i32 0, 7
	%var75 = call i32 @ifat(i32 %var71, i32 %var72, i32 %var73, i32 %var74)
	call void @printi(i32 %var75)
	%var76 = add i32 0, 1
	br label %label_165
label_165:
	br label %label_169
label_167:
	br label %label_169
label_169:
	%var77 = phi i32 [1, %label_165], [0, %label_167]
	%var78 = add i32 0, 7
	%var79 = add i32 0, 5
	%var80 = call i32 @ifat(i32 %var76, i32 %var77, i32 %var78, i32 %var79)
	call void @printi(i32 %var80)
	%var81 = add i32 0, 7
	br label %label_177
label_177:
	br label %label_181
label_179:
	br label %label_181
label_181:
	%var82 = phi i32 [1, %label_177], [0, %label_179]
	%var83 = add i32 0, 1
	%var84 = add i32 0, 5
	%var85 = call i32 @ifat(i32 %var81, i32 %var82, i32 %var83, i32 %var84)
	call void @printi(i32 %var85)
	%var86 = add i32 0, 7
	br label %label_189
label_189:
	br label %label_193
label_191:
	br label %label_193
label_193:
	%var87 = phi i32 [1, %label_189], [0, %label_191]
	%var88 = add i32 0, 5
	%var89 = add i32 0, 1
	%var90 = call i32 @ifat(i32 %var86, i32 %var87, i32 %var88, i32 %var89)
	call void @printi(i32 %var90)
	%var91 = add i32 0, 5
	br label %label_201
label_201:
	br label %label_205
label_203:
	br label %label_205
label_205:
	%var92 = phi i32 [1, %label_201], [0, %label_203]
	%var93 = add i32 0, 7
	%var94 = add i32 0, 1
	%var95 = call i32 @ifat(i32 %var91, i32 %var92, i32 %var93, i32 %var94)
	call void @printi(i32 %var95)
	call void @print(i8* getelementptr ([11 x i8], [11 x i8]* @.var96, i32 0, i32 0))
	%var97 = add i32 0, 1
	br label %label_216
label_214:
	br label %label_218
label_216:
	br label %label_218
label_218:
	%var98 = phi i32 [1, %label_214], [0, %label_216]
	%var99 = add i32 0, 5
	%var100 = add i32 0, 7
	%var101 = call i32 @ifat(i32 %var97, i32 %var98, i32 %var99, i32 %var100)
	call void @printi(i32 %var101)
	%var102 = add i32 0, 5
	br label %label_228
label_226:
	br label %label_230
label_228:
	br label %label_230
label_230:
	%var103 = phi i32 [1, %label_226], [0, %label_228]
	%var104 = add i32 0, 1
	%var105 = add i32 0, 7
	%var106 = call i32 @ifat(i32 %var102, i32 %var103, i32 %var104, i32 %var105)
	call void @printi(i32 %var106)
	%var107 = add i32 0, 1
	br label %label_240
label_238:
	br label %label_242
label_240:
	br label %label_242
label_242:
	%var108 = phi i32 [1, %label_238], [0, %label_240]
	%var109 = add i32 0, 7
	%var110 = add i32 0, 5
	%var111 = call i32 @ifat(i32 %var107, i32 %var108, i32 %var109, i32 %var110)
	call void @printi(i32 %var111)
	%var112 = add i32 0, 7
	br label %label_252
label_250:
	br label %label_254
label_252:
	br label %label_254
label_254:
	%var113 = phi i32 [1, %label_250], [0, %label_252]
	%var114 = add i32 0, 1
	%var115 = add i32 0, 5
	%var116 = call i32 @ifat(i32 %var112, i32 %var113, i32 %var114, i32 %var115)
	call void @printi(i32 %var116)
	%var117 = add i32 0, 7
	br label %label_264
label_262:
	br label %label_266
label_264:
	br label %label_266
label_266:
	%var118 = phi i32 [1, %label_262], [0, %label_264]
	%var119 = add i32 0, 5
	%var120 = add i32 0, 1
	%var121 = call i32 @ifat(i32 %var117, i32 %var118, i32 %var119, i32 %var120)
	call void @printi(i32 %var121)
	%var122 = add i32 0, 5
	br label %label_276
label_274:
	br label %label_278
label_276:
	br label %label_278
label_278:
	%var123 = phi i32 [1, %label_274], [0, %label_276]
	%var124 = add i32 0, 7
	%var125 = add i32 0, 1
	%var126 = call i32 @ifat(i32 %var122, i32 %var123, i32 %var124, i32 %var125)
	call void @printi(i32 %var126)
	ret void
}

