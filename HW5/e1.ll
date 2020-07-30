@.div_by_zero_error = constant [23 x i8] c"Error division by zero\00"
@.str_specifier = constant [4 x i8] c"%s\0A\00"
@.int_specifier = constant [4 x i8] c"%d\0A\00"
@.var11 = constant [6 x i8] c"Hello\00"
@.var19 = constant [4 x i8] c"End\00"
@_var24 = internal global i32 0
@_var24_isDefined = internal global i32 1
@_var30 = internal global i32 0
@_var30_isDefined = internal global i32 0

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

define i32 @foo(i32 %arg0) {
	%stack_array_ptr = alloca [50 x i32]
	%var0 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %arg0, i32* %var0
	%var2 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var1 = load i32, i32* %var2
	%var3 = trunc i32 %var1 to i1
	br i1 %var3, label %label_22, label %label_18
label_18:
	%var4 = add i32 0, 0
	ret i32 %var4
	br label %label_22
label_22:
	%var5 = add i32 0, 0
	%var6 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %var5, i32* %var6
	br label %label_27
label_27:
	%var8 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var7 = load i32, i32* %var8
	%var9 = add i32 0, 5
	%var10 = icmp slt i32 %var7, %var9
	br i1 %var10, label %label_33, label %label_45
label_33:
	call void @print(i8* getelementptr ([6 x i8], [6 x i8]* @.var11, i32 0, i32 0))
	%var13 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var12 = load i32, i32* %var13
	call void @printi(i32 %var12)
	%var15 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var14 = load i32, i32* %var15
	%var16 = add i32 0, 1
	%var17 = add i32 %var14, %var16
	%var18 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %var17, i32* %var18
	br label %label_27
label_45:
	call void @print(i8* getelementptr ([4 x i8], [4 x i8]* @.var19, i32 0, i32 0))
	br label %label_48
label_48:
	ret i32 0
}

define i32 @fun(i32 %arg0) {
	%stack_array_ptr = alloca [50 x i32]
	%var20 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %arg0, i32* %var20
	%var22 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var21 = load i32, i32* %var22
	%var23 = trunc i32 %var21 to i1
	br i1 %var23, label %label_59, label %label_67
label_59:
	%var25 = load i32, i32* @_var24
	%var26 = add i32 0, 1
	%var27 = add i32 %var25, %var26
	store i32 %var27, i32* @_var24
	%var28 = load i32, i32* @_var24
	ret i32 %var28
	br label %label_84
label_67:
	%var29 = add i32 0, 1
	%var31 = load i32, i32* @_var30_isDefined
	%var32 = icmp eq i32 %var31, 0
	br i1 %var32, label %label_72, label %label_76
label_72:
	store i32 %var29, i32* @_var30
	store i32 1, i32* @_var30_isDefined
	br label %label_76
label_76:
	%var33 = load i32, i32* @_var30
	%var34 = add i32 0, 1
	%var35 = add i32 %var33, %var34
	store i32 %var35, i32* @_var30
	%var36 = load i32, i32* @_var30
	ret i32 %var36
	br label %label_84
label_84:
	ret i32 0
}

define void @main() {
	%stack_array_ptr = alloca [50 x i32]
	br label %label_90
label_90:
	br label %label_94
label_92:
	br label %label_94
label_94:
	%var37 = phi i32 [1, %label_90], [0, %label_92]
	%var38 = call i32 @fun(i32 %var37)
	call void @printi(i32 %var38)
	br label %label_99
label_99:
	br label %label_103
label_101:
	br label %label_103
label_103:
	%var39 = phi i32 [1, %label_99], [0, %label_101]
	%var40 = call i32 @fun(i32 %var39)
	call void @printi(i32 %var40)
	br label %label_108
label_108:
	br label %label_112
label_110:
	br label %label_112
label_112:
	%var41 = phi i32 [1, %label_108], [0, %label_110]
	%var42 = call i32 @fun(i32 %var41)
	call void @printi(i32 %var42)
	br label %label_117
label_117:
	br label %label_121
label_119:
	br label %label_121
label_121:
	%var43 = phi i32 [1, %label_117], [0, %label_119]
	%var44 = call i32 @fun(i32 %var43)
	call void @printi(i32 %var44)
	br label %label_126
label_126:
	br label %label_130
label_128:
	br label %label_130
label_130:
	%var45 = phi i32 [1, %label_126], [0, %label_128]
	%var46 = call i32 @fun(i32 %var45)
	call void @printi(i32 %var46)
	br label %label_137
label_135:
	br label %label_139
label_137:
	br label %label_139
label_139:
	%var47 = phi i32 [1, %label_135], [0, %label_137]
	%var48 = call i32 @fun(i32 %var47)
	call void @printi(i32 %var48)
	br label %label_146
label_144:
	br label %label_148
label_146:
	br label %label_148
label_148:
	%var49 = phi i32 [1, %label_144], [0, %label_146]
	%var50 = call i32 @fun(i32 %var49)
	call void @printi(i32 %var50)
	ret void
}

