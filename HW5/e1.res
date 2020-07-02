@.div_by_zero_error = constant [23 x i8] c"Error division by zero\00"
@.str_specifier = constant [4 x i8] c"%s\0A\00"
@.int_specifier = constant [4 x i8] c"%d\0A\00"
@.var10 = constant [10 x i8] c"Shraga lo\00"
@.var11 = constant [4 x i8] c"AAA\00"

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

define i32 @foo(i32 %0, i32 %1) {
	%stack_array_ptr = alloca [50 x i32]
	%var0 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %0, i32* %var0
	%var1 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 1
	store i32 %1, i32* %var1
	%var2 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var3 = load i32, i32* %var2
	%var4 = trunc i32 %var3 to i1
	br i1 %var4, label %label_20, label %label_30
label_20:
	%var5 = add i32 0, 3
	%var6 = add i32 0, 1
	%var7 = sub i32 %var5, %var6
	%var8 = add i32 0, 8
	%var9 = icmp slt i32 %var7, %var8
	br i1 %var9, label %label_27, label %label_30
label_27:
	call void @print(i8* getelementptr ([10 x i8], [10 x i8]* @.var10, i32 0, i32 0))
	br label %label_33
label_30:
	call void @print(i8* getelementptr ([4 x i8], [4 x i8]* @.var11, i32 0, i32 0))
	br label %label_33
label_33:
	ret i32 0
}

define void @main() {
	%stack_array_ptr = alloca [50 x i32]
	br label %label_41
label_39:
	br label %label_43
label_41:
	br label %label_43
label_43:
	%var12 = phi i32 [1, %label_39], [0, %label_41]
	%var13 = add i32 0, 5
	%var14 = call i32 @foo(i32 %var12, i32 %var13)
	%var15 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %var14, i32* %var15
	ret void
}

