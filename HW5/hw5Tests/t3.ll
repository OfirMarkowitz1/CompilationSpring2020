@.div_by_zero_error = constant [23 x i8] c"Error division by zero\00"
@.str_specifier = constant [4 x i8] c"%s\0A\00"
@.int_specifier = constant [4 x i8] c"%d\0A\00"
@.var19 = constant [21 x i8] c"shouldn't be printed\00"
@.var36 = constant [18 x i8] c"should be printed\00"

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

define void @explainWhileElse(i32 %0) {
	%stack_array_ptr = alloca [50 x i32]
	%var0 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %0, i32* %var0
	%var1 = add i32 0, 0
	%var2 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %var1, i32* %var2
	br label %label_18
label_18:
	%var3 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var4 = load i32, i32* %var3
	%var5 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var6 = load i32, i32* %var5
	%var7 = icmp slt i32 %var4, %var6
	br i1 %var7, label %label_25, label %label_45
label_25:
	%var8 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var9 = load i32, i32* %var8
	call void @printi(i32 %var9)
	%var10 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var11 = load i32, i32* %var10
	%var12 = add i32 0, 1
	%var13 = add i32 %var11, %var12
	%var14 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %var13, i32* %var14
	%var15 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var16 = load i32, i32* %var15
	%var17 = add i32 0, 5
	%var18 = icmp eq i32 %var16, %var17
	br i1 %var18, label %label_40, label %label_43
label_40:
	br label %label_48
	br label %label_43
label_43:
	br label %label_18
label_45:
	call void @print(i8* getelementptr ([21 x i8], [21 x i8]* @.var19, i32 0, i32 0))
	br label %label_48
label_48:
	br label %label_50
label_50:
	%var20 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var21 = load i32, i32* %var20
	%var22 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var23 = load i32, i32* %var22
	%var24 = icmp slt i32 %var21, %var23
	br i1 %var24, label %label_57, label %label_77
label_57:
	%var25 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var26 = load i32, i32* %var25
	call void @printi(i32 %var26)
	%var27 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var28 = load i32, i32* %var27
	%var29 = add i32 0, 1
	%var30 = add i32 %var28, %var29
	%var31 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %var30, i32* %var31
	%var32 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	%var33 = load i32, i32* %var32
	%var34 = add i32 0, 5
	%var35 = icmp eq i32 %var33, %var34
	br i1 %var35, label %label_72, label %label_75
label_72:
	br label %label_80
	br label %label_75
label_75:
	br label %label_50
label_77:
	call void @print(i8* getelementptr ([18 x i8], [18 x i8]* @.var36, i32 0, i32 0))
	br label %label_80
label_80:
	ret void
}

define void @main() {
	%stack_array_ptr = alloca [50 x i32]
	%var37 = add i32 0, 7
	%var38 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %var37, i32* %var38
	%var39 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var40 = load i32, i32* %var39
	call void @explainWhileElse(i32 %var40)
	ret void
}

