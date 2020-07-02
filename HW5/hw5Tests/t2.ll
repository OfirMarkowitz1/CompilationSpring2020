@.div_by_zero_error = constant [23 x i8] c"Error division by zero\00"
@.str_specifier = constant [4 x i8] c"%s\0A\00"
@.int_specifier = constant [4 x i8] c"%d\0A\00"
@.var40 = constant [2 x i8] c"*\00"
@.var48 = constant [2 x i8] c"*\00"

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

define i32 @fib(i32 %0) {
	%stack_array_ptr = alloca [50 x i32]
	%var0 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %0, i32* %var0
	%var1 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var2 = load i32, i32* %var1
	%var3 = add i32 0, 0
	%var4 = icmp eq i32 %var2, %var3
	br i1 %var4, label %label_26, label %label_20
	br label %label_20
label_20:
	%var5 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var6 = load i32, i32* %var5
	%var7 = add i32 0, 1
	%var8 = icmp eq i32 %var6, %var7
	br i1 %var8, label %label_26, label %label_30
label_26:
	%var9 = add i32 0, 1
	ret i32 %var9
	br label %label_30
label_30:
	%var10 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var11 = load i32, i32* %var10
	%var12 = add i32 0, 1
	%var13 = sub i32 %var11, %var12
	%var14 = trunc i32 %var13 to i8
	%var15 = zext i8 %var14 to i32
	%var16 = call i32 @fib(i32 %var15)
	%var17 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var18 = load i32, i32* %var17
	%var19 = add i32 0, 2
	%var20 = sub i32 %var18, %var19
	%var21 = trunc i32 %var20 to i8
	%var22 = zext i8 %var21 to i32
	%var23 = call i32 @fib(i32 %var22)
	%var24 = add i32 %var16, %var23
	ret i32 %var24
	ret i32 0
}

define void @main() {
	%stack_array_ptr = alloca [50 x i32]
	%var25 = add i32 0, 0
	%var26 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %var25, i32* %var26
	br label %label_55
label_55:
	%var27 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var28 = load i32, i32* %var27
	%var29 = add i32 0, 10
	%var30 = icmp slt i32 %var28, %var29
	br i1 %var30, label %label_61, label %label_86
label_61:
	%var31 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var32 = load i32, i32* %var31
	%var33 = call i32 @fib(i32 %var32)
	call void @printi(i32 %var33)
	%var34 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var35 = load i32, i32* %var34
	%var36 = add i32 0, 1
	%var37 = add i32 %var35, %var36
	%var38 = add i32 0, 10
	%var39 = icmp slt i32 %var37, %var38
	br i1 %var39, label %label_73, label %label_76
label_73:
	call void @print(i8* getelementptr ([2 x i8], [2 x i8]* @.var40, i32 0, i32 0))
	br label %label_76
label_76:
	%var41 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var42 = load i32, i32* %var41
	%var43 = add i32 0, 1
	%var44 = add i32 %var42, %var43
	%var45 = trunc i32 %var44 to i8
	%var46 = zext i8 %var45 to i32
	%var47 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %var46, i32* %var47
	br label %label_55
label_86:
	call void @print(i8* getelementptr ([2 x i8], [2 x i8]* @.var48, i32 0, i32 0))
	ret void
}

